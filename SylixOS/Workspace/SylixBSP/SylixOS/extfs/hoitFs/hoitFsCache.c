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
** 文   件   名: hoitCache.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 04 月 02 日
**
** 描        述: 缓存层
*********************************************************************************************************/
#include "hoitFsCache.h"
#include "hoitFsGC.h"
#include "../../driver/mtd/nor/nor.h"

//TODO: 1. now_sector不能与LOG重叠，可是为啥还是重叠了？    DONE
//TODO: 2. 强制GC                                         DONE  
//TODO: 3. 注意无空间了就报错                              DONE 
//TODO: 4. 
/*********************************************************************************************************
 * 宏定义
*********************************************************************************************************/
#define __HOITFS_CACHE_LOCK(pcacheHdr)          API_SemaphoreMPend(pcacheHdr->HOITCACHE_hLock, \
                                                LW_OPTION_WAIT_INFINITE)
#define __HOITFS_CACHE_UNLOCK(pcacheHdr)        API_SemaphoreMPost(pcacheHdr->HOITCACHE_hLock)

/*********************************************************************************************************
                                           API 函数
*********************************************************************************************************/
/*********************************************************************************************************
 * 缓存层主体代码，用于缓存flash数据到内存
*********************************************************************************************************/

/*    
** 函数名称:    hoitEnableCache
** 功能描述:    初始化 hoitfs cache
** 输　入  :    uiCacheBlockSize       单个cache大小
**              uiCacheBlockNums       cache最大数量
**              phoitfs                hoitfs文件卷结构体
** 输　出  : LW_NULL 表示失败，PHOIT_CACHE_HDR地址 表示成功
** 全局变量:
** 调用模块:    
*/
PHOIT_CACHE_HDR hoitEnableCache(UINT32 uiCacheBlockSize, UINT32 uiCacheBlockNums, PHOIT_VOLUME phoitfs){
    PHOIT_CACHE_HDR pcacheHdr;

    pcacheHdr = (PHOIT_CACHE_HDR)__SHEAP_ALLOC(sizeof(HOIT_CACHE_HDR));
    if (pcacheHdr == LW_NULL) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (LW_NULL);        
    }

    lib_bzero(pcacheHdr,sizeof(HOIT_CACHE_HDR));

    pcacheHdr->HOITCACHE_hLock = API_SemaphoreMCreate("hoit_cache_lock", LW_PRIO_DEF_CEILING,
                                             LW_OPTION_WAIT_PRIORITY | LW_OPTION_DELETE_SAFE | 
                                             LW_OPTION_INHERIT_PRIORITY | LW_OPTION_OBJECT_GLOBAL,
                                             LW_NULL);

    if (!pcacheHdr->HOITCACHE_hLock) {
        __SHEAP_FREE(pcacheHdr);
        return  (LW_NULL);
    }

    pcacheHdr->HOITCACHE_cacheLineHdr    = (PHOIT_CACHE_BLK)__SHEAP_ALLOC(sizeof(HOIT_CACHE_BLK));
    if (pcacheHdr == LW_NULL) {
        __SHEAP_FREE(pcacheHdr);
        _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (LW_NULL);        
    }

    pcacheHdr->HOITCACHE_hoitfsVol      = phoitfs;
    pcacheHdr->HOITCACHE_blockNums      = 0;
    pcacheHdr->HOITCACHE_blockMaxNums   = uiCacheBlockNums;
    pcacheHdr->HOITCACHE_blockSize      = uiCacheBlockSize;
    pcacheHdr->HOITCACHE_flashBlkNum    = (NOR_FLASH_SZ - NOR_FLASH_START_OFFSET)/uiCacheBlockSize + 1;
    pcacheHdr->HOITCACHE_nextBlkToWrite = 0;
    
    /* 链表头也是一个HOIT_CACHE_BLK，不同在于有效位为0，且HOITBLK_buf为空，以此作为区别 */
    pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_bType            = HOIT_CACHE_TYPE_INVALID;
    pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext    = pcacheHdr->HOITCACHE_cacheLineHdr;
    pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListPrev    = pcacheHdr->HOITCACHE_cacheLineHdr;
    pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_buf              = LW_NULL;

    phoitfs->HOITFS_cacheHdr = pcacheHdr;

    hoitInitFilter(pcacheHdr,uiCacheBlockSize);

    return pcacheHdr;
}
/*    
** 函数名称:    hoitFreeCache
** 功能描述:    初始化 hoit cache
** 输　入  :    uiCacheBlockSize       单个cache大小
**              uiCacheBlockNums       cache最大数量
**              phoitfs                hoitfs文件卷结构体
** 输　出  : LW_NULL 表示失败，PHOIT_CACHE_HDR地址 表示成功
** 全局变量:
** 调用模块:    
*/
BOOL hoitFreeCache(PHOIT_CACHE_HDR pcacheHdr) {
    PHOIT_CACHE_BLK pcache;
    PHOIT_CACHE_BLK pfree;
    if (pcacheHdr == LW_NULL) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "hoitfs cache header is null.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (PX_ERROR);        
    }   

    pcache = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
    while (pcache != pcacheHdr->HOITCACHE_cacheLineHdr)
    {
        pcache->HOITBLK_cacheListPrev->HOITBLK_cacheListNext = pcache->HOITBLK_cacheListNext;   /* 断开pcache */
        pcache->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache->HOITBLK_cacheListPrev;
        pfree = pcache;
        pcache = pcache->HOITBLK_cacheListNext;
        __SHEAP_FREE(pfree->HOITBLK_buf);
        __SHEAP_FREE(pfree);        
    }
    __SHEAP_FREE(pcache->HOITBLK_buf);  /* 释放循环链表头 */
    __SHEAP_FREE(pcacheHdr);
    if(pcacheHdr->HOITCACHE_hLock){
        API_SemaphoreMDelete(&pcacheHdr->HOITCACHE_hLock);
    }
    return ERROR_NONE;
}

