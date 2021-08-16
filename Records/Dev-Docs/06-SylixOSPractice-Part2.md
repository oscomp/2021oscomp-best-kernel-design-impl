# 研读JFFS2源代码情景分析



## 第6章 jffs2中写正规文件的方法

当用户进程访问文件时方法表jffs2_file_operation（位于inode的i_fop域）中的相应函数会被调用，而它又会进一步调用address_space_operations方法表中的相关函数来完成与flash交换数据实体的操作。



### sys_write函数

1. 函数接收文件句柄fd（int类型），调用fget函数通过fd得到文件的file结构体。
2. 调用file->f_op中的write函数执行写入操作。

==在进行写操作前首先要由locks_verify_area检查在待写入的区域上没有已存在的写强制锁。（这也就是“强制”锁名称的来历了：任何写入操作都会执行检查）如果通过了检查，则调用file->f_op所指方法表中的write方法==。

注意在fget中会用inode对应的inode.i_fop设置file.f_op。因此这里的write就是jffs2_file_operation方法表中的generic_file_wirte。

写入完毕后，还将调用fput(file)减少文件file对象的引用计数。



### generic_file_write函数

1. 首先就是对要写的文件、写入的位置pos、写入的数据量进行各种安全检查。

2. 接着更新inode中的两个时间戳，并用**mark_inode_dirty_sync**函数将其标记为“脏”，以后它就会被写回到设备索引节点了。

3. 如果是O_DIRECT类型的文件，则调用**generic_file_direct_IO**函数进行处理。

4. ==将文件看作是一页页组成的==。通过pos计算出要写的页框的页号index，以及页内偏移offset。调用**__grab_cache_page**函数返回该页面在内核页高速缓存中对应的物理页框。

   > 内核页高速缓存中的所有物理页框的指针被组织在哈希表page_hash_table中，由于“页面在文件内的偏移”在系统内显然不唯一，所以在计算散列值时要使用文件的address_space结构的指针mapping（将该指针值当作无符号长整型来使用）

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



### jffs2_prepare_write函数

jffs2_prepare_write在每写一页时都要调用一次。

down(semaphore)是获取锁；up(semaphore)是归还锁；

底层的GC也在时不时地修改文件中的nodes链表或其他的属性，因此在prepare_write时就要注意避免竞争；

prepare_write可能写入代表空洞的数据实体，而commit_write则可能写入代表数据的数据实体；

- 先通过jffs2_reserve_space函数返回flash上一个合适的区间（为新创建的空洞实体jffs2_raw_inode预留一个合适的位置存放）

- 将新建的raw_inode（此时这个raw_inode结构体只存在于内存中）中的compr属性设置为JFFS2_COMPR_ZERO来表明这是个空洞；

- 调用**jffs2_write_dnode**函数将新建的raw_inode写到flash，并创建dnode作为返回值返回；

- 调用**jffs2_add_full_dnode_to_inode**函数将新的dnode加入到inode的链表中（红黑树中？对）

- 如果发现文件inode_info中的metadata是有效的话就将metadata指向的dnode所含的raw_inode标记为过时obsolete。

  > 在创建一个正规文件时、写入任何有意义的数据前，就首先向flash中写入了一个jffs2_raw_inode数据实体，其上层的jffs2_full_dnode则直接由jffs2_inode_info的metadata指向。而等到第一次真正写入有效数据时再将其标记为“过时”，而且以后的jffs2_full_dnode都组织在fragtree红黑树中。

- 如果发现page不是up to date的，就调用**==jff2_do_readpage_nolock==**函数进行处理；

注意洞依然会算文件的大小，但是不算写入数据的大小。

jffs2_complete_reservation函数用于向GC内核线程发送SIGHUP信号，使其唤醒。（一般在对Flash设备进行实际操作后就可以唤醒一次尝试垃圾回收）



### jffs2_commit_write函数

注意普通文件的红黑树是不会包含过期数据的。

这个函数中raw_inode是由**jffs2_alloc_raw_inode**函数获得的；（prepare_write中就是一个临时变量）

这个函数中raw_inode写入是由**jffs2_write_inode_range**函数进行的；（prepare_write中由**jffs2_write_dnode**函数进行，且其中Flash空间是由**jffs2_reserve_space**预留的），其实下面的jffs2_write_inode_range函数也是通过**jffs2_reserve_space**和**jffs2_write_dnode**实现的。



