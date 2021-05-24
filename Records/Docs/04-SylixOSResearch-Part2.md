# 第三周记录

## Linux中的VFS

VFS（Virtual Filesystem Switch）称为虚拟文件系统或虚拟文件系统转换，是一个内核软件层，在具体的文件系统之上抽象的一层，用来处理与Posix文件系统相关的所有调用，表现为能够**给各种文件系统提供一个通用的接口，使上层的应用程序能够使用通用的接口访问不同文件系统，同时也为不同文件系统的通信提供了媒介**。

### 统一文件模型

其实VFS中虚拟出的文件系统就和我们在OS课中学的EXT2的结构相同，

- superblock：存储文件系统基本的元数据。如文件系统类型、大小、状态，以及其他元数据相关的信息（元元数据）

- index node（inode）：保存一个文件相关的元数据。包括文件的所有者（用户、组）、访问时间、文件类型等，但不包括这个文件的名称。文件和目录均有具体的inode对应
- directory entry（dentry）：保存了文件（目录）名称和具体的inode的对应关系，用来粘合二者，同时可以实现目录与其包含的文件之间的映射关系。另外也作为缓存的对象，缓存最近最常访问的文件或目录，提示系统性能
- file：一组逻辑上相关联的数据，被一个进程打开并关联使用

#### SuperBlock

静态：superblock保存了一个文件系统的最基础的元信息，一般都保存在底层存储设备的开头；

动态：挂载之后会读取文件系统的superblock并常驻内存，部分字段是动态创建时设置的。

super_block结构体中重要字段：

```c
struct list_head s_list;
struct file_system_type *s_type;
const struct super_operations *s_ops;
struct dentry *s_root;
struct block_device *b_bdev;
struct list_head s_inodes;
struct list_head s_dirty;
struct list_head s_io;
struct list_head s_more_io;
struct list_head s_files;
...
```

由于Linux系统支持同时挂载多个文件系统，因此s_list字段用于在内存中构建superblock链表来支持挂载多个文件系统。s_root字段标识该文件系统的根目录，s_bdev标识该文件系统所在的设备信息。其中最重要的字段是s_op，这个指针指向该文件系统所支持的各种操作的结构体，称为“super_operations”，具体定义如下：

```c
struct super_operations {
    struct inode *(*alloc_inode)(struct super_block *sb);
    void (*destroy_inode)(struct inode *);
    void (*dirty_inode) (struct inode *);
    int (*write_inode) (struct inode *, int);
    void (*drop_inode) (struct inode *);
    void (*delete_inode) (struct inode *);
    ...
}
```

结构体的每个成员都代表这个操作具体应该执行的底层操作。例如需要写入数据时，VFS会通过superblock中的s_op字段最终去调用write_super来执行文件系统的具体操作：sb->s_op->write_super(sb)。所有这些调用都由VFS完成，向上对接了操作系统的sys_write系统调用，向下转交到具体文件系统的底层操作。

每一个文件系统使用前必须进行挂载（mount），superblock包含的s_type字段定义了这个文件系统的类型。通过系统调用register_filesystem()、unregister_filesystem()可以实现对具体文件系统的挂载和卸载，它们都只有一个参数，就是文件系统类型“ file_system_type”。本质上就是告诉操作系统挂载的文件系统信息。对于2.6.18之后的内核版本，该结构体定义如下：

```c
truct file_system_type {
    const char *name; //文件系统类型名称
    int fs_flags;
    struct super_block *(*get_sb)(struct file_system_type *,
    int, char *, void *, struct vfsmount *); //挂载文件系统时由kernel调用，用于创建内存中的    superblock
    void (*kill_sb) (struct super_block *); //卸载文件系统是由kernel调用，用于移除内存中的        superblock
    struct module *owner;
    struct file_system_type *next;
    struct list_head fs_supers;
    struct lock_class_key s_lock_key;
    struct lock_class_key s_umount_key;
};
```

get_sb的最后一个参数是vfsmount类型，这是系统用来记录挂载信息的数据结构。它保存了挂载点、设备、挂载选项等信息。对于每个一个打开的进程来说，都会在其内核部分维护两个数据结构：fs_struct和file；分别用来描述关联的文件系统信息和打开的文件信息。

#### Inode，Dirent，File

这三个数据结构都类似，其中都包含了一个xxx_operations的结构体，里面存放着各种必要操作的函数指针，以此来实现通用接口。



## Linux中的文件系统挂载

以linux3.2.0, ext4文件系统为例，当执行挂载命令时，

### 调用关系

> SYSCALL_DEFINE5 
>    | 
>    |----do_mount 
>        | 
>        |----do_new_mount 
>             |----do_kern_mount 
>             |     | 
>             |     |----vfs_kern_mount 
>             |            | 
>             |            |----mount_fs 
>             |                | 
>             |                |----type->mount(ext4_mount) 
>             |----do_add_mount

### 1 do_mount 

当执行mount -t ext4..挂载命令时, 会执行此函数；
因为mount命令有很多的形式，比如remount的等等，因此,do_mount会对这些情况作区分，当然如果紧紧是挂载，那么会执行do_new_mount的分支；

### 2 do_new_mount

