# 操作系统

我们选择基于 [TianhuaTao/uCore-SMP](https://github.com/TianhuaTao/uCore-SMP/tree/label-riscv) 和 rCore，加入用户态中断和标签的支持，完成软硬件的协同。

# 操作系统：标签化 uCore-SMP

uCore-SMP 在设计时目标平台为 QEMU ，将其移植到 FPGA 平台主要需要解决如下问题：内存布局、时钟周期、页表项属性和硬盘外设。移植完成后，还需添加对标签系统的支持。代码仓库位于 https://github.com/TianhuaTao/uCore-SMP/tree/label-riscv 。

## 内存布局

QEMU 中内存的起始地址为 0x80000000 ，而 FPGA 平台起始地址为 0x100000000 ，相应的内核启动地址也要调整到 0x100200000 ，这两个值需要在 `memory_layout.h` 和 `kernelld.py` 中进行修改。QEMU 中配置总内存空间为 128MB，而尽管 FPGA 平台有 2GB DRAM，由于内存带宽有限，uCore-SMP 在启动时又会将所有内存地址遍历并初始化。为了减少操作系统的启动等待时间，综合考虑内核和用户程序所需的内存空间，最终将可分配的内存限制在 32MB 。

## 时钟周期

QEMU 中 CPU 运行频率（即 `cycle` 寄存器的增加频率）为 3GHz ，而实时时钟（RTC）频率（即 `time` 寄存器的增加频率）为 12.5MHz ；FPGA 平台上 CPU 频率为 100MHz ，RTC 频率为 10MHz，需要在 `timer.h` 中修改。

## 页表项属性

根据 RISC-V 规范中虚拟内存章节的描述，页表项中有“已访问（A）”和“已修改（D）”两个标志位，对这两个标志位，规范允许两种处理方式：

> - When a virtual page is accessed and the A bit is clear, or is written and the D bit is clear, a page-fault exception is raised.
> - When a virtual page is accessed and the A bit is clear, or is written and the D bit is clear, the implementation sets the corresponding bit(s) in the PTE. The PTE update must be atomic with respect to other accesses to the PTE, and must atomically check that the PTE is valid and grants sufficient permissions. The PTE update must be exact (i.e., not speculative), and observed in program order by the local hart. Furthermore, the PTE update must appear in the global memory order no later than the explicit memory access, or any subsequent explicit memory access to that virtual page by the local hart. The ordering on loads and stores provided by FENCE instructions and the acquire/release bits on atomic instructions also orders the PTE updates associated with those loads and stores as observed by remote harts.

即直接触发页错误异常，或由硬件实现直接对页表项置位。FPGA 平台实现为前一种，而 QEMU 为后一种。这两个标志位主要应用于虚拟存储系统中的页面置换，而 uCore-SMP 中对此并无支持，简单起见，可修改 `virtual.c` ，在分配新的页表项并映射物理页时，直接将 A 和 D 置位。

## 硬盘外设

QEMU 中通过 virtio 提供了虚拟块存储设备，FPGA 平台中没有。在此情况下，为了仍然能够使用文件系统，陶天骅同学提供了虚拟磁盘接口和 ramdisk 功能，通过宏开关在编译期进行切换。系统初始化时， ramdisk 将在内存中开辟一块区域，并将其按照 nfs 格式进行初始化。

## 标签系统

在内核初始化时，将标签系统控制平面基址以恒等映射挂载到 0x20000 地址处，权限为内核可读写。读取和写入参数的方式为基址+偏移，偏移量单位为 32bit ，即需要将地址强制转换为指向 32 位无符号整数的指针类型。具体各参数的偏移量和写入方法见 `dsid.c/h` 。

目前该系统支持至多 8 个不同的进程标签。进程标签存储于进程控制块中，在程序上下文切换时，内核将标签写入相应的 CSR 。内核的进程标签硬编码为 0 。

内核提供三个和标签相关的系统调用：

```c
int set_dsid(int pid, uint32 dsid);
int set_dsid_param(uint32 dsid, uint32 freq, uint32 size, uint32 inc, uint32 mask);
uint32 get_l2_traffic(uint32 dsid);
```

- `set_dsid` 将 `pid` 对应进程的标签设为 `dsid` ，若进程不存在，则返回 -1 ，否则返回 0 ；
- `set_dsid_param` 设置 `dsid` 标签相应的令牌桶参数和缓存掩码，参数含义见[“硬件”章节](#硬件)。只有非零的参数会被写入控制平面。
- `get_l2_traffic` 读取 `dsid` 标签对应的 L1 和 L2 缓存之间的数据流量，单位为 64bit 。

## 与内核共同调试和 debug

标签系统开发过程中，主要查出内核中以下几个缺陷。

### 调度器初值

uCore-SMP 使用 stride 算法进行调度，在初始实现中，fork 产生的子进程的 stride 会置为 0 。在使用标签系统监测负载程序的缓存性能时，设置父进程每隔一秒输出一次信息。第一批负载表现正常，但启动第二波负载时，会出现直到负载运行结束父进程才会输出的情况。分析后发现，由于子进程 stride 为 0 ，而运行第二波负载时，父进程已有一定的 stride ，此时调度器会持续优先调度子进程，导致父进程陷入“饥饿”。解决方法为，将子进程的 stride 置为和父进程相同。

### 子进程回收内存泄露

初始实现中，若父进程没有使用 wait/waitpid 等待子进程结束，则子进程占用的内存和文件等资源在进程结束后不会被回收，当大量 fork 时会耗尽系统内存。为此，陶天骅同学在内核中加入了 reparent 机制，当进程退出时会检查其子进程，若子进程已退出则回收，否则将其父进程置为 NULL ；若退出的进程父进程为 NULL ，则立即回收自身占用的资源，这样解决了这一内存泄露问题。

### syscall 中的悬垂指针

系统调用进入内核中 syscall 函数时，会保存进程的 trapframe 地址，在具体系统调用执行完成后，使用该地址中的 a0 寄存器保存系统调用的返回值。在运行标签负载时发现，当用户程序运行到 0x1070 地址（用户态下的虚拟地址）时会触发非法指令异常，stval 寄存器显示相应的指令值为 0 ，但查看相应程序的反汇编发现此处并非 0 。起初怀疑是固件中非对齐 load/store 模拟存在问题，导致加载用户程序时出现数据异常；尽管发现固件确实存在问题，但解决该问题后依然会触发非法指令异常。

后来使用内核中 pushtrace 工具在各处记录 0x1070 对应内存地址的数值发现，当执行 execv 系统调用时，会回收原有的 trapframe 内存页，该页可能被后续用于加载用户程序的代码段，程序分配的新 trapframe 的 a0 和 a1 寄存器会分别用于存储 `argc` 和 `argv` ；但 execv 执行完成返回 syscall 函数中时（其返回值总为 0 ），仍会使用旧的 trapframe 地址，而 a0 寄存器对应变量在 trapframe 中相应的偏移量即为 0x70 ，向其中写入 0 会破坏用户程序代码，导致错误。解决方法为，当系统调用为 execv 时，在 syscall 函数中不写入 `trapframe->a0` 。
