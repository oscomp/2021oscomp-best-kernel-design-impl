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
#include "SylixOS.h"
#include "hoitFs.h"
#include "hoitFsGC.h"
#include "hoitFsCache.h"

#ifndef HOITFS_DISABLE
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
//#if LW_CFG_MAX_VOLUMES > 0 //&& LW_CFG_RAMFS_EN > 0
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
#define NAMESPACE   hoitFs
USE_LIST_TEMPLATE(NAMESPACE, HOIT_ERASABLE_SECTOR_REF);
LW_API
INT  API_HoitFsDevCreate(PCHAR   pcName, PLW_BLK_DEV  pblkd)
{
    PHOIT_VOLUME     pfs;
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

    pfs = (PHOIT_VOLUME)lib_malloc(sizeof(HOIT_VOLUME));
    if (pfs == LW_NULL) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (PX_ERROR);
    }
    lib_bzero(pfs, sizeof(HOIT_VOLUME));                              /*  清空卷控制块                */

    pfs->HOITFS_bValid = LW_TRUE;

    pfs->HOITFS_hVolLock = API_SemaphoreMCreate("hoit_volume_lock", LW_PRIO_DEF_CEILING,
        LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
        LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
        LW_NULL);
    if (!pfs->HOITFS_hVolLock) {                                      /*  无法创建卷锁                */
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));
        return  (PX_ERROR);
    }

    /* 建立三个链表锁 */
    pfs->HOITFS_dirtyLock = API_SemaphoreMCreate("hoit_dirty_lock", LW_PRIO_DEF_CEILING,
        LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
        LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
        LW_NULL);
    if (!pfs->HOITFS_dirtyLock) {                                      /*  无法创建卷锁                */
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));
        return  (PX_ERROR);
    }
    pfs->HOITFS_cleanLock = API_SemaphoreMCreate("hoit_clean_lock", LW_PRIO_DEF_CEILING,
        LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
        LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
        LW_NULL);
    if (!pfs->HOITFS_cleanLock) {                                      /*  无法创建卷锁                */
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));
        return  (PX_ERROR);
    }
    pfs->HOITFS_freeLock = API_SemaphoreMCreate("hoit_free_lock", LW_PRIO_DEF_CEILING,
        LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
        LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
        LW_NULL);
    if (!pfs->HOITFS_freeLock) {                                      /*  无法创建卷锁                */
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));
        return  (PX_ERROR);
    }

    pfs->HOITFS_mode            = S_IFDIR | DEFAULT_DIR_PERM;
    pfs->HOITFS_uid             = getuid();
    pfs->HOITFS_gid             = getgid();
    pfs->HOITFS_time            = lib_time(LW_NULL);
    //TODO 内存消耗空间计算
    pfs->HOITFS_ulCurBlk        = 0ul;
    pfs->HOITFS_ulMaxBlk        = 0ul;
    pfs->HOITFS_now_sector      = LW_NULL;
    pfs->HOITFS_pRootDir        = LW_NULL;
    pfs->HOITFS_totalUsedSize   = 0;

    pfs->HOITFS_hGCThreadId     = LW_NULL;

                                                                        /* GC相关 */
    pfs->HOITFS_curGCSector        = LW_NULL;
    pfs->HOITFS_GCMsgQ             = PX_ERROR;
    pfs->ulGCBackgroundTimes       = 0;
    pfs->ulGCForegroundTimes       = 0;
    pfs->HOITFS_erasableSectorList = LW_NULL;
    pfs->HOITFS_bShouldKillGC      = LW_FALSE;

    InitList(pfs->HOITFS_dirtySectorList,    NAMESPACE, HOIT_ERASABLE_SECTOR_REF); /* 初始化模板链表 */
    InitList(pfs->HOITFS_cleanSectorList,    NAMESPACE, HOIT_ERASABLE_SECTOR_REF);
    InitList(pfs->HOITFS_freeSectorList,     NAMESPACE, HOIT_ERASABLE_SECTOR_REF);
    InitIterator(pfs->HOITFS_sectorIterator, NAMESPACE, HOIT_ERASABLE_SECTOR_REF);

                                                                        /* Log相关 */
    pfs->HOITFS_logInfo            = LW_NULL;
    //__ram_mount(pramfs);

    hoitEnableCache(GET_SECTOR_SIZE(8), 8, pfs);
    //TODO 文件总大小暂时硬编码
    pfs->HOITFS_totalSize       = pfs->HOITFS_cacheHdr->HOITCACHE_blockSize * 28;
    __hoit_mount(pfs);

#ifdef BACKGOURND_GC_ENABLE 
    hoitStartGCThread(pfs, pfs->HOITFS_totalSize / 3);