定义一个 vfsmount结构的变量mnt；
调用vfs_kern_mount返回挂载后的mnt: mnt = do_kern_mount；
do_add_mount将mnt链到内核维护的vfsmount结构中，do_add_mount(mnt, path, mnt_flags)这个函数参数mnt为vfsmount结构, 其中mnt与挂载的根dentry：root已经建立的联系,path是挂载的路径名称。这个函数是将挂载的路径名称与mnt建立了联系。

**举个例子**：假设 /home/abc/1.txt 这里abc目录是一个挂载点,在打开1.txt的过程中获取到了abc目录的inode，通过inode发现是个挂载点(s_mount==1)， 那么内核会在维护vfsmount的hash里去找，会根据路径"/home/abc"去找对应的vfsmnount结构mnt,找到后，获取vfsmount中的dentry，即mnt->mnt_root，即挂载点的entry（挂载的文件系统的根目录），之后找到对应的inode,即dentry->d_inode，之后找到对应的1.txt文件。

### 3 do_kern_mount

首先通过struct file_system_type *type = get_fs_type(fstype); 获取type类型,这个操作是根据什么来的呢？ 
**①在super.c文件中的最后** 
module_init(ext4_init_fs) 
module_exit(ext4_exit_fs) 
所以在系统刚启动时，会调用ext4_init_fs 
**②在ext4_init_fs中会调用函数register_filesystem(&ext4_fs_type);** 
对ext4_fs_type进行注册，会将ext4_fs_type中的.name="ext4"注册到内核的全局变量file_systems 
中 
**③ext4_fs_type** 
ext4_fs_type是提前定义好的 

```c
static struct file_system_type ext4_fs_type = { 
    .owner  = THIS_MODULE, 
    .name = "ext4", 
    .mount  = ext4_mount, 
    .kill_sb  = kill_block_super, 
    .fs_flags  = FS_REQUIRES_DEV, 
}; 
```


所以，get_fs_type(fstype);函数中 fstype传入的是"ext4"，函数会在file_systems中寻找"ext4"对应的fs_type即 ext4_fs_type,之后调用vfs_kern_mount。

### 4 vfs_kern_mount

**①首先定义两个变量** 
struct vfsmount *mnt; //完成挂载后的mnt 
struct dentry *root;  //完成挂载后的root dentry 
**②malloc出一个mnt,根据名字来分配** 
mnt = alloc_vfsmnt(name); 其中name是设备名,比如在/dev/sdaxx的mem_cache中分配一个地方 
**③获取根dentry, root = mount_fs(type, flags, name, data);** 
**④建立 mnt与root的关系** 
mnt->mnt_root = root; 
mnt->mnt_sb = root->d_sb; 
mnt->mnt_mountpoint = mnt->mnt_root; 
mnt->mnt_parent = mnt; //在do_add_mount会对mnt_parent进行更改 

### 5 mount_fs

调用type->mount,其中type是do_kern_mount传下来的，即ext4_fs_type。ext4_fs_type中的mount就是ext4_mount。



### 6 ext4_mount

> 对于挂载来说, 要研究一下ext4_mount这个函数,VFS比较重要的地方都在这里了

#### 调用关系

>  ext4_mount 
>    | 
>    |----mount_bdev 
>        | 
>        |----ext4_fill_super 
>             | 
>             |----ext4_iget 
>             | 
>             |----d_alloc_root 
>                  | 
>                  |----__d_alloc 
>                  |----d_instantiate 

#### 1 ext4_fill_super

首先获取了超级快结构sb 
接着调用root = ext4_iget(sb, EXT4_ROOT_INO);获取根"/"的inode 

#### 2 ext4_iget 
从磁盘获取root的inode后(因为"/"是目录，即对inode的i_op进行赋值) 
inode->i_op = &ext4_dir_inode_operations; 
inode->i_fop = &ext4_dir_operations; 这里便是VFS重点所在 
举个例子: 当我们把ext4类型的磁盘设备挂载到根目录"/"下，由于挂载时将inode赋值成ext4类型 
在"/"目录下创建文件1.txt 
那么"/"所对饮inode,即调用inode->i_op->ext4_create去创建文件 
ext4_create函数会先创建1.txt对应的inode 
inode = ext4_new_inode(handle, dir, mode, &dentry->d_name, 0, NULL); 
然后对inode的i_op赋值成ext4类型 
inode->i_op = &ext4_file_inode_operations; 
inode->i_fop = &ext4_file_operations; 

#### 3 d_alloc_root 
**创建一个dentry**名字为"/" 
将inode与entry建立联系

dentry->d_inode = inode;

### 实例：实现不同文件系统之间的COPY

要把 /dnw/1.txt 拷贝到 /abc下 
1.首先要明确/ 与/abc是两个挂载点 
ext4磁盘挂载到了/下 ，所以挂载后/的inode操作对应的是ext4的操作 
fat32磁盘挂载到了 /abc下 ,我们知道abc文件夹的生成时通过"/"的ext4_create创建的 
挂载了fat32后, 在abc inode下的s_mount会被置1（表明是个挂载点inode）, 等目录搜索到/abc时会找到vfsmountB 
~~从而得到dentry->d_inode，即找到在/abc在挂载的"/"的inode，于是换成此"/"目录的inode操作函数,即fat32_create等去操作文件~~

应该是从vfsmount->mnt_root得到根目录的dentry，再dentry->d_inode得到在fat32中的inode？



