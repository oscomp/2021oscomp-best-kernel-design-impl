/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SyluiIXOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: spifFsGlue.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 06日
**
** 描        述: Spiffs文件系统胶水层，即上层实现
*********************************************************************************************************/
#include "spifFsGlue.h"
#include "spifFsCache.h"
#include "spifFsLib.h"
#include "spifFsFDLib.h"
#include "../driver/mtd/nor/nor.h"
#include "spifFsGC.h"

#define __spiffsAlign8Byte(pMem, uiMemSZ) \
do {\
    UINT8           uiAddrLSB;      /* 用于指针对齐 */\
    uiAddrLSB = ((UINT8)(intptr_t)pMem) & (sizeof(PVOID) - 1);\
    if(uiAddrLSB){\
        pMem += (sizeof(PVOID) - uiAddrLSB);\
        uiMemSZ -= (sizeof(PVOID) - uiAddrLSB);\
    }\
}while(0)
/*********************************************************************************************************
** 函数名称: __spiffs_format
** 功能描述: 格式化Norflash介质，第一次挂载时需要
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_format(PSPIFFS_VOLUME pfs){
    INT32 iRes;
    SPIFFS_BLOCK_IX blkIX = 0;
    if(pfs->uiMountedFlag != 0){
        pfs->uiErrorCode = SPIFFS_ERR_MOUNTED;
        return -1;
    }
    //LOCK
    while (blkIX < pfs->uiBlkCount)
    {
        pfs->uiMaxEraseCount = 0;
        iRes = spiffsEraseBlk(pfs, blkIX);
        if(iRes != SPIFFS_OK){
            iRes = SPIFFS_ERR_ERASE_FAIL;
        }
        pfs->uiErrorCode = iRes;
        blkIX++;
    }
    //UNLOCK
    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_probe_fs
** 功能描述: 探测Spiffs文件系统
** 输　入  : pConfig        配置文件
** 输　出  : 文件系统大小
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_probe_fs(PSPIFFS_CONFIG pConfig){
    SPIFFS_API_DBG("%stat\n", __func__);
    INT32           iRes = SPIFFS_OK;
    UINT32          uiAddr;
    SPIFFS_VOLUME   dummyFs;
    SPIFFS_OBJ_ID   objIdMagic[3];
    SPIFFS_OBJ_ID   blkIXCount[3];  /* 数量递减，第一个代表All Blk，第二个代表All Blk - 1……  */
    SPIFFS_BLOCK_IX blkIX;

    lib_memcpy(&dummyFs, pConfig, sizeof(SPIFFS_CONFIG));
    dummyFs.uiBlkCount = 0;

    for (blkIX = 0; blkIX < 3; blkIX++)
    {
        uiAddr = SPIFFS_MAGIC_PADDR(&dummyFs, blkIX);
        iRes = read_nor(uiAddr, (PUCHAR)&objIdMagic[blkIX], sizeof(SPIFFS_OBJ_ID));
        blkIXCount[blkIX] = objIdMagic[blkIX] ^ SPIFFS_MAGIC(&dummyFs, 0);
        SPIFFS_CHECK_RES(iRes);
    }
    /* BlkCount - blkIX */
    if (blkIXCount[0] < 3) 
        return SPIFFS_ERR_PROBE_TOO_FEW_BLOCKS;
    // check that the order is correct, take aborted erases in calculation
    // first block aborted erase
    //TODO: objIdMagic[0] == (SPIFFS_OBJ_ID)(-1)为何能够说明aborted erase
    if (objIdMagic[0] == (SPIFFS_OBJ_ID)(-1) && blkIXCount[1] - blkIXCount[2] == 1) {
        return (blkIXCount[1]+1) * pConfig->uiLogicBlkSize;
    }
    // second block aborted erase
    if (objIdMagic[1] == (SPIFFS_OBJ_ID)(-1) && blkIXCount[0] - blkIXCount[2] == 2) {
        return blkIXCount[0] * pConfig->uiLogicBlkSize;
    }
    // third block aborted erase
    if (objIdMagic[2] == (SPIFFS_OBJ_ID)(-1) && blkIXCount[0] - blkIXCount[1] == 1) {
        return blkIXCount[0] * pConfig->uiLogicBlkSize;
    }
    // no block has aborted erase
    if (blkIXCount[0] - blkIXCount[1] == 1 && blkIXCount[1] - blkIXCount[2] == 1) {
        return blkIXCount[0] * pConfig->uiLogicBlkSize;
    }

    return SPIFFS_ERR_PROBE_NOT_A_FS;
}
/*********************************************************************************************************
** 函数名称: __spiffs_mount
** 功能描述: 挂载文件
** 输　入  : pfs          文件头
**          pConfig         配置文件
**          pucWorkBuffer   分配WorkBuffer
**          pucFdSpace      分配的文件描述符区域
**          uiFdSpaceuiSize   分配的文件描述符区域大小
**          pCache          分配的缓存空间
**          uiCacheSize     分配的Cached大小
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_mount(PSPIFFS_VOLUME pfs, PSPIFFS_CONFIG pConfig, PUCHAR pucWorkBuffer,
                     UINT8 *pucFdSpace, UINT32 uiFdSpaceuiSize,
                     PUCHAR pCache, UINT32 uiCacheSize,
                     spiffsCheckCallback checkCallbackFunc){
    //TODO:什么意思？
    PVOID           pUserData = LW_NULL;      
    INT32           iRes = SPIFFS_OK;
    pfs->hVolLock = API_SemaphoreMCreate("spiffs_volume_lock", LW_PRIO_DEF_CEILING,
                                         LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE |
                                         LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
                                         LW_NULL);              /* 创建文件头锁 */
    if (!pfs->hVolLock) {                                       /*  无法创建卷锁                */
        __SHEAP_FREE(pfs);
        return  (PX_ERROR);
    }

    lib_memset(pfs, 0, sizeof(SPIFFS_VOLUME));
    lib_memset(pucFdSpace, 0, uiFdSpaceuiSize);
    lib_memcpy(&pfs->cfg, pConfig, sizeof(SPIFFS_CONFIG));

    pfs->pUserData              = pUserData;
    pfs->uiBlkCount             = SPIFFS_CFG_PHYS_SZ(pfs) / SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs);
    pfs->pucWorkBuffer          = pucWorkBuffer;
    pfs->pucLookupWorkBuffer    = pucWorkBuffer + SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs);

    /* 对齐puiFdSpace，8字节对齐 */
    __spiffsAlign8Byte(pucFdSpace, uiFdSpaceuiSize);
    pfs->pucFdSpace = pucFdSpace;
    pfs->uiFdCount  = uiFdSpaceuiSize / sizeof(SPIFFS_FD);
    
    /* 对齐Cache，8字节对齐 */
    __spiffsAlign8Byte(pCache, uiCacheSize);
    if(uiCacheSize & (sizeof(PVOID) - 1)) {
        uiCacheSize -= (uiCacheSize & (sizeof(PVOID) - 1));
    }   
    pfs->pCache         = pCache;
    pfs->uiCacheSize    = uiCacheSize;

    iRes = spiffsCacheInit(pfs);

    pfs->uiConfigMagic = SPIFFS_CONFIG_MAGIC;
    //TODO: 扫描介质
    iRes = spiffsObjLookUpScan(pfs);

    SPIFFS_DBG("page index byte iLen:        "_SPIPRIi"\n", (UINT32)SPIFFS_CFG_LOGIC_PAGE_SZ(pfs));
    SPIFFS_DBG("object lookup pages:         "_SPIPRIi"\n", (UINT32)SPIFFS_OBJ_LOOKUP_PAGES(pfs));
    SPIFFS_DBG("page pages per block:        "_SPIPRIi"\n", (UINT32)SPIFFS_PAGES_PER_BLOCK(pfs));
    SPIFFS_DBG("page header length:          "_SPIPRIi"\n", (UINT32)sizeof(SPIFFS_PAGE_HEADER));
    /* 一个Index Header的Index Page所能索引的最大页面数 */
    SPIFFS_DBG("object header index entries: "_SPIPRIi"\n", (UINT32)SPIFFS_OBJ_HDR_IX_LEN(pfs));
    /* 一个非Index Header的Index Page所能索引的最大页面数 */
    SPIFFS_DBG("object index entries:        "_SPIPRIi"\n", (UINT32)SPIFFS_OBJ_IX_LEN(pfs));
    /* 配置的最大文件描述符 */
    SPIFFS_DBG("available file descriptors:  "_SPIPRIi"\n", (UINT32)pfs->uiFdCount);
    SPIFFS_DBG("total blocks:                "_SPIPRIi"\n", (UINT32)(SPIFFS_CFG_PHYS_SZ(pfs) / SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs)));
    SPIFFS_DBG("free blocks:                 "_SPIPRIi"\n", (UINT32)pfs->uiFreeBlks);

    // printf("Spiffs is a flat file system, which means directory is not permitted here\n");
    pfs->checkCallbackFunc = checkCallbackFunc;
    pfs->uiMountedFlag = 1;
    
    return SPIFFS_OK;
}
/*********************************************************************************************************
** 函数名称: __spiffs_unmount
** 功能描述: 卸载文件系统
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __spiffs_unmount(PSPIFFS_VOLUME pfs){
    SPIFFS_API_DBG("%stat\n", __func__);
    UINT32 i;
    PSPIFFS_FD pFds = (PSPIFFS_FD)pfs->pucFdSpace;
    PSPIFFS_FD pCurFd;
    if(pfs->uiMountedFlag == 0){
        return;
    }
    for (i = 0; i < pfs->uiFdCount; i++)
    {
        pCurFd = &pFds[i];
        if(pCurFd->fileN != 0) {
            ////TODO: 还未揭开面纱
            spiffsCacheFflush(pfs, pCurFd->fileN);
            spiffsFdReturn(pfs,  pCurFd->fileN);    /* 释放文件描述符 */
        }
    }
    pfs->uiMountedFlag = 0;
    return;
}
/*********************************************************************************************************
** 函数名称: __spiffs_create
** 功能描述: 创建一个新文件
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_create(PSPIFFS_VOLUME pfs, const PCHAR pcPath, SPIFFS_MODE mode) {
    
    SPIFFS_API_DBG("%stat '%stat'\n", __func__, pcPath);
    //TODO: 1. mode为何没有 2. Lock/Unlock
    (VOID)          mode; 
    SPIFFS_OBJ_ID   objId;
    INT32           iRes;

    SPIFFS_API_CHECK_MOUNT(pfs);
    if (lib_strlen(pcPath) > SPIFFS_OBJ_NAME_LEN - 1) {
        SPIFFS_API_CHECK_RES(pfs, SPIFFS_ERR_NAME_TOO_LONG);
    }
    
    iRes = spiffsObjLookUpFindFreeObjId(pfs, &objId, pcPath);
    SPIFFS_CHECK_RES(iRes);
    iRes = spiffsObjectCreate(pfs,objId, pcPath, SPIFFS_TYPE_FILE, LW_NULL);
    SPIFFS_CHECK_RES(iRes);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_open
** 功能描述: 打开/创建一个新文件
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式: SPIFFS_O_APPEND, SPIFFS_O_TRUNC, SPIFFS_O_CREAT, SPIFFS_O_RDONLY,
**                                  SPIFFS_O_WRONLY, SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL, Ignored
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
SPIFFS_FILE __spiffs_open(PSPIFFS_VOLUME pfs, const PCHAR pcPath, SPIFFS_FLAGS flags, SPIFFS_MODE mode){
    (VOID)mode;
    PSPIFFS_FD      pFd;
    SPIFFS_PAGE_IX  pageIX;
    INT32           iRes;
    SPIFFS_OBJ_ID   objId;

    SPIFFS_API_CHECK_MOUNT(pfs);
    SPIFFS_API_DBG("%s '%s' "_SPIPRIfl "\n", __func__, pcPath, flags);
    if (strlen(pcPath) > SPIFFS_OBJ_NAME_LEN - 1) {
        return SPIFFS_ERR_NAME_TOO_LONG;
    }
    iRes = spiffsFdFindNew(pfs, &pFd, pcPath);                              /* 获取一个pFd */
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsObjectFindObjectIndexHeaderByName(pfs, pcPath, &pageIX);   /* 根据Name找到IndexHeader */
    if ((flags & SPIFFS_O_CREAT) == 0) {                                    /* 检查文件合法性 */
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        SPIFFS_CHECK_RES(iRes);
    }

    if (iRes == SPIFFS_OK &&                                                /* 只有SPIFFS_O_CREAT 、 SPIFFS_O_EXCL，错误了？ */
        (flags & (SPIFFS_O_CREAT | SPIFFS_O_EXCL)) == (SPIFFS_O_CREAT | SPIFFS_O_EXCL)) {
        // creat and excl and file exists - fail
        iRes = SPIFFS_ERR_FILE_EXISTS;
        spiffsFdReturn(pfs, pFd->fileN);
        SPIFFS_CHECK_RES(iRes);
    }

    if ((flags & SPIFFS_O_CREAT) && iRes == SPIFFS_ERR_NOT_FOUND) {                 /* 创建文件 */
        // no need to enter conflicting pcName here, already looked for it above
        iRes = spiffsObjLookUpFindFreeObjId(pfs, &objId, LW_NULL);                  /* 找到一个空的 Obj Id*/
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        SPIFFS_CHECK_RES(iRes);
        iRes = spiffsObjectCreate(pfs, objId, pcPath, SPIFFS_TYPE_FILE, &pageIX);   /* 创建一个OBJECT */
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
        flags &= ~SPIFFS_O_TRUNC;
    } 
    else {
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
    }
    iRes = spiffsObjectOpenByPage(pfs, pageIX, pFd, flags, mode);
    if (iRes < SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_CHECK_RES(iRes);
    
    if (flags & SPIFFS_O_TRUNC) {
        iRes = spiffsObjectTruncate(pFd, 0, 0);
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);
    }

    pFd->uiFdOffset = 0;

    //SPIFFS_UNLOCK(pfs);

    return (SPIFFS_FILE)pFd->fileN; //SPIFFS_FH_OFFS(pfs, pFd->fileN);
}
/*********************************************************************************************************
** 函数名称: __spiffs_open_by_dirent
** 功能描述: 打开一个新目录
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式: SPIFFS_O_APPEND, SPIFFS_O_TRUNC, SPIFFS_O_CREAT, SPIFFS_O_RDONLY,
**                                  SPIFFS_O_WRONLY, SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL, Ignored
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
SPIFFS_FILE __spiffs_open_by_dirent(PSPIFFS_VOLUME pfs, PSPIFFS_DIRENT pDirent, 
                                    SPIFFS_FLAGS flags, SPIFFS_MODE mode){
    PSPIFFS_FD pFd;
    INT32 iRes;
    SPIFFS_API_DBG("%stat '%stat':"_SPIPRIid " "_SPIPRIfl "\n", 
                   __func__, pDirent->ucName, pDirent->objId, flags);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);


    iRes = spiffsFdFindNew(pfs, &pFd, 0);
    
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);

    iRes = spiffsObjectOpenByPage(pfs, pDirent->pageIX, pFd, flags, mode);
    if (iRes < SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }

    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    
    if (flags & SPIFFS_O_TRUNC) {
        iRes = spiffsObjectTruncate(pFd, 0, LW_FALSE);
        if (iRes < SPIFFS_OK) {
            spiffsFdReturn(pfs, pFd->fileN);
        }
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    }
    pFd->uiFdOffset = 0;
    //SPIFFS_UNLOCK(pfs);
    return pFd->fileN;
}
/*********************************************************************************************************
** 函数名称: __spiffs_read
** 功能描述: 读文件
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式: SPIFFS_O_APPEND, SPIFFS_O_TRUNC, SPIFFS_O_CREAT, SPIFFS_O_RDONLY,
**                                  SPIFFS_O_WRONLY, SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL, Ignored
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_read(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PCHAR pcContent, INT32 iLen){
    SPIFFS_API_DBG("%stat "_SPIPRIfd " "_SPIPRIi "\n", __func__, fileHandler, iLen);
    INT32 iRes = spiffsFileRead(pfs, fileHandler, pcContent, iLen);
    if (iRes == SPIFFS_ERR_END_OF_OBJECT) {
        iRes = 0;
    }
    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_read
** 功能描述: 读文件
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式: SPIFFS_O_APPEND, SPIFFS_O_TRUNC, SPIFFS_O_CREAT, SPIFFS_O_RDONLY,
**                                  SPIFFS_O_WRONLY, SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL, Ignored
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_write(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler,  PCHAR pcContent, INT32 iLen) {
    SPIFFS_API_DBG("%s "_SPIPRIfd " "_SPIPRIi "\n", __func__, fileHandler, iLen);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    PSPIFFS_FD pFd;
    INT32 iRes;
    UINT32 uiOffset;

    iRes = spiffsFdGet(pfs, fileHandler, &pFd);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    if ((pFd->flags & SPIFFS_O_WRONLY) == 0) {
        iRes = SPIFFS_ERR_NOT_WRITABLE;
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_API_CHECK_RES(pfs, iRes);
    }

    if ((pFd->flags & SPIFFS_O_APPEND)) {
        pFd->uiFdOffset = pFd->uiSize == SPIFFS_UNDEFINED_LEN ? 0 : pFd->uiSize;
    }
    uiOffset = pFd->uiFdOffset;

    if (pFd->pCachePage == LW_NULL) {
        // see if object id is associated with pCache already
        pFd->pCachePage = spiffsCachePageGetByFd(pfs, pFd);
    }
    if (pFd->flags & SPIFFS_O_APPEND) {
        if (pFd->uiSize == SPIFFS_UNDEFINED_LEN) {
            uiOffset = 0;
        } 
        else {
            uiOffset = pFd->uiSize;
        }
        if (pFd->pCachePage) {
            uiOffset = MAX(uiOffset, pFd->pCachePage->uiOffset + pFd->pCachePage->uiSize);
        }
    }

    if ((pFd->flags & SPIFFS_O_DIRECT) == 0) {
        if (iLen < (INT32)SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)) {
            // small write, try to pCache it
            BOOL bIsAllocCachePage = LW_TRUE;
            if (pFd->pCachePage) {
                // have a cached page for this pFd already, check pCache page boundaries
                if (uiOffset < pFd->pCachePage->uiOffset || // writing before pCache
                    uiOffset > pFd->pCachePage->uiOffset + pFd->pCachePage->uiSize || // writing after pCache
                    uiOffset + iLen > pFd->pCachePage->uiOffset + SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)) // writing beyond pCache page
                {
                    // boundary violation, write back pCache first and allocate new
                    SPIFFS_CACHE_DBG("CACHE_WR_DUMP: dumping pCache page "_SPIPRIi" for pFd "_SPIPRIfd":"_SPIPRIid", boundary viol, offs:"_SPIPRIi" uiSize:"_SPIPRIi"\n",
                                     pFd->pCachePage->uiIX, pFd->fileN, pFd->objId, 
                                     pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
                    iRes = spiffsFileWrite(pfs, pFd, SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, SPIFFS_GET_CACHE_HDR(pfs), pFd->pCachePage->uiIX),
                                           pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
                    spiffsCacheFdRelease(pfs, pFd->pCachePage);
                    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);

                } 
                else {
                    // writing within pCache
                    bIsAllocCachePage = LW_FALSE;
                }
            }

            if (bIsAllocCachePage) {
                //TODO: spiffs_cache_page_allocate_by_fd
                pFd->pCachePage = spiffsCachePageAllocateByFd(pfs, pFd);    /* 为该文件分配一个CachePage */
                if (pFd->pCachePage) {
                    pFd->pCachePage->uiOffset = uiOffset;
                    pFd->pCachePage->uiSize = 0;
                    SPIFFS_CACHE_DBG("CACHE_WR_ALLO: allocating pCache page "_SPIPRIi" for pFd "_SPIPRIfd":"_SPIPRIid"\n",
                                     pFd->pCachePage->uiIX, pFd->fileN, pFd->objId);
                }
            }

            if (pFd->pCachePage) {          /* 若分配成功 */
                UINT32 uiOffsetInCachePage = uiOffset - pFd->pCachePage->uiOffset;
                SPIFFS_CACHE_DBG("CACHE_WR_WRITE: storing to pCache page "_SPIPRIi" for pFd "_SPIPRIfd":"_SPIPRIid", offs "_SPIPRIi":"_SPIPRIi" iLen "_SPIPRIi"\n",
                    pFd->pCachePage->uiIX, pFd->fileN, pFd->objId,
                    uiOffset, uiOffsetInCachePage, iLen);
                PSPIFFS_CACHE pCache = SPIFFS_GET_CACHE_HDR(pfs);
                PUCHAR pPageData = SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, pFd->pCachePage->uiIX);

                lib_memcpy(&pPageData[uiOffsetInCachePage], pcContent, iLen);
                pFd->pCachePage->uiSize = MAX(pFd->pCachePage->uiSize, uiOffsetInCachePage + iLen);
                pFd->uiFdOffset += iLen;
                //SPIFFS_UNLOCK(pfs);
                return iLen;
            } 
            else {                          /* 否则直接写文件 */
                iRes = spiffsFileWrite(pfs, pFd, pcContent, uiOffset, iLen);
                //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
                SPIFFS_API_CHECK_RES(pfs, iRes);
                pFd->uiFdOffset += iLen;
                //SPIFFS_UNLOCK(pfs);
                return iRes;
            }
        } 
        else {
        // big write, no need to pCache it - but first check if there is a cached write already
            if (pFd->pCachePage) {
                // write back pCache first
                SPIFFS_CACHE_DBG("CACHE_WR_DUMP: dumping pCache page "_SPIPRIi" for pFd "_SPIPRIfd":"_SPIPRIid", big write, offs:"_SPIPRIi" uiSize:"_SPIPRIi"\n",
                                 pFd->pCachePage->uiIX, pFd->fileN, pFd->objId, 
                                 pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
                iRes = spiffsFileWrite(pfs, pFd, SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, SPIFFS_GET_CACHE_HDR(pfs), pFd->pCachePage->uiIX),
                                       pFd->pCachePage->uiOffset, pFd->pCachePage->uiSize);
                spiffsCacheFdRelease(pfs, pFd->pCachePage);
                //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
                SPIFFS_API_CHECK_RES(pfs, iRes);
                // data written below
            }
        }
    }

    iRes = spiffsFileWrite(pfs, pFd, pcContent, uiOffset, iLen);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    pFd->uiFdOffset += iLen;

    //SPIFFS_UNLOCK(pfs);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_lseek