### jffs2_write_inode_range函数

这个函数负责将raw_inode写入到缓存页框中，之后再把这个页框写回flash设备，同时再创建相应的node_ref、full_dnode、node_frag等，并更新红黑树（要么让红黑树创建新节点，要么让之前的节点指向新的node_frag并标记以前的node_frag为过期；**貌似这里的节点意思是node_frag的上一级**）。

==**每个数据实体所携带的数据有长度限制**==。

==注意在调用commit_write之前要写入的数据已经通过copy_from_user函数从用户提供的数据中写到内存中的page了，而在commit_write中，以及在write_inode_range中只需要将这个page写到flash中即可==。

本函数大致流程：

- 由于每个数据实体携带的数据有长度限制，因此实际上这里还要分多次（分成多个raw_inode）写入flash；
- 每一次，先调用jffs2_reserve_space函数在flash上预订位置，得到物理位置phys_oft；预定位置不是你想预定多大就给你多大的位置，而是通过alloclen参数返回实际分配到的空间。
- 根据前面的alloclen以及剩下要写的数据量writelen来设置一个压缩数据的缓冲区comprbuf。
- 调用**jffs2_compress**函数在comprbuf中对要写入的数据buf进行压缩，契值datalen返回的是实际参与压缩的数据长度，而cdatalen返回的是压缩后的数据长度；

- 之后再补充一下raw_inode的属性，再调用**jffs2_write_dnode**将一个raw_inode以及其携带的压缩数据写入到之前预定的flash位置phys_oft上。这个函数还会同时分配内核描述符jffs2_raw_node_ref以及相应的jffs2_full_dnode，将前者加入文件的链表（inode_cache的nodes）并返回后者的地址。

- 之后再调用**jffs2_add_full_dnode_to_inode**函数将新dnode组织到文件的红黑树中。

  > jffs2_add_full_dnode_to_inode函数在文件的红黑树中查找相应数据实体的jffs2_node_frag数据结构，如果没找到，则创建新的并插入红黑树；如果找到，则将其改为指向新的jffs2_full_node，并递减原有jffs2_full_node的移用计数

- 如果metadata指针有效，还需记得将其设置为无效。

  > 当调用目录文件的create方法创建正规文件时，除了向父目录文件写入其目录项jffs2_raw_dirent数据实体外，还要向flash中写入“代表文件存在”的第一个jffs2_raw_inode数据实体。注意此时并没有需要写入的数据，所以这个jffs2_raw_inode数据实体的dsize域为0，而且其jffs2_full_dnode由jffs2_inode_info的metadata直接指向而没有组织到fragtree红黑树中，参见jffs2_do_create函数的相关部分。直到真正进行第一次写操作时将这个数据结点标记为过时的。



### jffs2_write_dnode函数

```c++
struct jffs2_full_dnode *jffs2_write_dnode(struct jffs2_sb_info *c, struct jffs2_inode_info *f, struct jffs2_raw_inode *ri, const unsigned char *data, uint32_t datalen, uint32_t flash_ofs, uint32_t *writelen)
```

该函数就是先将raw_inode写到flash设备的flash_ofs位置，再把data所指的长为datalen的数据写到raw_inode的后面。

jffs2_raw_node_ref是记录物理数据实体的，因此含有的是**在flash分区内**的逻辑偏移和整个数据实体的长度。

jffs2_full_dnode是用于描述后继数据在文件内的位置和长度，因此含有的是**在文件内**的逻辑偏移和长度。

本函数流程：

- 调用**jffs2_alloc_raw_node_ref**函数和**jffs2_alloc_full_dnode**函数申请新的内核描述符raw_node_ref和full_dnode。并为内核描述符设置相应的属性。
- 调用**jffs2_flash_writev**函数根据iovec（记录了要写入的数据的起始地址和数据长度的数据体）
- 写入完毕后就设置flash_offset的后两位为REF_NORMAL，如果是数据占满了一个PAGE，就设置为REF_PRISTINE。
- 将raw_node_ref加入到inode_cache的nodes中，再调用**jffs2_add_physical_node_ref**函数更新flash擦除块和文件系统内的相关统计信息。



## 第7章 jffs2中读正规文件的方法

