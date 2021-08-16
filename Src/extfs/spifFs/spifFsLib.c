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
** 文   件   名: spifFsLib.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统核心
*********************************************************************************************************/
#include "spifFsLib.h"
#include "spifFsCache.h"
#include "spifFsGC.h"
#include "spifFsFDLib.h"

INT32 spiffsPhysCpy(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, UINT32 uiDst,
                    UINT32 uiSrc, UINT32 uiLen) {
    (void)fileHandler;
    INT32 iRes;
    UCHAR ucBuffer[SPIFFS_COPY_BUFFER_STACK];
    while (uiLen > 0) {
        UINT32 uiChunkSize = MIN(SPIFFS_COPY_BUFFER_STACK, uiLen);
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_MOVS, fileHandler, 
                               uiSrc, uiChunkSize, ucBuffer);
        SPIFFS_CHECK_RES(iRes);
        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_MOVD,  fileHandler, 
                                uiDst, uiChunkSize, ucBuffer);
        SPIFFS_CHECK_RES(iRes);
        uiLen -= uiChunkSize;
        uiSrc += uiChunkSize;
        uiDst += uiChunkSize;
    }
    return SPIFFS_OK;
}
/*********************************************************************************************************
** 函数名称: __spiffsObjLookUpFindIdAndSpanVistor
** 功能描述: 寻找ID，及其对应的Span
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsObjLookUpFindIdAndSpanVistor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                           const PVOID pUserConst, PVOID pUserVar){
    INT32 iRes;
    SPIFFS_PAGE_HEADER pageHeader;
    SPIFFS_PAGE_IX pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iLookUpEntryIX);
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                           sizeof(SPIFFS_PAGE_HEADER), (PCHAR)&pageHeader);
    SPIFFS_CHECK_RES(iRes);
    //TODO:什么意思？？？
    if(pageHeader.objId == objId &&
       pageHeader.spanIX == *((SPIFFS_SPAN_IX *)pUserVar) &&
       (pageHeader.flags & (SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_USED)) == SPIFFS_PH_FLAG_DELET &&
       !((objId & SPIFFS_OBJ_ID_IX_FLAG) &&                                             /* 不是Obj IX */
       (pageHeader.flags & SPIFFS_PH_FLAG_IXDELE) == 0 && pageHeader.spanIX == 0) &&    /* 且没有被删，且不是第0个Span */
       (pUserConst == LW_NULL || *((const SPIFFS_PAGE_IX *)pUserConst) != pageIX)){     
        return SPIFFS_OK;
    }
    else {
        return SPIFFS_VIS_COUNTINUE;
    }
}
/*********************************************************************************************************
** 函数名称: __spiffsObjLookUpScanVistor
** 功能描述: 扫描并统计Page信息的Vistor
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsObjLookUpScanVistor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                  const PVOID pUserConst, PVOID pUserVar){
    (VOID) blkIX;
    (VOID) pUserConst;
    (VOID) pUserVar;

    if(objId == SPIFFS_OBJ_ID_FREE){
        if(iLookUpEntryIX == 0){        /* 一个块的首objId为空，默认该块为空，如何理解？事实上很简单，LFS使然*/
            pfs->uiFreeBlks++;
        }
    }
    else if(objId == SPIFFS_OBJ_ID_DELETED){
        pfs->uiStatsPageDeleted++;
    }
    else {
        pfs->uiStatsPageAllocated++;
    }

    return SPIFFS_VIS_COUNTINUE;
}
/*********************************************************************************************************
** 函数名称: __spiffsObjLookUpFindFreeObjIdBitmapVistor
** 功能描述: 用位于WorkBuffer的内存收集有效块信息
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsObjLookUpFindFreeObjIdBitmapVistor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                                 const PVOID pUserConst, PVOID pUserVar){
    SPIFFS_OBJ_ID objIdMin; 
    PCHAR pucConflictingName;
    UINT32 bitIX;
    UINT32 byteIX;
    SPIFFS_PAGE_IX pageIX;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    INT32 iRes = SPIFFS_OK;

    if(objId != SPIFFS_OBJ_ID_FREE && objId != SPIFFS_OBJ_ID_DELETED){
       objIdMin = *((SPIFFS_OBJ_ID*)pUserVar);
        pucConflictingName = (const PCHAR)pUserConst;
        /* 找到一个标记为Index的Entry */
        if(pucConflictingName && (objId & SPIFFS_OBJ_ID_IX_FLAG)){
            pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iLookUpEntryIX);
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                   SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                                   sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), &objIXHdr);
            SPIFFS_CHECK_RES(iRes);
            if(objIXHdr.pageHdr.spanIX == 0 &&
               (objIXHdr.pageHdr.flags & (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_IXDELE)) 
               == (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_IXDELE)){
                if(lib_strcmp((const PCHAR)pUserConst, (PCHAR)objIXHdr.ucName) == 0){       /* 文件名冲突 */
                    return SPIFFS_ERR_CONFLICTING_NAME;
                }
            }
        }

        objId   &= ~SPIFFS_OBJ_ID_IX_FLAG;
        bitIX   = (objId - objIdMin) & 0x111;
        byteIX  = (objId - objIdMin) >> 3;
        if(byteIX >= 0 && byteIX < SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)){
            pfs->pucWorkBuffer[byteIX] |= (1 << bitIX);
        }
    }
    return SPIFFS_VIS_COUNTINUE;
}
/*********************************************************************************************************
** 函数名称: __spiffsObjLookUpFindFreeObjIdCompactVistor
** 功能描述: 用位于WorkBuffer的内存收集有效块信息，重用压缩收集
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsObjLookUpFindFreeObjIdCompactVistor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                                  const PVOID pUserConst, PVOID pUserVar){
    (VOID) pUserVar;
    INT32 iRes = SPIFFS_OK;
    const PSPIFFS_FREE_OBJ_ID_STATE pState = (const PSPIFFS_FREE_OBJ_ID_STATE)pUserConst;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    SPIFFS_PAGE_IX pageIX;
    PCHAR pucObjBitMap;
    UINT   uiIX;

    if(objId != SPIFFS_OBJ_ID_FREE && objId != SPIFFS_OBJ_ID_DELETED 
       && (objId & SPIFFS_OBJ_ID_IX_FLAG)){
        pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iLookUpEntryIX);
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                               SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                               sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), &objIXHdr);
        SPIFFS_CHECK_RES(iRes);
        if(objIXHdr.pageHdr.spanIX == 0 &&
           (objIXHdr.pageHdr.flags & (SPIFFS_PH_FLAG_INDEX | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_DELET))
           == (SPIFFS_PH_FLAG_DELET)){
            if(pState->pucConflictingName &&
               lib_strcmp((const PCHAR)pUserConst, (PCHAR)objIXHdr.ucName) == 0){       /* 文件名冲突 */
                return SPIFFS_ERR_CONFLICTING_NAME;
            }

            objId &= ~SPIFFS_OBJ_ID_IX_FLAG;
            if(objId >= pState->objIdMin && objId <= pState->objIdMax){
                pucObjBitMap = (PCHAR)(pfs->pucWorkBuffer);
                /* (id - min) * page_sz  / (max - min)*/
                uiIX = (objId - pState->objIdMin) / pState->uiCompaction;   /* 重叠的第几个页 */
                SPIFFS_DBG("free_obj_id: add ix "_SPIPRIi" for id "_SPIPRIid" min"_SPIPRIid" max"_SPIPRIid" comp:"_SPIPRIi"\n", 
                           uiIX, objId, pState->objIdMin, pState->objIdMax, pState->uiCompaction);
                pucObjBitMap[uiIX]++;
            }
        }
    }
    return SPIFFS_VIS_COUNTINUE;
}

INT32 __spiffsObjectFindObjectIndexHeaderByNameVisitor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                                                       const PVOID pUserConst, PVOID pUserVar){
    (VOID)pUserVar;
    INT32 iRes;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    SPIFFS_PAGE_IX pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iLookUpEntryIX);
    if (objId == SPIFFS_OBJ_ID_FREE || objId == SPIFFS_OBJ_ID_DELETED ||
        (objId & SPIFFS_OBJ_ID_IX_FLAG) == 0) {
        return SPIFFS_VIS_COUNTINUE;
    }
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX), sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)
                           ,(PCHAR)&objIXHdr);
    SPIFFS_CHECK_RES(iRes);
    if (objIXHdr.pageHdr.spanIX == 0 &&
        (objIXHdr.pageHdr.flags & (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_IXDELE)) ==
            (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_IXDELE)) {
        if (strcmp((const PCHAR)pUserConst, (PCHAR)objIXHdr.ucName) == 0) {
            return SPIFFS_OK;
        }
    }

    return SPIFFS_VIS_COUNTINUE;
}
INT32 __spiffsReadDirVisitor(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX blkIX, INT iLookUpEntryIX, 
                             const PVOID pUserConst, PVOID pUserVar) {
    (VOID)pUserConst;
    INT32 iRes;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    if (objId == SPIFFS_OBJ_ID_FREE || objId == SPIFFS_OBJ_ID_DELETED ||
        (objId & SPIFFS_OBJ_ID_IX_FLAG) == 0) {
        return SPIFFS_VIS_COUNTINUE;
    }

    SPIFFS_PAGE_IX pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iLookUpEntryIX);
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                            SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                            sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), (PCHAR)&objIXHdr);
    if (iRes != SPIFFS_OK) 
        return iRes;
    if ((objId & SPIFFS_OBJ_ID_IX_FLAG) &&
        objIXHdr.pageHdr.spanIX == 0 &&
        (objIXHdr.pageHdr.flags & (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_IXDELE)) ==
        (SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_IXDELE)) {
        PSPIFFS_DIRENT pDirent = (PSPIFFS_DIRENT)pUserVar;
        pDirent->objId = objId;
        lib_strcpy((PCHAR)pDirent->ucName, (PCHAR)objIXHdr.ucName);
        pDirent->objType = objIXHdr.type;
        pDirent->uiSize = objIXHdr.uiSize == SPIFFS_UNDEFINED_LEN ? 0 : objIXHdr.uiSize;
        pDirent->pageIX = pageIX;
        return SPIFFS_OK;
    }
    return SPIFFS_VIS_COUNTINUE;
}

