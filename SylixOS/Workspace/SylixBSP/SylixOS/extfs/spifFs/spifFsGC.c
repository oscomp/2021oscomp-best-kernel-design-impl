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
** 文   件   名: spifFsGC.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统垃圾回收器
*********************************************************************************************************/
#include "spifFsGC.h"
#include "spifFsCache.h"
#include "spifFsLib.h"
#include "spifFsFDLib.h"

INT32 __spiffsGCEraseBlk(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIX) {
    INT32   iRes;
    UINT32  i;

    SPIFFS_GC_DBG("gc: erase block "_SPIPRIbl"\n", blkIX);
    iRes = spiffsEraseBlk(pfs, blkIX);
    SPIFFS_CHECK_RES(iRes);

    for (i = 0; i < SPIFFS_PAGES_PER_BLOCK(pfs); i++) {
        spiffsCacheDropPage(pfs, SPIFFS_PAGE_FOR_BLOCK(pfs, blkIX) + i);
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffsPageDelete
** 功能描述: 删除页面，并删除所在lookup page中的 entry
** 输　入  : pfs          文件头
**           uiLen        申请创建长度
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsPageDelete(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX){
    INT32 iRes;
    UINT8 uiFlags = 0xff;
    /* 删除Entry */
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_DELE, 0,
                            SPIFFS_BLOCK_TO_PADDR(pfs, SPIFFS_BLOCK_FOR_PAGE(pfs, pageIX)) 
                            + SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIX) * sizeof(SPIFFS_OBJ_ID),
                            sizeof(SPIFFS_OBJ_ID), (PUCHAR)SPIFFS_OBJ_ID_DELETED);
    SPIFFS_CHECK_RES(iRes);

    pfs->uiStatsPageAllocated--;
    pfs->uiStatsPageDeleted++;
    
    uiFlags &= ~(SPIFFS_PH_FLAG_DELET | SPIFFS_PH_FLAG_USED);
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_DELE, 0,
                            SPIFFS_PAGE_TO_PADDR(pfs, pageIX) + offsetof(SPIFFS_PAGE_HEADER, flags),
                            sizeof(UINT8), (PUCHAR)&uiFlags);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsPageMove
** 功能描述: 移动页面到空闲处
** 输　入  : pfs          文件头
**           uiLen        申请创建长度
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsPageMove(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PUCHAR pucPageData,
                     SPIFFS_OBJ_ID objId, PSPIFFS_PAGE_HEADER pPageHeader, SPIFFS_PAGE_IX pageIXSrc,
                     SPIFFS_PAGE_IX *pPageIXDst){
    INT32 iRes;
    BOOL bIsFinal = 0;
    SPIFFS_PAGE_HEADER *pPageHeaderRef;
    SPIFFS_BLOCK_IX blkIX;
    INT iEntry;
    SPIFFS_PAGE_IX pageIXFree;

    /* 寻找一个空闲的Entry*/
    iRes = spiffsObjLookUpFindFreeEntry(pfs, pfs->blkIXFreeCursor, pfs->objLookupEntryFreeCursor, &blkIX, &iEntry);
    SPIFFS_CHECK_RES(iRes);
    /* 根据该Entry找到相应的Page */
    pageIXFree = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry);

    if (pPageIXDst) 
        *pPageIXDst = pageIXFree;

    pPageHeaderRef = pucPageData ? (PSPIFFS_PAGE_HEADER)pucPageData : pPageHeader;
    if (pucPageData) {
        /* 检查是否是完成修改？ */
        bIsFinal = (pPageHeaderRef->flags & SPIFFS_PH_FLAG_FINAL) == 0;
        // write unfinalized page
        /* 正在被修改 */
        pPageHeaderRef->flags |= SPIFFS_PH_FLAG_FINAL;
        pPageHeaderRef->flags &= ~SPIFFS_PH_FLAG_USED;
        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, 0, 
                                SPIFFS_PAGE_TO_PADDR(pfs, pageIXFree), SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), 
                                pucPageData);
    } 
    else {
        // copy page data
        iRes = spiffsPhysCpy(pfs, fileHandler, SPIFFS_PAGE_TO_PADDR(pfs, pageIXFree), 
                             SPIFFS_PAGE_TO_PADDR(pfs, pageIXSrc), SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
    }
    SPIFFS_CHECK_RES(iRes);

    // mark entry in destination object lookup
    /* 为页面分配lookup entry */
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_UPDT, 0, 
                            SPIFFS_BLOCK_TO_PADDR(pfs, SPIFFS_BLOCK_FOR_PAGE(pfs, pageIXFree)) 
                            + SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIXFree) * sizeof(SPIFFS_OBJ_ID),
                            sizeof(SPIFFS_OBJ_ID), (PUCHAR)&objId);
    SPIFFS_CHECK_RES(iRes);

    pfs->uiStatsPageAllocated++;

    if (bIsFinal) {
        // mark finalized in destination page
        /*  
            FinalFlag 0：写完了；
            UsedFlag 0：正在被使用 
        */
        pPageHeaderRef->flags &= ~(SPIFFS_PH_FLAG_FINAL | SPIFFS_PH_FLAG_USED);
        iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_DA | SPIFFS_OP_C_UPDT, fileHandler,
                                SPIFFS_PAGE_TO_PADDR(pfs, pageIXFree) + offsetof(SPIFFS_PAGE_HEADER, flags),
                                sizeof(UINT8), (PUCHAR)&pPageHeaderRef->flags);
        SPIFFS_CHECK_RES(iRes);
    }
    // mark source deleted
    /* 删除原页面 */
    iRes = __spiffsPageDelete(pfs, pageIXSrc);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffsGCFindCandiate
