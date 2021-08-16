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
** 文   件   名: spifFsGlue.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 09日
**
** 描        述: Spiffs文件系统胶水层，即上层实现
*********************************************************************************************************/

#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSGLUE_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSGLUE_H_
#include "spifFsType.h"
/*********************************************************************************************************
 * SPIFFS 上层函数定义
*********************************************************************************************************/

INT32       __spiffs_format(PSPIFFS_VOLUME pfs);
INT32       __spiffs_probe_fs(PSPIFFS_CONFIG pConfig);
INT32       __spiffs_mount(PSPIFFS_VOLUME pfs, PSPIFFS_CONFIG pConfig, PUCHAR pucWorkBuffer,
                           UINT8 *puiFdSpace, UINT32 uiFdSpaceSize,
                           PUCHAR pCache, UINT32 uiCacheSize,
                           spiffsCheckCallback checkCallbackFunc);
        
VOID        __spiffs_unmount(PSPIFFS_VOLUME pfs);
INT32       __spiffs_create(PSPIFFS_VOLUME pfs, const PCHAR pcPath, SPIFFS_MODE mode);
SPIFFS_FILE __spiffs_open(PSPIFFS_VOLUME pfs, const PCHAR pcPath, SPIFFS_FLAGS flags, SPIFFS_MODE mode);
SPIFFS_FILE __spiffs_open_by_dirent(PSPIFFS_VOLUME pfs, PSPIFFS_DIRENT pDirent, SPIFFS_FLAGS flags, SPIFFS_MODE mode);
INT32       __spiffs_read(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler,  PCHAR pcContent, INT32 iLen);
INT32       __spiffs_write(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler,  PCHAR pcContent, INT32 iLen);
INT32       __spiffs_lseek(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, UINT32 uiOffset, INT iSeekFlag);

INT32       __spiffs_rename(PSPIFFS_VOLUME pfs, const PCHAR pcOldPath, const PCHAR pcNewPath);
INT32       __spiffs_remove(PSPIFFS_VOLUME pfs, const PCHAR pcPath);
INT32       __spiffs_fremove(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler);

INT32       __spiffs_stat(PSPIFFS_VOLUME pfs, const PCHAR pcPath, PSPIFFS_STAT pStat); 
INT32       __spiffs_fstat(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PSPIFFS_STAT pStat);
INT32       __spiffs_fflush(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler);
INT32       __spiffs_close(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler);
INT32       __spiffs_gc_quick(PSPIFFS_VOLUME pfs, UINT16 uiMaxFreePages);
INT32       __spiffs_gc(PSPIFFS_VOLUME pfs, UINT32 uiSize);
INT32       __spiffs_set_file_callback_func(PSPIFFS_VOLUME pfs, spiffsFileCallback fileCallBackFunc);

PSPIFFS_DIR    __spiffs_opendir(PSPIFFS_VOLUME pfs, const PCHAR pcName, PSPIFFS_DIR pDir);
INT32          __spiffs_closedir(PSPIFFS_DIR pDir);
PSPIFFS_DIRENT __spiffs_readdir(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent);

/*********************************************************************************************************
 * SylixOS 适配 
*********************************************************************************************************/
INT            __spif_mount(PSPIF_VOLUME pfs);
INT            __spif_unmount(PSPIF_VOLUME pfs);
PSPIFN_NODE    __spif_open(PSPIF_VOLUME pfs, PCHAR pcName, INT iFlags, INT iMode, BOOL *pbIsRoot);
INT            __spif_close(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn);
INT            __spif_remove(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn);
INT            __spif_stat(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, struct stat* pstat);

INT            __spif_read(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR pcContent, UINT32 uiOffset, UINT32 uiLen);
INT            __spif_write(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR pcContent, UINT32 uiOffset, UINT32 uiLen);
INT            __spif_rename(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR  pcNewName);
INT            __spif_lseek(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, UINT32 uiOffset);

INT            __spif_statfs(PSPIF_VOLUME pfs, struct statfs *pstatfs);
PSPIFFS_DIR    __spif_opendir(PSPIF_VOLUME pfs, const PCHAR pcName, PSPIFFS_DIR pDir);
PSPIFFS_DIRENT __spif_readdir(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent);
INT            __spif_closedir(PSPIF_VOLUME pfs, PSPIFFS_DIR pDir);
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSGLUE_H_ */