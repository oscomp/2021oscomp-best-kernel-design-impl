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
** 文   件   名: s3c2440a_lcd.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 19 日
**
** 描        述: LCD 底层驱动函数 (16位色彩)
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"
#include "SylixOS.h"
/*********************************************************************************************************
  选择液晶显示器
*********************************************************************************************************/
#ifdef MINI2440_PACKET
#define __ATO070                    1                                   /*  使用 800 * 480              */
#elif defined(MICRO2440_PACKET)
#define __P35                       1
#endif
/*********************************************************************************************************
  framebuffer base 
*********************************************************************************************************/
static void     *GpvFbMemBase = LW_NULL;
/*********************************************************************************************************
  framebuffer base (没有使用 MMU)
*********************************************************************************************************/
#define __LCD_FRAMEBUFFER_BASE      (ULONG)GpvFbMemBase

#if defined(__P35)
#define LCD_XSIZE                   320
#define LCD_YSIZE                   240
#elif defined(__ATO070)
#define LCD_XSIZE                   800
#define LCD_YSIZE                   480
#else
#define LCD_XSIZE                   640
#define LCD_YSIZE                   480
#endif                                                                  /*  __ATO070                    */
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define __LCD_VIRTUAL_XSIZE     LCD_XSIZE                               /*  X 方向虚屏                  */
#define __LCD_VIRTUAL_YSIZE     LCD_YSIZE                               /*  Y 方向虚屏                  */

#define __BITS_PERPIXEL         16u
#define __BYTES_PERPIXEL        ((__BITS_PERPIXEL  + 7) / 8)
#define __HALFWORD_PERPIXEL     ((__BYTES_PERPIXEL + 1) / 2)
#define __BYTES_PERLINE         (__BYTES_PERPIXEL  * __LCD_VIRTUAL_XSIZE)

