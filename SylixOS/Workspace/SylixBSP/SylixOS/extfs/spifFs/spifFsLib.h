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
** 文   件   名: spifFsLib.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统核心
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSLIB_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSLIB_H_
#include "spifFsType.h"

/*********************************************************************************************************
 * SPIFFS Vistor相关定义
*********************************************************************************************************/
// check id, only visit matching objec ids
#define SPIFFS_VIS_CHECK_ID     (1<<0)
// report argument object id to visitor - else object lookup id is reported
#define SPIFFS_VIS_CHECK_PH     (1<<1)
// stop searching at end of all look up pages
#define SPIFFS_VIS_NO_WRAP      (1<<2)

// visitor result, continue searching
#define SPIFFS_VIS_COUNTINUE            (SPIFFS_ERR_INTERNAL - 20)
// visitor result, continue searching after reloading lu buffer
#define SPIFFS_VIS_COUNTINUE_RELOAD     (SPIFFS_ERR_INTERNAL - 21)
// visitor result, stop searching
#define SPIFFS_VIS_END                  (SPIFFS_ERR_INTERNAL - 22)

/*********************************************************************************************************
 * SPIFFS 事件相关定义
*********************************************************************************************************/
// updating an object index contents
#define SPIFFS_EV_IX_UPD                (0)
// creating a new object index
#define SPIFFS_EV_IX_NEW                (1)
// deleting an object index
#define SPIFFS_EV_IX_DEL                (2)
// moving an object index without updating contents
#define SPIFFS_EV_IX_MOV                (3)
// updating an object index header data only, not the table itself
#define SPIFFS_EV_IX_UPD_HDR            (4)


typedef INT32 (*spiffsVisitorFunc)(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                   const PVOID pUserConst, PVOID pUserVar);

/*********************************************************************************************************
 * SPIFFS 库函数定义
*********************************************************************************************************/

INT32 spiffsPhysCpy(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, UINT32 uiDst,
                    UINT32 uiSrc, UINT32 uiLen);
/*********************************************************************************************************
 * SPIFFS Lookup Vistor相关
*********************************************************************************************************/
INT32 spiffsObjLookUpFindEntryVisitor(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                                      UINT8 flags, SPIFFS_OBJ_ID objId, spiffsVisitorFunc vistor,
                                      const PVOID pUserConst, PVOID pUserVar, SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry);
INT32 spiffsObjLookUpScan(PSPIFFS_VOLUME pfs);
INT32 spiffsObjLookUpFindFreeObjId(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID *pObjId, const PCHAR pucConflictingName);
INT32 spiffsObjLookUpFindId(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                            SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry);
INT32 spiffsObjLookUpFindFreeEntry(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                                   SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry);
INT32 spiffsObjLookUpFindIdAndSpan(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_SPAN_IX spanIX,
                                   SPIFFS_PAGE_IX pageIXExclusion, SPIFFS_PAGE_IX *pPageIX);
INT32 spiffsObjectFindObjectIndexHeaderByName(PSPIFFS_VOLUME pfs, UCHAR ucName[SPIFFS_OBJ_NAME_LEN], SPIFFS_PAGE_IX *pPageIX);

/*********************************************************************************************************
 * SPIFFS 页面工具
*********************************************************************************************************/
INT32 spiffsPageIndexCheck(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd, SPIFFS_PAGE_IX pageIX, SPIFFS_SPAN_IX spanIX);
INT32 spiffsPageDataCheck(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd,  SPIFFS_PAGE_IX pageIX, SPIFFS_SPAN_IX spanIX);
INT32 spiffsPageDelete(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX);
INT32 spiffsPageAllocateData(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, PSPIFFS_PAGE_HEADER pPageHeader,
                             PCHAR pData, UINT32 uiLen, UINT32 uiPageOffs, BOOL bIsFinalize,
                             SPIFFS_PAGE_IX *pageIX);
/*********************************************************************************************************
 * SPIFFS Object 相关
*********************************************************************************************************/
INT32 spiffsObjectUpdateIndexHdr(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd, SPIFFS_OBJ_ID objId, SPIFFS_PAGE_IX pageIXObjIXHdr,
                                 PCHAR pucNewObjIXHdrData , const UCHAR ucName[], UINT32 uiSize, SPIFFS_PAGE_IX *pageIXNew);
INT32 spiffsObjectOpenByPage(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX, PSPIFFS_FD pFd, SPIFFS_FLAGS flags, SPIFFS_MODE mode);

INT32 spiffsObjectCreate(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId,
                         const UCHAR cPath[], SPIFFS_OBJ_TYPE type, SPIFFS_PAGE_IX* pObjIndexHdrPageIX);
INT32 spiffsObjectTruncate(PSPIFFS_FD pFd, UINT32 uiNewSize, BOOL bIsRemoveFull); 
//DONE：--spiffsObjectAppend(完成)--、--spiffsObjectModify(完成)--、--spiffsObjectRead(完成)--、 
//DONE: --spiffsFileWrite(完成)--、 --spiffsFileRead(完成)--
INT32 spiffsObjectAppend(PSPIFFS_FD pFd, UINT32 uiOffset, PCHAR pContent, UINT32 uiLen);
INT32 spiffsObjectModify(PSPIFFS_FD pFd, UINT32 uiOffset, PCHAR pContent, UINT32 uiLen);
INT32 spiffsObjectRead(PSPIFFS_FD pFd, UINT32 uiOffset, UINT32 uiLen, PCHAR puDst);
/*********************************************************************************************************
 * SPIFFS 其他工具
*********************************************************************************************************/
INT32        spiffsStatPageIX(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX, SPIFFS_FILE fileHandler, PSPIFFS_STAT pStat);
SPIFFS_FLAGS spiffsTranslateToSylixOSFlag(INT iFlag);
/*********************************************************************************************************
 * SPIFFS 读写相关
*********************************************************************************************************/
INT32           spiffsFileWrite(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PCHAR pcContent, UINT32 uiOffset, INT32 iLen);
INT32           spiffsFileRead(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PCHAR pcContent, INT32 iLen);
PSPIFFS_DIRENT  spiffsDirRead(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent);
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSLIB_H_ */