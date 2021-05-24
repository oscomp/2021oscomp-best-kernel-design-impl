# 研读Linux源码——JFFS2

> 基于Linux 2.5.0版
>
> 初识JFFS2，感觉代码量除了那个zlib.c以外挺少的





**jffs2中目录文件**由一个jffs2_raw_inode数据实体和若干jffs2_raw_dirent数据实体组成，由于目录文件的数据实体之间都是离散存放的，所以每个jffs2_raw_dirent中还得描述其所属目录文件的索引结点号。

正规文件所需jffs2_raw_inode个数大于等于1；

除了根文件，每个文件都至少存在一个jffs2_raw_dirent，以此来实现硬链接；



flash 上每个数据实体的位置、长度都由 jffs2_raw_node_ref 数据结构**描述**；

```c++
struct jffs2_raw_node_ref
{
    struct jffs2_raw_node_ref *next_in_ino;
    struct jffs2_raw_node_ref *next_phys;
    uint32_t flash_offset;
    uint32_t totlen;
};
```

其中flash_offset表示相应数据实体在flash分区上的物理地址，totlen为包括后继数据的总长度。同一个文件的多个jffs2_raw_node_ref由next_in_ino组成一个循环链表，链表首为文件内核描述符jffs2_inode_cache数据结构的nodes域，链表末尾元素的next_in_ino则指向jffs2_inode_cache，这样任何一个jffs2_raw_node_ref元素就都知道自己所在的文件了。

一个flash擦除块内所有数据实体的内核描述符由next_phys域组织成一个链表，其首尾元素分别由擦除块描述符jffs2_eraseblock数据结构的first_node和last_node域指向。



```c++
struct jffs2_inode_cache {
    struct jffs2_full_dirent *scan_dents;
    struct jffs2_inode_cache *next;
    struct jffs2_raw_node_ref *nodes;
    uint32_t ino;
    int nlink;
    int state;
};
```

ino为文件的在文件系统中唯一的索引结点号；所有文件内核描述符被组织在一个inocache_list哈希表中，next用于组织冲突项的链表。

nlink为文件的硬链接个数，在挂载文件系统时会计算指向每个文件的目录项个数，然后赋值给nlink。注意上层VFS所使用的nlink与此不同：在打开文件时会首先将jffs2_inode_cache的nlink复制到inode的nlink，然后对于非叶目录，会根据其下子目录的目录项个数增加inode的nlink。

**在挂载jffs2文件系统时**将遍历整个文件系统（扫描jffs2文件系统映象所在的整个flash分区），为flash上每个jffs2_raw_dirent和jffs2_raw_inode数据实体创建相应的内核描述符jffs2_raw_node_ref、为每个文件创建内核描述符jffs2_inode_cache，具体过程详见下文。

另外**在打开文件**时，如果是目录文件，则还要为每个jffs2_raw_dirent创建相应的jffs2_full_dirent数据结构并组织为链表；如果是正规文件等，则为每个jffs2_raw_inode创建相应的jffs2_full_dnode、jffs2_tmp_dnode_info、jffs2_node_frag数据结构，并组织到红黑树中。



```c++
struct jffs2_raw_dirent
{
    jint16_t magic;
    jint16_t nodetype; /* == JFFS_NODETYPE_DIRENT */
    jint32_t totlen;
    jint32_t hdr_crc;
    jint32_t pino;
    jint32_t version;
    jint32_t ino; /* == zero for unlink */
    jint32_t mctime;
    uint8_t nsize;
    uint8_t type;
    uint8_t unused[2];
    jint32_t node_crc;
    jint32_t name_crc;
    uint8_t name[0];
} __attribute__((packed));
```

紧随jffs2_raw_dirent的是相应文件的文件名，长度由nsize域表示，而name域预留了文件名字符串结束符的空间，ino表示相应文件的索引结点号。如前所述flash上目录文件的若干jffs2_raw_dirent数据实体是离散的，而且也没有类似ext2_inode的“索引”机制，所以就必须由每个jffs2_raw_dirent数据实体表明自己所属的目录文件。pino即是为这个目的设计的，表示该目录项所属的目录文件的索引节点号。另外在挂载文件系统时，**会将jffs2_raw_dirent数据实体的描述符加入pino所指文件，即该目录项所属目录文件的jffs2_inode_cache的nodes链表**，参见jffs2_scan_dirent_node函数。

