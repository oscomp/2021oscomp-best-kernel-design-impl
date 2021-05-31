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
** 文   件   名: hoitFsGC.c
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 04 月 25 日
**
** 描        述: 垃圾回收实现
*********************************************************************************************************/
#include "hoitFsGC.h"
#include "hoitFsCache.h"
#include "hoitFsFDLib.h"
#include "hoitFsLib.h"

//TODO: GC时，不一定需要一个空快的支持，因为可能某个块中所有数据实体都过期，需要修改逻辑
//TODO: move_home返回错误值，表示不行了，GC抛出异常

#define IS_MSG_GC_END(acMsg, stLen)             lib_memcmp(acMsg, MSG_GC_END, stLen) == 0
#define IS_MSG_BG_GC_END(acMsg, stLen)          lib_memcmp(acMsg, MSG_BG_GC_END, stLen) == 0
#define KILL_LOOP()                             break
/*********************************************************************************************************
** 函数名称: __hoitGCSectorRawInfoFixUp
** 功能描述: 释放所有pErasableSector中的过期RawInfo，修改next_phys关系
** 输　入  : pErasableSector        目标擦除块
** 输　出  : 是否还需回收？
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitGCSectorRawInfoFixUp(PHOIT_ERASABLE_SECTOR pErasableSector){
    PHOIT_RAW_INFO          pRawInfoFirst;
    PHOIT_RAW_INFO          pRawInfoLast;
    
    PHOIT_RAW_INFO          pRawInfoTrailing;               /* 前一个指针 */
    PHOIT_RAW_INFO          pRawInfoTraverse;               /* 当前指针 */
    PHOIT_RAW_INFO          pRawInfoObselete;

    BOOL                    bIsReset;

    pRawInfoFirst       = LW_NULL;
    pRawInfoLast        = LW_NULL;

    pRawInfoTrailing    = LW_NULL;
    pRawInfoTraverse    = pErasableSector->HOITS_pRawInfoFirst;
    
    bIsReset = LW_FALSE;

    if((pErasableSector->HOITS_pRawInfoCurGC 
    && pErasableSector->HOITS_pRawInfoCurGC->is_obsolete == HOIT_FLAG_OBSOLETE) 
    || pErasableSector->HOITS_pRawInfoCurGC == LW_NULL){                /* 如果当前GC RawInfo过期，或还不存在当前GC RawInfo，则重新开始RawInfo的GC */
        bIsReset = LW_TRUE;
    }
    
    while (pRawInfoTraverse && pRawInfoTraverse->is_obsolete == HOIT_FLAG_OBSOLETE)           /* 寻找第一个非obselete的RawInfo，并释放已过期的RawInfo */
    {
        pRawInfoObselete = pRawInfoTraverse;
        pRawInfoTraverse = pRawInfoTraverse->next_phys;
        if(pRawInfoObselete == pErasableSector->HOITS_pRawInfoLast){    /* 全是空，直接返回咯 */
            lib_free(pRawInfoObselete);
            pErasableSector->HOITS_pRawInfoFirst    = LW_NULL;
            pErasableSector->HOITS_pRawInfoLast     = LW_NULL;
            pErasableSector->HOITS_pRawInfoCurGC    = LW_NULL;
            pErasableSector->HOITS_pRawInfoPrevGC   = LW_NULL;
            pErasableSector->HOITS_pRawInfoLastGC   = LW_NULL;
            return LW_FALSE;
        }
        lib_free(pRawInfoObselete);
    }

    pRawInfoFirst    = pRawInfoLast = pRawInfoTraverse;                 /* 设置RawInfo First与RawInfo Last */
    pRawInfoTrailing = pRawInfoTraverse;                    
    pRawInfoTraverse = pRawInfoTraverse->next_phys;
    
    while (LW_TRUE)
    {
        if(pRawInfoTrailing == pErasableSector->HOITS_pRawInfoLast
        || pRawInfoTraverse == LW_NULL){    /* 扫描完毕 */
            break;
        }
        if(pRawInfoTraverse->is_obsolete == HOIT_FLAG_OBSOLETE){                                      /* 如果过期 */
            pRawInfoObselete                    = pRawInfoTraverse;             
            pRawInfoTrailing->next_phys         = pRawInfoTraverse->next_phys;  /* 修改指针——前一块指向当前块的下一块 */
            pRawInfoTraverse                    = pRawInfoTraverse->next_phys;  /* 置当前块为下一块 */
            
            //!不能在GC过程中修改一个Sector的各种Size，否则会出现很严重的情况
            // pErasableSector->HOITS_uiUsedSize   -= pRawInfoObselete->totlen; 
            // pErasableSector->HOITS_uiFreeSize   += pRawInfoObselete->totlen; 
            
            lib_free(pRawInfoObselete);                                         /* 释放过期的块 */
        }
        else {
            pRawInfoLast                = pRawInfoTraverse;                     /* 更新pRawInfoLast */
            pRawInfoTrailing            = pRawInfoTraverse;              
            pRawInfoTraverse            = pRawInfoTraverse->next_phys;
        }
    }

    pErasableSector->HOITS_pRawInfoFirst    = pRawInfoFirst;
    pErasableSector->HOITS_pRawInfoLast     = pRawInfoLast;

    if(bIsReset){                                                   /* 初始化 Last GC， Info CurGC, Info Prev GC */
        pErasableSector->HOITS_pRawInfoCurGC    = LW_NULL;
        pErasableSector->HOITS_pRawInfoPrevGC   = LW_NULL;
        if(pErasableSector->HOITS_pRawInfoLastGC == LW_NULL){
            pErasableSector->HOITS_pRawInfoLastGC = (PHOIT_RAW_INFO)lib_malloc(sizeof(HOIT_RAW_INODE));
        }
        if(pRawInfoLast){
            lib_memcpy(pErasableSector->HOITS_pRawInfoLastGC, pRawInfoLast, sizeof(HOIT_RAW_INFO));
        }
        else {
            lib_free(pErasableSector->HOITS_pRawInfoLastGC);
            pErasableSector->HOITS_pRawInfoLastGC = LW_NULL;        /* 该Sector中没有Raw Info了…… */
        }
    }
    return LW_TRUE;
}
/*********************************************************************************************************
** 函数名称: __hoitGCFindErasableSector
** 功能描述: 根据HoitFS设备头中的信息，寻找一个可擦除数据块，目前寻找FreeSize最小的作为待GC
** 输　入  : pfs        HoitFS文件设备头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_ERASABLE_SECTOR __hoitGCFindErasableSector(PHOIT_VOLUME pfs, ENUM_HOIT_GC_LEVEL gcLevel){
    PHOIT_ERASABLE_SECTOR       pErasableVictimSector;
    PHOIT_ERASABLE_SECTOR       pErasableListTraverse;
    
    UINT                        uiMinVictimSan;            /* 最小受害者San值 */
    UINT                        uiVictimSan;               /* 受害者San值，越小越受害 */

    UINT                        uiFreeSize;
    UINT                        uiAge;

    pErasableVictimSector       = LW_NULL;
    uiMinVictimSan              = INT_MAX;

    pErasableListTraverse       = pfs->HOITFS_erasableSectorList;
    
    while (pErasableListTraverse)
    {
        if(hoitLogCheckIfLog(pfs, pErasableListTraverse)){          /* 不回收Log Sector中的任何文件 */
            pErasableListTraverse   = pErasableListTraverse->HOITS_next;
            continue;
        }
        uiFreeSize  = pErasableListTraverse->HOITS_uiFreeSize;   
#ifdef GC_TEST                                  
        if(pErasableListTraverse->HOITS_next == LW_NULL){           /* 如果最后一个Sector了 */
            pErasableListTraverse->HOITS_uiFreeSize -= 3;           /* 适配一下 */
            pErasableListTraverse->HOITS_uiUsedSize += 3;
        }
#endif // GC_TEST
        if(pErasableListTraverse->HOITS_uiUsedSize == 0){
            pErasableListTraverse   = pErasableListTraverse->HOITS_next;
            continue;
        }

        switch (gcLevel)
        {
        case GC_BACKGROUND:{
            uiAge       = API_TimeGet() - pErasableListTraverse->HOITS_tBornAge;
            uiVictimSan = (0.5 / uiAge) + 0.5 * uiFreeSize;  
        }
            break;
        case GC_FOREGROUND:{
            uiVictimSan = uiFreeSize;
        }
            break;
        default:
            break;
        }

        if(uiVictimSan < uiMinVictimSan){
            pErasableVictimSector   = pErasableListTraverse;
            uiMinVictimSan          = uiVictimSan;
        }

        pErasableListTraverse   = pErasableListTraverse->HOITS_next;
    }
    return pErasableVictimSector;
}
/*********************************************************************************************************
** 函数名称: __hoitGCCollectRawInfoAlive
** 功能描述: 从pErasableSector的pRawInfoCurGC处起，获取有效信息，一次获取一个
** 输　入  : pfs        HoitFS文件设备头
** 输　出  : 完成对该Sector的GC  LW_TRUE， 否则LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitGCCollectRawInfoAlive(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector, PHOIT_RAW_INFO pRawInfoCurGC){
    PHOIT_ERASABLE_SECTOR   pNowSectorOrigin;
    BOOL                    bIsMoveSuccess;

    if(pErasableSector == LW_NULL){
#ifdef GC_DEBUG
        printf("[%s] setcor can not be null\n", __func__);
#endif // GC_DEBUG
        return LW_TRUE;
    }
    
    pNowSectorOrigin = pfs->HOITFS_now_sector;              /* 保存原有now_sector */
    //!把RawInfo及其对应的数据实体搬家
    bIsMoveSuccess = __hoit_move_home(pfs, pRawInfoCurGC);  /* 搬家失败，说明该RawInfo要么是LOG要么是一段错误的数据，我们直接跳过 */
    pfs->HOITFS_now_sector = pNowSectorOrigin;              /* 恢复原有now_sector */

    return bIsMoveSuccess;
}

