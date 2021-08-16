# 第一周记录

整理第一周在了解sylixOS, Nor Flash存储器的过程中收集到的有用信息。



## 关于sylixOS

sylixOS所有进程共享一个进程空间，切换进程时不换页表



## 关于Flash

FLASH就是写操作是通过AND来完成（只能从1变0，不可从0变1）；FLASH其实就是代表着擦除的意思（一整块全置为1）

如果想在Flash上实现读写操作，通常在NorFlash上我们会选取jffs及jffs2文件系统，在NandFlash上选用yaffs或yaffs2文件系统。

典型的Flash芯片由Package, die, plane, block和page组成：

+ Die也是可以单独执行命令和返回状态的最小单位。
+ Plane: 一个die可以包含几个Plane.
+ Block: 重要的概念，它是擦除操作的最小单位。
+ Page：也很重要，它是写入动作的最小单位。
+ 对于每一片（Plane），都有一个页缓冲区，名字叫page register（应该是Nand才有页缓冲器）

flash损坏的单位是块block；

#### Nor Flash与Nand Flash

Nand Flash文件系统有OOB区（存放ECC校验码以及其它必要标记），Nor Flash则没有OOB区。

Nand Flash的ECC，常见的算法有海明码和BCH，大容量时一般用BCH算法。

NOR FLASH支持Execute On Chip,即程序可以直接在FLASH片内执行；而NAND不可以；

写驱动，是写Nand Flash 控制器的驱动，而不是Nand Flash 芯片的驱动，我们只能写代码控制Nand Flash的控制器，由控制器再去控制Nand Flash设备。

NOR型Flash操作是以“字”为基本单位，而NAND型Flash以“页面”为基本单位；

NAND和NOR的读都可以以字节为单位，但NAND的写以page为单位，而NOR可以随机写每一个字节。NAND和NOR的擦除都以block为单位，但一般NAND的block比NOR的block小；

NOR型Flash的地址线和数据线是分开的，传输效率很高，程序可以在芯片内部执行，NOR型的读速度比NAND稍快一些；NAND型Flash写入速度比NOR型Flash快很多；

NorFlash根据数据传输的位数可以分为并行（Parallel，即地址线和数据线直接和处理器相连）NorFlash和串行（SPI，即通过SPI接口和处理器相连）NorFlash；区别主要就是：1、SPINorFlash每次传输一bit位的数据，parallel连接的NorFlash每次传输多个bit位的数据（有x8和x16bit两种）；2、SPINorFlash比parallel便宜，接口简单点，但速度慢。

SPIflash一般支持3种擦写方式：按sector（扇区，应该差不多就是page）擦写，按block擦写，整片chip擦写；



## SPIFFS

SPIFFS假设运行设备中没有堆Heap，必须使用用户传入的RAM缓冲。因此代码中可能不能用malloc等动态分配内存的操作。（还未看源代码，不确定）

选取最优页尺寸(Optimal Page Size)看似变得非常棘手. 实际也不用烦恼：

- Logical Page Size = Logical Block Size / 256；

  

#### SPIFFS结构

**每个block**的第一个页被设计为了Object查询表（look-up）.

**每个Object**由两类页构成: 索引页(index)和数据页(data).

> The veiled example unveiled would look like this, with some extra pages:
>
> PAGE 0  [  12   12  545   12   12   34   34    4    0    0    0    0 ...]
> PAGE 1  page header:   [obj_id:12  span_ix:0  flags:USED|DATA] ...
> PAGE 2  page header:   [obj_id:12  span_ix:1  flags:USED|DATA] ...
> PAGE 3  page header:   [obj_id:545 span_ix:13 flags:USED|DATA] ...
> PAGE 4  page header:   [obj_id:12  span_ix:2  flags:USED|DATA] ...
> PAGE 5  page header:   [obj_id:12  span_ix:0  flags:USED|INDEX] ...
> PAGE 6  page header:   [obj_id:34  span_ix:0  flags:USED|DATA] ...
> PAGE 7  page header:   [obj_id:34  span_ix:1  flags:USED|DATA] ...
> PAGE 8  page header:   [obj_id:4   span_ix:1  flags:USED|INDEX] ...
> PAGE 9  page header:   [obj_id:23  span_ix:0  flags:DELETED|INDEX] ...
> PAGE 10 page header:   [obj_id:23  span_ix:0  flags:DELETED|DATA] ...
> PAGE 11 page header:   [obj_id:23  span_ix:1  flags:DELETED|DATA] ...
> PAGE 12 page header:   [obj_id:23  span_ix:2  flags:DELETED|DATA] ...
> ...