**版本号version**是相对于某一文件内部的概念。任何文件都由若干jffs2_raw_dirent或者jffs2_raw_inode数据实体组成，修改文件的“某一个区域”时将向flash写入新的数据实体，它的version总是不断递增的。一个文件的所有数据实体的最高version号由**其inode的u域，即jffs2_inode_info数据结构**中的highest_version记录。文件内同一“区域”可能由若干数据实体表示，它们的version互不相同，而且除了最新的一个数据结点外，其余的都被标记为“过时”。（另外，按照jffs2作者的论文，如果flash上数据实体含有相同的数据则允许它们的version号相同）

**打开目录文件时要创建其VFS的dentry 、inode、file对象**，在创建inode时要调用super_operations函数指针接口中的read_inode方法，根据相应的内核描述符jffs2_raw_node_ref为每个目录项创建一个上层的jffs2_full_dirent数据结构，**并读出jffs2_raw_dirent数据实体后的文件名到jffs2_full_dirent数据结构后面**。jffs2_full_dirent组成的链表则由目录文件的索引结点inode.u.dents（即jffs2_inode_info.dents）指向。

==总结：==u域就是jffs2_raw_inode结构体中的jffs2_inode_info结构体；raw就是指存放在物理辅存上的数据结构体；文件名紧跟着dirent结构体；



```c++
struct jffs2_full_dirent
{
    struct jffs2_raw_node_ref *raw;
    struct jffs2_full_dirent *next;
    uint32_t version;
    uint32_t ino; /* == zero for unlink */
    unsigned int nhash;
    unsigned char type;
    unsigned char name[0];
};
```

jffs2_full_dirent数据结构在打开目录文件时才创建，用于保存读出的jffs2_raw_dirent数据实体的结果。其中raw指向相应的jffs2_raw_node_ref结构，紧随其后的为从flash上读出的文件名。

==总结：==jffs2_inode_info就类似一个各种指针的大本营，**应该也是打开文件时创建**？包含了一个文件的jffs2_inode_cache、jffs2_full_dnode、jffs2_full_dirent链表（如果是目录文件）等；



```c++
struct jffs2_raw_inode
{
    jint16_t magic; /* A constant magic number. */
    jint16_t nodetype; /* == JFFS_NODETYPE_INODE */
    jint32_t totlen; /* Total length of this node (inc data, etc.) */
    jint32_t hdr_crc;
    jint32_t ino; /* Inode number. */
    jint32_t version; /* Version number. */
    jint32_t mode; /* The file's type or mode. */
    jint16_t uid; /* The file's owner. */
    jint16_t gid; /* The file's group. */
    jint32_t isize; /* Total resultant size of this inode (used for truncations) */
    jint32_t atime; /* Last access time. */
    jint32_t mtime; /* Last modification time. */
    jint32_t ctime; /* Change time. */
    jint32_t offset; /* Where to begin to write. */
    jint32_t csize; /* (Compressed) data size */
    jint32_t dsize; /* Size of the node's data. (after decompression) */
    uint8_t compr; /* Compression algorithm used */
    uint8_t usercompr; /* Compression algorithm requested by the user */
    jint16_t flags; /* See JFFS2_INO_FLAG_* */
    jint32_t data_crc; /* CRC for the (compressed) data. */
    jint32_t node_crc; /* CRC for the raw inode (excluding data) */
    // uint8_t data[dsize];
} __attribute__((packed));
```

正规文件、符号链接、设备文件的jffs2_raw_inode**后都有相应的数据**，**共同组成一个flash上的数据实体**；

一个正规文件可能**由若干jffs2_raw_inode数据实体组成**，每个数据实体含有文件一个区域的数据。**即使文件的同一个区域也可能因为修改而在flash上存在多个数据实体**，它们都**含有相同的ino**，即文件的索引结点编号。

文件在逻辑上被当作一个连续的线性空间，每个jffs2_raw_inode**所含数据在该线性空间内的偏移由offset域表示**。注意offset为文件内的偏移，而该jffs2_raw_inode**在flash分区中的偏移则由其内核描述符jffs2_raw_node_ref的flash_offset**域表示。