/*********************************************************************************************************
** 函数名称: __hoitGCCollectSectorAlive
** 功能描述: 从pErasableSector的pRawInfoCurGC处起，获取有效信息，一次获取一个，并修改相应指针
** 输　入  : pfs                HoitFS文件设备头
**          pErasableSector     
** 输　出  : 完成对该Sector的GC  LW_TRUE， 否则LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitGCCollectSectorAlive(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector){
    BOOL                    bIsCollectOver;
    BOOL                    bIsMoveSuccess;
    BOOL                    bIsNeedMoreCollect;
    INTREG                  iregInterLevel;

    PHOIT_RAW_INFO          pRawInfoNextGC;
    PHOIT_RAW_INFO          pRawInfoCurGC;
    PHOIT_RAW_INFO          pRawInfoPrevGC;

    bIsNeedMoreCollect = __hoitGCSectorRawInfoFixUp(pErasableSector);            /* FixUp后，会更新 HOITS_pRawInfoCurGC，HOITS_pRawInfoPrevGC，HOITS_pRawInfoLastGC等 */
    if(!bIsNeedMoreCollect){
        bIsCollectOver = LW_TRUE;
        goto __hoitGCCollectSectorAliveEnd;                                      /* 结束了 */
    }
#ifdef GC_DEBUG
    printf("[%s]: Fix over the Sector %d\n", __func__, pErasableSector->HOITS_bno);