## Linux中的ramFs

Ramfs顾名思义是内存文档系统，他工作于虚拟文档系统（VFS）层。不能格式化，能够创建多个，在创建时能够指定其最大能使用的内存大小。假如您的Linux已将Ramfs编译进内核，您就能够很容易地使用Ramfs了。创建一个目录，加载Ramfs到该目录即可。

```c
# mkdir -p /RAM1
# mount -t ramfs none /RAM1
```

RAMFS是一个非常巧妙的，利用**VFS自身结构**而形成的内存文件系统。RAMFS没有自已的文件存储结构,它的文件存储于page cache中，目录结构由dentry链表本身描述，文件则由VFS的inode结构本身描述。

**常用文件类型定义**：

```c
#define         S_IFMT  0170000 /* type of file ，文件类型掩码*/
#define         S_IFREG 0100000 /* regular 普通文件*/
#define         S_IFBLK 0060000 /* block special 块设备文件*/
#define         S_IFDIR 0040000 /* directory 目录文件*/
#define         S_IFCHR 0020000 /* character special 字符设备文件*/
#define         S_IFIFO 0010000 /* fifo */
#define         S_IFNAM 0050000 /* special named file */
```

**ramfs对应的底层操作函数**：

1)mmu

- 简单处理

2)nommu

- 内存映射转换

**初始化**：

- struct backing_dev_info ramfs_backing_dev_info，cache的预读设置

- bdi_init()，继续设置 ramfs_backing_dev_info

- register_filesystem()，注册rootfs_fs_type

**分配新文件节点**：

- ramfs_mknod()，建立新节点调用这个函数
- ramfs_mkdir()，内部调用ramfs_mknod()
- ramfs_create()，建立 S_IFREG 类型节点
- ramfs_symlink()，建立 S_IFLNK|S_IRWXUGO 类型节点
- 新节点默认给予755权限
- 当使用new_inode(sb)时，sb指针将该fs实例的相关信息全部带入，在此基础上分配新inode。

#### struct backing_dev_info

> 这个结构体又涉及到Linux中一大知识点：writeback，如果这周有空的话再进一步研究

系统中每个设备均对应这样一个结构体，该结构体最初是为了设备预读而设计的，但内核后来对其扩充，增加了设备回写相关的成员变量。与设备脏文件回写的相关成员如下列举：

```c
struct backing_dev_info{
	struct list_head bdi_list;
    ...

    struct bdi_writeback wb; 
    spinlock_t wb_lock;  
    struct list_head work_list;
    ...
};
```

系统提供bdi_list将所有设备的bdi（backing_dev_info）结构串联成链表，便于统一管理；wb是该设备对应的回写线程的数据结构，会在下面仔细描述；work_list是设备上所有任务的链表，发起回写的调用者只是构造一个回写任务挂入该链表即可；wb_lock是保护任务链表的锁。



## Sylix中的ramFs

#### RAM_NODE结构

```c
typedef struct ramfs_node {
    LW_LIST_LINE        RAMN_lineBrother;                               /*  兄弟节点链表                */
    struct ramfs_node  *RAMN_pramnFather;                               /*  父亲指针                    */
    PLW_LIST_LINE       RAMN_plineSon;                                  /*  子节点链表                  */
    PRAM_VOLUME         RAMN_pramfs;                                    /*  文件系统                    */
    
    BOOL                RAMN_bChanged;                                  /*  文件内容是否更改            */
    mode_t              RAMN_mode;                                      /*  文件 mode                   */
    time_t              RAMN_timeCreate;                                /*  创建时间                    */
    time_t              RAMN_timeAccess;                                /*  最后访问时间                */
    time_t              RAMN_timeChange;                                /*  最后修改时间                */
    
    size_t              RAMN_stSize;                                    /*  当前文件大小 (可能大于缓冲) */
    size_t              RAMN_stVSize;                                   /*  lseek 出的虚拟大小          */
    
    uid_t               RAMN_uid;                                       /*  用户 id                     */
    gid_t               RAMN_gid;                                       /*  组   id                     */
    PCHAR               RAMN_pcName;                                    /*  文件名称                    */
    PCHAR               RAMN_pcLink;                                    /*  链接目标                    */
    
    PLW_LIST_LINE       RAMN_plineBStart;                               /*  文件头                      */
    PLW_LIST_LINE       RAMN_plineBEnd;                                 /*  文件尾                      */
    ULONG               RAMN_ulCnt;                                     /*  文件数据块数量              */
    
    PRAM_BUFFER         RAMN_prambCookie;                               /*  文件 cookie                 */
    ULONG               RAMN_ulCookie;                                  /*  文件 cookie 下标            */
} RAM_NODE;
typedef RAM_NODE       *PRAM_NODE;
```



#### _LIST_ENTRY函数

_LIST_ENTRY就是container_of；

#### API_RamFsDrvInstall函数

在系统中安装注册驱动程序，_G_iRamfsDrvNum是安装好驱动程序后系统返回的**驱动程序号**；如果_G_iRamfsDrvNum小于0就代表驱动程序还未安装成功；

用__fsRegister函数向系统中注册文件系统，就是告知系统"ramfs"文件系统所要使用的**用于添加device**的函数是API_RamFsDevCreate；

#### API_RamFsDevCreate函数