#define __LCDBANK(bufaddr)      ((((unsigned int)bufaddr) & 0x7FC00000) >> 1) 
#define __LCDBASEU(bufaddr)     ((((unsigned int)bufaddr) & 0x003FFFFE) >> 1)
/*********************************************************************************************************
** Function name:           __lcdPortInit
**
** Descriptions:            设置lcd控制口线
**
** input parameters:        NONE
** output parameters:       NONE
**                            
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static  void  __lcdPortInit (void)
{
    rGPCUP      = rGPCUP   | (0x7E0Fu << 1);
    rGPCCON     = (rGPCCON & (~(0x3FFC00FFu << 2))) | (0x2AA800AAu << 2);
    rGPDUP      = rGPDUP   | (0x3F3F << 2);
    rGPDCON     = (rGPDCON & (~(0x0FFF0FFFu << 4))) | (0x0AAA0AAAu << 4);
}
/*********************************************************************************************************
** Function name:           __lcdControlInit
**
** Descriptions:            初始化lcd控制器,虚屏指向 (0,0)
**
** input parameters:        NONE
** output parameters:       NONE
**                            
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static  void  __lcdControlInit (void)
{
#if defined(__P35)

    /*
     *  |------------------------------------------------
     *  | 27:18 | 17: 8 |   7   |  6:5  |  4:1  |   0   |
     *  |------------------------------------------------
     *  |LINECNT| CLKVAL| MMODE |PNRMODE|BPPMODE| ENVID |
     *  |------------------------------------------------
     */

    rLCDCON1  = (6      << 8)   |                                       /*  VCLK = HCLK/[(CLKVAL+1)*2]  */
                (0      << 7)   |                                       /*  Determine the toggle rate   */
                (3      << 5)   |                                       /*  TFT lcd pannel              */
                (0x0c   << 1)   |                                       /*  16bit per Pixel             */
                (0      << 0);                                          /*  Disable the video output    */

    /*
     *  |--------------------------------
     *  | 31:24 | 23:14 | 13:6  |  5:0  |
     *  |--------------------------------
     *  |  VBPD |LINEVAL|  VFPD |  VSPW |
     *  |--------------------------------
     */

    rLCDCON2  = (9      << 24)  |                                       /*  场同步信号后等待 26 Line    */
                ((LCD_YSIZE - 1) << 14) |                               /*  LINEVAL = LINE - 1          */
                (1      << 6)   |                                       /*  帧完毕后等待 21 Line        */
                (2      << 0);                                          /*  帧信号(Vsync)宽度为 3 Line  */

    /*
     *  |------------------------
     *  | 25:19 | 18:8  |  7:0  |
     *  |------------------------
     *  |  HBPD | HOZVAL|  HFPD |
     *  |------------------------
     */

    rLCDCON3  = (5      << 19)  |                                       /*  行同步信号后51个空周期      */
                ((LCD_XSIZE - 1) << 8) |                                /*  HOZVAL = 行像素数 - 1       */
                (12     << 0);                                          /*  行信号前51个周期            */

    /*
     *  |----------------
     *  | 15:8  |  7:0  |
     *  |----------------
     *  |  MVAL |  HSPW |
     *  |----------------
     */

    rLCDCON4  = (13 <<  8) | (1 << 0);                                  /*  行同步脉冲高电平宽度        */

    /*
     *  |--------------------------------------------------------------------
     *  | 16:15 | 14:13 |   12   |   11  |   10  |   9    |    8    |   7   |
     *  |--------------------------------------------------------------------
     *  |VSTATUS|HSTATUS|BPP24BL | FRM565|INVVCLK|INVVLINE|INVVFRAME| INVVD |
     *  |--------------------------------------------------------------------
     *  |   6   |   5    |   4   |   3   |   2   |   1    |    0    |       |
     *  |--------------------------------------------------------------------
     *  |INVVDEN|INVPWREN|INVLEND| PWREN | ENLEND|  BSWP  |  HWSWP  |       |
     *  |--------------------------------------------------------------------
     */

    rLCDCON5  = (1      << 11)  |                                       /*  RGB 565 格式                */
                (1      << 10)  |                                       /*  上降沿发送数据              */
                (1      << 9)   |                                       /*  HSYNC脉冲极性翻转           */
                (1      << 8)   |                                       /*  VSYNC脉冲极性翻转           */
                (0      << 7)   |                                       /*  像素时钟翻转                */
                (0      << 6)   |                                       /*  VDEN脉冲不翻转              */
                (0      << 5)   |                                       /*  PWREN脉冲不翻转             */
                (0      << 4)   |                                       /*  LEND脉冲不翻转              */
                (1      << 3)   |                                       /*  PWREN信号不使能             */
                (0      << 2)   |                                       /*  LEND信号不使能              */
                (0      << 1)   |                                       /*  字节翻转不使能              */
                (1      << 0);                                          /*  半字翻转使能                */


    rLCDSADDR1 = __LCDBANK(__LCD_FRAMEBUFFER_BASE) |
                 __LCDBASEU(__LCD_FRAMEBUFFER_BASE);                    /*  设置显示缓冲区--左上角      */
                                                                        /*  (FrameBuffer需要字对齐)     */
    rLCDSADDR2 = __LCDBASEU(__LCD_FRAMEBUFFER_BASE) +
                 __LCD_VIRTUAL_XSIZE * LCD_YSIZE * __HALFWORD_PERPIXEL; /*  显示区域缓冲区--右下角      */

    rLCDSADDR3 = (((__LCD_VIRTUAL_XSIZE - LCD_XSIZE) * __HALFWORD_PERPIXEL) << 11) |
                                                                        /*  上行显示最后像素与本行显示首*/
                                                                        /*  像素间隔，即行中不显示半字  */
                                                                        /*  虚屏偏移地址                */
                 (LCD_XSIZE * __HALFWORD_PERPIXEL);                     /*  屏幕真正宽度，实际显示宽度  */
    rLCDINTPND = 0;
    rLCDINTMSK |= 3;                                                    /*  禁止FIFO中断、帧同步中断    */
    rTPAL      = 0x000000000;                                           /*  禁止临时调色板              */

