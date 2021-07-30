

### 对齐问题

​		k210在内存访问上有着较为严格的对齐要求。对齐取决于定义的类型，如u32就要对齐32位，u64对齐64位。

但是FAT32的一些字段是不对齐的，例如有从0x0B开始的u16。因此读入时需要进行处理，而不能像ext2那样与结构体直接映射。

​		为解决该问题，读入的时候简单以字节buffer的形式读入，通过移位等运算获取信息。因为BPB块只需要读，而且大多信息对我们没有用处，因此只需把必要的信息一次读入，常驻内存。同时可以按照扇区结构定义结构体，用于从buffer获取指定字段数据。

​		rust使用\#[repr(packed)]限制结构体非对齐。

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

​		FAT32的目录项分为长文件名和短文件名两种，长度均为32B；

​		需要注意的是，长文件名编码为unicode，短文件名则为ascii；

​		长文件名的目录项还要有对11位短文件名的校验和；

​		长文件名以0xFF填充空缺，短文件用0x20填充；

​		目录项首为0xE5时表示删除；

​		实际上，目录的目录项的size字段恒为0；

​		短文件名统一转为大写存储；

#### 时间

目录项存储了文件的创建时间，主要用两个字节

存储格式为[year;m;day]  [hour;min;sec]

关键代码

```rust
	pub fn get_time(&self) -> (u32,u32,u32,u32,u32,u32) {
        // year-month-day-Hour-min-sec
        let year: u32  = ((self.creation_date & 0xFE00)>>9) as u32 + 1980;
        let month:u32  = ((self.creation_date & 0x01E0)>>5) as u32 ;    
        let day:u32  = (self.creation_date & 0x001F) as u32 ;    
        let hour:u32  = ((self.creation_time & 0xF800)>>11) as u32;    
        let min:u32  = ((self.creation_time & 0x07E0)>>5) as u32;    
        let sec:u32  = ((self.creation_time & 0x001F)<<1) as u32; // 秒数需要*2   
        (year,month,day,hour,min,sec)
    }
```

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

​		有一点需要注意，长文件名的编码为unicode。但幸运的是，对英文字符而言，其编码和ASCII一致，高位补0即可（好耶！

​		还有一点要注意，长文件名最后要有\0

#### 目录项的增减

因为目录项有长短文件名之分，因此不利于原有FS设计的无气泡删除策略。

同时看到win是直接在后面追加。

但如果一直在最后追加，效率会比较低，因为不知道末尾在哪，无法根据大小计算出

可以采用贪心策略，从size处开始找，但是... FAT32的目录没有size...... 只能遍历

删的时候直接置0xE5，磁盘空间不够了再统一整理。

#### 上下级目录与搜索

<img src="pic\初始化目录.jpg" style="zoom:60%;" />

从截图可以看出，目录新建时就具有./..两个目录，这和以前是一致的，但是根目录并没有这样的设计

这里有个问题，既然是在根目录下创建，“..”的26字段理应指向根目录所在的2号簇，但这里却为0。。

临时处置方式：<=2一律按2处理

在该目录内又创建了一个目录，./..的指向均正确：

<img src="pic\初始化目录2.jpg" style="zoom:60%;" />

在xv6 on k210中，直接对 . 和 .. 进行判断，并在每个目录项结构体引入了指针，指向parent。

而在rCore里，在进程控制块存储了当前路径的字符串，以栈的形式操作

这样的方法可以适应不同的文件系统，并且也利于返回当前工作路径。如果进程控制块存储的是当前路径的虚拟文件指针，那么用户获得路径就要回溯，但是，.. 并不指向上级目录的目录项，而是直接指向目录的簇，因此无法获得上级的文件名。

因此，我们也决定采用这种方法，在TCB存储字符串

这样，搜索的流程就变为

- 从process取出当前目录
- 开始循环
- 如果.就continue，如果..就pop，其他情况在当前目录搜索
  - 如果是根目录，不管./..，都不操作

这样，可执行文件在建立进程控制块的时候，就需要存入当前路径？

### 文件

#### 新建

新建文件无需分配簇。

#### 读写

因为簇比较大，实际上还是要按扇区（块）读写。

读文件的理想参数为

```rust
	pub fn read_at(
        &self,
        offset: usize,
        buf: &mut [u8],
        block_device: &Arc<dyn BlockDevice>,
    )
```

因为块号不在目录项中，不能像EXT那样直接检索

目录项中存放着起始簇号，因此需要由起始簇号和offset计算出块号和偏移(b_off)