一个Dev对应一个RAM_VOLUEME，该函数就是在创建并设置一个RAM_VOLUME，再调用iosDevAddEx将该设备注册到系统。其中ram_mount应该就是在设置RAM_VOLUME其余属性，但目前ram_mount为空；

传入设备名pcName和代表块设备的结构体pblkd，pblkd中BLKD_pcName如果是数字的话就是设置的RamFs最大内存使用量。在RAM_VOLUME中RamFs最大内存使用量以块block为单位，一个block是2KB（2的11次方）。

**API_IosDevAddEx**中4个参数分别是：设备头header、设备名、设备驱动程序号、设备的data_type；在sylix中通过设备头的链表来管理所有设备，而在添加RamFs设备时data_type为4，猜测是指RamFs设备的字长为4字节？

#### API_RamFsDevDelete函数

该函数就是按照已添加的RamFs设备的名称删除该设备。首先通过调用API_IosDevMatchFull函数在系统中找到该设备的设备头，如果是设备那么MatchFull函数就会返回一个设备头的指针，接着调用unlink函数将RamFs设备的link数减1（因为设备也被看作文件，如果其link数降低为0后就自然会在系统中被删除）



> 以下先介绍ramFsLib.c中的函数，这部分函数是ramFs在实现用户接口函数时所要调用的内部函数。

#### __ram_open函数

```c
PRAM_NODE  __ram_open (PRAM_VOLUME  pramfs,
                       CPCHAR       pcName,
                       PRAM_NODE   *ppramnFather,
                       BOOL        *pbRoot,
                       BOOL        *pbLast,
                       PCHAR       *ppcTail)
```

其中pramfs就是一个ramfs设备，pcName是路径字符串，剩下四个参数都是要返回的值，ppramnFather是找到的文件的上一级文件的node，如果找不到要找的文件则这个Father就是最后一个途径的文件的node，pbRoot是代表找的文件是否是根目录，pbLast是代表在open失败时是否是因为最后一级文件名出错，ppcTail是如果存在连接文件, 指向连接文件名后的路径(字符串)。

对于pcName="/A/B/C"，函数首先会去掉第一个'/'，即pcTempName="A/B/C"，初始化pcNext=pcTempName。接着取出pcNode="A"，pcNext="B/C"，然后就在根目录的文件中找A（注意同一级的文件是用一个链表连接起来的，因此在文件夹中查找也是顺序查找），找到A后又从A的node中的RAMN_plineSon得到下一级文件node组成的链表以此重复...

注意迭代是所用的指针其实指向的并不是node的首地址，而是node中RAMN_lineBrother成员的地址，通过调用_LIST_ENTRY函数即可通过这个成员的地址得到node的首地址。

#### __ram_maken函数

```c
PRAM_NODE  __ram_maken (PRAM_VOLUME  pramfs,
                        CPCHAR       pcName,
                        PRAM_NODE    pramnFather,
                        mode_t       mode,
                        CPCHAR       pcLink)
```

maken是make node的意思，在pramfs中pramnFather文件夹下创建一个pcName文件。基本上代码就是创建一个RAM_NODE然后再填充相应属性，如果是Link文件再把pcLink参数复制进该node中的RAMN_pcLink中去。创建完node后，再调用List_Line_Add_Ahead函数将新创建的node中的lineBrother成员的地址添加到上一级的plineSon链表的头部即可。

#### __ram_unlink函数

```c
INT  __ram_unlink (PRAM_NODE  pramn)
```

该函数首先判断pramn是否是目录，如果是目录且其中的plineSon(目录中包含的文件)非空，则报错。

在ramFs中没有为每个文件设计link数，因此只要某个文件被调用unlink就会直接被删除，删除时首先取出parmn的父结点和所属ramfs，如果父结点不为空，就从父结点的plineSon中删除本结点，否则（说明是根目录文件）从ramfs的plineSon中删除本节点。之后再循环删除pramn->RAMN_plineBStart，即删除数据块（先List_Line_Del从链表中删除一个数据块的地址，再调用__RAM_BFREE释放该数据块的空间。最后如果是link文件还会释放RAMN_pcLink所占的空间。

#### __ram_truncate函数

```c
VOID  __ram_truncate (PRAM_NODE  pramn, size_t  stOft)
```

说实话感觉这个函数有问题。

```c
for (plineTemp  = pramn->RAMN_plineBStart;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {
        
        if (stOft <= stTemp) {                                          /*  从此块开始删除              */
            break;
        } else {
            stTemp += __RAM_BDATASIZE;                                  /*  文件内容指针向前移动        */
        }
    }
```

该函数就是从parmn文件的第st0ft字节开始截断后面所有数据，函数就是从相对应的块，顺着RAMN_plineBStart数据块链表依次删除相应数据块，每次记得文件占用块大小RAMN_ulCnt减1，同时整个ramFs占用内存的块大小RAMFS_ulCurBlk也减1。

#### __ram_automem函数

```c
static INT  __ram_automem (PRAM_NODE  pramn, ULONG  ulNBlk, size_t  stStart, size_t  stLen)
```

这个函数就是将给定的文件pramn占用的空间扩大到ulNBlk个块，且在[stStart,stStart+stLen]字节范围内的数据不清零。这个函数不能缩小文件的大小（会报错）。