#elif defined(__ATO070)
    /*
     *  |------------------------------------------------
     *  | 27:18 | 17: 8 |   7   |  6:5  |  4:1  |   0   |
     *  |------------------------------------------------
     *  |LINECNT| CLKVAL| MMODE |PNRMODE|BPPMODE| ENVID |
     *  |------------------------------------------------
     */
    rLCDCON1  = (1      << 8)   |                                       /*  VCLK = HCLK/[(CLKVAL+1)*2]  */
                (0      << 7)   |                                       /*  Determine the toggle rate   */
                (3      << 5)   |                                       /*  TFT lcd pannel              */
                (0x0c   << 1)   |                                       /*  16bit per Pixel             */
                (0      << 0);                                          /*  Disable the video output    */

    /*
     *  |--------------------------------
     *  | 31:24 | 23:14 | 13:6  |  5:0  |
     *  |--------------------------------
     *  |  VBPD |LINEVAL|  VFPD |  VSPW |
     *  |--------------------------------
     */

    rLCDCON2  = (8      << 24)  |                                       /*  场同步信号后等待 26 Line    */
                ((LCD_YSIZE - 1) << 14) |                               /*  LINEVAL = LINE - 1          */
                (20     << 6)   |                                       /*  帧完毕后等待 21 Line        */
                (3      << 0);                                          /*  帧信号(Vsync)宽度为 3 Line  */

    /*
     *  |------------------------
     *  | 25:19 | 18:8  |  7:0  |
     *  |------------------------
     *  |  HBPD | HOZVAL|  HFPD |
     *  |------------------------
     */

    rLCDCON3  = (113    << 19)  |                                       /*  行同步信号后51个空周期      */
                ((LCD_XSIZE - 1) << 8) |                                /*  HOZVAL = 行像素数 - 1       */
                (50     << 0);                                          /*  行信号前51个周期            */

    /*
     *  |----------------
     *  | 15:8  |  7:0  |
     *  |----------------
     *  |  MVAL |  HSPW |
     *  |----------------
     */

    rLCDCON4  = (30    << 0);                                           /*  行同步脉冲高电平宽度       */

    /*
     *  |--------------------------------------------------------------------
     *  | 16:15 | 14:13 |   12   |   11  |   10  |   9    |    8    |   7   |
     *  |--------------------------------------------------------------------
     *  |VSTATUS|HSTATUS|BPP24BL | FRM565|INVVCLK|INVVLINE|INVVFRAME| INVVD |
     *  |--------------------------------------------------------------------
     *  |   6   |   5    |   4   |   3   |   2   |   1    |    0    |       |
     *  |--------------------------------------------------------------------
     *  |INVVDEN|INVPWREN|INVLEND| PWREN | ENLEND|  BSWP  |  HWSWP  |       |
     *  |--------------------------------------------------------------------
     */

    rLCDCON5  = (1      << 11)  |                                       /*  RGB 565 格式                */
                (1      << 10)  |                                       /*  上降沿发送数据              */
                (1      << 9)   |                                       /*  HSYNC脉冲极性翻转           */
                (1      << 8)   |                                       /*  VSYNC脉冲极性翻转           */
                (0      << 7)   |                                       /*  像素时钟翻转                */
                (0      << 6)   |                                       /*  VDEN脉冲不翻转              */
                (0      << 5)   |                                       /*  PWREN脉冲不翻转             */
                (0      << 4)   |                                       /*  LEND脉冲不翻转              */
                (1      << 3)   |                                       /*  PWREN信号不使能             */
                (0      << 2)   |                                       /*  LEND信号不使能              */
                (0      << 1)   |                                       /*  字节翻转不使能              */
                (1      << 0);                                          /*  半字翻转使能                */


    rLCDSADDR1 = __LCDBANK(__LCD_FRAMEBUFFER_BASE) |
                 __LCDBASEU(__LCD_FRAMEBUFFER_BASE);                    /*  设置显示缓冲区--左上角      */
                                                                        /*  (FrameBuffer需要字对齐)     */
    rLCDSADDR2 = __LCDBASEU(__LCD_FRAMEBUFFER_BASE) +
                 __LCD_VIRTUAL_XSIZE * LCD_YSIZE * __HALFWORD_PERPIXEL; /*  显示区域缓冲区--右下角      */

    rLCDSADDR3 = (((__LCD_VIRTUAL_XSIZE - LCD_XSIZE) * __HALFWORD_PERPIXEL) << 11) |
                                                                        /*  上行显示最后像素与本行显示首*/
                                                                        /*  像素间隔，即行中不显示半字  */
                                                                        /*  虚屏偏移地址                */
                 (LCD_XSIZE * __HALFWORD_PERPIXEL);                     /*  屏幕真正宽度，实际显示宽度  */
    rLCDINTPND = 0;
    rLCDINTMSK |= 3;                                                    /*  禁止FIFO中断、帧同步中断    */
    rTPAL      = 0x000000000;                                           /*  禁止临时调色板              */