** 功能描述: 搜集受害者块
** 输　入  : pfs          文件头
**           uiLen        申请创建长度
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsGCFindCandiate(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX **ppBlkIXCandidates, INT *piCandidateCount,
                             BOOL bIsFsCrammed){
    INT32               iRes = SPIFFS_OK;
    UINT32              uiBlks = pfs->uiBlkCount;
    SPIFFS_BLOCK_IX     blkIXCur = 0;
    UINT32              uiBlkIXCurAddr = 0;
    SPIFFS_OBJ_ID*      pObjLookUpBuffer = (SPIFFS_OBJ_ID*)pfs->pucLookupWorkBuffer;
    INT                 iEntryCur = 0;
    /* 利用pfs->pucWorkBuffer来对Candidate进行排序 */
    INT                 iMaxCandidates = MIN(uiBlks, (SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - 8) / (sizeof(SPIFFS_BLOCK_IX) + sizeof(INT32)));
    
    SPIFFS_BLOCK_IX*    pCandidatesBlkIX; 
    INT32*              pCandidatesScores;
    
    INT                 iEntriesPerPage = (SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(SPIFFS_OBJ_ID));
    UINT16              uiDeletedPagesInBlks = 0;
    UINT16              uiUsedPagesInBlks = 0;
    
    SPIFFS_PAGE_IX      pageIXLookUp = 0;
    INT                 iEntryOffset;
    SPIFFS_OBJ_ID       objIdEraseCount;
    SPIFFS_OBJ_ID       objIdEraseAge;
    SPIFFS_OBJ_ID       objId;
    INT32               iCandidateIX;
    INT32               iReorderCandidateIX;
    INT32               iScore;

    *piCandidateCount = 0;
    lib_memset(pfs->pucWorkBuffer, -1, SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
    
    pCandidatesBlkIX = (SPIFFS_BLOCK_IX *)pfs->pucWorkBuffer;
    pCandidatesScores = (INT32 *)(pfs->pucWorkBuffer + iMaxCandidates * sizeof(SPIFFS_BLOCK_IX));
    /* INT32对齐（4字节对齐） */
    pCandidatesScores = (INT32 *)(((intptr_t)pCandidatesScores + sizeof(intptr_t) - 1) & ~(sizeof(intptr_t) - 1));

    *ppBlkIXCandidates = pCandidatesBlkIX;

    //TODO:为啥不用Vistor实现？？？
    /* 遍历每个Blk */
    while (iRes == SPIFFS_OK && uiBlks--)
    {
        pageIXLookUp = 0;
        /* 查看每个Look Up Page */
        while (iRes == SPIFFS_OK &&
               pageIXLookUp < (INT)SPIFFS_OBJ_LOOKUP_PAGES(pfs))
        {
            iEntryOffset = pageIXLookUp * iEntriesPerPage;
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                   uiBlkIXCurAddr + SPIFFS_PAGE_TO_PADDR(pfs, pageIXLookUp), 
                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);
            while (iRes == SPIFFS_OK && 
                   iEntryCur < (INT)SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs) &&   /* 最后一个页面 */
                   iEntryCur - iEntryOffset < iEntriesPerPage){              /* 非最后一个页面 */
                objId = pObjLookUpBuffer[iEntryCur - iEntryOffset];
                if(objId == SPIFFS_OBJ_ID_FREE){
                    iRes = SPIFFS_OK;
                    break;
                }
                else if(objId == SPIFFS_OBJ_ID_DELETED){
                    uiDeletedPagesInBlks++;
                }
                else {
                    uiUsedPagesInBlks++;
                }
                iEntryCur++;
            }
            pageIXLookUp++;
        } /* 结束查看每个Look Up Page */
        
        /* 计算分数，并且插入到候选表中 */
        if(iRes == SPIFFS_OK){
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                   SPIFFS_ERASE_COUNT_PADDR(pfs, blkIXCur), 
                                   sizeof(SPIFFS_OBJ_ID), (PUCHAR)&objIdEraseCount);
            SPIFFS_CHECK_RES(iRes);
            if(pfs->uiMaxEraseCount > objIdEraseCount){
                objIdEraseAge = pfs->uiMaxEraseCount - objIdEraseCount;
            }
            else {
                objIdEraseAge = SPIFFS_OBJ_ID_FREE - (objIdEraseCount - pfs->uiMaxEraseCount);
            }

            iScore = uiDeletedPagesInBlks * SPIFFS_GC_HEUR_W_DELET +
                     uiUsedPagesInBlks    * SPIFFS_GC_HEUR_W_USED +
                     objIdEraseAge        * (bIsFsCrammed ? 0 : SPIFFS_GC_HEUR_W_ERASE_AGE); 
            iCandidateIX = 0;
            SPIFFS_GC_DBG("gc_check: blkIX:"_SPIPRIbl" del:"_SPIPRIi" use:"_SPIPRIi" score:"_SPIPRIi"\n", blkIXCur, uiDeletedPagesInBlks, uiUsedPagesInBlks, iScore);
            /* 插入排序？ */
            while (iCandidateIX < iMaxCandidates)
            {
                if(pCandidatesBlkIX[iCandidateIX] == (SPIFFS_BLOCK_IX)-1){
                    pCandidatesBlkIX[iCandidateIX] = blkIXCur;
                    pCandidatesScores[iCandidateIX] = iScore;
                    break;
                }
                else if(pCandidatesScores[iCandidateIX] < iScore){
                    iReorderCandidateIX = iMaxCandidates - 2;
                    while (iReorderCandidateIX >= iCandidateIX)
                    {
                        pCandidatesBlkIX[iReorderCandidateIX + 1] = pCandidatesBlkIX[iReorderCandidateIX];
                        pCandidatesScores[iReorderCandidateIX + 1] = pCandidatesScores[iReorderCandidateIX]; 
                        iReorderCandidateIX--;
                    }
                    pCandidatesBlkIX[iCandidateIX] = blkIXCur;
                    pCandidatesScores[iCandidateIX] = iScore;
                    break;
                }
                iCandidateIX++;
            }
            
            *piCandidateCount++;
        }

        iEntryCur = 0;
        blkIXCur++;
        uiBlkIXCurAddr += SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs);
    }/* 结束遍历每个Blk */
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsGCClean
** 功能描述: 清理块
** 输　入  : pfs          文件头
**           blkIX        回收的块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsGCClean(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIX){
    INT                          iRes = SPIFFS_OK;
    INT                          iEntriesPerPage = (SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(SPIFFS_OBJ_ID));
    // this is the global localizer being pushed and popped
    INT                          iEntryCur = 0;
    SPIFFS_OBJ_ID                *pLookUpWorkBuffer = (SPIFFS_OBJ_ID *)pfs->pucLookupWorkBuffer;
    SPIFFS_GC                    gc; // our stack frame/state
    SPIFFS_PAGE_IX               pageIXCur = 0;
    SPIFFS_PAGE_OBJECT_IX_HEADER *objIXHeader = (SPIFFS_PAGE_OBJECT_IX_HEADER *)pfs->pucWorkBuffer;
    SPIFFS_PAGE_OBJECT_IX        *objIX = (SPIFFS_PAGE_OBJECT_IX*)pfs->pucWorkBuffer;
    
    BOOL                         bIsScan;
         
    INT                          pageIXLookUp;
    INT                          iEntryOffset;
    SPIFFS_OBJ_ID                objId;

    SPIFFS_PAGE_HEADER           pageHeader;
    SPIFFS_PAGE_IX               pageIXNewDataPage;
    SPIFFS_PAGE_IX               pageIXObjIX;

    SPIFFS_GC_DBG("gc_clean: cleaning block "_SPIPRIbl"\n", blkIX);

    memset(&gc, 0, sizeof(SPIFFS_GC));
    gc.state = FIND_OBJ_DATA;

    if (pfs->blkIXFreeCursor == blkIX) {    /* 前去下一个空闲lk */
        // move free cursor to next block, cannot use free pages from the block we want to clean
        pfs->blkIXFreeCursor = (blkIX + 1)%pfs->uiBlkCount;
        pfs->objLookupEntryFreeCursor = 0;
        SPIFFS_GC_DBG("gc_clean: move free cursor to block "_SPIPRIbl"\n", pfs->blkIXFreeCursor);
    }

    while (iRes == SPIFFS_OK && gc.state != FINISHED) {
        SPIFFS_GC_DBG("gc_clean: state = "_SPIPRIi" entry:"_SPIPRIi"\n", gc.state, iEntryCur);
        gc.uiObjIdFound = 0;        // reset (to no found data page)
        // scan through lookup pages
        pageIXLookUp = iEntryCur / iEntriesPerPage;
        bIsScan = 1;
        // check each object lookup page
        /* 遍历每个look up page */
        while (bIsScan && iRes == SPIFFS_OK && 
               pageIXLookUp < (INT)SPIFFS_OBJ_LOOKUP_PAGES(pfs)) {
            iEntryOffset = pageIXLookUp * iEntriesPerPage;
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ,0, 
                                   blkIX * SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs) + SPIFFS_PAGE_TO_PADDR(pfs, pageIXLookUp),
                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);
            // check each object lookup entry
            /* 遍历每个look up page中的entry */
            while (bIsScan && iRes == SPIFFS_OK &&
                   iEntryCur - iEntryOffset < iEntriesPerPage && 
                   iEntryCur < (INT)SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs)) {
                objId = pLookUpWorkBuffer[iEntryCur - iEntryOffset];
                pageIXCur = SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntryCur);
                // act upon object id depending on gc state
                switch (gc.state) {
                case FIND_OBJ_DATA:
                    /* 找到 Data Page */
                    if (objId != SPIFFS_OBJ_ID_DELETED && 
                        objId != SPIFFS_OBJ_ID_FREE &&
                        ((objId & SPIFFS_OBJ_ID_IX_FLAG) == 0)) {
                        // found a data page, stop scanning and handle in switch case below
                        SPIFFS_GC_DBG("gc_clean: FIND_DATA state:"_SPIPRIi" - found obj id "_SPIPRIid"\n", gc.state, objId);
                        gc.uiObjIdFound = 1;
                        gc.objIdCur = objId;
                        gc.pageIXDataCur = pageIXCur;
                        bIsScan = 0;
                    }
                    break;
                case MOVE_OBJ_DATA:
                    /* GC OBJ DATA页面 */
                    // evacuate found data pages for corresponding object index we have in memory,
                    // update memory representation
                    if (objId == gc.objIdCur) {
                        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                               SPIFFS_PAGE_TO_PADDR(pfs, pageIXCur), 
                                               sizeof(SPIFFS_PAGE_HEADER), (UINT8 *)&pageHeader);
                        SPIFFS_CHECK_RES(iRes);
                        SPIFFS_GC_DBG("gc_clean: MOVE_DATA found data page "_SPIPRIid":"_SPIPRIsp" @ "_SPIPRIpg"\n", gc.objIdCur, pageHeader.spanIX, pageIXCur);
                        /* 数据页面找到的Index页面的span IX与当前回收的Span IX不等 */
                        if (SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, pageHeader.spanIX) != gc.spanIXObjIXCur) {
                            SPIFFS_GC_DBG("gc_clean: MOVE_DATA no objix spix match, take in another run\n");
                        } 
                        else {
                            if (pageHeader.flags & SPIFFS_PH_FLAG_DELET) {  /* 页面有效 */
                                // move page
                                iRes = spiffsPageMove(pfs, 0, LW_NULL, objId, &pageHeader, 
                                                        pageIXCur, &pageIXNewDataPage);
                                SPIFFS_GC_DBG("gc_clean: MOVE_DATA move objix "_SPIPRIid":"_SPIPRIsp" page "_SPIPRIpg" to "_SPIPRIpg"\n", 
                                              gc.objIdCur, pageHeader.spanIX, pageIXCur, pageIXNewDataPage);
                                SPIFFS_CHECK_RES(iRes);
                                // move wipes obj_lu, reload it
                                /* 重载lookUp Work Buffer，因为lookup entry被move改变了 */
                                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                                       blkIX * SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs) + SPIFFS_PAGE_TO_PADDR(pfs, pageIXLookUp),
                                                       SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);
                                SPIFFS_CHECK_RES(iRes);
                            } 
                            else {                  /* 在lookup中找到了一个已经被删除的页面，说明，Data被删，但是lookup没有删 */
                                // page is deleted but not deleted in lookup, scrap it -
                                // might seem unnecessary as we will erase this block, but
                                // we might get aborted
                                SPIFFS_GC_DBG("gc_clean: MOVE_DATA wipe objix "_SPIPRIid":"_SPIPRIsp" page "_SPIPRIpg"\n", objId, pageHeader.spanIX, pageIXCur);
                                iRes = __spiffsPageDelete(pfs, pageIXCur);
                                SPIFFS_CHECK_RES(iRes);
                                pageIXNewDataPage = SPIFFS_OBJ_ID_FREE;
                            }
                            // update memory representation of object index page with new data page
                            /* 更新在内存中的lookup */
                            //TODO: 当pageHeader.flags & SPIFFS_PH_FLAG_DELET时应该已经不用搞这一手了
                            if (gc.spanIXObjIXCur == 0) {
                                // update object index header page
                                /* 
                                    index page header
                                    common header xx
                                    [span = 0] [span = 1] [span = 2] [span = 3]
                                 */
                                /* 更新Object Index Header页面 */
                                ((SPIFFS_PAGE_IX*)((PUCHAR)objIXHeader + sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)))[pageHeader.spanIX] = pageIXNewDataPage;
                                SPIFFS_GC_DBG("gc_clean: MOVE_DATA wrote page "_SPIPRIpg" to objix_hdr entry "_SPIPRIsp" in mem\n", pageIXNewDataPage, 
                                              (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, pageHeader.spanIX));
                            } else {
                                // update object index page
                                /* 更新Object Index页面 */
                                ((SPIFFS_PAGE_IX*)((PUCHAR)objIX + sizeof(SPIFFS_PAGE_OBJECT_IX)))[SPIFFS_OBJ_IX_ENTRY(pfs, pageHeader.spanIX)] = pageIXNewDataPage;
                                SPIFFS_GC_DBG("gc_clean: MOVE_DATA wrote page "_SPIPRIpg" to objix entry "_SPIPRIsp" in mem\n", pageIXNewDataPage, 
                                              (SPIFFS_SPAN_IX)SPIFFS_OBJ_IX_ENTRY(pfs, pageHeader.spanIX));
                            }
                        }
                    }
                    break;
                case MOVE_OBJ_IX:
                    /* GC OBJ ID页面 */
                // find and evacuate object index pages
                    if (objId != SPIFFS_OBJ_ID_DELETED && 
                        objId != SPIFFS_OBJ_ID_FREE &&
                        (objId & SPIFFS_OBJ_ID_IX_FLAG)) {
                        // found an index object id
                        // load header
                        iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                               SPIFFS_PAGE_TO_PADDR(pfs, pageIXCur), sizeof(SPIFFS_PAGE_HEADER), 
                                               (PUCHAR)&pageHeader);
                        SPIFFS_CHECK_RES(iRes);
                        if (pageHeader.flags & SPIFFS_PH_FLAG_DELET) {  /* 页面有效 */
                            // move page
                            iRes = spiffsPageMove(pfs, 0, 0, objId, &pageHeader, pageIXCur, &pageIXNewDataPage);
                            SPIFFS_GC_DBG("gc_clean: MOVE_OBJIX move objix "_SPIPRIid":"_SPIPRIsp" page "_SPIPRIpg" to "_SPIPRIpg"\n", 
                                          objId, pageHeader.spanIX, pageIXCur, pageIXNewDataPage);
                            SPIFFS_CHECK_RES(iRes);
                            
                            spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)&pageHeader,
                                                SPIFFS_EV_IX_MOV, objId, pageHeader.spanIX, pageIXNewDataPage, 0);
                            
                            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                                   blkIX * SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs) + SPIFFS_PAGE_TO_PADDR(pfs, pageIXLookUp),
                                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucLookupWorkBuffer);
                            SPIFFS_CHECK_RES(iRes);
                        } 
                        else {
                            // page is deleted but not deleted in lookup, scrap it -
                            // might seem unnecessary as we will erase this block, but
                            // we might get aborted
                            SPIFFS_GC_DBG("gc_clean: MOVE_OBJIX wipe objix "_SPIPRIid":"_SPIPRIsp" page "_SPIPRIpg"\n", 
                                          objId, pageHeader.spanIX, pageIXCur);
                            iRes = __spiffsPageDelete(pfs, pageIXCur);
                            if (iRes == SPIFFS_OK) {
                                spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)0,
                                                    SPIFFS_EV_IX_DEL, objId, pageHeader.spanIX, pageIXCur, 0);
                            }
                        }
                        SPIFFS_CHECK_RES(iRes);
                    }
                    break;
                default:
                    bIsScan = 0;
                    break;
                } // switch gc state
                iEntryCur++;
            } // per entry
            pageIXLookUp++; // no need to check scan variable here, iObjLookUpPage is set in start of loop
        } // per object lookup page
        if (iRes != SPIFFS_OK) 
            break;

        // state finalization and switch
        switch (gc.state) {
        case FIND_OBJ_DATA:
            if (gc.uiObjIdFound) {
                // handle found data page -
                // find out corresponding obj ix page and load it to memory
                /* 处理数据页面 */
                gc.iStoredScanEntryIndex = iEntryCur; // push cursor
                iEntryCur = 0; // restart scan from start
                gc.state = MOVE_OBJ_DATA;
                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                       SPIFFS_PAGE_TO_PADDR(pfs, gc.pageIXDataCur), sizeof(SPIFFS_PAGE_HEADER), (PUCHAR)&pageHeader);
                SPIFFS_CHECK_RES(iRes);
                gc.spanIXObjIXCur = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, pageHeader.spanIX);
                SPIFFS_GC_DBG("gc_clean: FIND_DATA find objix span_ix:"_SPIPRIsp"\n", gc.spanIXObjIXCur);
                iRes = spiffsObjLookUpFindIdAndSpan(pfs, gc.objIdCur | SPIFFS_OBJ_ID_IX_FLAG, gc.spanIXObjIXCur, 0, &pageIXObjIX);
                if (iRes == SPIFFS_ERR_NOT_FOUND) {
                    // on borked systems we might get an ERR_NOT_FOUND here -
                    // this is handled by simply deleting the page as it is not referenced
                    // from anywhere
                    SPIFFS_GC_DBG("gc_clean: FIND_OBJ_DATA objix not found! Wipe page "_SPIPRIpg"\n", gc.pageIXDataCur);
                    iRes = __spiffsPageDelete(pfs, gc.pageIXDataCur);
                    SPIFFS_CHECK_RES(iRes);
                    // then we restore states and continue scanning for data pages
                    iEntryCur = gc.iStoredScanEntryIndex; // pop cursor
                    gc.state = FIND_OBJ_DATA;
                    break; // done
                }
                SPIFFS_CHECK_RES(iRes);
                SPIFFS_GC_DBG("gc_clean: FIND_DATA found object index at page "_SPIPRIpg"\n", pageIXObjIX);
                iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_READ, 0, 
                                       SPIFFS_PAGE_TO_PADDR(pfs, pageIXObjIX), SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
                SPIFFS_CHECK_RES(iRes);
                // cannot allow a gc if the presumed index in fact is no index, a
                // check must run or lot of data may be lost
                SPIFFS_VALIDATE_OBJIX(objIX->pageHdr, gc.objIdCur | SPIFFS_OBJ_ID_IX_FLAG, gc.spanIXObjIXCur);
                gc.pageIXObjIXCur = pageIXObjIX;
            } 
            else {
                // no more data pages found, passed thru all block, start evacuating object indices
                gc.state = MOVE_OBJ_IX;
                iEntryCur = 0; // restart entry scan index
            }
            break;
        case MOVE_OBJ_DATA: {
            // store modified objix (hdr) page residing in memory now that all
            // data pages belonging to this object index and residing in the block
            // we want to evacuate
            SPIFFS_PAGE_IX new_objix_pix;
            gc.state = FIND_OBJ_DATA;
            iEntryCur = gc.iStoredScanEntryIndex; // pop cursor
            if (gc.spanIXObjIXCur == 0) {
                // store object index header page
                iRes = spiffsObjectUpdateIndexHdr(pfs, 0, gc.objIdCur | SPIFFS_OBJ_ID_IX_FLAG, 
                                                  gc.pageIXObjIXCur, pfs->pucWorkBuffer, LW_NULL, 
                                                  LW_NULL, &pageIXObjIX);
                SPIFFS_GC_DBG("gc_clean: MOVE_DATA store modified objix_hdr page, "_SPIPRIpg":"_SPIPRIsp"\n"
                              , pageIXObjIX, 0);
                SPIFFS_CHECK_RES(iRes);
            } else {
                // store object index page
                iRes = spiffsPageMove(pfs, 0, pfs->pucWorkBuffer, gc.objIdCur | SPIFFS_OBJ_ID_IX_FLAG, 
                                      LW_NULL, gc.pageIXObjIXCur, &pageIXObjIX);
                SPIFFS_GC_DBG("gc_clean: MOVE_DATA store modified objix page, "_SPIPRIpg":"_SPIPRIsp"\n"
                              , pageIXObjIX, objIX->pageHdr.spanIX);
                SPIFFS_CHECK_RES(iRes);
                spiffsCBObjectEvent(pfs, (SPIFFS_PAGE_OBJECT_IX *)pfs->pucWorkBuffer,
                                    SPIFFS_EV_IX_UPD, gc.objIdCur, objIX->pageHdr.spanIX, 
                                    pageIXObjIX, 0);
            }
        }
            break;
        case MOVE_OBJ_IX:
            // scanned thru all block, no more object indices found - our work here is done
            gc.state = FINISHED;
            break;
        default:
            iEntryCur = 0;
            break;
        } // switch gc.state
        SPIFFS_GC_DBG("gc_clean: state-> "_SPIPRIi"\n", gc.state);
    } // while state != FINISHED


    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsGCCheck
