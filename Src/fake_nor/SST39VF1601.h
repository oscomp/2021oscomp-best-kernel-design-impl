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
** 文件创建日期: 2021 年 02 月 24 日
**
** 描        述: RAM模拟NorFlash驱动 SST39VF1601 规格
*********************************************************************************************************/


#ifndef SYLIXOS_DRIVER_MTD_NOR_SST39VF1601_H_
#define SYLIXOS_DRIVER_MTD_NOR_SST39VF1601_H_

/*********************************************************************************************************
  SST39VF1601 配置规格
*********************************************************************************************************/
#define NOR_FLASH_NM                    "SST39VF1601 Power NOR"
#define NOR_FLASH_SZ                    (2 * 1024 * 1024)                  /*  Nor Flash Size 2MB */
#define NOR_FLASH_NBLK                  32
#define NOR_FLASH_BLKSZ                 (NOR_FLASH_SZ / NOR_FLASH_NBLK)
#define NOR_FLASH_SECTPBLK              16
#define NOR_FLASH_NSECTOR               (NOR_FLASH_NBLK * NOR_FLASH_SECTPBLK)               
#define NOR_FLASH_SECTORSZ              (4 * 1024)                         /* 4KB per sector */
#define NOR_FLASH_MAX_ERASE_CNT         1                                  /* 最大擦除次数 */

#endif /* SYLIXOS_DRIVER_MTD_NOR_SST39VF1601_H_ */
