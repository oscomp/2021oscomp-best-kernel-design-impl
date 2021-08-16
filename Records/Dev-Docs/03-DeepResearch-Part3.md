# F2 FS文件系统

## 简介

[F2FS简介](https://www.kernel.org/doc/html/v5.7/filesystems/f2fs.html#background-and-design-issues)

[F2FS源码分析](https://blog.csdn.net/u011649400/article/details/94588840)

F2FS是专门运用于Nand flash的文件系统，也是基于LFS架构的。

## 名词解释


卷(volume): 格式化以后由文件系统使用的分区或分区集合，是逻辑上的划分。

分区(partition):物理磁盘的一部分，其作用相当于一个物理分隔单元，通常分为主分区和拓展分区。

FTL( Flash Translation Layers ): FTL存在于文件系统和物理介质（闪存）之间 ，SSD采用[软件](http://www.liangchanba.com/misc.php?mod=tag&id=241)的方式把闪存的操作虚拟成磁盘的独立扇区操作，这就是FTL 。负责翻译逻辑块地址到物理块地址的单元。

## 设计思路

- F2FS在磁盘层(on-disk layout)有三种分配单元：segment，section，zone。F2FS从大量独立的zone中以segment为单位分配空间。在清理的时候以section为单位。这些单元和下层的FTL操作单元是对齐的。

- 针对LFS的wandering tree问题，F2FS想出了一个全新的索引结构，node address table。

- Multi-head logging。F2FS在分配block时，用到特殊的算法区分冷数据和热数据。F2FS会运行多道日志段(log segment)。在添加数据时，它会根据这些数据的预期更新频率添加到不同的日志段中。由于存储设备采用了并行性，因此多日志带来的额外开销（和单日志相比）是微不足道的。

  > log segment就是保存文件的区域

- fsync函数的优化（Linux上用于立刻将写入的数据更新到磁盘上）。F2FS的做法是最小化元数据的写操作以及用一个前滚恢复(roll-forward recovery)机制恢复要同步的数据。

  ## 磁盘层数据结构

  ​    F2FS的卷被分成了固定大小的segment，这也是管理的基本单元，决定初始文件系统的元数据布局。section由连续的segment组成，而zone又由连续的section组成。

  ### F2FS将卷分为六个部分

  1.超级块(Super Block)

  2.检查点(CheckPoint)：保存文件系统持续状态，为掉电恢复做准备。CP区保存两个点，一个用于最后一个稳定版本，另一个用于中间（过时）版本。

  3.分段信息表( Segment Information Table ):保存每个分段有多少个有效块和bitmap。后面垃圾回收要用。

  4.节点地址表( Node Address Table ):保存到每个节点的地址

  5.段总结区域( Segment Summary Area): 这个区域主要保存了jounal(SIT/NAT临时的修改信息)以及summary(记录了逻辑地址和物理地址关系的结构，主要用与GC)。SSA区域在内存中没有专门的数据结构。 

  6.主区域(Main Area):存储文件的区域

  > F2FS中每个block大小为4KB，分为node块和data块两种。node块保存inode或data块索引，data块保存目录和数据本身。一个section里只保存一种块。

  ![F2FS 存储结构](./image/F2FS存储结构.png)



### F2FS文件结构

![F2FS 存储结构](./image/文件结构.png)



论文说当需要更新文件数据，LFS可能会需要修改多个pointer block，但是F2FS只用修改direct block。所以F2FS能很好地解决wandering tree问题。

> 为什么不用修改indirect inode？
>
>  **直接dnode的每一个表项指向的是一个DATA block的地址，而间接dnode的每一个表项指针指向的NAT表中的一个表项**。 
>
>  上面说到，直接dnode的表项指向的是DATA block的地址，所以DATA page变脏了之后，DATA block就要变更了，所以被殃及的直接dnode当然也脏喽。但是注意，此时火势并不会蔓延到间接dnode上，因为间接dnode表项指针指的并不是直接dnode的block地址，而是NAT表中的一个表项，所以NAT就像防火枪一样防止了tree任意滋生：你只要把NAT中相应的block_addr域给更新掉就可以了，我间接dnode的指针还是指向这个NAT表项。 下面是NAT中entry的数据结构。

```c
structf2fs_nat_entry {
  __u8 version;    /* latest version of cached nat entry */
  __le32 ino;   /* inode number */
  __le32 block_addr; /* block address */
} __packed;
```

另外，F2FS可以让用户在inode添加内联数据和拓展变量。

### F2FS目录结构

每个目录entry包括hash/ino/len/type四个成员，占用11bytes。每个entry代表一个子目录、符号链接或者普通文件。 

```
- hash          hash value of the file name
- ino           inode number
- len           the length of file name
- type          file type such as directory, symlink, etc
```

[F2FS目录哈希索引详细解释](http://blog.chinaunix.net/uid-28989651-id-3909400.html)

![F2FS哈希桶](.\image\哈希桶.png)

F2FS的目录哈希查找大概就是先计算文件名的哈希值，然后再逐层去查找对应的桶(bucket)，然后比较文件名来避免冲突。F2FS在每一层仅需扫描一个桶下的block。



### Multi-head logging特性

F2FS用温度来衡量block的更新频率，它一共有三种温度，hot,warm,cold。

![F2FS哈希桶](.\image\温度评判标准.png)

- **HOT NODE区域**：给目录的direct node使用，因为打开目录、读取目录是最频繁的操作。
- **WARM NODE区域**：给普通文件的direct node使用
- **COLD NODE区域**：给indirect node使用，一般而言只有较大的文件才会使用到这个log区域。
- **HOT DATA区域**：给目录的数据使用，目录数据记录了当前目录有多少个子文件、子文件夹。
- **WARM DATA区域**：给普通文件的数据使用，常规的fwrite/write函数写入的数据都是在这里分配。
- **COLD DATA区域**：给不频繁修改的数据使用，如多媒体文件(多为只读文件)，或者用户指定的只读文件，如GC产生写的数据(gc会挑热度最低的数据)。

文章提到FTL有三种算法 (block-associative, set-associative and fully-associative)。好家伙，分别是直接相联、组相联和全相联。FTL里有cache，用的就是组相联。

### 清理回收 

F2FS有前台回收和后台回收两种方式。

 前台GC一般在系统空间紧张的情况下运行，目的是尽快回收空间; 而后台GC则是在系统空闲的情况下进行，目的是在不影响用户体验的情况回收一定的空间。前台GC一般情况下是在checkpoint或者写流程的时候触发，因为F2FS能够感知空间的使用率，如果空间不够了会常触发前台GC加快回收空间，这意味着文件系统空间不足的时候，性能可能会下降。后台GC则是被一个线程间隔一段时间进行触发。而接下来我们主要讨论的都是后台GC。 

#### 清理过程大概分为三步：

1.选择受害者（也就是要回收的块）。

- 前台GC用贪心算法，找空余最大的section。

- 后台GC用成本效益算法( cost-benefit policy )，综合考虑section的利用率和年龄（修改时间至今）。sction的年龄是其所包含的segment的年龄平均值，segment年龄记录在SIT中。

2.有效块识别和迁移

- 对于前台GC，F2FS选择了一个section回收后，它先在SIT中找到所有该section下的有效块（保存在bitmap中），然后到SSA中去找含有这些有效块索引的父亲 node block，最后迁移这些块。

- 对于后台GC，不直接使用I/O来迁移有效块。它用一种懒惰迁移(lazy migration)的方法 。先把要迁移的块加载到cache里，后再由内核迁移到其他地方。

3.后清理处理

受害section在迁移完内部的有效块之后，就会变成pre-free section，在生成一个checkpoint之后才能成为free section并被分配。这是防止在掉电时前面的checkpoint引用的数据会丢失。

### 自适应日志

传统LFS写数据有两种方法，normal logging和 threaded logging。F2FS两者兼用，根据文件系统状态来进行切换。还可以设置个阈值k，clean section数量大过k就用normal logging，小过k就用thread logging。

### 检查点和恢复机制

 当F2FS需要通过 `fsync` 或 `umount` 等命令对系统进行同步的时候，F2FS会触发一次Checkpoint机制 。F2FS触发检查点机制时，要做以下步骤

> 1.页缓存的脏node和dentry block会刷写回到磁盘 
>
> 2.暂停常规的写操作（包括系统调用create、mkdir等）。
>
> 3.文件系统元数据（NAT、SIT、SSA）保存到专门的区域。
>
> 4.将checkpoint pack写入专门保存的CP区。

checkpoint pack包括以下信息：

> 1.头部和尾部保存版本号
>
> 2.NAT和SIT的bitmap
>
> 3.NAT和SIT近期修改的entry
>
> 4.Summary blocks of active segments 
>
> 5.孤儿节点。inode在关闭之前被删除的话就会成为孤儿节点

#### 后滚恢复

当遇到突然失去电源等情况，F2FS会回滚到最近的Checkpoint点中。为了保持一个稳定可用的Checkpoint(防止Chcekpoint在创建或者更新的时候宕机)，F2FS维护了两个Checkpoint。如果一个Checkpoint在header和footer包含可以区分的内容，那么F2FS就会认为是可用的，可以使用这个Checkpoint，否则就会被丢弃掉。

与之相似，每一个Checkpoint都包含一份NAT bitmap和SIT bitmap，用于管理NAT Blocks和SIT Blocks。F2FS会选择其中一组Checkpoint数据进行更新，然后标记为最新。

如果有少量的NAT和SIT entries被频繁更新，可能会导致频繁大量的IO读写。因此F2FS在Checkpoint使用了SIT journals和NAT journals去缓存一些频繁更改。通过这种方式可以减少CP和读写的Overheads。

在F2FS进行挂载的时候，F2FS需要根据Headers和footers找到合适的可用的Checkpoint，如果两个Checkpoint都是可用的，那么F2FS会挑选version最新的Checkpoint进行恢复。在F2FS得到可用的Checkpoint之后，就会检查是否存在Orphan Inodes，如果找到了orphan inode，就会truncate它们的所有数据。然后，F2FS会根据SIT Blocks和NAT Blocks的bitmap恢复meata data和建立映射表。



## SylixOS文件系统源码查看笔记

- 在加载根文件系统（rootfs）的时候，要注册设备驱动程序(API_IosDrvInstallEx())。挂载载fatfs要用API_FatFsDevCreate()。加载romfs、ramfs的时候，要注册文件系统（__fsRegister()）。

### FATFS

FAT文件系统的卷信息保存在结构体FAT_VOLUME里，FAT控制信息位置、数据位置保存在那个扇区都记录在结构体FATFS里。正式装载文件系统用的是f_mount_ex()函数。

### YAFFS2

SylixOS里很多代码都是英文注释，是从其他平台的源码移植过来的还是外籍工程师写的还不知道。SylixOS与YAFFS的交互主要在yaffs_sylixos.c中。

- SylixOS通过\_\_tshellYaffsCmd()向yaffs发送指令（bad,info,erase等指令）。\_\_tshellYaffsCmd()先查找是否要操作的yaffs系统是否存在，然后再进行操作。
- SylixOS通过API_YaffsDrvInstall()安装驱动程序。在API_YaffsDrvInstall()里用到了API_IosDrvInstallEx2()来注册驱动程序。与上面不同的是API_IosDrvInstallEx2()还多传了“设备驱动类型”这个参数到函数里。
- 在API_IosDrvInstallEx()或API_IosDrvInstallEx2()中，SylixOS先进入IO临界区遍历==驱动程序表==。在表中空闲的地方装载函数（包括read,write,erase等等）。这些函数的函数指针会预先保存在结构体file_operations中。



