#endif  /* BACKGOURND_GC_ENABLE */

    if (iosDevAddEx(&pfs->HOITFS_devhdrHdr, pcName, _G_iHoitFsDrvNum, DT_DIR)
        != ERROR_NONE) {                                                /*  安装文件系统设备            */
        API_SemaphoreMDelete(&pfs->HOITFS_hVolLock);
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));
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
static LONG __hoitFsOpen(PHOIT_VOLUME     pfs,
    PCHAR           pcName,
    INT             iFlags,
    INT             iMode)
{
    PLW_FD_NODE         pfdnode;
    PHOIT_INODE_INFO    phoitn;
    PHOIT_INODE_INFO    phoitFather;
    BOOL                bRoot;
    BOOL                bLast;
    BOOL                bIsNew;
    PCHAR               pcTail;
    BOOL                bCreate;
    struct stat         statGet;

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

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }

    /************************************ TODO ************************************/
    phoitn = __hoit_open(pfs, pcName, &phoitFather, LW_NULL,&bRoot, &bLast, &pcTail);
    
    if (phoitn) {
        if (!S_ISLNK(phoitn->HOITN_mode)) {
            if ((iFlags & O_CREAT) && (iFlags & O_EXCL)) {              /*  排他创建文件                */
                __HOIT_VOLUME_UNLOCK(pfs);
                _ErrorHandle(EEXIST);                                   /*  已经存在文件                */
                return  (PX_ERROR);            
            } else if ((iFlags & O_DIRECTORY) && !S_ISDIR(phoitn->HOITN_mode)) {
                __HOIT_VOLUME_UNLOCK(pfs);
                _ErrorHandle(ENOTDIR);
                return  (PX_ERROR);
            } else {
                goto    __file_open_ok;
            }
        }

    } else if ((iFlags & O_CREAT) && bLast) {                           /*  创建节点                    */
        phoitn = __hoit_maken(pfs, pcName, phoitFather, iMode, LW_NULL);
        iFlags &= !O_TRUNC;
        if (phoitn) {
            bCreate = LW_TRUE;
            goto    __file_open_ok;
        } else {
            return (PX_ERROR);
        }
    }

    if (phoitn) {                                                       /*  符号链接处理                */
        INT                 iError;
        PCHAR               pcPrefix;
        INT                 iFollowLinkType;
        PHOIT_FULL_DIRENT   pFullDirent = __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName);
        PCHAR               pcSymfile = pcTail - lib_strlen(pFullDirent->HOITFD_file_name) - 1; 
                                                                        /* pcSymfile指向pcName中软链接文件名首字母前面的'/' */

        if (*pcSymfile != PX_DIVIDER) {
            pcSymfile--;
        }
        if (pcSymfile == pcName) {
            pcPrefix = LW_NULL;                                         /*  没有前缀                    */
        } else {
            pcPrefix = pcName;
            *pcSymfile = PX_EOS;
        }        
        if (pcTail && lib_strlen(pcTail)) {
            iFollowLinkType = FOLLOW_LINK_TAIL;                         /*  连接目标内部文件            */
        } else {
            iFollowLinkType = FOLLOW_LINK_FILE;                         /*  链接文件本身                */
        }
        //TODO PHOIT_INODE_INFO还没有 symbol link
        iError = _PathBuildLink(pcName, MAX_FILENAME_LENGTH, 
                                        LW_NULL, pcPrefix, phoitn->HOITN_pcLink, pcTail);

        if (iError) {
            __HOIT_VOLUME_UNLOCK(pfs);
            return  (PX_ERROR);                                         /*  无法建立被链接目标目录      */
        } else {
            __HOIT_VOLUME_UNLOCK(pfs);
            return  (iFollowLinkType);
        }                                        
    } else if (bRoot == LW_FALSE) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(ENOENT);                                           /*  没有找到文件                */
        return  (PX_ERROR);
    } else if (bRoot == LW_TRUE){
        phoitn = pfs->HOITFS_pRootDir;
    }