jffs2支持数据压缩。如果后继数据没有被压缩，则compr被设置JFFS2_COMPR_NONE。压缩前（或解压缩后）的数据长度由==dsize==表示，而压缩后的数据长度由==csize==表示。从后文的相关函数分析中可以看到，在修改文件的已有内容或者写入新内容时，首先要将数据压缩，然后在内存中组装合适的jffs2_raw_inode结构，最后再将二者连续地写入flash。而在读flash上的设备结点时首先读出jffs2_raw_inode结构，然后根据其中的csize域的值，分配合适大小的缓冲区，第二次再读出紧随其后的（压缩了的）数据。在解压缩时则根据dsize大小分配合适的缓冲区。另外，如果jffs2_raw_node没有后继数据而是代表一个洞，那么compr被设置为JFFS2_COMPR_ZERO。



```c++
struct jffs2_full_dnode
{
    struct jffs2_raw_node_ref *raw;
    uint32_t ofs; /* Don't really need this, but optimisation */
    uint32_t size;
    uint32_t frags; /* Number of fragments which currently refer
    to this node. When this reaches zero, the node is obsolete. */
};

struct jffs2_node_frag
{
    rb_node_t rb;
    struct jffs2_full_dnode *node; /* NULL for holes */
    uint32_t size;
    uint32_t ofs; /* Don't really need this, but optimisation */
};
```



## background.c

spin_lock_bh这种东西就简单地理解为lock就可；

send_sig(sig,*task struct,priv)第三个参数：0代表这个中断信号来自用户进程，1代表来自内核，2代表来自内核且这个signal是SIGSTOP或SIGKILL；



## 草稿

> 便于自己阅读pdf手册

> 第1章

在挂载时，

- 一个jffs2_raw_dirent对应一个jffs2_raw_node_ref；
- 一个jffs2_raw_inode对应一个jffs2_raw_node_ref；
- 一个文件对应一个jffs2_inode_cache（一个文件可以含有多个jffs2_raw_inode）；

在打开一个文件时，

- 一个jffs2_raw_dirent对应一个jffs2_full_dirent（如果打开的是目录文件，且这些full_dirent被组织为链表）
- 一个jffs2_raw_inode对应一个jffs2_full_dnode、jffs2_tmp_dnode_info、jffs2_node_frag（如果是一个正规文件，并通过jffs2_node_frag组织到文件的**红黑树**中）

jffs2_node_frag差不多就可以看作是文件的红黑树上的一个结点，代表一个raw_inode中的数据。

jffs2_raw_inode所代表的数据就是紧随这个数据结构后面的数据，就像文件名紧随dirent后面一样。

inode的u域（jffs2_inode_info中的fragtree就是文件的红黑树，对于例如目录文件、符号链接等不需要红黑树的文件，只需要用u域中的metadata指向**唯一的一个**jffs2_full_dnode结构体即可；红黑树则是由fragtree指向各个jffs2_node_frag，而每个jffs2_node_frag再对应一个jffs2_full_dnode）



> 第2章

jffs2在flash上没有超级块实体，u域就是union域，**数据体中相应的u域就是VFS中每个文件系统的个性区域**，super_block（指VFS中的super_block）的u域就是jffs2_sb_info结构体；

jffs2_sb_info中的**mtd_info**就是与底层Flash设备打交道的；

jffs2_sb_info中的**highest_info**就是要分配给下一个文件的inode number号（不知道是一个文件唯一一个inode number，（肯定是要一个文件一个，不然怎么通过inode number在哈希表中得到文件的inode cache呢）~~还是指给每个jffs2_raw_inode一个~~）。

jffs2_sb_info中就是**包含论文中的重要的三部分**：为了使各flash擦除块都被均衡使用的各种xxxx_list链表和GC内核线程，以及用于组织所有文件的内核描述符的哈希表inocache_list，而文件描述符（就是文件内核描述符？）的主要作用就是描述一个文件的所有离散的数据结点的位置等信息。

jffs2_eraseblock是flash擦除块描述符，就是代表一个block，在jffs2中的各种xxx_list负责控制的基本单位就是这些一块块block；所有的jffs2_eraseblock都有存放在jffs2_sb_info中的block数组中。

所有的jffs2_inode_cache组成一个哈希表，这个哈希表就是jff2_sb_info中的inocache_list。文件的所有jffs2_raw_node_ref组成一个链表，这个链表就是jffs2_inode_cache的nodes。

在打开文件创建其inode时由具体文件系统提供的**read_inode**方法来初始化inode的u域。

