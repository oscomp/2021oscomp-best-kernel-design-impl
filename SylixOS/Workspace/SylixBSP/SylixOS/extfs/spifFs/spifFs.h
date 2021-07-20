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
** 文   件   名: spifFs.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统接口层
*********************************************************************************************************/

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFS_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFS_H_

#include "SylixOS.h"
#include "spifFsType.h"
#include "spifFsGlue.h"
#include "../../driver/mtd/nor/nor.h"
/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0// && LW_CFG_RAMFS_EN > 0
/*********************************************************************************************************
  API
*********************************************************************************************************/
LW_API INT          API_SpifFsDrvInstall(VOID);
LW_API INT          API_SpifFsDevCreate(PCHAR   pcName, PLW_BLK_DEV  pblkd);
LW_API INT          API_SpifFsDevDelete(PCHAR   pcName);

#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0 */
#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFS_H_ */