__file_open_ok:
    __hoit_stat(phoitn, pfs, &statGet);
    pfdnode = API_IosFdNodeAdd(&pfs->HOITFS_plineFdNodeHeader,
                               statGet.st_dev,
                               (ino64_t)statGet.st_ino,
                               iFlags,
                               iMode,
                               statGet.st_uid,
                               statGet.st_gid,
                               statGet.st_size,
                               (PVOID)phoitn,
                               &bIsNew);    

    if (pfdnode == LW_NULL) {                                           /*  无法创建 fd_node 节点       */
        __HOIT_VOLUME_UNLOCK(pfs);
        if (bCreate) {                                                  /*  删除新建的节点              */
            //TOOPT hoit_unlink 待整合
            if (S_ISDIR(phoitn->HOITN_mode)) {
                __hoit_unlink_dir(phoitFather, 
                                    __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName));
            } else { //TODO 尚不能识别普通文件
                __hoit_unlink_regular(phoitFather,
                                    __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName));
            }             
        }
        return  (PX_ERROR);
    }

    pfdnode->FDNODE_pvFsExtern = (PVOID)pfs;                            /*  记录文件系统信息            */

    if ((iFlags & O_TRUNC) && ((iFlags & O_ACCMODE) != O_RDONLY)) {     /*  需要截断                    */
        if (phoitn) {
            __hoit_truncate(phoitn, 0);
            pfdnode->FDNODE_oftSize = 0;
        }
    }

    if(bRoot != LW_TRUE)
        LW_DEV_INC_USE_COUNT(&pfs->HOITFS_devhdrHdr);                     /*  更新计数器                  */
    /************************************ END  ************************************/
    __HOIT_VOLUME_UNLOCK(pfs);

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
static INT  __hoitFsRemove(PHOIT_VOLUME   pfs,
    PCHAR         pcName)
{
    PHOIT_INODE_INFO    phoitn;
    PHOIT_INODE_INFO    phoitFather;
    BOOL                bRoot;
    PCHAR               pcTail;
    INT                 iError;

    if (pcName == LW_NULL) {
        _ErrorHandle(ERROR_IO_NO_DEVICE_NAME_IN_PATH);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }

    //************************************ TODO ************************************
    phoitn = __hoit_open(pfs, pcName, &phoitFather, LW_NULL, &bRoot, LW_NULL, &pcTail);
    if (phoitn) {
        //TODO 建立软链接部分，PHOIT_INODE_INFO尚缺少pcLink
        if (S_ISLNK(phoitn->HOITN_mode)) {
            size_t  stLenTail = 0;
            PHOIT_FULL_DIRENT   pFullDirent = __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName);

            if (pcTail) {
                stLenTail = lib_strlen(pcTail);                         /*  确定 tail 长度              */
            }
            if (stLenTail) {                                            /*  指向其他文件                */
                PCHAR   pcSymfile = pcTail - lib_strlen(pFullDirent->HOITFD_file_name) - 1;
                PCHAR   pcPrefix;

                if (*pcSymfile != PX_DIVIDER) {
                    pcSymfile--;
                }
                if (pcSymfile == pcName) {
                    pcPrefix = LW_NULL;                                 /*  没有前缀                    */
                } else {
                    pcPrefix = pcName;
                    *pcSymfile = PX_EOS;
                }
                if (_PathBuildLink(pcName, MAX_FILENAME_LENGTH, 
                                   LW_NULL, pcPrefix, phoitn->HOITN_pcLink, pcTail) < ERROR_NONE) {
                    __HOIT_VOLUME_UNLOCK(pfs);
                    return  (PX_ERROR);                                 /*  无法建立被链接目标目录      */
                } else {
                    __HOIT_VOLUME_UNLOCK(pfs);
                    return  (FOLLOW_LINK_TAIL);
                }
            }
        }


        //TOOPT hoit_unlink 待整合
        if (S_ISDIR(phoitn->HOITN_mode)) {
            iError = __hoit_unlink_dir(phoitFather, 
                                __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName));
        } else { //TODO 尚不能识别普通文件
            iError = __hoit_unlink_regular(phoitFather,
                                __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pcName));
        }   
        __HOIT_VOLUME_UNLOCK(pfs);
        return (iError);
    } else if (bRoot) {
        if (pfs->HOITFS_bValid == LW_FALSE) {
            __HOIT_VOLUME_UNLOCK(pfs);
            return (ERROR_NONE);
        }
__re_umount_vol:
        if (LW_DEV_GET_USE_COUNT((LW_DEV_HDR *)pfs) > 1) {
            if (!pfs->HOITFS_bForceDelete) {
                __HOIT_VOLUME_UNLOCK(pfs);
                _ErrorHandle(EBUSY);
                return  (PX_ERROR);
            }

            pfs->HOITFS_bValid = LW_FALSE;

            __HOIT_VOLUME_UNLOCK(pfs);

            _DebugHandle(__ERRORMESSAGE_LEVEL, "disk have open file.\r\n");
            iosDevFileAbnormal(&pfs->HOITFS_devhdrHdr);               /*  将所有相关文件设为异常模式  */

            __HOIT_VOLUME_LOCK(pfs);
            goto    __re_umount_vol;
        } else {
            pfs->HOITFS_bValid = LW_FALSE;
        }

        iosDevDelete((LW_DEV_HDR *)pfs);                             /*  IO 系统移除设备             */
        API_SemaphoreMDelete(&pfs->HOITFS_hVolLock);
        API_SemaphoreMDelete(&pfs->HOITFS_dirtyLock);
        API_SemaphoreMDelete(&pfs->HOITFS_cleanLock);
        API_SemaphoreMDelete(&pfs->HOITFS_freeLock); 

        __hoit_unmount(pfs);
        hoit_free(pfs, pfs, sizeof(HOIT_VOLUME));

        _DebugHandle(__LOGMESSAGE_LEVEL, "hoitfs unmount ok.\r\n");

        return (ERROR_NONE);
    } else {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(ENOENT);
        return  (PX_ERROR);       
    }
    /************************************ END  ************************************/
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
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    BOOL                bRemove = LW_FALSE;

    if(phoitn->HOITN_ino == HOIT_ROOT_DIR_INO){
        return (ERROR_NONE);
    }
    /* 先得到要close的文件在hoitfs文件系统中的相对路径, 相对于hoitfs的根目录 */
    PLW_DEV_HDR    pdevhdrHdr;
    CHAR           cFullFileName[MAX_FILENAME_LENGTH] = {0};
    CHAR           cRealFileName[MAX_FILENAME_LENGTH] = {0};
