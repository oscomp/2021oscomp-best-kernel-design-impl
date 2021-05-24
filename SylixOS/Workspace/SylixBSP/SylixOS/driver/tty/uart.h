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
** 文   件   名: uart.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 20 日
**
** 描        述: uart 驱动.
*********************************************************************************************************/

#ifndef __UART_H
#define __UART_H

/*********************************************************************************************************
  MACRO
*********************************************************************************************************/
#define COM0            0                                               /*  串口 0                      */
#define COM1            1                                               /*  串口 1                      */
#define COM2            2                                               /*  串口 2                      */

#define CHK_ODD         0x4                                             /*  奇校验                      */
#define CHK_EVEN        0x5                                             /*  偶校验                      */
#define CHK_NONE        0x0                                             /*  没有校验                    */

#define USE_INF         1                                               /*  使用红外模式                */
#define UNUSE_INF       0                                               /*  不使用红外模式              */

#define ONE_STOPBIT     0                                               /*  一个停止位                  */
#define TWO_STOPBIT     1                                               /*  两个停止位                  */
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
*********************************************************************************************************/
int  uartInit(int   iCom,
              int   iInFrared,
              int   iData,
              int   iStopBit,
              int   iCheck,
              int   iBaud,
              void *pvIsrRoutine);
                  
/*********************************************************************************************************
** Function name:           uartSendByte
** Descriptions:            UART 发送一个字节的数据
** input parameters:        iCom                串口号
**                          ucData              数据
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int  uartSendByte(int  iCom, unsigned char  ucData);

/*********************************************************************************************************
** Function name:           uartSendByteCnt
** Descriptions:            UART 发送指定长度的数据
** input parameters:        iCom                串口号
**                          pucData             数据缓冲区
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void uartSendByteCnt(int             iCom, 
                     unsigned char  *pucData,
                     int             iCnt);
                         
/*********************************************************************************************************
** Function name:           uartSendString
** Descriptions:            UART 发送一个字符串
** input parameters:        iCom                串口号
**                          pcData              字符串
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void uartSendString(int   iCom, char  *pcData);

/*********************************************************************************************************
** Function name:           debugChannelInit
** Descriptions:            初始化调试接口
** input parameters:        iChannelNum                 通道号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void debugChannelInit(int   iChannelNum);

#endif                                                                  /*  __UART_H                    */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