span_ix是该page在文件的index（该文件的第span_ix页）

事实上有look up页中有两个Object ID号是具有特殊含义的:

obj id 0 (all bits zeroes) - 表示一个已经被删除的页

obj id 0xff.. (all bits ones) - 表示一个未被使用的页

实际上, object id还有另外一个特性: 如果最高位为1表示一个页为object 索引页, 如果最高位为0表示一个页为数据页. 所以上述PAGE 0的完全正确表示如下(*表示最高位为1):

> PAGE 0  [  12   12  545   12  *12   34   34   *4    0    0    0    0 ...]



#### SPIFF术语

> **块(block)**: SPI Flash最小擦除单位
>
> **页**: SPIFFS文件系统的最小存储单元
>
> **索引页**: SPIFFS中用于存储Object头结构的页
>
> **数据页**: SPIFFS中用于存储数据的页
>
> **Object**: SPIFFS文件系统的文件定义, 等同于其他文件系统的文件(File)
>
> **扇区(Sector)**: 某些SPI Flash数据块物理分区的细分单元
>
> **堆(Heap)**: 一个数据区, 典型的用法是通过malloc和free进行申请或释放



## JFFS

对于JFFS系统，主要通过[系统分析文档](https://wenku.baidu.com/view/f4fa09e3524de518964b7d24.html#)进行了解。对于JFFS2系统，则通过JFFS2的[PDF论文](http://sourceware.org/jffs2/jffs2.pdf)了解。

#### JFFS文件系统分析

JFFS最初被用于Linux 2.0中。

JFFS中只有一种node，那就是jffs_raw_inode，文件就是由一个或多个raw_inode组成。

注意Flash设备的文件系统，写操作一般都是将内容写到一个新的node，即使只是修改也是这样。

dirty的node是指该块的数据不全1，但已经没有用；而free的node中的数据全1；

JFFS在垃圾回收时是按照扇区sector为单位，即每次找到了完整的至少一个sector的dirty空间，就可以把该sector擦掉，从而增加一个sector的free空间。

**jffs_file**

> 文件的结构是jffs_file，其中有两个链表range_head和version_head，range_head到range_tail组成的链表代表了文件的数据node，version_head和version_tail则是版本链表（暂时不懂版本链表的作用）

**jffs_control**

> jffs_control就相当于整个文件系统，其中有super_block的指针，也有分配下一个文件所要用的inode_number，同时还有垃圾回收GC所涉及的各种数据结构（例如垃圾回收进程的pid等）。
>
> jffs_control可以直接凭借inode_number返回对应文件的jffs_file结构。

**jffs_fmcontrol**

> jffs_control中有一个jffs_fmcontrol，jffs_fmcontrol是用来管理整个flash的空间使用。
>
> jffs_fmcontrol中记录当前flash设备总大小，已使用大小，在已使用的空间中dirty空间大小等。

**jffs_fm**

> jffs_fm代表了flash上物理的一块数据，一个node对应一个fm？（不确定）
>
> jffs_fm就相当于是一个Page？or一个Block？应该是和一个Block的概念类似（不确定）
>
> 一个fm上可能有多个node，因此一个fm可能对应多个node。

**jffs_node**

> jffs_node就是jffs_raw_inode的内存版本，方便文件系统管理。
>
> jffs_node中有data_offset对应该node的数据在文件中的偏移，fm_offset则是该node在自己所属的fm上的偏移。
>
> jffs_node中有一个version，和文件的inode_number一样，该version也是个32位无符号整数，且第一个node的version=1，之后每因为写入而创建新node时，version就会加1，同文件号一样，version是不会循环使用的，因为flash的寿命有限，等32位的空间都被用完了那么flash肯定早就报废了。

**jffs_file**

> 文件就是由一系列version不同的node组成，其中jffs_file结构上有两个链表range_head和version_head，range_head到range_tail组成的链表代表了文件的数据node，version_head和version_tail则是版本链表（~~暂时不懂版本链表的作用~~）version链表指向的node集合和range链表的相同，只不过range链表是按照数据offset从0到大排序的，而version链表的node则是按version**从小到大**（~~不确定~~）排序的。最老的node就是version_head。
>
> jffs_file中还有parent、children、sibling_prev、sibling_next四个jffs_file的指针，使jffs中所有的文件形成一个树。

JFFS中由于文件都是由raw inode组成，不像EXT2系统中文件有inode和data node之分，因此JFFS**没有硬链接**。

JFFS中的node大小应该是不固定的；

同属于一个inode的node中有一个32位无符号数，代表版本（时间戳而已），越新的node这个数值越大。

inode也有个32位的无符整数，代表文件id，也是nerver reused。

一个携带数据的node可能会被淘汰，如果出现了另一个携带了最新该部分数据的node。那么就认为这块旧数据是obsoleted，这片space就成为dirty space。

对于代表设备或者软连接的inode，只会有一个永不淘汰的node。

删除一个文件会先将这个文件的所有node标记deleted flag，之后再全部变为obsolete nodes。

在挂载JFFS文件系统时，整个Flash介质被扫描，系统将从扫描到的信息中搭建起文件系统目录的层次结构，以及一个完整的从数据在文件系统中的逻辑位置到Flash介质上的物理位置的映射。（相当于把目录结构那些全部读取到内存来了？）这样在查找文件和读取文件就都很快。

**JFFS的垃圾回收机制**：在内核进程或者用户进程中都可能因为写入操作触发垃圾回收机制。如果没有足够的obsolete空间，内核进程会sleep，并返回ENOSPC错误。

> 因为JFFS是按照sector来擦除的，因此回收一片区域也是按照sector地来完成。JFFS的回收机制是这样的，有个head和tail指针分别代表了Flash已经使用的区域的head和tail，每次要回收垃圾时，就将head所在的一个sector给free掉（全置为1），如果head所在的sector不是全部obsolete的nodes，那么还需将这些仍在使用的node给移动到尾部tail。
>
> JFFS的垃圾回收机制同时也要求有一个最小的free space，注意不是dirty sapce而是free space，避免移动node时空间不够，这样就可能会陷入死锁状态。

**JFFS的几个明显问题**：

> 写入只能在文件末尾写；在一个进程拥有文件描述符时删除那个文件会导致内核错误。
>
> JFFS的垃圾回收是线性循环的导致很多非必要的flash擦除操作。
>
> JFFS不可执行压缩操作。
>
> JFFS不支持硬链接。



## JFFS2

JFFS2最初用于Linux 2.4。JFFS2与JFFS的主要区别就是结构不再只是个类似循环的列表，而是由dirty_list、clean_list、free_list这三大由Block组成的链表结构构成。每个Block独立，在垃圾回收时也不再死板地从head取块，而是按照几率从dirty_list或clean_list中取块。且JFFS2在内存中的结构也得到进一步简化，节省了内存占用空间。



#### JFFS2 Header

JFFS2为了改善JFFS只有一种node的缺陷，在每个node前面增加了header来区分，格式如下：

> |幻数屏蔽位|节点类型    |
>
> |节点总长度                  	|
>
> |节点头部CRC校验码   	|

幻数屏蔽位：0x1985 用来标识 JFFS2 文件系统。

节点类型：JFFS2 自身定义了三种节点类型，但是考虑到文件系统可扩展性和兼容性，JFFS2从 ext2 借鉴了经验，**节点类型的最高两位**被用来定义节点的兼容属性，具体来说有下面几种兼容属性：

**JFFS2_FEATURE_INCOMPAT**：当 JFFS2 发现了一个不能识别的节点类型，并且它的兼容属性是JFFS2_FEATURE_INCOMPAT，那么 JFFS2 必须拒绝挂载(mount)文件系统。

**JFFS2_FEATURE_ROCOMPAT**：当 JFFS2 发现了一个不能识别的节点类型，并且它的兼容属性是JFFS2_FEATURE_ROCOMPAT，那么 JFFS2 必须以只读的方式挂载文件系统。

**JFFS2_FEATURE_RWCOMPAT_DELETE**：当 JFFS2 发现了一个不能识别的节点类型，并且它的兼容属性是JFFS2_FEATURE_RWCOMPAT_DELETE，那么在垃圾回收的时候，这个节点可以被删除。

**JFFS2_FEATURE_RWCOMPAT_COPY**：当 JFFS2 发现了一个不能识别的节点类型，并且它的兼容属性是JFFS2_FEATURE_RWCOMPAT_COPY，那么在垃圾回收的时候，这个节点要被拷贝到新的位置。



#### JFFS2节点类型

在上述header的支持下，JFFS2有3种node类型

**JFFS2_NODETYPE_INODE**：INODE类型的node就相当于是之前JFFS中的raw_inode。

**JFFS2_NODETYPE_DIRENT**：DIRENT类型的node就是目录中的一个entry，把文件名和inode联系起来（通过inode号或者inode指针），当我们要从一个目录中删除一个 dentry 时，我们要写一个 DIRENT 节点，节点中的文件名与被删除的 dentry 中的文件名相同，i 节点号置为 0，同时设置一个更高的版本号。

**JFFS2_NODETYPE_CLEANMARKER**：当一个擦写块被擦写完毕后，CLEANMARKER 节点会被写在 NOR flash 的开头，或 NAND flash 的 OOB(Out-Of-Band) 区域来表明这是一个干净，可写的擦写块。在 之前的JFFS中，如果扫描到开头的 1K 都是 0xFF 就认为这个擦写块是干净的。但是在实际的测试中发现，如果在擦写的过程中突然掉电，擦写块上也可能会有大块连续 0xFF，但是这并不表明这个擦写块是干净的。于是我们需要 CLEANMARKER 节点来确切的标识一个干净的擦写块。



#### JFFS2文件系统结构

JFFS2 维护了几个链表来管理擦写块，根据擦写块上的内容，一个擦写块会在不同的链表上。具体来说，当一个擦写块上都是合法(valid)的节点时，它会在 **clean_list** 上；当一个擦写块包含至少一个过时(obsolete)的节点时，它会在 **dirty_list** 上；当一个擦写块被擦写完毕，并被写入 CLEANMARKER 节点node后，它会在 **free_list** 上。



#### JFFS2磨损平衡策略

如前所述，垃圾收集代码使用列表(the lists)为垃圾收集选择扇区。一个非常简单的概率方法用于确定应该选择哪个块，基于jiffies计数器。如果jiffies%100为非零，则从脏列表dirty_list中获取一个块。另外，在一百分之一的情况下，公式为零，则从干净的列表clean_list中选取一个块。通过这种方式，我们优化了垃圾收集以重用已经过时的块，但是随着时间的推移，我们仍然可以有效地在介质上移动数据，以确保没有一个擦除块会在其他块之前被磨损。相当于保证了那些clean_list中的block也会有几率被擦除掉，从而使冷热数据流动，避免出现频繁使用dirty_list中的块。



#### JFFS2内存数据结构

通常情况下，JFFS2 顺序的在擦写块上写入不同的节点，直到一个擦写块被写满。此时 JFFS2 从 free_list 上取下一个擦写块，继续从擦写块的开头开始写入节点。当 free_list 上擦写块的数量逐渐减少到一个预先设定的阀值的时候，垃圾回收就被触发了，为文件系统清理出更多的可用擦写块。为了减少对内存的占用，JFFS2 并没有把 i 节点所有的信息都保留在内存中，而只是把那些在**请求到来时不能很快获得**的信息保留在内存中。具体来说，对于在闪存上的每个 i 节点，在内存里都有一个 struct jffs2_inode_cache 与之对应，这个结构里保存了 i 节点号，指向 i 节点的连接数，以及一个指向属于这个 i 节点的物理节点链表的指针。所有的 struct jffs2_inode_cache 存储在一个哈希表中。闪存上的每个节点在内存中由一个 struct jffs2_raw_node_ref 表示，这个结构里保存了此节点的物理偏移，总长度，以及两个指向 struct jffs2_raw_node_ref 的指针。一个指针指向此节点在物理擦写块上的下一个节点，另一个指针指向属于同一个 i-节点的物理节点链表的下一个节点。

在闪存上的节点的起始偏移都是 4 字节对齐的，所以 struct jffs2_inode_cache 中flash_offset 的最低两位没有被用到。JFFS2 正好利用最低位作为此节点是否过时的标记。最低位是obsolete flag，其次是unused flag。

VFS 调用 iget() 来得到一个 i 节点的信息，当这个 i 节点不在缓存中的时候，VFS 就会调用 JFFS2 的 read_inode() 回调函数来得到 i 节点信息。传给 read_inode() 的参数是 i 节点号，JFFS2 用这个 i 节点号从哈希表中查找相应的 struct jffs2_inode_cache，然后利用属于这个 i 节点的节点链表从闪存上读入节点信息，建立类似于JFFS中的内存信息。相当于JFFS2中内存中保存的文件结构信息——inode信息和node信息都是精简后的（jffs2_inode_cache和jffs2_raw_node_ref），当真的要使用时，会调用相关read函数来填充其余信息，最终填充为类似JFFS中的那样在内存中保存的完整映射信息。



#### JFFS2挂载过程

JFFS2 的挂载过程分为四个阶段：

- JFFS2 扫描闪存介质，检查每个节点 CRC 校验码的合法性，同时分配了 struct jffs2_inode_cache 和 struct jffs2_raw_node_ref

- 扫描每个 i 节点的物理节点链表，标识出过时的物理节点；对每一个合法的 dentry 节点，将相应的 jffs2_inode_cache 中的 nlink 加一。

- 找出 nlink 为 0 的 jffs2_inode_cache，释放相应的节点。

- 释放在扫描过程中使用的临时信息。



#### JFFS2垃圾回收

上面已经写了JFFS的垃圾回收机制会导致每个Block多出很多不必要的擦除操作。

JFFS2 的处理方式是以 99% 的概率从 dirty_list，1% 的概率从 clean_list 上取一个擦写块下来。由此可以看出 JFFS2 的设计思想是偏向于性能，同时兼顾磨损平衡。对**这个块**上每一个**没有过时**的节点执行相同的操作：

1 找出这个节点所属的 i 节点号。

2 调用 iget()，建立这个 i 节点的文件映射表。

3 找出这个节点上没有过时的数据内容，并且如果合法的数据太少，JFFS2 还会合并相邻的节点。

4 将数据读入到缓存里，然后将它拷贝到新的擦写块上。

5 将回收的节点(node)置为过时。

当擦写块上所有的节点都被置为过时，就可以擦写这个擦写块，回收使用它。



#### JFFS2的缺点

**1 挂载时间过长**

JFFS2 的挂载过程需要对闪存从头到尾的扫描，这个过程是很慢的，我们在测试中发现，挂载一个 16M 的闪存有时需要半分钟以上的时间。

**2 磨损平衡的随意性(random nature)**

JFFS2 对磨损平衡是用概率的方法来解决的，这很难保证磨损平衡的确定性。在某些情况下，可能造成对擦写块不必要的擦写操作；在某些情况下，又会引起对磨损平衡调整的不及时。

**3 很差的扩展性**

JFFS2 中有两个地方的处理是 O(N) 的，这使得它的扩展性很差。

首先，挂载时间同闪存的大小，闪存上节点数目成正比。

其次，虽然 JFFS2 尽可能的减少内存的占用，但通过上面对 JFFS2 的介绍我们可以知道实际上它对内存的占用量是同 i 节点数和闪存上的节点数成正比的。

因此在实际应用中，JFFS2 最大能用在 128M 的闪存上。



#### 针对JFFS2的改进

**1 summary patch**

在每个Block(erase block)的末尾添加summary属性，summary就是该块中每个节点(node)的元数据信息，在挂载JFFS2时，系统只需要读取每块末尾的summary信息即可，相当于不需要扫描以及从扫描的信息中建立相应元信息即可。

**2 改进磨损平衡**

在块中设置一个字段来存储该块的擦写次数，当闪存上各个擦写块的擦写次数的差距超过某个预定的阀值，开始进行磨损平衡的调整。将擦写次数小的擦写块上的数据迁移到擦写次数大的擦写块上，相当于将热数据与冷数据换个位置。

**3 擦写块头部补丁**

在上述为每个块存储擦写次数的过程中，发现 JFFS2 中缺少一种灵活的对每个擦写块的信息进行扩展的机制。因此为每个擦写块引入了擦写块头部(header)，这个头部负责纪录每个擦写块的信息(比如说擦写次数)，并且它提供了灵活的扩展机制，将来如果有新的信息需要记录，可以很容易的加入到头部之中。



## Github项目 磨损平衡算法

本项目是针对SPI Nor Flash设备设计的一个磨损平衡中间层，作者在其个人博客中声称用了它的WL能在Flash设备上运行FATFS，[GitHub地址](https://github.com/nickfox-taterli/nor_flash_wl)。目前研究完代码后，~~怀疑这个代码是否真的有效~~。噢，我突然明白了！

#### Flash设备的地址空间

将整个Flash设备的存储空间的**末尾部分**用来存放三块固定的数据，state1，state2，config，其中config的数据一般是不变的，是各种配置信息；而state中则是保存了一些有关磨损平衡算法中间运行状态的参数，因此会在运行过程中发生改变。而state1和state2两个state是完全相同的(冗余存储)，每个state和config中末尾都有个CRC校验码。在开机时，如果两个state的CRC都正确就代表设备正常；如果其中一个不对，就代表设备损坏，把正确的一个state信息复制到损坏的一个state中；如果两个state的CRC都不正确就代表该Flash设备是未经使用的(买来第一次启动)，就需要进行初始化等操作。

每个state的存储区域并不仅仅就是一个struct WL_State_s，在它后面还为每个sector(注意不是page，因为假设的最小可擦除区域的大小依然是一个sector的大小)设置了一个16B的区域，这个16B说实话感觉很浪费也没怎么看懂(可能是作者的失误，把16位想成了16字节)，因为为每个sector分配一定区域确实在其它磨损平衡算法中也有，例如为每个sector分配2bit(类似位图一样的区域)用11表示为空，01表示正被使用，00表示该sector已失效可以被回收，但是在这个项目中为每个sector分配了16B的空间是很大的一笔Flash空间开销。在作者的代码中0xff表示该sector为空，其它表示不为空。（其实这部分代码也很奇怪，明明预留空间时为每个sector预留了16B，在读取每个sector状态时用的又是uint8？）

#### 磨损平衡算法

这个项目的磨损平衡算法说实话在写这段话时我还没怎么看懂，它主要的思想是：

在该项目中假设的是一个可擦除的最小单位是sector，而pos的单位是page，而在该项目中page_size等于sector_size，一般来说sector_size是大于page_size的，因此这是其中一个让我觉得作者代码有问题的地方，因为它的代码中有类似size/sector_size的，而传入的size一般是page_size，如果page_size/sector_size那么结果一般是0(因为page小于sector)，这就会导致作者的代码很多地方出错(但是作者的代码中page_size等于sector_size因此这点暂不会出错)。

在state中有一个pos和move_count两个uint16变量。初始时pos和move_count都是0，read和write操作不会改变pos和move_count。只有当要擦除erase一个page时，会将pos++（如果pos到达了末尾就置为0，相当于是个circle），而每当pos到达一次末尾，move_count就会++，因此作者说move_count代表了pos移动的圈数。第pos个page被作者称为dummy_block，这个东西的作用暂时不知道，只知道每次移动pos前，会将第pos个page的后一页page给移动到第pos个page上来。

在传入一个虚拟地址VA，会通过这个磨损平衡层计算出真正的物理地址PA，计算公式如下：

```c
/* 计算得到物理地址了. */
uint32_t result = (WL_Flash->flash_size - WL_Flash->state.move_count * WL_Flash->cfg.page_size + addr) % WL_Flash->flash_size;
/* 计算出对应的dummy_addr. */
uint32_t dummy_addr = WL_Flash->state.pos * WL_Flash->cfg.page_size;
/* 如果计算结果没占用到dummy_addr储存区域,就不用管.如果占用到了,那要往下一个page挪动. */
if (result >= dummy_addr)
{
    result += WL_Flash->cfg.page_size;
}
return result;
```

可以看到计算物理地址时只与move_count有关，而与pos无关，同时如果result的结果大于dummy_block的地址，即所要操作的page在dummy_block之后的，则该page的物理地址还需额外增加一个page的大小。

#### 真相只有一个

这个磨损平衡的巧妙之处就在于在每次进行擦除时，都将一个page向前循环移动了，其中dummy_block就是用来每次移动一个page的工具，例如用1代表其它page，0代表dummy_block，初始时是这样的0 1 1，经过一次移动后就变成了1 0 1，**感觉这个思想用文字很难表达**。

主要思想就是，每次擦除一个块时就将一个块向前移动一块（第一块的内容会循环移动到最末尾），而在dummy_block之后的就是还未被向前移动的块，因此这些块的地址在进行物理地址转换时还会在后面额外加上一个块的长度来跳过这个dummy_block。

这样举个例子把，用0代表dummy_block，用00代表虚拟地址为0的块，用11代表虚拟地址为1的块，初始时情况是0 00 11，可见虚拟地址为0的块实际上物理地址为1，VA为1的块的PA为2，经过一次移动后变为00 0 11，这个时候VA为0的块的PA就变成了0，而VA为1的块的PA依然为2，再经过一次移动后变为00 11 0，如此循环下去。这也解释上面计算物理地址的代码中的原理。