/*    
** 函数名称:    hoitAllocCache
** 功能描述:    分配cache,读取映射在flash的内容
** 输　入  :    pcacheHdr               cache头结构
**              flashBlkNo              分配cache映射的物理块号
**              cacheType               cache中的数据类型，暂时还未用到
**              pSector                 cache对应erasable_sector_list中的sector。
** 输　出  : LW_NULL 表示失败，成功时返回分配的cache指针
** 全局变量:
** 调用模块:    
*/
PHOIT_CACHE_BLK hoitAllocCache(PHOIT_CACHE_HDR pcacheHdr, UINT32 flashBlkNo, UINT32 cacheType, PHOIT_ERASABLE_SECTOR pSector) {
    INT             i;
    PHOIT_CACHE_BLK pcache;/* 当前创建的cache指针 */
    PHOIT_CACHE_BLK cacheLineHdr = pcacheHdr->HOITCACHE_cacheLineHdr; /* cache链表头指针 */
    size_t          cacheBlkSize = pcacheHdr->HOITCACHE_blockSize;
    BOOL            flag; /* 换块标记 */

    if (!pcacheHdr || !pSector) {
        return LW_NULL;
    }
    
    if (pcacheHdr->HOITCACHE_blockNums == pcacheHdr->HOITCACHE_blockMaxNums) { 
        /* cache分配数量已满，需要换块 */
        flag = LW_TRUE;
        //TOOPT 换块算法暂时采用FIFO，即替换链表头的上一个节点（链表尾）
        pcache = cacheLineHdr->HOITBLK_cacheListPrev;

        /* 从链表尾部断开 */
        cacheLineHdr->HOITBLK_cacheListPrev = pcache->HOITBLK_cacheListPrev;
        pcache->HOITBLK_cacheListPrev->HOITBLK_cacheListNext = pcache->HOITBLK_cacheListNext;

        /* 直接插入链表头部 */
        pcache->HOITBLK_cacheListPrev   = cacheLineHdr;
        pcache->HOITBLK_cacheListNext   = cacheLineHdr->HOITBLK_cacheListNext;
        cacheLineHdr->HOITBLK_cacheListNext->HOITBLK_cacheListPrev  = pcache;
        cacheLineHdr->HOITBLK_cacheListNext     = pcache;
    } else {
        flag = LW_FALSE;
        pcache = (PHOIT_CACHE_BLK)__SHEAP_ALLOC(sizeof(HOIT_CACHE_BLK));
        if (pcache == NULL) {
            _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
            return LW_NULL;
        }

        pcache->HOITBLK_buf = (PCHAR)__SHEAP_ALLOC(cacheBlkSize);
        if (pcache->HOITBLK_buf == NULL) {
            _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
            return LW_NULL;
        }

        /* 插入新分配的cache */
        pcache->HOITBLK_cacheListPrev   = cacheLineHdr;
        pcache->HOITBLK_cacheListNext   = cacheLineHdr->HOITBLK_cacheListNext;
        cacheLineHdr->HOITBLK_cacheListNext->HOITBLK_cacheListPrev  = pcache;
        cacheLineHdr->HOITBLK_cacheListNext     = pcache;

        pcacheHdr->HOITCACHE_blockNums++;
    }

    if (flag) { 
        /* 换了块要写回 */
        write_nor(pcache->HOITBLK_blkNo*pcacheHdr->HOITCACHE_blockSize + NOR_FLASH_START_OFFSET,
                    pcache->HOITBLK_buf, 
                    pcacheHdr->HOITCACHE_blockSize, 
                    WRITE_KEEP);
    }

    lib_bzero(pcache->HOITBLK_buf,cacheBlkSize);
    read_nor(flashBlkNo * cacheBlkSize + NOR_FLASH_START_OFFSET, pcache->HOITBLK_buf, cacheBlkSize);

    pcache->HOITBLK_bType           = cacheType;
    pcache->HOITBLK_sector          = pSector;
    pcache->HOITBLK_blkNo           = flashBlkNo;

    return pcache;
}
/*    
** 函数名称:    hoitCheckCacheHit
** 功能描述:    检测相应flashBlkNo是否命中
** 输　入  :    pcacheHdr               cache头结构
**              flashBlkNo              物理块号
** 输　出  : LW_NULL 表示失败，成功时返回内存中命中的cache指针
** 全局变量:
** 调用模块:    
*/
PHOIT_CACHE_BLK hoitCheckCacheHit(PHOIT_CACHE_HDR pcacheHdr, UINT32 flashBlkNo) {
    PHOIT_CACHE_BLK pcacheLineHdr = pcacheHdr->HOITCACHE_cacheLineHdr;
    PHOIT_CACHE_BLK pcache = pcacheLineHdr->HOITBLK_cacheListNext;
    
    while (pcache != pcacheLineHdr){
        if (pcache->HOITBLK_blkNo == flashBlkNo &&
            pcache->HOITBLK_bType != HOIT_CACHE_TYPE_INVALID) {
            return pcache;
        }
        pcache = pcache->HOITBLK_cacheListNext;
    }
    return LW_NULL;
}
/*    
** 函数名称:    hoitReadFromCache
** 功能描述:    读取flash数据，优先从内存中读取
** 输　入  :    pcacheHdr               cache头结构
**              uiOfs                   数据读取起始位置
**              pContent                目的地址
**              uiSize                  读取字节
** 输　出  : LW_FALSE 表示失败，LW_TRUE返回成功（暂时都是TRUE）
** 全局变量:
** 调用模块:    
*/
BOOL hoitReadFromCache(PHOIT_CACHE_HDR pcacheHdr, UINT32 uiOfs, PCHAR pContent, UINT32 uiSize){
    // read_nor(uiOfs, pContent, uiSize);
   PCHAR   pucDest         = pContent;
   size_t  cacheBlkSize    = pcacheHdr->HOITCACHE_blockSize;
   size_t  stStart         = uiOfs % cacheBlkSize;
   PHOIT_CACHE_BLK pcache;
   //UINT32  blkNoStart      = uiOfs/cacheBlkSize;
   //UINT32  blkNoEnd        = (uiOfs + uiSize) / cacheBlkSize;
   UINT32  readBytes      = 0;
   UINT32  i;

   while(uiSize != 0) {
       UINT32  stBufSize = (cacheBlkSize - stStart);
       i = (uiOfs + readBytes)/cacheBlkSize;
       if (stBufSize > uiSize) {
           pcache = hoitCheckCacheHit(pcacheHdr, i);
           if (!pcache) {
                read_nor(uiOfs + readBytes + NOR_FLASH_START_OFFSET, pucDest, uiSize);
           } else {
               /* 命中了 */
               lib_memcpy(pContent, pcache->HOITBLK_buf+stStart, uiSize);
           }
           readBytes   += uiSize;
           uiSize       = 0;
       } else {
           pcache = hoitCheckCacheHit(pcacheHdr, i);
           read_nor(uiOfs + readBytes + NOR_FLASH_START_OFFSET, pucDest, stBufSize);
           if (!pcache) {
                read_nor(uiOfs + readBytes + NOR_FLASH_START_OFFSET, pucDest, stBufSize);
           } else {
               /* 命中了 */
               lib_memcpy(pContent, pcache->HOITBLK_buf+stStart, uiSize);
           }
           pucDest     += stBufSize;
           uiSize      -= stBufSize;
           readBytes   += stBufSize;
           stStart      = 0;
       }
   }

    return LW_TRUE;
}
/*    
** 函数名称:    hoitWriteThroughCache
** 功能描述:    将一段给定长度的数据实体写入特定地址。
** 输　入  :    pcacheHdr               cache头结构
**              uiOfs                   数据写入起始位置，上层将flash介质中的NOR_FLASH_START_OFFSET位置视为0地址。
**              pContent                原地址
**              uiSize                  写入字节
** 输　出  : LW_FALSE 表示失败，LW_TRUE返回成功（暂时都是TRUE）
** 全局变量:
** 调用模块:    
*/
BOOL hoitWriteThroughCache(PHOIT_CACHE_HDR pcacheHdr, UINT32 uiOfs, PCHAR pContent, UINT32 uiSize){
    PCHAR   pucDest         = pContent;
    size_t  cacheBlkSize    = pcacheHdr->HOITCACHE_blockSize;
    size_t  stStart         = uiOfs % cacheBlkSize;
    UINT32  blkNoStart      = uiOfs/cacheBlkSize;
    UINT32  blkNoEnd        = (uiOfs + uiSize) / cacheBlkSize;
    UINT32  writeBytes      = 0;
    UINT32  i;                      /* 下一块要写入的flash块 */
    UINT32                  writeAddr = uiOfs + writeBytes + NOR_FLASH_START_OFFSET;
    PHOIT_CACHE_BLK         pcache;
    PHOIT_ERASABLE_SECTOR   pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;
    
    //! 22021-07-07 既然这个写函数主要用于标记过期，改成写不分配以及FIFO比较好
    while(uiSize != 0) {
        size_t  stBufSize = (cacheBlkSize - stStart);
        i = (uiOfs + writeBytes)/cacheBlkSize;

        pSector = hoitFindSector(pcacheHdr, i);/* 查找块号对应的pSector */

        writeAddr = uiOfs + writeBytes + NOR_FLASH_START_OFFSET;

        pcache = hoitCheckCacheHit(pcacheHdr, i);
        if (stBufSize > uiSize) { /* 不用写满整个sector */
            if (pcache == LW_NULL) { /* 未命中 */
                // pcache = hoitAllocCache(pcacheHdr, i, HOIT_CACHE_TYPE_DATA, pSector);
                // if (pcache == LW_NULL) { /* 未成功分配cache，直接写入flash */                      
                //     write_nor(writeAddr, pContent, uiSize, WRITE_KEEP);
                // }
                // else { /* 成功分配cache，则写入cache */
                //     lib_memcpy(pcache->HOITBLK_buf + stStart, pucDest, uiSize);
                // }
                write_nor(writeAddr, pContent, uiSize, WRITE_KEEP);
            } else {    /* cache命中则直接写入数据，并将链表提前到链表头 */
                lib_memcpy(pcache->HOITBLK_buf + stStart, pucDest, uiSize);
                //! 2021-07-04 ZN cache替换修改为LRU
                // pcache->HOITBLK_cacheListPrev->HOITBLK_cacheListNext = pcache->HOITBLK_cacheListNext;
                // pcache->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache->HOITBLK_cacheListPrev;
                
                // pcache->HOITBLK_cacheListPrev = pcacheHdr->HOITCACHE_cacheLineHdr;
                // pcache->HOITBLK_cacheListNext = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
                
                // pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache;
                // pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext = pcache;                
            }

            writeBytes += uiSize;
            if (stStart + uiSize > pSector->HOITS_offset) {
                /* 如果write through位置大于当前sector偏移，则需要修改 */
                pSector->HOITS_offset       = stStart + uiSize;
                pSector->HOITS_uiUsedSize   = stStart + uiSize;
                pSector->HOITS_uiFreeSize   = cacheBlkSize - (stStart + uiSize);
                pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_totalUsedSize += uiSize;
            }
            uiSize      = 0;
            break;
        } else { /* 写满整个sector */
            if (pcache == LW_NULL) { /* 未命中 */
                // pcache = hoitAllocCache(pcacheHdr, i, HOIT_CACHE_TYPE_DATA, pSector);
                // if (pcache == LW_NULL) { /* 未成功分配cache，直接写入flash */
                //     write_nor(writeAddr, pContent, stBufSize, WRITE_KEEP);
                // }
                // else { /* 成功分配cache，则写入cache */
                //     lib_memcpy(pcache->HOITBLK_buf + stStart, pucDest, stBufSize);
                // }
                write_nor(writeAddr, pContent, uiSize, WRITE_KEEP);
            } else {
                lib_memcpy(pcache->HOITBLK_buf + stStart, pucDest, stBufSize);
                //! 2021-07-04 ZN cache替换修改为LRU
                // pcache->HOITBLK_cacheListPrev->HOITBLK_cacheListNext = pcache->HOITBLK_cacheListNext;
                // pcache->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache->HOITBLK_cacheListPrev;
                
                // pcache->HOITBLK_cacheListPrev = pcacheHdr->HOITCACHE_cacheLineHdr;
                // pcache->HOITBLK_cacheListNext = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
                
                // pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache;
                // pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext = pcache;                 
            }

            pucDest     += stBufSize;
            uiSize      -= stBufSize;
            writeBytes  += stBufSize;
            stStart      = 0;

            pSector->HOITS_offset       = cacheBlkSize;
            pSector->HOITS_uiFreeSize   = 0;
            pSector->HOITS_uiUsedSize   = cacheBlkSize;
        }
    }
    // if(uiSize >= sizeof(HOIT_RAW_HEADER)){  /* 实体头 */
    //     PHOIT_RAW_HEADER headr = (PHOIT_RAW_HEADER)pContent;
    //     headr->magic_num == HOIT_MAGIC_NUM
    //     {
    //         pSector.usedCount++;               
    //     }
    // }
    return LW_TRUE;   
}
/*    
** 函数名称:    hoitUpdateEBS
** 功能描述:    在写入一个数据实体之后，调用该函数更新相应的EBS entry 
** 输　入  :    pcache                   cache块结构体
                inode                   数据实体对应的文件inode号
                pageNum                 数据实体所占页数
                offset                  数据实体写入的sector内位置
** 输　出  :    
** 全局变量:
** 调用模块:    
*/
UINT32 hoitUpdateEBS(PHOIT_CACHE_HDR pcacheHdr, PHOIT_CACHE_BLK pcache, UINT32 inode, UINT32 pageNum, UINT32 offset) {
    UINT32          startPageNo = offset/HOIT_FILTER_PAGE_SIZE;     /* 起始页号 */
    UINT32          i;
    PHOIT_EBS_ENTRY pentry;
    if(pcacheHdr==LW_NULL || pcache== LW_NULL) {
        return PX_ERROR;
    }

    pentry      = (PHOIT_EBS_ENTRY)(pcache->HOITBLK_buf + pcacheHdr->HOITCACHE_EBSStartAddr + (size_t)startPageNo * HOIT_FILTER_EBS_ENTRY_SIZE);

    // if ( (pentry+pageNum-1) > pcacheHdr->HOITCACHE_blockSize ) {    /* 越界检测 */
    //     return PX_ERROR;
    // }

    for(i=0 ; i<pageNum ; i++) {        /* 逐项更新EBS entry */
        pentry->HOIT_EBS_ENTRY_inodeNo  = inode;
        pentry->HOIT_EBS_ENTRY_obsolete = UINT_MAX;
        pentry ++;
    }
    return ERROR_NONE;
}
/*    
** 函数名称:    hoitWriteToCache
** 功能描述:    将一段给定长度的数据实体写入cache，cache自动查找可以装下该数据实体的位置进行写入
** 输　入  :    pcacheHdr               cache头结构
**              pContent                原地址
**              uiSize                  写入字节，最大不能超过一个cache块大小
** 输　出  : 成功时返回写入首地址（以NOR_FLASH_START_OFFSET为0地址），
**          失败返回PX_ERROR
** 全局变量:
** 调用模块:    
*/
UINT32 hoitWriteToCache(PHOIT_CACHE_HDR pcacheHdr, PCHAR pContent, UINT32 uiSize){
    PCHAR   pucDest         = pContent;
    UINT32  writeAddr       = 0;
    UINT32  i;
    UINT32  inode;          /* 数据实体所属文件inode号 */
    UINT32  pageNum;        /* 数据实体需要页数量 */
    PHOIT_CACHE_BLK         pcache;
    PHOIT_ERASABLE_SECTOR   pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;

    //! 添加头部检测
    if(uiSize < sizeof(HOIT_RAW_HEADER)){
        /* 不是数据实体 */
        return PX_ERROR;
    }
    if(((PHOIT_RAW_HEADER)pContent)->magic_num != HOIT_MAGIC_NUM){
        /* 不是数据实体 */
        return PX_ERROR;
    }

    if (pSector == LW_NULL) {
        return PX_ERROR;
    }
    
    if ((size_t)uiSize > pcacheHdr->HOITCACHE_blockSize ) {
        return PX_ERROR;
    }

    //! 2021-07-04 添加EBS处理
    inode       = ((PHOIT_RAW_HEADER)pContent)->ino;
    pageNum     = uiSize%HOIT_FILTER_PAGE_SIZE?uiSize/HOIT_FILTER_PAGE_SIZE:uiSize/HOIT_FILTER_PAGE_SIZE+1;

    //i = hoitFindNextToWrite(pcacheHdr, HOIT_CACHE_TYPE_DATA, uiSize);
    //! 设成页对齐
    i = hoitFindNextToWrite(pcacheHdr, HOIT_CACHE_TYPE_DATA, pageNum*HOIT_FILTER_PAGE_SIZE);

    if (i == PX_ERROR) {
        return PX_ERROR;
    } else {
        pSector = hoitFindSector(pcacheHdr, i);      
    }

    writeAddr = pSector->HOITS_bno * 
                pcacheHdr->HOITCACHE_blockSize + 
                pSector->HOITS_offset + 
                NOR_FLASH_START_OFFSET;         /* 确定数据实体写入的flash地址 */

    pcache = hoitCheckCacheHit(pcacheHdr, pSector->HOITS_bno);
    if (pcache == LW_NULL) {                                    /* 未命中 */
        pcache = hoitAllocCache(pcacheHdr, pSector->HOITS_bno, HOIT_CACHE_TYPE_DATA, pSector);
        if (pcache == LW_NULL) {                                /* 未成功分配cache，直接写入flash */                        
            write_nor(writeAddr, pContent, uiSize, WRITE_KEEP);
        }
        else {                                                  /* 成功分配cache，则写入cache，并将该cache块提到表头 */
            lib_memcpy(pcache->HOITBLK_buf + pSector->HOITS_offset, pucDest, uiSize);
        }
    } else {    /* cache命中则直接写入数据，并将链表提前到链表头 */
        lib_memcpy(pcache->HOITBLK_buf + pSector->HOITS_offset, pucDest, uiSize);
        //! 2021-07-04 ZN cache替换修改为LRU
        pcache->HOITBLK_cacheListPrev->HOITBLK_cacheListNext = pcache->HOITBLK_cacheListNext;
        pcache->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache->HOITBLK_cacheListPrev;
        
        pcache->HOITBLK_cacheListPrev = pcacheHdr->HOITCACHE_cacheLineHdr;
        pcache->HOITBLK_cacheListNext = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
        
        pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext->HOITBLK_cacheListPrev = pcache;
        pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext = pcache;

    }

    /* 更新EBS entry */
    hoitUpdateEBS(pcacheHdr, pcache, inode, pageNum, pSector->HOITS_offset);

    /* 更新HOITFS_now_sector */
    pSector->HOITS_offset       += uiSize;
    pSector->HOITS_uiFreeSize   -= uiSize;
    pSector->HOITS_uiUsedSize   += uiSize;
    pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_totalUsedSize += uiSize;

    /* 当前写的块满了，则去找下一个仍有空闲的块 */
    //! 减去EBS区域
    if (pSector->HOITS_uiFreeSize  == 0) {
        pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;
        i = hoitFindNextToWrite(pcacheHdr, HOIT_CACHE_TYPE_DATA, sizeof(HOIT_RAW_HEADER));
        if (i != PX_ERROR) {
            pSector = hoitFindSector(pcacheHdr, i);
        }
    }

    pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector = pSector;

    
    return writeAddr - NOR_FLASH_START_OFFSET;
}