#elif defined(__LQ104)
    /*
     *  |------------------------------------------------
     *  | 27:18 | 17: 8 |   7   |  6:5  |  4:1  |   0   |
     *  |------------------------------------------------
     *  |LINECNT| CLKVAL| MMODE |PNRMODE|BPPMODE| ENVID |
     *  |------------------------------------------------
     */

    rLCDCON1  = (1      << 8)   |                                       /*  VCLK = HCLK/[(CLKVAL+1)*2]  */
                (0      << 7)   |                                       /*  Determine the toggle rate   */
                (3      << 5)   |                                       /*  TFT lcd pannel              */
                (0x0c   << 1)   |                                       /*  16bit per Pixel             */
                (0      << 0);                                          /*  Disable the video output    */

    /*
     *  |--------------------------------
     *  | 31:24 | 23:14 | 13:6  |  5:0  |
     *  |--------------------------------
     *  |  VBPD |LINEVAL|  VFPD |  VSPW |
     *  |--------------------------------
     */

    rLCDCON2  = (25     << 24)  |                                       /*  场同步信号后等待 26 Line    */
                ((LCD_YSIZE - 1) << 14) |                               /*  LINEVAL = LINE - 1          */
                (20     << 6)   |                                       /*  帧完毕后等待 21 Line        */
                (10     << 0);                                          /*  帧信号(Vsync)宽度为 11 Line */

    /*
     *  |------------------------
     *  | 25:19 | 18:8  |  7:0  |
     *  |------------------------
     *  |  HBPD | HOZVAL|  HFPD |
     *  |------------------------
     */

    rLCDCON3  = (50    << 19)  |                                        /*  行同步信号后51个空周期      */
                ((LCD_XSIZE - 1) << 8) |                                /*  HOZVAL = 行像素数 - 1       */
                (50     << 0);                                          /*  行信号前51个周期            */

    /*
     *  |----------------
     *  | 15:8  |  7:0  |
     *  |----------------
     *  |  MVAL |  HSPW |
     *  |----------------
     */

    rLCDCON4  = (95    << 0);                                           /*  行同步脉冲高电平宽度        */

    /*
     *  |--------------------------------------------------------------------
     *  | 16:15 | 14:13 |   12   |   11  |   10  |   9    |    8    |   7   |
     *  |--------------------------------------------------------------------
     *  |VSTATUS|HSTATUS|BPP24BL | FRM565|INVVCLK|INVVLINE|INVVFRAME| INVVD |
     *  |--------------------------------------------------------------------
     *  |   6   |   5    |   4   |   3   |   2   |   1    |    0    |       |
     *  |--------------------------------------------------------------------
     *  |INVVDEN|INVPWREN|INVLEND| PWREN | ENLEND|  BSWP  |  HWSWP  |       |
     *  |--------------------------------------------------------------------
     */

    rLCDCON5  = (1      << 11)  |                                       /*  RGB 565 格式                */
                (0      << 10)  |                                       /*  下降沿发送数据              */
                (1      << 9)   |                                       /*  HSYNC脉冲极性翻转           */
                (1      << 8)   |                                       /*  VSYNC脉冲极性翻转           */
                (0      << 7)   |                                       /*  像素时钟翻转                */
                (0      << 6)   |                                       /*  VDEN脉冲不翻转              */
                (0      << 5)   |                                       /*  PWREN脉冲不翻转             */
                (0      << 4)   |                                       /*  LEND脉冲不翻转              */
                (1      << 3)   |                                       /*  PWREN信号不使能             */
                (0      << 2)   |                                       /*  LEND信号不使能              */
                (0      << 1)   |                                       /*  字节翻转不使能              */
                (1      << 0);                                          /*  半字翻转使能                */
                      
    rLCDSADDR1 = __LCDBANK(__LCD_FRAMEBUFFER_BASE) | 
                 __LCDBASEU(__LCD_FRAMEBUFFER_BASE);                    /*  设置显示缓冲区--左上角      */
                                                                        /*  (FrameBuffer需要字对齐)     */
    rLCDSADDR2 = __LCDBASEU(__LCD_FRAMEBUFFER_BASE) +  
                 __LCD_VIRTUAL_XSIZE * LCD_YSIZE * __HALFWORD_PERPIXEL; /*  显示区域缓冲区--右下角      */

    rLCDSADDR3 = (((__LCD_VIRTUAL_XSIZE - LCD_XSIZE) * __HALFWORD_PERPIXEL) << 11) |  
                                                                        /*  上行显示最后像素与本行显示首*/
                                                                        /*  像素间隔，即行中不显示半字  */
                                                                        /*  虚屏偏移地址                */
                 (LCD_XSIZE * __HALFWORD_PERPIXEL);                     /*  屏幕真正宽度，实际显示宽度  */
    rLCDINTPND = 0;
    rLCDINTMSK |= 3;                                                    /*  禁止FIFO中断、帧同步中断    */
    rTPAL      = 0x000000000;                                           /*  禁止临时调色板              */