jffs2_inode_info的fragtree/metedata、或者dents来组织文件的所有数据实体的内核描述符。

在普通文件中，是frag->dnode->node_ref；在目录文件中，是dirent->node_ref。



> 第3章

**文件系统注册有两种时机**，一是在配置内核时选择对文件系统的支持，那么**其代码被静态链接到内核中**，在内核初始化期间init内核线程执行init_jffs2_fs函数完成jffs2的注册；二是在安装文件系统模块时在模块初始化函数中完成注册。

在注册文件系统类型时的file_system_type结构体中，**read_super**（函数指针）是最重要的，这个函数指针即为上层VFS框架与具体文件系统的特定挂载方法之间的接口，VFS框架中挂载文件系统的代码向下只调用到该函数指针，是具体文件系统所提供的各种方法的“总入口”。

如果是通过配置内核添加jffs2文件系统，则会执行**init_jffs2_fs**函数，在函数中会执行**register_filesystem将jffs2注册到内核全局变量file_systems**中（就是添加到file_system_type结构体组成的链表的末尾）。



### 第4章

在挂载文件系统时，调用**read_super**函数，就是读取super_block，而super_block中就有各种super_operations，super_operations中是一些打开文件、读写文件的方法；

在打开文件、创建文件inode时，会调用**super_operations**中的方法，并针对不同类型的文件再实例化不同的file_operations方法表（这个方法表应该是在inode结构中），file_operations中就是针对这种类型文件的操作函数；

读写文件时调用的就是**file_operations**；

在jffs2中read_super就是**jffs2_read_super**函数；这个函数就是填好一个VFS中的super_block再return，而且负责扫描整个flash设备，在扫描过程中建立起各种数据实体的内核描述符；

**jffs2_read_super**函数调用**jffs2_do_fill_super**函数进行真正初始化VFS超级块super_block数据结构、为flash上所有数据实体建立内核描述符jffs2_raw_node_ref、为所有文件创建内核描述符jffs2_inode_cache。

``c->cleanmarker_size = sizeof(struct jffs2_unknown_node);``在完全擦除掉一个块后，就会写入cleanmarker来标记，**从这个代码来看cleanmarker就是jffs2_unknown_node**？



#### jffs2_do_fill_super函数

**jffs2_do_fill_super**函数调用**jffs2_do_mount_fs**函数进行文件系统挂载的绝大部分工作。在这个函数中就是扫描flash设备中所有的block，并根据这些block的情况分别加入到jffs2_sb_info中的xxx_list中；

**jffs2_do_fill_super**函数还为JFFS2的根目录"/"创建VFS的inode和dentry；通过iget函数创建inode，inode number是1。

**回顾**：目录文件只有一个jffs2_raw_inode，其它都是jffs2_raw_dirent，在打开目录文件时为其惟一的jffs2_raw_inode创建jffs2_full_dnode数据结构，并由jffs2_inode_info的metedata直接指向；为目录文件的所有jffs2_raw_dirent目录项创建相应的jffs2_full_dirent数据结构并组织为链表。

**jffs2_do_fill_super**函数还会启动GC内核线程。



#### jffs2_do_mount_fs函数

sector_size是可擦除块的大小。

**jffs2_do_mount_fs**函数调用**jffs2_build_filesystem**函数完成挂载文件系统的绝大部分操作。

**jffs2_do_mount_fs**函数只完成初始化一些链表这种工作。

**回顾**：一个擦除块内所有数据实体的内核描述符jffs2_raw_node_ref由其next_phys域组织成一个链表，其首尾元素分别由jffs2_eraseblock的first_node和last_node指向。

**文件的索引结点号在某个设备上的文件系统内部才唯一**（意思是不同文件系统之间的索引结点号不共享），当前flash分区的jffs2文件系统中最高的索引结点号由jffs2_sb_info的highest_ino域记录，而1是根目录“/”的索引结点号。



#### **jffs2_build_filesystem**函数

**jffs2_build_filesystem**函数真正地完成jffs2文件系统的挂载工作。

**jffs2_build_filesystem**函数中会为每个jffs2_raw_dirent创建临时的jffs2_full_dirent，目的只是为了计算所有文件的硬链接计数。在jffs2_build_filesystem函数最后会释放这些临时的jffs2_full_dirent。