/*

*/
VOID hoitCheckCacheList(PHOIT_CACHE_HDR pcacheHdr) {
    UINT32  i = 0;
    printf("======================  hoit cache check   ===========================\n");

    if (pcacheHdr->HOITCACHE_blockNums == 0) {
        printf("No block in cache\n");
        return ;
    } else {
        PHOIT_CACHE_BLK tempCache = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
        printf("Cache block Size:%d",pcacheHdr->HOITCACHE_blockSize);
        while (tempCache != LW_NULL) {
            i++;
            printf("Cache No.%d\n",i);
            printf("Cache Block Number: %d\n",tempCache->HOITBLK_blkNo);
            printf("Cache Free Size: %d\n",tempCache->HOITBLK_sector->HOITS_uiFreeSize);
        }
    }
    
}

/*    
** 函数名称:    hoitFlushCache
** 功能描述:    将所有cache数据写回flash
** 输　入  :    pcacheHdr               cache头结构
** 输　出  :    写回的cache块数量
** 全局变量:
** 调用模块:    
*/
UINT32 hoitFlushCache(PHOIT_CACHE_HDR pcacheHdr) {
    PHOIT_CACHE_BLK tempCache;
    UINT32  writeCount = 0;
    UINT32  writeAddr;
    if (pcacheHdr->HOITCACHE_blockNums == 0)
        return 0;
    
    tempCache = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
    while (tempCache != pcacheHdr->HOITCACHE_cacheLineHdr) {
        if (tempCache->HOITBLK_sector == LW_NULL)
            continue;
        writeAddr   = tempCache->HOITBLK_blkNo*pcacheHdr->HOITCACHE_blockSize + NOR_FLASH_START_OFFSET;
        write_nor(  writeAddr, 
                    tempCache->HOITBLK_buf, 
                    pcacheHdr->HOITCACHE_blockSize, 
                    WRITE_KEEP);

        tempCache = tempCache->HOITBLK_cacheListNext;
        writeCount ++;
    }

    return writeCount;
}
/*    
** 函数名称:    hoitReleaseCache
** 功能描述:    释放内存中的cache块
** 输　入  :    pcacheHdr               cache头结构
** 输　出  :    写回的cache块数量
** 全局变量:
** 调用模块:    
*/
BOOL hoitReleaseCache(PHOIT_CACHE_HDR pcacheHdr) {
    PHOIT_CACHE_BLK tempCache, tempCachePre;
    tempCache = pcacheHdr->HOITCACHE_cacheLineHdr->HOITBLK_cacheListNext;
    while (tempCache!= pcacheHdr->HOITCACHE_cacheLineHdr)
    {
        if (tempCache->HOITBLK_buf != LW_NULL) {
            __SHEAP_FREE(tempCache->HOITBLK_buf);
        }
        tempCachePre = tempCache;
        tempCache = tempCache->HOITBLK_cacheListNext;
        __SHEAP_FREE(tempCachePre);
    }
    return LW_TRUE;
}