/*********************************************************************************************************
** 函数名称: spiffsObjLookUpFindEntryVisitor
** 功能描述: 用Vistor来访问每个Entry，访问到objId时会发生回调，flags也与回调有关
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpFindEntryVisitor(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                                      UINT8 flags, SPIFFS_OBJ_ID objId, spiffsVisitorFunc vistor,
                                      const PVOID pUserConst, PVOID pUserVar, SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry){
    INT32           iRes               = SPIFFS_OK;
    INT32           iEntryCount        = pfs->uiBlkCount * SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs);  /* 总共最多的Entry */
    SPIFFS_BLOCK_IX blkIXCur           = blkIXStarting;
    UINT32          uiBlkCurAddr       = SPIFFS_BLOCK_TO_PADDR(pfs, blkIXCur);
    SPIFFS_OBJ_ID   *pObjLookUpBuffer  = (SPIFFS_OBJ_ID *)pfs->pucLookupWorkBuffer;
    INT             iEntryCur          = iLookUpEntryStarting;
    INT             iEntriesPerPage    = SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(SPIFFS_OBJ_ID); /* LookUp Page一个Page可以有多少个Entry */
    
    SPIFFS_PAGE_IX  pageIXOffset;   /* 相对Blk LookUp页面，1 ~ SPIFFS_OBJ_LOOKUP_PAGES */
    INT             iEntryOffset;              

    /* 超出了一个块的最大Entry数 - 1，略过Erase Count，检查下一个块 */
    if(iEntryCur > (INT)SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs) - 1){
        iEntryCur = 0;
        blkIXCur++;
        uiBlkCurAddr = SPIFFS_BLOCK_TO_PADDR(pfs, blkIXCur);
        if(blkIXCur >= pfs->uiBlkCount){
            if(flags & SPIFFS_VIS_NO_WRAP){
                return SPIFFS_VIS_END;
            }
            else {
                iEntryCur = 0;
                blkIXCur = 0;
            }
        }
    }

    /* 遍历每个Blk */
    while (iRes == SPIFFS_OK && iEntryCount > 0)    
    {
        pageIXOffset = iEntryCur / iEntriesPerPage;  
        /* 查看每个LookUp Page */
        while (iRes == SPIFFS_OK && pageIXOffset < (INT)SPIFFS_OBJ_LOOKUP_PAGES(pfs))
        {
            iEntryOffset = pageIXOffset * iEntriesPerPage;
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                   uiBlkCurAddr + pageIXOffset * SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), 
                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);

            /* 查看LookUp Page上的所有Entry */
            while (iRes == SPIFFS_OK && 
                   iEntryCur < (INT)SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs) &&   /* 最后一个页面 */
                   iEntryCur - iEntryOffset < iEntriesPerPage)              /* 非最后一个页面 */
            {
                if((flags & SPIFFS_VIS_CHECK_ID) == 0 || 
                   pObjLookUpBuffer[iEntryCur - iEntryOffset] == objId){
                    if(pBlkIX){
                        *pBlkIX = blkIXCur;
                    } 
                    if(piLookUpEntry){
                        *piLookUpEntry = iEntryCur;
                    }
                    if(vistor){
                        iRes = vistor(pfs, 
                                      (flags & SPIFFS_VIS_CHECK_PH) ? objId : pObjLookUpBuffer[iEntryCur - iEntryOffset],
                                      blkIXCur,
                                      iEntryCur, 
                                      pUserConst,
                                      pUserVar);
                        if(iRes == SPIFFS_VIS_COUNTINUE || iRes == SPIFFS_VIS_COUNTINUE_RELOAD) {
                            if(iRes == SPIFFS_VIS_COUNTINUE_RELOAD){
                                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                                       uiBlkCurAddr + pageIXOffset * SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), 
                                                       SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);
                                SPIFFS_CHECK_RES(iRes);
                            }
                            iRes = SPIFFS_OK;
                            iEntryCur++;
                            iEntryCount--;
                            continue;
                        }
                        else {
                            return iRes;
                        }
                    }
                    else {
                        //TODO: 为啥不是返回 SPIFFS_VIS_END
                        return SPIFFS_OK;
                    }
                }
                iEntryCount--;
                iEntryCur++;
            } /* 结束遍历每个Look Up Page的Entry */
            pageIXOffset++;
        } /* 结束遍历每个Look Up Page */
        iEntryCur = 0;
        blkIXCur++;
        uiBlkCurAddr = SPIFFS_BLOCK_TO_PADDR(pfs, blkIXCur);
        if(blkIXCur >= pfs->uiBlkCount){
            if(flags & SPIFFS_VIS_NO_WRAP){
                return SPIFFS_VIS_END;
            }
            else {
                iEntryCur = 0;
                blkIXCur = 0;
            }
        }
    } /* 结束遍历每个Blk */
    
    SPIFFS_CHECK_RES(iRes);
    return SPIFFS_VIS_END;
}
/*********************************************************************************************************
** 函数名称: spiffsObjLookUpScan
** 功能描述: 扫描Flash介质上的所有Lookup Page，记录页面状态（Deleted、Used等），并找到最大块擦除次数
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpScan(PSPIFFS_VOLUME pfs){
    INT32           iRes;
    SPIFFS_BLOCK_IX blkIX;
    INT             iEntry;
    SPIFFS_BLOCK_IX blkIXUnerased = (SPIFFS_BLOCK_IX)-1;
    SPIFFS_OBJ_ID   objIdEraseCount;
    SPIFFS_OBJ_ID   objIdEraseCountFinal;
    SPIFFS_OBJ_ID   objIdEraseCountMin = SPIFFS_OBJ_ID_FREE;
    SPIFFS_OBJ_ID   objIdEraseCountMax = 0;
    SPIFFS_OBJ_ID   objIdMagic;

    blkIX           = 0;
    
    /* 完成EraseCount的计数 */
    while (blkIX < pfs->uiBlkCount)
    {
#ifdef SPIFFS_USE_MAGIC
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                               SPIFFS_MAGIC_PADDR(pfs, blkIX), sizeof(SPIFFS_OBJ_ID), 
                               (PCHAR)&objIdMagic);
        SPIFFS_CHECK_RES(iRes);
        //TODO: 这里扫描的时候有些块未配置，也就是说必须要先format才能使用该文件系统嘛？
        if(objIdMagic != SPIFFS_MAGIC_PADDR(pfs, blkIX)){
            if(blkIXUnerased == SPIFFS_OBJ_ID_FREE){   /* 分配一个未擦写块，有可能是掉电了，Magic才不同 */
                blkIXUnerased = blkIX;
            }
            else {
                SPIFFS_CHECK_RES(SPIFFS_ERR_NOT_A_FS);
            }
        }
#endif
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                               SPIFFS_ERASE_COUNT_PADDR(pfs, blkIX), sizeof(SPIFFS_OBJ_ID),
                               (PCHAR)&objIdEraseCount);
        SPIFFS_CHECK_RES(iRes);
        if(objIdEraseCount != SPIFFS_OBJ_ID_FREE){      
            objIdEraseCountMin = MIN(objIdEraseCountMin, objIdEraseCount);
            objIdEraseCountMax = MAX(objIdEraseCountMax, objIdEraseCount);
        }
        blkIX++;
    }

    //TODO: 搞清楚这个擦除次数是如何计算的
    /* 未经擦除的系统 */
    if(objIdEraseCountMin == 0 && objIdEraseCountMax == SPIFFS_OBJ_ID_FREE){
        objIdEraseCountFinal = 0;
    }
    //TODO: 最高位为1代表Index Page，这里是在做约束吗
    /* Max: 11 -  Min: 1 = 3 - 1 > (11 >> 2) = 1 */
    else if((objIdEraseCountMax - objIdEraseCountMin) > (SPIFFS_OBJ_ID_FREE) / 2){
        objIdEraseCountFinal = objIdEraseCountMin + 1;
    }
    else {
        objIdEraseCountFinal = objIdEraseCountMax + 1;
    }

    pfs->uiMaxEraseCount = objIdEraseCountFinal;

    if(blkIXUnerased != SPIFFS_OBJ_ID_FREE) {
        SPIFFS_DBG("mount: erase block "_SPIPRIbl"\n", blkIXUnerased);
        iRes = spiffsEraseBlk(pfs, blkIXUnerased);
        SPIFFS_CHECK_RES(iRes);
    }

    iRes = spiffsObjLookUpFindEntryVisitor(pfs, 0, 0, 0, 0, __spiffsObjLookUpScanVistor, 
                                           LW_NULL, LW_NULL, LW_NULL, LW_NULL);
    if(iRes == SPIFFS_VIS_END){
        iRes = SPIFFS_OK;
    }

    SPIFFS_CHECK_RES(iRes);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjLookUpFindFreeObjId
