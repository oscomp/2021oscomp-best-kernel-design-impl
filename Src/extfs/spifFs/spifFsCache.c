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
** 文   件   名: spifFsCache.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统缓存层
*********************************************************************************************************/
#include "spifFsCache.h"
#include "spifFsFDLib.h"
#include "spifFsLib.h"
#include "../SylixOS/driver/mtd/nor/nor.h"

#define WRITE_BACK 1
#define NO_WRITE_BACK 0

#define SPIFFS_CHECK_CACHE_EXIST(pCache)            !((pCache->uiCpageUseMap & pCache->uiCpageUseMask) \
                                                    == 0)                                             /* 检查Cache是否存有页面 */       
#define SPIFFS_CHECK_CACHE_FREE(pCache)             ((pCache->uiCpageUseMap & pCache->uiCpageUseMask) \
                                                    != pCache->uiCpageUseMask)                         /* 检查Cache是否有空余页面 */
#define SPIFFS_CHECK_CACHE_PAGE_VALID(pCache, i)    ((pCache->uiCpageUseMap & (1 << i)) == (1 << i))
#define SPIFFS_MAP_USE_CACHE_PAGE(pCache, ix)       pCache->uiCpageUseMap |= (1 << ix);
#define SPIFFS_MAP_FREE_CACHE_PAGE(pCache, ix)      pCache->uiCpageUseMap &= ~(1 << ix);
/*********************************************************************************************************
** 函数名称: __spiffsCachePageFree
** 功能描述: 释放Cache中uiIX位置的Cache Page
** 输　入  : pfs          文件头
**          uiIX          待检查Cache中的页面编号
**          bIsWriteBack  是否写回Flash
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsCachePageFree(PSPIFFS_VOLUME pfs, UINT uiIX, BOOL bIsWriteBack){
    INT32 iRes = SPIFFS_OK;
    PSPIFFS_CACHE pCache            = SPIFFS_GET_CACHE_HDR(pfs);;
    PSPIFFS_CACHE_PAGE pCachePage   = SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, uiIX);
    PCHAR pPageContent;
    if(SPIFFS_CHECK_CACHE_PAGE_VALID(pCache, uiIX)){                    /* 该页面是有效的 */
        //TODO: SPIFFS_CACHE_FLAG_TYPE_WR??
        if(bIsWriteBack &&                                               
           (pCachePage->flags & SPIFFS_CACHE_FLAG_TYPE_WR) == 0 &&      /*  页面配置是可写的?? */
           (pCachePage->flags & SPIFFS_CACHE_FLAG_DIRTY)){              /* 页面被写脏（被写过了） */
            pPageContent = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, uiIX);
            SPIFFS_CACHE_DBG("CACHE_FREE: write cache page "_SPIPRIi" pageIX "_SPIPRIpg"\n", uiIX, pCachePage->pageIX);
            iRes = write_nor(SPIFFS_PAGE_TO_PADDR(pfs, pCachePage->pageIX), pPageContent, 
                             SPIFFS_CFG_LOGIC_PAGE_SZ(pfs), WRITE_KEEP);
        }

        if(pCachePage->flags & SPIFFS_CACHE_FLAG_TYPE_WR){
            SPIFFS_CACHE_DBG("CACHE_FREE: free cache page "_SPIPRIi" objid "_SPIPRIid"\n", uiIX, pCachePage->objId);
        }
        else {
            SPIFFS_CACHE_DBG("CACHE_FREE: free cache page "_SPIPRIi" pageIX "_SPIPRIpg"\n", uiIX, pCachePage->pageIX);
        }
        SPIFFS_MAP_FREE_CACHE_PAGE(pCache, uiIX);
        pCachePage->flags = 0;
    }

    return iRes;
}

