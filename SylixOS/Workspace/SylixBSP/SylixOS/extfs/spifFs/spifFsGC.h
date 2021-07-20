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
** 文   件   名: spifFsGC.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 13日
**
** 描        述: Spiffs文件系统垃圾回收器
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSGC_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSGC_H_
#include "spifFsType.h"
#define SPIFFS_GC_MAX_RUNS 5

typedef enum {
  FIND_OBJ_DATA,
  MOVE_OBJ_DATA,
  MOVE_OBJ_IX,
  FINISHED
} SPIFFS_GC_CLEAN_STATE;

typedef struct spiffs_gc{
  SPIFFS_GC_CLEAN_STATE state;
  SPIFFS_OBJ_ID objIdCur;
  SPIFFS_SPAN_IX spanIXObjIXCur;
  SPIFFS_PAGE_IX pageIXObjIXCur;
  SPIFFS_PAGE_IX pageIXDataCur;
  INT iStoredScanEntryIndex;
  UINT8 uiObjIdFound;
} SPIFFS_GC;


INT32 spiffsPageMove(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PUCHAR pucPageData,
                     SPIFFS_OBJ_ID objId, PSPIFFS_PAGE_HEADER pPageHeader, SPIFFS_PAGE_IX pageIXSrc,
                     SPIFFS_PAGE_IX *pPageIXDst);
INT32 spiffsGCCheck(PSPIFFS_VOLUME pfs, UINT32 uiLen);
INT32 spiffsGCClean(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIX);
INT32 spiffsGCQuick(PSPIFFS_VOLUME pfs, UINT16 uiMaxFreePages);
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSGC_H_ */