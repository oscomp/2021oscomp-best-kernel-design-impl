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
** 文   件   名: timer.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 12 月 09 日
**
** 描        述: S3C2440 硬件定时器.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"
#include "timer.h"
/*********************************************************************************************************
** Function name:           timerSetPrescaler0
** Descriptions:            设置定时器0 与 定时器1 的预分频因子
** input parameters:        ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  timerSetPrescaler0 (unsigned char   ucPrescaler)
{
    rTCFG0 &= 0xFFFFFF00;
    rTCFG0 |= ucPrescaler;
}
/*********************************************************************************************************
** Function name:           timerSetPrescaler1
** Descriptions:            设置定时器2 3 与定时器4 的预分频因子
** input parameters:        ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  timerSetPrescaler1 (unsigned char   ucPrescaler)
{
    rTCFG0 &= 0xFFFF00FF;
    rTCFG0 |= (unsigned int)(ucPrescaler << 8);
}
/*********************************************************************************************************
** Function name:           timerSetPrescaler
** Descriptions:            设置定时器预分频因子
** input parameters:        ucGroup         定时器组，0:定时器0,1   1:定时器 2,3,4
**                          ucPrescaler     新的预分频因子
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerSetPrescaler (unsigned char  ucGroup, unsigned char  ucPrescaler)
{
    switch (ucGroup) {
    
    case 0:                                                             /*  定时器组 0                  */
        timerSetPrescaler0(ucPrescaler);
        break;
        
    case 1:                                                             /*  定时器组 1                  */
        timerSetPrescaler1(ucPrescaler);
        break;
        
    default:                                                            /*  定时器组错误                */
        return  (-1);
    }
    
    return  (0);
}
/*********************************************************************************************************
** Function name:           timerSetDeadZone
** Descriptions:            设置 定时器0 的死区
** input parameters:        ucDeadZone      死区大小
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  timerSetDeadZone (unsigned char  ucDeadZone)
{
    rTCFG0 &= 0xFF00FFFF;
    rTCFG0 |= (unsigned int)(ucDeadZone << 16);
}
/*********************************************************************************************************
** Function name:           timerSetMuxCnt
** Descriptions:            设置定时器的分频器
** input parameters:        ucTimer             定时器
**                          ucDivider           分频值   0000 = 1/2  0001 = 1/4 0010 = 1/8
**                                                       0011 = 1/16 01xx = External TCLKx
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerSetMuxCnt (unsigned char  ucTimer, unsigned char  ucDivider)
{
    switch (ucTimer) {
    
    case 0:                                                             /*  定时器 0                    */
        rTCFG1 &= 0xFFFFFFF0;
        rTCFG1 |= ucDivider;
        break;
        
    case 1:                                                             /*  定时器 1                    */
        rTCFG1 &= 0xFFFFFF0F;
        rTCFG1 |= (ucDivider << 4);
        break;
        
    case 2:                                                             /*  定时器 2                    */
        rTCFG1 &= 0xFFFFF0FF;
        rTCFG1 |= (unsigned int)(ucDivider << 8);
        break;
        
    case 3:                                                             /*  定时器 3                    */
        rTCFG1 &= 0xFFFF0FFF;
        rTCFG1 |= (unsigned int)(ucDivider << 12);
        break;
        
    case 4:                                                             /*  定时器 4                    */
        rTCFG1 &= 0xFFF0FFFF;
        rTCFG1 |= (unsigned int)(ucDivider << 16);
        break;
        
    default:                                                            /*  定时器错误                  */
        return  (-1);
    }
    
    return  (0);
}
/*********************************************************************************************************
** Function name:           timerSetDMA
** Descriptions:            设置定时器的 DMA 链接
** input parameters:        连接DMA的定时器号   为 0 不连接， 1 为 TIMER0 依此类推
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerSetDMA (unsigned char ucTimer)
{
    if (ucTimer > 5) {
        return  (-1);
    }
    
    rTCFG1 &= 0xFF0FFFFF;
    rTCFG1 |= (unsigned int)(ucTimer << 20);
    
    return  (0);
}
/*********************************************************************************************************
** Function name:           timerConfig
** Descriptions:            设置定时器控制选项
** input parameters:        ucTimer             定时器号
**                          ucOption            选项(timer.h)
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerConfig (unsigned char  ucTimer, unsigned char   ucOption)
{
    switch (ucTimer) {
    
    case 0:                                                             /*  定时器 0                    */
        rTCON &= 0xFFFFFF00;
        rTCON |= ucOption;
        break;
        
    case 1:                                                             /*  定时器 1                    */
        rTCON &= 0xFFFFF0FF;
        rTCON |= (unsigned int)(ucOption << 8);
        break;
        
    case 2:                                                             /*  定时器 2                    */
        rTCON &= 0xFFFF0FFF;
        rTCON |= (unsigned int)(ucOption << 12);
        break;
        
    case 3:                                                             /*  定时器 3                    */
        rTCON &= 0xFFF0FFFF;
        rTCON |= (unsigned int)(ucOption << 16);
        break;
        
    case 4:                                                             /*  定时器 4                    */
        rTCON &= 0xFF0FFFFF;
        
        if (ucOption &  TIMER_RELOAD) {                                 /*  timer4 没有 INVERTER 位     */
            ucOption &= 0x07;
            ucOption |= TIMER_INVERTER;                                 /*  INVERTER 位 决定 Reload     */
        }
        
        rTCON |= (unsigned int)(ucOption << 20);
        break;
    
    default:
        return  (-1);
    }
        
    return  (0);
}
/*********************************************************************************************************
** Function name:           timerSetCnt
** Descriptions:            设置定时器初始计数值
** input parameters:        ucTimer             定时器号
**                          usCnt               计数值
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerSetCnt (unsigned char  ucTimer, unsigned short   usCnt)
{
    switch (ucTimer) {
    
    case 0:                                                             /*  定时器 0                    */
        rTCNTB0 = usCnt;
        break;
    
    case 1:                                                             /*  定时器 1                    */
        rTCNTB1 = usCnt;
        break;
        
    case 2:                                                             /*  定时器 2                    */
        rTCNTB2 = usCnt;
        break;
        
    case 3:                                                             /*  定时器 3                    */
        rTCNTB3 = usCnt;
        break;
        
    case 4:                                                             /*  定时器 4                    */
        rTCNTB4 = usCnt;
        break;
        
    default:
        return  (-1);
    }
    
    return   (0);
}
/*********************************************************************************************************
** Function name:           timerSetCmp
** Descriptions:            设置定时器比较值
** input parameters:        ucTimer             定时器号
**                          usCnt               比较值
** output parameters:       NONE
** Returned value:          正确返回 0,  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerSetCmp (unsigned char  ucTimer, unsigned short   usCnt)
{
    switch (ucTimer) {
    
    case 0:                                                             /*  定时器 0                    */
        rTCMPB0 = usCnt;
        break;
    
    case 1:                                                             /*  定时器 1                    */
        rTCMPB1 = usCnt;
        break;
        
    case 2:                                                             /*  定时器 2                    */
        rTCMPB2 = usCnt;
        break;
        
    case 3:                                                             /*  定时器 3                    */
        rTCMPB3 = usCnt;
        break;
        
    default:                                                            /*  没有定时器 4                */
        return  (-1);
    }
    
    return   (0);
}
/*********************************************************************************************************
** Function name:           timerGetCnt
** Descriptions:            获得定时器计数值
** input parameters:        ucTimer             定时器号
** output parameters:       NONE
** Returned value:          计数值  错误返回 -1
** Created by:              Hanhui
** Created Date:            2007/09/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
int  timerGetCnt (unsigned char  ucTimer)
{
    switch (ucTimer) {
    
    case 0:                                                             /*  定时器 0                    */
        return  (rTCNTO0);
        
    case 1:                                                             /*  定时器 1                    */
        return  (rTCNTO1);
        
    case 2:                                                             /*  定时器 2                    */
        return  (rTCNTO2);
        
    case 3:                                                             /*  定时器 3                    */
        return  (rTCNTO3);
        
    case 4:                                                             /*  定时器 4                    */
        return  (rTCNTO4);
        
    default:
        return  (0);
    }
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