读正规文件时函数调用路径如下：
sys_read > do_generic_file_read > jffs2_readpage > jffs2_do_readpage_unlock > jffs2_do_readpage_nolock



###  jffs2_do_readpage_unlock函数

前面几个函数差不多就只是调用下一级函数。

```c++
int jffs2_do_readpage_nolock (struct inode *inode, struct page *pg)
```

本函数流程：

- 调用kmap函数将page映射到linux内存虚拟空间。

  > 通过alloc_page函数得到的page本来是在linux的高端内存区（非直接映射区），必须通过kmap将其映射到内核地址空间。这部分知识属于拓展知识，有兴趣再[看](https://blog.csdn.net/gatieme/article/details/52705142)。

- 调用**jffs2_read_inode_range**函数将flash设备上的一段区间的数据读取到page对应映射的虚拟内存区域中。

注意：用完page后（kmap过的page）要调用kunmap解除内核页表中对相应页框的映射。



### jffs2_read_inode_range函数

```c++
int jffs2_read_inode_range(struct jffs2_sb_info *c, struct jffs2_inode_info *f, unsigned char *buf, uint32_t offset, uint32_t len)
```

该函数将文件f中第[offset, offset+len]字节范围的数据读取到页高速缓存buf中。

offset相当于是当前读取数据的指针。

本函数流程：（下面是一次循环的流程，每循环一次，处理一个数据实体）

-  调用**jffs2_lookup_node_frag**函数从文件f的红黑树中找到第offset字节数据所在的fragment；
- 调用==**jffs2_read_dnode**==函数从fragment中读取数据到buf中（用的是相对于这个数据实体的偏移量offset以及要读取的数据量readlen）。
- 读取完一个fragment后调用frag_next函数用于返回红黑树中“后继”数据实体的指针（得到逻辑上的下一个fragment）



### jffs2_read_dnode函数

```c++
int jffs2_read_dnode(struct jffs2_sb_info *c, struct jffs2_full_dnode *fd, unsigned char *buf, int ofs, int len)
```

从dnode的偏移量ofs开始读取len长的数据到buf中，其中ofs是dnode内的偏移。

==data_crc是从压缩后的数据算出来的crc==；

==解压操作必须先读取出全部的被压缩数据才行==；

流程：

- 调用jffs2_alloc_raw_inode创建raw_inode；
- 调用jffs2_flash_read从flash中读取数据到raw_inode中；
- 检查raw_inode；
- 如果是空洞则，则直接将buf中清0，直接返回即可；

- 创建read_buf（这个buf长度必为csize)；创建decomprbuf（这个buf长度必为dsize），之后再解压缩到decomprbuf中，再截取数据到上级buf中即可。



## 第8章 jffs2中符号链接文件的方法表（new）

```c++
struct inode_operations jffs2_symlink_inode_operations =
{
    .readlink = jffs2_readlink,
    .follow_link = jffs2_follow_link,
    .setattr = jffs2_setattr
};
```

在**path_walk**函数中逐层解析路径名时，如果当前路径名分量对应的方法表中的==follow_link指针不为空==，则通过**do_follow_link**函数调用符号链接文件的**follow_link**方法“跳转”到真正被链接的目标文件。

由于索引结点号只在当前文件系统内惟一，所以==硬链接的目标只能在同文件系统内==。与硬链接不同，符号链接的对象可以在其它文件系统中。这是因为符号链接文件的本质类似正规文件，==其数据即为被链接的文件名，那么从根文件系统的根目录出发总是可以到达被链接文件的==。

由于符号链接文件的目标可能在另外一个文件系统中，所以可想而知在==调用具体文件系统的follow_link方法跟踪符号链接文件时是一定会首先回到高层VFS的框架代码的==，然后从那里再通过**path_walk**进入其它的文件系统。在ext2中符号链接文件的数据，即被链接的文件名保存在ext2_inode的i_block[]数组中（和ext2_inode_info的i_data[]数组中），而jffs2中被链接文件名保存在其惟一的jffs2_raw_inode数据结点后。不同文件系统只需得到被链接文件名，然后就可以借助**vfs_follow_link**函数来实现各自的**follow_link**方法了。

> 重要的就是follow_link函数了

### jffs2_follow_link函数

```c++
int jffs2_follow_link(struct dentry *dentry, struct nameidata *nd)
```