**jffs2_build_filesystem**函数调用**jffs2_scan_medium**函数完成扫描flash，建立内核描述符，将jffs2_inode_cache加入inocache_list，根据各个擦除块的内容加入到不同的xxx_list中。（同时还会建立起临时的jffs2_full_dirent，虽然在jffs2_build_filesystem函数结束时会将所有的jffs2_full_dirent释放掉）



#### jffs2_scan_medium函数

在代码中==jffs2_cleanmarker_oob(c)==这一行代码就可以判断是Nand Flash还是Nor Flash了，如果返回为1，代表存在OOB区，就是Nand Flash，返回0就是Nor Flash。c是jffs2_sb_info。

**jffs2_scan_medium**函数中，针对jffs2_sb_info中的blocks[]数组的每个eraseblock，调用**jffs2_scan_eraseblock**函数进行扫描，**jffs2_scan_eraseblock函数除了将block加入到某个xxx_list以外的工作都做了**。

如果过时数据实体所占空间超过擦除块的一半大小，则认为该擦除块“很脏”VERY DIRTY。

jffs2_sb_info的nextblock指向当前写入操作发生的擦除块。如果当前擦除块的剩余空间大小超过nextblock所指的擦除块，则将nextblock指向当前擦除块，而把原先的擦除块加入(very)dirty_list。

就是说，在**jffs2_scan_medium**函数中，对于那种partly dirty的擦除块，如果这个dirty块的free size还很大，就不忙加入xxx_list，就可以先变为jffs2_sb_info的nextblock块使用（就是选个free size最大的dirty块作为nextblock）。

在开发板上使用的是NOR flash，所以CONFIG_JFFS2_FS_NAND宏未定义，所以==jffs2_can_mark_obsolete(c)==被定义为1。



#### jffs2_scan_eraseblock函数

该函数完成对flash中的一个擦除块进行扫描等工作，最后返回一个代表该擦除块状态的数据（例如BLK_STATE_BADBLOCK等宏定义）好让上层函数jffs2_scan_medium将这个block加入到相应xxx_list中。

在挂载时，为了从Flash中读取数据，且如果不用直接内存映射的方式，那么就需要设置读缓冲区，如果是Nand Flash则缓冲区大小为整个擦除块的大小，如果是Nor Flash则缓冲区大小为一个内存页框大小（PAGE_SIZE和EMPTY_SCAN_SIZE貌似都是1024）。

通过**jffs2_fill_scan_buf**函数读取flash分区上偏移为buf_ofs、长度为buf_len的数据到buf缓冲区中。这个函数就是直接调用jffs2_flash_read函数，而后者为直接调用flash驱动程序read方法的宏。在Nor Flash的情况下会多次调用**jffs2_fill_scan_buf**函数，每次读一个内存页框到buf中：

```c++
#define jffs2_flash_read(c, ofs, len, retlen, buf) ((c)->mtd->read((c)->mtd, ofs, len, retlen, buf))
```

在扫描时，认为一个块前面的0xFF数据都是dirty的数据而非free的；

jffs2_unknown_node是指所有node公共的前面结构的数据结构（magic数，nodetype等）；

数据实体在块内都是4字节对齐。

jffs2要求任何数据实体不能跨越一个擦除块。如果这种情况发生了，则跳过4字节并开始新的循环。

jffs2_raw_inode所代表的一段数据是紧随在jffs2_raw_inode结构体后面的，**头部中totlen**为包括了后继数据的数据实体总长度。

在jffs2_scan_eraseblock函数扫描过程中，

- 如果发现是一个jffs2_raw_inode数据实体，则又调用**jffs2_scan_inode_node**函数为该raw_inode创建内核描述符，如果是一个文件的第一个raw_inode则还会创建inode_cache并加入全局哈希表中。
- 如果发现是一个jffs2_raw_dirent数据实体，则调用**jffs2_scan_dirent_node**函数分析。
- 如果发现是一个CLEANMARKER，则检查几下后再为其创建内核描述符jffs2_raw_node_ref。
- 如果发现是一个PADDING，则说明就是个填充块，跳过node->totlen长度即可。
- 如果发现是其它特殊类型，例如某些可能导致必须以只读挂载方式挂载JFFS2，再进行相应特殊处理。

flash上数据实体不仅起始地址是**4字节对齐**，长度也是也是**4字节对齐**。



#### jffs2_scan_inode_node函数

