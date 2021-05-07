

### 资料

[FAT32介绍]: https://blog.csdn.net/u010650845/article/details/60881687?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-4.control&amp;dist_request_id=1331973.7587.16185600288003081&amp;depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-4.control

### 对齐问题

​		k210在内存访问上有着较为严格的对齐要求。对齐取决于定义的类型，如u32就要对齐32位，u64对齐64位。

但是FAT32的一些字段是不对齐的，例如有从0x0B开始的u16。因此读入时需要进行处理，而不能像ext2那样与结构体直接映射。

​		为解决该问题，读入的时候简单以字节buffer的形式读入，通过移位等运算获取信息。因为BPB块只需要读，而且大多信息对我们没有用处，因此只需把必要的信息一次读入，常驻内存。但是给出的结构体也很好用, 可以用于从buffer获取数据

​		rust使用\#[repr(packed)]限制结构体非对齐

​		有些字段需要写入，定义具有针对性的接口，往指定的扇区+offset写

### 重要字段

#### BIOS Parameter Block（0号扇区）

| Offset (decimal) | Offset (hex) | Size (in bytes) | Meaning                                                      |
| ---------------- | ------------ | --------------- | ------------------------------------------------------------ |
| 11               | 0x0B         | 2               | The number of Bytes per sector (remember, all numbers are in the little-endian format). |
| 13               | 0x0D         | 1               | Number of sectors per cluster.                               |
| 14               | 0x0E         | 2               | Number of reserved sectors. The boot record sectors are included in this value.（可以确定FAT表的位置） |
| 16               | 0x10         | 1               | Number of File Allocation Tables (FAT's) on the storage media. Often this value is 2. |
| 17               | 0x11         | 2               | Number of directory entries (must be set so that the root directory occupies entire sectors). |
| 19               | 0x13         | 2               | The total sectors in the logical volume. If this value is 0, it means there are more than 65535 sectors in the volume, and the actual count is stored in the Large Sector Count entry at 0x20. |
| 28               | 0x1C         | 4               | Number of hidden sectors. (i.e. the LBA of the beginning of the partition.) |
| 32               | 0x20         | 4               | Large sector count. This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the *Number of Sectors* entry at 0x13. |

#### Extended Boot Record（紧随其后）

| Offset (decimal) | Offset (hexadecimal) | Length (in bytes) | Meaning                                                      |
| ---------------- | -------------------- | ----------------- | ------------------------------------------------------------ |
| 36               | 0x024                | 4                 | Sectors per FAT. The size of the FAT in sectors.（FAT表扇区数，第二个FAT表的位置可以由此计算） |
| 44               | 0x02C                | 4                 | The cluster number of the root directory. Often this field is set to 2. |
| 48               | 0x030                | 2                 | The sector number of the FSInfo structure.                   |
| 50               | 0x032                | 2                 | The sector number of the backup boot sector.                 |
| 510              | 0x1FE                | 2                 | Bootable partition signature 0xAA55.                         |

#### FSInfo Structure (FAT32 only)

| Offset (decimal) | Offset (hexadecimal) | Length (in bytes) | Meaning                                                      |
| ---------------- | -------------------- | ----------------- | ------------------------------------------------------------ |
| 0                | 0x0                  | 4                 | Lead signature (must be 0x41615252 to indicate a valid FSInfo structure) |
| 484              | 0x1E4                | 4                 | Another signature (must be 0x61417272)                       |
| 488              | 0x1E8                | 4                 | Contains the last known free cluster count on the volume. If the value is 0xFFFFFFFF, then the free count is unknown and must be computed. However, this value might be incorrect and should at least be range checked (<= volume cluster count) |
| 492              | 0x1EC                | 4                 | Indicates the cluster number at which the filesystem driver should start looking for available clusters. If the value is 0xFFFFFFFF, then there is no hint and the driver should start searching at 2. Typically this value is set to the last allocated cluster number. As the previous field, this value should be range checked. |
| 508              | 0x1FC                | 4                 | Trail signature (0xAA550000)                                 |

### 文件分配表

FAT32使用了32位中的28位，最高四位为保留，需要忽略

需要注意的是，FAT可能非常非常大，将其常驻内存不是明智的选择，但是可以缓存几个扇区。

示例代码

```c
unsigned char FAT_table[sector_size];
unsigned int fat_offset = active_cluster * 4;
unsigned int fat_sector = first_fat_sector + (fat_offset / sector_size);
unsigned int ent_offset = fat_offset % sector_size;
 
//at this point you need to read from sector "fat_sector" on the disk into "FAT_table".
 
//remember to ignore the high 4 bits.
unsigned int table_value = *(unsigned int*)&FAT_table[ent_offset] & 0x0FFFFFFF;
 
//the variable "table_value" now has the information you need about the next cluster in the chain.
```

### 校验与损坏检测

FAT表中，标记为0x0FFFFFF7表示簇有问题

FSInfo有两个签名0x52526141和0x72724161



### 目录

#### 基本信息

​		FAT32的目录项分为长文件名和短文件名两种，长度均为32B。

​		需要注意的是，长文件名编码为unicode，短文件名则为ascii（有毒

​		长文件名的目录项还要有对11位短文件名的校验和

​		长文件名以0xFF填充空缺，段文件用0x20填充

​		目录项首为0xE5时表示删除

#### 文件类型

​		不论是长短目录项，都有一个attribute字段。有以下的标志可取：

​		READ_ONLY=0x01 HIDDEN=0x02 SYSTEM=0x04 VOLUME_ID=0x08 DIRECTORY=0x10 ARCHIVE=0x20 		

​		LFN=READ_ONLY|HIDDEN|SYSTEM|VOLUME_ID

​		LFN=0x0F，表示目录项为长文件名，其余则用于判断文件/目录和读写权限

#### 长文件名目录项		

​		长文件名目录项的第一个字段为order，用于表示使用多个目录项时的顺序。

​		如果oder的第无位为1，即顺序与0x40求或，则为最后一个长名目录项

​		长文件名的目录项可以由多个目录项拼接而成，以倒序布局于磁盘，同时【也要分配一个短文件名紧随其后】。例如文件abcdefghijklmnopq.txt的目录项布局为

| 此处为低地址  |
| ------------- |
| nopq.txt      |
| abcdefghijklm |
| abcdef~1 txt  |
| 此处为高地址  |

#### 目录项的增减

因为目录项有长短文件名之分，因此不利于原有FS设计的无气泡删除策略。

是看到win是直接在后面追加。。。也罢，爆了再说

但如果一直在最后追加，效率会比较低，因为不知道末尾在哪，无法根据大小计算出

暂时采用贪心策略，从size处开始找。

删的时候直接置0xE5

#### 上下级目录于搜索

<img src="C:\Users\Eobard\Desktop\os竞赛\pic\初始化目录.jpg" style="zoom:60%;" />

从截图可以看出，目录新建时就具有./..两个目录，这和以前是一致的，但是根目录并没有这样的设计

这里有个问题，既然是在根目录下创建，“..”的26字段理应指向根目录所在的2号簇，但这里却为0。。

临时处置方式：<=2一律按2处理

在该目录内又创建了一个目录，./..的指向均正确：

<img src="C:\Users\Eobard\Desktop\os竞赛\pic\初始化目录2.jpg" style="zoom:60%;" />

看了下xv6 on k210，他们直接对 . 和 .. 进行判断，并在每个目录项结构体引入了指针，指向parent。

而在rCore里，在进程控制块存储了当前的路径，以栈的形式操作

因为这样的方法可以适应不同的文件系统，我们也决定采用这种方法

这样，搜索的流程就变为

- 从process取出当前目录
- 开始循环
- 如果.就continue，如果..就pop，其他情况在当前目录搜索
  - 如果是根目录，不管./..，都不操作

这样，可执行文件在建立进程控制块的时候，就需要存入当前路径？

### 文件

#### 新建

新建文件无需分配簇

### 寻址

#### 扇区计算

FAT表存的是簇号，要转化为扇区号才能提供给磁盘层访问。

经过验证，根目录的扇区号为 BS->reserved_sector_count + EBS->fat_size * BS->table_count  (BS表示boot sector)

由此，其他簇的扇区可以通过以下代码计算得到

```rust
// in fat32_manager
(cluster - 2) * self.sectors_per_cluster as u32 + self.root_sec
```

#### 文件名匹配

因为FAT32采用长短文件名目录项设计，按文件名搜索会比较复杂

长文件名目录项包含13个字符，短文件名包含8+3个字符

为了简化搜索过程，文件系统要在搜索前进行预处理

- 首先判断去除后缀名后，文件名长度是否大于8，是则为长文件名，不是就直接搜

- 如果是上文件名将其拆分，例如abcdefghijklmnopq.txt，可以拆成abcdefghijklm和nopq.txt，访问的时候就每次取出两个块进行匹配。因为在磁盘中文件名以倒序排放，即nopq.txt在低地址，abcdefghijklm在高地址，因此可以首先使用nopq.txt进行匹配，品牌完成后再查看高地址的是否为abcdefghijklm。需要注意的是，nopq.txt应保证为长名目录项的最后一项，即第一个字节为0x4*。

- 如果长名匹配成功，则理论上下一项为短文件名，可以计算校验和并检验。

  

### 缓存设计

#### 多路缓存

​		Ultra OS的文件系统采用了多缓存的设计，让缓存更具针对性。整体上，FAT32经常操作的数据可以分为info扇区、FAT表、目录项、数据块四个部分，因此我们也将缓存划分为四种，合计缓存20个块（20*512 = 10KB）。
​		其中，info扇区有效的字段很少，但为了读写方便，还是将其作为一个块缓存。
​		FAT表实际上可以非常大，因此不宜全部放内存。实际上，一个扇区就足以存放128个簇号，因此就比赛而言，一个缓存块就足矣，但是为了通用性，使用两个块存放。FAT文件系统使用2个FAT，其中一个作备份用，因此对两个FAT的更新都需要同步。因此，我们采用纠缠缓存设计，共计缓存4个块，每个FAT分别两个块，且两个FAT被缓存的块是一一对应的。

​		目前还剩余15个块。FAT32中目录实际上代表了文件，它涵盖文件的所有信息，大小为32B，即一个块可以包含16个目录项（长文件名的情况会少一些），使用3个块缓存。最后，文件数据则使用12个块缓存。

#### 简化版多路缓存以及锁机制		

​		经过反思，这样的设计不是很可行，一是不方便替换，二是不能按需分配，占用空间大，三十结构复杂，查找效率不高。最后决定保留两个缓存，一个用于存放检索信息，另一个用于存储数据，并采用spin::RwLock进行加锁

### 分层设计

UltraOS的文件系统依旧采用和原来一致的分层结构，结构如下：

| 层级 (low to high) | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| 块设备接口         | 一个提供给块设备的trait，SDCardWrapper实现了该特性           |
| 块缓存层           | 对外提供get_block_cache接口，会自动向设备读取/写回/替换块，现在采用双缓存策略 |
| 磁盘数据结构层     | 对BS，EXT_BS，SINFO，目录项作了相关的映射；实现了抽象的FAT，提供了一些链式结构的接口。此层的短文件名目录项对应于原来的DiskInode，为访问文件的入口对象。 |
| FS管理层           | 对磁盘布局的一种抽象。可以打开已有的FAT32文件系统，控制簇的分配与回收。 |
| 虚拟文件系统       | 主体对象为vFile（原来的inode），文件系统顶层，这里是真正意义上为系统提供了文件操作的接口，比如文件的搜索、创建、读写、清空 |
|                    | 以下为OS内核                                                 |
| inode.rs           | 主体对象为OSInode，实际就是内核对文件操作的载体，其实现了File trait。每个进程控制块内存放的文件描述符对应一个打开文件的OSInode的引用（fd_table）。 |

#### 接口定义

##### 块缓存层

```rust
/*获取fs数据块*/
pub fn get_block_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    mode: CacheMode,
)
/*获取fs信息块*/
pub fn get_info_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    mode: CacheMode,
)
```

##### 磁盘数据结构层

```rust

```

##### FS管理层

##### 虚拟文件系统