#endif //GC_DEBUG
    pRawInfoCurGC   = LW_NULL;
    pRawInfoPrevGC  = LW_NULL;
    pRawInfoNextGC  = LW_NULL;
    bIsCollectOver  = LW_FALSE;
    bIsMoveSuccess  = LW_FALSE;
    
    pRawInfoCurGC   = pErasableSector->HOITS_pRawInfoCurGC;
    pRawInfoPrevGC  = pErasableSector->HOITS_pRawInfoPrevGC;

    if(pRawInfoCurGC == LW_NULL){
        pRawInfoCurGC = pErasableSector->HOITS_pRawInfoCurGC = pErasableSector->HOITS_pRawInfoFirst;
    }

    pRawInfoNextGC  = pRawInfoCurGC->next_phys;

#ifdef GC_DEBUG
    API_ThreadLock();
    printf("[%s]: GC sector %ld, GC raw info at %u\n", __func__, 
            pErasableSector->HOITS_bno, pRawInfoCurGC->phys_addr);
    API_ThreadUnlock();
#endif // GC_DEBUG

    if(pErasableSector->HOITS_pRawInfoLastGC == LW_NULL){                                  /* 不存在Last了 */
        bIsCollectOver = LW_TRUE;
    }

    if(pErasableSector->HOITS_pRawInfoLastGC 
    && pErasableSector->HOITS_pRawInfoLastGC->phys_addr == pRawInfoCurGC->phys_addr
    && pErasableSector->HOITS_pRawInfoLastGC->next_logic == pRawInfoCurGC->next_logic
    && pErasableSector->HOITS_pRawInfoLastGC->totlen == pRawInfoCurGC->totlen){             /* 不能比较next_phys和phys，会被修改之 */
        bIsCollectOver = LW_TRUE;
    }

    bIsMoveSuccess = __hoitGCCollectRawInfoAlive(pfs, pErasableSector, pRawInfoCurGC);


    if(bIsMoveSuccess){                                                  /* 移动成功 */
        if(pRawInfoCurGC == pErasableSector->HOITS_pRawInfoFirst){       /* 如果当前GC块是Sector的第一个RawInfo */
            pErasableSector->HOITS_pRawInfoPrevGC = LW_NULL;             /* Prev = LW_NULL */
            pErasableSector->HOITS_pRawInfoFirst  = pRawInfoNextGC;      /* 重置Sector的第一个RawInfo */
        }
        else {                                                           /* 如果不是第一个RawInfo */
            pRawInfoPrevGC->next_phys = pRawInfoNextGC;                  /* 让前一个RawInfo指向Cur的下一个 */
            if(bIsCollectOver){                                          /* 如果Cur是最后一个RawInfo */
                if(pfs->HOITFS_now_sector == pErasableSector){           /* 如果该RawInfo仍然被写到该Sector */
                    /* Do Nothing */
                }
                else {                                                   /* 如果该RawInfo被写到别的Sector */
                    pRawInfoPrevGC->next_phys = LW_NULL;                 /* 让Last指针指向前一个RawInfo即可 */
                    pErasableSector->HOITS_pRawInfoLast = pRawInfoPrevGC;
                }
            }
        }
    }
    else {                                                               /* 如果没有MOVE成功 */
        pErasableSector->HOITS_pRawInfoPrevGC = pRawInfoCurGC;           /* Prev就是当前的RawInfo */
    }

    pErasableSector->HOITS_pRawInfoCurGC  = pRawInfoNextGC;              /* 调整Cur指针 */