在ramFs在每个文件的数据块结构体是RAM_BUFFER。函数具体过程就是计算要新建几个块，然后再依次RAM_BALLOC每个RAM_BUFFER，再将新RAM_BUFFER的地址加入到文件node的plineBStart链表中。

**ramFs中的每个文件的大小叫做文件缓冲区大小**，因为用到ramFs的每个文件其实都是磁盘文件因为某种需求缓存到内存中来。

#### __ram_increase函数

```c
VOID  __ram_increase (PRAM_NODE  pramn, size_t  stNewSize)
```

就是将pramn中的RAMN_stSize增大到stNewSize(不能缩小)，同时RAMN_stVSize也增大到stNewSize(不能缩小)

#### __ram_getbuf函数

```c
static PRAM_BUFFER  __ram_getbuf (PRAM_NODE  pramn, size_t  stOft, size_t  *pstBlkOft)
```

**首先说明，每个文件node中的prambCookie是该文件最近一次被访问的数据块的指针，而ulCookie是该数据块的在文件的下标(例如块大小为10B，则第32字节，就在ulCookie=3的数据块中)**。

该函数就是从文件pramn中取出第stOft字节所在的块的块指针PRAM_BUFFER，并且返回第stOft字节数据在该块的块内偏移（就是取块大小的余数）。

函数具体过程就是首先计算该字节的数据所在块的块下标ulBlkIndex以及块内偏移pstBlkOft，然后再判断是否是Cookie块，然后再判断从plineBStart指针开始寻找比较好还是从plineBEnd指针开始寻找比较好，接着调用list_line_get_next(或list_line_get_prev)函数，去取得这块数据块的指针。

#### __ram_getbuf_next函数

```c
static PRAM_BUFFER  __ram_getbuf_next (PRAM_NODE  pramn)
```

这个函数就是从pramn的Cookie数据块得到它的下一块，并将下一块作为新的Cookie块，返回这个新的Cookie块的指针。（回顾：Cookie块就是该文件的最近一次被访问的数据块）

#### __ram_read函数

```c
ssize_t  __ram_read (PRAM_NODE  pramn, PVOID  pvBuffer, size_t  stSize, size_t  stOft)
```

该函数就是从文件node pramn中从第stOft字节开始读取数据到pvBuffer中即可，返回读取数据的总字节数，其中stSize是pvBuffer的最大大小，而pramn中数据总字节数由RAMN_stVSize规定（**暂时还不懂RAMN_stSize和RAMN_stVSize的区别**）。

函数具体过程就是首先通过上述函数ram_getbuf得到起始字节所在块指针，以及块内偏移，然后通过lib_memcpy复制到pvBuffer中，之后再通过ram_getbuf_next函数得到下一块，以此循环下去...

#### __ram_write函数

```c
ssize_t  __ram_write (PRAM_NODE  pramn, CPVOID  pvBuffer, size_t  stNBytes, size_t  stOft)
```

该函数就是将pvBuffer中的数据写入到文件pramn的第stOft字节处，stNBytes是pvBuffer的大小。

函数具体过程就是首先计算文件经过write后所占块数ulNBlk，再调用ram_automem函数将文件的数据块扩展到ulNBlk个，接着就是调用ram_getbuf获取第stOft字节所在块的指针pramb，再通过libmemcpy将pvBuffer中的数据写入到文件的数据块，写完一块后通过ram_getbuf_next获取下一块。函数最后返回写入的数据大小stWrite。

写完后将RAMN_stSize和RAMN_stVSize增大为stOft+stWrite（如果stSize和stVSize大于stOft+stWrite则不变，意思就是不会缩小）

#### __ram_mount函数

空函数，原本的意思是将ramfs挂载。这里应该是由于ramFs不需要mount。（见下面unmount函数）

#### __ram_unlink_dir函数

```c
static VOID  __ram_unlink_dir (LW_LIST_LINE_HEADER plineDir)
```

注释说这个函数是删除一个目录dir，其实只是删除一个目录下面的文件，并没删除目录自身。

函数接受的plineDir是一个由指针组成的链表，上面是每个下级文件的node的成员变量RAMN_lineBrother的地址，我们取到每个指针后通过LIST_ENTRY函数即可得到相对应文件的node的指针（回顾：LIST_ENTRY就是container_of函数）。取到文件node的指针后，判断该文件是否也是个目录，若是目录则递归调用ram_unlink_dir函数先删除这个目录下级所有文件，之后再调用ram_unlink函数删除该目录文件。

在删除掉一个文件后，调用list_line_get_next函数即可得到下一个文件的node指针，以此循环下去直到该目录中的所有文件都被删除。

#### __ram_unmount函数

```c
VOID  __ram_unmount (PRAM_VOLUME  pramfs)
```

该函数就是调用上面的ram_unlink_dir函数删除pramfs->RAMFS_plineSon，即把根目录中所有文件都删除了就算是unmount了ramFs。

#### __ram_close函数

```c
VOID  __ram_close (PRAM_NODE  pramn, INT  iFlag)
```

这个函数意思是关闭文件pramn，但实际上做的工作就是改一下文件pramn中的最后访问时间。如果该文件是以可写方式打开的，则还会改一下文件pramn中的最后修改时间。（就是改两个时间）

iFlag是打开文件时用的flag。