//    PCHAR pcFileName = lib_rindex(pfdentry->FDENTRY_pcName, PX_DIVIDER);
//    if (pcFileName) {
//        pcFileName++;
//    }
//    else {
//        pcFileName = pfdentry->FDENTRY_pcName;
//    }

    lib_memcpy(cFullFileName, pfdentry->FDENTRY_pcName, lib_strlen(pfdentry->FDENTRY_pcName));
    /*
     *  需要在结尾处理掉多余的 / 符号, 例如: /aaa/bbb/ccc/ 应为 /aaa/bbb/ccc
     */
    UINT stFullLen = lib_strlen(cFullFileName);
    if (stFullLen > 1) {                                                /*  本身就是 "/" 不能略去       */
        if (cFullFileName[stFullLen - 1] == PX_DIVIDER) {
            cFullFileName[stFullLen - 1] =  PX_EOS;                     /*  去掉末尾的 /                */
        }
        _PathCondense(cFullFileName);                                   /*  去除 ../ ./                 */
    }

    PCHAR pcTail = LW_NULL;
    pdevhdrHdr = iosDevFind((cFullFileName), &pcTail);
    if (pdevhdrHdr == LW_NULL) {
        _ErrorHandle(ENOENT);
        return  (PX_ERROR);
    }

    /*
     *  如果 ppdevhdr == rootfs dev header 则, pcTail 起始位置没有 '/' 字符,
     *  此处需要修正由于根文件系统设备名为 "/" 产生的问题.
     */
    if (pcTail && ((*pcTail != PX_EOS) && (*pcTail != PX_DIVIDER))) {
        lib_strlcpy(&cRealFileName[1], pcTail, MAX_FILENAME_LENGTH - 1);
        cRealFileName[0] = PX_ROOT;

    } else {
        lib_strlcpy(cRealFileName, pcTail, MAX_FILENAME_LENGTH);
    }
    

    // PHOIT_INODE_INFO phoitFather;
    // PHOIT_INODE_INFO pTempInode = __hoit_open(pfs, cRealFileName, &phoitFather, LW_NULL, LW_NULL, LW_NULL, LW_NULL);

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);                                            /*  设备出错                    */
        return  (PX_ERROR);
    }
    
    if (API_IosFdNodeDec(&pfs->HOITFS_plineFdNodeHeader, 
                         pfdnode, &bRemove) == 0) {
        if (phoitn) {
            __hoit_close(phoitn, pfdentry->FDENTRY_iFlag);
        }
    }

    LW_DEV_DEC_USE_COUNT(&pfs->HOITFS_devhdrHdr);


    // if (bRemove && phoitn) {
    //     if (S_ISDIR(phoitn->HOITN_mode)) {
    //         __hoit_unlink_dir(phoitFather, 
    //                             __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pfdentry->FDENTRY_pcName));
    //     } else { //TODO 尚不能识别普通文件
    //         __hoit_unlink_regular(phoitFather,
    //                             __hoit_search_in_dents(phoitFather, phoitn->HOITN_ino, pfdentry->FDENTRY_pcName));
    //     }
    // }

    // __hoit_close(phoitFather, 0);
    // __hoit_close(pTempInode, 0);
    __HOIT_VOLUME_UNLOCK(pfs);

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
    PLW_FD_NODE         pfdnode     = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn      = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs         = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    ssize_t             sstReadNum  = PX_ERROR;

    if (!pcBuffer) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);   
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (stMaxBytes) { //TODO __hoit_read尚未添加定义
        sstReadNum = __hoit_read(phoitn, pcBuffer, stMaxBytes, (size_t)pfdentry->FDENTRY_oftPtr);
        if (sstReadNum > 0) {
            pfdentry->FDENTRY_oftPtr +=(off_t)sstReadNum;
        }
    } else {
        sstReadNum = 0;
    }

    __HOIT_VOLUME_UNLOCK(pfs);
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
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn      = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs         = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    ssize_t             sstReadNum  = PX_ERROR;

    if (!pcBuffer || (oftPos < 0)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);   
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (stMaxBytes) {
        sstReadNum = __hoit_read(phoitn, pcBuffer, stMaxBytes, (size_t)oftPos);
        /* 相比read操作少了判断修改pfdnode文件指针的步骤 */
    } else {    
        sstReadNum = 0;
    }   

    __HOIT_VOLUME_UNLOCK(pfs);
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
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn      = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs         = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    ssize_t             sstWriteNum = PX_ERROR;

    if (!pcBuffer) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);       
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (pfdentry->FDENTRY_iFlag & O_APPEND) {                           /*  追加模式                    */
        pfdentry->FDENTRY_oftPtr = pfdnode->FDNODE_oftSize;             /*  移动读写指针到末尾          */
    }

    if (stNBytes) { //TODO __hoit_write尚未添加定义
        sstWriteNum = __hoit_write(phoitn, pcBuffer, stNBytes, (size_t)pfdentry->FDENTRY_oftPtr, 1);
        if (sstWriteNum > 0) {
            pfdentry->FDENTRY_oftPtr += (off_t)sstWriteNum;             /*  更新文件指针                */
            //TODO HOITN_stSize尚未定义
            /*pfdnode->FDNODE_oftSize   = (off_t)phoitn->HOITN_stSize;*/
        }
    } else {
        sstWriteNum = 0;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

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
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    ssize_t       sstWriteNum = PX_ERROR;

    if (!pcBuffer || (oftPos < 0)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (stNBytes) { //TODO __hoit_write尚未添加定义
        /*sstWriteNum = __hoit_write(phoitn, pcBuffer, stNBytes, (size_t)oftPos, 1);*/
        if (sstWriteNum > 0) {
            //TODO HOITN_stSize尚未定义
            /*pfdnode->FDNODE_oftSize   = (off_t)phoitn->HOITN_stSize;*/
        }
    } else {
        sstWriteNum = 0;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (sstWriteNum);
}

/*********************************************************************************************************
** 函数名称: __hoitFsNRead
** 功能描述: hoitFs nread 操作
** 输　入  : pfdentry         文件控制块
**           piNRead          剩余数据量
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsNRead (PLW_FD_ENTRY  pfdentry, INT  *piNRead)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (piNRead == LW_NULL) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    } 

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    //TODO HOITN_stSize尚未定义
    /* *piNRead = (INT)(phoitn->HOITN_stSize - (size_t)pfdentry->FDENTRY_oftPtr); */

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);
}