** 功能描述: 用于寻找Free Obj ID
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpFindFreeObjId(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID *pObjId, const PCHAR pucConflictingName){
    INT32                    iRes         = SPIFFS_OK;
    UINT32                   uiMaxObjects = (pfs->uiBlkCount * SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs)) / 2;  /* 最大Objects数，因为一个Object至少由一个Index和一个Data构成 */
    SPIFFS_OBJ_ID            objIdFree    = SPIFFS_OBJ_ID_FREE;
    SPIFFS_FREE_OBJ_ID_STATE state;
    UINT                     i,j;
    
    UINT32                   uiMinIndex = 0;
    UINT8                    uiMinCount = (UINT8)-1;
    PCHAR                   pucBitMap;

    UINT8                    uiMask8;
    //TODO: State是用来干嘛的？
    //!用于打包
    state.objIdMin = 1;
    state.objIdMax = uiMaxObjects + 1;
    if(state.objIdMax & SPIFFS_OBJ_ID_IX_FLAG){
        state.objIdMax = ((SPIFFS_OBJ_ID)-1) & ~SPIFFS_OBJ_ID_IX_FLAG;
    }
    state.uiCompaction = 0;
    state.pucConflictingName = pucConflictingName;

    while (iRes == SPIFFS_OK && objIdFree == SPIFFS_OBJ_ID_FREE)
    {   
        /* 可以装在一个页面里，一个字节8位 */
        if(state.objIdMax - state.objIdMin 
           <= (SPIFFS_OBJ_ID)SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) * 8){
            /* 可以放在一个bitmap里 */
            SPIFFS_DBG("free_obj_id: BITMap min:"_SPIPRIid" max:"_SPIPRIid"\n", state.objIdMin, state.objIdMax);
            lib_memset(pfs->pucWorkBuffer, 0, SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));   /* 清空work，用于记录位图数据 */
            iRes = spiffsObjLookUpFindEntryVisitor(pfs, 0, 0, 0, 0, __spiffsObjLookUpFindFreeObjIdBitmapVistor,
                                                   pucConflictingName, &state.objIdMin, LW_NULL, LW_NULL);
            if(iRes == SPIFFS_VIS_END){
                iRes = SPIFFS_OK;
            }
            SPIFFS_CHECK_RES(iRes);

            /* 遍历每个字节 */
            for (i = 0; i < SPIFFS_CFG_LOGIC_PAGE_SZ(pfs); i++)
            {
                uiMask8 = pfs->pucWorkBuffer[i];
                if(uiMask8 == (UINT8)-1){
                    continue;
                }
                /* 遍历每个位 */
                /* 8位掩码 */
                for (j = 0; j < 8; j++)
                {
                    if((uiMask8 & (1 << j)) == 0){
                        *pObjId = state.objIdMin + j + (i << 3);
                        return SPIFFS_OK;
                    }
                }
                
            }
            return SPIFFS_ERR_FULL;     /* 没有空余的Obj了 */           
        }
        /* 不能装在一个页面里，不过可以多次重叠，最后找小的，你懂的 */
        else {
            if(state.uiCompaction != 0){
                uiMinIndex = 0;
                uiMinCount = (UINT8)-1;
                pucBitMap = pfs->pucWorkBuffer;

                for (i = 0; i < SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(UINT8); i++)
                {
                    if(pucBitMap[i] < uiMinCount){
                        uiMinCount = pucBitMap[i];
                        uiMinIndex = i;
                        if(uiMinCount == 0){
                            break;
                        }
                    }
                }

                if(uiMinCount == state.uiCompaction){       /* 没有空闲OBJ了 */
                    SPIFFS_DBG("free_obj_id: compacted table is full\n");
                    return SPIFFS_ERR_FULL;
                }
                
                SPIFFS_DBG("free_obj_id: COMP select index:"_SPIPRIi" min_count:"_SPIPRIi" min:"_SPIPRIid" max:"_SPIPRIid" compact:"_SPIPRIi"\n",
                            uiMinIndex, uiMinCount, state.objIdMin, state.objIdMax, state.uiCompaction);
                if(uiMinCount == 0){
                    *pObjId = uiMinIndex * state.uiCompaction + state.objIdMin;
                    return SPIFFS_OK;
                }
                else {
                    /* 减少压缩范围 */
                    SPIFFS_DBG("free_obj_id: COMP SEL chunk:"_SPIPRIi" min:"_SPIPRIid" -> "_SPIPRIid"\n", state.uiCompaction, state.objIdMin, state.objIdMin + uiMinIndex *  state.uiCompaction);
                    state.objIdMin += (uiMinIndex * state.uiCompaction);
                    state.objIdMax = state.objIdMin + state.uiCompaction;
                }

                if(state.objIdMax - state.objIdMin 
                    <= (SPIFFS_OBJ_ID)SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) * 8){
                    /* 可以用一页装了，那就直接装，不用压缩了 */
                    continue;
                }
            }
            state.uiCompaction = (state.objIdMax - state.objIdMin) / ((SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(UINT8))); /* 页面总数 */
            SPIFFS_DBG("free_obj_id: COMP min:"_SPIPRIid" max:"_SPIPRIid" compact:"_SPIPRIi"\n", state.objIdMin, 
                       state.objIdMax, state.uiCompaction);
            lib_memset(pfs->pucWorkBuffer, 0, SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
            iRes = spiffsObjLookUpFindEntryVisitor(pfs, 0, 0, 0, 0, 
                                                   __spiffsObjLookUpFindFreeObjIdCompactVistor, 
                                                   &state, LW_NULL, LW_NULL, LW_NULL);
            if(iRes == SPIFFS_VIS_END){
                iRes = SPIFFS_OK;
            }
            SPIFFS_CHECK_RES(iRes);
            state.pucConflictingName = LW_NULL; /* 找一次就够了 */
        }
    }
}
/*********************************************************************************************************
** 函数名称: spiffsObjLookUpFindId
** 功能描述: 用于寻找给定的ObjId
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpFindId(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                            SPIFFS_OBJ_ID objId, SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry){
    INT32 iRes = spiffsObjLookUpFindEntryVisitor(pfs, blkIXStarting, iLookUpEntryStarting, 
                                                SPIFFS_VIS_CHECK_ID, objId, LW_NULL, LW_NULL, LW_NULL, 
                                                pBlkIX, piLookUpEntry);
    if(iRes == SPIFFS_VIS_END) {
        iRes = SPIFFS_ERR_NOT_FOUND;
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjLookUpFindFreeEntry
** 功能描述: 用于寻找Free Look Up Entry
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpFindFreeEntry(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIXStarting, INT iLookUpEntryStarting,
                                   SPIFFS_BLOCK_IX *pBlkIX, INT *piLookUpEntry){
    INT32 iRes;
    if (!pfs->uiCleaningFlag && pfs->uiFreeBlks < 2) {
        iRes = spiffsGCQuick(pfs, 0);
        if (iRes == SPIFFS_ERR_NO_DELETED_BLOCKS) {
            iRes = SPIFFS_OK;
        }
        SPIFFS_CHECK_RES(iRes);
        if (pfs->uiFreeBlks < 2) {
            return SPIFFS_ERR_FULL;
        }
    }
    /* 找到一个Id为111的Entry */
    iRes = spiffsObjLookUpFindId(pfs, blkIXStarting, iLookUpEntryStarting,
                                 SPIFFS_OBJ_ID_FREE, pBlkIX, piLookUpEntry);
    if (iRes == SPIFFS_OK) {
        pfs->blkIXFreeCursor = *pBlkIX;
        pfs->objLookupEntryFreeCursor = (*piLookUpEntry) + 1;
        if (*piLookUpEntry == 0) {
            pfs->uiFreeBlks--;
        }
    }
    if (iRes == SPIFFS_ERR_FULL) {
        SPIFFS_DBG("pfs full\n");
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjLookUpFindIdAndSpan
** 功能描述: 用于寻找给定的ObjID与SpanIX，返回PageIX
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None 
** 全局变量: 
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjLookUpFindIdAndSpan(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, SPIFFS_SPAN_IX spanIX,
                                   SPIFFS_PAGE_IX pageIXExclusion, SPIFFS_PAGE_IX *pPageIX){
    INT32 iRes;
    SPIFFS_BLOCK_IX blkIX;
    INT iEntry;

    iRes = spiffsObjLookUpFindEntryVisitor(pfs, pfs->blkIXCursor, pfs->objLookupEntryCursor, SPIFFS_VIS_CHECK_ID, objId,
                                           __spiffsObjLookUpFindIdAndSpanVistor, pageIXExclusion ? &pageIXExclusion : LW_NULL,
                                           &spanIX, &blkIX, &iEntry);
    if (iRes == SPIFFS_VIS_END) {
        iRes = SPIFFS_ERR_NOT_FOUND;
    }

    SPIFFS_CHECK_RES(iRes);

    if (pPageIX) {
        *pPageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry);
    }

    pfs->blkIXCursor = blkIX;
    pfs->objLookupEntryCursor = iEntry;

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectFindObjectIndexHeaderByName
** 功能描述: 用于寻找Free Obj ID
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectFindObjectIndexHeaderByName(PSPIFFS_VOLUME pfs, UCHAR ucName[SPIFFS_OBJ_NAME_LEN], SPIFFS_PAGE_IX *pPageIX){
    INT32 iRes;
    SPIFFS_BLOCK_IX blkIX;
    INT iEntry;

    iRes = spiffsObjLookUpFindEntryVisitor(pfs,
                                           pfs->blkIXCursor,
                                           pfs->objLookupEntryCursor,
                                           0,
                                           0,
                                           __spiffsObjectFindObjectIndexHeaderByNameVisitor,
                                           ucName,
                                           LW_NULL,
                                           &blkIX,
                                           &iEntry);
    
    if (iRes == SPIFFS_VIS_END) {
        iRes = SPIFFS_ERR_NOT_FOUND;
    }
    SPIFFS_CHECK_RES(iRes);

    if (pPageIX) {
        *pPageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry);
    }

    pfs->blkIXCursor = blkIX;
    pfs->objLookupEntryCursor = iEntry;

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsPageIndexCheck
** 功能描述: 检查页面索引
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsPageIndexCheck(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd, 
                           SPIFFS_PAGE_IX pageIX, SPIFFS_SPAN_IX spanIX){
    UINT32 iRes = SPIFFS_OK;
    SPIFFS_PAGE_HEADER pageHeader;
    if (pageIX == (SPIFFS_PAGE_IX)-1) {
        // referring to page 0xffff...., bad object index
        return SPIFFS_ERR_INDEX_FREE;
    }
    if (pageIX % SPIFFS_PAGES_PER_BLOCK(pfs) < SPIFFS_OBJ_LOOKUP_PAGES(pfs)) {
        /* 页面不能指向LookUp page */
        // referring to an object lookup page, bad object index
        return SPIFFS_ERR_INDEX_LU;
    }
    if (pageIX > SPIFFS_MAX_PAGES(pfs)) {
        // referring to a bad page
        return SPIFFS_ERR_INDEX_INVALID;
    }

    /* Check Page */
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN,
                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX), sizeof(SPIFFS_PAGE_HEADER), 
                           (PCHAR)&pageHeader);
    SPIFFS_CHECK_RES(iRes);
    SPIFFS_VALIDATE_OBJIX(pageHeader, pFd->objId, spanIX);
    
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsPageDataCheck
** 功能描述: 检查数据页面
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsPageDataCheck(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd,  
                          SPIFFS_PAGE_IX pageIX, SPIFFS_SPAN_IX spanIX){
    UINT32 iRes = SPIFFS_OK;
    SPIFFS_PAGE_HEADER pageHeader;
    if (pageIX == (SPIFFS_PAGE_IX)-1) {
        // referring to page 0xffff...., bad object index
        return SPIFFS_ERR_INDEX_REF_FREE;
    }
    if (pageIX % SPIFFS_PAGES_PER_BLOCK(pfs) < SPIFFS_OBJ_LOOKUP_PAGES(pfs)) {
        // referring to an object lookup page, bad object index
        return SPIFFS_ERR_INDEX_REF_LU;
    }
    if (pageIX > SPIFFS_MAX_PAGES(pfs)) {
        // referring to a bad page
        return SPIFFS_ERR_INDEX_REF_INVALID;
    }
    /* Check Page */
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_READ, pFd->fileN, 
                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX),
                           sizeof(SPIFFS_PAGE_HEADER),(PCHAR)&pageHeader);
    SPIFFS_CHECK_RES(iRes);
    /* 保证不是Index页面 */
    SPIFFS_VALIDATE_DATA(pageHeader, pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG, spanIX);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsPageDelete
** 功能描述: 删除指定页面（标记删除即可）
** 输　入  : pfs          文件头
**           pageIX         页面IX
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsPageDelete(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX){
    INT32 iRes;
    // mark deleted iEntry in source object lookup
    /* 先标记LookUp Entry */
    SPIFFS_OBJ_ID d_obj_id = SPIFFS_OBJ_ID_DELETED;
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_DELE, 0,
                            SPIFFS_BLOCK_TO_PADDR(pfs, SPIFFS_BLOCK_FOR_PAGE(pfs, pageIX)) + 
                            SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIX) * sizeof(SPIFFS_PAGE_IX),
                            sizeof(SPIFFS_OBJ_ID), (PCHAR)&d_obj_id);
    SPIFFS_CHECK_RES(iRes);

    pfs->uiStatsPageAllocated--;
    pfs->uiStatsPageDeleted++;


    // mark deleted in source page
    /* 再标记待删除页面 */
    UINT8 flags = 0xff;
    flags &= ~(SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_USED);
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_DELE, 0,
                            SPIFFS_PAGE_TO_PADDR(pfs, pageIX) + offsetof(SPIFFS_PAGE_HEADER, flags),
                            sizeof(flags), &flags);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsPageDelete
** 功能描述: 删除指定页面（标记删除即可）
** 输　入  : pfs          文件头
**           pageIX         页面IX
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsPageAllocateData(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId, PSPIFFS_PAGE_HEADER pPageHeader,
                             PCHAR pData, UINT32 uiLen, UINT32 uiPageOffs, BOOL bIsFinalize,
                             SPIFFS_PAGE_IX *pageIX) {
    INT32 iRes = SPIFFS_OK;
    SPIFFS_BLOCK_IX blkIX;
    INT iEntry;

    // find free iEntry
    /* 找到一个空闲的Entry */
    iRes = spiffsObjLookUpFindFreeEntry(pfs, pfs->blkIXFreeCursor, 
                                        pfs->objLookupEntryFreeCursor, &blkIX, &iEntry);
    SPIFFS_CHECK_RES(iRes);

    // occupy page in object lookup
    /* 用当前数据页面的objId占用这个entry */
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_UPDT, 0, 
                            SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + iEntry * sizeof(SPIFFS_OBJ_ID), 
                            sizeof(SPIFFS_OBJ_ID), (PCHAR)&objId);
    SPIFFS_CHECK_RES(iRes);

    pfs->uiStatsPageAllocated++;

    // write page header
    pPageHeader->flags &= ~SPIFFS_PH_FLAG_USED;
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, 0, 
                            SPIFFS_OBJ_LOOKUP_ENTRY_TO_PADDR(pfs, blkIX, iEntry), 
                            sizeof(SPIFFS_PAGE_HEADER), (PCHAR)pPageHeader);
    SPIFFS_CHECK_RES(iRes);

    // write page data
    if (pData) {
        iRes = spiffsCacheWrite(pfs,  SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, 0, 
                                SPIFFS_OBJ_LOOKUP_ENTRY_TO_PADDR(pfs, blkIX, iEntry) + sizeof(SPIFFS_PAGE_HEADER) + uiPageOffs, 
                                uiLen, pData);
        SPIFFS_CHECK_RES(iRes);
    }

    // finalize header if necessary
    /* 结束写入 */
    if (bIsFinalize && (pPageHeader->flags & SPIFFS_PH_FLAG_FINAL)) {
        pPageHeader->flags &= ~SPIFFS_PH_FLAG_FINAL;
        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, 0, 
                                SPIFFS_OBJ_LOOKUP_ENTRY_TO_PADDR(pfs, blkIX, iEntry) + offsetof(SPIFFS_PAGE_HEADER, flags),  
                                sizeof(UINT8),(PCHAR)&pPageHeader->flags);
        SPIFFS_CHECK_RES(iRes);
    }

    // return uiByteHasWritten page
    /* 返回当前写入的页面 */
    if (pageIX) {
        *pageIX = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry);
    }

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectUpdateIndexHdr
** 功能描述: 更新索引页头
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectUpdateIndexHdr(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd, SPIFFS_OBJ_ID objId, SPIFFS_PAGE_IX pageIXObjIXHdr,
                                 PCHAR pucNewObjIXHdrData , const UCHAR ucName[], UINT32 uiSize, SPIFFS_PAGE_IX *pageIXNew){
    INT32                           iRes = SPIFFS_OK;
    PSPIFFS_PAGE_OBJECT_IX_HEADER   objIXHdr;                           /* Index页面的SpanIX = 0的头 */
    SPIFFS_PAGE_IX                  pageIXObjIXHdrNew;                  /* SpanIX = 0的Index页面的page IX */

    /* 获取对应索引页的ObjID */
    objId |=  SPIFFS_OBJ_ID_IX_FLAG;

    /* 获取Index页面的Hdr */
    if (pucNewObjIXHdrData) {
        // object index header page already given to us, no need to load it
        objIXHdr = (PSPIFFS_PAGE_OBJECT_IX_HEADER)pucNewObjIXHdrData;
    } 
    else {
        // read object index header page
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                               SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXHdr), SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), 
                               pfs->pucWorkBuffer);
        SPIFFS_CHECK_RES(iRes);
        objIXHdr = (PSPIFFS_PAGE_OBJECT_IX_HEADER)pfs->pucWorkBuffer;
    }

    SPIFFS_VALIDATE_OBJIX(objIXHdr->pageHdr, objId, 0);

    // change name
    if (ucName) {
        strncpy((char*)objIXHdr->ucName, (const char*)ucName, sizeof(objIXHdr->ucName) - 1);
        ((char*) objIXHdr->ucName)[sizeof(objIXHdr->ucName) - 1] = '\0';
    }

    if (uiSize) {
        objIXHdr->uiSize = uiSize;
    }
    
    // move and update page
    //TODO: 为什么要移动页面，因为页面更新了
    iRes = spiffsPageMove(pfs, pFd == LW_NULL ? LW_NULL : pFd->fileN, 
                          (PCHAR)objIXHdr, objId, 0, pageIXObjIXHdr, &pageIXObjIXHdrNew);

    if (iRes == SPIFFS_OK) {
        if (pageIXNew) {
            *pageIXNew = pageIXObjIXHdrNew;
        }
        // callback on object index update
        spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)objIXHdr, 
                            pucNewObjIXHdrData ? SPIFFS_EV_IX_UPD : SPIFFS_EV_IX_UPD_HDR,
                            objId, objIXHdr->pageHdr.spanIX, pageIXObjIXHdrNew, objIXHdr->uiSize);
        if (pFd){
            pFd->pageIXObjIXHdr = pageIXObjIXHdrNew; // if this is not in the registered cluster
        } 
    }

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectCreate
** 功能描述: 用于创建一个Object
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectCreate(PSPIFFS_VOLUME pfs, SPIFFS_OBJ_ID objId,
                         const UCHAR ucPath[], 
                         SPIFFS_OBJ_TYPE type, SPIFFS_PAGE_IX* pPageIXObjIndexHdr)
{
    INT32 iRes = SPIFFS_OK;
    SPIFFS_BLOCK_IX blkIX;
    SPIFFS_PAGE_OBJECT_IX_HEADER  objIXHdr;
    INT iEntry;
    
    iRes = spiffsGCCheck(pfs, SPIFFS_DATA_PAGE_SIZE(pfs));
    SPIFFS_CHECK_RES(iRes);

    objId |= SPIFFS_OBJ_ID_IX_FLAG;             /* 转换成该ID对应的目录 */
    iRes = spiffsObjLookUpFindFreeEntry(pfs, pfs->blkIXFreeCursor, 
                                        pfs->objLookupEntryFreeCursor, &blkIX, &iEntry);
    SPIFFS_DBG("create: found free page @ "_SPIPRIpg" blkIX:"_SPIPRIbl" iEntry:"_SPIPRIsp"\n", 
               (SPIFFS_PAGE_IX)SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry), blkIX, iEntry);

    // occupy page in object lookup
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_UPDT, 0, 
                            SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + iEntry * sizeof(SPIFFS_OBJ_ID), 
                            sizeof(SPIFFS_OBJ_ID), (PCHAR)&objId);