该情况下，存储结构由大到小为 簇 > 块 > b_off

以下为我们的块级读写算法：

- 维护一个current_offset，记录当前写到的位置，初始为输入参数offset

- 先计算start_cluster，在计算start_block，以及当前块末尾的offset

- 读写当前块
- 递增current_offset，递增start_block，就可以继续写。
  - 簇内的块是连续的，因此递增块号就可以继续操作
- 如果current_offset大于簇的大小，调用FAT得到下一个簇，继续前述工作

因此关键就在于，要知道当前文件系统簇的大小，扇区的大小，这只能通过参数传入

同时也要传入FAT的引用

#### 大小修改

对文件写的时候会存在修改大小的问题。因为不具备索引结构，所以修改大小的时候只要修改FAT以及短名目录项的size。

FAT32的目录并不具备size信息，因此在increase_size的时候不能简单地得到需要多少空间，以及是否需要增加簇。

不过，得益于管理层提供的接口，可以轻松获知目录占有多少个簇，以及新的size需要多少个簇，这样就可以计算得到差值

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

​		经过反思，这样的设计不是很可行，一是不方便替换，二是不能按需分配，占用空间大，三十结构复杂，查找效率不高。最后决定保留两个缓存，一个用于存放检索信息，包括目录项，另一个用于存储数据，并采用spin::RwLock进行加锁

### 分层设计

UltraOS的文件系统依旧采用和原来一致的分层结构，结构如下：

| 层级 (low to high) | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| 块设备接口         | 一个提供给块设备的trait，SDCardWrapper实现了该特性           |
| 块缓存层           | 对外提供get_block_cache接口，会自动向设备读取/写回/替换块，现在采用双缓存策略 |
| 磁盘布局层         | 对BS，EXT_BS，FSINFO，目录项作了相关的映射；实现了抽象的FAT，提供了一些链式结构的接口。此层的短文件名目录项对应于原来的DiskInode，为访问文件的入口对象。 |
| FS管理层           | 对磁盘布局的一种抽象。可以打开已有的FAT32文件系统，控制簇的分配与回收(管理FAT)。提供FAT32相关的计算接口。 |
| 虚拟文件系统       | 主体对象为vFile（原来的inode），文件系统顶层，这里是真正意义上为系统提供了文件操作的接口，比如文件的搜索、创建、读写、清空 |
|                    | 以下为OS内核                                                 |
| inode.rs           | 主体对象为OSInode，实际就是内核对文件操作的载体，其实现了File trait。每个进程控制块内存放的文件描述符对应一个打开文件的OSInode的引用（fd_table）。 |

#### 接口定义

##### 块缓存层

​		文件系统以块(扇区)为基本读写单位，因此缓存的基本单位也是块。参照CPU缓存指令数据分离的设计，我们将块缓存分为信息缓存和数据缓存。信息缓存用于缓存存储检索信息的块，例如文件系统信息扇区、FAT、目录等，数据缓存则用于缓存文件的数据。这样做的好处是显然的：因为开发板内存很小，我们不能设置大的块缓存，如果不采用双缓存，一旦操作比较大的文件，该文件的块会充满整个缓存，覆盖检索信息块，造成目录操作、检索操作、信息修改等操作效率低下，尤其是修改文件时，会频繁访问FAT和磁盘信息扇区。使用两个缓存则可以避免覆盖的问题。

​		对于缓存本身，我们设置其单个大小为10个块，即5KB，双缓存合计10KB。因为UltraOS暂未实现完善的时钟，所以难以实现严谨的LRU策略，为此我们使用了近似LRU算法——Clock算法。我们使用了一个队列维护所有缓存块，该队列使用了alloc库提供的VecDequeue，其在堆上动态增减，进而可以灵活地减少内存占用。文件系统上层通过智能指针Arc访问缓存块，其会为引用原子计数。当队列满时，寻找引用量为0的块进行替换即可。

​		通过缓存，我们屏蔽了扇区读写的具体细节。我们定义了get_block/info_cache以及modify_block/info_cache四个接口，通过这些接口，上层模块不必考虑分区的起始偏移等问题，只需向缓存寻求需要的逻辑块号，缓存就会帮其获取。为了让上层模块可以直接按照需要的数据类型对缓存块操作，这四个接口支持传入闭包，通过闭包可以指定数据类型和处理的过程，进而对块中指定类型的数据进行操作。

