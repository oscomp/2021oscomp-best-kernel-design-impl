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
** 文   件   名: hoitFsGC.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 04 月 25 日
**
** 描        述: 垃圾回收实现
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_HOITFS_HOITGC_H_
#define SYLIXOS_EXTFS_HOITFS_HOITGC_H_

#include "hoitType.h"

#define MAX_MSG_COUNTER     2
#define MAX_MSG_BYTE_SIZE   40

#define MSG_GC_END              "msg_gc_end"
#define MSG_BG_GC_END           "msg_gc_background_end"

/*********************************************************************************************************
  GC参数结构体
*********************************************************************************************************/
typedef struct hoitGCAttr
{
    PHOIT_VOLUME pfs; 
    UINT uiThreshold;
} HOIT_GC_ATTR;

typedef HOIT_GC_ATTR * PHOIT_GC_ATTR;

typedef enum hoitGCLevel
{
    GC_FOREGROUND,
    GC_BACKGROUND
} ENUM_HOIT_GC_LEVEL;

//TODO:注意 当删除RawInfo的时候，一定要记得调整它属于的GC_Sector的属性内容
//!该部分已经被修改为上层不再删除RawInfo，仅仅对其予以标记，删除步骤下层完成
/*********************************************************************************************************
  GC相关函数
*********************************************************************************************************/
VOID    hoitGCBackgroundThread(PHOIT_VOLUME pfs);
VOID    hoitGCForegroundForce(PHOIT_VOLUME pfs);
VOID    hoitGCThread(PHOIT_GC_ATTR pGCAttr);
VOID    hoitGCClose(PHOIT_VOLUME pfs);

/*********************************************************************************************************
  GC初始化构造函数
*********************************************************************************************************/
static inline VOID hoitStartGCThread(PHOIT_VOLUME pfs, UINT uiThreshold){
    LW_CLASS_THREADATTR     gcThreadAttr;
    PHOIT_GC_ATTR           pGCAttr;

    pGCAttr                 = (PHOIT_GC_ATTR)lib_malloc(sizeof(HOIT_GC_ATTR));
    pGCAttr->pfs            = pfs;
    pGCAttr->uiThreshold    = uiThreshold;
    
    API_ThreadAttrBuild(&gcThreadAttr,
                        4 * LW_CFG_KB_SIZE, 
                        LW_PRIO_NORMAL,
                        LW_OPTION_THREAD_STK_CHK, 
                        (VOID *)pGCAttr);

    pfs->HOITFS_hGCThreadId = API_ThreadCreate("t_hoit_gc_thread",
                                               (PTHREAD_START_ROUTINE)hoitGCThread,
                                               &gcThreadAttr,
                                               LW_NULL);
}

#endif /* SYLIXOS_EXTFS_HOITFS_HOITGC_H_ */
