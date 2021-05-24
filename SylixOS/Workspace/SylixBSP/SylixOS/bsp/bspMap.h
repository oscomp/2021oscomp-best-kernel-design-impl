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
** 文   件   名: bspMap.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 12 月 23 日
**
** 描        述: C 程序入口部分. 物理分页空间与全局映射关系表定义.
*********************************************************************************************************/

#ifndef __BSPMAP_H
#define __BSPMAP_H

/*********************************************************************************************************
   内存分配关系图

    +-----------------------+--------------------------------+
    |       通用内存区      |          VMM 管理区            |
    |         CACHE         |                                |
    +-----------------------+--------------------------------+

*********************************************************************************************************/
/*********************************************************************************************************
  physical memory
*********************************************************************************************************/
#ifdef  __BSPINIT_MAIN_FILE

LW_MMU_PHYSICAL_DESC    _G_physicalDesc[] = {
    {                                                                   /*  中断向量表                  */
        BSP_CFG_RAM_BASE,
        0,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_VECTOR
    },

    {                                                                   /*  内核代码段                  */
        BSP_CFG_RAM_BASE,
        BSP_CFG_RAM_BASE,
        BSP_CFG_TEXT_SIZE,
        LW_PHYSICAL_MEM_TEXT
    },

    {                                                                   /*  内核数据段                  */
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE,
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE,
        BSP_CFG_DATA_SIZE,
        LW_PHYSICAL_MEM_DATA
    },

    {                                                                   /*  DMA 缓冲区                  */
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE + BSP_CFG_DATA_SIZE,
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE + BSP_CFG_DATA_SIZE,
        BSP_CFG_DMA_SIZE,
        LW_PHYSICAL_MEM_DMA
    },

    {                                                                   /*  APP 通用内存                */
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE + BSP_CFG_DATA_SIZE + BSP_CFG_DMA_SIZE,
        BSP_CFG_RAM_BASE + BSP_CFG_TEXT_SIZE + BSP_CFG_DATA_SIZE + BSP_CFG_DMA_SIZE,
        BSP_CFG_APP_SIZE,
        LW_PHYSICAL_MEM_APP
    },

    /*
     *  external io & memory interface
     */
    {                                                                   /*  BANK4 - CAN CONTROLER       */
        0x20000000,
        0x20000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR                                         /*  状态属性 NCNB               */
    },
    {                                                                   /*  BANK5 - AX88796             */
        0x28000000,
        0x28000000,
        (1 * 1024 * 1024),
        LW_PHYSICAL_MEM_BOOTSFR                                         /*  状态属性 NCNB               */
    },
    
    /*
     *  internal sfr area
     */
    {                                                                   /*  memory controller           */
        0x48000000,
        0x48000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  USB HOST controller         */
        0x49000000,
        0x49000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  INTERRUPT controller        */
        0x4a000000,
        0x4a000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  DMA controller              */
        0x4b000000,
        0x4b000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  CLOCK & POWER controller    */
        0x4c000000,
        0x4c000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  LCD controller              */
        0x4d000000,
        0x4d000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  NAND FLASH controller       */
        0x4E000000,
        0x4E000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  CAMERA controller           */
        0x4F000000,
        0x4F000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  UART0 controller            */
        0x50000000,
        0x50000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  UART1 controller            */
        0x50004000,
        0x50004000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  UART2 controller            */
        0x50008000,
        0x50008000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  PWM TIMER controller        */
        0x51000000,
        0x51000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  USB DEV controller          */
        0x52000000,
        0x52000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  WATCH DOG TIMER controller  */
        0x53000000,
        0x53000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  IIC controller              */
        0x54000000,
        0x54000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  IIS controller              */
        0x55000000,
        0x55000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  I/O PORT  controller        */
        0x56000000,
        0x56000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  RTC  controller             */
        0x57000000,
        0x57000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  A/DC  controller            */
        0x58000000,
        0x58000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },
    
    {                                                                   /*  SPI  controller             */
        0x59000000,
        0x59000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },

    {                                                                   /*  SD Interface  controller    */
        0x5a000000,
        0x5a000000,
        LW_CFG_VMM_PAGE_SIZE,
        LW_PHYSICAL_MEM_BOOTSFR
    },

    {                                                                   /*  结束                        */
        0,
        0,
        0,
        0
    }
};
/*********************************************************************************************************
  virtual memory
*********************************************************************************************************/
LW_MMU_VIRTUAL_DESC    _G_virtualDesc[] = {
    {                                                                   /*  应用程序虚拟空间            */
        0x60000000,
        ((size_t)2 * LW_CFG_GB_SIZE),
        LW_VIRTUAL_MEM_APP
    },

    {
        0xe0000000,                                                     /*  ioremap 空间                */
        (256 * LW_CFG_MB_SIZE),
        LW_VIRTUAL_MEM_DEV
    },

    {                                                                   /*  结束                        */
        0,
        0,
        0
    }
};

#endif                                                                  /*  __BSPINIT_MAIN_FILE         */
#endif                                                                  /*  __BSPMAP_H                  */
/*********************************************************************************************************
  END
*********************************************************************************************************/