函数流程：

- 调用**jffs2_getlink**函数得到dentry所指的inode的文件名（绝对路径）
- 调用**vfs_follow_link**函数（VFS提供）处理刚得到的文件名即可。



### **jffs2_getlink**

```c++
char *jffs2_getlink(struct jffs2_sb_info *c, struct jffs2_inode_info *f)
```

这个函数只需得到文件f的文件名即可。

调用这个函数的inode的类型一定是symlink类型，因此只有唯一的一个raw_inode，其dnode被metadata指向。

函数流程：

- 调用**jffs2_read_dnode**函数从dnode中读取数据即可。



## 第9章 jffs2中目录文件的方法表（new）

> 在jffs2_read_inode函数中创建inode的最后，会根据文件的类型将i_op等指针设置为具体类型文件的方法表。目录文件的方法表为jffs2_dir_inode_operations。

```c++
struct inode_operations jffs2_dir_inode_operations =
{
    .create = jffs2_create,
    .lookup = jffs2_lookup,
    .link = jffs2_link,
    .unlink = jffs2_unlink,
    .symlink = jffs2_symlink,
    .mkdir = jffs2_mkdir,
    .rmdir = jffs2_rmdir,
    .mknod = jffs2_mknod,
    .rename = jffs2_rename,
    .setattr = jffs2_setattr,
};
```



### jffs2_create函数

在以O_CREATE的方式打开一个不存在的目录文件时，会最终调用到open_namei函数，在这个函数中：

- 会先调用**lookup_hash**函数创建VFS的dentry（不是JFFS2的dirent）并加入到VFS的系统目录树中；

- 接着调用**jffs2_lookup**函数尝试在JFFS2中从父目录的inode_info中的dents中找到文件对应的full_dirent，结果肯定会失败。这时就会转而去调用**vfs_create**函数，vfs_create函数负责创建出这个即将诞生的目录文件的inode以及第一个raw_inode。（vfs_create函数就是靠调用**jffs2_create**函数）

```c++
static int jffs2_create(struct inode *dir_i, struct dentry *dentry, int mode)
```

dir_i是父目录的inode，dentry是当前这个文件的dentry。

==总之jffs2_create就是创建两个inode的函数，一个是VFS的inode，一个是jffs2中的raw_inode==，并建立它们之间的联系；

本函数流程：

- 调用**jffs2_alloc_raw_inode**函数申请新raw_inode(还未写入flash)

- 调用==**jffs2_new_inode**==函数用父目录inode、raw_inode、mode创建好一个VFS的inode并返回。

  > 调用jffs2_new_inode函数完成如下工作：
  >
  > 1. 分配、初始化文件的内核描述符jffs2_inode_cache数据结构；
  >
  > 2. 设置jffs2_raw_inode数据实体；
  >
  > 3. 分配、初始化inode；
  > 4. 建立jffs2_inode_cache和inode的联系，注册inode。

- 为inode设置好几个方法表；

- 调用==**jffs2_do_create**==函数。

  > 向其父目录中写入相应的目录项jffs2_raw_dirent数据实体、向flash写入“代表其存在”的第一个jffs2_raw_inode数据实体，以及创建上层的jffs2_full_dirent和jffs2_full_dnode。

- 调用**d_instantiate**函数用d_alias和d_inode域建立dentry和inode之间的联系。



### jffs2_new_inode函数

```c++
struct inode *jffs2_new_inode (struct inode *dir_i, int mode, struct jffs2_raw_inode *ri)
```

本函数流程：

- 调用**new_inode**函数向内存申请新的inode；
- 调用**jffs2_init_inode_info**函数将新的inode的u域设置为inode_info并初始化；
- 调用==**jffs2_do_new_inode**==函数将新得到的inode_info创建出inode_cache，并使其被inode_info中的inocache指向，之后再将这个inode_cache加入到超级块的inocache_list[]哈希表；
- 调用insert_inode_hash函数将这个新的inode利用inode.i_hash域将其加入索引节点哈希表inode_hashtable的某个队列；



### jffs2_do_create函数

```c++
int jffs2_do_create(struct jffs2_sb_info *c, struct jffs2_inode_info *dir_f, struct jffs2_inode_info *f, struct jffs2_raw_inode *ri, const char *name, int namelen)
```