/*
    释放cache头
*/
BOOL hoitReleaseCacheHDR(PHOIT_CACHE_HDR pcacheHdr) {
    if (pcacheHdr->HOITCACHE_cacheLineHdr != LW_NULL) {
        __SHEAP_FREE(pcacheHdr->HOITCACHE_cacheLineHdr);
    }
    API_SemaphoreMDelete(&pcacheHdr->HOITCACHE_hLock);
}
/*
    返回下一个要写的块，并更新PHOIT_CACHE_HDR中HOITCACHE_nextBlkToWrite(要写的下一块)
    pcacheHdr   cache头
    cacheType   块类型
    uiSize      块的剩余空间要求，只有cacheType == HOIT_CACHE_TYPE_DATA下才有意义。
                如果HOITFS_now_sector空间充足，则默认返回HOITFS_now_sector号
*/
UINT32 hoitFindNextToWrite(PHOIT_CACHE_HDR pcacheHdr, UINT32 cacheType, UINT32 uiSize) {
    //TOOPT: 2021-07-04 如果当前块写不下，是否可以先从cache中找能放得下数据实体的空闲块，再去整个sector列表中找？
    //! 2021-07-04 ZN 添加EBS区域，与uiSize比较时减少一个cache块可写空间。
    PHOIT_ERASABLE_SECTOR pSector;
    switch (cacheType)
    {
    case HOIT_CACHE_TYPE_INVALID:
        return (PX_ERROR);
    case HOIT_CACHE_TYPE_DATA:
        pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector;
        /* 如果当前块写不下，找下一块 */
        if (pSector->HOITS_uiFreeSize  < (size_t)uiSize) {
            pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;
        } else {
            return pSector->HOITS_bno;
        }

        while (pSector != LW_NULL) {
            if(!hoitLogCheckIfLog(pcacheHdr->HOITCACHE_hoitfsVol, pSector)                  /* 当不是LOG SECTOR*/
               && pSector != pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector){            /* 且不是NOW SECTOR时，才检查 */
                if(pSector->HOITS_uiFreeSize  >= (size_t)uiSize) {
                    return pSector->HOITS_bno;
                }
            }
            pSector = pSector->HOITS_next;
        }      
        if (pSector == LW_NULL) {                                   /* flash空间整体不足，开始执行强制GC */
            hoitGCForegroundForce(pcacheHdr->HOITCACHE_hoitfsVol);
        }

        /* GC之后重新找块 */
        pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;

        while (pSector != LW_NULL) {
            if(!hoitLogCheckIfLog(pcacheHdr->HOITCACHE_hoitfsVol, pSector)                  /* 当不是LOG SECTOR*/
               && pSector != pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector){            /* 且不是NOW SECTOR时，才检查 */
                if(pSector->HOITS_uiFreeSize  >= (size_t)uiSize) {
                    return pSector->HOITS_bno;
                }
            }
            pSector = pSector->HOITS_next;
        }
        /* 仍然没有可用块则返回错误 */
        return PX_ERROR;

    case HOIT_CACHE_TYPE_DATA_EMPTY:
        pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;
        while (pSector != LW_NULL)
        {
            if(!hoitLogCheckIfLog(pcacheHdr->HOITCACHE_hoitfsVol, pSector)                  /* 当不是LOG SECTOR*/
               && pSector != pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector){            /* 且不是NOW SECTOR时，才检查 */
                if(pSector->HOITS_uiFreeSize == pcacheHdr->HOITCACHE_blockSize) {
                    return pSector->HOITS_bno;
                }
            }
            pSector = pSector->HOITS_next;
        }
        /* 找不到，调用GC */
        if (pSector == LW_NULL) {
            hoitGCForegroundForce(pcacheHdr->HOITCACHE_hoitfsVol);
            while (pSector != LW_NULL)
            {
                //! 2021-05-04 Modified By PYQ
                if(!hoitLogCheckIfLog(pcacheHdr->HOITCACHE_hoitfsVol, pSector)                  /* 当不是LOG SECTOR*/
                && pSector != pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_now_sector){            /* 且不是NOW SECTOR时，才检查 */
                    if(pSector->HOITS_uiFreeSize == pcacheHdr->HOITCACHE_blockSize) {
                        return pSector->HOITS_bno;
                    }
                }
                pSector = pSector->HOITS_next;
            }            
        }
        return (PX_ERROR);
    //TODO 将来有了gc之后就不能单纯的将下一个写入块加一
    default:
        _ErrorHandle(ENOSYS);
        return  (PX_ERROR);
    }
    
}
/*
** 函数名称: hoitResetSectorState
** 功能描述: 重置一个Sector的状态， Added By PYQ
** 输　入  : pcacheHdr        缓存信息头部
**          pErasableSector   目标Sector
** 输　出  : None
** 全局变量:
** 调用模块:
*/
VOID hoitResetSectorState(PHOIT_CACHE_HDR pcacheHdr, PHOIT_ERASABLE_SECTOR pErasableSector){
    pErasableSector->HOITS_uiFreeSize = pErasableSector->HOITS_length;
    pErasableSector->HOITS_uiUsedSize = 0;
    pErasableSector->HOITS_offset     = 0;
}

                                     
/*    
** 函数名称:    hoitFindSector
** 功能描述:    根据sector号获取pSector指针
** 输　入  :    pcacheHdr               cache头结构
                sector_no               sector号
** 输　出  :    pSector指针，返回LW_NULL表示失败
** 全局变量:
** 调用模块:    
*/
PHOIT_ERASABLE_SECTOR hoitFindSector(PHOIT_CACHE_HDR pcacheHdr, UINT32 sector_no) {
    PHOIT_ERASABLE_SECTOR pSector;
    pSector = pcacheHdr->HOITCACHE_hoitfsVol->HOITFS_erasableSectorList;
    while (pSector != LW_NULL)
    {
        if (pSector->HOITS_bno == sector_no)
            break;
        pSector = pSector->HOITS_next;
    }
    return pSector;
}
//! 2021-07-04 ZN 创建过滤层
/*********************************************************************************************************
 * 过滤层主体代码，flash中的EBS区域对上层透明，在这一层对写入数据进行转换。
 * !注意事项：
 *      1. 每个sector的 free size 暂时在这层要减去一个EBS区域大小，这对上层是未知的
*********************************************************************************************************/