/*********************************************************************************************************
** 函数名称: __hoitFsNRead64
** 功能描述: hoitFs nread 操作
** 输　入  : pfdentry         文件控制块
**           poftNRead        剩余数据量
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsNRead64 (PLW_FD_ENTRY  pfdentry, off_t  *poftNRead)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (poftNRead == LW_NULL) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    } 

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }   
    //TODO HOITN_stSize尚未定义
    /* *poftNRead = (off_t)(phoitn->HOITN_stSize - (size_t)pfdentry->FDENTRY_oftPtr); */

    __HOIT_VOLUME_UNLOCK(pfs);

    return (ERROR_NONE);
}

/*********************************************************************************************************
** 函数名称: __ramFsRename
** 功能描述: ramFs rename 操作
** 输　入  : pfdentry         文件控制块
**           pcNewName        新的名称
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsRename (PLW_FD_ENTRY  pfdentry, PCHAR  pcNewName)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    PHOIT_VOLUME        pfsNew;
    CHAR                cNewPath[PATH_MAX + 1];
    INT                 iError;

    
    PCHAR dirPath = (PCHAR)hoit_malloc(pfs, lib_strlen(pfdentry->FDENTRY_pcName) + 1);
    lib_bzero(dirPath, lib_strlen(pfdentry->FDENTRY_pcName) + 1);
    lib_memcpy(dirPath, pfdentry->FDENTRY_pcName, lib_strlen(pfdentry->FDENTRY_pcName));
    PCHAR pDivider = lib_rindex(dirPath, PX_DIVIDER);
    *pDivider = '\0';
    PHOIT_INODE_INFO pInodeFather = __hoit_open(pfs, dirPath, NULL, NULL, NULL, NULL, NULL);

    if (phoitn == LW_NULL) {                                             /*  检查是否为设备文件          */
        _ErrorHandle(ERROR_IOS_DRIVER_NOT_SUP);                         /*  不支持设备重命名            */
        return (PX_ERROR);
    }       

    if (pcNewName == LW_NULL) {
        _ErrorHandle(EFAULT);                                           /*  Bad address                 */
        return (PX_ERROR);
    }
    
    if (__STR_IS_ROOT(pcNewName)) {
        _ErrorHandle(ENOENT);
        return (PX_ERROR);
    }    

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (ioFullFileNameGet(pcNewName, 
                          (LW_DEV_HDR **)&pfsNew, 
                          cNewPath) != ERROR_NONE) {                    /*  获得新目录路径              */
        __HOIT_VOLUME_UNLOCK(pfs);
        return  (PX_ERROR);
    }

    if (pfsNew != pfs) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EXDEV);
        return  (PX_ERROR);        
    }

    //TODEBUG 没法获取父亲节点
    iError = __hoit_move(pInodeFather, phoitn, pcNewName);

    __HOIT_VOLUME_UNLOCK(pfs);
    hoit_free(pfs, dirPath, lib_strlen(pfdentry->FDENTRY_pcName) + 1);
    return  (iError);
}

/*********************************************************************************************************
** 函数名称: __hoitFsSeek
** 功能描述: hoitFs seek 操作
** 输　入  : pfdentry         文件控制块
**           oftOffset        偏移量
** 输　出  : 驱动相关
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsSeek (PLW_FD_ENTRY  pfdentry,
                         off_t         oftOffset)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (oftOffset > (size_t)~0) {
        _ErrorHandle(EOVERFLOW);
        return  (PX_ERROR);
    } 

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }   

    pfdentry->FDENTRY_oftPtr = oftOffset;
    //TODO HOITN_stSize尚未定义

    if (phoitn->HOITN_stSize < (size_t)oftOffset) {
        phoitn->HOITN_stSize = (size_t)oftOffset;
    }


   __HOIT_VOLUME_UNLOCK(pfs);
   return   (ERROR_NONE);
}                         

/*********************************************************************************************************
** 函数名称: __hoitFsWhere
** 功能描述: hoitFs 获得文件当前读写指针位置 (使用参数作为返回值, 与 FIOWHERE 的要求稍有不同)
** 输　入  : pfdentry            文件控制块
**           poftPos             读写指针位置
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsWhere (PLW_FD_ENTRY  pfdentry, off_t  *poftPos)
{
    if (poftPos) {
        *poftPos = (off_t)pfdentry->FDENTRY_oftPtr;
        return  (ERROR_NONE);
    }
    
    return  (PX_ERROR);
}

/*********************************************************************************************************
** 函数名称: __hoitFsStat
** 功能描述: hoitFs stat 操作
** 输　入  : pfdentry         文件控制块
**           pstat            文件状态
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsStat (PLW_FD_ENTRY  pfdentry, struct stat *pstat)
{/* 通过文件控制块fd获取文件状态 */
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (!pstat) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);        
    }

    __hoit_stat(phoitn, pfs, pstat);

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);
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
static INT  __hoitFsLStat(PHOIT_VOLUME  pfs, PCHAR  pcName, struct stat* pstat)
{/* 通过文件名获取文件状态 */
    BOOL                bRoot;
    PHOIT_INODE_INFO    phoitn;
    PHOIT_INODE_INFO    phoitnFather;

    if (!pcName || !pstat) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    phoitn = __hoit_open(pfs, pcName, &phoitnFather, LW_NULL, &bRoot, LW_NULL, LW_NULL);
    if (phoitn) {
        __hoit_stat(phoitn, pfs, pstat);
    } else if (bRoot) {
        __hoit_stat(LW_NULL, pfs, pstat);
    } else {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(ENOENT);
        return  (PX_ERROR);        
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);
}