/*********************************************************************************************************
** 函数名称: __spiffsCachePageRemoveOldest
** 功能描述: 移除Cache中最古老的一个页面
** 输　入  : pfs          文件头
**          uiFlagMask     标志位遮罩
**          uiFlags        标志位
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffsCachePageRemoveOldest(PSPIFFS_VOLUME pfs, UINT8 uiFlagMask, UINT8 uiFlags){
    /* Flag Mask : 1 << 7 */
    /* Flag      : 0 */
    INT32 iRes = SPIFFS_OK;
    PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
    PSPIFFS_CACHE_PAGE pCachePage = LW_NULL;

    INT i;
    INT iCandidateIX    = -1;
    
    UINT32 uiAge;
    UINT32 uiOldestVal  = 0;

    BOOL bIsCacheHasFree = SPIFFS_CHECK_CACHE_FREE(pCache);
    if(bIsCacheHasFree){
        return iRes;
    }
    /* Cache中页面占满了 */
    for (i = 0; i < pCache->uiCpageCount; i++)
    {
        pCachePage = SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, i);
        uiAge = pCache->uiLastAccess - pCachePage->uiLastAccess;
        if(uiAge > uiOldestVal && 
           ((pCachePage->flags & uiFlagMask) == uiFlags)){
            uiOldestVal     = uiAge;
            iCandidateIX    = i;
        }
    }

    if(iCandidateIX >= 0){
        iRes = __spiffsCachePageFree(pfs, iCandidateIX, WRITE_BACK);
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffsCachePageHit
** 功能描述: 检查页面pageIX是否在Cache中命中，如果命中，则返回页面
** 输　入  : pfs          文件头
**          pageIX         待检查页面
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_CACHE_PAGE __spiffsCachePageHit(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX){
    PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
    INT i;
    PSPIFFS_CACHE_PAGE pCachePage;
    if(!SPIFFS_CHECK_CACHE_EXIST(pCache)){     /* 检查Cache中是否有可用页面 */
        return LW_NULL;
    }    
    for (i = 0; i < pCache->uiCpageCount; i++)
    {
        pCachePage = SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, i);
        //TODO：SPIFFS_CACHE_FLAG_TYPE_WR代表着什么？ 非LOOKUP PAGE ？
        if(SPIFFS_CHECK_CACHE_PAGE_VALID(pCache, i) && 
           (pCachePage->flags & SPIFFS_CACHE_FLAG_TYPE_WR) == 0 &&      
           pCachePage->pageIX == pageIX){
            SPIFFS_CACHE_DBG("CACHE_HIT: hit cache page " _SPIPRIi  " for " _SPIPRIpg "\n", i, pageIX);
            pCachePage->uiLastAccess = pCache->uiLastAccess;
            return pCachePage;
        }
    }
    return LW_NULL;
}
/*********************************************************************************************************
** 函数名称: __spiffsCachePageAllocate
** 功能描述: Cache分配一个页面
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_CACHE_PAGE __spiffsCachePageAllocate(PSPIFFS_VOLUME pfs) {
    PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
    PSPIFFS_CACHE_PAGE pCachePage;
    INT i;
    if (!SPIFFS_CHECK_CACHE_FREE(pCache)) {   /* 没有空余页面了 */
        return LW_NULL;
    }
    for (i = 0; i < pCache->uiCpageCount ; i++) {
        if (!SPIFFS_CHECK_CACHE_PAGE_VALID(pCache, i)) {
            pCachePage = SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, i);
            SPIFFS_MAP_USE_CACHE_PAGE(pCache, i);
            pCachePage->uiLastAccess = pCache->uiLastAccess;
            return pCachePage;
        }
    }
    return LW_NULL;
}
/*********************************************************************************************************
** 函数名称: spiffsCacheRead
** 功能描述: 读Cache，未命中就替换或者读入
** 输　入  : pfs          文件头
**          pageIX         待检查页面
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsCacheRead(PSPIFFS_VOLUME pfs, UINT8 uiOps, SPIFFS_FILE fileHandler, 
                     UINT32 uiAddr, UINT32 uiLen, PCHAR pDst){
    (VOID)fileHandler;      /* 不使用 */
    PSPIFFS_CACHE       pCache = SPIFFS_GET_CACHE_HDR(pfs);
    PSPIFFS_CACHE_PAGE  pCachePage = __spiffsCachePageHit(pfs, SPIFFS_PADDR_TO_PAGE(pfs, uiAddr));
    PCHAR               pPageContent;
    INT32               iRes = SPIFFS_OK;
    INT32               iRes2;
    if(uiLen > SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)){      /* 不允许读超过逻辑页面大小 */
        return SPIFFS_ERR_CACHE_OVER_RD;
    }

    pCache->uiLastAccess++;

    if(pCachePage){         /* 命中 */
        pfs->uiCacheHits++;
        pCachePage->uiLastAccess = pCache->uiLastAccess;
        pPageContent = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, pCachePage->uiIX);
        lib_memcpy(pDst, pPageContent + SPIFFS_PADDR_TO_PAGE_OFFSET(pfs, uiAddr), uiLen);
    }
    else {                  /* 未命中 */
        if((uiOps & SPIFFS_OP_TYPE_MASK) == SPIFFS_OP_T_OBJ_LU2){   /* 不Cache */
            iRes = read_nor(uiAddr, pDst, uiLen);
            return iRes;
        }
        pfs->uiCacheMisses++;
        iRes = __spiffsCachePageRemoveOldest(pfs, SPIFFS_CACHE_FLAG_TYPE_WR, 0);
        pCachePage = __spiffsCachePageAllocate(pfs);
        if(pCachePage){
            //TODO: 为啥是WRTHRU
            pCachePage->flags   = SPIFFS_CACHE_FLAG_WRTHRU;
            pCachePage->pageIX  = SPIFFS_PADDR_TO_PAGE(pfs, uiAddr);
            SPIFFS_CACHE_DBG("CACHE_ALLO: allocated cache page "_SPIPRIi" for pageIX "_SPIPRIpg "\n", pCachePage->uiIX, pCachePage->pageIX);
            iRes2 = read_nor(uiAddr - SPIFFS_PADDR_TO_PAGE_OFFSET(pfs, uiAddr), 
                             SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, pCachePage->uiIX), 
                             SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
            if(iRes2 != SPIFFS_OK){                 /* !!读错误先于写错误 */
                iRes = iRes2;           
            }
            pPageContent = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, pCachePage->uiIX);
            lib_memcpy(pDst, pPageContent + SPIFFS_PADDR_TO_PAGE_OFFSET(pfs, uiAddr), uiLen);
        }
        else {
            iRes2 = read_nor(uiAddr, pDst, uiLen);  /* 直接读 */
            if(iRes2 != SPIFFS_OK){                 /* !!读错误先于写错误 */
                iRes = iRes2;           
            }
        }
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsCacheInit
** 功能描述: 初始化pfs->pCache段结构，结构如下：
----------------------------------------------------------------------------------------------------------
                |                   |                  |                    |              |   ...
