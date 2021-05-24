/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: HoitFs.c
**
** 创   建   人: Hoit Group
**
** 文件创建日期: 2021 年 03 月 19 日
**
** 描        述: HoitFs
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/fs/fsCommon/fsCommon.h"
#include "../SylixOS/fs/include/fs_fs.h"
#include "HoitFs.h"
/*********************************************************************************************************
  内部全局变量
*********************************************************************************************************/
static INT                              _G_iHoitFsDrvNum = PX_ERROR;
/*********************************************************************************************************
  内部函数
*********************************************************************************************************/
static LONG     __hoitFsOpen();
static INT      __hoitFsRemove();
static INT      __hoitFsClose();
static ssize_t  __hoitFsRead();
static ssize_t  __hoitFsPRead();
static ssize_t  __hoitFsWrite();
static ssize_t  __hoitFsPWrite();
static INT      __hoitFsLStat();
static INT      __hoitFsIoctl();
static INT      __hoitFsSymlink();
static ssize_t  __hoitFsReadlink();
/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0 //&& LW_CFG_RAMFS_EN > 0
#include "HoitFsLib.h"
/*********************************************************************************************************
** 函数名称: API_HoitFsDrvInstall
** 功能描述: 安装 hoitfs 文件系统驱动程序
** 输　入  :
** 输　出  : < 0 表示失败
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_HoitFsDrvInstall(VOID)
{
    struct file_operations     fileop;

    if (_G_iHoitFsDrvNum > 0) {
        return  (ERROR_NONE);
    }

    lib_bzero(&fileop, sizeof(struct file_operations));

    fileop.owner = THIS_MODULE;
    fileop.fo_create = __hoitFsOpen;
    fileop.fo_release = __hoitFsRemove;
    fileop.fo_open = __hoitFsOpen;
    fileop.fo_close = __hoitFsClose;
    fileop.fo_read = __hoitFsRead;
    fileop.fo_read_ex = __hoitFsPRead;
    fileop.fo_write = __hoitFsWrite;
    fileop.fo_write_ex = __hoitFsPWrite;
    fileop.fo_lstat = __hoitFsLStat;
    fileop.fo_ioctl = __hoitFsIoctl;
    fileop.fo_symlink = __hoitFsSymlink;
    fileop.fo_readlink = __hoitFsReadlink;

    _G_iHoitFsDrvNum = iosDrvInstallEx2(&fileop, LW_DRV_TYPE_NEW_1);     /*  使用 NEW_1 型设备驱动程序   */

    DRIVER_LICENSE(_G_iHoitFsDrvNum, "GPL->Ver 2.0");
    DRIVER_AUTHOR(_G_iHoitFsDrvNum, "HITSZ.HoitGroup");
    DRIVER_DESCRIPTION(_G_iHoitFsDrvNum, "norflash fs driver.");

    _DebugHandle(__LOGMESSAGE_LEVEL, "norflash file system installed.\r\n");

    __fsRegister("hoitfs", API_HoitFsDevCreate, LW_NULL, LW_NULL);        /*  注册文件系统                */

    return  ((_G_iHoitFsDrvNum > 0) ? (ERROR_NONE) : (PX_ERROR));
}

/*********************************************************************************************************
** 函数名称: API_HoitFsDevCreate
** 功能描述: 创建 hoitfs 文件系统设备.
** 输　入  : pcName            设备名(设备挂接的节点地址)
**           pblkd             使用 pblkd->BLKD_pcName 作为 最大大小 标示.
** 输　出  : < 0 表示失败
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_HoitFsDevCreate(PCHAR   pcName, PLW_BLK_DEV  pblkd)
{
    PHOIT_SB     pfs;

    if (_G_iHoitFsDrvNum <= 0) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "hoitfs Driver invalidate.\r\n");
        _ErrorHandle(ERROR_IO_NO_DRIVER);
        return  (PX_ERROR);
    }
    if ((pblkd == LW_NULL) || (pblkd->BLKD_pcName == LW_NULL)) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "block device invalidate.\r\n");
        _ErrorHandle(ERROR_IOS_DEVICE_NOT_FOUND);
        return  (PX_ERROR);
    }
    if ((pcName == LW_NULL) || __STR_IS_ROOT(pcName)) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "mount name invalidate.\r\n");
        _ErrorHandle(EFAULT);                                           /*  Bad address                 */
        return  (PX_ERROR);
    }

    pfs = (PHOIT_SB)__SHEAP_ALLOC(sizeof(HOIT_SB));
    if (pfs == LW_NULL) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (PX_ERROR);
    }
    lib_bzero(pfs, sizeof(HOIT_SB));                              /*  清空卷控制块                */

    pfs->HOITFS_bValid = LW_TRUE;

    pfs->HOITFS_hVolLock = API_SemaphoreMCreate("hoit_sb_lock", LW_PRIO_DEF_CEILING,
        LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
        LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
        LW_NULL);
    if (!pfs->HOITFS_hVolLock) {                                      /*  无法创建卷锁                */
        __SHEAP_FREE(pfs);
        return  (PX_ERROR);
    }

    pfs->HOITFS_mode = S_IFDIR | DEFAULT_DIR_PERM;
    pfs->HOITFS_uid = getuid();
    pfs->HOITFS_gid = getgid();
    pfs->HOITFS_time = lib_time(LW_NULL);
    pfs->HOITFS_ulCurBlk = 0ul;

    //__ram_mount(pramfs);

    if (iosDevAddEx(&pfs->HOITFS_devhdrHdr, pcName, _G_iHoitFsDrvNum, DT_DIR)
        != ERROR_NONE) {                                                /*  安装文件系统设备            */
        API_SemaphoreMDelete(&pfs->HOITFS_hVolLock);
        __SHEAP_FREE(pfs);
        return  (PX_ERROR);
    }

    _DebugFormat(__LOGMESSAGE_LEVEL, "target \"%s\" mount ok.\r\n", pcName);

    return  (ERROR_NONE);
}

