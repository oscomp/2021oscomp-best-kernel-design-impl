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
** 描        述: RAM模拟NorFlash驱动
*********************************************************************************************************/

#ifndef SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_
#define SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_


#include <stdio.h>
#include <stdlib.h>
#include "SST39VF1601.h"


/*********************************************************************************************************
  记录每个Sector的擦写次数
*********************************************************************************************************/
typedef struct sector_info
{
    INT32 erase_cnt;
    BOOL  is_bad;
} sector_info_t;

sector_info_t* sector_infos;
/*********************************************************************************************************
  模拟nor_flash
*********************************************************************************************************/
PCHAR nor_flash_base_addr;
#define GET_NOR_FLASH_ADDR(offset)            (nor_flash_base_addr + ((UINT)offset))                        /* 根据片内偏移获取真实地址 */
#define GET_NOR_FLASH_BLK_ADDR(block_no)      GET_NOR_FLASH_ADDR(((block_no) * NOR_FLASH_BLKSZ))                
#define GET_NOR_FLASH_SECTOR_ADDR(sector_no)  GET_NOR_FLASH_ADDR(((sector_no) * NOR_FLASH_SECTORSZ))     
#define GET_NOR_FLASH_SECTOR(offset)          (((UINT)offset) / NOR_FLASH_SECTORSZ)                         /* 根据片内地址获取SECTOR号（0 ~ 511） */
#define GET_NOR_FLASH_BLK(offset)             (GET_NOR_FLASH_SECTOR((offset)) / NOR_FLASH_SECTPBLK)         /* 根据片内地址获取BLOCK号（0 ~ 31） */

/*********************************************************************************************************
  擦除选项
*********************************************************************************************************/
typedef enum erase_options
{
    ERASE_CHIP,
    ERASE_BLOCK,
    ERASE_SECTOR
} ERASE_OPTIONS;

/*********************************************************************************************************
  读取选项
*********************************************************************************************************/
typedef enum read_options
{
    READ_SECTOR,
    READ_BYTE
} READ_OPTIONS;

typedef struct read_content
{
    PCHAR content;
    UINT  size;
    BOOL  is_success;
} read_content_t;

/*********************************************************************************************************
  SST39VF1601 方法定义
*********************************************************************************************************/
VOID    nor_init(VOID);
VOID    scan_nor(VOID);
VOID    erase_nor(UINT offset, ERASE_OPTIONS ops);
VOID    write_nor(UINT offset, PCHAR content, UINT size);
read_content_t   read_nor(UINT offset, READ_OPTIONS ops);
VOID    reset_nor(VOID);

#endif /* SYLIXOS_DRIVER_MTD_NOR_FAKE_NOR_H_ */
