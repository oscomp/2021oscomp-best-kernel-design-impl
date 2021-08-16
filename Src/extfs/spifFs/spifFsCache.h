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
** 文   件   名: spifFsCache.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 09日
**
** 描        述: Spiffs文件系统缓存层
*********************************************************************************************************/

#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSCACHE_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSCACHE_H_
#include "spifFsType.h"

/*********************************************************************************************************
 * SPIFFS Cache Flag
*********************************************************************************************************/
#define SPIFFS_CACHE_FLAG_DIRTY       (1<<0)
#define SPIFFS_CACHE_FLAG_WRTHRU      (1<<1)
#define SPIFFS_CACHE_FLAG_OBJLU       (1<<2)
#define SPIFFS_CACHE_FLAG_OBJIX       (1<<3)
#define SPIFFS_CACHE_FLAG_DATA        (1<<4)
#define SPIFFS_CACHE_FLAG_TYPE_WR     (1<<7)

/*********************************************************************************************************
 * SPIFFS Cache相关宏定义
*********************************************************************************************************/
/**
 * @brief 获取CachePage大小 = CachePage头信息大小 + 逻辑页面大小
 */
#define SPIFFS_CACHE_PAGE_SIZE(pfs)                     (sizeof(SPIFFS_CACHE_PAGE) + SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))
#define SPIFFS_GET_CACHE_PAGE_SIZE(pfs)                 (sizeof(SPIFFS_CACHE_PAGE) + SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))

#define SPIFFS_GET_CACHE_HDR(pfs)                       (PSPIFFS_CACHE)pfs->pCache
#define SPIFFS_GET_CACHE_PAGE_HDR(pfs, pCache, ix)      ((PSPIFFS_CACHE_PAGE)(&((pCache)->Cpages[(ix) * SPIFFS_GET_CACHE_PAGE_SIZE(pfs)])))
#define SPIFFS_GET_CACHE_PAGE_CONTENT(pfs, pCache, ix)  ((PCHAR)(&((pCache)->Cpages[(ix) * SPIFFS_CACHE_PAGE_SIZE(pfs)]) \
                                                        + sizeof(SPIFFS_CACHE_PAGE)))
/*********************************************************************************************************
 * SPIFFS Cache相关函数
*********************************************************************************************************/
INT32 spiffsCacheInit(PSPIFFS_VOLUME pfs);
INT32 spiffsCacheRead(PSPIFFS_VOLUME pfs, UINT8 uiOps, SPIFFS_FILE fileHandler, 
                      UINT32 uiAddr, UINT32 uiLen, PCHAR pDst);
INT32 spiffsCacheWrite(PSPIFFS_VOLUME pfs, UINT8 uiOps, SPIFFS_FILE fileHandler, 
                       UINT32 uiAddr, UINT32 uiLen, PCHAR pSrc);
INT32 spiffsEraseBlk(PSPIFFS_VOLUME pfs, SPIFFS_BLOCK_IX blkIX);



INT32               spiffsCacheFflush(PSPIFFS_VOLUME pfs, SPIFFS_FILE fileHandler);
PSPIFFS_CACHE_PAGE  spiffsCachePageGetByFd(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd);
VOID                spiffsCacheFdRelease(PSPIFFS_VOLUME pfs, PSPIFFS_CACHE_PAGE pCachePage);
VOID                spiffsCacheDropPage(PSPIFFS_VOLUME pfs, SPIFFS_PAGE_IX pageIX);
PSPIFFS_CACHE_PAGE  spiffsCachePageAllocateByFd(PSPIFFS_VOLUME pfs, PSPIFFS_FD pFd);
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFSCACHE_H_ */