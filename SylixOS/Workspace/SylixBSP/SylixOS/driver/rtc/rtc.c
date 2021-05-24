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
** 文   件   名: rtc.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 20 日
**
** 描        述: RTC 驱动
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
#include "time.h"
/*********************************************************************************************************
** Function name:           rtcSetTime
** Descriptions:            设置 RTC 时间
** input parameters:        prtcTimeNow       当前时间
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/12/20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static int  __rtcSetTime (PLW_RTC_FUNCS  prtcfuncs, time_t  *ptimeNow)
{
    INTREG       iregInterLevel;
    struct tm    tmNow;
    
    int     iHigh;                                                      /*  BCD 码高位                  */
    int     iLow;                                                       /*  BCD 码低位                  */
    
    gmtime_r(ptimeNow, &tmNow);                                         /*  转换成 tm 时间格式          */
    
    API_InterLock(&iregInterLevel);
    
    rRTCCON = (unsigned char)((rRTCCON & ~(0xf)) | 0x1);
    
    iHigh   = tmNow.tm_sec / 10;
    iLow    = tmNow.tm_sec % 10;
    rBCDSEC = (unsigned char)((iHigh << 4) + iLow);                     /*  秒寄存器                    */
    
    iHigh   = tmNow.tm_min / 10;
    iLow    = tmNow.tm_min % 10;
    rBCDMIN = (unsigned char)((iHigh << 4) + iLow);                     /*  分寄存器                    */
    
    iHigh    = tmNow.tm_hour / 10;
    iLow     = tmNow.tm_hour % 10;
    rBCDHOUR = (unsigned char)((iHigh << 4) + iLow);                    /*  小时寄存器                  */
    
    iHigh    = tmNow.tm_mday / 10;
    iLow     = tmNow.tm_mday % 10;
    rBCDDATE = (unsigned char)((iHigh << 4) + iLow);                    /*  日期寄存器                  */
    
    iHigh    = (tmNow.tm_mon + 1) / 10;
    iLow     = (tmNow.tm_mon + 1) % 10;
    rBCDMON  = (unsigned char)((iHigh << 4) + iLow);                    /*  月份寄存器                  */
    
    iHigh    = (tmNow.tm_year + 1900 - 2000) / 10;
    iLow     = (tmNow.tm_year + 1900 - 2000) % 10;
    rBCDYEAR = (unsigned char)((iHigh << 4) + iLow);                    /*  年寄存器                    */

    rBCDDAY  = (unsigned char)(tmNow.tm_wday + 1);                      /*  星期寄存器   2440: 1~7      */

    rRTCCON  = 0x0;

    API_InterUnlock(iregInterLevel);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           rtcGetTime
** Descriptions:            获取 RTC 时间
** input parameters:        NONE
** output parameters:       prtcTimeNow       当前时间
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/12/20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static int  __rtcGetTime (PLW_RTC_FUNCS  prtcfuncs, time_t  *ptimeNow)
{
    struct tm    tmNow;
    
    int     iHigh;                                                      /*  BCD 码高位                  */
    int     iLow;                                                       /*  BCD 码低位                  */
    
    rRTCCON = (unsigned char)((rRTCCON & ~(0xf)) | 0x1);
    
    iHigh = ((rBCDSEC & 0xF0) >> 4);
    iLow  =  (rBCDSEC & 0x0F);
    tmNow.tm_sec = (unsigned char)(iHigh * 10 + iLow);
    
    iHigh = ((rBCDMIN & 0xF0) >> 4);
    iLow  =  (rBCDMIN & 0x0F);
    tmNow.tm_min = (unsigned char)(iHigh * 10 + iLow);
    
    iHigh = ((rBCDHOUR & 0xF0) >> 4);
    iLow  =  (rBCDHOUR & 0x0F);
    tmNow.tm_hour = (unsigned char)(iHigh * 10 + iLow);
    
    iHigh = ((rBCDDATE & 0xF0) >> 4);
    iLow  =  (rBCDDATE & 0x0F);
    tmNow.tm_mday = (unsigned char)(iHigh * 10 + iLow);
    
    iHigh = ((rBCDMON & 0xF0) >> 4);
    iLow  =  (rBCDMON & 0x0F);
    tmNow.tm_mon = (unsigned char)((iHigh * 10 + iLow) - 1);
    
    iHigh = ((rBCDYEAR & 0xF0) >> 4);
    iLow  =  (rBCDYEAR & 0x0F);
    tmNow.tm_year = (unsigned short)(iHigh * 10 + iLow + 2000 - 1900);
    
    tmNow.tm_wday = (unsigned char)(rBCDDAY - 1);
    
    rRTCCON  = 0x0;
    
    if (ptimeNow) {
        *ptimeNow = timegm(&tmNow);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           rtcGetFuncs
** Descriptions:            获取 RTC 驱动程序集
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          pfuncs      驱动程序集
** Created by:              Hanhui
** Created Date:            2007/12/20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
PLW_RTC_FUNCS   rtcGetFuncs (VOID)
{
    static LW_RTC_FUNCS    rtcfuncs = {LW_NULL, __rtcSetTime, __rtcGetTime, LW_NULL};
    
    return  (&rtcfuncs);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