__hoitGCCollectSectorAliveEnd:    
    if(bIsCollectOver){
        pErasableSector->HOITS_pRawInfoCurGC  = LW_NULL;                  /* 当前Sector中GC的RawInfo为空 */
        pErasableSector->HOITS_pRawInfoPrevGC = LW_NULL;
#ifdef GC_DEBUG
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("[%s] Sector %d is collected Over, Total Moved %dKB to Survivor Sector %d\n", 
                __func__, pErasableSector->HOITS_bno, (pfs->HOITFS_curGCSuvivorSector->HOITS_uiUsedSize / 1024), 
                pfs->HOITFS_curGCSuvivorSector->HOITS_bno);
        API_TShellColorEnd(STD_OUT);
#endif
    }

    return bIsCollectOver;
}

/*********************************************************************************************************
** 函数名称: __hoitGCClearBackground
** 功能描述: HoitFS清除后台GC线程
** 输　入  : pfs        HoitFS文件设备头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitGCClearBackground(PHOIT_VOLUME pfs, BOOL * pBIsBackgroundThreadStart, LW_OBJECT_HANDLE hGcThreadId){
    if(*pBIsBackgroundThreadStart){
        API_MsgQueueSend(pfs->HOITFS_GCMsgQ, MSG_BG_GC_END, sizeof(MSG_BG_GC_END));
        API_ThreadWakeup(hGcThreadId);
        API_ThreadJoin(hGcThreadId, LW_NULL);
        *pBIsBackgroundThreadStart = LW_FALSE;
    }
}
/*********************************************************************************************************
** 函数名称: hoitFSGCForgroudForce
** 功能描述: HoitFS前台强制GC，Greedy算法，阻塞至回收完整个Sector
** 输　入  : pfs        HoitFS文件设备头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitGCForegroundForce(PHOIT_VOLUME pfs){
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    INTREG                  iregInterLevel;
    BOOL                    bIsCollectOver;
    
    pErasableSector = LW_NULL;
    
    if(pfs->HOITFS_curGCSector == LW_NULL && pfs->HOITFS_curGCSuvivorSector == LW_NULL) {
        pErasableSector                 = __hoitGCFindErasableSector(pfs, GC_FOREGROUND);      
        pfs->HOITFS_curGCSector         = pErasableSector;     
        pfs->HOITFS_curGCSuvivorSector  = hoitFindAvailableSector(pfs);
    }

    if(pErasableSector){
        LW_SPIN_LOCK_QUICK(&pErasableSector->HOITS_lock, &iregInterLevel);      /* 尝试加锁，阻塞整个回收 */
    }

    while (LW_TRUE)
    {
        if(pErasableSector) {
            bIsCollectOver = __hoitGCCollectSectorAlive(pfs, pErasableSector);
            if(bIsCollectOver){                                                  /*! 显示置空 */
                pfs->HOITFS_curGCSector        = LW_NULL;                        /* 当前GC的Sector为空 */
                pfs->HOITFS_curGCSuvivorSector = LW_NULL;                        /* 幸存者Sector为空 */
                hoitResetSectorState(pfs->HOITFS_cacheHdr, pErasableSector);     /* 重置该Sector状态，表明为空 */
                break;
            }
        }
        else {
#ifdef GC_DEBUG
            API_ThreadLock();
            printf("[%s]: there's no sector can be GCed\n", __func__);
            API_ThreadUnlock();
#endif // GC_DEBUG
            break;
        }
    }

    if(pErasableSector){
        LW_SPIN_UNLOCK_QUICK(&pErasableSector->HOITS_lock, iregInterLevel);              /* 尝试解锁 */
    }
}