#ifdef SPIFFS_CACHE_TEST
    objId = 0;
    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                            SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + iEntry * sizeof(SPIFFS_OBJ_ID), 
                            sizeof(SPIFFS_OBJ_ID), (PCHAR)&objId);
#endif
    SPIFFS_CHECK_RES(iRes);

    pfs->uiStatsPageAllocated++;

    // write empty object index page
    objIXHdr.pageHdr.objId = objId;
    objIXHdr.pageHdr.spanIX = 0;
    objIXHdr.pageHdr.flags = 0xff & ~(SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_INDEX | SPIFFS_PH_FLAG_USED);
    objIXHdr.type = type;
    objIXHdr.uiSize = SPIFFS_UNDEFINED_LEN; // keep ones so we can update later without wasting this page
    strncpy((PCHAR)objIXHdr.ucName, ucPath, sizeof(objIXHdr.ucName) - 1);
    ((PCHAR)objIXHdr.ucName)[sizeof(objIXHdr.ucName) - 1] = '\0';

    // update page
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, 0, 
                            SPIFFS_OBJ_LOOKUP_ENTRY_TO_PADDR(pfs, blkIX, iEntry), 
                            sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), (PCHAR)&objIXHdr);

    //ANCHOR : 读回来看看对不对

    SPIFFS_CHECK_RES(iRes);
    spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)&objIXHdr,
                        SPIFFS_EV_IX_NEW, objId, 0, 
                        SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry), 
                        SPIFFS_UNDEFINED_LEN);

    if (pPageIXObjIndexHdr) {
        *pPageIXObjIndexHdr = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry);
    }

    return iRes;
}