```rust
// block_cache:
/*获取fs数据块*/
pub fn get_block_cache(
    block_id: usize, //所需块号
    block_device: Arc<dyn BlockDevice>, //块设备接口
    mode: CacheMode, //指定读写
) -> Arc<RwLock<BlockCache>> 
/*获取fs信息块*/
pub fn get_info_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    mode: CacheMode,
) -> Arc<RwLock<BlockCache>> 

// block_cache::BlockCache
// 获取缓存块的可变引用
pub fn get_mut<T>(
    &mut self, 
    offset: usize
) -> &mut T where T: Sized ;
// 获取缓存块的引用
pub fn get_ref<T>(
    &self, 
    offset: usize
) -> &T where T: Sized ;

// 通过闭包对缓存块进行读操作
pub fn read<T, V>(&self, offset: usize, f: impl FnOnce(&T) -> V) -> V
// 通过闭包对缓存块进行写操作
pub fn modify<T, V>(&mut self, offset:usize, f: impl FnOnce(&mut T) -> V) -> V
```

​		UltraOS的文件系统是支持分区磁盘的，这实际需要缓存的支持。在管理层启动文件系统后，其会从0号扇区读入隐藏扇区数（第一个分区的偏移量），并传递给缓存层，之后缓存层每次进行块设备读写时都会加上此偏移量，进而上层模块只需使用逻辑块号。

##### 磁盘布局层

​		磁盘布局层用于组织FAT32特有的数据结构。需要注意的是，K210要求对齐读写，即某类型变量的地址必须以该类型大小对齐。通过查看FAT32文档可知，引导扇区的一些字段是不对齐的，例如半字类型的字段可能与字节对齐。对于这些字段，我们以字节数组的形式读入，设计接口封装其读写以使上层直接使用字段类型控制。

​		首先介绍引导扇区和扩展引导扇区。他们给出了块设备的基本信息，例如扇区大小数量、簇大小、版本、校验签名、保留扇区数目、文件信息扇区所在位置等。如果设备进行了分区，引导扇区还包含了第一个分区的起始扇区。UltraOS的文件系统是支持分区磁盘的，这实际需要缓存的支持。在管理层启动文件系统后，其会从0号扇区读入隐藏扇区数（第一个分区的偏移量），并传递给缓存层，之后缓存层每次进行块设备读写时都会加上此偏移量，进而上层模块只需使用逻辑扇区号访问。引导扇区的信息通常不需要修改，他们只负责在文件系统其动时进行校验并为管理层提供相关的信息。事实上，这两个扇区也仅在文件系统起动时会读入。因为这两个扇区的字段较多，我们定义了完整的结构体，将所有字段作为成员，以与磁盘数据一一对应。使用时需从cache读入对应的块，然后将对块的引用转换为对应结构体的引用。

​		其次是文件系统信息扇区。该扇区有五个重要字段，前两个字段为校验签名，第三个为FS剩余簇数、第四个字段为起始空闲簇、第五个字段为校验签名。显然，第三、四个字段经常需要读写。该扇区的有效字段分布于扇区首尾，中间均为无效字节，为了节约空间，不必建立包含整个扇区字段成员的结构体。我们定义了一个FSInfo结构体，其只包含扇区号这一个成员，但是实现了丰富的接口，包括签名校验，簇信息字段的读写。只保留扇区号一个成员是合理的，因为扇区的读写经由缓存，而缓存的提供的接口只需要扇区号和偏移信息。

​		然后是目录项结构。目录项结构复杂，字段较多，因此依然实现了完整的结构体以与磁盘数据一一对应。FAT32的目录项长32字节，包括长文件名目录项和短文件名目录项两种类型，当文件名较长时，采用多个长名目录项加一个短名目录项的形式存储。文件的所有信息都存储于短名目录项，包括名称、扩展、属性、创建/访问/修改时间、大小、起始簇号等，因此我们将其作为文件的访问入口。对于长短名目录项，我们均设计了丰富的接口以进行封装，这样上层在访问时可以以直观的数据类型读写信息，而不必考虑复杂的内部结构。

##### FS管理层

​		FS管理层可以看作一个助手，但凡需要FAT32相关的计算，或者文件系统的相关信息，都可以向其寻求帮助。例如计算文件起始扇区、拆分长文件名、文件名格式化、短文件名生成、获取可用簇等等。管理层也起到一个统筹全局的作用，根目录、FAT都由其管理，因此簇的分配与回收也由管理层负责。

​		实用接口如下：