/*********************************************************************************************************
** 函数名称: hoitFsGCBackgroundThread
** 功能描述: HoitFS后台GC线程，只阻塞回收一个实体
** 输　入  : pfs        HoitFS文件设备头
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitGCBackgroundThread(PHOIT_VOLUME pfs){
    INTREG                iregInterLevel;
    PHOIT_ERASABLE_SECTOR pErasableSector;
    BOOL                  bIsCollectOver;
    CHAR                  acMsg[MAX_MSG_BYTE_SIZE];
    size_t                stLen;

    while (LW_TRUE)
    {
        API_MsgQueueReceive(pfs->HOITFS_GCMsgQ, 
                            acMsg, 
                            sizeof(acMsg), 
                            &stLen, 
                            LW_OPTION_NOT_WAIT);
        if(IS_MSG_BG_GC_END(acMsg, stLen)){
            KILL_LOOP();
        }

        if(pfs->HOITFS_curGCSector == LW_NULL && pfs->HOITFS_curGCSuvivorSector == LW_NULL) {
            pErasableSector                 = __hoitGCFindErasableSector(pfs, GC_BACKGROUND);
            pfs->HOITFS_curGCSector         = pErasableSector;     
            pfs->HOITFS_curGCSuvivorSector  = hoitFindAvailableSector(pfs);
        }

        if(pErasableSector) {
            LW_SPIN_LOCK_QUICK(&pErasableSector->HOITS_lock, &iregInterLevel); 
            bIsCollectOver = __hoitGCCollectSectorAlive(pfs, pErasableSector);      /* 阻塞回收单一实体 */
            if(bIsCollectOver){
                pfs->HOITFS_curGCSector        = LW_NULL;                           /* 当前GC的Sector为空 */
                pfs->HOITFS_curGCSuvivorSector = LW_NULL;                           /* 幸存者Sector为空 */
                hoitResetSectorState(pfs->HOITFS_cacheHdr, pErasableSector);        /* 重置该Sector状态，表明为空 */
            }
            LW_SPIN_UNLOCK_QUICK(&pErasableSector->HOITS_lock, iregInterLevel);
        }
        else {
#ifdef GC_DEBUG
            API_ThreadLock();
            printf("[%s]: there's no sector can be GCed\n", __func__);
            API_ThreadUnlock();
#endif // GC_DEBUG
        }
        sleep(5);
    }
}