#else
    /*
     *  |------------------------------------------------
     *  | 27:18 | 17: 8 |   7   |  6:5  |  4:1  |   0   |
     *  |------------------------------------------------
     *  |LINECNT| CLKVAL| MMODE |PNRMODE|BPPMODE| ENVID |
     *  |------------------------------------------------
     */

    rLCDCON1  = (1      << 8)   |                                       /*  VCLK = HCLK/[(CLKVAL+1)*2]  */
                (0      << 7)   |                                       /*  Determine the toggle rate   */
                (3      << 5)   |                                       /*  TFT lcd pannel              */
                (0x0c   << 1)   |                                       /*  16bit per Pixel             */
                (0      << 0);                                          /*  Disable the video output    */

    /*
     *  |--------------------------------
     *  | 31:24 | 23:14 | 13:6  |  5:0  |
     *  |--------------------------------
     *  |  VBPD |LINEVAL|  VFPD |  VSPW |
     *  |--------------------------------
     */

    rLCDCON2  = (32     << 24)  |                                       /*  场同步信号后等待 33 Line    */
                ((LCD_YSIZE - 1) << 14) |                               /*  LINEVAL = LINE - 1          */
                ( 9     << 6)   |                                       /*  帧完毕后等待 10 Line        */
                ( 0     << 0);                                          /*  帧信号(Vsync)宽度为 1 Line  */

    /*
     *  |------------------------
     *  | 25:19 | 18:8  |  7:0  |
     *  |------------------------
     *  |  HBPD | HOZVAL|  HFPD |
     *  |------------------------
     */

    rLCDCON3  = (47    << 19)  |                                        /*  行同步信号后49个空周期      */
                ((LCD_XSIZE - 1) << 8) |                                /*  HOZVAL = 行像素数 - 1       */
                (15     << 0);                                          /*  行信号前16个周期            */

    /*
     *  |----------------
     *  | 15:8  |  7:0  |
     *  |----------------
     *  |  MVAL |  HSPW |
     *  |----------------
     */

    rLCDCON4  = (95    << 0);                                           /*  行同步脉冲高电平宽度        */

    /*
     *  |--------------------------------------------------------------------
     *  | 16:15 | 14:13 |   12   |   11  |   10  |   9    |    8    |   7   |
     *  |--------------------------------------------------------------------
     *  |VSTATUS|HSTATUS|BPP24BL | FRM565|INVVCLK|INVVLINE|INVVFRAME| INVVD |
     *  |--------------------------------------------------------------------
     *  |   6   |   5    |   4   |   3   |   2   |   1    |    0    |       |
     *  |--------------------------------------------------------------------
     *  |INVVDEN|INVPWREN|INVLEND| PWREN | ENLEND|  BSWP  |  HWSWP  |       |
     *  |--------------------------------------------------------------------
     */

    rLCDCON5  = (1      << 11)  |                                       /*  RGB 565 格式                */
                (0      << 10)  |                                       /*  下降沿发送数据              */
                (1      << 9)   |                                       /*  HSYNC脉冲极性翻转           */
                (1      << 8)   |                                       /*  VSYNC脉冲极性翻转           */
                (0      << 7)   |                                       /*  像素时钟翻转                */
                (0      << 6)   |                                       /*  VDEN脉冲不翻转              */
                (0      << 5)   |                                       /*  PWREN脉冲不翻转             */
                (0      << 4)   |                                       /*  LEND脉冲不翻转              */
                (1      << 3)   |                                       /*  PWREN信号不使能             */
                (0      << 2)   |                                       /*  LEND信号不使能              */
                (0      << 1)   |                                       /*  字节翻转不使能              */
                (1      << 0);                                          /*  半字翻转使能                */
                      
    rLCDSADDR1 = __LCDBANK(__LCD_FRAMEBUFFER_BASE) | 
                 __LCDBASEU(__LCD_FRAMEBUFFER_BASE);                    /*  设置显示缓冲区--左上角      */
                                                                        /*  (FrameBuffer需要字对齐)     */
    rLCDSADDR2 = __LCDBASEU(__LCD_FRAMEBUFFER_BASE) +  
                 __LCD_VIRTUAL_XSIZE * LCD_YSIZE * __HALFWORD_PERPIXEL; /*  显示区域缓冲区--右下角      */

    rLCDSADDR3 = (((__LCD_VIRTUAL_XSIZE - LCD_XSIZE) * __HALFWORD_PERPIXEL) << 11) |  
                                                                        /*  上行显示最后像素与本行显示首*/
                                                                        /*  像素间隔，即行中不显示半字  */
                                                                        /*  虚屏偏移地址                */
                 (LCD_XSIZE * __HALFWORD_PERPIXEL);                     /*  屏幕真正宽度，实际显示宽度  */
    rLCDINTPND = 0;
    rLCDINTMSK |= 3;                                                    /*  禁止FIFO中断、帧同步中断    */
    rTPAL      = 0x000000000;                                           /*  禁止临时调色板              */
        
