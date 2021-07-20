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
** 文   件   名: spifFsFDLib.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 10日
**
** 描        述: Spiffs文件句柄管理库
*********************************************************************************************************/

#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSFDLIB_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSFDLIB_H_
#include "spifFsType.h"

INT32 spiffsFdGet(PSPIFFS_VOLUME pfs, SPIFFS_FILE file, PSPIFFS_FD *ppfd);
INT32 spiffsFdReturn(PSPIFFS_VOLUME pfs, SPIFFS_FILE file);
INT32 spiffsFdFindNew(PSPIFFS_VOLUME pfs, PSPIFFS_FD *ppfd, const PCHAR pcName); 
VOID  spiffsCBObjectEvent(PSPIFFS_VOLUME pfs, PSPIFFS_PAGE_OBJECT_IX objIX, INT ev,
                          SPIFFS_OBJ_ID objIdRaw, SPIFFS_SPAN_IX spanIX, SPIFFS_PAGE_IX pageIXNew,
                          UINT uiNewSize);
VOID  spiffsFdTemporalCacheRehash(PSPIFFS_VOLUME pfs, const PCHAR pcOldPath, const PCHAR pcNewPath);
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSFDLIB_H_ */