/*********************************************************************************************************
 * EBS设计
 * EBS满足式子：uiCacheBlockSize = (PageSize + EBSEntrySize) * PageAmount
 *  flash head                       Nor Flash                  flash end
 * --------------------------------------------------------------------------
 *                                                         |
 *                   data entry area                       |  EBS entry area
 *                                                         |
 * --------------------------------------------------------------------------
 * 设计如下：
 *          EBSEntrySize    = 8B
 *              EBSEntry前32位保存该页所属文件的inode号，全1表示该页未写入数据
 *              后32位为过期标记在inode号（前32位）不为全1时才有意义，全1时表示未过期，全0为过期。
 *          PageSize        = 56B
 *          PageAmount      =  64KB / 64B = 1K    
*********************************************************************************************************/ 
/*    
** 函数名称:    hoitInitFilter
** 功能描述:    初始化过滤层，计算EBS区域
** 输　入  :    pcacheHdr               cache头结构
                uiCacheBlockSize        单个cache大小，暂时为一个sector大小：64KB(设备为Am29LV160DB)
** 输　出  :    pSector指针，返回LW_NULL表示失败
** 全局变量:
** 调用模块:    
*/
UINT32 hoitInitFilter(PHOIT_CACHE_HDR pcacheHdr, UINT32 uiCacheBlockSize) {
    if (!pcacheHdr || uiCacheBlockSize < (HOIT_FILTER_EBS_ENTRY_SIZE+HOIT_FILTER_PAGE_SIZE)) {
        return PX_ERROR;
    }
    pcacheHdr->HOITCACHE_PageAmount     = uiCacheBlockSize/(HOIT_FILTER_EBS_ENTRY_SIZE+HOIT_FILTER_PAGE_SIZE);
    pcacheHdr->HOITCACHE_EBSStartAddr   = HOIT_FILTER_PAGE_SIZE * pcacheHdr->HOITCACHE_PageAmount;
    return ERROR_NONE;
}