在jffs2_create函数中前面已经调用了jffs2_new_inode函数完成了创建inode的工作，接下来就是要：

1. 将唯一的raw_inode写回到flash，并且还在flash中将其父目录中创建新的raw_dirent；
2. 同时注意要像打开文件那样创建这新的两个数据实体的内核描述符。

注意：凡是带alloc的函数意思都是在内存在分配一个内存上的空间，都是在flash上还没有空间的，需要之后写到flash之前先预订位置。

本函数流程：

- 在flash上给raw_inode预定位置，调用jffs2_reserve_space函数；
- 将raw_inode写回flash，调用**jffs2_write_dnode**函数（顺便创建full_dnode）；
- 向内存申请raw_dirent的空间，调用jffs2_alloc_raw_dirent函数；
- 设置raw_dirent；
- 如果之前向flash预定的空间已经满了，则再次预定空间，这次是为了raw_dirent预定位置；
- 将raw_dirent写回flash，调用**jffs2_write_dirent**函数（顺便创建full_dirent）；
- 将raw_dirent对应的full_dirent加入到父目录的dents链表中，调用jffs2_add_fd_to_list函数；



### jffs2_do_new_inode函数

```c++
int jffs2_do_new_inode(struct jffs2_sb_info *c, struct jffs2_inode_info *f, uint32_t mode, struct jffs2_raw_inode *ri)
```

这个函数在jffs2_new_inode函数中被调用。就是负责创建inode_cache的，并且进一步设置raw_inode中的相关属性。

本函数流程：

- 申请新inode_cache，调用**jffs2_alloc_inode_cache**函数；
- 将这个inode_cache加入到sb_info中的inocache_list，调用**jffs2_add_ino_cache**函数；



## 第10章 jffs2的Garbage Collection

在挂载jffs2文件系统时、在jffs2_do_fill_super函数的最后创建并启动GC内核线程：

```c++
if (!(sb->s_flags & MS_RDONLY))
    jffs2_start_garbage_collect_thread(c);
return 0;
```

### jffs2_start_garbage_collect_thread函数

这个函数就是启动GC内核线程的，具体执行函数为**jffs2_garbage_collect_thread**函数；



### jffs2_garbage_collect_thread函数

这个函数就是GC的main了。

大致流程：

- daemonize函数，使GC进程脱离父进程；（随后进入死循环中）

> 一次循环开始

- siginitsetinv函数，设置好GC进程所能接受的信号；（不能接受的都会被屏蔽掉）

- recalc_sigpending函数，检查当前是否有可以接受的信号，之后可以用signal_pending获取到这些信号；

- thread_should_wake函数，判断当前GC进程是否有事做，如果没事就再睡过去；

  > while(signal_pending(current))	//可能有多个待处理的信号，因此要多次判断

- signal_pending函数，该函数检查当前进程是否有信号处理，返回不为0表示有信号需要处理；

- dequeue_signal函数，取出**待处理的信号中的一个**，为signr；

  - 如果是STOP信号，设置当前进程状态为STOPPED，再让出CPU；
  - 如果是KILL信号，则需要结束当前GC进程，将sb_info中的gc_task设置为NULL，再调用complete_and_exit函数结束；
  - 如果是HUP信号，说明是有人需要垃圾回收，因此直接break跳出while循环。

- 结束while信号后，就要开始进行真正地垃圾回收工作了，在工作时就不必再接受HUP信号了，因此将可接受的信号变为只有STOP, KILL, CONT。最后调用**==jffs2_garbage_collect_pass函数==**进行垃圾回收；



### jffs2_garbage_collect_pass函数

首先一定要知道的是，这个函数每次只尝试回收一个raw_inode或raw_dirent，只有当某次回收完后刚好这个eraseblock上所有数据实体都已经移动到别处了才可以将这个eraseblock回收掉。

大致流程：

- 首先如果flash设备上还有unchecked的空间，则会从第一个未被checked的文件开始（通过checked_ino）对文件执行iget函数，在iget中相当于是打开一次文件（前面已经有说，打开文件时会进行CRC检查），因此这样可以check一个文件。

