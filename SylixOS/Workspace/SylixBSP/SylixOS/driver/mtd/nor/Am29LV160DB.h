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
** 文   件   名: nor_util.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 11 日
**
** 描        述: mini2440 norflash配置
*********************************************************************************************************/



#ifndef SYLIXOS_DRIVER_MTD_NOR_AM29LV160DB_H_
#define SYLIXOS_DRIVER_MTD_NOR_AM29LV160DB_H_

//#define NOR_TEST
//#define NOR_DEBUG

#include <stdio.h>

#define CFI_FLAG                                        "QRY"
/*********************************************************************************************************
  Mini2440 基本信息
*********************************************************************************************************/
#define NOR_FLASH_SZ                                    (2 * 1024 * 1024)
#define NOR_FALSH_NREGION                               4 
#define NOR_FLASH_NSECTOR                               35
#define NOR_FLASH_MANUID                                1
#define NOR_FLASH_DEVID                                 0x2249



#define UBOOT_SIZE                                      (226 * 1024)   


/*********************************************************************************************************
  擦除选项
*********************************************************************************************************/
typedef enum erase_options
{
    ERASE_CHIP,
    ERASE_REGION,
    ERASE_SECTOR
} ENUM_ERASE_OPTIONS;
/*********************************************************************************************************
  写入选项
*********************************************************************************************************/
typedef enum write_options
{
    WRITE_OVERWRITE,
    WRITE_KEEP
} ENUM_WRITE_OPTIONS;

/*********************************************************************************************************
  Mini2440 Sector布局
*********************************************************************************************************/
typedef struct am29LV160DB_sector_infos
{
    UINT8 sector_no;                        /* sector */
    UINT32 sector_start_offset;             /* sector基于Base的起始偏移地址 */
    UINT32 sector_end_offset;               /* setcor基于Base的结束偏移地址 */
    UINT8 sector_size;                      /* setcor大小，单位KB */
    UINT8 region_no;                        /* sector所属Region number */
} am29LV160DB_sector_infos_t;

static am29LV160DB_sector_infos_t _G_am29LV160DB_sector_infos[] = {
    {                                                                   /*  Sector 1                  */
        0,
        0x0,
        0x3FFF,
        16,
        0
    },
    {                                                                   /*  Sector 2                  */
        1,
        0x4000,
        0x5FFF,
        8,
        1
    },
    {                                                                   /*  Sector 3                  */
        2,
        0x6000,
        0x7FFF,
        8,
        1
    },
    {                                                                   /*  Sector 4                  */
        3,
        0x8000,
        0xFFFF,
        32,
        2
    },
    {                                                                   /*  Sector 5                  */
        4,
        0x10000,
        0x1FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 6                  */
        5,
        0x20000,
        0x2FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 7                  */
        6,
        0x30000,
        0x3FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 8                  */
        7,
        0x40000,
        0x4FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 9                  */
        8,
        0x50000,
        0x5FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 10                  */
        9,
        0x60000,
        0x6FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 11                 */
        10,
        0x70000,
        0x7FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 12                  */
        11,
        0x80000,
        0x8FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 13                  */
        12,
        0x90000,
        0x9FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 14                 */
        13,
        0xA0000,
        0xAFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 15                  */
        14,
        0xB0000,
        0xBFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 16                 */
        15,
        0xC0000,
        0xCFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 17                 */
        16,
        0xD0000,
        0xDFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 18                 */
        17,
        0xE0000,
        0xEFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 19                  */
        18,
        0xF0000,
        0xFFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 20                 */
        19,
        0x100000,
        0x10FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 21                 */
        20,
        0x110000,
        0x11FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 22                 */
        21,
        0x120000,
        0x12FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 23                  */
        22,
        0x130000,
        0x13FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 24                  */
        23,
        0x140000,
        0x14FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 25                  */
        24,
        0x150000,
        0x15FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 26                  */
        25,
        0x160000,
        0x16FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 27                  */
        26,
        0x170000,
        0x17FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 28                  */
        27,
        0x180000,
        0x18FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 29                  */
        28,
        0x190000,
        0x19FFFF,
        64,
        3
    },
    {                                                                   /*  Sector 30                  */
        29,
        0x1A0000,
        0x1AFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 31                  */
        30,
        0x1B0000,
        0x1BFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 32                  */
        31,
        0x1C0000,
        0x1CFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 33                  */
        32,
        0x1D0000,
        0x1DFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 34                  */
        33,
        0x1E0000,
        0x1EFFFF,
        64,
        3
    },
    {                                                                   /*  Sector 35                  */
        34,
        0x1F0000,
        0x1FFFFF,
        64,
        3
    }
};



#endif /* SYLIXOS_DRIVER_MTD_NOR_AM29LV160DB_H_ */