#ifdef HOIT_CACHE_TEST
/*
    cache测试
*/
BOOL test_hoit_cache() {
    PHOIT_CACHE_HDR pcacheHdr;
    CHAR            data_write[8] = "1234567\0";
    CHAR            data_read[8];
    INT32           i;
    INT32           j=0;
    INT32           k=0;
    lib_memset(data_read,0,sizeof(data_read));
    printf("======================  hoit cache test   ============================\n");
    pcacheHdr = hoitEnableCache(64, 8, LW_NULL);

    // printf("1.common write read\n");
    // hoitWriteToCache(pcacheHdr, 0, data_write, sizeof(data_write));
    // hoitReadFromCache(pcacheHdr, 4, data_read, sizeof(data_read));
    // printf("result: %s\n",data_read);

    // printf("2.common flush\n");
    // hoitFlushCache(pcacheHdr);
    // lib_memset(data_read,0,sizeof(data_read));
    // read_nor((pcacheHdr->HOITCACHE_nextBlkToWrite-1)*pcacheHdr->HOITCACHE_blockSize+4+NOR_FLASH_START_OFFSET,data_read,sizeof(data_read));
    // printf("result: %s\n",data_read);

    printf("3.mutiple blocks write (no flush)\n");
    for (i=0 ; i < 64 ; i++) {
        k = i/8 + 1;
        j = j%8 + 1;
        data_write[0] = '0' + k;
        data_write[1] = '0' + j;
        hoitWriteThroughCache(pcacheHdr, i*sizeof(data_write), data_write, sizeof(data_write));
    }
    printf("current cache number: %d", pcacheHdr->HOITCACHE_blockNums);
    printf("\nread result:\n");
    for (i=0 ; i<64 ; i++) {
        hoitReadFromCache(pcacheHdr, i*sizeof(data_write), data_read, sizeof(data_read));
        printf("%s\n",data_read);
    }
    printf("\ncache data:\n");

    for (i=0 ; i<64 ; i++) {    /* 从0块开始读 */
        read_nor(   0+
                    i*sizeof(data_write)+
                    NOR_FLASH_START_OFFSET,
                    data_read,
                    sizeof(data_read));
        printf("%s\n",data_read);
    }
    printf("\nflushed cache data:\n");
    hoitFlushCache(pcacheHdr);
    for (i=0 ; i<64 ; i++) { /* 从0块开始读 */
        read_nor(   0+
                    i*sizeof(data_write)+
                    NOR_FLASH_START_OFFSET,
                    data_read,
                    sizeof(data_read));
        printf("%s\n",data_read);
    }

    printf("\nswap cache block:\n");
    data_write[0] = 'e';
    data_write[1] = 'n';
    data_write[2] = 'd';
    hoitWriteThroughCache(pcacheHdr, 64*sizeof(data_write), data_write, sizeof(data_write));
    hoitReadFromCache(pcacheHdr, 64*sizeof(data_write), data_read, sizeof(data_read));
    printf("%s\n",data_read);
    printf("======================  hoit cache test end  =========================\n");

    return LW_TRUE;
}
#endif
