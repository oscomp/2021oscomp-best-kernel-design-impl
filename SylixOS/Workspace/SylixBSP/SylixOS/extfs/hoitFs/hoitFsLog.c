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
** 文   件   名: hoitFsLog.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 04 月 25 日
**
** 描        述: 日志层实现
*********************************************************************************************************/
#include "hoitFsLog.h"
#include "hoitFsLib.h"
#include "hoitFsCache.h"
/*********************************************************************************************************
** 函数名称: __hoitInsertLogSector
** 功能描述: 向pLogSectorList插入一个新的LogSector节点
** 输　入  : pfs            HoitFS设备头
**          pLogSectorNew   新的节点
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitInsertLogSectorList(PHOIT_VOLUME pfs, PHOIT_LOG_SECTOR pLogSectorNew){
    PHOIT_LOG_SECTOR    pLogSectorTraverse;
    PHOIT_LOG_SECTOR    pLogSectorTrailling;

    if(pfs->HOITFS_erasableSectorList == LW_NULL){
        pfs->HOITFS_erasableSectorList = pLogSectorNew;
        return;
    }

    pLogSectorTraverse = pfs->HOITFS_logInfo->pLogSectorList;

    while (pLogSectorTraverse)
    {
        pLogSectorTrailling = pLogSectorTraverse;
        pLogSectorTraverse = pLogSectorTraverse->pErasableNextLogSector;
    }
    pLogSectorTrailling->pErasableNextLogSector = pLogSectorNew;
}
/*********************************************************************************************************
** 函数名称: __hoitDeleteLogSectorList
** 功能描述: 向pLogSectorList删除一个的LogSector节点
** 输　入  : pfs            HoitFS设备头
**          pLogSector      待删除节点
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitDeleteLogSectorList(PHOIT_VOLUME pfs, PHOIT_LOG_SECTOR pLogSector){
    PHOIT_LOG_SECTOR    pLogSectorTraverse;
    PHOIT_LOG_SECTOR    pLogSectorTrailling;

    if(pfs->HOITFS_logInfo->pLogSectorList == pLogSector){
        pfs->HOITFS_logInfo->pLogSectorList = pLogSector->pErasableNextLogSector;
        lib_free(pLogSector);
        return;
    }

    pLogSectorTraverse = pfs->HOITFS_logInfo->pLogSectorList;
    

    while (pLogSectorTraverse)
    {
        pLogSectorTrailling = pLogSectorTraverse;
        pLogSectorTraverse = pLogSectorTraverse->pErasableNextLogSector;
        
        if(pLogSectorTraverse == pLogSector){
            pLogSectorTrailling->pErasableNextLogSector = pLogSector->pErasableNextLogSector;
            lib_free(pLogSector);
            break;
        }
    }
}

/*********************************************************************************************************
** 函数名称: __hoitLogSectorCleanUp
** 功能描述: 标记LOG Sector为过期
** 输　入  : pfs            HoitFS设备头
**           pLogSector     待标记的Setcor
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitLogSectorCleanUp(PHOIT_VOLUME pfs, PHOIT_LOG_SECTOR pLogSector){
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    PHOIT_RAW_INFO          pRawInfoTraverse;

    pErasableSector  =  pLogSector->pErasableSetcor;
    pRawInfoTraverse =  pErasableSector->HOITS_pRawInfoFirst;

    while (LW_TRUE)
    {
        __hoit_del_raw_data(pfs, pRawInfoTraverse);
        pRawInfoTraverse->is_obsolete = 1;
        
        if(pRawInfoTraverse == pErasableSector->HOITS_pRawInfoLast){
            break;
        }
        pRawInfoTraverse = pRawInfoTraverse->next_phys;
    }

    __hoitDeleteLogSectorList(pfs, pLogSector);
}

/*********************************************************************************************************
** 函数名称: __hoitLogHdrCleanUp
** 功能描述: 标记LOG Hdr为过期
** 输　入  : pfs            HoitFS设备头
**           pLogInfo     LOG 文件头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitLogHdrCleanUp(PHOIT_VOLUME pfs, PHOIT_LOG_INFO pLogInfo){
    UINT                    uiRawLogHdrAddr;
    UINT                    uiSectorNum;
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    
    PHOIT_RAW_INFO          pRawInfoTraverse;

    
    uiRawLogHdrAddr = pLogInfo->uiRawLogHdrAddr;
    
    uiSectorNum     = hoitGetSectorNo(uiRawLogHdrAddr);
    pErasableSector = hoitFindSector(pfs->HOITFS_cacheHdr, uiSectorNum);

    pRawInfoTraverse = pErasableSector->HOITS_pRawInfoFirst;
    while (pRawInfoTraverse)
    {
        if(pRawInfoTraverse->phys_addr == uiRawLogHdrAddr){
            __hoit_del_raw_data(pfs, pRawInfoTraverse);
            pRawInfoTraverse->is_obsolete = 1;
            return;
        }
        pRawInfoTraverse = pRawInfoTraverse->next_phys;
    }
#ifdef DEBUG_LOG
    printf("[%s] LOG Hdr not find\n", __func__);
#endif // DEBUG_LOG
}
/*********************************************************************************************************
** 函数名称: __hoitScanLogSector
** 功能描述: 扫描Log Sector，记录相关信息
** 输　入  : pfs                    HoitFS设备头
**          pRawLog                 Flash上的pRawLog数据实体
**          pErasableLogSector      当前Log Sector 
**          puiEntityNum            记录实体数量
** 输　出  : 写偏移
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT __hoitScanLogSector(PHOIT_VOLUME pfs, PHOIT_RAW_LOG pRawLogHdr, PHOIT_ERASABLE_SECTOR pErasableLogSector , UINT * puiEntityNum){
    PCHAR                   pcLogSector;
    UINT                    uiSectorNum;
    UINT                    uiSectorSize;
    UINT                    uiSectorAddr;
    UINT                    uiOfs;
    PCHAR                   pcCurSectorPos;
    PHOIT_RAW_HEADER        pRawHeader;
    PHOIT_RAW_INFO          pRawInfo;

    uiSectorNum     = hoitGetSectorNo(pRawLogHdr->uiLogFirstAddr);
    uiSectorSize    = hoitGetSectorSize(uiSectorNum);
    uiSectorAddr    = pRawLogHdr->uiLogFirstAddr;
    uiOfs           = 0;

    pcLogSector = (PCHAR)lib_malloc(uiSectorSize);
    hoitReadFromCache(pfs->HOITFS_cacheHdr, pRawLogHdr->uiLogFirstAddr, pcLogSector, uiSectorSize);
    pcCurSectorPos = pcLogSector;
    
    while (pcCurSectorPos < pcLogSector + uiSectorSize) {
        PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)pcCurSectorPos;
        if (pRawHeader->magic_num == HOIT_MAGIC_NUM 
            && __HOIT_IS_TYPE_LOG(pRawHeader)) {
            
            /* 将初始的pRawLog对应的RawInfo加入到 LOG SECTOR 中 */
            pRawInfo                = (PHOIT_RAW_INFO)lib_malloc(sizeof(HOIT_RAW_INFO));
            pRawInfo->phys_addr     = uiSectorAddr + (pcCurSectorPos - pcLogSector);
            pRawInfo->totlen        = pRawHeader->totlen;
            pRawInfo->is_obsolete   = 0;
            pRawInfo->next_logic    = LW_NULL;
            pRawInfo->next_phys     = LW_NULL;
            __hoit_add_raw_info_to_sector(pErasableLogSector, pRawInfo); 
            
            pcCurSectorPos += __HOIT_MIN_4_TIMES(pRawHeader->totlen);
            uiOfs = (pcCurSectorPos - pcLogSector); 
            *puiEntityNum = *puiEntityNum + 1;
        }
        else {
            pcCurSectorPos += 4;   /* 每次移动4字节 */
        }
    }
    lib_free(pcLogSector);
    return uiOfs;
}
/*********************************************************************************************************
** 函数名称: __hoitFindAvailableSector
** 功能描述: 寻找一个全空的Sector
** 输　入  : pfs            HoitFS设备头
** 输　出  : 一个全空的Sector
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_ERASABLE_SECTOR hoitFindAvailableSector(PHOIT_VOLUME pfs){
    UINT                    uiAvaiSectorNum;
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    
    uiAvaiSectorNum = hoitFindNextToWrite(pfs->HOITFS_cacheHdr, HOIT_CACHE_TYPE_DATA_EMPTY, LW_NULL);
    if(uiAvaiSectorNum == PX_ERROR){
        return LW_NULL;
    }
#ifdef DEBUG_LOG
    printf("[%s] sector %d will be our log sector \n", __func__ ,uiAvaiSectorNum);
#endif // DEBUG_LOG
    pErasableSector = hoitFindSector(pfs->HOITFS_cacheHdr, uiAvaiSectorNum);
    return pErasableSector;
}
/*********************************************************************************************************
** 函数名称: hoitLogInit
** 功能描述: Build的时候，如果没有扫描到LOG，就调用InitLOG，初始化一个日志系统
** 输　入  : pfs            HoitFS设备头
**          uiLogSize       Log大小
**          uiSectorNum     Log Sector数量，暂时只支持1
** 输　出  : LOG文件信息
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_LOG_INFO hoitLogInit(PHOIT_VOLUME pfs, UINT uiLogSize, UINT uiSectorNum){
    PHOIT_RAW_INFO          pRawInfo;
    PHOIT_RAW_LOG           pRawLogHdr;
    PHOIT_LOG_INFO          pLogInfo;
    
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    PHOIT_LOG_SECTOR        pLogSector;

    UINT                    uiSectorAddr;

    if(uiLogSize == 0 || uiSectorNum == 0){
        return LW_NULL;
    }

    if(uiSectorNum > 1){
#ifdef DEBUG_LOG
        printf("[%s] only support one log sector for now \n", __func__);
#endif // DEBUG_LOG
        return LW_NULL;
    }
    
    pRawLogHdr            = (PHOIT_RAW_LOG)lib_malloc(sizeof(HOIT_RAW_LOG));
    lib_memset(pRawLogHdr, 0, sizeof(HOIT_RAW_LOG));
    pRawLogHdr->file_type = S_IFLOG;
    pRawLogHdr->magic_num = HOIT_MAGIC_NUM;
    pRawLogHdr->flag      = HOIT_FLAG_TYPE_LOG | HOIT_FLAG_OBSOLETE;
    pRawLogHdr->ino       = __hoit_alloc_ino(pfs);
    pRawLogHdr->totlen    = sizeof(HOIT_RAW_LOG);
    pRawLogHdr->version   = pfs->HOITFS_highest_version++;

    //TODO: 找到一个空的Sector作为LOG Sector
    pErasableSector    = hoitFindAvailableSector(pfs);
    if(pErasableSector == LW_NULL){
#ifdef DEBUG_LOG
        printf("[%s] can't find a sector for log\n", __func__);
#endif // DEBUG_LOG
        pfs->HOITFS_logInfo = LW_NULL;
        return LW_NULL;
    }
    pRawLogHdr->uiLogFirstAddr = pErasableSector->HOITS_addr;
    pRawLogHdr->uiLogSize = uiLogSize;

    uiSectorAddr = hoitWriteToCache(pfs->HOITFS_cacheHdr, (PCHAR)pRawLogHdr, pRawLogHdr->totlen);
    hoitFlushCache(pfs->HOITFS_cacheHdr);
    
    printf("[%s] our LOG HDR' version is %d, inode no is %d, log sector is %d\n", 
            __func__, pRawLogHdr->version, pRawLogHdr->ino, pErasableSector->HOITS_bno);

    /* 将初始的pRawLogHdr对应的RawInfo加入到管理中 */
    pRawInfo                = (PHOIT_RAW_INFO)lib_malloc(sizeof(HOIT_RAW_INFO));
    pRawInfo->phys_addr     = uiSectorAddr;
    pRawInfo->totlen        = pRawLogHdr->totlen;
    pRawInfo->is_obsolete   = 0;
    pRawInfo->next_logic    = LW_NULL;
    pRawInfo->next_phys     = LW_NULL;
    __hoit_add_raw_info_to_sector(pfs->HOITFS_now_sector, pRawInfo); 

    pLogInfo = (PHOIT_LOG_INFO)lib_malloc(sizeof(HOIT_LOG_INFO));
    lib_memset(pLogInfo, 0, sizeof(HOIT_LOG_INFO));
    
    pLogSector = (PHOIT_LOG_SECTOR)lib_malloc(sizeof(HOIT_LOG_SECTOR));
    lib_memset(pLogSector, 0, sizeof(HOIT_LOG_SECTOR));
    pLogSector->pErasableSetcor = pErasableSector;
    
    pLogSector->pErasableNextLogSector = LW_NULL;

    pLogInfo->pLogSectorList           = pLogSector;
    pLogInfo->uiLogCurAddr             = pRawLogHdr->uiLogFirstAddr;
    pLogInfo->uiLogCurOfs              = 0;
    pLogInfo->uiLogEntityCnt           = 0;
    pLogInfo->uiLogSize                = uiLogSize;
    pLogInfo->uiRawLogHdrAddr          = uiSectorAddr;
    
    if(pfs->HOITFS_logInfo != LW_NULL){
        lib_free(pfs->HOITFS_logInfo);
        pfs->HOITFS_logInfo = LW_NULL;
    }

    pfs->HOITFS_logInfo                = pLogInfo;
    return pLogInfo;
}