- 当flash设备所有空间都被check了后，就会从sb_info中的gcblock开始（gcblock就是个擦除块），如果gcblock为空，则代表要开始从新的一个擦除块开始进行尝试回收工作了（前面说的每次回收只回收一个数据实体而非擦除块，因此gcblock就是当前正在努力擦除的块）。通过jffs2_find_gc_block函数可以得到下一个要擦除的块，这个函数就体现了jffs2的磨损平衡算法。

- （如果这个块used_size即有效数据大小为0，就直接可以擦除），否则，从eraseblock的gc_node开始。gc_node和gcblock性质相同，代表着下一个要尝试回收的数据实体。如果gc_node已经是过期的，则通过next_phys域找下一个数据实体，直到找到一个有效数据实体为止，此时gc_node就是该块上的将要被回收的有效数据实体。

  - 如果该数据实体的next_in_ino为NULL，就代表是cleanmarker或snapshot，直接标记为过期即可退出本次jffs2_garbage_collect_pass函数。

  - 如果不是cleanmarker或snapshot，就获取其inode number，再得到其文件的inode。如果这个数据实体被文件的metadata指向，就调用==jffs2_garbage_collect_metadata==进行回收即可。

  - 否则就代表是个普通数据实体，则从文件的红黑树根开始，遍历整个红黑树，找到所有节点对应的内核描述符node_ref的地址和此时这个gc_node地址相同的节点（**==按照代码的意思来看，可能有多个红黑树的节点对应的内核描述符相同？？==**）。

    遍历完红黑树后，如果gc_node对应多个红黑树节点，那么就只能说明这个数据实体代表一个空洞，

    > 看到这里我终于有点明白到底是个什么情况了，一个eraseblock由多个page组成，正常的数据实体不会越过page边界，而只有空洞才会跨越page的边界。

    调用==jffs2_garbage_collect_hole==函数进行回收即可。否则调用==jffs2_garbage_collect_dnode==回收即可。

    如果遍历完红黑树，gc_node一个节点也对应不上，则说明gc_node是个raw_dirent的node_ref，因此再在文件的dents链表中进行遍历查找，如果找到了且文件的ino不为0，就调用==jffs2_garbage_collect_dirent==函数进行回收即可，否则如果ino为0，就调用==jffs2_garbage_collect_deletion_dirent==进行回收即可。

  - 最后，如果发现当前块gcblock的有效数据大小已为0，就将其加入到erase_pending_list中，将gcblock重置为NULL，再调用==**jffs2_erase_pending_trigger**==函数进行处理。



### jffs2_erase_pending_trigger函数

```c++
void jffs2_erase_pending_trigger(struct jffs2_sb_info *c)
{
	OFNI_BS_2SFFJ(c)->s_dirt = 1;
}
```

这个函数就是将superblock中的s_dirt置为1，在卸载jffs2或电脑关机时，就会将所有s_dirt = 1的超级块调用其write_super方法，jffs2_write_super就会调用jffs2_garbage_collect_trigger函数，而这个trigger函数便会向GC线程发送HUP信号。

> 说实话这里感觉很有问题，就是没有看到实际擦除块，将块写回free_list的过程，相反按照作者这里的描述，则本来应该擦除一个块的时候又会再次给自己发送信号？这么不就会又循环回GC进程了吗？

> 噢这里作者应该是省略掉了将块加入到free_list的函数，在write_super函数中除了调用jffs2_garbage_collect_trigger函数，还有就是jffs2_erase_pending_blocks函数以及jffs2_mark_erased_blocks函数，这后面两个函数就是对erasing_list中的块进行擦除并加入到free_list的函数。



## 第11章 讨论和体会
当标记原有数据实体“过时”时，在jffs2_mark_node_obsolete函数（尚未详细分析）中还会同时修改原有数据实体头部的nodetype区域。

每次GC内核线程运行时只把被GC的擦除块中一个有效数据实体变成过时，当整个擦除块中只含有过时数据实体时进行擦除操作。

在挂载jffs2时会计算数据实体自身的CRC检验，如果不通过则认为是过时的；另外在打开文件时（无论是dir还是normal file）也都会在建立其full_xxx数据时进行检验，如果有多个数据实体指向同一片区域，则取version最大的为有效的，其余的就会被标记为过时。（==在jffs2_mark_node_obsolete函数中标记数据实体过时时，还会同时修改flash中数据实体本身头部的nodetype，使得头部的crc校验失败==）

