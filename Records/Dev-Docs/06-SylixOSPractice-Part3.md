# simpleFs试验

## mount指令

函数：__tshellFsCmdMount

位置：ttinyShellFsCmd

#### 重要结构体

>LW_MOUNT_NODE 
>
>​	-> LW_BLK_RAW
>
>​		 -> LW_BLK_DEV
>
>RAM_VOLUME 
>
>​	-> LW_DEV_HDR

##### LW_MOUNT_NODE 

```c
typedef struct {
    LW_BLK_RAW              MN_blkraw;                                  /*  BLOCK RAW 设备              */
#define MN_blkd             MN_blkraw.BLKRAW_blkd
#define MN_iFd              MN_blkraw.BLKRAW_iFd

    BOOL                    MN_bNeedDelete;                             /*  是否需要删除 BLOCK RAW      */
    LW_LIST_LINE            MN_lineManage;                              /*  管理链表                    */
    CHAR                    MN_cVolName[1];                             /*  加载卷的名字                */
} LW_MOUNT_NODE;
typedef LW_MOUNT_NODE      *PLW_MOUNT_NODE;
```

##### RAM_VOLUME 

```c
/*********************************************************************************************************
  类型
*********************************************************************************************************/
typedef struct {
    LW_DEV_HDR          RAMFS_devhdrHdr;			/*  ramfs 文件系统设备头        */
    LW_OBJECT_HANDLE    RAMFS_hVolLock;             /*  卷操作锁                    */
    LW_LIST_LINE_HEADER RAMFS_plineFdNodeHeader;    /*  fd_node 链表                */
    LW_LIST_LINE_HEADER RAMFS_plineSon;             /*  儿子链表                    */
    
    BOOL                RAMFS_bForceDelete;         /*  是否允许强制卸载卷          */
    BOOL                RAMFS_bValid;
    
    uid_t               RAMFS_uid;                                      /*  用户 id                     */
    gid_t               RAMFS_gid;                                      /*  组   id                     */
    mode_t              RAMFS_mode;                                     /*  文件 mode                   */
    time_t              RAMFS_time;                                     /*  创建时间                    */
    ULONG               RAMFS_ulCurBlk;                                 /*  当前消耗内存大小            */
    ULONG               RAMFS_ulMaxBlk;                                 /*  最大内存消耗量              */
} RAM_VOLUME;
```



#### 流程cc

```mermaid
graph TB
A[__tshellFsCmdMount] --> B[API_MountEx]
B --> C[__mount]
C --> D[__fsCreateFuncGet]
C --> E[创建LW_MOUNT_NODE]
C --> F[pfuncFsCreate]
F -.-> G[API_SimpleFsDevCreate]
G --> H[API_IosDevAddEx]

```

#### 各函数的作用

> **__tshellFsCmdMount：**
>
> ​	处理mount命令中的参数。
>
> **__mount：**
>
> ​	创建PLW_MOUNT_NODE。
>
> ​	保存卷挂载名（即/mnt/simple）到PLW_MOUNT_NODE。
>
> ​	保存设备名（即10000）到PLW_MOUNT_NODE。
>
> ​	调用API_RamFsDevCreate。
>
> ​	将PLW_MOUNT_NODE挂到链表_G_plineMountDevHeader上。
>
> **API_IosDevAddEx：**
>
> ​	创建PLW_DEV_HDR。
>
> ​	填写PLW_DEV_HDR相关信息。
>
> ​	创建根目录节点（rootFsMakeDev）。
>
> ​	将PLW_DEV_HDR挂载到链表_S_plineDevHdrHeader上。



```c
/*********************************************************************************************************
** 函数名称: API_MountEx
** 功能描述: 挂载一个分区
** 输　入  : pcDevName         块设备名   例如: /dev/sda1 [mount RAMFS文件系统时，传入的不是设备文件名而是RAMFS占用的最大内存空间，这个参数必须是10进行，单位是字节]
**           pcVolName         挂载目标   例如: /mnt/usb (不能使用相对路径, 否则无法卸载)
**           pcFileSystem      文件系统格式 "vfat" "iso9660" "ntfs" "nfs" "romfs" "ramfs" ... 
                               NULL 表示使用默认文件系统
**           pcOption          选项, 当前支持 ro 或者 rw
** 输　出  : < 0 表示失败
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
INT  API_MountEx (CPCHAR  pcDevName, CPCHAR  pcVolName, CPCHAR  pcFileSystem, CPCHAR  pcOption)
{
    INT     iRet;
    
    __KERNEL_SPACE_ENTER();
    iRet = __mount(pcDevName, pcVolName, pcFileSystem, pcOption);
    __KERNEL_SPACE_EXIT();
    
    return  (iRet);
}
```