** 功能描述: 检查是否需要GC，如果需要，则进行GC
** 输　入  : pfs          文件头
**           uiLen        申请创建长度
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsGCCheck(PSPIFFS_VOLUME pfs, UINT32 uiLen){
    INT32  iFreePages;
    UINT32 uiNeededPages;
    INT32  iRes   = SPIFFS_OK;
    INT    iTries = 0;
    SPIFFS_BLOCK_IX blkIXCandidate;
    SPIFFS_BLOCK_IX* pBlkIXCandidates;
    UINT   uiCount;
    INT32  iPreFreePages;

    iFreePages = (SPIFFS_PAGES_PER_BLOCK(pfs) - SPIFFS_OBJ_LOOKUP_PAGES(pfs)) * (pfs->uiBlkCount - 2)   /* 预留2个Blk？？？ */
                 - pfs->uiStatsPageAllocated - pfs->uiStatsPageDeleted;

    /* 不需要GC */
    if(pfs->uiFreeBlks > 3 && 
       (INT32)uiLen < iFreePages * (INT32)SPIFFS_DATA_PAGE_SIZE(pfs)){
        return SPIFFS_OK;
    }

    uiNeededPages = (uiLen + SPIFFS_DATA_PAGE_SIZE(pfs) - 1) / SPIFFS_DATA_PAGE_SIZE(pfs);
    if((INT32)uiNeededPages > (INT32)(iFreePages + pfs->uiStatsPageDeleted)){
        SPIFFS_GC_DBG("gc_check: full freeblk:"_SPIPRIi" needed:"_SPIPRIi" free:"_SPIPRIi" dele:"_SPIPRIi"\n", pfs->uiFreeBlks, uiNeededPages, iFreePages, pfs->uiStatsPageDeleted);
        return SPIFFS_ERR_FULL;
    }

    do {
        SPIFFS_GC_DBG("\ngc_check #"_SPIPRIi": run gc free_blocks:"_SPIPRIi" pfree:"_SPIPRIi" pallo:"_SPIPRIi" pdele:"_SPIPRIi" ["_SPIPRIi"] len:"_SPIPRIi" of "_SPIPRIi"\n",
                      iTries,
                      pfs->uiFreeBlks, iFreePages, pfs->uiStatsPageAllocated, pfs->uiStatsPageDeleted, (iFreePages + pfs->uiStatsPageAllocated + pfs->uiStatsPageDeleted),
                      uiLen, (UINT32)(iFreePages * SPIFFS_DATA_PAGE_SIZE(pfs)));
        iPreFreePages = iFreePages;
        iRes = __spiffsGCFindCandiate(pfs, &pBlkIXCandidates, &uiCount, iFreePages <= 0);
        SPIFFS_CHECK_RES(iRes);

        if(uiCount == 0){
            SPIFFS_GC_DBG("gc_check: no candidates, return\n");
            return (INT32)uiNeededPages < iFreePages ? SPIFFS_OK : SPIFFS_ERR_FULL;
        }

        pfs->uiStatsGCRuns++;

        blkIXCandidate = pBlkIXCandidates[0];
        /* GC已经即将操作了 */
        pfs->uiCleaningFlag = 1;
        iRes = spiffsGCClean(pfs, blkIXCandidate);
        pfs->uiCleaningFlag = 0;


    } while (++iTries < SPIFFS_GC_MAX_RUNS && (pfs->uiFreeBlks <= 2 ||
             (INT32)uiLen > iFreePages * (INT32)SPIFFS_DATA_PAGE_SIZE(pfs)));
             
    iFreePages = (SPIFFS_PAGES_PER_BLOCK(pfs) - SPIFFS_OBJ_LOOKUP_PAGES(pfs)) * (pfs->uiBlkCount - 2)
                  - pfs->uiStatsPageAllocated - pfs->uiStatsPageDeleted;
    if ((INT32)uiLen > iFreePages *(INT32)SPIFFS_DATA_PAGE_SIZE(pfs)) {
        iRes = SPIFFS_ERR_FULL;
    }

    SPIFFS_GC_DBG("gc_check: finished, "_SPIPRIi" dirty, uiBlkCount "_SPIPRIi" free, "_SPIPRIi" pages free, "_SPIPRIi" tries, iRes "_SPIPRIi"\n",
                   pfs->uiStatsPageAllocated + pfs->uiStatsPageDeleted,
                   pfs->uiFreeBlks, iFreePages, iTries, iRes);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsGCQuick
** 功能描述: 快速GC，这个函数用于干嘛呢？
** 输　入  : pfs          文件头
**           uiLen        申请创建长度
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsGCQuick(PSPIFFS_VOLUME pfs, UINT16 uiMaxFreePages){
    INT32           iRes = SPIFFS_OK;
    UINT32          uiBlkCount = pfs->uiBlkCount;
    SPIFFS_BLOCK_IX blkIXCur = 0;
    UINT32          uiBlkCurAddr = 0;
    INT             iEntryCur = 0;
    SPIFFS_OBJ_ID   *objLookUpBuffer = (SPIFFS_OBJ_ID *)pfs->pucLookupWorkBuffer;

    SPIFFS_GC_DBG("gc_quick: running\n");
    pfs->uiStatsGCRuns++;

    INT iEntriesPerPage = (SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) / sizeof(SPIFFS_OBJ_ID));

    // find fully deleted uiBlkCount
    // check each block
    while (iRes == SPIFFS_OK && uiBlkCount--) {
        UINT16 uiDeletedPagesInBlk = 0;
        UINT16 uiFreePagesInBlk = 0;
        INT    iObjLookUpPage = 0;
        // check each object lookup page
        while (iRes == SPIFFS_OK && iObjLookUpPage < (INT)SPIFFS_OBJ_LOOKUP_PAGES(pfs)) {
            INT iEntryOffset = iObjLookUpPage * iEntriesPerPage;
            iRes = spiffsCacheRead(pfs, SPIFFS_OP_T_OBJ_LU | SPIFFS_OP_C_READ, 0, 
                                   uiBlkCurAddr + SPIFFS_PAGE_TO_PADDR(pfs, iObjLookUpPage), 
                                   SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), pfs->pucWorkBuffer);
            // check each entry
            while (iRes == SPIFFS_OK &&
                iEntryCur - iEntryOffset < iEntriesPerPage &&
                iEntryCur < (INT)(SPIFFS_PAGES_PER_BLOCK(pfs)-SPIFFS_OBJ_LOOKUP_PAGES(pfs))) {
                SPIFFS_OBJ_ID objId = objLookUpBuffer[iEntryCur - iEntryOffset];
                if (objId == SPIFFS_OBJ_ID_DELETED) {
                    uiDeletedPagesInBlk++;
                } 
                else if (objId == SPIFFS_OBJ_ID_FREE) {
                    // kill scan, go for next block
                    uiFreePagesInBlk++;
                    if (uiFreePagesInBlk > uiMaxFreePages) {
                        iObjLookUpPage = SPIFFS_OBJ_LOOKUP_PAGES(pfs);
                        iRes = 1; // kill object lu loop
                        break;
                    }
                }  
                else {
                    // kill scan, go for next block
                    iObjLookUpPage = SPIFFS_OBJ_LOOKUP_PAGES(pfs);
                    iRes = 1; // kill object lu loop
                    break;
                }
                iEntryCur++;
            } // per entry
            iObjLookUpPage++;
        } // per object lookup page
        if (iRes == 1) 
            iRes = SPIFFS_OK;

        if (iRes == SPIFFS_OK &&
            uiDeletedPagesInBlk + uiFreePagesInBlk == SPIFFS_PAGES_PER_BLOCK(pfs) - SPIFFS_OBJ_LOOKUP_PAGES(pfs) &&
            uiFreePagesInBlk <= uiMaxFreePages) {
            // found a fully deleted block
            pfs->uiStatsPageDeleted -= uiDeletedPagesInBlk;
            iRes = __spiffsGCEraseBlk(pfs, blkIXCur);
            return iRes;
        }

        iEntryCur = 0;
        blkIXCur++;
        uiBlkCurAddr += SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs);
    } // per block

    if (iRes == SPIFFS_OK) {
        iRes = SPIFFS_ERR_NO_DELETED_BLOCKS;
    }
    return iRes;
}