#### __ram_move_check函数

```c
static INT  __ram_move_check (PRAM_NODE  pramn1, PRAM_NODE  pramn2)
```

检查pramn2是否是pramn1的后代，如果是返回-1，否则返回0（很奇怪这个返回值）

函数过程就是不断pramn2=pramn2->RAMN_pramnFather，直到pramn2=pramn1或pramn2为空。

#### __ram_move函数

```c
INT  __ram_move (PRAM_NODE  pramn, PCHAR  pcNewName)
```

这个函数就是将文件的node即pramn换个位置，换到路径为pcNewName上。pcNewName是绝对路径（因为该参数直接传给了ram_open）。

函数过程首先调用ram_open函数尝试打开在pcNewName上现存的文件pramnTemp，

如果pcNewName路径上的文件已存在（pramnTemp指针非空）：

> 如果pramnTemp和pramn相同，则说明不需要移动，返回0代表move操作已成功；
>
> 如果pramnTemp和pramn不相同，则需要再判断pramnTemp和pramn的类型是否兼容，（要求文件代替文件，目录代替目录），如果兼容，则把原有的文件pramnTemp通过ram_unlink函数删除掉；如果不兼容直接return ERROR；

open函数会返回新路径的文件的父目录pramnNewFather，以及bRoot代表pcNewName是否是根目录，bLast代表是否是最后一级名称的文件找不到。

正常情况应该是最后一级文件找不到（因为move函数正要创建这个文件），因此bLast会为1。

首先调用ram_move_check函数判断pramnNewFather是否是现在要移动文件pramn的后代，如果是则移动后会产生：目录A->目录B->目录A->...这样的无穷结构，因此不合法会报错。

接着判断新路径的父目录pramnNewFather和现有文件的父目录pramnFather是否相同：

> 如果pramnNewFather和pramnFather相同，代表是在同一个文件夹下，因此只需将现有文件改个名字即可；
>
> 如果pramnNewFather和pramnFather不同，代表移动到了不同的文件夹下，需要调用相应函数从原来父目录的plineSon链表中删除，并将自己lineBrother成员的地址添加到新文件夹的plineSon链表中。

最后将pramn中的文件名RAMN_pcName改为新文件名pcTemp即可。pcTemp是从绝对路径pcNewName分离得最后一个/后面的字符串，例如"/A/B/C.txt"中的pcTemp就是"C.txt"。

#### __ram_stat函数

```c
VOID  __ram_stat (PRAM_NODE  pramn, PRAM_VOLUME  pramfs, struct stat  *pstat)
```

和xv6中的stat函数类似，就是从pramn或pramfs中读取相应数据到pstat。

这里读取pramfs时，差不多就是读取根目录的文件信息。

**注意在注册ramFs设备时，pramfs的mode是"dir"和默认"dir"**。

#### __ram_statfs函数

```c
VOID  __ram_statfs (PRAM_VOLUME  pramfs, struct statfs  *pstatfs)
```

和ram_stat函数类似，读取**文件系统**的相关信息到pstatfs。





### ramFs.c

> 以下继续介绍ramFs.c中的函数，这部分函数就是ramFs文件系统在实现对外接口了，在API_RamFsDrvInstall中已将这些函数作为驱动程序注册到了系统中

#### __ramFsOpen函数

```c
static LONG __ramFsOpen (PRAM_VOLUME     pramfs,
                         PCHAR           pcName,
                         INT             iFlags,
                         INT             iMode)
```

iMode真的不如叫iType，表明文件的类型（设备文件，目录文件，普通文件...）

函数具体过程：

首先是各种错误检查，比如文件名称合法性，以及文件类型合法性，**注意ramFs不支持创建FIFO、BLK、CHR类型的文件**。然后将pramfs锁住，进行互斥访问。

再调用ram_open尝试打开该路径，以获得该文件的node，即pramn。然后又是一顿检查，例如根据iFlags判断是否有错。

如果pramn为空且bLast为1（代表是最后一级文件找不到），同时iFlags中有O_CREAT则会调用ram_maken创建新文件。

**重点要说明的是LINK文件**，对于Link文件会首先进行地址的转换，通过_PathBuildLink函数，例如：

> '/a/b/c'中，如果'/a/b'是link文件，则'/a'是前缀prefix，'/c'是后缀tail，而如果'/a/b'中存储的链接目标文件的地址是绝对路径例如'/x/y'则最终地址会被组合成'/x/y/c'；如果存储的链接目标文件的地址是相对路径'x/y'，则最终地址会被组合成'/a/x/y/c'。
>
> 本来pathBuildLink函数中还可传入设备名，但在ramFs使用时不传入设备名。

LINK文件还有点奇怪的是，LINK文件在PathBuildLink函数后，~~就直接return了~~（**是因为open函数不能打开link文件？**），而其余类型的文件则还会通过 API_IosFdNodeAdd函数去将该文件的文件句柄得到并加入到ramFs中的句柄链表，最后return也是文件句柄。在返回前会将ramFs中的文件打开个数**DEVHDR_atomicOpenNum**加1。

```c
return  ((LONG)pfdnode);
```



#### __ramFsRemove函数

```c
static INT  __ramFsRemove (PRAM_VOLUME   pramfs,
                           PCHAR         pcName)
```

remove函数过程：