/*********************************************************************************************************
** 函数名称: API_HoitFsDevDelete
** 功能描述: 删除一个 hoitfs 文件系统设备, 例如: API_HoitFsDevDelete("/mnt/ram0");
** 输　入  : pcName            文件系统设备名(物理设备挂接的节点地址)
** 输　出  : < 0 表示失败
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_HoitFsDevDelete(PCHAR   pcName)
{
    if (API_IosDevMatchFull(pcName)) {                                  /*  如果是设备, 这里就卸载设备  */
        return  (unlink(pcName));

    }
    else {
        _ErrorHandle(ENOENT);
        return  (PX_ERROR);
    }
}

/*********************************************************************************************************
** 函数名称: __hoitFsOpen
** 功能描述: 打开或者创建文件
** 输　入  : pfs              内存中HoitFs文件系统的super block
**           pcName           文件名
**           iFlags           方式
**           iMode            mode_t
** 输　出  : < 0 错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static LONG __hoitFsOpen(PHOIT_SB     pfs,
    PCHAR           pcName,
    INT             iFlags,
    INT             iMode)
{
    PLW_FD_NODE pfdnode;

    if (pcName == LW_NULL) {
        _ErrorHandle(EFAULT);                                           /*  Bad address                 */
        return  (PX_ERROR);
    }

    if (iFlags & O_CREAT) {                                             /*  创建操作                    */
        if (__fsCheckFileName(pcName)) {
            _ErrorHandle(ENOENT);
            return  (PX_ERROR);
        }
        if (S_ISFIFO(iMode) ||
            S_ISBLK(iMode) ||
            S_ISCHR(iMode)) {
            _ErrorHandle(ERROR_IO_DISK_NOT_PRESENT);                    /*  不支持以上这些格式          */
            return  (PX_ERROR);
        }
    }

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }

    //************************************ TODO ************************************
    

    //************************************ END  ************************************
    __HOITFS_SB_UNLOCK(pfs);

    return  ((LONG)pfdnode);                                            /*  返回文件节点                */
}