/*********************************************************************************************************
** 函数名称: hoitFsGCThread
** 功能描述: HoitFS GC监听线程，用于监听空间变化，从而判断此时该执行的GC方式
** 输　入  : pfs        HoitFS文件设备头
**          uiThreshold GC阈值，参考F2FS
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitGCThread(PHOIT_GC_ATTR pGCAttr){
    PHOIT_VOLUME                pfs; 
    UINT                        uiThreshold;
    UINT                        uiCurUsedSize;
    BOOL                        bIsBackgroundThreadStart;
    LW_CLASS_THREADATTR         gcThreadAttr;
    LW_OBJECT_HANDLE            hGcThreadId;
    CHAR                        acMsg[MAX_MSG_BYTE_SIZE];
    size_t                      stLen;

    bIsBackgroundThreadStart = LW_FALSE;
    pfs                      = pGCAttr->pfs;
    uiThreshold              = pGCAttr->uiThreshold;

    printf("\n\n[GC Thread Start...]\n\n");
    pfs->HOITFS_GCMsgQ = API_MsgQueueCreate("q_gc_thread_msgq", 
                                            MAX_MSG_COUNTER, 
                                            MAX_MSG_BYTE_SIZE, 
                                            LW_OPTION_WAIT_FIFO | LW_OPTION_OBJECT_LOCAL, 
                                            LW_NULL);
#ifdef GC_TEST
    uiCurUsedSize = 60;
#endif // GC_TEST

    while (LW_TRUE)
    {
        API_MsgQueueReceive(pfs->HOITFS_GCMsgQ, 
                            acMsg, 
                            sizeof(acMsg), 
                            &stLen, 
                            10);
        if(IS_MSG_GC_END(acMsg, stLen)){                                            /* 关闭GC监听线程  */
            printf("[%s] recev msg %s\n", __func__, MSG_GC_END);                
            __hoitGCClearBackground(pfs, &bIsBackgroundThreadStart, hGcThreadId);   /* 关闭后台GC线程 */
            KILL_LOOP();
        }
        
        uiCurUsedSize = pfs->HOITFS_totalUsedSize;
        if(uiCurUsedSize > uiThreshold){                                            /* 执行Foreground */
            //__hoitGCClearBackground(pfs, &bIsBackgroundThreadStart, hGcThreadId); /* 好像没必要终止GC后台线程，因为引入了锁机制，呵呵 */
            hoitGCForegroundForce(pfs);
        }
        else {
            if(!bIsBackgroundThreadStart 
                && uiCurUsedSize > (pfs->HOITFS_totalSize / 2)){                    /* 执行Background */
                
                bIsBackgroundThreadStart = LW_TRUE;
                
                API_ThreadAttrBuild(&gcThreadAttr, 
                                    4 * LW_CFG_KB_SIZE, 
                                    LW_PRIO_NORMAL,
                                    LW_OPTION_THREAD_STK_CHK, 
                                    (VOID *)pfs);

                hGcThreadId = API_ThreadCreate("t_gc_background_thread",
						                       (PTHREAD_START_ROUTINE)hoitGCBackgroundThread,
        						               &gcThreadAttr,
		        				               LW_NULL);
            }
        }
        sleep(5);
    }

}

/*********************************************************************************************************
** 函数名称: hoitGCClose
** 功能描述: 关闭 HoitFS GC线程
** 输　入  : pfs        HoitFS文件设备头
**          uiThreshold GC阈值，参考F2FS
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitGCClose(PHOIT_VOLUME pfs){
    if(pfs->HOITFS_hGCThreadId){
        API_MsgQueueSend(pfs->HOITFS_GCMsgQ, MSG_GC_END, sizeof(MSG_GC_END));
        API_MsgQueueShow(pfs->HOITFS_GCMsgQ);
        API_ThreadWakeup(pfs->HOITFS_hGCThreadId);                              /* 强制唤醒GC线程 */
        API_ThreadJoin(pfs->HOITFS_hGCThreadId, LW_NULL);
        API_MsgQueueDelete(&pfs->HOITFS_GCMsgQ);
        pfs->HOITFS_hGCThreadId = LW_NULL;
    }
    printf("================ Goodbye GC ================\n");
}