SPIFFS_Cache    |   SPIFFS_Page_HDR |                  |    SPIFFS_Page_HDR |              |   ...
                |                   |                  |                    |              |   ...
----------------------------------------------------------------------------------------------------------
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsCacheInit(PSPIFFS_VOLUME pfs){
    UINT32 uiSize;
    UINT32 uiCacheMask;
    INT i;
    INT iCacheEntries;
    SPIFFS_CACHE spiffsCache;
    PSPIFFS_CACHE pCache;

    if(pfs->pCache == LW_NULL){
        return SPIFFS_ERR_CACHE_NO_INIT;
    }
    uiSize          = pfs->uiCacheSize;
    uiCacheMask     = 0;
    iCacheEntries   = (uiSize - sizeof(SPIFFS_CACHE)) / SPIFFS_CACHE_PAGE_SIZE(pfs);    /* 减去缓存头部长度 */
    
    if(iCacheEntries <= 0){
        return SPIFFS_ERR_CACHE_NO_MEM;
    }
    /* 记录哪些位可用 */
    for (i = 0; i < iCacheEntries; i++)
    {
        uiCacheMask <<= 1;
        uiCacheMask |= 1;
    }

    lib_memset(&spiffsCache, 0, sizeof(SPIFFS_CACHE));
    spiffsCache.uiCpageCount = iCacheEntries;
    spiffsCache.Cpages = (PCHAR)((PCHAR)pfs->pCache + sizeof(SPIFFS_CACHE));
    spiffsCache.uiCpageUseMap   = (UINT32)-1;
    spiffsCache.uiCpageUseMask  = uiCacheMask;

    lib_memcpy(pfs->pCache, &spiffsCache, sizeof(SPIFFS_CACHE));
    
    
    pCache = SPIFFS_GET_CACHE_HDR(pfs);
    lib_memset(pCache->Cpages, 0 ,
               pCache->uiCpageCount * SPIFFS_GET_CACHE_PAGE_SIZE(pfs));
    
    pCache->uiCpageUseMap &= ~(pCache->uiCpageUseMask);         /* 111/000000 */
    for (i = 0; i < pCache->uiCpageCount; i++)
    {
        SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, i)->uiIX = i;
    }
    return SPIFFS_OK;
}
/*********************************************************************************************************
** 函数名称: spiffsCacheWrite
** 功能描述: 写Cache，如果WriteThru就不仅要写Cache还要写介质，采用Write Not Alloc方法
** 输　入  : pfs          文件头
**          blkIX         逻辑块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsCacheWrite(PSPIFFS_VOLUME pfs, UINT8 uiOps, SPIFFS_FILE fileHandler, 
                       UINT32 uiAddr, UINT32 uiLen, PCHAR pSrc){
    (VOID)fileHandler;
    SPIFFS_PAGE_IX pageIX = SPIFFS_PADDR_TO_PAGE(pfs, uiAddr);
    PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
    PSPIFFS_CACHE_PAGE pCachePage = __spiffsCachePageHit(pfs, pageIX);
    PCHAR pPageContent;
    if(pCachePage && ((uiOps & SPIFFS_OP_COM_MASK) != SPIFFS_OP_C_WRTHRU)){
        if((uiOps & SPIFFS_OP_COM_MASK) == SPIFFS_OP_C_DELE &&          /* 将要被删除 */
           (uiOps & SPIFFS_OP_TYPE_MASK) != SPIFFS_OP_T_OBJ_LU){        /* 且不是Look Up Page */
            __spiffsCachePageFree(pfs, pCachePage->uiIX, 0);
            return write_nor(uiAddr, pSrc, uiLen, WRITE_KEEP);
        }
        pCache->uiLastAccess++;
        pCachePage->uiLastAccess = pCache->uiLastAccess;

        pPageContent = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, pCachePage->uiIX);
        lib_memcpy(pPageContent + SPIFFS_PADDR_TO_PAGE_OFFSET(pfs, uiAddr), pSrc, uiLen);

        if(pCachePage->flags & SPIFFS_CACHE_FLAG_WRTHRU){
            return write_nor(uiAddr, pSrc, uiLen, WRITE_KEEP);
        }
        else {
            return SPIFFS_OK;
        }
    }
    else {
        return write_nor(uiAddr, pSrc, uiLen, WRITE_KEEP);
    }
}
/*********************************************************************************************************
** 函数名称: spiffsEraseBlk
** 功能描述: 擦除一个块，因为Cache是与硬件相接，因此，将该函数放在这里
** 输　入  : pfs          文件头
**          blkIX         逻辑块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsEraseBlk(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIX){
    INT32 iRes;
    UINT uiAddr = SPIFFS_BLOCK_TO_PADDR(pfs, blkIX);
    INT  iSize = SPIFFS_CFG_LOGIC_PAGE_SZ(pfs);
    
    UINT uiSectorSize;
    UINT uiSectorNo;

    SPIFFS_OBJ_ID objIdMagic;
    
    while (iSize > 0) {
        SPIFFS_DBG("erase "_SPIPRIad":"_SPIPRIi"\n", uiAddr,  SPIFFS_CFG_PHYS_ERASE_SZ(pfs));
        erase_nor(uiAddr, ERASE_SECTOR);

        uiSectorNo = GET_SECTOR_NO(uiAddr);    
        uiSectorSize = GET_SECTOR_SIZE(uiSectorNo);

        uiAddr += uiSectorSize;
        iSize -= uiSectorSize;
    }
    pfs->uiFreeBlks++;

    /* 擦除后，写入uiMaxEraseCount */
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_WRTHRU, 0, 
                            SPIFFS_ERASE_COUNT_PADDR(pfs, blkIX), sizeof(SPIFFS_OBJ_ID),
                            (PCHAR)&pfs->uiMaxEraseCount);
    SPIFFS_CHECK_RES(iRes);

    /* 擦除后，写入Magic*/
    objIdMagic = SPIFFS_MAGIC(pfs, blkIX);
    iRes = spiffsCacheWrite(pfs, SPIFFS_OP_T_OBJ_LU2 | SPIFFS_OP_C_WRTHRU, 0, 
                            SPIFFS_MAGIC_PADDR(pfs, blkIX), sizeof(SPIFFS_OBJ_ID),
                            (PCHAR)&objIdMagic);

    SPIFFS_CHECK_RES(iRes);

    pfs->uiMaxEraseCount++;
    /* 最高位不得为 1 */
    if(pfs->uiMaxEraseCount == SPIFFS_OBJ_ID_IX_FLAG) {
        pfs->uiMaxEraseCount = 0;
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsCachePageGetByFd
** 功能描述: 返回文件描述符指向的Cache Page，如果没有则返回LW_NULL
** 输　入  : pfs          文件头
**          fileHandler    文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_CACHE_PAGE spiffsCachePageGetByFd(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd){
    PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
    PSPIFFS_CACHE_PAGE pCachePage;
    INT i;
    if(!SPIFFS_CHECK_CACHE_EXIST(pCache)){  /* 所有Cache Page都是空闲的，没有页面可以被分配给ObjId */
        return LW_NULL;
    }
    for (i = 0; i < pCache->uiCpageCount; i++)
    {
        pCachePage = SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, i);
        if(SPIFFS_CHECK_CACHE_PAGE_VALID(pCache, i) &&
           (pCachePage->flags & SPIFFS_CACHE_FLAG_TYPE_WR) &&
            pCachePage->objId == pFd->objId){
            return pCachePage;
        }
    }
    return LW_NULL;
}

