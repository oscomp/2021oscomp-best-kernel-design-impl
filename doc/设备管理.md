### 概述

本文档主要为rCore-Tutorial-v3的学习笔记以及相关概念和资料的整合。

首先是CPU访问外设的方式，在RISC-V环境中，把外设相关的控制寄存器映射为某特定的内存区域（即MMIO映射方式），然后CPU通过读写这些特殊区域来访问外设（即PIO访问方式）。外设可以通过DMA来读写主机内存中的数据，并可通过中断来通知CPU。外设并不直接连接CPU，这就需要了解RISC-V中的平台级中断控制器（Platform-Level Interrupt Controller，PLIC），它管理并收集各种外设中断信息，并传递给CPU。



### I/O设备抽象

如果我们不考虑具体的设备，而是站在一个高度抽象的角度来让软件管理设备，那么我们就不用太关注设备的内部结构，而重点考虑设备的接口如何进行简化。其实一个简化的抽象设备接口需要包括三部分：状态、命令、数据。

引入中断机制后，这个简化的抽象设备接口需要包括四部分：状态、命令、数据、中断。CPU与设备间的I/O接口的交互协议如下所示：

```rust
DATA =  data;          // 把数据传给设备
COMMAND = command;     // 发命令给设备
do_otherwork();        // 做其它事情
...                    // I/O设备完成I/O操作，并产生中断
...                    // CPU执行被打断以响应中断
trap_handler();        // 执行中断处理例程中的相关I/O中断处理
restore_do_otherwork();// 恢复CPU之前被打断的执行
...                    // 可继续进行I/O操作
```

站在软件的角度来看，为提高一大块数据传输效率引入的DMA机制并没有改变抽象设备接口的四个部分。仅仅是上面协议伪码中的 `data` 变成了 `data block` 。这样传输单个数据产生的中断频度会大大降低，从而进一步提高CPU利用率和系统效率。

#### 基于文件的I/O设备抽象

计算机专家为此进行了诸多的探索，希望能给I/O设备提供一个统一的抽象。首先是把本来专门针对存储类I/O设备的文件进行扩展，认为所有的I/O设备都是文件，这就是传统UNIX中常见的设备文件。所有的I/O设备按照文件的方式进行处理。

由于各种设备的功能繁多，仅仅靠 `read/write` 这样的方式很难有效地与设备交互。于是UNIX的后续设计者提出了一个非常特别的系统调用 `ioctl`。

 `ioctl` 系统调用很灵活，但它的问题是太灵活了，请求码的定义无规律可循，文件的接口太面向用户应用，并没有挖掘出操作系统在进行I/O设备处理过程中的共性特征。所以文件这个抽象还不足覆盖到操作系统对设备进行管理的整个执行过程中。

#### 基于流的I/O设备抽象

流是用户进程和设备或伪设备之间的全双工连接。它由几个线性连接的处理模块（module）组成，类似于一个shell程序中的管道（pipe），只是数据双向流动。流中的模块通过向邻居模块传递消息来进行通信。除了一些用于流量控制的常规变量，模块不需要访问其邻居模块的其他数据。此外，一个模块只为每个邻居提供一个入口点，即一个接受消息的例程。

#### 基于virtio的I/O设备抽象

IBM资深工程师 Rusty Russell 在开发Lguest（Linux 内核中的的一个hypervisor（一种高效的虚拟计算机的系统软件）)时，深感写模拟计算机中的高效虚拟I/O设备的困难，且编写I/O设备的驱动程序繁杂且很难形成一种统一的表示。于是他经过仔细琢磨，提出了一组通用I/O设备的抽象 – virtio规范。虚拟机（VMM或Hypervisor）提供virtio设备的实现，virtio设备有着统一的virtio接口，guest操作系统只要能够实现这些通用的接口，就可以管理和控制各种virtio设备。而虚拟机与guest操作系统的virtio设备驱动程序间的通道是基于共享内存的异步访问方式来实现的，效率很高。虚拟机会进一步把相关的virtio设备的I/O操作转换成物理机上的物理外设的I/O操作。这就完成了整个I/O处理过程。

由于virtio设备的设计，使得虚拟机不用模拟真实的外设，从而可以设计一种统一和高效的I/O操作规范来让guest操作系统处理各种I/O操作。这种I/O操作规范其实就形成了基于virtio的I/O设备抽象，并逐渐形成了事实的上的虚拟I/O设备的标准。

#### Linux设备文件系统