该函数中，首先调用**jffs2_get_ino_cache**函数在哈希表中通过inode number（来自于raw_inode结构体）查找inode cache，如果该cache暂未创建，就调用**jffs2_scan_make_ino_cache**函数创建inode cache。

由于flash上数据结点的总是4字节地址对齐的，所以jffs2_raw_node_ref的flash_offset的最低两个bit总是0，所以可用利用它们标记相应数据实体的状态。

在挂载文件系统时并没有检查flash上数据实体的有效性，而把crc校验的工作推迟到了打开文件、为文件创建jffs2_full_dnode或者jffs2_full_dirent时进行。（在下文jffs2_scan_dirent_node函数分析中可以看到，对目录项数据实体即刻进行了crc校验）

在jffs2_scan_inode_node函数末尾会将该inode所在的block中free_size减小，并增大unchecked_size，即认为整个文件所占的空间都认为是unchecked。



#### jffs2_scan_make_ino_cache函数

该函数接受待建立inode_cache的文件的索引号inode number，返回该inode number对应的inode cache，无论是新建立还是已在hash表中。

由该函数也负责将inode_cache插入到文件系统的hash表。

注意该函数在创建根目录的inode cache时会自动将nlink设置为1，否则由于根目录没有对应的dirent来指向它就会使nlink为0。



#### jffs2_scan_dirent_node函数

该函数在挂载文件系统时为已读出的jffs2_raw_dirent目录项数据实体创建内核描述符jffs2_raw_node_ref和临时的jff2_full_dirent。如果为相应目录的jffs2_inode_cache尚未创建则创建之，并建立三者之间的连接关系。

jffs2_raw_dirent的totlen是指jffs2_raw_dirent和紧随其后的文件名一共的长度。

**回顾**：在为dirent创建内核描述符时也会临时创建full_dirent。

jffs2_raw_dirent的nsize是指紧随其后的文件名的长度，在分配内存时记得加1为'\0'留空间。

jffs2_full_dirent 的nhash 域为根据文件名计算的一个hash值，然后在通过 jffs2_add_fd_to_list 函数将其插入由jffs2_inode_cache.scan_dents 指向的链表时，按照 nhash **由小到大**的顺序插入。

对于目录文件唯一的raw_inode，其对应的node_ref并不一定是其inode cache的nodes链表的第一个，nodes 链表中各描述符的先后顺序完全由 flash 上各数据实体的先后顺序决定，每扫描一个就加入链表首部。

==jffs2_inode_cache中的scan_dents其实是没有用的==，只在scan阶段有用（即挂载阶段）。在之后目录文件所有的dirents由jffs2_inode_info中的dents指向而此时jffs2_inode_cache中的scan_dents早已是NULL了。



#### jffs2_build_inode_pass1函数

该函数是在jffs2_build_filesystem函数中当调用完jffs2_scan_medium函数，就会接着为每个inode cache调用一次jffs2_build_inode_pass1来统计各个文件的硬连接数。

该函数就是根据传入的inode cache（简称ic），如果其scan_dents非空就代表是目录文件，就顺着scan_dents去遍历子文件的inode cache，将其nlink++即可。



### 第5章 打开文件时建立inode的方法

在前面的jffs2_do_fill_super函数执行完jffs2_do_mount_fs函数后，就是要为根目录建立inode和dentry（这里指VFS中的inode和dentry）

```c++
root_i = iget(sb, 1);	//为根目录建立inode
```

**函数调用过程**：iget > iget4 > get_new_inode > super_operations.read_inode

==文件索引节点号在文件系统所在的设备上是惟一的==。意思是不同文件系统的inode number可能有重复的，比如根节点的inode number都是1？

==在Linux中VFS所有的inode都在一张hash表inode_hashtable中==，为了找到某个文件系统的某个节点号的文件，就调用iget4函数，iget4函数中会结合文件系统的super block以及文件索引号inode number**计算出这个文件在VFS的inode_hashtable偏移地址**，**当然是个链表**（为了解决hash冲突的问题），通过这个链表再调用**find_inode**函数查找指定inode number的文件，返回其inode结构体。

如果根目录的inode已存在就增加其引用数就行。如果不存在才会进一步调用get_new_inode函数创建VFS中的inode（同时会传入hash表inode_hashtable中该inode该插入到的链表头head）。

