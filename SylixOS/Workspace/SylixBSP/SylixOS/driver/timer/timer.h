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
** 文   件   名: timer.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 12 月 09 日
**
** 描        述: S3C2440 硬件定时器.
*********************************************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

/*********************************************************************************************************
  定时器配置参数
*********************************************************************************************************/

#define TIMER_ENABLE            (1 << 0)                                /*  启动定时器                  */
#define TIMER_DISABLE           (0 << 0)                                /*  停止定时器                  */

#define TIMER_MANUAL_UPDATE     (1 << 1)                                /*  定时器手动重载              */
#define TIMER_UNMANUAL_UPDATE   (0 << 1)                                /*  定时器自动重载              */

#define TIMER_INVERTER          (1 << 2)                                /*  定时器输出翻转              */
#define TIMER_UNINVERTER        (0 << 2)                                /*  定时器输出不翻转            */

#define TIMER_RELOAD            (1 << 3)                                /*  循环计数                    */
#define TIMER_UNRELOAD          (0 << 3)                                /*  单次计数                    */

#define TIMER_DEADZONE          (1 << 4)                                /*  定时器0使用死区             */
#define TIMER_UNDEAD_ZONE       (0 << 4)                                /*  定时器0不使用死区           */

/*********************************************************************************************************
  定时器操作函数
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           timerSetPrescaler0
** Descriptions:            设置定时器0 与 定时器1 的预分频因子
** input parameters:        ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void    timerSetPrescaler0(unsigned char   ucPrescaler);
/*********************************************************************************************************
** Function name:           timerSetPrescaler1
** Descriptions:            设置定时器2 3 与定时器4 的预分频因子
** input parameters:        ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void    timerSetPrescaler1(unsigned char   ucPrescaler);
/*********************************************************************************************************
** Function name:           timerSetPrescaler
** Descriptions:            设置定时器预分频因子
** input parameters:        ucGroup         定时器组，0:定时器0,1   1:定时器 2,3,4
**                          ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerSetPrescaler(unsigned char  ucGroup, unsigned char  ucPrescaler);
/*********************************************************************************************************
** Function name:           timerSetDeadZone
** Descriptions:            设置 定时器0 的死区
** input parameters:        ucDeadZone      死区大小
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void    timerSetDeadZone(unsigned char  ucDeadZone);
/*********************************************************************************************************
** Function name:           timerSetMuxCnt
** Descriptions:            设置定时器的分频器
** input parameters:        ucTimer             定时器
**                          ucDivider           分频值   0000 = 1/2  0001 = 1/4 0010 = 1/8
**                                                       0011 = 1/16 01xx = External TCLKx
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerSetMuxCnt(unsigned char  ucTimer, unsigned char  ucDivider);
/*********************************************************************************************************
** Function name:           timerSetDMA
** Descriptions:            设置定时器的 DMA 链接
** input parameters:        连接DMA的定时器号   为 0 不连接， 1 为 TIMER0 依此类推
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerSetDMA(unsigned char ucTimer);
/*********************************************************************************************************
** Function name:           timerConfig
** Descriptions:            设置定时器控制选项
** input parameters:        ucTimer             定时器号
**                          ucOption            选项(timer.h)
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerConfig(unsigned char  ucTimer, unsigned char   ucOption);
/*********************************************************************************************************
** Function name:           timerSetCnt
** Descriptions:            设置定时器初始计数值
** input parameters:        ucTimer             定时器号
**                          usCnt               计数值
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerSetCnt(unsigned char  ucTimer, unsigned short   usCnt);
/*********************************************************************************************************
** Function name:           timerSetCmp
** Descriptions:            设置定时器比较值
** input parameters:        ucTimer             定时器号
**                          usCnt               比较值
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
*********************************************************************************************************/
int     timerSetCmp(unsigned char  ucTimer, unsigned short   usCnt);
/*********************************************************************************************************
** Function name:           timerGetCnt
** Descriptions:            获得定时器计数值
** input parameters:        ucTimer             定时器号
** output parameters:       NONE
** Returned value:          计数值  错误返回 -1
*********************************************************************************************************/
int     timerGetCnt(unsigned char  ucTimer);

#endif                                                                  /*  __TIMER_H                   */
/*********************************************************************************************************
  END
*********************************************************************************************************/