/*********************************************************************************************************
** 函数名称: hoitLogOpen
** 功能描述: Build的时候，如果扫描到 有效 LOG，就调用hoitLogOpen，从而初始化LOG系统
** 输　入  : pfs            HoitFS设备头
**          pRawLog        Flash上的RAW_LOG信息 
** 输　出  : LOG文件信息
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_LOG_INFO hoitLogOpen(PHOIT_VOLUME pfs, PHOIT_RAW_LOG pRawLogHdr){
    PHOIT_LOG_INFO          pLogInfo;

    PHOIT_LOG_SECTOR        pLogSector;
    PHOIT_ERASABLE_SECTOR   pErasableLogSector;
    PHOIT_ERASABLE_SECTOR   pErasableSectorTraverse;

    UINT                    uiEntityCnt;
    UINT                    uiLogCurOfs;
    
    uiEntityCnt = 0;

    pLogInfo = (PHOIT_LOG_INFO)lib_malloc(sizeof(HOIT_LOG_INFO));
    lib_memset(pLogInfo, 0, sizeof(HOIT_LOG_INFO));

    pLogSector = (PHOIT_LOG_SECTOR)lib_malloc(sizeof(HOIT_LOG_SECTOR));
    lib_memset(pLogInfo, 0, sizeof(HOIT_LOG_SECTOR));
    
    pErasableLogSector = LW_NULL;
    pErasableSectorTraverse = pfs->HOITFS_erasableSectorList;
    while (pErasableSectorTraverse)
    {
        if(pErasableSectorTraverse->HOITS_addr == pRawLogHdr->uiLogFirstAddr){
            pErasableLogSector = pErasableSectorTraverse;
            break;
        }
    }
    if(pErasableLogSector == LW_NULL){
#ifdef DEBUG_LOG
        printf("[%s] not found log setcor\n", __func__);
#endif // DEBUG_LOG
        return LW_NULL;
    }
    uiLogCurOfs = __hoitScanLogSector(pfs, pRawLogHdr, pErasableLogSector,  &uiEntityCnt);

    pLogSector->pErasableNextLogSector = LW_NULL;
    pLogSector->pErasableSetcor = pErasableLogSector;

    pLogInfo->pLogSectorList = pLogSector;
    pLogInfo->uiLogCurAddr   = pRawLogHdr->uiLogFirstAddr;
    pLogInfo->uiLogCurOfs    = uiLogCurOfs;
    pLogInfo->uiLogSize      = pRawLogHdr->uiLogSize;
    pLogInfo->uiLogEntityCnt = uiEntityCnt;
    
    pfs->HOITFS_logInfo = pLogInfo;
    return pLogInfo;
}
/*********************************************************************************************************
** 函数名称: hoitLogRead
** 功能描述: 读取Log Sector上的信息
** 输　入  : pfs            HoitFS设备头
**          uiEntityNum      该Sector上第uiEntityNum个实体的信息
** 输　出  :返回RAW Header指向地址，即跳过LOG Header
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PCHAR hoitLogEntityGet(PHOIT_VOLUME pfs, UINT uiEntityNum){
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    PHOIT_RAW_INFO          pRawInfoTraverse;

    UINT                    uiEntitySize;
    UINT                    uiEntityIndex;
    PCHAR                   pcEntity;
    
    if(uiEntityNum > pfs->HOITFS_logInfo->uiLogEntityCnt){
#ifdef DEBUG_LOG
        printf("[%s] uiEntityNum start from 0, Log has no enough entities to get\n",__func__);
#endif // DEBUG_LOG
        return PX_ERROR;
    }

    uiEntityIndex = 0;
    pErasableSector = pfs->HOITFS_logInfo->pLogSectorList->pErasableSetcor;     /* 获取Log Sector */
    pRawInfoTraverse = pErasableSector->HOITS_pRawInfoFirst;

    while (LW_TRUE)
    {
        if(uiEntityIndex == uiEntityNum){
            break;
        }
        uiEntityIndex++;
        pRawInfoTraverse = pRawInfoTraverse->next_phys;
    }
    
    uiEntitySize = pRawInfoTraverse->totlen - sizeof(HOIT_RAW_LOG);
    pcEntity = (PCHAR)lib_malloc(uiEntitySize);

    hoitReadFromCache(pfs->HOITFS_cacheHdr, pRawInfoTraverse->phys_addr + sizeof(HOIT_RAW_LOG),     /* 目标 Entity 所在RawInfo */
                      pcEntity, uiEntitySize);  
    
    return pcEntity;
}