- 首先是调用ram_open函数获取到要删除的文件的node，如果文件是LINK文件，则根据相应的路径报PX_ERROR或FOLLOW_LINK_TAIL的错误（**同样也是不能打开link文件？**），如果文件不是LINK文件就直接调用ram_unlink删除即可。

- 如果文件不存在且路径pcName是根目录，则说明系统想要删除整个ramFs，**这里就涉及文件系统的卸载**。ramFs的卸载首先是得到设备头中的DEVHDR_atomicOpenNum，即（文件的？）打开个数，如果在卸载ramFs时还有文件是打开着的，因此会根据ramFs是否允许强制删除而作出不同反应，如果不允许强制删除就return error，如果允许删除就先将ramFs的RAMFS_bvalid设置为false，然后再iosDevFileAbnormal将所有ramFs涉及的文件设置为abnormal，之后再跳转回刚开始进行ramFs卸载的地方以此循环，直到所有文件都关闭后再进一步卸载。

  等到所有文件都关闭后，先调用iosDevDelete将ramFs设备删除，接着__ram_unmount将ramFs文件系统解除（实际上就是删除ramFs占用的内存）即可。

- 如果文件不存在且路径pcName不是根目录，则说明remove的文件不存在，return error；

#### __ramFsClose函数

```c
static INT  __ramFsClose (PLW_FD_ENTRY    pfdentry)
```

在这里是第一次见**PLW_FD_ENTRY**结构体。**FD_ENTRY和FD_NODE的关系暂时还不清楚**。

FD_ENTRY中含有FD_NODE，文件描述符FD_DESC中含有FD_ENTRY，~~这三个是1:1:1的关系~~。

ramFsClose函数调用API_IosFdNodeDec函数将传入的文件的引用数减1，如果引用数为0则该fd被删除，API_IosFdNodeDec函数返回减1后的引用数。如果引用数减为0，ramFsClose还会调用ram_close函数（改两个时间而已）。接着还会将ramFs中设备头中的文件打开个数减1。

#### __ramFsRead函数

```c
static ssize_t  __ramFsRead (PLW_FD_ENTRY pfdentry,
                             PCHAR        pcBuffer,//读取到pcBuffer中,最多stMaxBytes
                             size_t       stMaxBytes)
```

函数首先从fd_entry中得到fd_node，再从fd_node（**不同的fs都应从fd_node中的FDNODE_pvFile变量得到相应文件的数据结构**）中得到该文件在ramFs文件系统中的node，即pramn。

接着调用ram_read函数即可，读取完后还需将fd_entry中的FDENTRY_oftPtr增加读取数据的长度，代表读指针移动。

#### __ramFsPRead函数

```c
static ssize_t  __ramFsPRead (PLW_FD_ENTRY pfdentry,
                              PCHAR        pcBuffer,
                              size_t       stMaxBytes,
                              off_t        oftPos)
```

pread函数比read函数多的功能就是**多带了一个offset**，即可以从指定字节处开始读。

该函数过程和ramFsRead函数相同。

#### __ramFsWrite函数

```c
static ssize_t  __ramFsWrite (PLW_FD_ENTRY  pfdentry,
                              PCHAR         pcBuffer,
                              size_t        stNBytes)
```

调用ram_write即可。ramFSPWrite函数也是**多带了一个offset**，略。

#### __ramFsNRead函数

```c
static INT  __ramFsNRead (PLW_FD_ENTRY  pfdentry, INT  *piNRead)
```

该函数读取文件指针偏移处到文件末尾的数据大小，写到piNRead中。

函数过程就是 pramn->RAMN_stSize - (size_t)pfdentry->FDENTRY_oftPtr；

**__ramFsNRead64函数**和该函数一模一样，只不过piNRead所指向的是一个64位的数据。



#### __ramFsRename函数

>  讲这个函数前要首先重点讲一下**API_IoFullFileNameGet**函数：

```c
INT  API_IoFullFileNameGet (CPCHAR         pcPathName,
                            PLW_DEV_HDR   *ppdevhdr,
                            PCHAR          pcFullFileName)
```

该函数的意思是，首先pcPathName可以是绝对地址也可以是相对地址，如果是相对地址该函数会首先将当前路径cmd添加（通过pathCat函数）到pcPathName前面形成绝对路径cFullPathName(不是pcFull)，这样无论怎样cFullPathName都可以假设为绝对路径。有了绝对路径后，再调用iosDevFind函数得到该绝对路径所属的设备的设备头ppdevhdr，以及在该设备内的设备内地址**pcTail**，之后便可以把pcTail复制到pcFullFileName中。

即最终ppdevhdr是路径所属设备，pcFullFileName是路径在该设备内的路径。

> 好接下来就可以回到__ramFsRename函数了

```c
static INT  __ramFsRename (PLW_FD_ENTRY  pfdentry, PCHAR  pcNewName)
```

首先该函数接受的pcNewName可以是绝对路径也可是相对路径，首先函数调用上述的**API_IoFullFileNameGet**函数得到**新文件路径所属的设备**pramfsNew，以及新文件路径在那个设备中的**设备内路径**cNewPath。之后判断新设备pramfsNew和旧设备是否相同，**因为ramFsRename函数只接受同一个设备内的重命名！**之后再调用ram_move函数即可将文件移动新位置cNewPath（设备内地址）。