#endif                                                                  /*  __LQ104                     */

#if 0
        (*(volatile unsigned *)0x4d000014) = 0x180D8000;                /*  STN/TFT Frame buffer start  */
        (*(volatile unsigned *)0x4d000018) = 0x000EAC00;                /*  STN/TFT Frame buffer start  */
        (*(volatile unsigned *)0x4d00001c) = 0x00000140;                /*  STN/TFT Virtual screen      */
        (*(volatile unsigned *)0x4d000020) = 0x00000000;                /*  STN Red lookup table        */
        (*(volatile unsigned *)0x4d000024) = 0x00000000;                /*  STN Green lookup table      */
        (*(volatile unsigned *)0x4d000028) = 0x00000000;                /*  STN Blue lookup table       */
        
        (*(volatile unsigned *)0x4d00004c) = 0x00000000;                /*  STN Dithering mode          */
        (*(volatile unsigned *)0x4d000050) = 0x00000000;                /*  TFT Temporary palette       */
        (*(volatile unsigned *)0x4d000054) = 0x00000000;                /*  LCD Interrupt pending       */
        (*(volatile unsigned *)0x4d000058) = 0x00000003;                /*  LCD Interrupt source        */
        (*(volatile unsigned *)0x4d00005c) = 0x00000003;                /*  LCD Interrupt mask          */
        (*(volatile unsigned *)0x4d000060) = 0x00000CE0;
        