/*********************************************************************************************************
** 函数名称: __hoitFsRemove
** 功能描述: hoitfs remove 操作
** 输　入  : pfs              卷设备
**           pcName           文件名
**           注意文件名如果为空就是卸载本文件系统
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsRemove(PHOIT_SB   pfs,
    PCHAR         pcName)
{
    if (pcName == LW_NULL) {
        _ErrorHandle(ERROR_IO_NO_DEVICE_NAME_IN_PATH);
        return  (PX_ERROR);
    }

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }

    //************************************ TODO ************************************


    //************************************ END  ************************************
    __HOITFS_SB_UNLOCK(pfs);
    return 0;
}

/*********************************************************************************************************
** 函数名称: __hoitFsClose
** 功能描述: hoitfs close 操作
** 输　入  : pfdentry         文件控制块
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsClose(PLW_FD_ENTRY    pfdentry)
{
    PLW_FD_NODE   pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_SB      pfs     = (PHOIT_SB)pfdnode->FDNODE_pvFsExtern;

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }
    
    LW_DEV_DEC_USE_COUNT(&pfs->HOITFS_devhdrHdr);

    __HOITFS_SB_UNLOCK(pfs);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __hoitFsRead
** 功能描述: hoitfs read 操作
** 输　入  : pfdentry         文件控制块
**           pcBuffer         接收缓冲区
**           stMaxBytes       接收缓冲区大小
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __hoitFsRead(PLW_FD_ENTRY pfdentry,
    PCHAR        pcBuffer,
    size_t       stMaxBytes)
{
    PLW_FD_NODE   pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    
    ssize_t       sstReadNum = PX_ERROR;

    if (!pcBuffer) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    return  (sstReadNum);
}

/*********************************************************************************************************
** 函数名称: __hoitFsPRead
** 功能描述: hoitfs pread 操作
** 输　入  : pfdentry         文件控制块
**           pcBuffer         接收缓冲区
**           stMaxBytes       接收缓冲区大小
**           oftPos           位置
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __hoitFsPRead(PLW_FD_ENTRY pfdentry,
    PCHAR        pcBuffer,
    size_t       stMaxBytes,
    off_t        oftPos)
{
    PLW_FD_NODE   pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    
    ssize_t       sstReadNum = PX_ERROR;

    if (!pcBuffer || (oftPos < 0)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    return  (sstReadNum);
}

/*********************************************************************************************************
** 函数名称: __hoitFsWrite
** 功能描述: hoitfs write 操作
** 输　入  : pfdentry         文件控制块
**           pcBuffer         缓冲区
**           stNBytes         需要写入的数据
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __hoitFsWrite(PLW_FD_ENTRY  pfdentry,
    PCHAR         pcBuffer,
    size_t        stNBytes)
{
    PLW_FD_NODE   pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    
    ssize_t       sstWriteNum = PX_ERROR;

    if (!pcBuffer) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (pfdentry->FDENTRY_iFlag & O_APPEND) {                           /*  追加模式                    */
        pfdentry->FDENTRY_oftPtr = pfdnode->FDNODE_oftSize;             /*  移动读写指针到末尾          */
    }

    return  (sstWriteNum);
}

/*********************************************************************************************************
** 函数名称: __hoitFsPWrite
** 功能描述: hoitfs pwrite 操作
** 输　入  : pfdentry         文件控制块
**           pcBuffer         缓冲区
**           stNBytes         需要写入的数据
**           oftPos           位置
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __hoitFsPWrite(PLW_FD_ENTRY  pfdentry,
    PCHAR         pcBuffer,
    size_t        stNBytes,
    off_t         oftPos)
{
    PLW_FD_NODE   pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;

    ssize_t       sstWriteNum = PX_ERROR;

    if (!pcBuffer || (oftPos < 0)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    return  (sstWriteNum);
}

/*********************************************************************************************************
** 函数名称: __hoitFsLStat
** 功能描述: hoitFs stat 操作
** 输　入  : pfs              hoitfs 文件系统
**           pcName           文件名
**           pstat            文件状态
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsLStat(PHOIT_SB  pfs, PCHAR  pcName, struct stat* pstat)
{
    BOOL          bRoot;

    if (!pcName || !pstat) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    __HOITFS_SB_UNLOCK(pfs);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __hoitFsSymlink
** 功能描述: hoitFs 创建符号链接文件
** 输　入  : pfs                 hoitfs 文件系统
**           pcName              链接原始文件名
**           pcLinkDst           链接目标文件名
**           stMaxSize           缓冲大小
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsSymlink(PHOIT_SB   pfs,
    PCHAR         pcName,
    CPCHAR        pcLinkDst)
{

    BOOL          bRoot;

    if (!pcName || !pcLinkDst) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__fsCheckFileName(pcName)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    

    __HOITFS_SB_UNLOCK(pfs);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __hoitFsReadlink
** 功能描述: hoitFs 读取符号链接文件内容
** 输　入  : pfs                 hoitfs 文件系统
**           pcName              链接原始文件名
**           pcLinkDst           链接目标文件名
**           stMaxSize           缓冲大小
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t __hoitFsReadlink(PHOIT_SB   pfs,
    PCHAR         pcName,
    PCHAR         pcLinkDst,
    size_t        stMaxSize)
{
    size_t      stLen;

    if (!pcName || !pcLinkDst || !stMaxSize) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOITFS_SB_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    __HOITFS_SB_UNLOCK(pfs);

    return  ((ssize_t)stLen);
}
/*********************************************************************************************************
** 函数名称: __hoitFsIoctl
** 功能描述: hoitFs ioctl 操作
** 输　入  : pfdentry           文件控制块
**           request,           命令
**           arg                命令参数
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsIoctl(PLW_FD_ENTRY  pfdentry,
    INT           iRequest,
    LONG          lArg)
{
    return  (ERROR_NONE);
}
#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