#### __ramFsSeek函数

```c
static INT  __ramFsSeek (PLW_FD_ENTRY  pfdentry,
                         off_t         oftOffset)
```

该函数就是将pfdentry中的文件偏移量FDENTRY_oftPtr设置为oftOffset，同时如果FD_ENTRY中的FD_NODE中的stVSize小于oftOffset，则令其也等于oftOffset。**还是没弄懂stVSize和stSize是干嘛的**。

#### __ramFsWhere函数

```c
static INT  __ramFsWhere (PLW_FD_ENTRY  pfdentry, off_t  *poftPos)
```

就是将pfdentry中的文件偏移量FDENTRY_oftPtr写入poftPos。

#### __ramFsStat函数

```c
static INT  __ramFsStat (PLW_FD_ENTRY  pfdentry, struct stat *pstat)
```

就是调用ram_stat函数将文件信息读取到pstat。

#### __ramFsLStat函数

```c
static INT  __ramFsLStat (PRAM_VOLUME  pramfs, PCHAR  pcName, struct stat *pstat)
```

LStat函数和上面的Stat函数差不多，区别就是**LStat会先调用ram_open函数在该ramFs中找到pcName对应的文件node**，再根据文件node是否是根目录节点执行相应的ram_stat操作将相应的文件信息读取到pstat中。

#### __ramFsStatfs函数

```c
static INT  __ramFsStatfs (PLW_FD_ENTRY  pfdentry, struct statfs *pstatfs)
```

就是调用ram_statfs函数将文件系统信息读取到pstatfs。

#### __ramFsReadDir函数

```c
static INT  __ramFsReadDir (PLW_FD_ENTRY  pfdentry, DIR  *dir)
```

该函数将目录文件pfdentry中的相关信息读取到dir中，**DIR结构体是POSIX规范中目录的一般结构**。DIR结构体应该不是指一个目录，而是目录中的一个文件，因为DIR结构体中有变量dir_pos代表该文件是在其dir中的第几个文件，即在父文件夹的plineSon中的第几个。

这里要求pfdentry必须是DIR类型的文件，否则报错。且如果pfdentry为空，则默认是读取ramfs根目录的信息。

#### __ramFsTimeset函数

```c
static INT  __ramFsTimeset (PLW_FD_ENTRY  pfdentry, struct utimbuf  *utim)
```

utimbuf结构体也是POSIX规范中的，其中存放了两个时间，访问时间和修改时间。

该函数就是将pfdentry对应的文件最后访问时间和最后修改时间写入到utim中。当然如果pfdentry对应的文件node为空，则是代表根目录。

#### __ramFsTruncate函数

```c
static INT  __ramFsTruncate (PLW_FD_ENTRY  pfdentry, off_t  oftSize)
```

该函数就是判断pfdentry对应的文件大小RAMN_stSize如果小于oftSize就调用ram_increase增大文件大小，如果大于oftSize就调用ram_truncate减小文件大小。

#### __ramFsChmod函数

```c
static INT  __ramFsChmod (PLW_FD_ENTRY  pfdentry, INT  iMode)
```

回顾：之前说iMode就是文件类型的意思。**但其实并不准确，因为iMode只有0xf000字段代表文件类型，后面12位则是代表文件的其它属性**。

而这个函数就是负责改变pfdentry的mode的后12位为iMode的后12位。**不改变文件类型，只改变文件的其它属性**。

#### __ramFsChown函数

```c
static INT  __ramFsChown (PLW_FD_ENTRY  pfdentry, LW_IO_USR  *pusr)
```

chown就是change owner的意思。

该函数就是改变文件中的uid和gid。

#### __ramFsSymlink函数

```c
static INT  __ramFsSymlink (PRAM_VOLUME   pramfs,
                            PCHAR         pcName,
                            CPCHAR        pcLinkDst)
```

该函数就是将pcName上的文件创建为一个指向pcLinkDst文件的Symlink文件。

函数具体过程首先是尝试打开pcName对应的文件，如果此文件已经存在或者是ramFs的根目录就报错。

否则调用ram_maken函数创建symlink文件即可。

#### __ramFsReadlink函数

```c
static ssize_t __ramFsReadlink (PRAM_VOLUME   pramfs,
                                PCHAR         pcName,
                                PCHAR         pcLinkDst,
                                size_t        stMaxSize)
```

这个函数就是读取ramFs中名字为pcName的symlink文件的pcLink字符串到pcLinkDst。其中stMaxSize是pcLinkDst缓冲区的最大大小。函数返回读取的字符串长度。

#### __ramFsIoctl函数

```c
static INT  __ramFsIoctl (PLW_FD_ENTRY  pfdentry,
                          INT           iRequest,
                          LONG          lArg)
```

iRequest是操作命令的宏，**该函数就是根据iRequest去执行ramFs.c中的各种函数**，lArg是函数参数，可以是一些结构体的指针。

#### ramFs总结

ramFs中的文件结构很简单，就是FD_NODE，其中的数据块由双向链表组成，不能进行随机读取，只能顺着链表查找一块数据。

ramFs的目录文件结构也是FD_NODE，但其中文件项不是那种采用dir_entry的方式，而是将子文件FD_NODE的指针用双向链表组成，即plineSon。