inode_lock是内核全局变量的一个spinlock，应该是对inode_hashtable有改动的话甚至是改变一个inode的state都应先锁住这个inode_lock。



#### get_new_inode函数

该函数中首先调用**alloc_inode**函数创建新的inode，接着就是根据传入的参数（super_bloc, ino, head）为inode填充基本属性，并将其加入到inode_hashtable对应的链表中，同时也加入到内核链表inode_in_use中（所有inode也都存在于inode_in_use中）。之后再调用不同文件系统各自的**read_inode**函数（即**jffs2_read_inode**函数）来为inode填充跟特殊的属性，例如特定文件操作的函数指针。

在这里还需知道的一个小知识，将VFS的inode的state或上I_LOCK标志后，==就会使任何其它想要碰这个inode的进程阻塞到inode.i_wait队列上==，且这一过程不需要inode_lock锁，因此能保证接下来本进程对该inode的操作是原子性的。



#### jffs2_read_inode函数

在调用这个函数之前，VFS中的inode已经填了一部分通用的属性了，剩下的就是需要jffs2_read_inode函数来对inode中的u域——**jffs2_inode_info**来进行填写。

函数首先从inode中获取文件系统的u域即jffs2_sb_info，再获取inode的u域即jffs2_inode_info（注意此时的inode_info还未填写）。接着便调用**jffs2_init_inode_info**函数来对inode_info进行**初始化**。

接着调用**jffs2_do_read_inode**函数来进行**真正重要的工作**，即将raw变为full的工作，特别是如果是普通文件还会建立红黑树来组织所有数据结点。

这里需要知道的知识是，如果jffs2_do_read_inode函数执行失败，则会将该inode标记为"bad"，具体是将VFS中的inode的i_op和i_fop分别指向Linux中的bad_inode_ops和bad_file_ops，==在Linux中判断一个inode是否是坏的也是通过判断i_op是否指向bad_inode_ops来实现的==。

VFS的inode中的i_nlink就是该inode的硬链接数，因为任何一个目录的都有"/."与"/.."，因此目录文件的硬连接数至少为2，而目录文件每有一个子目录，则硬链接数同样加1，因为子目录的"/.."指向的是自己。

inode的i_mapping是内存映射有关的函数指针集合。

如果通过jffs2_do_read_inode函数得知该inode是设备文件，则会调用**jffs2_read_dnode**函数读取紧随在jffs2_raw_inode后面的设备号。**jffs2_read_dnode**函数就是从一个dnode开始读取其数据。

对于特殊文件（设备文件、SOCKET、FIFO文件），它们的inode通过**init_special_inode**函数来进一步初始化。

==设备文件一般需要两个设备号==，一个是设备文件所在的设备的设备号，另一个是设备文件所代表的这个设备的设备号，这两个号在VFS中的inode分别由i_dev和i_rdev表示。在JFFS2中没有i_dev，只是在设备文件的jffs2_raw_inode后面紧跟着设备文件所代表的设备的设备号。



#### jffs2_do_read_inode函数

这个函数在jffs2_read_inode被用来**与Flash设备交互**，即从Flash上读取数据并建立相应内存数据结构。

注意metadata指向的是full_dnode类型。

==jffs2中只有根目录一个文件没有jffs2_raw_inode数据实体==（哪怕对于新创建、且尚未写入任何数据的正规文件，也在创建当下写入了一个临时的jffs2_raw_inode，相应的jffs2_full_dnode则由metadata指向，并在稍后真正第一次写入数据时被标记为过时）

**latest_node**就是随便读个raw_inode数据实体出来返回给上层的jffs2_read_inode用于设置文件的inode数据结构。

1. 该函数已经有了一个文件的inode info（即u域），首先调用**jffs2_get_ino_cache**通过inode number获取inode cache，再调用**jffs2_get_inode_nodes**函数获取full_dirent的链表和tmp_dnode的链表。==注意tmp_dnode的链表其实也就是full_dnode的链表，tmp_dnode中的fn就是full_dnode==。

2. 接着该函数遍历tmp_dnode的链表，取出每个full_dnode，并根据其大小是否为0判定是否是metadata该指向（因为目录文件、SOCKET、FIFO文件的jffs2_raw_inode后没有数据，所以fn->size等于0），且根据其version判断其是否过期（obsolete），对于OK的full_dnode就会加入到数据的红黑树中，当然大小为0的点就会直接被metadata指向。