/*********************************************************************************************************
** 函数名称: __hoitFsStatfs
** 功能描述: hoitFs statfs 操作
** 输　入  : pfdentry         文件控制块
**           pstatfs          文件系统状态
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsStatfs (PLW_FD_ENTRY  pfdentry, struct statfs *pstatfs)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (!pstatfs) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }    

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    //TODO __hoit_statfs尚未完工
    __hoit_statfs(pfs, pstatfs);

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE); 
}

/*********************************************************************************************************
** 函数名称: __hoitFsReadDir
** 功能描述: hoitFs 获得指定目录信息
** 输　入  : pfdentry            文件控制块
**           dir                 目录结构
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __hoitFsReadDir (PLW_FD_ENTRY  pfdentry, DIR  *dir)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    INT                 i;
    LONG                iStart;
    INT                 iError = ERROR_NONE;  
    PHOIT_INODE_INFO    pInodeInfo;
    if (!dir) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }   

    //TODO 还不知道下层如何遍历目录，暂时保留
    PHOIT_FULL_DIRENT pFullDirent;
    if (phoitn == LW_NULL) {
        pFullDirent = pfs->HOITFS_pRootDir->HOITN_dents;
    } else {
        if (!S_ISDIR(phoitn->HOITN_mode)) {
            __HOIT_VOLUME_UNLOCK(pfs);
            _ErrorHandle(ENOTDIR);
            return  (PX_ERROR);
        }
        pFullDirent = phoitn->HOITN_dents;
    }

    iStart = dir->dir_pos;

    for (i = 0;
         (pFullDirent != LW_NULL) && (i < iStart);
         (pFullDirent = pFullDirent->HOITFD_next), (i++));         /*  忽略                        */

    if (pFullDirent == LW_NULL) {
        _ErrorHandle(ENOENT);
        iError = PX_ERROR;                                              /*  没有多余的节点              */

    } else {
        pInodeInfo = __hoit_get_full_file(pfs, pFullDirent->HOITFD_ino);
        dir->dir_pos++;

        lib_strlcpy(dir->dir_dirent.d_name,
                    pFullDirent->HOITFD_file_name,
                    sizeof(dir->dir_dirent.d_name));

        dir->dir_dirent.d_type = IFTODT(pInodeInfo->HOITN_mode);
        dir->dir_dirent.d_shortname[0] = PX_EOS;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  iError;
}

/*********************************************************************************************************
** 函数名称: __hoitFsTimeset
** 功能描述: hoitfs 设置文件时间
** 输　入  : pfdentry            文件控制块
**           utim                utimbuf 结构
** 输　出  : < 0 表示错误
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __hoitFsTimeset (PLW_FD_ENTRY  pfdentry, struct utimbuf  *utim)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (!utim) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }    

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    } 

    if (phoitn) {
        //TODO PHOIT_INODE_INFO时间相关变量尚未定义

        /*phoitn->HOITN_timeAccess = utim->actime;
        phoitn->HOITN_timeChange = utim->modtime;*/
    } else {
        pfs->HOITFS_time = utim->modtime;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);    
}

/*********************************************************************************************************
** 函数名称: __hoitFsTruncate
** 功能描述: hoitfs 设置文件大小
** 输　入  : pfdentry            文件控制块
**           oftSize             文件大小
** 输　出  : < 0 表示错误
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __hoitFsTruncate (PLW_FD_ENTRY  pfdentry, off_t  oftSize)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    size_t              stTru;

    if (phoitn == LW_NULL) {
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);        
    }    

    if (oftSize < 0) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (oftSize > (size_t)~0) {
        _ErrorHandle(ENOSPC);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    if (S_ISDIR(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EISDIR);
        return  (PX_ERROR);
    }  

    stTru = (size_t)oftSize;

    //TODO HOITN_stSize尚未定义，__hoit_increase尚未实现
    /*if (stTru > phoitn->HOITN_stSize) {
        __hoit_increase();
    } else if (stTru < phoitn->HOITN_stSize) {
        __hoit_truncate(phoitn, stTru);
    }*/

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE); 
}

/*********************************************************************************************************
** 函数名称: __hoitFsChmod
** 功能描述: hoitfs chmod 操作
** 输　入  : pfdentry            文件控制块
**           iMode               新的 mode
** 输　出  : < 0 表示错误
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __hoitFsChmod (PLW_FD_ENTRY  pfdentry, INT  iMode)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    iMode |= S_IRUSR;
    iMode &= ~S_IFMT;/* S_IFMT是文件类型的位掩码，这意味着本函数不允许改变文件类型 */

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    if (phoitn) {
        phoitn->HOITN_mode &= S_IFMT;
        phoitn->HOITN_mode |= iMode;
    } else {
        pfs->HOITFS_mode &= S_IFMT;
        pfs->HOITFS_mode |= iMode;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);     
}

