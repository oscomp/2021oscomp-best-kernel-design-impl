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
** 文   件   名: uart.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 20 日
**
** 描        述: uart 驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"
#include "uart.h"
#include "SylixOS.h"
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define __COM0_CLKBIT       (1 << 10)                                   /*  COM0 在 CLKCON 中的位置     */
#define __COM1_CLKBIT       (1 << 11)                                   /*  COM1 在 CLKCON 中的位置     */
#define __COM2_CLKBIT       (1 << 12)                                   /*  COM2 在 CLKCON 中的位置     */

#define __COM0_GPIO         ((1 << 2) | (1 << 3))                       /*  COM0 在 IO 口线中的位置     */
#define __COM1_GPIO         ((1 << 4) | (1 << 5))                       /*  COM1 在 IO 口线中的位置     */
#define __COM2_GPIO         ((1 << 6) | (1 << 7))                       /*  COM2 在 IO 口线中的位置     */

#define __COM0_GPHCON       ((0x2 <<  4) | (0x2 <<  6))                 /*  COM0 在 GPHCON 中的设置     */
#define __COM1_GPHCON       ((0x2 <<  8) | (0x2 << 10))                 /*  COM1 在 GPHCON 中的设置     */
#define __COM2_GPHCON       ((0x2 << 12) | (0x2 << 14))                 /*  COM1 在 GPHCON 中的设置     */

