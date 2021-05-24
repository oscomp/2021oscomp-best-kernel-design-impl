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
** 文   件   名: fake_nor.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 17 日
**
** 描        述: RAM模拟NorFlash驱动，相关数据定义
*********************************************************************************************************/

#ifndef SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_
#define SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_

#include "Am29LV160DB.h"
#include <stdio.h>
#include <stdlib.h>


#define NOR_FLASH_MAX_ERASE_CNT                 1000
/*********************************************************************************************************
  记录每个Sector的擦写次数
*********************************************************************************************************/
typedef struct sector_info
{
    INT32 erase_cnt;
    BOOL  is_bad;
} sector_info_t;

sector_info_t* sector_infos;


VOID nor_summary();
VOID generate_bad_sector();
VOID assign_sector_bad(INT sector_no);
BOOL get_sector_is_bad(INT sector_no);
#endif /* SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_ */