/*********************************************************************************************************
** 函数名称: spiffsCacheFdRelease
** 功能描述:  unrefers all fds that this cache page refers to and releases the cache page
** 输　入  : pfs          文件头
**          pCachePage    缓存页
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID spiffsCacheFdRelease(PSPIFFS_VOLUME pfs, PSPIFFS_CACHE_PAGE pCachePage){
    UINT i;
    PSPIFFS_FD  pFds;
    PSPIFFS_FD  pFdCur;
    if(pCachePage == LW_NULL)
        return;
    pFds = (PSPIFFS_FD)pfs->pucFdSpace;
    for (i = 0; i < pfs->uiFdCount; i++) {
        pFdCur = &pFds[i];
        if (pFdCur->fileN != 0 && pFdCur->pCachePage == pCachePage) {
            pFdCur->pCachePage = LW_NULL;
        }
    }
    __spiffsCachePageFree(pfs, pCachePage->uiIX, 0);
    pCachePage->objId = 0;
    return;
}
/*********************************************************************************************************
** 函数名称: spiffsCacheFflush
** 功能描述: 把与文件fileHandler相关的CacheWrite都写回介质
** 输　入  : pfs          文件头
**          fileHandler    文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 spiffsCacheFflush(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler){
    (VOID)pfs;
    (VOID)fileHandler;
    INT32 iRes = SPIFFS_OK;

    PSPIFFS_FD  pFd;
    PCHAR       pPageData; 
    iRes = spiffsFdGet(pfs, fileHandler, &pFd);
    
    SPIFFS_API_CHECK_RES(pfs, iRes);

    if ((pFd->flags & SPIFFS_O_DIRECT) == 0) {
        if (pFd->pCachePage == LW_NULL) {
            // see if object id is associated with cache already
            pFd->pCachePage = spiffsCachePageGetByFd(pfs, pFd);
        }
        if (pFd->pCachePage) {
            SPIFFS_CACHE_DBG("CACHE_WR_DUMP: dumping cache page "_SPIPRIi" for pFd "_SPIPRIfd":"_SPIPRIid", flush, offs:"_SPIPRIi" size:"_SPIPRIi"\n",
                             pFd->pCachePage->uiIX, pFd->fileN,  pFd->objId, pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
            pPageData = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, SPIFFS_GET_CACHE_HDR(pfs), pFd->pCachePage->uiIX);
            iRes = spiffsFileWrite(pfs, fileHandler, pPageData,
                                   pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
            if (iRes < SPIFFS_OK) {
                pfs->uiErrorCode = iRes;
            }
            spiffsCacheFdRelease(pfs, pFd->pCachePage);
        }
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: spiffsCacheDropPage
** 功能描述: 直接扔掉一个页面pageIX
** 输　入  : pfs          文件头
**          fileHandler    文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID spiffsCacheDropPage(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX) {
    PSPIFFS_CACHE_PAGE cachePage =  __spiffsCachePageHit(pfs, pageIX);
    if (cachePage) {
        __spiffsCachePageFree(pfs, cachePage->uiIX, 0);
    }
}
/*********************************************************************************************************
** 函数名称: spiffsCacheDropPage
** 功能描述: 直接扔掉一个页面pageIX
** 输　入  : pfs          文件头
**          fileHandler    文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_CACHE_PAGE spiffsCachePageAllocateByFd(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd) {
    // before this function is called, it is ensured that there is no already existing
    // cache page with same object id
    __spiffsCachePageRemoveOldest(pfs, SPIFFS_CACHE_FLAG_TYPE_WR, 0);
    PSPIFFS_CACHE_PAGE pCachePage = __spiffsCachePageAllocate(pfs);
    if (pCachePage == 0) {
        // could not get cache page
        return 0;
    }

    pCachePage->flags = SPIFFS_CACHE_FLAG_TYPE_WR;
    pCachePage->objId = pFd->objId;
    pFd->pCachePage = pCachePage;
    SPIFFS_CACHE_DBG("CACHE_ALLO: allocated cache page "_SPIPRIi" for pFd "_SPIPRIfd ":"_SPIPRIid "\n", 
                     pCachePage->uiIX, pFd->fileN, pFd->objId);
    return pCachePage;
}