#endif
}
/*********************************************************************************************************
** Function name:           __lcdPowerEnable
**
** Descriptions:            打开lcd控制器
**
** input parameters:        NONE
** output parameters:       NONE
**
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if defined(__P35)

static void  __lcdPowerEnable (int invpwren, int pwren)
{
    /*
     * GPG4 is setted as LCD_PWREN
     */
    rGPGUP  = rGPGUP  | (1 << 4);                                       /*  Pull-up disable             */
    rGPGCON = rGPGCON | (3 << 8);                                       /*  GPG4 = LCD_PWREN            */

    /*
     * Enable LCD POWER ENABLE Function
     */
    rLCDCON5 = (rLCDCON5 & (~(1 << 3))) | (pwren << 3);                 /*  PWREN                       */
    rLCDCON5 = (rLCDCON5 & (~(1 << 5))) | (invpwren << 5);              /*  INVPWREN                    */
}

#endif
/*********************************************************************************************************
** Function name:           __lcdOn
**
** Descriptions:            打开lcd控制器
**
** input parameters:        NONE
** output parameters:       NONE
**                            
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void  __lcdOn (void)
{
#if defined(__P35)
    unsigned char bl;
    int fd;

    __lcdPowerEnable(0, 1);

    fd = open("/dev/input/touch0", O_RDWR, 0666);
    if (fd > 0) {
        bl = 70;
        ioctl(fd, 202, &bl);
        close(fd);
    }
#endif

    rTPAL     = 0;                                                      /*  关闭临时调色板              */
    rLCDCON1 |= 0x01;                                                   /*  ENVID=1，打开显示           */
}
/*********************************************************************************************************
** Function name:           __lcdOff
**
** Descriptions:            关闭lcd控制器
**
** input parameters:        NONE
** output parameters:       NONE
**                            
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void  __lcdOff (void)
{
    rLCDCON1 &= (~(0X1U));                                              /*  ENVID=0，关闭显示           */
}
/*********************************************************************************************************
** Function name:           __lcdInit
**
** Descriptions:            初始化lcd
**
** input parameters:        NONE
** output parameters:       NONE
**                            
** Returned value:          NONE
**
** Created by:              lixintian
** Created Date:            2007-09-18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void  __lcdInit (void)
{
    __lcdPortInit();                                                    /*  初始化lcd控制口线           */
    __lcdControlInit();                                                 /*  初始化lcd控制器             */
}
/*********************************************************************************************************
  显示信息
*********************************************************************************************************/
static LW_GM_FILEOPERATIONS _G_gmfoS3c2440a;
static LW_GM_DEVICE         _G_gmdevS3c2440a;
/*********************************************************************************************************
  驱动函数
*********************************************************************************************************/
static INT      __lcdOpen(PLW_GM_DEVICE  pgmdev, INT  iFlag, INT  iMode);
static INT      __lcdClose(PLW_GM_DEVICE  pgmdev);
static INT      __lcdGetVarInfo(PLW_GM_DEVICE  pgmdev, PLW_GM_VARINFO  pgmvi);
static INT      __lcdGetScrInfo(PLW_GM_DEVICE  pgmdev, PLW_GM_SCRINFO  pgmsi);
/*********************************************************************************************************
** Function name:           __lcdOpen
** Descriptions:            打开 lcd 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ERROR CODE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT  __lcdOpen (PLW_GM_DEVICE  pgmdev, INT  iFlag, INT  iMode)
{
    if (GpvFbMemBase == LW_NULL) {
        GpvFbMemBase =  API_VmmPhyAllocAlign(800 * 480 * 2,             /*  最大 800 * 480 分辨率       */
                                             4 * 1024 * 1024,
                                             LW_ZONE_ATTR_DMA);         /*  4 MB 对齐地址               */
        if (GpvFbMemBase == LW_NULL) {
            __lcdOff();
            printk(KERN_ERR "__lcdOpen() low vmm memory!\n");
            return  (PX_ERROR);
        }
        
        __lcdInit();
    }
    
    __lcdOn();
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __lcdClose
** Descriptions:            关闭 lcd 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ERROR CODE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT      __lcdClose (PLW_GM_DEVICE  pgmdev)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __lcdGetVarInfo
** Descriptions:            获得 lcd 设备 LW_GM_VARINFO 参数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ERROR CODE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT      __lcdGetVarInfo (PLW_GM_DEVICE  pgmdev, PLW_GM_VARINFO  pgmvi)
{
    if (pgmvi) {
        pgmvi->GMVI_ulXRes = LCD_XSIZE;
        pgmvi->GMVI_ulYRes = LCD_YSIZE;
        
        pgmvi->GMVI_ulXResVirtual = LCD_XSIZE;
        pgmvi->GMVI_ulYResVirtual = LCD_YSIZE;
        
        pgmvi->GMVI_ulXOffset = 0;
        pgmvi->GMVI_ulYOffset = 0;
        
        pgmvi->GMVI_ulBitsPerPixel  = 16;
        pgmvi->GMVI_ulBytesPerPixel = 2;
        
        pgmvi->GMVI_ulGrayscale = 65536;
        
        pgmvi->GMVI_ulRedMask   = 0xF800;
        pgmvi->GMVI_ulGreenMask = 0x03E0;
        pgmvi->GMVI_ulBlueMask  = 0x001F;
        pgmvi->GMVI_ulTransMask = 0;
        
        pgmvi->GMVI_bHardwareAccelerate = LW_FALSE;
        pgmvi->GMVI_ulMode              = LW_GM_SET_MODE;
        pgmvi->GMVI_ulStatus            = 0;
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __lcdGetScrInfo
** Descriptions:            获得 lcd 设备 PLW_GM_SCRINFO 参数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ERROR CODE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT      __lcdGetScrInfo (PLW_GM_DEVICE  pgmdev, PLW_GM_SCRINFO  pgmsi)
{
    if (pgmsi) {
        pgmsi->GMSI_pcName    = "/dev/fb0";
        pgmsi->GMSI_ulId      = 0;
        pgmsi->GMSI_stMemSize        = 800 * 480 * 2;
        pgmsi->GMSI_stMemSizePerLine = LCD_XSIZE * 2;
        pgmsi->GMSI_pcMem            = (caddr_t)__LCD_FRAMEBUFFER_BASE;
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           lcdDevCreate
** Descriptions:            创建 lcd 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ERROR CODE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
INT  lcdDevCreate (VOID)
{
    /*
     *  仅支持 framebuffer 模式.
     */
    _G_gmfoS3c2440a.GMFO_pfuncOpen       = __lcdOpen;
    _G_gmfoS3c2440a.GMFO_pfuncClose      = __lcdClose;
    _G_gmfoS3c2440a.GMFO_pfuncGetVarInfo = (INT (*)(LONG, PLW_GM_VARINFO))__lcdGetVarInfo;
    _G_gmfoS3c2440a.GMFO_pfuncGetScrInfo = (INT (*)(LONG, PLW_GM_SCRINFO))__lcdGetScrInfo;

    _G_gmdevS3c2440a.GMDEV_gmfileop = &_G_gmfoS3c2440a;
    
    return  (gmemDevAdd("/dev/fb0", &_G_gmdevS3c2440a));
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