/*********************************************************************************************************
** 函数名称: __hoitFsChown
** 功能描述: hoitfs chown 操作
** 输　入  : pfdentry            文件控制块
**           pusr                新的所属用户
** 输　出  : < 0 表示错误
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __hoitFsChown (PLW_FD_ENTRY  pfdentry, LW_IO_USR  *pusr)
{
    PLW_FD_NODE         pfdnode = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_INODE_INFO    phoitn  = (PHOIT_INODE_INFO)pfdnode->FDNODE_pvFile;
    PHOIT_VOLUME        pfs     = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;

    if (!pusr) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }    
    if (phoitn) {
        phoitn->HOITN_uid = pusr->IOU_uid;
        phoitn->HOITN_gid = pusr->IOU_gid;
    } else {
        pfs->HOITFS_uid = pusr->IOU_uid;
        pfs->HOITFS_gid = pusr->IOU_gid;
    }

    __HOIT_VOLUME_UNLOCK(pfs);

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
static INT  __hoitFsSymlink(PHOIT_VOLUME   pfs,
    PCHAR         pcName,
    CPCHAR        pcLinkDst)
{

    BOOL                bRoot;
    PHOIT_INODE_INFO    phoitn;
    PHOIT_INODE_INFO    phoitnFather;

    if (!pcName || !pcLinkDst) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__fsCheckFileName(pcName)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    phoitn = __hoit_open(pfs, pcName, &phoitnFather, LW_NULL, &bRoot, LW_NULL, LW_NULL);
    if (phoitn || bRoot) { /* 同名文件存在或为根节点 */
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EEXIST);
        return  (PX_ERROR);        
    }
    
    //TODO 软链接建立功能尚未实现
    phoitn = __hoit_maken(pfs, pcName, phoitnFather, S_IFLNK | DEFAULT_SYMLINK_PERM, pcLinkDst);
    if (phoitn == LW_NULL) {
        __HOIT_VOLUME_UNLOCK(pfs);
        return  (PX_ERROR);
    }

    __HOIT_VOLUME_UNLOCK(pfs);

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
static ssize_t __hoitFsReadlink(PHOIT_VOLUME   pfs,
    PCHAR         pcName,
    PCHAR         pcLinkDst,
    size_t        stMaxSize)
{

    size_t      stLen;
    PHOIT_INODE_INFO    phoitn;

    if (!pcName || !pcLinkDst || !stMaxSize) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    phoitn = __hoit_open(pfs, pcName, LW_NULL, LW_NULL, LW_NULL, LW_NULL, LW_NULL);
    if ((phoitn == LW_NULL) || !S_ISLNK(phoitn->HOITN_mode)) {
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(ENOENT);
        return  (PX_ERROR);
    }
    
    //TODO 尚未定义HOITN_pcLink，软链接部分功能也尚未实现
    
    stLen = lib_strlen(phoitn->HOITN_pcLink);
    lib_strncpy(pcLinkDst, phoitn->HOITN_pcLink, stMaxSize);
    

    if (stLen > stMaxSize) {
        stLen = stMaxSize;
    }
    __HOIT_VOLUME_UNLOCK(pfs);

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
    PLW_FD_NODE    pfdnode  = (PLW_FD_NODE)pfdentry->FDENTRY_pfdnode;
    PHOIT_VOLUME   pfs      = (PHOIT_VOLUME)pfdnode->FDNODE_pvFsExtern;
    off_t         oftTemp;
    INT           iError;

    switch (iRequest) {
    case FIOCONTIG:
    case FIOTRUNC:
    case FIOLABELSET:
    case FIOATTRIBSET:
        if ((pfdentry->FDENTRY_iFlag & O_ACCMODE) == O_RDONLY) {
            _ErrorHandle(ERROR_IO_WRITE_PROTECTED);
            return  (PX_ERROR);
        }
	}
    
	switch (iRequest) {
	
	case FIODISKINIT:                                                   /*  磁盘初始化                  */
        return  (ERROR_NONE);
        
    case FIOSEEK:                                                       /*  文件重定位                  */
        oftTemp = *(off_t *)lArg;
        return  (__hoitFsSeek(pfdentry, oftTemp));

    case FIOWHERE:                                                      /*  获得文件当前读写指针        */
        iError = __hoitFsWhere(pfdentry, &oftTemp);
        if (iError == PX_ERROR) {
            return  (PX_ERROR);
        } else {
            *(off_t *)lArg = oftTemp;
            return  (ERROR_NONE);
        }
        
    case FIONREAD:                                                      /*  获得文件剩余字节数          */
        return  (__hoitFsNRead(pfdentry, (INT *)lArg));
        
    case FIONREAD64:                                                    /*  获得文件剩余字节数          */
        iError = __hoitFsNRead64(pfdentry, &oftTemp);
        if (iError == PX_ERROR) {
            return  (PX_ERROR);
        } else {
            *(off_t *)lArg = oftTemp;
            return  (ERROR_NONE);
        }

    case FIORENAME:                                                     /*  文件重命名                  */
        return  (__hoitFsRename(pfdentry, (PCHAR)lArg));
    
    case FIOLABELGET:                                                   /*  获取卷标                    */
    case FIOLABELSET:                                                   /*  设置卷标                    */
        _ErrorHandle(ENOSYS);
        return  (PX_ERROR);
    
    case FIOFSTATGET:                                                   /*  获得文件状态                */
        return  (__hoitFsStat(pfdentry, (struct stat *)lArg));
    
    case FIOFSTATFSGET:                                                 /*  获得文件系统状态            */
        return  (__hoitFsStatfs(pfdentry, (struct statfs *)lArg));
    
    case FIOREADDIR:                                                    /*  获取一个目录信息            */
        return  (__hoitFsReadDir(pfdentry, (DIR *)lArg));
    
    case FIOTIMESET:                                                    /*  设置文件时间                */
        return  (__hoitFsTimeset(pfdentry, (struct utimbuf *)lArg));
        
    case FIOTRUNC:                                                      /*  改变文件大小                */
        oftTemp = *(off_t *)lArg;
        return  (__hoitFsTruncate(pfdentry, oftTemp));
    
    case FIOSYNC:                                                       /*  将文件缓存回写              */
    case FIOFLUSH:
    case FIODATASYNC:
        hoitFlushCache(pfs->HOITFS_cacheHdr, (PHOIT_CACHE_BLK)-1);
        return  (ERROR_NONE);
        
    case FIOCHMOD:
        return  (__hoitFsChmod(pfdentry, (INT)lArg));                    /*  改变文件访问权限            */
    
    case FIOSETFL:                                                      /*  设置新的 flag               */
        if ((INT)lArg & O_NONBLOCK) {
            pfdentry->FDENTRY_iFlag |= O_NONBLOCK;
        } else {
            pfdentry->FDENTRY_iFlag &= ~O_NONBLOCK;
        }
        return  (ERROR_NONE);
    
    case FIOCHOWN:                                                      /*  修改文件所属关系            */
        return  (__hoitFsChown(pfdentry, (LW_IO_USR *)lArg));
    
    case FIOFSTYPE:                                                     /*  获得文件系统类型            */
        *(PCHAR *)lArg = "RAM FileSystem";
        return  (ERROR_NONE);
    
    case FIOGETFORCEDEL:                                                /*  强制卸载设备是否被允许      */
        *(BOOL *)lArg = pfs->HOITFS_bForceDelete;
        return  (ERROR_NONE);
        
#if LW_CFG_FS_SELECT_EN > 0
    case FIOSELECT:
        if (((PLW_SEL_WAKEUPNODE)lArg)->SELWUN_seltypType != SELEXCEPT) {
            SEL_WAKE_UP((PLW_SEL_WAKEUPNODE)lArg);                      /*  唤醒节点                    */
        }
        return  (ERROR_NONE);
         
    case FIOUNSELECT:
        if (((PLW_SEL_WAKEUPNODE)lArg)->SELWUN_seltypType != SELEXCEPT) {
            LW_SELWUN_SET_READY((PLW_SEL_WAKEUPNODE)lArg);
        }
        return  (ERROR_NONE);
#endif                                                                  /*  LW_CFG_FS_SELECT_EN > 0     */
        
    default:
        _ErrorHandle(ENOSYS);
        return  (PX_ERROR);
    }

}