/*********************************************************************************************************
** 函数名称: hoitLogAppend
** 功能描述: 向LOG Sector追加写入LOG
** 输　入  : pfs                HoitFS设备头
**          pcEntityContent     要写入Log的实体
**          uiEntitySize        实体大小 
** 输　出  : 追加错误类型
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT hoitLogAppend(PHOIT_VOLUME pfs, PCHAR pcEntityContent, UINT uiEntitySize){
    PHOIT_RAW_HEADER    pRawHeader;
    PHOIT_RAW_LOG       pRawLog;
    PHOIT_RAW_INFO      pRawInfo;

    PHOIT_LOG_INFO      pLogInfo;
    UINT                uiLogAddr;
    UINT                uiLogCurOfs;
    UINT                uiLogSize;
    UINT                uiLogRemainSize;
    
    UINT                uiSize;
    PCHAR               pcLogContent;

    if(pfs->HOITFS_logInfo == LW_NULL){
#ifdef DEBUG_LOG
        printf("[%s] no space for log sector\n", __func__);
#endif // DEBUG_LOG
        return (LOG_APPEND_NO_SECTOR);
    }

    if(uiEntitySize < sizeof(HOIT_RAW_HEADER)){
#ifdef DEBUG_LOG
        printf("[%s] cannot append something that not is Entity\n", __func__);
#endif // DEBUG_LOG
        return (LOG_APPEND_BAD_ENTITY);
    }

    pRawHeader = (PHOIT_RAW_HEADER)pcEntityContent;
    if(pRawHeader->magic_num != HOIT_MAGIC_NUM){
#ifdef DEBUG_LOG
        printf("[%s] cannot append something that not is Entity\n", __func__);
#endif // DEBUG_LOG
        return (LOG_APPEND_BAD_ENTITY);
    }


    pLogInfo                = pfs->HOITFS_logInfo;
    uiLogAddr               = pLogInfo->uiLogCurAddr;
    uiLogSize               = pLogInfo->uiLogSize;
    uiLogCurOfs             = pLogInfo->uiLogCurOfs;
    uiLogRemainSize         = uiLogSize - uiLogCurOfs;
    uiSize                  = uiEntitySize + sizeof(HOIT_RAW_LOG);
                                                            /* 声明一个LOG实体头 */                                                                        
    pRawLog                 = (PHOIT_RAW_LOG)lib_malloc(sizeof(HOIT_RAW_LOG));
    pRawLog->file_type      = S_IFLOG;
    pRawLog->flag           = HOIT_FLAG_TYPE_LOG | HOIT_FLAG_OBSOLETE;
    pRawLog->magic_num      = HOIT_MAGIC_NUM;
    pRawLog->totlen         = uiSize;
    pRawLog->uiLogFirstAddr = PX_ERROR;
    pRawLog->uiLogSize      = uiLogSize;
    pRawLog->version        = pfs->HOITFS_highest_version++;
    pRawLog->ino            = __hoit_alloc_ino(pfs);


    pcLogContent            = (PCHAR)lib_malloc(uiSize);
    lib_memcpy(pcLogContent, pRawLog, sizeof(HOIT_RAW_LOG));
    lib_memcpy(pcLogContent + sizeof(HOIT_RAW_LOG), pcEntityContent, uiEntitySize);

    pRawInfo                = (PHOIT_RAW_INFO)lib_malloc(sizeof(HOIT_RAW_INFO));    /* 建立RawInfo */
    pRawInfo->is_obsolete   = 0;
    pRawInfo->next_logic    = LW_NULL;
    pRawInfo->next_phys     = LW_NULL;
    pRawInfo->totlen        = uiSize;

    if(uiLogRemainSize < uiSize){                                           /* 当前Log Sector不够写，则清除当前Sector */
        __hoitLogSectorCleanUp(pfs, pfs->HOITFS_logInfo->pLogSectorList);   /* 清除LOG HDR指向的Sector的内容，全部标记为过期 */
        __hoitLogHdrCleanUp(pfs, pLogInfo);                                 /* 清除LOG HDR，并标记为过期 */
        if(hoitLogInit(pfs, uiLogSize, 1) == LW_NULL) {                     /* 新创一个LOG */
#ifdef DEBUG_LOG
            printf("[%s] log memory not enough\n", __func__);
#endif // DEBUG_LOG
            return (LOG_APPEND_NO_SECTOR);
        }

        uiLogAddr   = pfs->HOITFS_logInfo->uiLogCurAddr;                 
        uiLogCurOfs = pfs->HOITFS_logInfo->uiLogCurOfs;
    }

    pRawInfo->phys_addr     = uiLogAddr + uiLogCurOfs;
    
    __hoit_add_raw_info_to_sector(pfs->HOITFS_logInfo->pLogSectorList->pErasableSetcor, pRawInfo);      /* 将新写入LOG加入到Sector管理 */
    pRawHeader = (PHOIT_RAW_HEADER)pcLogContent;

    hoitWriteThroughCache(pfs->HOITFS_cacheHdr, uiLogAddr + uiLogCurOfs, pcLogContent, uiSize);
    hoitFlushCache(pfs->HOITFS_cacheHdr);
                                                                    /* 修改相应LOG参数 */
    pfs->HOITFS_logInfo->uiLogEntityCnt++;
    pfs->HOITFS_logInfo->uiLogCurOfs += uiSize;

    lib_free(pcLogContent);

    return (LOG_APPEND_OK);
}
/*********************************************************************************************************
** 函数名称: hoitLogCheckIfLog
** 功能描述: 查看某个Sector是否是Log Sector
** 输　入  : pfs                HoitFS设备头
**          pErasableSector     欲检查的块
** 输　出  : 是 LW_TRUE， 否 LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL hoitLogCheckIfLog(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector){
    PHOIT_LOG_SECTOR    pLogSectorTraverse;
    if(pfs->HOITFS_logInfo == LW_NULL){
        return LW_FALSE;
    }
    pLogSectorTraverse = pfs->HOITFS_logInfo->pLogSectorList;
    while (pLogSectorTraverse)
    {
        if(pLogSectorTraverse->pErasableSetcor == pErasableSector){
            return LW_TRUE;
        }
        pLogSectorTraverse = pLogSectorTraverse->pErasableNextLogSector;
    }
    return LW_FALSE;
}