** 功能描述: 移动文件指针
** 输　入  : pfs          文件头
**           pcPath        文件路径
**           mode          文件模式: SPIFFS_O_APPEND, SPIFFS_O_TRUNC, SPIFFS_O_CREAT, SPIFFS_O_RDONLY,
**                                  SPIFFS_O_WRONLY, SPIFFS_O_RDWR, SPIFFS_O_DIRECT, SPIFFS_O_EXCL, Ignored
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_lseek(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, UINT32 uiOffset, INT iSeekFlag){
    PSPIFFS_FD pFd;
    INT32      iRes;
    
    SPIFFS_API_DBG("%stat "_SPIPRIfd " "_SPIPRIi " %stat\n", 
                    __func__, fileHandler, uiOffset, 
                    (const char* []){"SET","CUR","END","???"}[MIN(iSeekFlag, 3)]);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    iRes = spiffsFdGet(pfs, fileHandler, &pFd);
    
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_CHECK_RES(iRes);

    // #if SPIFFS_CACHE_WR
    // spiffs_fflush_cache(pfs, fileHandler);
    // #endif
    spiffsCacheFflush(pfs, fileHandler);            /* 把与该file相关的Cache的内容 */

    INT32 iFileuiSize = pFd->uiSize == SPIFFS_UNDEFINED_LEN ? 0 : pFd->uiSize;

    switch (iSeekFlag) {
    case SPIFFS_SEEK_CUR:           /* 从当前位置开始 */
        uiOffset = pFd->uiFdOffset + uiOffset;
        break;
    case SPIFFS_SEEK_END:           /* 从文件末尾开始 */
        uiOffset = iFileuiSize + uiOffset;
        break;
    }
    if (uiOffset < 0) {
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, SPIFFS_ERR_SEEK_BOUNDS);
        iRes = SPIFFS_ERR_SEEK_BOUNDS; 
        SPIFFS_CHECK_RES(iRes);
    }
    if (uiOffset > iFileuiSize) {     /* Offset 大于 文件大小就不行了 */
        pFd->uiFdOffset = iFileuiSize;
        iRes = SPIFFS_ERR_END_OF_OBJECT;
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_CHECK_RES(iRes);

    SPIFFS_SPAN_IX spanIXObjData = (uiOffset > 0 ? (uiOffset - 1) : 0) / SPIFFS_DATA_PAGE_SIZE(pfs);    /* 计算Data Page的Span uiIX */
    SPIFFS_SPAN_IX spanIXObjuiIX = SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIXObjData);                       /* 计算对应Index Page的Span uiIX */
    
    if (pFd->spanIXObjIXCursor != spanIXObjuiIX) {    /* 调整文件的 ObjuiIX 的指针spanuiIX */
        SPIFFS_PAGE_IX pageIX;
        iRes = spiffsObjLookUpFindIdAndSpan(pfs, pFd->objId | SPIFFS_OBJ_ID_IX_FLAG, spanIXObjuiIX, 
                                            0, &pageIX);
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_CHECK_RES(iRes);    
        pFd->spanIXObjIXCursor = spanIXObjuiIX;
        pFd->pageIXObjIXCursor = pageIX;
    }
    /* 调整文件内部指针 */
    pFd->uiFdOffset = uiOffset;

    //SPIFFS_UNLOCK(pfs);

    return uiOffset;
}
/*********************************************************************************************************
** 函数名称: __spiffs_remove
** 功能描述: 移除一个文件，相当于把它全部截断
** 输　入  : pfs          文件头
**           pcPath        文件路径
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_remove(PSPIFFS_VOLUME pfs, const PCHAR pcPath) {
    PSPIFFS_FD pFd;
    SPIFFS_PAGE_IX pageIX;
    INT32 iRes;

    SPIFFS_API_DBG("%stat '%stat'\n", __func__, pcPath);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    if (strlen(pcPath) > SPIFFS_OBJ_NAME_LEN - 1) {
        SPIFFS_API_CHECK_RES(pfs, SPIFFS_ERR_NAME_TOO_LONG);
    }
    //SPIFFS_LOCK(pfs);

    iRes = spiffsFdFindNew(pfs, &pFd, LW_NULL);             /* 获取一个新的pFd */
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_CHECK_RES(iRes);

    iRes = spiffsObjectFindObjectIndexHeaderByName(pfs, (PUCHAR)pcPath, &pageIX);    /* 找到pcPath对应的spanuiIX = 0的Index页面对应的pageIX */
    if (iRes != SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsObjectOpenByPage(pfs, pageIX, pFd, 0, 0);   /* 打开这个页面，即获取到pcPath对应的文件 */
    if (iRes != SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }
    
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsObjectTruncate(pFd, 0, LW_TRUE);            /* 截断并移除 */
    if (iRes != SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    
    //SPIFFS_UNLOCK(pfs);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_fremove
** 功能描述: 移除一个文件，相当于把它全部截断
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_fremove(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler) {
    SPIFFS_API_DBG("%stat "_SPIPRIfd "\n", __func__, fileHandler);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    PSPIFFS_FD pFd;
    INT32 iRes;
    iRes = spiffsFdGet(pfs, fileHandler, &pFd);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    if ((pFd->flags & SPIFFS_O_WRONLY) == 0) {
        iRes = SPIFFS_ERR_NOT_WRITABLE;
        //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
        SPIFFS_API_CHECK_RES(pfs, iRes);
    }
    //TODO:这个函数干嘛的？
    spiffsCacheFdRelease(pfs, pFd->pCachePage);

    iRes = spiffsObjectTruncate(pFd, 0, LW_TRUE);
    
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    //SPIFFS_UNLOCK(pfs);

    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_stat
** 功能描述: 描述一个文件，返回至pStat
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_stat(PSPIFFS_VOLUME pfs, const PCHAR pcPath, PSPIFFS_STAT pStat) {
    SPIFFS_API_DBG("%s '%s'\n", __func__, pcPath);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    if (strlen(pcPath) > SPIFFS_OBJ_NAME_LEN - 1) {
        SPIFFS_API_CHECK_RES(pfs, SPIFFS_ERR_NAME_TOO_LONG);
    }
    //SPIFFS_LOCK(pfs);

    INT32 iRes;
    SPIFFS_PAGE_IX pageIX;

    iRes = spiffsObjectFindObjectIndexHeaderByName(pfs, (PUCHAR)pcPath, &pageIX);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsStatPageIX(pfs, pageIX, 0, pStat);

    //SPIFFS_UNLOCK(pfs);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_fstat
** 功能描述: 描述一个文件，返回至pStat
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_fstat(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler, PSPIFFS_STAT pStat) {
    SPIFFS_API_DBG("%s " _SPIPRIfd "\n", __func__, fileHandler);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    INT32 iRes;
    SPIFFS_PAGE_IX pageIX;
    PSPIFFS_FD pFd;

    iRes = spiffsFdGet(pfs, fileHandler, &pFd);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsStatPageIX(pfs, pFd->pageIXObjIXHdr, fileHandler, pStat);

    //SPIFFS_UNLOCK(pfs);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_fflush
** 功能描述: 描述一个文件，返回至pStat
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_fflush(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler) {
    SPIFFS_API_DBG("%s "_SPIPRIfd "\n", __func__, fileHandler);
    (void)fileHandler;
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    INT32 iRes = SPIFFS_OK;
    //SPIFFS_LOCK(pfs);
    iRes = spiffsCacheFflush(pfs, fileHandler);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs,iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //SPIFFS_UNLOCK(pfs);
    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_close
** 功能描述: 关闭一个打开的文件，主要刷写Cache与释放pFd
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_close(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler) {
    SPIFFS_API_DBG("%s "_SPIPRIfd "\n", __func__, fileHandler);
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);

    INT32 iRes = SPIFFS_OK;
    //SPIFFS_LOCK(pfs);

    //fileHandler = SPIFFS_FH_UNOFFS(pfs, fileHandler);

    iRes = spiffsCacheFflush(pfs, fileHandler);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsFdReturn(pfs, fileHandler);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //SPIFFS_UNLOCK(pfs);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_rename
** 功能描述: 重命名
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_rename(PSPIFFS_VOLUME pfs, const PCHAR pcOldPath, const PCHAR pcNewPath) {
    SPIFFS_API_DBG("%s %s %s\n", __func__, pcOldPath, pcNewPath);

    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    if (lib_strlen(pcNewPath) > SPIFFS_OBJ_NAME_LEN - 1 ||
        lib_strlen(pcOldPath) > SPIFFS_OBJ_NAME_LEN - 1) {
        SPIFFS_API_CHECK_RES(pfs, SPIFFS_ERR_NAME_TOO_LONG);
    }
    //SPIFFS_LOCK(pfs);

    SPIFFS_PAGE_IX pageIXOld, pageIXDummy;
    PSPIFFS_FD pFd;

    INT32 iRes = spiffsObjectFindObjectIndexHeaderByName(pfs, (PUCHAR)pcOldPath, &pageIXOld);
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    /* 检查目的路径冲突 */
    iRes = spiffsObjectFindObjectIndexHeaderByName(pfs, (PUCHAR)pcNewPath, &pageIXDummy);
    
    if (iRes == SPIFFS_ERR_NOT_FOUND) {
        iRes = SPIFFS_OK;
    } 
    else if (iRes == SPIFFS_OK) {
        iRes = SPIFFS_ERR_CONFLICTING_NAME;
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsFdFindNew(pfs, &pFd, LW_NULL);     /* 找一个新的pFd */
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsObjectOpenByPage(pfs, pageIXOld, pFd, 0, 0);       /* 打开原来的页面到pFd中 */
    if (iRes != SPIFFS_OK) {
        spiffsFdReturn(pfs, pFd->fileN);
    }
    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);

    iRes = spiffsObjectUpdateIndexHdr(pfs, pFd, pFd->objId, pFd->pageIXObjIXHdr, 
                                      LW_NULL, (PUCHAR)pcNewPath, 0, &pageIXDummy); /* 更新一波IndexHdr */
    if (iRes == SPIFFS_OK) {
        /* 修改对应pFd的hash值 */
        spiffsFdTemporalCacheRehash(pfs, pcOldPath, pcNewPath);
    }
    
    spiffsFdReturn(pfs, pFd->fileN);        /* 关闭文件 */

    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //SPIFFS_UNLOCK(pfs);

    return iRes;
}
/*********************************************************************************************************
** 函数名称: __spiffs_opendir
** 功能描述: 重命名
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_DIR __spiffs_opendir(PSPIFFS_VOLUME pfs, const PCHAR pcName, PSPIFFS_DIR pDir) {
    SPIFFS_API_DBG("%s\n", __func__);
    (void)pcName;

    // if (!SPIFFS_CHECK_CFG((pfs))) {
    //     (pfs)->uiErrorCode = SPIFFS_ERR_NOT_CONFIGURED;
    //     return 0;
    // }

    if (!SPIFFS_CHECK_MOUNT(pfs)) {
        pfs->uiErrorCode = SPIFFS_ERR_NOT_MOUNTED;
        return 0;
    }

    pDir->pfs = pfs;
    pDir->blkIX = 0;
    pDir->uiEntry = 0;
    return pDir;
}
/*********************************************************************************************************
** 函数名称: __spiffs_closedir
** 功能描述: 重命名
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_closedir(PSPIFFS_DIR pDir) {
    SPIFFS_API_DBG("%s\n", __func__);
    //SPIFFS_API_CHECK_CFG(pDir->pfs);
    SPIFFS_API_CHECK_MOUNT(pDir->pfs);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_readdir
** 功能描述: 重命名
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_DIRENT __spiffs_readdir(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent) {
    SPIFFS_API_DBG("%s\n", __func__);
    if (!SPIFFS_CHECK_MOUNT(pDir->pfs)) {
        pDir->pfs->uiErrorCode = SPIFFS_ERR_NOT_MOUNTED;
        return 0;
    }
    //SPIFFS_LOCK(pDir->pfs);
    pDirent = spiffsDirRead(pDir, pDirent);
    
    //SPIFFS_UNLOCK(pDir->pfs);
    return pDirent;
}
/*********************************************************************************************************
** 函数名称: __spiffs_gc_quick
** 功能描述: 快速GC
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_gc_quick(PSPIFFS_VOLUME pfs, UINT16 uiMaxFreePages){
    SPIFFS_API_DBG("%s "_SPIPRIi "\n", __func__, uiMaxFreePages);
    INT32 iRes;
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    iRes = spiffsGCQuick(pfs, uiMaxFreePages);

    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //SPIFFS_UNLOCK(pfs);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_gc_quick
** 功能描述: 快速GC
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_gc(PSPIFFS_VOLUME pfs, UINT32 uiSize) {
    SPIFFS_API_DBG("%s "_SPIPRIi "\n", __func__, uiSize);
    INT32 iRes;
    //SPIFFS_API_CHECK_CFG(pfs);
    SPIFFS_API_CHECK_MOUNT(pfs);
    //SPIFFS_LOCK(pfs);

    iRes = spiffsGCCheck(pfs, uiSize);

    //SPIFFS_API_CHECK_RES_UNLOCK(pfs, iRes);
    SPIFFS_API_CHECK_RES(pfs, iRes);
    //SPIFFS_UNLOCK(pfs);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __spiffs_gc_quick
** 功能描述: 快速GC
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT32 __spiffs_set_file_callback_func(PSPIFFS_VOLUME pfs, spiffsFileCallback fileCallBackFunc) {
    SPIFFS_API_DBG("%s\n", __func__);
    //SPIFFS_LOCK(pfs);
    pfs->fileCallbackFunc = fileCallBackFunc;
    //SPIFFS_UNLOCK(pfs);
    return 0;
}
/*********************************************************************************************************
 * ANCHOR: SyliXOS 适配小节
*********************************************************************************************************/
/*********************************************************************************************************
** 函数名称: __spif_mount
** 功能描述: 封装mount过程
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_mount(PSPIF_VOLUME pfs){
    PSPIFFS_CONFIG  pConfig;

    UINT32          uiWorkSize;
    PUCHAR          pucWorkBuffer;
    
    UINT32          uiCachePages = 8;
    UINT32          uiCacheSize;
    PUCHAR          pCache;

    UINT32          uiDescriptors = 16;
    UINT32          uiFdsSize;     
    PSPIFFS_FD      pFds; 

     /* 初始化配置*/
    pConfig                     = (PSPIFFS_CONFIG)lib_malloc(sizeof(SPIFFS_CONFIG));
    pConfig->halEraseFunc       = LW_NULL;
    pConfig->halReadFunc        = LW_NULL;
    pConfig->halWriteFunc       = LW_NULL;
    pConfig->uiLogicBlkSize     = 64 * 1024;                /* 64KB */
    pConfig->uiLogicPageSize    = 256;                      /* 256B */
    pConfig->uiPhysEraseBlkSize = 64 * 1024;
    pConfig->uiPhysAddr         = NOR_FLASH_START_OFFSET;   /* 其实 */
    pConfig->uiPhysSize         = NOR_FLASH_SZ - NOR_FLASH_START_OFFSET;

    /* 分配各种buffer */
    uiWorkSize                  = pConfig->uiLogicBlkSize * 2;
    uiCacheSize                 = sizeof(SPIFFS_CACHE) + 
                                  uiCachePages * (sizeof(SPIFFS_CACHE_PAGE) + pConfig->uiLogicPageSize);
    uiFdsSize                   = uiDescriptors * sizeof(SPIFFS_FD);

    pucWorkBuffer               = (PUCHAR)lib_malloc(uiWorkSize);
    pCache                      = (PUCHAR)lib_malloc(uiCacheSize);
    pFds                        = (PSPIFFS_FD)lib_malloc(uiFdsSize);

    return __spiffs_mount(SYLIX_TO_SPIFFS_PFS(pfs), pConfig, pucWorkBuffer, 
                          (UINT8 *)pFds, uiFdsSize, pCache, uiCacheSize, LW_NULL);

}
/*********************************************************************************************************
** 函数名称: __spif_unmount
** 功能描述: 封装unmount过程
** 输　入  : pfs          文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_unmount(PSPIF_VOLUME pfs){
    __spiffs_unmount(SYLIX_TO_SPIFFS_PFS(pfs));
    return SPIFFS_OK;
}
/*********************************************************************************************************
** 函数名称: __spif_open
** 功能描述: 封装open过程
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFN_NODE __spif_open(PSPIF_VOLUME pfs, PCHAR pcName, INT iFlags, INT iMode, BOOL *pbIsRoot){
    PSPIFFS_FD          pFd;
    SPIFFS_FILE         fileHandler;
    SPIFFS_FLAGS        flags;
    INT                 iRes;
    PSPIFN_NODE         pspifn;
    PSPIFFS_VOLUME      spiffsPfs;
    
    CHAR                pcTempName[MAX_FILENAME_LENGTH];
    INT                 iNameLen;

    if (*pcName == PX_ROOT) {                                           /*  忽略根符号                  */
        lib_strlcpy(pcTempName, (pcName + 1), PATH_MAX);
    }
    else {
        lib_strlcpy(pcTempName, pcName, PATH_MAX);
    }

    if (pcTempName[0] == PX_EOS) {
        if (pbIsRoot) {
            *pbIsRoot = LW_TRUE;                                          /*  pcName 为根                 */
        }
        return  (LW_NULL);
    }
    else {
        if (pbIsRoot) {
            *pbIsRoot = LW_FALSE;                                         /*  pcName 不为根               */
        }
    }

    spiffsPfs   = SYLIX_TO_SPIFFS_PFS(pfs);
    /* 转换Flag */
    flags       = spiffsTranslateToSylixOSFlag(iFlags);
    fileHandler = __spiffs_open(spiffsPfs, pcTempName, flags, 0);
    if(fileHandler < 0){
        return LW_NULL;
    }
    iRes        = spiffsFdGet(spiffsPfs, fileHandler, &pFd);
    if(iRes != SPIFFS_OK){
        spiffsFdReturn(spiffsPfs, fileHandler);
        SPIFFS_DBG("Open File Failed, Only Allow %d File Opened\n", spiffsPfs->uiFdCount);
        return LW_NULL;
    }
    iNameLen = lib_strlen(pcTempName);
    //TODO: pspifn更多字段填写
    pspifn                   = (PSPIFN_NODE)__SHEAP_ALLOC(sizeof(SPIFN_NODE));
    pspifn->pFd              = pFd;
    pspifn->pfs              = pfs;
    pspifn->SPIFN_gid        = getgid(); 
    pspifn->SPIFN_uid        = getuid();
    /* 暂时默认为Reguler文件 */
    pspifn->SPIFN_mode       = (iMode | S_IFREG);
    pspifn->SPIFN_pcLink     = LW_NULL;
    pspifn->SPIFN_pcName     = (PCHAR)__SHEAP_ALLOC(iNameLen * sizeof(CHAR));
    lib_memcpy(pspifn->SPIFN_pcName, pcName, iNameLen);
    //pspifn->SPIFN_bChanged = LW_TRUE;
    return pspifn;
}   
/*********************************************************************************************************
** 函数名称: __spif_close
** 功能描述: 封装close过程
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_close(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn){
    SPIFFS_FILE fileHandler = SYLIX_TO_SPIFFS_FD(pspifn);
    return __spiffs_close(SYLIX_TO_SPIFFS_PFS(pfs), fileHandler);
}
/*********************************************************************************************************
** 函数名称: __spif_remove
** 功能描述: 封装remove过程
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_remove(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn){
    SPIFFS_FILE fileHandler = SYLIX_TO_SPIFFS_FD(pspifn);
    return __spiffs_fremove(SYLIX_TO_SPIFFS_PFS(pfs), fileHandler);
}
/*********************************************************************************************************
** 函数名称: __spif_stat
** 功能描述: SylixOS描述文件
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __spif_stat(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, struct stat* pstat) {
    SPIFFS_STAT   stat;
    if(pspifn) {
        __spiffs_fstat(SYLIX_TO_SPIFFS_PFS(pfs), SYLIX_TO_SPIFFS_FD(pspifn), &stat);
    }
    if (pspifn) {
        pstat->st_dev = LW_DEV_MAKE_STDEV(&pfs->SPIFFS_devhdrHdr);
        pstat->st_ino = (ino_t)pspifn;
        pstat->st_mode = pspifn->SPIFN_mode;
        pstat->st_nlink = 1;
        pstat->st_uid = pspifn->SPIFN_uid;
        pstat->st_gid = pspifn->SPIFN_gid;
        pstat->st_rdev = 1;
        pstat->st_size = (off_t)stat.uiSize;
        // pstat->st_atime = pInodeInfo->HOITN_timeAccess;
        // pstat->st_mtime = pInodeInfo->HOITN_timeChange;
        // pstat->st_ctime = pInodeInfo->HOITN_timeCreate;
        pstat->st_blksize = 0;
        pstat->st_blocks = 0;
    }
    else {
        pstat->st_dev = LW_DEV_MAKE_STDEV(&pfs->SPIFFS_devhdrHdr);
        pstat->st_ino = (ino_t)LW_NULL;
        pstat->st_mode = pfs->SPIFFS_mode;
        pstat->st_nlink = 1;
        pstat->st_uid = pfs->SPIFFS_uid;
        pstat->st_gid = pfs->SPIFFS_gid;
        pstat->st_rdev = 1;
        pstat->st_size = 0;
        pstat->st_atime = pfs->SPIFFS_time;
        pstat->st_mtime = pfs->SPIFFS_time;
        pstat->st_ctime = pfs->SPIFFS_time;
        pstat->st_blksize = 0;
        pstat->st_blocks = 0;
    }

    pstat->st_resv1 = LW_NULL;
    pstat->st_resv2 = LW_NULL;
    pstat->st_resv3 = LW_NULL;
    return ERROR_NONE;
}
/*********************************************************************************************************
** 函数名称: __spif_read
** 功能描述: 封装read操作
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_read(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR pcContent, UINT32 uiOffset, UINT32 uiLen){
    UINT32 iRes = SPIFFS_OK;
    UINT32 uiReadBytes = 0;
    __spiffs_lseek(SYLIX_TO_SPIFFS_PFS(pfs),SYLIX_TO_SPIFFS_FD(pspifn), uiOffset, SPIFFS_SEEK_SET);
    iRes = __spiffs_read(SYLIX_TO_SPIFFS_PFS(pfs), SYLIX_TO_SPIFFS_FD(pspifn), pcContent, uiLen);
    if(pspifn->pFd->uiSize == SPIFFS_UNDEFINED_LEN){
        uiReadBytes = 0;
    }
    else {
        uiReadBytes = MIN(pspifn->pFd->uiSize - uiOffset, uiLen);
    }
    return uiReadBytes;
}
/*********************************************************************************************************
** 函数名称: __spif_write
** 功能描述: 封装write操作
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_write(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR pcContent, UINT32 uiOffset, UINT32 uiLen){
    UINT32 iRes = SPIFFS_OK;
    __spiffs_lseek(SYLIX_TO_SPIFFS_PFS(pfs),SYLIX_TO_SPIFFS_FD(pspifn), uiOffset, SPIFFS_SEEK_SET);
    iRes = __spiffs_write(SYLIX_TO_SPIFFS_PFS(pfs), SYLIX_TO_SPIFFS_FD(pspifn), pcContent, uiLen);
    return uiLen;
}
/*********************************************************************************************************
** 函数名称: __spif_rename
** 功能描述: 封装rename操作
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_rename(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, PCHAR  pcNewName){
    return __spiffs_rename(SYLIX_TO_SPIFFS_PFS(pfs), pspifn->SPIFN_pcName, pcNewName);
}
/*********************************************************************************************************
** 函数名称: __spif_lseek
** 功能描述: 封装lseek操作
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_lseek(PSPIF_VOLUME pfs, PSPIFN_NODE pspifn, UINT32 uiOffset){
    return __spiffs_lseek(SYLIX_TO_SPIFFS_PFS(pfs), SYLIX_TO_SPIFFS_FD(pspifn), uiOffset, SPIFFS_SEEK_SET);
}
/*********************************************************************************************************
** 函数名称: __spif_statfs
** 功能描述: statfs
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_statfs(PSPIF_VOLUME pfs, struct statfs *pstatfs){
    pstatfs->f_type = SPIFFS_CONFIG_MAGIC;  //需要修改
    pstatfs->f_bsize = 0;
    pstatfs->f_blocks = 0;
    pstatfs->f_bfree = 0;
    pstatfs->f_bavail = 1;

    pstatfs->f_files = 0;
    pstatfs->f_ffree = 0;

    #if LW_CFG_CPU_WORD_LENGHT == 64
    pstatfs->f_fsid.val[0] = (int32_t)((addr_t)pfs >> 32);
    pstatfs->f_fsid.val[1] = (int32_t)((addr_t)pfs & 0xffffffff);
    #else
    pstatfs->f_fsid.val[0] = (int32_t)pfs;
    pstatfs->f_fsid.val[1] = 0;
    #endif

    pstatfs->f_flag = 0;
    pstatfs->f_namelen = PATH_MAX;
    return ERROR_NONE;
}
/*********************************************************************************************************
** 函数名称: __spif_opendir
** 功能描述: 封装打开一个目录，目录暂时必须是'/'
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_DIR __spif_opendir(PSPIF_VOLUME pfs, const PCHAR pcName, PSPIFFS_DIR pDir){
    return __spiffs_opendir(SYLIX_TO_SPIFFS_PFS(pfs), pcName, pDir);
}
/*********************************************************************************************************
** 函数名称: __spif_closedir
** 功能描述: 关闭一个目录文件
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT __spif_closedir(PSPIF_VOLUME pfs, PSPIFFS_DIR pDir){
    (VOID)pfs;
    return __spiffs_closedir(pDir);
}
/*********************************************************************************************************
** 函数名称: __spif_readdir
** 功能描述: 封装读一个目录下的内容
** 输　入  : pfs          文件头
**           fileHandler   文件句柄
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PSPIFFS_DIRENT __spif_readdir(PSPIFFS_DIR pDir, PSPIFFS_DIRENT pDirent){
    return __spiffs_readdir(pDir, pDirent);
}
