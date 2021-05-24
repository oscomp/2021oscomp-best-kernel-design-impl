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
** 文   件   名: nor.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 11 日
**
** 描        述: NorFlash裸板驱动
*********************************************************************************************************/
#ifndef SYLIXOS_DRIVER_MTD_NOR_NOR_H_
#define SYLIXOS_DRIVER_MTD_NOR_NOR_H_

#include "Am29LV160DB.h"
#include "nor_util.h"


/*********************************************************************************************************
  Mini2440 NorFlash基地址
  这里要为其计算，因为SylixOS只能写入虚拟地址，需要用API_VmmIoRemap2将其映射到某个地方
*********************************************************************************************************/
UINT32 NOR_FLASH_BASE;
/*********************************************************************************************************
  NorFlash启动模式
*********************************************************************************************************/
typedef enum nor_init_flag
{
    INIT_FAKE_NOR,
    INIT_TRUE_NOR
} ENUM_NOR_INIT_FLAG;

ENUM_NOR_INIT_FLAG _G_nor_flash_init_flag;

#define FAKE_MODE()             _G_nor_flash_init_flag = INIT_FAKE_NOR
#define TRUE_MODE()             _G_nor_flash_init_flag = INIT_TRUE_NOR
#define IS_FAKE_MODE()          _G_nor_flash_init_flag == INIT_FAKE_NOR
/*********************************************************************************************************
  有用的宏和内联函数
*********************************************************************************************************/
static inline UINT8 GET_SECTOR_NO(UINT32 offset){
    UINT i;
    for (i = 0; i < NOR_FLASH_NSECTOR; i++)                             
    {                                                                       
        if(_G_am29LV160DB_sector_infos[i].sector_start_offset <= offset &&  
            _G_am29LV160DB_sector_infos[i].sector_end_offset >= offset){    
            return i;                                                         
        }                                                                   
    }
    return -1;
}

static inline UINT32 GET_SECTOR_OFFSET(UINT8 sector_no){
    if(sector_no < 0 || sector_no >= NOR_FLASH_NSECTOR){
        return -1;
    }
    return _G_am29LV160DB_sector_infos[sector_no].sector_start_offset;
}

static inline UINT GET_SECTOR_SIZE(UINT8 sector_no){
    if(sector_no < 0 || sector_no >= NOR_FLASH_NSECTOR){
        return -1;
    }
    return 1024 * (_G_am29LV160DB_sector_infos[sector_no].sector_size);     
}

static inline UINT8 GET_SECTOR_REGION(UINT8 sector_no){
    if(sector_no < 0 || sector_no >= NOR_FLASH_NSECTOR){
        return -1;
    }
    return _G_am29LV160DB_sector_infos[sector_no].region_no;     
}


static inline BOOL IS_SECTOR_DIRTY(UINT32 base, UINT8 sector_no){
    INT i;
    if(sector_no < 0 || sector_no >= NOR_FLASH_NSECTOR){
        return TRUE;
    }
    UINT sector_size = GET_SECTOR_SIZE(sector_no);
    UINT32 addr = GET_SECTOR_OFFSET(sector_no) + base;
    volatile UINT8* p;
    for (i = 0; i < sector_size; i++)
    {
        p = (volatile UINT8*)(addr + i);
        if((*p & 0xFF) != 0xFF){
          return TRUE;
        }
    }
    return FALSE;
}

#define NOR_FLASH_START_OFFSET                     (GET_SECTOR_OFFSET(GET_SECTOR_NO(UBOOT_SIZE) + 1))
/*********************************************************************************************************
  Mini2440 NorFLash操作 
*********************************************************************************************************/
VOID             nor_init(ENUM_NOR_INIT_FLAG nor_init_flag);
VOID             scan_nor();
UINT8            erase_nor(UINT offset, ENUM_ERASE_OPTIONS ops);
UINT8            write_nor(UINT offset, PCHAR content, UINT size_bytes, ENUM_WRITE_OPTIONS ops);
UINT8            read_nor(UINT offset, PCHAR content, UINT size_bytes);

#ifdef NOR_TEST
/*********************************************************************************************************
  测试函数
*********************************************************************************************************/
BOOL             test_nor();
#endif // NOR_TEST

#endif /* SYLIXOS_DRIVER_MTD_NOR_NOR_H_ */