/*********************************************************************************************************
** 函数名称: spiffsObjectOpenByPage
** 功能描述: 根据一个pageIX打开一个文件
** 输　入  : pfs          文件头
**           pageIX        返回的Object ID
**           pFd           空闲的Fd
**           flags         打开方式
**           mode          目的在于与Posix兼容
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectOpenByPage(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX, 
                             PSPIFFS_FD pFd, SPIFFS_FLAGS flags, SPIFFS_MODE mode){
    //TODO: 该页面是Index页吗？
    (VOID)mode;
    INT32                        iRes = SPIFFS_OK;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    SPIFFS_OBJ_ID                objId;
    SPIFFS_BLOCK_IX              blkIX = SPIFFS_BLOCK_FOR_PAGE(pfs, pageIX);
    INT                          iEntry = SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIX);

    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                           sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), (PCHAR)&objIXHdr);
    SPIFFS_CHECK_RES(iRes);


    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                           SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + iEntry * sizeof(SPIFFS_OBJ_ID), 
                           sizeof(SPIFFS_OBJ_ID), (PCHAR)&objId);

    pFd->pfs = pfs;
    pFd->pageIXObjIXHdr = pageIX;
    pFd->uiSize = objIXHdr.uiSize;
    pFd->uiOffset = 0;
    pFd->pageIXObjIXCursor = pageIX;
    pFd->spanIXObjIXCursor = 0;
    pFd->objId = objId;
    pFd->flags = flags;

    SPIFFS_VALIDATE_OBJIX(objIXHdr.pageHdr, pFd->objId, 0);

    SPIFFS_DBG("open: pFd "_SPIPRIfd" is obj id "_SPIPRIid"\n", 
                pFd->fileN, pFd->objId);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectTruncate
** 功能描述: 用于截断一个Object
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectTruncate(PSPIFFS_FD pFd, UINT32 uiNewSize, BOOL bIsRemoveFull){
    INT32          iRes = SPIFFS_OK;
    PSPIFFS_VOLUME pfs = pFd->pfs;

    /* 不用干任何事情 */
    if ((pFd->uiSize == SPIFFS_UNDEFINED_LEN || pFd->uiSize == 0) && !bIsRemoveFull) {
        // no op
        return iRes;
    }

    // need 2 pages if not removing: object index page + possibly chopped data page
    if (bIsRemoveFull == LW_FALSE) {
        iRes = spiffsGCCheck(pfs, SPIFFS_DATA_PAGE_SIZE(pfs) * 2);
        SPIFFS_CHECK_RES(iRes);
    }

    SPIFFS_PAGE_IX                  pageIXObjIX = pFd->pageIXObjIXHdr;  /* 索引Obj的页码 */
    SPIFFS_SPAN_IX                  spanIXObjData = (pFd->uiSize > 0 ? pFd->uiSize - 1 : 0) / SPIFFS_DATA_PAGE_SIZE(pfs);
    UINT32                          uiCurSize = pFd->uiSize == (UINT32)SPIFFS_UNDEFINED_LEN ? 0 : pFd->uiSize;
    SPIFFS_SPAN_IX                  spanIXObjIXCur = 0;
    SPIFFS_SPAN_IX                  spanIXObjIXPrev = (SPIFFS_SPAN_IX)-1;
    PSPIFFS_PAGE_OBJECT_IX_HEADER   pObjIXHdr = (SPIFFS_PAGE_OBJECT_IX_HEADER *)pfs->pucWorkBuffer;
    SPIFFS_PAGE_OBJECT_IX           *pObjIX = (SPIFFS_PAGE_OBJECT_IX *)pfs->pucWorkBuffer;
    SPIFFS_PAGE_IX                  pageIXData;
    SPIFFS_PAGE_IX                  newPageIXObjIXHdr;

    // before truncating, check if object is to be fully removed and mark this
    /* 检查是否全部删除，如果是，则给出以下标记
       ~( SPIFFS_PH_FLAG_USED | SPIFFS_PH_FLAG_INDEX | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_IXDELE ) 
    */
    if (bIsRemoveFull && uiNewSize == 0) {
        UINT8 flags = ~( SPIFFS_PH_FLAG_USED | SPIFFS_PH_FLAG_INDEX | SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_IXDELE);
        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                SPIFFS_PAGE_TO_PADDR(pfs, pFd->pageIXObjIXHdr) + offsetof(SPIFFS_PAGE_HEADER, flags),
                                sizeof(UINT8), (UINT8 *)&flags);
        SPIFFS_CHECK_RES(iRes);
    }

    // delete from end of object until desired uiLen is reached
    /* 从后往前删 */
    while (uiCurSize > uiNewSize) {
        spanIXObjIXCur = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIXObjData);      /* 当前 SpanIX Data 对应于那个Object IX的Span */
        // put object index for current data span index in pucWorkBuffer buffer
        if (spanIXObjIXPrev != spanIXObjIXCur) {            /* 之前的与现在的不同 */
            if (spanIXObjIXPrev != (SPIFFS_SPAN_IX)-1) {    /* 前一个页面非无效页面 */
                // remove previous object index page
                /* 移除前一页 */
                SPIFFS_DBG("truncate: delete pObjIX page "_SPIPRIpg":"_SPIPRIsp"\n", pageIXObjIX, spanIXObjIXPrev);

                iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIX, spanIXObjIXPrev);
                SPIFFS_CHECK_RES(iRes);

                iRes = spiffsPageDelete(pfs, pageIXObjIX);
                SPIFFS_CHECK_RES(iRes);
                spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)0, SPIFFS_EV_IX_DEL, 
                                    pFd->objId, pObjIX->pageHdr.spanIX, pageIXObjIX, 0);
                if (spanIXObjIXPrev > 0) {  /* 普通Index Page头部 */
                    // Update object index header page, unless we totally want to remove the file.
                    // If fully removing, we're not keeping consistency as good as when storing the header between chunks,
                    // would we be aborted. But when removing full files, a crammed system may otherwise
                    // report ERR_FULL a la windows. We cannot have that.
                    // Hence, take the risk - if aborted, a file check would free the lost pages and mend things
                    // as the file is marked as fully deleted in the beginning.
                    if (bIsRemoveFull == LW_FALSE) {
                        SPIFFS_DBG("truncate: update pObjIX hdr page "_SPIPRIpg":"_SPIPRIsp" to uiSize "_SPIPRIi"\n", 
                                   pFd->pageIXObjIXHdr, spanIXObjIXPrev, uiCurSize);
                        iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                                          LW_NULL, LW_NULL, uiCurSize, &newPageIXObjIXHdr);
                        SPIFFS_CHECK_RES(iRes);
                    }
                    pFd->uiSize = uiCurSize;
                }
            }
            // load current object index (header) page
            /* 加载当前Index Page */
            if (spanIXObjIXCur == 0) {      /* 这是头部IndexPage */
                pageIXObjIX = pFd->pageIXObjIXHdr;
            }
            else {                          /* 非头部IndexPage */
                /* 加载当前页面 */
                iRes = spiffsObjLookUpFindIdAndSpan(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, 
                                                    spanIXObjIXCur, 0, &pageIXObjIX);
                SPIFFS_CHECK_RES(iRes);
            }

            SPIFFS_DBG("truncate: load pObjIX page "_SPIPRIpg":"_SPIPRIsp" for data spix:"_SPIPRIsp"\n", 
                       pageIXObjIX, spanIXObjIXCur, spanIXObjData);
            /* 读入Work Buffer */
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                   SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIX), SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), 
                                   pfs->pucWorkBuffer);
            SPIFFS_CHECK_RES(iRes);
            SPIFFS_VALIDATE_OBJIX(pObjIXHdr->pageHdr, pFd->objId, spanIXObjIXCur);
            pFd->pageIXObjIXCursor = pageIXObjIX;
            pFd->spanIXObjIXCursor = spanIXObjIXCur;
            pFd->uiOffset = uiCurSize;

            spanIXObjIXPrev = spanIXObjIXCur;
        }

        //TODO: 为什么要标记为free ? 先标记Free？下一轮再进行删除？
        if (spanIXObjIXCur == 0) {      /* 当前页面是首部Index，即SpanIndex = 0 */
            // get data page from object index header page
            pageIXData = ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData];
            ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData] = SPIFFS_OBJ_ID_FREE;
        } 
        else {
            // get data page from object index page
            pageIXData = ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)];
            ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)] = SPIFFS_OBJ_ID_FREE;
        }

        SPIFFS_DBG("truncate: got data pageIX "_SPIPRIpg"\n", pageIXData);

        if (uiNewSize == 0 || bIsRemoveFull 
         || uiCurSize - uiNewSize >= SPIFFS_DATA_PAGE_SIZE(pfs)) {
            // delete full data page
            //TODO：为啥这里还要再删除呢？前面不是已经删除了
            /* 可以删除一整个页面 */
            iRes = spiffsPageDataCheck(pfs, pFd, pageIXData, spanIXObjData);
            if (iRes != SPIFFS_ERR_DELETED 
                && iRes != SPIFFS_OK 
                && iRes != SPIFFS_ERR_INDEX_REF_FREE) {
                SPIFFS_DBG("truncate: err validating data pageIX "_SPIPRIi"\n", iRes);
                break;
            }

            if (iRes == SPIFFS_OK) {
                iRes = spiffsPageDelete(pfs, pageIXData);
                if (iRes != SPIFFS_OK) {
                    SPIFFS_DBG("truncate: err deleting data pageIX "_SPIPRIi"\n", iRes);
                    break;
                }
            } 
            else if (iRes == SPIFFS_ERR_DELETED || iRes == SPIFFS_ERR_INDEX_REF_FREE) {
                iRes = SPIFFS_OK;
            }

            // update current uiSize
            /* 更新当前大小 */
            if (uiCurSize % SPIFFS_DATA_PAGE_SIZE(pfs) == 0) {
                uiCurSize -= SPIFFS_DATA_PAGE_SIZE(pfs);
            } 
            else {
                uiCurSize -= uiCurSize % SPIFFS_DATA_PAGE_SIZE(pfs);
            }
            
            pFd->uiSize = uiCurSize;
            pFd->uiOffset = uiCurSize;
            SPIFFS_DBG("truncate: delete data page "_SPIPRIpg" for data spix:"_SPIPRIsp", uiCurSize:"_SPIPRIi"\n", 
                       pageIXData, spanIXObjData, uiCurSize);
        } 
        else {
            // delete last page, partially
            /* 最后一个页面不一定全部删除 */
            SPIFFS_PAGE_HEADER pageHeader;
            SPIFFS_PAGE_IX pageIXDataNew;
            UINT32 uiBytesToRemove = SPIFFS_DATA_PAGE_SIZE(pfs) - (uiNewSize % SPIFFS_DATA_PAGE_SIZE(pfs));
            
            SPIFFS_DBG("truncate: delete "_SPIPRIi" bytes from data page "_SPIPRIpg" for data spix:"_SPIPRIsp", uiCurSize:"_SPIPRIi"\n", 
                       uiBytesToRemove, pageIXData, spanIXObjData, uiCurSize);

            iRes = spiffsPageDataCheck(pfs, pFd, pageIXData, spanIXObjData);
            if (iRes != SPIFFS_OK) 
                break;

            pageHeader.objId = pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG;
            pageHeader.spanIX = spanIXObjData;
            pageHeader.flags = 0xff;
            // allocate new page and copy unmodified data
            //TODO: 直接传入pData不可以吗？
            iRes = spiffsPageAllocateData(pfs, pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG,
                                          &pageHeader, LW_NULL, 0, 0, LW_FALSE, &pageIXDataNew);
            if (iRes != SPIFFS_OK) 
                break;
            iRes = spiffsPhysCpy(pfs, 0,
                                 SPIFFS_PAGE_TO_PADDR(pfs, pageIXDataNew) + sizeof(SPIFFS_PAGE_HEADER),
                                 SPIFFS_PAGE_TO_PADDR(pfs, pageIXData) + sizeof(SPIFFS_PAGE_HEADER),
                                 SPIFFS_DATA_PAGE_SIZE(pfs) - uiBytesToRemove);
            if (iRes != SPIFFS_OK) 
                break;
            // delete original data page
            /* 直接删除原来的数据页面，因为有用的已经拷贝了 */
            iRes = spiffsPageDelete(pfs, pageIXData);
            if (iRes != SPIFFS_OK) 
                break;
            pageHeader.flags &= ~SPIFFS_PH_FLAG_FINAL;
            iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, pFd->fileN,
                                    SPIFFS_PAGE_TO_PADDR(pfs, pageIXDataNew) + offsetof(SPIFFS_PAGE_HEADER, flags),
                                    sizeof(UINT8), (PCHAR)&pageHeader.flags);
            if (iRes != SPIFFS_OK) 
                break;

            // update memory representation of object index page with new data page
            /* 更新内存 IX Obj */
            if (spanIXObjIXCur == 0) {
                /* 如果是IX OBJ的第0页 */
                // update object index header page
                ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData] = pageIXDataNew;
                SPIFFS_DBG("truncate: wrote page "_SPIPRIpg" to pObjIXHdr iEntry "_SPIPRIsp" in mem\n", 
                           pageIXDataNew, (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData));
            } 
            else {
                /* 如果是IX OBJ的非第0页 */
                // update object index page
                ((SPIFFS_PAGE_IX*)((UINT8 *)pObjIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)] = pageIXDataNew;
                SPIFFS_DBG("truncate: wrote page "_SPIPRIpg" to pObjIX iEntry "_SPIPRIsp" in mem\n", pageIXDataNew, (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData));
            }
            uiCurSize = uiNewSize;
            pFd->uiSize = uiNewSize;
            pFd->uiOffset = uiCurSize;
            break;
        }
        spanIXObjData--;
    } // while all data

    // update object indices
    if (spanIXObjIXCur == 0) {
        // update object index header page
        if (uiCurSize == 0) {
            if (bIsRemoveFull) {
                // remove object altogether
                SPIFFS_DBG("truncate: remove object index header page "_SPIPRIpg"\n", pageIXObjIX);

                iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIX, 0);
                SPIFFS_CHECK_RES(iRes);

                iRes = spiffsPageDelete(pfs, pageIXObjIX);
                SPIFFS_CHECK_RES(iRes);
                spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)0,
                                    SPIFFS_EV_IX_DEL, pFd->objId, 0, pageIXObjIX, 0);
            } 
            else {
                // make uninitialized object
                SPIFFS_DBG("truncate: reset pObjIXHdr page "_SPIPRIpg"\n", pageIXObjIX);
                memset(pfs->pucWorkBuffer + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), 0xff,
                       SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER));
                iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pageIXObjIX, 
                                                  pfs->pucWorkBuffer, LW_NULL, SPIFFS_UNDEFINED_LEN, 
                                                  &newPageIXObjIXHdr);
                SPIFFS_CHECK_RES(iRes);
            }
        } 
        else {
            // update object index header page
            SPIFFS_DBG("truncate: update object index header page with indices and uiSize\n");
            iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pageIXObjIX, pfs->pucWorkBuffer, 
                                              LW_NULL, uiCurSize, &newPageIXObjIXHdr);
            SPIFFS_CHECK_RES(iRes);
        }
    } 
    else {
        // update both current object index page and object index header page
        SPIFFS_PAGE_IX pageIXObjIXNew;

        iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIX, spanIXObjIXCur);
        SPIFFS_CHECK_RES(iRes);

        // move and update object index page
        iRes = spiffsPageMove(pfs, pFd->fileN, (UINT8*)pObjIXHdr, pFd->objId, LW_NULL, 
                              pageIXObjIX, &pageIXObjIXNew);
        SPIFFS_CHECK_RES(iRes);
        spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)pObjIXHdr, SPIFFS_EV_IX_UPD, 
                            pFd->objId, pObjIX->pageHdr.spanIX, pageIXObjIXNew, 0);
        SPIFFS_DBG("truncate: store modified pObjIX page, "_SPIPRIpg":"_SPIPRIsp"\n", pageIXObjIXNew, spanIXObjIXCur);
        pFd->pageIXObjIXCursor = pageIXObjIXNew;
        pFd->spanIXObjIXCursor = spanIXObjIXCur;
        pFd->uiOffset = uiCurSize;
        // update object index header page with new uiSize
        iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                          LW_NULL, LW_NULL, uiCurSize, &newPageIXObjIXHdr);
        SPIFFS_CHECK_RES(iRes);
    }
    pFd->uiSize = uiCurSize;
    return iRes;
}
//TODO:代码优化 + 思路整理
/*********************************************************************************************************
** 函数名称: spiffsObjectAppend
** 功能描述: 用于追加写一个Object
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectAppend(PSPIFFS_FD pFd, UINT32 uiOffset, PCHAR pContent, UINT32 uiLen){
    PSPIFFS_VOLUME pfs = pFd->pfs;
    INT32 iRes = SPIFFS_OK;
    UINT32 uiByteHasWritten = 0;

    SPIFFS_DBG("append: "_SPIPRIi" bytes @ offs "_SPIPRIi" of uiSize "_SPIPRIi"\n", 
                uiLen, uiOffset, pFd->uiSize);

    if (uiOffset > pFd->uiSize) {
        SPIFFS_DBG("append: uiOffset reversed to uiSize\n");
        uiOffset = pFd->uiSize;
    }

    iRes = spiffsGCCheck(pfs, uiLen + SPIFFS_DATA_PAGE_SIZE(pfs)); // add an extra page of data worth for meta
    if (iRes != SPIFFS_OK) {
        SPIFFS_DBG("append: gc check fail "_SPIPRIi"\n", iRes);
    }
    SPIFFS_CHECK_RES(iRes);

    PSPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr = (PSPIFFS_PAGE_OBJECT_IX_HEADER)pfs->pucWorkBuffer;
    PSPIFFS_PAGE_OBJECT_IX objIX = (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer;
    SPIFFS_PAGE_HEADER pageHeader;

    SPIFFS_SPAN_IX spanIXObjIXCur = 0;
    SPIFFS_SPAN_IX spanIXObjIXPrev = (SPIFFS_SPAN_IX)-1;
    SPIFFS_PAGE_IX pageIXObjIXCur = pFd->pageIXObjIXHdr;
    SPIFFS_PAGE_IX pageIXObjIXHdrNew;

    SPIFFS_SPAN_IX spanIXObjData = uiOffset / SPIFFS_DATA_PAGE_SIZE(pfs);
    SPIFFS_PAGE_IX pageIXObjData;
    UINT32 uiPageOffset = uiOffset % SPIFFS_DATA_PAGE_SIZE(pfs);

    // write all data
    while (iRes == SPIFFS_OK && uiByteHasWritten < uiLen) {
        // calculate object index page span index
        spanIXObjIXCur = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIXObjData);   /* 根据当前Data页面的SpanIX获取当前Index页面的SpanIX */

        // handle storing and loading of object indices
        if (spanIXObjIXCur != spanIXObjIXPrev) {
            // new object index page
            // within this clause we return directly if something fails, object index mess-up
            if (uiByteHasWritten > 0) {
                // store previous object index page, unless first pass
                SPIFFS_DBG("append: "_SPIPRIid" store objIX "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                            pFd->objId,pageIXObjIXCur, spanIXObjIXPrev, uiByteHasWritten);
                if (spanIXObjIXPrev == 0) {         /* 上一个Index页面的SpanIX = 0*/
                    // this is an update to object index header page
                    objIXHdr->uiSize = uiOffset + uiByteHasWritten;
                    if (uiOffset == 0) {
                        // was an empty object, update same page (uiSize was 0xffffffff)
                        iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, 0);
                        SPIFFS_CHECK_RES(iRes);
                        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                                SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                                SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                        SPIFFS_CHECK_RES(iRes);
                    } 
                    else {
                        // was a nonempty object, update to new page
                        iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                                          pfs->pucWorkBuffer, LW_NULL, uiOffset + uiByteHasWritten, 
                                                          &pageIXObjIXHdrNew);
                        SPIFFS_CHECK_RES(iRes);
                        SPIFFS_DBG("append: "_SPIPRIid" store new objIXHdr, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                                    pFd->objId, pageIXObjIXHdrNew, 0, uiByteHasWritten);
                    }
                } 
                else {                              /* 上一个Index页面的SpanIX != 0*/
                    // this is an update to an object index page
                    iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, spanIXObjIXPrev);
                    SPIFFS_CHECK_RES(iRes);

                    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                            SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                            SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                    SPIFFS_CHECK_RES(iRes);
                    spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer, SPIFFS_EV_IX_UPD,
                                        pFd->objId, objIX->pageHdr.spanIX, pageIXObjIXCur, 0);
                    // update length in object index header page
                    iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                                      LW_NULL, LW_NULL, 
                                                      uiOffset + uiByteHasWritten, &pageIXObjIXHdrNew);
                    SPIFFS_CHECK_RES(iRes);
                    SPIFFS_DBG("append: "_SPIPRIid" store new uiSize I "_SPIPRIi" in objIXHdr, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                                pFd->objId, uiOffset + uiByteHasWritten, pageIXObjIXHdrNew, 0, uiByteHasWritten);
                }
                pFd->uiSize = uiOffset + uiByteHasWritten;
                pFd->uiOffset = uiOffset + uiByteHasWritten;
            }

            // create or load new object index page
            if (spanIXObjIXCur == 0) {      /* 当前Index 页面的SpanIX = 0 */
                // load object index header page, must always exist
                SPIFFS_DBG("append: "_SPIPRIid" load objixhdr page "_SPIPRIpg":"_SPIPRIsp"\n", 
                            pFd->objId, pageIXObjIXCur, spanIXObjIXCur);
                /* 将其加载进内存 */    
                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                    SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                    SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                SPIFFS_CHECK_RES(iRes);
                SPIFFS_VALIDATE_OBJIX(objIXHdr->pageHdr, pFd->objId, spanIXObjIXCur);
            }   
            else {                          /* 当前Index 页面的SpanIX != 0 */
                SPIFFS_SPAN_IX spanIXObjIXLen = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, (pFd->uiSize - 1) / SPIFFS_DATA_PAGE_SIZE(pfs));
                // on subsequent passes, create a new object index page
                if (uiByteHasWritten > 0 || spanIXObjIXCur > spanIXObjIXLen) {
                    pageHeader.objId = pFd->objId | SPIFFS_OBJ_ID_IX_FLAG;
                    pageHeader.spanIX = spanIXObjIXCur;
                    pageHeader.flags = 0xff & ~(SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_INDEX);
                    iRes = spiffsPageAllocateData(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, &pageHeader, 
                                                  LW_NULL, 0, 0, LW_TRUE, &pageIXObjIXCur);
                    SPIFFS_CHECK_RES(iRes);
                    // quick "load" of new object index page
                    memset(pfs->pucWorkBuffer, 0xff, SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
                    lib_memcpy(pfs->pucWorkBuffer, &pageHeader, sizeof(SPIFFS_PAGE_HEADER));
                    spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer, SPIFFS_EV_IX_NEW, 
                                        pFd->objId, spanIXObjIXCur, pageIXObjIXCur, 0);
                    SPIFFS_DBG("append: "_SPIPRIid" create objIX page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                                pFd->objId, pageIXObjIXCur, spanIXObjIXCur, uiByteHasWritten);
                } 
                else {
                    // on first pass, we load existing object index page
                    SPIFFS_PAGE_IX pageIX;
                    SPIFFS_DBG("append: "_SPIPRIid" find objIX spanIX:"_SPIPRIsp"\n", 
                                pFd->objId, spanIXObjIXCur);
                    if (pFd->spanIXObjIXCursor == spanIXObjIXCur) {
                        pageIX = pFd->pageIXObjIXCursor;
                    } 
                    else {
                        iRes = spiffsObjLookUpFindIdAndSpan(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, 
                                                            spanIXObjIXCur, 0, &pageIX);
                        SPIFFS_CHECK_RES(iRes);
                    }
                    SPIFFS_DBG("append: "_SPIPRIid" found object index at page "_SPIPRIpg" [pFd uiSize "_SPIPRIi"]\n", 
                                pFd->objId, pageIX, pFd->uiSize);
                    iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                           SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                                           SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                    SPIFFS_CHECK_RES(iRes);
                    SPIFFS_VALIDATE_OBJIX(objIXHdr->pageHdr, pFd->objId, spanIXObjIXCur);
                    pageIXObjIXCur = pageIX;
                }
                pFd->pageIXObjIXCursor = pageIXObjIXCur;
                pFd->spanIXObjIXCursor = spanIXObjIXCur;
                pFd->uiOffset          = uiOffset+uiByteHasWritten;
                pFd->uiSize            = uiOffset+uiByteHasWritten;
            }
            spanIXObjIXPrev = spanIXObjIXCur;
        }

        // write data
        UINT32 uiByteToWrite = MIN(uiLen - uiByteHasWritten, SPIFFS_DATA_PAGE_SIZE(pfs) - uiPageOffset);
        if (uiPageOffset == 0) {
            /* 直接写在一个新的数据页 */
            // at beginning of a page, allocate and write a new page of data
            pageHeader.objId = pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG;
            pageHeader.spanIX = spanIXObjData;
            pageHeader.flags = 0xff & ~(SPIFFS_PH_FLAG_FINAL);  // finalize immediately
            iRes = spiffsPageAllocateData(pfs, pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG, &pageHeader, 
                                          &pContent[uiByteHasWritten], uiByteToWrite, uiPageOffset, 
                                          LW_TRUE, &pageIXObjData);
            SPIFFS_DBG("append: "_SPIPRIid" store new data page, "_SPIPRIpg":"_SPIPRIsp" uiOffset:"_SPIPRIi", uiLen "_SPIPRIi", uiByteHasWritten "_SPIPRIi"\n",
                        pFd->objId,pageIXObjData, spanIXObjData, uiPageOffset, uiByteToWrite, uiByteHasWritten);
        } 
        else {
            // append to existing page, fill out free data in existing page
            /* 追加写在已有的数据页之后 */
            if (spanIXObjIXCur == 0) {
                // get data page from object index header page
                pageIXObjData = ((SPIFFS_PAGE_IX*)((PCHAR)objIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData];
            } 
            else {
                // get data page from object index page
                pageIXObjData = ((SPIFFS_PAGE_IX*)((PCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)];
            }

            iRes = spiffsPageDataCheck(pfs, pFd, pageIXObjData, spanIXObjData);
            SPIFFS_CHECK_RES(iRes);

            iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                    SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + sizeof(SPIFFS_PAGE_HEADER) + uiPageOffset, 
                                    uiByteToWrite, &pContent[uiByteHasWritten]);
            SPIFFS_DBG("append: "_SPIPRIid" store to existing data page, "_SPIPRIpg":"_SPIPRIsp" uiOffset:"_SPIPRIi", uiLen "_SPIPRIi", uiByteHasWritten "_SPIPRIi"\n", 
                        pFd->objId , pageIXObjData, spanIXObjData, uiPageOffset, uiByteToWrite, uiByteHasWritten);
        }

        if (iRes != SPIFFS_OK) 
            break;

        // update memory representation of object index page with new data page
        /* 根据当前内存中缓存的Index页面来更新Entry */
        if (spanIXObjIXCur == 0) {
            // update object index header page
            ((SPIFFS_PAGE_IX*)((PCHAR)objIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData] = pageIXObjData;
            SPIFFS_DBG("append: "_SPIPRIid" wrote page "_SPIPRIpg" to objIXHdr entry "_SPIPRIsp" in mem\n", 
                        pFd->objId, pageIXObjData, spanIXObjData);
            objIXHdr->uiSize = uiOffset + uiByteHasWritten;
        } 
        else {
            // update object index page
            ((SPIFFS_PAGE_IX*)((PCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)] = pageIXObjData;
            SPIFFS_DBG("append: "_SPIPRIid" wrote page "_SPIPRIpg" to objIX entry "_SPIPRIsp" in mem\n", 
                        pFd->objId, pageIXObjData, (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData));
        }

        // update internals
        uiPageOffset = 0;
        spanIXObjData++;
        uiByteHasWritten += uiByteToWrite;
    } // while all data

    pFd->uiSize = uiOffset + uiByteHasWritten;
    pFd->uiOffset = uiOffset + uiByteHasWritten;
    pFd->pageIXObjIXCursor = pageIXObjIXCur;    /* 更新当前的写指针 */
    pFd->spanIXObjIXCursor = spanIXObjIXCur;

    // finalize updated object indices
    INT32 iRes2 = SPIFFS_OK;
    /* 最后再次更新索引 */
    if (spanIXObjIXCur != 0) {
        // wrote beyond object index header page
        // write last modified object index page, unless object header index page
        SPIFFS_DBG("append: "_SPIPRIid" store objIX page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                    pFd->objId,pageIXObjIXCur, spanIXObjIXCur, uiByteHasWritten);

        iRes2 = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, spanIXObjIXCur);
        SPIFFS_CHECK_RES(iRes2);

        iRes2 = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                 SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                 SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
        SPIFFS_CHECK_RES(iRes2);
        spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer, SPIFFS_EV_IX_UPD, 
                            pFd->objId, objIX->pageHdr.spanIX, pageIXObjIXCur, 0);

        // update uiSize in object header index page
        iRes2 = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                          LW_NULL, LW_NULL, uiOffset+uiByteHasWritten, &pageIXObjIXHdrNew);
        SPIFFS_DBG("append: "_SPIPRIid" store new uiSize II "_SPIPRIi" in objIXHdr, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi", iRes "_SPIPRIi"\n", 
                    pFd->objId, uiOffset + uiByteHasWritten, pageIXObjIXHdrNew, 0, uiByteHasWritten, iRes2);
        SPIFFS_CHECK_RES(iRes2);
    } 
    else {  /* spanIXObjIXCur == 0 */
        // wrote within object index header page
        if (uiOffset == 0) {
            // wrote to empty object - simply update uiSize and write whole page
            /* uiOffset = 0预示这是一个新的Obj，因此我们调用CacheWrite即可 */
            objIXHdr->uiSize = uiOffset + uiByteHasWritten;
            SPIFFS_DBG("append: "_SPIPRIid" store fresh objIXHdr page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                        pFd->objId, pageIXObjIXCur, spanIXObjIXCur, uiByteHasWritten);

            iRes2 = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, spanIXObjIXCur);
            SPIFFS_CHECK_RES(iRes2);

            iRes2 = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_UPDT, pFd->fileN, 
                                     SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                     SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
            SPIFFS_CHECK_RES(iRes2);
            // callback on object index update
            spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer, SPIFFS_EV_IX_UPD_HDR, 
                                pFd->objId, objIXHdr->pageHdr.spanIX, pageIXObjIXCur, objIXHdr->uiSize);
        } 
        else {
            /* uiOffset != 0预示这是一个已经存在的Obj，我们需要异地更新它的大小，因此调用spiffsObjectUpdateIndexHdr */
            // modifying object index header page, update uiSize and make new copy
            iRes2 = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                               pfs->pucWorkBuffer, LW_NULL, 
                                               uiOffset + uiByteHasWritten, &pageIXObjIXHdrNew);
            SPIFFS_DBG("append: "_SPIPRIid" store modified objIXHdr page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                        pFd->objId , pageIXObjIXHdrNew, 0, uiByteHasWritten);
            SPIFFS_CHECK_RES(iRes2);
        }
    }

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectModify
** 功能描述: 用于截断一个Object
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectModify(PSPIFFS_FD pFd, UINT32 uiOffset, PCHAR pContent, UINT32 uiLen){
    PSPIFFS_VOLUME pfs = pFd->pfs;
    INT32 iRes = SPIFFS_OK;
    UINT32 uiByteHasWritten = 0;

    iRes = spiffsGCCheck(pfs, uiLen + SPIFFS_DATA_PAGE_SIZE(pfs));
    SPIFFS_CHECK_RES(iRes);
    
    PSPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr = (PSPIFFS_PAGE_OBJECT_IX_HEADER)pfs->pucWorkBuffer; /* SpanIX为0的索引页面Header */
    PSPIFFS_PAGE_OBJECT_IX        objIX = (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer;           /* SpanIX非0的索引页面Header */
    SPIFFS_PAGE_HEADER            pageHeader;                                                   /* 普通页面的Header */

    SPIFFS_SPAN_IX spanIXObjIXCur = 0;                                                          /* 当前Index页面的Span IX */
    SPIFFS_SPAN_IX spanIXObjIXPrev = (SPIFFS_SPAN_IX)-1;                                        /* 前一个Index页面的Span IX */
    SPIFFS_PAGE_IX pageIXObjIXCur = pFd->pageIXObjIXHdr;                                        /* 当前Index页面的Page IX */
    SPIFFS_PAGE_IX pageIXObjIXHdrNew;                                                           /* 最新的 SpanIX = 0的Index页面的SpanIX */
    SPIFFS_PAGE_IX pageIXObjIXNew;                                                              /* 最新的 SpanIX != 0的Index页面的SpanIX */
    
    SPIFFS_SPAN_IX spanIXObjData = uiOffset / SPIFFS_DATA_PAGE_SIZE(pfs);                       /* Data页面的SpanIX */
    SPIFFS_PAGE_IX pageIXObjData;                                                               /* Data页面的PageIX */                                
    UINT32 uiPageOffset = uiOffset % SPIFFS_DATA_PAGE_SIZE(pfs);                                /* Data页面页内偏移 */


    // write all data
    while (iRes == SPIFFS_OK 
           && uiByteHasWritten < uiLen) {
        // calculate object index page span index
        spanIXObjIXCur = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIXObjData);                       /* 根据Data页面的spanIX计算当前Index页面的spanIX */

        // handle storing and loading of object indices
        if (spanIXObjIXCur != spanIXObjIXPrev) {                                                /* 当前Index页面的spanIX与上一个Inxdex页的spanIX不一样 */
            // new object index page
            // within this clause we return directly if something fails, object index mess-up
            if (uiByteHasWritten > 0) {
                // store previous object index (header) page, unless first pass
                if (spanIXObjIXPrev == 0) {                                                     /* 上一个Index页面的SpanIX = 0 */
                    // store previous object index header page
                    iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                                      pfs->pucWorkBuffer, LW_NULL, 0, 
                                                      &pageIXObjIXHdrNew);
                    SPIFFS_DBG("modify: store modified objIXHdr page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                                pageIXObjIXHdrNew, 0, uiByteHasWritten);
                    SPIFFS_CHECK_RES(iRes);
                } 
                else {
                    // store new version of previous object index page
                    iRes = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, spanIXObjIXPrev);
                    SPIFFS_CHECK_RES(iRes);
                    
                    iRes = spiffsPageMove(pfs, pFd->fileN, (PCHAR)objIX, pFd->objId, 
                                          0, pageIXObjIXCur, &pageIXObjIXNew);
                    SPIFFS_DBG("modify: store previous modified objIX page, "_SPIPRIid":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                                pageIXObjIXNew, objIX->pageHdr.spanIX, uiByteHasWritten);
                    SPIFFS_CHECK_RES(iRes);
                    spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)objIX,
                                        SPIFFS_EV_IX_UPD, pFd->objId, 
                                        objIX->pageHdr.spanIX, pageIXObjIXNew, 0);
                }
            }
            // load next object index page
            if (spanIXObjIXCur == 0) {
                // load object index header page, must exist
                SPIFFS_DBG("modify: load objixhdr page "_SPIPRIpg":"_SPIPRIsp"\n", 
                            pageIXObjIXCur, spanIXObjIXCur);
                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                        SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIXCur), 
                                        SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                SPIFFS_CHECK_RES(iRes);
                SPIFFS_VALIDATE_OBJIX(objIXHdr->pageHdr, pFd->objId, spanIXObjIXCur);
            } 
            else {
                // load existing object index page on first pass
                SPIFFS_PAGE_IX pageIX;
                SPIFFS_DBG("modify: find objIX spanIX:"_SPIPRIsp"\n", spanIXObjIXCur);
                if (pFd->spanIXObjIXCursor == spanIXObjIXCur) {
                    pageIX = pFd->pageIXObjIXCursor;
                } 
                else {
                    iRes = spiffsObjLookUpFindIdAndSpan(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, 
                                                        spanIXObjIXCur, 0, &pageIX);
                    SPIFFS_CHECK_RES(iRes);
                }
                SPIFFS_DBG("modify: found object index at page "_SPIPRIpg"\n", pageIX);
                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                       SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                                       SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                SPIFFS_CHECK_RES(iRes);
                SPIFFS_VALIDATE_OBJIX(objIXHdr->pageHdr, pFd->objId, spanIXObjIXCur);
                pageIXObjIXCur = pageIX;
            }
            
            pFd->pageIXObjIXCursor = pageIXObjIXCur;
            pFd->spanIXObjIXCursor = spanIXObjIXCur;
            pFd->uiOffset = uiOffset + uiByteHasWritten;
            spanIXObjIXPrev = spanIXObjIXCur;
        }

        // write partial data
        UINT32         uiByteToWrite = MIN(uiLen - uiByteHasWritten, SPIFFS_DATA_PAGE_SIZE(pfs) - uiPageOffset);
        SPIFFS_PAGE_IX pageIXObjDataOrigin;
        if (spanIXObjIXCur == 0) {
            // get data page from object index header page
            pageIXObjDataOrigin = ((SPIFFS_PAGE_IX *)((PCHAR)objIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData];
        } 
        else {
            // get data page from object index page
            pageIXObjDataOrigin = ((SPIFFS_PAGE_IX*)((PCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)];
        }

        pageHeader.objId = pFd->objId & ~SPIFFS_OBJ_ID_IX_FLAG;
        pageHeader.spanIX = spanIXObjData;
        pageHeader.flags = 0xff;
        if (uiPageOffset == 0 && 
            uiByteToWrite == SPIFFS_DATA_PAGE_SIZE(pfs)) {
            // a full page, allocate and write a new page of data
            /* 可以写满一个页面 */
            iRes = spiffsPageAllocateData(pfs, pageHeader.objId, &pageHeader, 
                                          &pContent[uiByteHasWritten], uiByteToWrite, 
                                          uiPageOffset, LW_TRUE, &pageIXObjData);
            SPIFFS_DBG("modify: store new data page, "_SPIPRIpg":"_SPIPRIsp" uiOffset:"_SPIPRIi", uiLen "_SPIPRIi", uiByteHasWritten "_SPIPRIi"\n", 
                        pageIXObjData, spanIXObjData, uiPageOffset, uiByteToWrite, uiByteHasWritten);
        } 
        else {
            // write to existing page, allocate new and copy unmodified data

            iRes = spiffsPageDataCheck(pfs, pFd, pageIXObjDataOrigin, spanIXObjData);
            SPIFFS_CHECK_RES(iRes);
            /* 分配一个数据页面 */
            iRes = spiffsPageAllocateData(pfs, pageHeader.objId, &pageHeader, 
                                          LW_NULL, 0, 0, LW_FALSE, &pageIXObjData);
            if (iRes != SPIFFS_OK) 
                break;

            // copy unmodified data
            if (uiPageOffset > 0) {
                // before modification
                iRes = spiffsPhysCpy(pfs, pFd->fileN,
                                     SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + sizeof(SPIFFS_PAGE_HEADER),
                                     SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjDataOrigin) + sizeof(SPIFFS_PAGE_HEADER),
                                     uiPageOffset);
                if (iRes != SPIFFS_OK) 
                    break;
            }
            if (uiPageOffset + uiByteToWrite < SPIFFS_DATA_PAGE_SIZE(pfs)) {
                // after modification
                iRes = spiffsPhysCpy(pfs, pFd->fileN,
                                     SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + sizeof(SPIFFS_PAGE_HEADER) + uiPageOffset + uiByteToWrite,
                                     SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjDataOrigin) + sizeof(SPIFFS_PAGE_HEADER) + uiPageOffset + uiByteToWrite,
                                     SPIFFS_DATA_PAGE_SIZE(pfs) - (uiPageOffset + uiByteToWrite));
                if (iRes != SPIFFS_OK) 
                    break;
            }

            iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, pFd->fileN,
                                    SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + sizeof(SPIFFS_PAGE_HEADER) + uiPageOffset, 
                                    uiByteToWrite, &pContent[uiByteHasWritten]);
            if (iRes != SPIFFS_OK) 
                break;

            pageHeader.flags &= ~SPIFFS_PH_FLAG_FINAL;
            iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, pFd->fileN,
                                    SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + offsetof(SPIFFS_PAGE_HEADER, flags),
                                    sizeof(UINT8), (PCHAR)&pageHeader.flags);
            if (iRes != SPIFFS_OK) 
                break;

            SPIFFS_DBG("modify: store to existing data page, src:"_SPIPRIpg", dst:"_SPIPRIpg":"_SPIPRIsp" uiOffset:"_SPIPRIi", uiLen "_SPIPRIi", uiByteHasWritten "_SPIPRIi"\n", 
                        pageIXObjDataOrigin, pageIXObjData, spanIXObjData, uiPageOffset, 
                        uiByteToWrite, uiByteHasWritten);
        }

        // delete original data page
        iRes = spiffsPageDelete(pfs, pageIXObjDataOrigin);
        if (iRes != SPIFFS_OK) 
            break;
        // update memory representation of object index page with new data page
        if (spanIXObjIXCur == 0) {
            // update object index header page
            ((SPIFFS_PAGE_IX*)((PCHAR)objIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData] = pageIXObjData;
            SPIFFS_DBG("modify: wrote page "_SPIPRIpg" to objIXHdr entry "_SPIPRIsp" in mem\n", pageIXObjData, spanIXObjData);
        } else {
            // update object index page
            ((SPIFFS_PAGE_IX*)((PCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)] = pageIXObjData;
            SPIFFS_DBG("modify: wrote page "_SPIPRIpg" to objIX entry "_SPIPRIsp" in mem\n", pageIXObjData, (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData));
        }

        // update internals
        uiPageOffset = 0;
        spanIXObjData++;
        uiByteHasWritten += uiByteToWrite;
    } // while all data

    pFd->uiOffset = uiOffset + uiByteHasWritten;
    pFd->pageIXObjIXCursor = pageIXObjIXCur;
    pFd->spanIXObjIXCursor = spanIXObjIXCur;

    // finalize updated object indices
    INT32 iRes2 = SPIFFS_OK;
    if (spanIXObjIXCur != 0) {
        // wrote beyond object index header page
        // write last modified object index page
        // move and update page
        SPIFFS_PAGE_IX pageIXObjIXNew;

        iRes2 = spiffsPageIndexCheck(pfs, pFd, pageIXObjIXCur, spanIXObjIXCur);
        SPIFFS_CHECK_RES(iRes2);

        iRes2 = spiffsPageMove(pfs, pFd->fileN, (PCHAR)objIX, pFd->objId, LW_NULL, 
                               pageIXObjIXCur, &pageIXObjIXNew);
        SPIFFS_DBG("modify: store modified objIX page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", pageIXObjIXNew, spanIXObjIXCur, uiByteHasWritten);
        pFd->pageIXObjIXCursor = pageIXObjIXNew;
        pFd->spanIXObjIXCursor = spanIXObjIXCur;
        SPIFFS_CHECK_RES(iRes2);
        spiffsCBObjectEvent(pfs, (PSPIFFS_PAGE_OBJECT_IX)objIX, SPIFFS_EV_IX_UPD, 
                            pFd->objId, objIX->pageHdr.spanIX, pageIXObjIXNew, 0);

    } 
    else {
        // wrote within object index header page
        iRes2 = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                           pfs->pucWorkBuffer, LW_NULL, 0, &pageIXObjIXHdrNew);
        SPIFFS_DBG("modify: store modified objIXHdr page, "_SPIPRIpg":"_SPIPRIsp", uiByteHasWritten "_SPIPRIi"\n", 
                    pageIXObjIXHdrNew, 0, uiByteHasWritten);
        SPIFFS_CHECK_RES(iRes2);
    }

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectRead
** 功能描述: 从一个Object中读取内容至puDst
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsObjectRead(PSPIFFS_FD pFd, UINT32 uiOffset, UINT32 uiLen, PCHAR puDst){
    INT32          iRes = SPIFFS_OK;
    PSPIFFS_VOLUME pfs = pFd->pfs;                                               /* 文件头 */                                                   
    SPIFFS_PAGE_IX pageIXObjIX;                                                 /* 当前Index页面的page index */
    SPIFFS_PAGE_IX pageIXObjData;                                               /* 当前Data页面的page index */
    SPIFFS_SPAN_IX spanIXObjData = uiOffset / SPIFFS_DATA_PAGE_SIZE(pfs);       /* 当前Data页面的Span index */
    UINT32         uiCurOffset = uiOffset;                                      /* 当前文件Obj的读偏移 */
    SPIFFS_SPAN_IX spanIXObjIXCur;                                              /* 当前Index页面的Span Index */
    SPIFFS_SPAN_IX spanIXObjIXPrev = (SPIFFS_SPAN_IX)-1;                        /* 先前Index页面的Span Index */
    
    PSPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr = (PSPIFFS_PAGE_OBJECT_IX_HEADER)pfs->pucWorkBuffer;
    PSPIFFS_PAGE_OBJECT_IX        objIX = (PSPIFFS_PAGE_OBJECT_IX)pfs->pucWorkBuffer;

    while (uiCurOffset < uiOffset + uiLen) {
        //TODO: SPIFFS_IX_MAP
    // #if SPIFFS_IX_MAP
    // // check if we have a memory, index map and if so, if we're within index map'pStat range
    // // and if so, if the entry is populated
    // if (pFd->ix_map && spanIXObjData >= pFd->ix_map->start_spix && spanIXObjData <= pFd->ix_map->end_spix
    //     && pFd->ix_map->map_buf[spanIXObjData - pFd->ix_map->start_spix]) {
    //     pageIXObjData = pFd->ix_map->map_buf[spanIXObjData - pFd->ix_map->start_spix];
    // } else {
    // #endif
        spanIXObjIXCur = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIXObjData);
        if (spanIXObjIXPrev != spanIXObjIXCur) {
            // load current object index (header) page
            if (spanIXObjIXCur == 0) {
                pageIXObjIX = pFd->pageIXObjIXHdr;
            } 
            else {
                SPIFFS_DBG("read: find objIX "_SPIPRIid":"_SPIPRIsp"\n", 
                            pFd->objId, spanIXObjIXCur);
                if (pFd->spanIXObjIXCursor == spanIXObjIXCur) {
                    pageIXObjIX = pFd->pageIXObjIXCursor;
                } 
                else {
                    iRes = spiffsObjLookUpFindIdAndSpan(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, spanIXObjIXCur, 
                                                        0, &pageIXObjIX);
                    SPIFFS_CHECK_RES(iRes);
                }
            }
            SPIFFS_DBG("read: load objIX page "_SPIPRIpg":"_SPIPRIsp" for data spix:"_SPIPRIsp"\n", pageIXObjIX, spanIXObjIXCur, spanIXObjData);
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, pFd->fileN, 
                                   SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIX), 
                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
            SPIFFS_CHECK_RES(iRes);
            SPIFFS_VALIDATE_OBJIX(objIX->pageHdr, pFd->objId, spanIXObjIXCur);

            pFd->uiOffset = uiCurOffset;
            pFd->pageIXObjIXCursor = pageIXObjIX;
            pFd->spanIXObjIXCursor = spanIXObjIXCur;

            spanIXObjIXPrev = spanIXObjIXCur;
        }

        /* 获取数据页面的索引 */
        if (spanIXObjIXCur == 0) {
            // get data page from object index header page
            /* Index Header中的内容就是一个个的页面 */
            pageIXObjData = ((SPIFFS_PAGE_IX*)((PCHAR)objIXHdr + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[spanIXObjData];
        } else {
            // get data page from object index page
            pageIXObjData = ((SPIFFS_PAGE_IX*)((PCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, spanIXObjData)];
        }
    // #if SPIFFS_IX_MAP
    // }
    // #endif
        // all remaining data
        UINT32 uiByteToRead = uiOffset + uiLen - uiCurOffset;
        // remaining data in page
        uiByteToRead = MIN(uiByteToRead, SPIFFS_DATA_PAGE_SIZE(pfs) - (uiCurOffset % SPIFFS_DATA_PAGE_SIZE(pfs)));
        // remaining data in file
        uiByteToRead = MIN(uiByteToRead, pFd->uiSize - uiCurOffset);
        SPIFFS_DBG("read: uiOffset:"_SPIPRIi" rd:"_SPIPRIi" data spix:"_SPIPRIsp" is pageIXObjData:"_SPIPRIpg" addr:"_SPIPRIad"\n", 
                    uiCurOffset, uiByteToRead, spanIXObjData, pageIXObjData, (UINT32)(SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + sizeof(SPIFFS_PAGE_HEADER) + (uiCurOffset % SPIFFS_DATA_PAGE_SIZE(pfs))));
        if (uiByteToRead <= 0) {
            iRes = SPIFFS_ERR_END_OF_OBJECT;
            break;
        }
        iRes = spiffsPageDataCheck(pfs, pFd, pageIXObjData, spanIXObjData);
        SPIFFS_CHECK_RES(iRes);
        
        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_READ, pFd->fileN, 
                               (SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjData) + 
                               sizeof(SPIFFS_PAGE_HEADER) + 
                               (uiCurOffset % SPIFFS_DATA_PAGE_SIZE(pfs))),             /* 获取偏移地址 */
                               uiByteToRead,
                               puDst);

        SPIFFS_CHECK_RES(iRes);
        puDst += uiByteToRead;
        uiCurOffset += uiByteToRead;
        pFd->uiOffset = uiCurOffset;
        spanIXObjData++;
    }

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsFileWrite
** 功能描述: 向一个文件中写入
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsFileWrite(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PCHAR pcContent, 
                      UINT32 uiOffset, INT32 iLen){
    INT32       iRes = SPIFFS_OK;
    INT32       iRemaining = iLen;
    PSPIFFS_FD  pFd;
    PCHAR      pData = pcContent; 
    
    spiffsFdGet(pfs, fileHandler, &pFd);            /* 获取文件描述符 */
    
    if (pFd->uiSize != SPIFFS_UNDEFINED_LEN && 
        uiOffset < pFd->uiSize) {                   /* 属于修改一部分 */
        INT32 iMinLen = MIN((INT32)(pFd->uiSize - uiOffset), iLen);
        iRes = spiffsObjectModify(pFd, uiOffset, pData, iMinLen);
        SPIFFS_CHECK_RES(iRes);
        iRemaining -= iMinLen;
        pData += iMinLen;
        uiOffset += iMinLen;
    }
    if (iRemaining > 0) {                           /* 剩下的追加写即可 */
        iRes = spiffsObjectAppend(pFd, uiOffset, pData, iRemaining);
        SPIFFS_CHECK_RES(iRes);
    }
    return iLen;
}
/*********************************************************************************************************
** 函数名称: spiffsObjectTruncate
** 功能描述: 用于截断一个Object
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsFileRead(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PCHAR pcContent, INT32 iLen) {
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    PSPIFFS_FD pFd;
    INT32 iRes;

    iRes = spiffsFdGet(pfs, fileHandler, &pFd);     /* 根据FileHander获取Fd */

    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_CHECK_RES(iRes);

    if ((pFd->flags & SPIFFS_O_RDONLY) == 0) {
        iRes = SPIFFS_ERR_NOT_READABLE;
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
    }

    if (pFd->uiSize == SPIFFS_UNDEFINED_LEN 
        && iLen > 0) {
        // special case for zero sized files
        iRes = SPIFFS_ERR_END_OF_OBJECT;
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
    }

    /* 把Cache里的内容刷回Flash，保证写后读一致性 */
    spiffsCacheFflush(pfs, fileHandler);


    if (pFd->uiFdOffset + iLen >= pFd->uiSize) {        /* 当前文件内偏移 + 长度 >= 文件大小 */
        // reading beyond file uiSize
        INT32 iAvail = pFd->uiSize - pFd->uiFdOffset;   /* 只可以读iAvail这么多 */
        if (iAvail <= 0) {
            //SPIFFS_API_CHECK_RES_UNLOCK(pfs, SPIFFS_ERR_END_OF_OBJECT);
            iRes = SPIFFS_ERR_END_OF_OBJECT;
            SPIFFS_CHECK_RES(iRes);
        }
        iRes = spiffsObjectRead(pFd, pFd->uiFdOffset, iAvail, pcContent);
        if (iRes == SPIFFS_ERR_END_OF_OBJECT) {     /* 正常现象 */
            pFd->uiFdOffset += iAvail;
            //SPIFFS_UNLOCK(pfs);
            return iAvail;
        } 
        else {
            //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
            SPIFFS_CHECK_RES(iRes);
            iLen = iAvail;
        }
    } 
    else {
        /* 可以读完，GG */
        // reading within file uiSize
        iRes = spiffsObjectRead(pFd, pFd->uiFdOffset, iLen, pcContent);
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
    }
    /* 更新文件指针 */
    pFd->uiFdOffset += iLen;
    //SPIFFS_UNLOCK(pfs);

    return iLen;
}
/*********************************************************************************************************
** 函数名称: spiffsStatPageIX
** 功能描述: 用于描述页面状态
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsStatPageIX(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX, SPIFFS_FILE fileHandler, PSPIFFS_STAT pStat) {
    (VOID)fileHandler;
    SPIFFS_PAGE_OBJECT_IX_HEADER objIXHdr;
    SPIFFS_OBJ_ID objId;
    /* 需要保证pageIX的spanIX为0 */
    INT32 iRes = spiffsCacheRead(pfs,  SPIFFS_OP_T_OBJ_IX | SPIFFS_OP_C_READ, fileHandler,
                                SPIFFS_PAGE_TO_PADDR(pfs, pageIX), 
                                sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER), (PCHAR)&objIXHdr);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //TODO:为啥需要计算ObjID对应的地址呢？objIXHdr.pageHdr.objId不是可以直接获取吗
    UINT32 uiObjIdAddr = SPIFFS_BLOCK_TO_PADDR(pfs, SPIFFS_BLOCK_FOR_PAGE(pfs , pageIX)) +
                                               SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIX) * sizeof(SPIFFS_OBJ_ID);
    iRes = spiffsCacheRead(pfs,  SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, fileHandler,
                           uiObjIdAddr, sizeof(SPIFFS_OBJ_ID), 
                           (PCHAR)&objId);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    pStat->objId = objId & ~SPIFFS_OBJ_ID_IX_FLAG;          /* 取消索引号 */
    pStat->objType = objIXHdr.type;
    pStat->uiSize = objIXHdr.uiSize == SPIFFS_UNDEFINED_LEN ? 0 : objIXHdr.uiSize;
    pStat->pageIX = pageIX;
    strncpy((PCHAR)pStat->ucName, (PCHAR)objIXHdr.ucName, SPIFFS_OBJ_NAME_LEN);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsTranslateToSylixOSFlag
** 功能描述: 转换Flag
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
SPIFFS_FLAGS spiffsTranslateToSylixOSFlag(INT iFlag){
    SPIFFS_FLAGS flags = 0;
    if((iFlag & O_CREAT) == O_CREAT){
        flags |= SPIFFS_O_CREAT;
    }
    if((iFlag & O_APPEND) == O_APPEND){
        flags |= SPIFFS_O_APPEND;
    }
    if((iFlag & O_TRUNC) == O_TRUNC){
        flags |= SPIFFS_O_TRUNC;
    }
    if((iFlag & O_EXCL) == O_EXCL){
        flags |= SPIFFS_O_EXCL;
    }
    if((iFlag & O_RDONLY) == O_RDONLY){       
        flags |= SPIFFS_O_RDONLY;
    }
    if((iFlag & O_WRONLY) == O_WRONLY){
        flags |= SPIFFS_O_WRONLY;
    }
    if((iFlag & O_RDWR) == O_RDWR){
        flags |= SPIFFS_O_RDWR;
    }
    return flags;
}
/*********************************************************************************************************
** 函数名称: spiffsDirRead
** 功能描述: 用于描述页面状态
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_DIRENT spiffsDirRead(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent){
    SPIFFS_BLOCK_IX blkIX;
    INT             iEntry;
    INT32           iRes;

    iRes =  spiffsObjLookUpFindEntryVisitor(pDir->pfs, pDir->blkIX, pDir->uiEntry,
                                            SPIFFS_VIS_NO_WRAP,
                                            0,
                                            __spiffsReadDirVisitor,
                                            0,
                                            pDirent,
                                            &blkIX,
                                            &iEntry);
    if (iRes == SPIFFS_OK) {
        pDir->blkIX = blkIX;
        pDir->uiEntry = iEntry + 1;
        pDirent->objId &= ~SPIFFS_OBJ_ID_IX_FLAG;
        return pDirent;
    } 
    else {
        pDir->pfs->uiErrorCode = iRes;
    }

    return LW_NULL;
}