[(13条消息) Linux 文件系统与设备文件系统 （一）—— udev 设备文件系统_zjy900507的博客-CSDN博客](https://blog.csdn.net/zjy900507/article/details/79567858)

[linux内核sysfs详解【转】 - sky-heaven - 博客园 (cnblogs.com)](https://www.cnblogs.com/sky-heaven/p/8145466.html)

### IO执行模型

五类：

- blocking IO
- nonblocking IO
- IO multiplexing
- signal driven IO
- asynchronous IO

当一个用户进程发出一个 `read` I/O系统调用时，主要经历两个阶段：

1. 等待数据准备好 (Waiting for the data to be ready)
2. 把数据从内核拷贝到用户进程中(Copying the data from the kernel to the process)



上述五种IO模型在这两个阶段有不同的处理方式。需要注意，阻塞与非阻塞关注的是进程的执行状态：

- 阻塞：进程执行系统调用后会被阻塞
- 非阻塞：进程执行系统调用后不会被阻塞

同步和异步关注的是消息通信机制：

- 同步：用户进程与操作系统（设备驱动）之间的操作是经过双方协调的，步调一致的
- 异步：用户进程与操作系统（设备驱动）之间并不需要协调，都可以随意进行各自的操作



从上述分析可以得知，阻塞和非阻塞的区别在于内核数据还没准备好时，用户进程是否会阻塞（一阶段是否阻塞）；同步与异步的区别在于当数据从内核copy到用户空间时，用户进程是否会阻塞/参与（二阶段是否阻塞）。

所以前述的阻塞IO（blocking IO），非阻塞IO（non-blocking IO），多路复用IO（IO multiplexing），信号驱动IO都属于同步IO（synchronous IO）。这四种模型都有一个共同点：在二阶段阻塞/参与，也就是在真正IO操作 `read` 的时候需要用户进程参与，因此以上四种模型均称为同步IO模型。

##### 

### 驱动

功能：

1. 设备初始化，即完成对设备的初始配置，分配I/O操作所需的内存，设置好中断处理例程
2. 如果设备会产生中断，需要有处理这个设备中断的中断处理例程（Interrupt Handler）
3. 根据操作系统上层模块（如文件系统）的要求（如读磁盘数据），给I/O设备发出命令
4. 与操作系统上层模块进行交互，完成上层模块的要求（如上传读出的磁盘数据）

#### 设备树

一般由bootloader（SBI）传入：它来完成对于包括物理内存在内的各外设的探测，将探测结果以 **设备树二进制对象（DTB，Device Tree Blob）** 的格式保存在物理内存中的某个地方。

把放置DTB的物理地址将放在 `a1` 寄存器中，而将会把 HART ID （**HART，Hardware Thread，硬件线程，可以理解为执行的 CPU 核**）放在 `a0` 寄存器上。

Rust已经有相关的库来处理

#### Linux设备号

[(13条消息) Linux设备号的构成和分配内核机制_LieBao-CSDN博客_linux设备号分配方法](https://blog.csdn.net/liebao_han/article/details/78931683)

[基于Linux的设备号的构成、分配、释放的详细解析-电子发烧友网 (elecfans.com)](http://www.elecfans.com/d/621900.html)



#### 平台级中断控制器

如果要让操作系统处理【外设中断】，就需要对中断控制器进行初始化设置。在RISC-V中，与外设连接的I/O控制器的一个重要组成是【平台级中断控制器】（Platform-Level Interrupt Controller，PLIC），它汇聚了各种外设的中断信号，并连接到CPU的外部中断引脚上。通过RISC-V的【 `mie` 】寄存器中的 `meie` 位，可以控制这个引脚【是否接收外部中断信号】。当然，通过RISC-V中M Mode的中断委托机制，【也可以在RISC-V的S Mode下】，通过 `sie` 寄存器中的 `seie` 位，对中断信号是否接收进行控制。

CPU可以通过MMIO方式来对PLIC进行管理，下面是一下与PLIC相关的寄存器：

```
寄存器         地址      功能描述
Priority   0x0c00_0000       设置特定中断源的优先级
Pending        0x0c00_1000   包含已触发（正在处理）的中断列表
Enable         0x0c00_2000   启用/禁用某些中断源
Threshold  0x0c20_0000       设置中断能够触发的阈值
Claim      0x0c20_0004       按优先级顺序返回下一个中断
Complete   0x0c20_0004       写操作表示完成对特定中断的处理
```

操作系统如要响应外设的中断，需要做两方面的初始化工作。首先是完成第三章讲解的中断初始化过程，并需要把 `sie` 寄存器中的 `seie` 位设置为1，让CPU能够接收通过PLIC传来的外部设备中断信号。然后还需要通过MMIO方式对PLIC的寄存器进行初始设置，才能让外设产生的中断传到CPU处。其主要操作包括：

- 设置外设中断的优先级
- 设置外设中断的阈值，优先级小于等于阈值的中断会被屏蔽
- 激活外设中断，即把 `Enable` 寄存器的外设中断编号为索引的位设置为1



### VirtIO

#### **virtqueue虚拟队列**

在virtio设备上进行【批量数据传输的机制】被称为虚拟队列（virtqueue），virtio设备的虚拟队列（virtqueue）可以由各种数据结构（如数组、环形队列等）来具体实现。每个【virtio设备可以拥有零个或多个virtqueue】，每个virtqueue占用2个或者更多个4K的物理页。 virtqueue有Split Virtqueues（向下兼容的一种组织方式）和Packed Virtqueues（更高效的一种组织方式）两种模式。在Split virtqueues模式下，virtqueue被分成若干个部分， 每个部分都是前端驱动或者后端设备单向可写。 每个virtqueue都有一个16bit的 `Queue Size` 参数，表示【队列的总长度】。virtqueue由三部分地址连续的物理内存块组成：

- 描述符表 Descriptor Table：IO传输【请求信息】（描述符）的数组，每个描述符都是对某buffer 的address/length描述。buffer中包含IO传输请求的【命令/数据/返回结果】等。
- 可用环 Available Ring：记录了描述符表中【被驱动程序更新的描述符】的索引集合，需要后端的【设备进行读取并完成相关I/O操作】；
- 已用环 Used Ring：记录了描述符表中【被设备更新的描述符的索引集合】，需要前端的驱动程序进行读取并完成对I/O操作【结果的响应】；

**描述符表**

描述符表用来指向I/O传输请求的信息，即是驱动程序与设备进行交互的缓冲区（buffer），由 `Queue Size` 个Descriptor（描述符）组成。描述符中包括【buffer的物理地址】 – addr字段，【buffer的长度】 – len字段，可以链接到【 `next Descriptor`】 的next指针（用于把多个描述符链接成描述符链）。buffer所在物理地址空间需要操作系统或运行时在初始化时分配好，并在后续由驱动程序在其中填写IO传输相关的命令/数据，或者是设备返回I/O操作的结果。多个描述符（I/O操作命令，I/O操作数据块，I/O操作的返回结果）形成的【描述符链】可以表示一个【完整的I/O操作请求】。

**可用环**

可用环中的条目是一个【描述符链】的【头部描述符】的【索引值】，并有头尾指针表示其可用条目范围，形成一个环形队列。它【仅由驱动程序写入】，并【由设备读出】。virtio设备通过【读取可用环中的条目】可【获取】驱动程序发出的I/O操作请求对应的【描述符链】，然后就可以进行进一步的处理了。【描述符】指向的缓冲区具有可读写属性，可读的缓冲区用于Driver发送数据，可写的缓冲区用于接收数据。比如对于（I/O操作命令 – “读磁盘块”，I/O操作数据块 – “数据缓冲区”，I/O操作的返回结果 –“结果缓冲区”）这三个描述符形成的链，可通过读取第一个描述符【指向的缓冲区】【了解到是“读磁盘块”操作】，这样就可把【磁盘块数据】通过DMA操作放到第二个描述符指向的“数据缓冲区”中，然后把【“OK”】写入到第三个描述符指向的“结果缓冲区”中。

**已用环**

已用环中的条目也一个是描述符链的头部描述符的索引值，并有头尾指针表示其已用条目的范围，形成一个环形队列。这个描述符是【Device完成相应I/O处理后】，将可用环中的对应“I/O操作的返回结果”的【描述符索引值】移入到已用环中，并通过轮询或中断机制来【通知virtio驱动程序】，并让virtio驱动程序读取已用环中的这个描述符，从而进行对设备完成I/O操作后的进一步处理。



### 设计

我们在实现ioctl时，计划同时完成IO设备控制的框架。

#### 设备管理

我们使用RustSBI作为Supervisor二进制接口，同时用于bootloader，但是其并未提供k210的设备树，因此我们暂不考虑设备树的实现。在这种情况下，设备文件系统的可拓展性收到影响，我们只能使用需要的设备。

FAT32文件系统并没有预留特殊的字段描述设备文件，因此不支持设备文件。目前再写一个根文件系统时间会很紧，因此我们我们使用了较为妥协的办法，将一些设备文件存储于内存，在文件系统启动时通过lazy_static的方式建立。这些设备文件主要实现了一些简单的读写接口，设备驱动则直接使用了k210-pac库以及Virtio-drivers库，因此相较之前不会占用更多内存。此外，我们为File Trait增加了一个ioctrl接口以辅助系统调用，普通的文件不必实现该接口。
定义如下：
```rust
    fn ioctl(&self, cmd: u32, arg: usize)-> isize {0}
```
该接口几乎保留了全部的syscall参数

当用户期望打开一个设备文件时，内核会顺序遍历所有的设备以匹配。对于目前设备比较少的情况，这效率不低。如果以后需要扩展更多设备，只需在内核初始化的时候构建一个树结构以便于管理和搜索。

#### ioctl

ioctl的格式为：

```C
int ioctrl(int fd, int cmd, ...)
//“...”表示参数arg，智能有一个，可以是整数，也可以是指针
```

如果arg作为指针传入，因为指针为用户空间地址，处理跨页的情况会比较复杂，我们考虑直接使用UserBuffer。

对于指针指向的结构体，驱动程序有两种处理情况：读或写。

如果是读，那么理想情况需要0或1次copy：

0的情况是：驱动直接使用结构体成员，例如执行条件语句等；

1的情况是：驱动要将内容存储在局部变量中；

在使用UserBuffer的情况下，前者恐怕很难实现。

为此我们还是进行copy，毕竟代价不算很大，几十个字节。


实际上，内核并不知道驱动程序需要读还是写结构体，因此读写要有统一的操作流程。此外，内核也不知道arg究竟是参数还是指针。因此理想情况下，内核应不必考虑ioctl的具体内容，只需托管给目标设备的驱动程序。同时，驱动程序也不应该过多地考虑到分页和用户地址空间的问题，可以较为直接的读写需要的内存。换句话说，内核只应作为一个数据传送的桥梁，用户的控制和驱动的具体实现都是透明的，内核和驱动之间是完全解耦的。以上就是我们想要实现的目标。

对于结构体，最简单的办法是将UserBuffer传递给驱动，调用ub的read或write接口，驱动可以得到字符串，进而进一步进行类型转换。在这种设计下，读写操作均只需1次copy，但缺点也是显然的：兼容性很差。UserBuffer是一个独有的设计，很难做到让驱动都使用这个类。同时，正如上文提及，我们不知道参数是不是一个结构体指针，只有驱动程序知道。

参考了Linux下驱动的实现，其使用了copy_from/to_user()。这种实现下，驱动使用内核的接口，隐蔽了内核处理的具体过程以及用户的空间，具有比较高的可行。同时，每次读或者写都是one copy，性能上也可以接受。目前的实现上，我们调用了UserBuffer的接口，buffer创建过程是有一定代价的，虽然copy次数有限，但需要对地址范围扫描。后期考虑对过程进行优化，直接操作用户空间。需要注意的是，一些对象在使用Mutex保护的时候，无法直接获取其地址，因此需要多一次copy。

这两个接口定义如下:
```rust
    pub fn copy_from_user<T>(dst: *mut T, src: usize, size: usize);
    pub fn copy_to_user<T>(dst: usize, src: *const T, size: usize);
```
接口位于page_table.rs，使用泛型实现，以支持不同类型的数据对象。

##### 目前出现的命令

TIOCGWINSZ

TIOCGPGRP

TIOCSPGRP

TCGETS

SNDCTL_TMR_START or TCSETS

一些命令的解释：

[(13条消息) Linux设备驱动开发详解【六】_ 字符设备驱动_santirenpc的博客-CSDN博客](https://blog.csdn.net/santirenpc/article/details/102913911)

[转： 控制终端与前台进程组-millerix-ChinaUnix博客](http://blog.chinaunix.net/uid-30485355-id-5301998.html)



busybox中ioctl目前涉及的控制主要是对于tty，tty结构体定义如下：

```rust
pub struct TtyINode {
    /// foreground process group
    foreground_pgid: RwLock<i32>,
    buf: Mutex<VecDeque<u8>>,
    eventbus: Mutex<EventBus>,
    winsize: RwLock<Winsize>,
    termios: RwLock<Termios>,
}
```

这是rCore的一个实现，当然，这个实现的一部分是假的。我们也直接参考了这种实现。

实际上，不管是k210还是qemu，我们都使用串口通信，这是无法实现一些tty所具备的特性的。

补充：

[(13条消息) linux 系统tty、pty和pts 的概念及区别_Lucky_mn的专栏-CSDN博客_linux pts](https://blog.csdn.net/mn_forever/article/details/12861411)

[(13条消息) pts/0代表什么意思？_weixin_34072857的博客-CSDN博客](https://blog.csdn.net/weixin_34072857/article/details/86259660?utm_medium=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-3.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-3.control)



