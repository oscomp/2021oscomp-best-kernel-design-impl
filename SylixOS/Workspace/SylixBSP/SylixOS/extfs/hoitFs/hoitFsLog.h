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
** 描        述: 日志层实现
*********************************************************************************************************/

#ifndef SYLIXOS_EXTFS_HOITFS_HOITLOG_H_
#define SYLIXOS_EXTFS_HOITFS_HOITLOG_H_

#include "hoitType.h"

#define S_IFLOG     0xd000



/*********************************************************************************************************
  LOG文件相关函数
*********************************************************************************************************/
PHOIT_LOG_INFO              hoitLogInit(PHOIT_VOLUME pfs, UINT uiLogSize, UINT uiSectorNum);
PHOIT_LOG_INFO              hoitLogOpen(PHOIT_VOLUME pfs, PHOIT_RAW_LOG pRawLog);
PCHAR                       hoitLogEntityGet(PHOIT_VOLUME pfs, UINT uiEntityNum);
INT                         hoitLogAppend(PHOIT_VOLUME pfs, PCHAR pcEntityContent, UINT uiEntitySize);

#ifdef LOG_TEST
VOID                        hoitLogTest();
#endif // LOG_TEST
#endif /* SYLIXOS_EXTFS_HOITFS_HOITLOG_H_ */