### mkdir指令



​	



函数：__tshellFsCmdMkdir

位置：ttinyShellFsCmd

流程：

```mermaid
graph TB
	A[__tshellFsCmdMkdir] --> B[mkdir]
	B --> C[open]
	B --> D[close]
	C --> E[_IoOpen]
	E --> F[_IosEnvGetDef]
	E --> G[ioFullFileNameGet]
	E --> H[iosFdNew]
	E --> I[iosOpen]
	I -.-> J[__ramFsOpen]
```



>**_IosEnvGetDef：**获取环境变量结构体，以便获得文件权限标志（IOE_modeUMask）。
>
>**ioFullFileNameGet：**获取完整路径和该路径对应的设备。
>
>**iosFdNew：**由系统创建一个新的文件描述符并在对应LW_FD_ENTRY中记录设备头等信息。
>
>**iosCreate（API_IosCreate）：**从驱动程序表（_S_deventryTbl）获取文件创建函数pfuncDrvCreate，然后调用该函数。ramfs的驱动设备在API_RamFsDrvInstall中的iosDrvInstallEx2函数里就在驱动程序表上注册了这个函数。
>
>**iosOpen：**和上一条函数差不多，调用的是文件系统打开文件函数pfuncDrvOpen。在ramfs中pfuncDrvCreate和pfuncDrvOpen都是__ramFsOpen，所以都一样。
>
>

函数：__ramFsOpen

位置：ramFs.c

​	该函数一共要创建两种结构体，PRAM_NODE（目录结构）和PLW_FD_NODE（打开的文件）。

流程：

```mermaid
graph TB
	A[__ramFsOpen] --> B[__ram_open]
	A --> C[__ram_maken]
	A --> D[__ram_stat]
	A --> E[API_IosFdNodeAdd]
	
```



> 
>
> __RAMFS_VOL_LOCK：**
>
> ​	为ramfs上锁。
>
> **__ram_open：**
>
> ​	ramfs把"/mnt/simple/"作为了根目录，所以"/mnt/simple/"对应参数pcname为"/"。ramfs里文件inode以链表的方式存储。当ramfs打开文件时，它会从根目录开始匹配输入的文件路径，最后返回ramfs下该文件的节点（PRAM_NODE）。
>
> **__ram_maken：**
>
> ​	如果上边\_\_ram_open没有找到节点（mkdir就是这个情况），则在这里创建一个新的节点，记录文件名称，类型等信息。
>
> **__ram_stat：**
>
> ​	初始化该文件的stat。
>
> **API_IosFdNodeAdd：**
>
> ​	参数里的inode64是node的结构体地址。
>
> ​	遍历文件系统已打开的文件（pramfs->RAMFS_plineFdNodeHeader），如果要添加的fdnode已经存在，则增加该fdnode的引用数；如果没有则创建一个新的fdnode节点。这里为fdnode新建了一个信号量锁，可能将来需要看看这些锁的创建。
>
> **LW_DEV_INC_USE_COUNT**：
>
> ​	用原子操作让该文件的打开次数加一。	

## ramFs修改成simpleFs的过程

> mount -t ramfs 10000 /mnt/ram
>
> mount -t simplefs 10000 /mnt/simple

1. 更改了加载驱动、创建设备和删除设备(simpleFs.c)三个函数的名字以及相应的宏定义(simpleFs.h)。

2. 更改了simpleFs.c和simpleFsLib.c里的include。

3. 将simpleFs头文件添加到文件系统头文件(fs_fs.h)中。

4. 修改API_SemaphoreMCreate参数，把锁的名称改为simplevol_lock。

5. 把DRIVER_DESCRIPTION和__fsRegister参数中的"ram"改成"simple"。

6. 修改了simpleFs.h中的裁剪宏__SIMPLEFS_H。

   > 裁剪宏（ifndef）：如果某个宏定义没有被定义过，则会编译"#ifndef"到"#endif"之间的程序。

7. 在__tshellFsCmdMount中打印mount信息。

8. 在__mount中，ramfs不需要操作 BLK RAM设备（bNeeddelete为0）。所以添加一个判断，让simpleFs也不需要。

9. 添加了一些打印。





## 其他

1. 为什么创建目录之后要添加fdnode节点。

   之后在__tshellFsCmdMkdir会关闭该文件，所以这里要维持打开状态。

2. 在_IoOpen中创建文件的那一行