3. 接着就是将那些fn->size不为0，但是只有一个raw_inode文件的full_dnode改为由metadata直接指向。



#### jffs2_get_inode_nodes函数

该函数就是**制造full_dirent的链表和tmp_dnode的链表的函数**。顺着raw_node_ref链表（文件的jffs2_inode_cache的nodes域指向）。

1. 顺着raw_node_ref链表调用**jffs2_flash_read**函数从Flash中读取数据到jffs2_node_union结构体中。

   ```c++
   union jffs2_node_union {
       struct jffs2_raw_inode i;
       struct jffs2_raw_dirent d;
       struct jffs2_unknown_node u;
   };
   ```

   接着再根据读出的数据中的node type来判断是raw_dirent还是raw_inode再进行相应操作。

2. 如果是dirent，就接着从flash中读取文件名到full_dirent结构体中，并随后调用**jffs2_add_fd_to_list**函数将这个full_dirent加入到要制造的full_dirent的链表中。

3. 如果是inode，因为之前挂载时没有对raw_inode进行check数据，此时打开文件就要进行check。就从flash中读取紧跟着raw_inode的data计算crc进行check，check通过后该文件的内核描述符就从REF_UNCHECKED变为REF_NORMAL。之后再创建tmp_dnode和full_dnode，再调用**jffs2_add_tn_to_list**函数将tmp_dnode加入到正在制造的tmp_dnode链表中即可。



### 第6章 jffs2中写正规文件的方法

当用户进程访问文件时方法表jffs2_file_operation（位于inode的i_fop域）中的相应函数会被调用，而它又会进一步调用address_space_operations方法表中的相关函数来完成与flash交换数据实体的操作。



#### sys_write函数

1. 函数接收文件句柄fd（int类型），调用fget函数通过fd得到文件的file结构体。
2. 调用file->f_op中的write函数执行写入操作。

==在进行写操作前首先要由locks_verify_area检查在待写入的区域上没有已存在的写强制锁。（这也就是“强制”锁名称的来历了：任何写入操作都会执行检查）如果通过了检查，则调用file->f_op所指方法表中的write方法==。

注意在fget中会用inode对应的inode.i_fop设置file.f_op。因此这里的write就是jffs2_file_operation方法表中的generic_file_wirte。

写入完毕后，还将调用fput(file)减少文件file对象的引用计数。



#### generic_file_write函数

1. 首先就是对要写的文件、写入的位置pos、写入的数据量进行各种安全检查。

2. 接着更新inode中的两个时间戳，并用**mark_inode_dirty_sync**函数将其标记为“脏”，以后它就会被写回到设备索引节点了。

3. 如果是O_DIRECT类型的文件，则调用**generic_file_direct_IO**函数进行处理。

4. ==将文件看作是一页页组成的==。通过pos计算出要写的页框的页号index，以及页内偏移offset。调用**__grab_cache_page**函数返回该页面在内核页高速缓存中对应的物理页框。

   > 内核页高速缓存中的所有物理页框的指针被组织在哈希表page_hash_table中，由于“页面在文件内的偏移”在系统内显然不唯一，所以在计算散列值时要使用文件的address_space结构的指针mapping（将该指针值当作无符号长整型来使用）
   >
   
   在本函数起始位置处已经对文件进行了lock，在循环每页写入的时候对一页进行lock。
   
5. 调用**kmap**(page)函数返回相应页框的内核虚拟地址kaddr。

6. 接着调用address_space_operation中的**prepare_write**函数为写入做如下准备：

   > 如果页框不是“Uptodate”的话就得首先从设备上读出相应的页面。
   >
   > 另外如果page页框在文件内的起始大于文件大小，则本次循环将在文件中==**造成一个洞（hole）**==，而且后面的写操作并不会描述这个洞，所以得向flash写入一个jffs2_raw_node数据实体来描述它。

7. 用**__copy_from_user**函数从用户进程空间中读取buf缓冲区的bytes个字节到该页框内offset偏移处。

8. 调用address_space_operation方法表的**commit_write**函数（即**jffs2_commit_write**函数）完成将高速缓存中的页框写入到Flash设备。写入完成后再更新一些数值属性。

9. 写入完一页后还记得unlockpage，page_cache_release等一系列操作。

   这么循环直到所有写入的数据量都写入完毕。



#### jffs2_prepare_write函数

P98