/*********************************************************************************************************
** 函数名称: __hoitFsHardlink
** 功能描述: hoitFs 创建硬链接文件
** 输　入  : pfs                 hoitfs 文件系统
**           pcName              链接原始文件名
**           pcLinkDst           链接目标文件名
**           stMaxSize           缓冲大小
** 输　出  : < 0 表示错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoitFsHardlink(PHOIT_VOLUME   pfs,
    PCHAR         pcName,
    CPCHAR        pcLinkDst)
{

    BOOL                bRoot;
    PHOIT_INODE_INFO    phoitn;
    PHOIT_INODE_INFO    phoitnFather;

    if (!pcName || !pcLinkDst) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (__fsCheckFileName(pcName)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    PLW_DEV_HDR    pdevhdrHdr;
    CHAR           cFullFileName[MAX_FILENAME_LENGTH] = { 0 };
    CHAR           cFullLinkDst[MAX_FILENAME_LENGTH] = { 0 };

    API_IoFullFileNameGet(pcName, &pdevhdrHdr, cFullFileName);
    API_IoFullFileNameGet(pcLinkDst, &pdevhdrHdr, cFullLinkDst);

    if (__HOIT_VOLUME_LOCK(pfs) != ERROR_NONE) {
        _ErrorHandle(ENXIO);
        return  (PX_ERROR);
    }

    phoitn = __hoit_open(pfs, cFullFileName, &phoitnFather, LW_NULL, &bRoot, LW_NULL, LW_NULL);
    if (phoitn || bRoot) { /* 同名文件存在或为根节点 */
        __HOIT_VOLUME_UNLOCK(pfs);
        _ErrorHandle(EEXIST);
        return  (PX_ERROR);
    }

    //建立硬链接
    phoitn = __hoit_maken(pfs, cFullFileName, phoitnFather, S_IFSOCK | DEFAULT_SYMLINK_PERM, cFullLinkDst);
    if (phoitn == LW_NULL) {
        __HOIT_VOLUME_UNLOCK(pfs);
        return  (PX_ERROR);
    }

    __HOIT_VOLUME_UNLOCK(pfs);

    return  (ERROR_NONE);
}

//#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
#endif // HOITFS_DISABLE