| 接口               | 描述                                 |
| ------------------ | ------------------------------------ |
| calculate_pos      | 计算簇号对应表项所在的扇区和偏移     |
| next_free_cluster  | 获取可用簇的簇号                     |
| get_next_cluster   | 获取当前簇的下一簇                   |
| set_next_cluster   | 设置当前簇的下一簇                   |
| get_cluster_at     | 获取某个簇链的第i个簇(i为参数)       |
| final_cluster      | 获取某个簇链的最后一个簇             |
| get_all_cluster_of | 获得某个簇链从指定簇开始的所有簇     |
| count_cluster_num  | 统计某个簇链从指定簇开始到结尾的簇数 |

##### 虚拟文件系统

​		虚拟文件系统层主要负责为内核提供接口，屏蔽文件系统的内部细节，首要任务就是实现复杂的功能。因为将短目录项作为文件的访问接口，所以在虚拟文件系统中，我们定义了虚拟文件对象vFile，其记录了文件短目录项所在扇区和偏移，是一个简单的抽象:

```rust
pub struct VFile {
    name:String,
    short_sector: usize, 
    short_offset: usize, //文件短目录项所在扇区和偏移
    long_pos_vec: Vec<(usize, usize)>, // 长目录项的位置<sector, offset>
    attribute:u8,
    fs: Arc<RwLock<FAT32Manager>>,
    block_device: Arc<dyn BlockDevice>,
}
```

这样做对文件进行了比较好的抽象，也便于调用下层的接口，但也存在问题，因为根目录没有目录项。重构已经不太可能，不如将虚拟进行到底。为此，我们创建了虚拟根目录项，驻留于fs管理层。该目录项是只读的，因为没有需要修改之处，因此在vfs中，唯一能接触该目录项的就是read_short_dirent()，我们只要在此处判断根目录，然后获取manager中的根目录项即可。

目前已经实现的接口

```rust
	pub fn new(
        name:String,
        short_sector: usize, 
        short_offset: usize,
        long_pos_vec:Vec<(usize, usize)>, 
        attribute:u8,
        size:u32,
        fs: Arc<RwLock<FAT32Manager>>,
        block_device: Arc<dyn BlockDevice>
    )->Self;

    pub fn get_attribute(&self)->u8;

    pub fn get_size(&self)->u32;

    pub fn get_fs(&self) -> Arc<RwLock<FAT32Manager>>;

    pub fn is_dir(&self)->bool;

    pub fn is_short(&self)->bool;

    pub fn read_short_dirent<V>(&self, f: impl FnOnce(&ShortDirEntry) -> V)->V;

    fn modify_long_dirent<V>(&self, index:usize ,f: impl FnOnce(&mut LongDirEntry) -> V)->V;

    pub fn modify_short_dirent<V>(&self, f: impl FnOnce(&mut ShortDirEntry) -> V)->V;

    pub fn get_pos(&self, offset:usize) -> (usize, usize);

    fn find_long_name(
        &self, 
        name: &str,
        dir_ent: &ShortDirEntry
    )->Option<VFile>;

    fn find_short_name(
        &self, 
        name:&str, 
        dir_ent: &ShortDirEntry
    ) -> Option<VFile>;

    /* 根据名称搜索当前目录下的文件 */
    pub fn find_vfile_byname(
        &self,
        name: &str,
    ) -> Option<VFile>;
    
    /* 根据路径递归搜索文件 */
    pub fn find_vfile_bypath(&self, path: Vec<&str>)-> Option<Arc<VFile>>;

    fn increase_size( &self, new_size:u32,);

    /* 在当前目录下创建文件 */ // TODO
    pub fn create(&self, name: &str, attribute: u8) -> Option<Arc<VFile>>;

    pub fn first_cluster(&self)->u32;

    /* 获取当前目录下的所有文件名以及属性，以Vector形式返回 */
    // 如果出现错误，返回None
    pub fn ls(&self)-> Option<Vec<(String, u8)>>;
    /* ls精简版，上面那个又臭又长，但这个不保证可靠 */
    // DEBUG
    pub fn ls_lite(&self)-> Option<Vec<(String, u8)>>;


    pub fn read_at(&self, offset: usize, buf: &mut [u8])->usize;

    pub fn write_at(&self, offset: usize, buf: &mut [u8])->usize;

    pub fn clear(&self);

    /* 查找可用目录项，返回offset，簇不够也会返回相应的offset，caller需要及时分配 */
    fn find_free_dirent(&self)->Option<usize>;
```

### 资料

[FAT32介绍]: https://blog.csdn.net/u010650845/article/details/60881687?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-4.control&amp;dist_request_id=1331973.7587.16185600288003081&amp;depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-4.control

[目录项]: https://blog.csdn.net/u010650845/article/details/60780979