#define __COM0_MASK         ((0x3 <<  4) | (0x3 <<  6))                 /*  COM0 在 GPHCON 中的区域     */
#define __COM1_MASK         ((0x3 <<  8) | (0x3 << 10))                 /*  COM1 在 GPHCON 中的区域     */
#define __COM2_MASK         ((0x3 << 12) | (0x3 << 14))                 /*  COM2 在 GPHCON 中的区域     */
/*********************************************************************************************************
** Function name:           uartInit
** Descriptions:            初始化 UART
** input parameters:        iCom                串口号
**                          iInFrared           是否使用红外模式
**                          iData               数据位数
**                          iStopBit            结束位
**                          iCheck              校验方法
**                          iBaud               波特率
**                          pvIsrRoutine        中断服务函数, 为NULL表示不允许中断
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int     uartInit (int   iCom,
                  int   iInFrared,
                  int   iData,
                  int   iStopBit,
                  int   iCheck,
                  int   iBaud,
                  void *pvIsrRoutine)
{
    unsigned int        uiUBRDIVn;                                      /*  波特率发生器值              */
    unsigned int        uiULCONn;                                       /*  线控制器值                  */
    unsigned int        uiUCONn;                                        /*  串口控制寄存器值            */
    unsigned int        uiUFCONn;                                       /*  FIFO 控制寄存器值           */
    
    unsigned int        uiInterEn;                                      /*  是否允许中断                */
    
    if (iData < 5) {                                                    /*  数据位数错误                */
        return  (-1);
    }
    iData -= 5;                                                         /*  确定寄存器中的值            */
    
    uiInterEn = (pvIsrRoutine == (void *)0) ? 0 : 1;                    /*  确定是否需要中断            */
    
    iBaud = (iBaud << 4);                                               /*  baud = baud * 16            */
    iBaud = PCLK / iBaud;
    iBaud = (int)(iBaud - 0.5);
    
    uiUBRDIVn = iBaud;                                                  /*  波特率                      */
    
    uiULCONn  = ((iInFrared << 6)
              |  (iCheck    << 3)
              |  (iStopBit  << 2)
              |  (iData));                                              /*  基本控制信息                */
    
    uiUCONn   = ((0x00 << 10)                                           /*  PCLK                        */
              |  (1 << 9)                                               /*  Tx Interrupt Type LEVEL     */
              |  (1 << 8)                                               /*  Rx Interrupt Type LEVEL     */
              |  (1 << 7)                                               /*  Rx Time Out Enable          */
              |  (0 << 6)                                               /*  Rx Error Status             */
                                                                        /*  Interrupt Disable           */
              |  (0 << 5)                                               /*  Loopback Mode Disable       */
              |  (0 << 4)
              |  (1 << 2)                                               /*  Tx Interrupt or poll        */
              |  (1));                                                  /*  Rx Interrupt or poll        */
    
    uiUFCONn  = ((0x0 << 6)                                             /*  Tx FIFO Trigger Level 0     */
              |  (0x2 << 4)                                             /*  Rx FIFO Trigger Level 16    */
              |  (1 << 2)                                               /*  Tx FIFO Reset               */
              |  (1 << 1)                                               /*  Rx FIFO Reset               */
              |  (1));                                                  /*  FIFO Enable                 */

    if (iCom == COM0) {                                                 /*  设置 UART0 的 管脚          */

        rGPHCON &= ~(__COM0_MASK);
        rGPHCON |=   __COM0_GPHCON;
        rGPHUP  &= ~(__COM0_GPIO);                                      /*  使用上拉电阻                */
        
        rCLKCON |=   __COM0_CLKBIT;                                     /*  时钟挂接                    */

        rUCON0   = 0;
        rUFCON0  = uiUFCONn;
        rUMCON0  = 0;                                                   /*  关闭流控                    */
        rULCON0  = uiULCONn;
        rUCON0   = uiUCONn;
        rUBRDIV0 = uiUBRDIVn;
        
        if (uiInterEn) {                                                /*  链接中断服务函数            */
            API_InterVectorConnect(VIC_CHANNEL_UART0, 
                                   (PINT_SVR_ROUTINE)pvIsrRoutine,
                                   LW_NULL,
                                   "uart0_isr");
            INTER_CLR_MSK((1u << VIC_CHANNEL_UART0));                   /*  解除屏蔽中断                */
            INTER_CLR_SUBMSK(BIT_SUB_RXD0);                             /*  打开接收中断                */
        }
    
    } else if (iCom == COM1) {                                          /*  设置 UART1 的 管脚          */
    
        rGPHCON &= ~(__COM1_MASK);
        rGPHCON |=   __COM1_GPHCON;
        rGPHUP  &= ~(__COM1_GPIO);                                      /*  使用上拉电阻                */
        
        rCLKCON |=   __COM1_CLKBIT;                                     /*  时钟挂接                    */
        
        rUCON1   = 0;
        rUFCON1  = uiUFCONn;
        rUMCON1  = 0;                                                   /*  关闭流控                    */
        rULCON1  = uiULCONn;
        rUCON1   = uiUCONn;
        rUBRDIV1 = uiUBRDIVn;
        
        if (uiInterEn) {                                                /*  链接中断服务函数            */
            API_InterVectorConnect(VIC_CHANNEL_UART1, 
                                   (PINT_SVR_ROUTINE)pvIsrRoutine,
                                   LW_NULL,
                                   "uart1_isr");
            INTER_CLR_MSK((1u << VIC_CHANNEL_UART1));                   /*  解除屏蔽中断                */
            INTER_CLR_SUBMSK(BIT_SUB_RXD1);                             /*  打开接收中断                */
        }
    
    } else if (iCom == COM2) {                                          /*  设置 UART2 的 管脚          */
        
        rGPHCON &= ~(__COM2_MASK);
        rGPHCON |=   __COM2_GPHCON;
        rGPHUP  &= ~(__COM2_GPIO);                                      /*  使用上拉电阻                */
        
        rCLKCON |=   __COM2_CLKBIT;                                     /*  时钟挂接                    */
        
        rUCON2   = 0;
        rUFCON2  = uiUFCONn;
        rUMCON2  = 0;                                                   /*  关闭流控                    */
        rULCON2  = uiULCONn;
        rUCON2   = uiUCONn;
        rUBRDIV2 = uiUBRDIVn;
        
        if (uiInterEn) {                                                /*  链接中断服务函数            */
            API_InterVectorConnect(VIC_CHANNEL_UART2, 
                                   (PINT_SVR_ROUTINE)pvIsrRoutine,
                                   LW_NULL,
                                   "uart2_isr");
            INTER_CLR_MSK((1u << VIC_CHANNEL_UART2));                   /*  解除屏蔽中断                */
            INTER_CLR_SUBMSK(BIT_SUB_RXD2);                             /*  打开接收中断                */
        }
    
    } else {                                                            /*  串口出错                    */
        
        return  (-1);
    }
    
    return  (0);
}
/*********************************************************************************************************
** Function name:           uartSendByte
** Descriptions:            UART 发送一个字节的数据
** input parameters:        iCom                串口号
**                          ucData              数据
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int     uartSendByte (int   iCom, unsigned char  ucData)
{
    switch (iCom) {
    
    case COM0:
        while (rUFSTAT0 & (1 << 14));
        while (((rUFSTAT0) >> 8) & 0x3F);                               /*  Tx_FIFO IS EMPTY            */
        WrUTXH0(ucData);
        break;
        
    case COM1:
        while (rUFSTAT1 & (1 << 14));
        while (((rUFSTAT1) >> 8) & 0x3F);                               /*  Tx_FIFO IS EMPTY            */
        WrUTXH1(ucData);
        break;
    
    case COM2:
        while (rUFSTAT2 & (1 << 14));
        while (((rUFSTAT2) >> 8) & 0x3F);                               /*  Tx_FIFO IS EMPTY            */
        WrUTXH2(ucData);
        break;
    
    default:                                                            /*  串口号错误                  */
        return  (1);
    }
    
    return  (0);
}
/*********************************************************************************************************
** Function name:           uartSendByteCnt
** Descriptions:            UART 发送指定长度的数据
** input parameters:        iCom                串口号
**                          pucData             数据缓冲区
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void    uartSendByteCnt (int             iCom, 
                         unsigned char  *pucData,
                         int             iCnt)
{
    for (; iCnt != 0; iCnt--) {
        uartSendByte(iCom, *pucData);                                   /*  发送数据                    */
        pucData++;
    }
}
/*********************************************************************************************************
** Function name:           uartSendString
** Descriptions:            UART 发送一个字符串
** input parameters:        iCom                串口号
**                          pcData              字符串
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void     uartSendString (int   iCom, char  *pcData)
{
    if (!pcData) {                                                      /*  指针为空                    */
        return;
    }
    
    while (*pcData != '\0') {                                           /*  发送字符串                  */
        uartSendByte(iCom, (unsigned char)*pcData);
        pcData++;
    }
}
/*********************************************************************************************************
** Function name:           debugChannelInit
** Descriptions:            初始化调试接口
** input parameters:        iChannelNum                 通道号
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void    debugChannelInit (int  iChannelNum)
{
    uartInit(iChannelNum, UNUSE_INF, 8, ONE_STOPBIT, CHK_NONE, 115200, (void *)0);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
