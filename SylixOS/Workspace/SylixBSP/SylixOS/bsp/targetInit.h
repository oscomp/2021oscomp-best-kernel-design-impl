/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: targetInit.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 12 月 09 日
**
** 描        述: S3C2440 关键硬件初始化.
*********************************************************************************************************/

#ifndef __TARGETINIT_H
#define __TARGETINIT_H

/*********************************************************************************************************
  系统变量声明
*********************************************************************************************************/
extern  const unsigned int  __GuiPllFreqTbl[2][7];
/*********************************************************************************************************
  系统主频定义
*********************************************************************************************************/

#define FCLK              (__GuiPllFreqTbl[INPUT_FREQ][MPLL_FREQ])      /*  系统主频                    */
#define HCLK              (FCLK / AHB_DIV)                              /*  系统外频                    */
#define PCLK              (HCLK / APB_DIV)                              /*  系统片内外设频率            */
#define UCLK              (48000000)                                    /*  UCLK must be 48MHz for USB  */

/*********************************************************************************************************
  ARM 模式定义
*********************************************************************************************************/

#define MODE_USR32         0x10                                         /*  用户状态                    */
#define MODE_FIQ32         0x11                                         /*  快速中断状态                */
#define MODE_IRQ32         0x12                                         /*  中断状态                    */
#define MODE_SVC32         0x13                                         /*  管理状态                    */
#define MODE_ABT32         0x17                                         /*  中止状态                    */
#define MODE_UND32         0x1B                                         /*  未定义状态                  */
#define MODE_SYS32         0x1F                                         /*  系统状态                    */

#define MODE_DISFIQ        0x40                                         /*  关闭 FIQ 中断               */
#define MODE_DISIRQ        0x80                                         /*  关闭 IRQ 中断               */

/*********************************************************************************************************
  中断向量定义
*********************************************************************************************************/

#define VIC_CHANNEL_EINT0           0                                   /*  外部中断 0                  */
#define VIC_CHANNEL_EINT1           1                                   /*  外部中断 1                  */
#define VIC_CHANNEL_EINT2           2                                   /*  外部中都 2                  */
#define VIC_CHANNEL_EINT3           3                                   /*  外部中断 3                  */
#define VIC_CHANNEL_EINT4_7         4                                   /*  外部中断 4 到 7             */
#define VIC_CHANNEL_EINT8_23        5                                   /*  外部中都 8 到 23            */
#define VIC_CHANNEL_CAM             6                                   /*  摄像头中断                  */
#define VIC_CHANNEL_BAT_FLT         7                                   /*  电池故障                    */
#define VIC_CHANNEL_TICK            8                                   /*  RTC Tick 中断               */
#define VIC_CHANNEL_WDT_AC97        9                                   /*  看门狗或 AC97 声卡接口中断  */
#define VIC_CHANNEL_TIMER0         10                                   /*  定时器 0                    */
#define VIC_CHANNEL_TIMER1         11                                   /*  定时器 1                    */
#define VIC_CHANNEL_TIMER2         12                                   /*  定时器 2                    */
#define VIC_CHANNEL_TIMER3         13                                   /*  定时器 3                    */
#define VIC_CHANNEL_TIMER4         14                                   /*  定时器 4                    */
#define VIC_CHANNEL_UART2          15                                   /*  串口 2                      */
#define VIC_CHANNEL_LCD            16                                   /*  LCD 控制器                  */
#define VIC_CHANNEL_DMA0           17                                   /*  DMA 通道 0                  */
#define VIC_CHANNEL_DMA1           18                                   /*  DMA 通道 1                  */
#define VIC_CHANNEL_DMA2           19                                   /*  DMA 通道 2                  */
#define VIC_CHANNEL_DMA3           20                                   /*  DMA 通道 3                  */
#define VIC_CHANNEL_SDI            21                                   /*  SDI 中断                    */
#define VIC_CHANNEL_SPI0           22                                   /*  SPI0 中断                   */
#define VIC_CHANNEL_UART1          23                                   /*  串口 1                      */
#define VIC_CHANNEL_NFCON          24                                   /*  NAND FLASH 控制器中断       */
#define VIC_CHANNEL_USBD           25                                   /*  USB 设备中断                */
#define VIC_CHANNEL_USBH           26                                   /*  USB 控制器中断              */
#define VIC_CHANNEL_IIC            27                                   /*  IIC 中断                    */
#define VIC_CHANNEL_UART0          28                                   /*  串口 0                      */
#define VIC_CHANNEL_SPI1           29                                   /*  SPI1 中断                   */
#define VIC_CHANNEL_RTC            30                                   /*  RTC 报警中断                */
#define VIC_CHANNEL_ADC            31                                   /*  ADC 中断                    */

/*********************************************************************************************************
  初始化函数
*********************************************************************************************************/
void  sdramInit(void);
void  targetInit(void);

#endif                                                                  /*  __TARGETINIT_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
