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
** 文   件   名: mciLib.c
**
** 创   建   人: Lu.ZhenPing (卢振平)
**
** 文件创建日期: 2014 年 10 月 15 日
**
** 描        述: 多媒体卡接口库

** BUG:
2014.11.06  修改__mciIrq() 在命令正确返回前(__close_xfer)未设置返回码的BUG.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include "2440addr.h"
#include "targetInit.h"
#include "config.h"
#include "mciLib.h"
/*********************************************************************************************************
  调试宏定义
*********************************************************************************************************/
#ifdef  DEBUG_SDI
#define SDI_DBG(fmt, args...)         _DebugFormat(__LOGMESSAGE_LEVEL, fmt, ##args)
#define SDI_ERR(fmt, args...)         _DebugFormat(__ERRORMESSAGE_LEVEL, fmt, ##args)
#else
#define SDI_DBG(fmt, args...)
#define SDI_ERR(fmt, args...)
#endif
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define SDIO_PRIO                     (LW_PRIO_NORMAL - 3)
#define XFER_NONE                     (0)
#define XFER_WRITE                    (1)
#define XFER_READ                     (2)
#define MCI_IRQ                       (VIC_CHANNEL_SDI)

#define SDI_SDIOINT_NOTIFY(psdio)                           \
        do {                                                \
            if (psdio->SDIO_userFunc) {                     \
                psdio->SDIO_userFunc(psdio->SDIO_ulVector); \
            }                                               \
            sdioIntEnable(psdio, 1);                        \
        } while (0)

#define SDI_USE_SDIOINT_NOTIFY        0
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
static INT  __mciFifoWrite(PSDIO_DAT  psdio);
static INT  __mciFifoRead(PSDIO_DAT  psdio);
static INT  __mciWakeupQueue(LW_OBJECT_HANDLE  hq);
       VOID __mciHwInit (VOID);
/*********************************************************************************************************
** 函数名称: __mciEnableIntMask
** 功能描述: SDIO 中断掩码使能
** 输　  入: uiMskVal  使能掩码的位
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciEnableIntMask (UINT32  uiMskVal)
{
    volatile UINT32  uiIMsk = rSDIIMSK;

    uiIMsk |= uiMskVal;
    rSDIIMSK = uiIMsk;
}
/*********************************************************************************************************
** 函数名称: __mciDisableIntMask
** 功能描述: MCI 中断掩码禁能
** 输　  入: uiMskVal  禁能掩码的位
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciDisableIntMask (UINT32  uiMskVal)
{
    volatile UINT32  uiIMsk = rSDIIMSK;

    uiIMsk &= ~uiMskVal;
    rSDIIMSK = uiIMsk;
}
/*********************************************************************************************************
** 函数名称: __mciClearMsk
** 功能描述: 清除中断掩码，保留 SDIO 中断
** 输　  入: NONE
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciClearMsk (VOID)
{
    volatile UINT32  uiIMsk = rSDIIMSK;

    uiIMsk &= SDI_IMSK_SDI_OIRQ;                                           /*  保留 SDIO 中断           */
    rSDIIMSK = uiIMsk;
}
/*********************************************************************************************************
** 函数名称: __mciDisableIrq
** 功能描述: 禁能主中断
** 输　  入: psdio    SDIO 结构
**           bTrans   禁能标志
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciDisableIrq (PSDIO_DAT  psdio, INT bTrans)
{
    INTREG  iRegInterLevel;

    LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);

    psdio->SDIO_bIrqDisabled = bTrans;

    if (bTrans && psdio->SDIO_bIrqState) {
        psdio->SDIO_bIrqState = 0;
        API_InterVectorDisable(psdio->SDIO_ulVector);
    }

    LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
}
/*********************************************************************************************************
** 函数名称: __mciGpioGetValue
** 功能描述: 获得 GPIO 的值
** 输　  入: uiMskVal  使能掩码的位
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static INT  __mciGpioGetValue (INT  iGpioVal)
{
    volatile UINT32  uiGpeData = rGPEDAT;

    return  (uiGpeData & (1 << iGpioVal));
}
/*********************************************************************************************************
** 函数名称: __mciCheckSdioIrq
** 功能描述: 检查 SDIO 中断
** 输　  入: psdio   SDIO 结构
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciCheckSdioIrq (PSDIO_DAT  psdio)
{
    INT  iRet;

    if (psdio->SDIO_bSdioIrqEn) {
        /*
         *  GPE8[DATA1] 低电平时是 SDIO 中断
         */
        if ((iRet = __mciGpioGetValue(GPE8)) == 0) {
            sdioIntEnable(psdio, 0);
            __mciWakeupQueue(psdio->SDIO_hSdioSync);
        }
    }
}
/*********************************************************************************************************
** 函数名称: __mciEnableIrq
** 功能描述: 使能主中断
** 输　  入: psdio  SDIO 结构
**           bMore  是否有更多的中断
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciEnableIrq (PSDIO_DAT  psdio, INT bMore)
{
    BOOL    bEnable = FALSE;
    INTREG  iRegInterLevel;

    LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);

    psdio->SDIO_bIrqEnabled = bMore;
    psdio->SDIO_bIrqDisabled = 0;

    bEnable = bMore | psdio->SDIO_bSdioIrqEn;

    if (psdio->SDIO_bIrqState != bEnable) {
        psdio->SDIO_bIrqState = bEnable;

        if (bEnable) {
            API_InterVectorEnable(psdio->SDIO_ulVector);
        } else {
            API_InterVectorDisable(psdio->SDIO_ulVector);
        }
    }

    LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
}
/*********************************************************************************************************
** 函数名称: sdioIntEnable
** 功能描述: SDIO 使能中断
** 输　  入: psdio    SDIO 结构
**           iEn      使能标志
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioIntEnable (PSDIO_DAT  psdio, INT  iEn)
{
    UINT32  uiCon;
    INTREG  iRegInterLevel;

    LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);

    uiCon = rSDICON;

    if (iEn == psdio->SDIO_bSdioIrqEn) {
        goto __same_state;
    }

    psdio->SDIO_bSdioIrqEn = iEn;

    if (iEn) {
        /*
         *  INT ENABLE
         */
        uiCon |= SDI_CON_SDI_OIRQ;
        __mciEnableIntMask(SDI_IMSK_SDI_OIRQ);

        if (!psdio->SDIO_bIrqState && !psdio->SDIO_bIrqDisabled) {
            psdio->SDIO_bIrqState = 1;
            API_InterVectorEnable(psdio->SDIO_ulVector);
        }
    } else {
        /*
         *  INT DISABLE
         */
        __mciDisableIntMask(SDI_IMSK_SDI_OIRQ);
        uiCon &= ~SDI_CON_SDI_OIRQ;

        if (!psdio->SDIO_bIrqEnabled && psdio->SDIO_bIrqState) {
            API_InterVectorDisable(psdio->SDIO_ulVector);
            psdio->SDIO_bIrqState = 0;
        }
    }

    rSDICON = uiCon;

__same_state:
    LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);

    __mciCheckSdioIrq(psdio);
}
/*********************************************************************************************************
** 函数名称: __mciRequestDone
** 功能描述: MCI 发送请求完成
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  __mciRequestDone (PSDIO_DAT  psdio)
{
    INT  iCmdError = psdio->SDIO_uiCmdError;

    if (iCmdError && psdio->SDIO_psdcmd->SDCMD_uiRetry) {
        SDI_ERR("(cmd:%u) request failed. retry....\r\n", psdio->SDIO_psdcmd->SDCMD_uiOpcode);
        psdio->SDIO_psdcmd->SDCMD_uiRetry--;
        psdio->SDIO_uiCmdError = ERROR_NONE;
        if (psdio->SDIO_request.MCIDRV_request) {
            psdio->SDIO_request.MCIDRV_request(psdio);
        }
    } else {
        SDI_DBG("(cmd:%u) request done.\r\n", psdio->SDIO_psdcmd->SDCMD_uiOpcode);
        __mciWakeupQueue(psdio->SDIO_hComplete);
    }
}
/*********************************************************************************************************
** 函数名称: __mciCardPresent
** 功能描述: 探测 MMC 卡
** 输　  入: NONE
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static INT  __mciCardPresent (PSDIO_DAT  psdio)
{
    /*
     *  TODO 实现卡的探测功能
     */
    return  (0^1);
}
/*********************************************************************************************************
** 函数名称: __mciDataStop
** 功能描述: 数据传输停止
** 输　  入: psdio  SDIO 结构
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciDataStop (PSDIO_DAT  psdio)
{
    (VOID)psdio;

    rSDIDCON = SDI_DCON_STOP;                                           /*  停止                        */
}
/*********************************************************************************************************
** 函数名称: sdioReset
** 功能描述: card 复位
** 输　  入: psdio  SDIO 结构
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioReset (PSDIO_DAT  psdio)
{
    volatile UINT32   uiCon = rSDICON;

    (VOID)psdio;

    sdioStopClock();
    __mciHwInit();
    uiCon   |= SDI_CON_SDRESET;
    rSDICON  = uiCon;
    sdioStartClock();
}
/*********************************************************************************************************
** 函数名称: __sdioHwInit
** 功能描述: 硬件初始化
** 输　  入: NONE
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID __mciHwInit (VOID)
{
    rGPEUP    = SDI_GPIO_PULLEN;                                        /*  SDDAT[0:3] pull disable     */
    rGPECON   = SDI_GPIO_FUNC;                                          /*  SDCMD, SDDAT[3:0]           */

    rSDICON   = SDI_CON_BYTEORDER |                                     /*  数据传输类型为B             */
                SDI_CON_MMCCLOCK  ;                                     /*  时钟类型 MMC                */

    rSDIFSTA  = SDI_FSTA_FIFORESET;                                     /*  FIFO复位                    */
    rSDIDSTA  = SDI_DATSTAT_RESET;                                      /*  清除SDI数据状态寄存器所有位 */
}
/*********************************************************************************************************
** 函数名称: __s3csdIoSetClock
** 功能描述: 设置 SDIO 时钟值
** 输　  入: uiRate   频率值
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioSetClock (UINT32 uiRate)
{
    sdioStopClock();
    rSDIPRE = (PCLK + uiRate - 1) / uiRate - 1;                         /*  将频率值写入分频寄存器      */
    sdioStartClock();
}
/*********************************************************************************************************
** 函数名称: sdioStopClock
** 功能描述: SDIO 时钟关
** 输　  入: NONE
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioStopClock (VOID)
{
    rSDICON &= ~SDI_CON_CLOCKTYPE;                                      /*  禁能时钟                    */
}
/*********************************************************************************************************
** 函数名称: sdioStartClock
** 功能描述: SDIO 时钟开
** 输　  入: NONE
** 输　  出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioStartClock (VOID)
{
    rSDICON |= SDI_CON_CLOCKTYPE;                                       /*  使能时钟                    */
}
/*********************************************************************************************************
** 函数名称: __s3csdIoSetBusWidth
** 功能描述: SD主控总线位宽设置
** 输    入: iBusWidth
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT sdioSetBusWidth (INT iBusWidth)
{
    if (SDARG_SETBUSWIDTH_4 == iBusWidth) {
        rSDIDCON |=  SDI_DCON_WIDEBUS;
    } else {
        rSDIDCON &= ~SDI_DCON_WIDEBUS;
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __s3cSendCmd
** 功能描述: 发送命令
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT __mciSendCmd (PSDIO_DAT  psdio)
{
    volatile UINT32  uiCmdCon = 0;
    volatile UINT32  uiIMsk   = 0;

    uiIMsk = SDI_IMSK_CRCSTATUS  |
             SDI_IMSK_CMDTIMEOUT |
             SDI_IMSK_RESPONSEND |
             SDI_IMSK_CMDSENT    |
             SDI_IMSK_RESPONSECRC;

    __mciEnableIntMask(uiIMsk);

    if (psdio->SDIO_bData) {
        psdio->SDIO_eCompleteWhat = COMPLETION_XFERFINISH_RSPFIN;
    } else if (SD_CMD_TEST_RSP((psdio->SDIO_psdcmd), SD_RSP_PRESENT)) {
        psdio->SDIO_eCompleteWhat = COMPLETION_RSPFIN;
    } else {
        psdio->SDIO_eCompleteWhat = COMPLETION_CMDSENT;
    }

    rSDICARG = psdio->SDIO_psdcmd->SDCMD_uiArg;                         /*  写入参数寄存器              */

    uiCmdCon  = (psdio->SDIO_psdcmd->SDCMD_uiOpcode & SDI_CCON_INDEX) | SDI_CCON_SENDERHOST;
    uiCmdCon |= SDI_CCON_CMDSTART;

    if (SD_CMD_TEST_RSP((psdio->SDIO_psdcmd), SD_RSP_PRESENT)) {
        uiCmdCon |= SDI_CCON_WAITRSP;
    }

    if (SD_CMD_TEST_RSP((psdio->SDIO_psdcmd), SD_RSP_136)) {
        uiCmdCon |=  SDI_CCON_LONGRSP;
    }

    rSDICCON = uiCmdCon;

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __handleIntTask
** 功能描述: 处理中断的请求任务
** 输    入: psdio    读入数据字节数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  __handleIntTask (PSDIO_DAT  psdio)
{
    if (psdio->SDIO_eCompleteWhat != COMPLETION_FINALIZE) {
        return  (PX_ERROR);
    }

    if (psdio->SDIO_bData) {
        if (psdio->SDIO_ucDmaEn) {
            return  (ERROR_NONE);
        }
    }

    if (psdio->SDIO_bData &&
        !psdio->SDIO_uiCmdError &&
        !psdio->SDIO_uiDataError)  {
        if (psdio->SDIO_ucDmaEn) {
            return  (ERROR_NONE);
        }
    }

    psdio->SDIO_psdcmd->SDCMD_uiResp[0] = rSDIRSP0;
    psdio->SDIO_psdcmd->SDCMD_uiResp[1] = rSDIRSP1;
    psdio->SDIO_psdcmd->SDCMD_uiResp[2] = rSDIRSP2;
    psdio->SDIO_psdcmd->SDCMD_uiResp[3] = rSDIRSP3;

    /*
     *  Cleanup controller
     */
    rSDICARG = 0;
    __mciDataStop(psdio);                                               /*  停止                        */
    rSDICCON = 0;
    __mciClearMsk();

    if (psdio->SDIO_bData && psdio->SDIO_uiCmdError) {
        psdio->SDIO_uiDataError = psdio->SDIO_uiCmdError;
    }

    /*
     *  如果我们没有数据传输，发送完成
     */
    if (!psdio->SDIO_bData) {
        goto __req_done;
    }
    /*
     *  如果在传输的时候有任何错误我们刷新 FIFO 或 DMA
     */
    if (psdio->SDIO_uiDataError) {
        if (psdio->SDIO_ucDmaEn) {
            /*
             *  TODO 刷新 DMA
             */
        }
        rSDIFSTA = SDI_FSTA_FIFORESET | SDI_FSTA_FIFOFAIL;
    }

__req_done:
    psdio->SDIO_eCompleteWhat = COMPLETION_NONE;
    __mciCheckSdioIrq(psdio);
    __mciRequestDone(psdio);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciIoWaitQueue
** 功能描述: 等队列
** 输    入: hq    队列 Handler ID
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciIoWaitQueue (LW_OBJECT_HANDLE  hq)
{
    ULONG       ulRecv;
    size_t      stLen  = 0;
    ULONG       iRet;

    iRet = API_MsgQueueReceive(hq,
                               (PVOID)&ulRecv,
                               sizeof(ULONG),
                               &stLen,
                               LW_OPTION_WAIT_INFINITE);
    if (iRet != ERROR_NONE) {
        SDI_ERR("Wait happen error.\r\n");
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciWakeupQueue
** 功能描述: 唤醒队列
** 输    入: hq    队列 Handler ID
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciWakeupQueue (LW_OBJECT_HANDLE  hq)
{
    ULONG  ulMsg = 1;
    INT    iRet;

    iRet = API_MsgQueueSend(hq, (PVOID)&ulMsg, sizeof(ulMsg));
    if (iRet != ERROR_NONE) {
        SDI_ERR("iret:%d\n", iRet);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciIoTasklet
** 功能描述: 处理中断请求任务
** 输    入: pvArg    线程参数
** 输    出: NONE
** 返    回: LW_NULL(不会主动返回)
*********************************************************************************************************/
static PVOID  __mciIoTasklet (PVOID  pvArg)
{
    PSDIO_DAT  psdio = (PSDIO_DAT)pvArg;

    if (!psdio) {
        SDI_ERR("Invalid thread args.\n");
        return  (LW_NULL);
    }

    for (;;) {
        __mciIoWaitQueue(psdio->SDIO_hTaskSync);

        __mciDisableIrq(psdio, 1);

        if (psdio->SDIO_iIoAct == XFER_WRITE) {
            __mciFifoWrite(psdio);
        }

        if (psdio->SDIO_iIoAct == XFER_READ) {
            __mciFifoRead(psdio);
        }

        if (psdio->SDIO_eCompleteWhat == COMPLETION_FINALIZE) {
            __mciClearMsk();
            if (psdio->SDIO_iIoAct != XFER_NONE) {
                SDI_DBG("Unfinished %d", (psdio->SDIO_iIoAct == XFER_READ) ? "Read" : "Write");
                if (psdio->SDIO_bData) {
                    psdio->SDIO_uiDataError = EINVAL;
                }
            }

            __mciEnableIrq(psdio, 0);
            __handleIntTask(psdio);

        } else {
            __mciEnableIrq(psdio, 1);
        }
    }

    return  (LW_NULL);
}
/*********************************************************************************************************
** 函数名称: __mciSdioThread
** 功能描述: 处理 SDIO 中断
** 输    入: pvArg    线程参数
** 输    出: NONE
** 返    回: LW_NULL(不会主动返回)
*********************************************************************************************************/
static PVOID  __mciSdioThread (PVOID  pvArg)
{
    PSDIO_DAT  psdio = (PSDIO_DAT)pvArg;

    if (!psdio) {
        SDI_ERR("Invalid thread args.\n");
        return  (LW_NULL);
    }

    for (;;) {
        __mciIoWaitQueue(psdio->SDIO_hSdioSync);

        if (psdio->SDIO_userFunc) {
            psdio->SDIO_userFunc(psdio->SDIO_ulFuncArg);
        }

        sdioIntEnable(psdio, 1);
    }

    return  (LW_NULL);
}
/*********************************************************************************************************
** 函数名称: __mciTaskletInit
** 功能描述: 从SD主控 FIFO 读入数据
** 输    入: psdio        SDIO 结构
**           pmciTasklet  任务函数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  __mciTaskletInit (PSDIO_DAT  psdio,  PTHREAD_START_ROUTINE  pmciTasklet)
{
    LW_CLASS_THREADATTR  threadAttr;

    if (!psdio || !pmciTasklet) {
        return  (PX_ERROR);
    }

    psdio->SDIO_eCompleteWhat = COMPLETION_NONE;
    psdio->SDIO_iIoAct        = XFER_NONE;
    psdio->SDIO_uiCmdError    = ERROR_NONE;
    psdio->SDIO_uiDataError   = ERROR_NONE;

    psdio->SDIO_hTaskSync = API_MsgQueueCreate("mci_wait",
                                                512,
                                                sizeof(ULONG),
                                                LW_OPTION_WAIT_FIFO,
                                                LW_NULL);
    if (psdio->SDIO_hTaskSync == LW_OBJECT_HANDLE_INVALID) {
        return  (PX_ERROR);
    }

    psdio->SDIO_hComplete = API_MsgQueueCreate("cmd_wait",
                                                512,
                                                sizeof(ULONG),
                                                LW_OPTION_WAIT_FIFO,
                                                LW_NULL);
    if (psdio->SDIO_hComplete == LW_OBJECT_HANDLE_INVALID) {
        return  (PX_ERROR);
    }

    threadAttr = API_ThreadAttrGetDefault();
    threadAttr.THREADATTR_pvArg       = (PVOID)psdio;
    threadAttr.THREADATTR_ucPriority  = SDIO_PRIO;
    threadAttr.THREADATTR_ulOption   |= LW_OPTION_OBJECT_GLOBAL;

    psdio->SDIO_hTaskId = API_ThreadCreate("t_sditask",
                                           (PTHREAD_START_ROUTINE)pmciTasklet,
                                           &threadAttr,
                                            LW_NULL);
    if (psdio->SDIO_hTaskId == LW_OBJECT_HANDLE_INVALID) {
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciSdioThreadInit
** 功能描述: SDIO 线程初始化
** 输    入: psdio     SDIO 结构
**           pTask     任务函数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciSdioThreadInit(PSDIO_DAT  psdio, PTHREAD_START_ROUTINE  pTask)
{
    LW_CLASS_THREADATTR  threadAttr;

    if (!psdio || !pTask) {
        return  (PX_ERROR);
    }

    psdio->SDIO_hSdioSync = API_MsgQueueCreate("sdio_wait",
                                                512,
                                                sizeof(ULONG),
                                                LW_OPTION_WAIT_FIFO,
                                                LW_NULL);
    if (psdio->SDIO_hSdioSync == LW_OBJECT_HANDLE_INVALID) {
        return  (PX_ERROR);
    }

    threadAttr = API_ThreadAttrGetDefault();
    threadAttr.THREADATTR_pvArg       = (PVOID)psdio;
    threadAttr.THREADATTR_ucPriority  = SDIO_PRIO;
    threadAttr.THREADATTR_ulOption   |= LW_OPTION_OBJECT_GLOBAL;

    psdio->SDIO_hTaskSdio = API_ThreadCreate("t_sdioint",
                                             (PTHREAD_START_ROUTINE)pTask,
                                             &threadAttr,
                                              LW_NULL);
    if (psdio->SDIO_hTaskSdio == LW_OBJECT_HANDLE_INVALID) {
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciIrq
** 功能描述: MCI 中断处理函数
** 输    入: pvArg   中断参数
**           iVector 中断号
** 输    出: NONE
** 返    回: LW_IRQ_HANDLED
*********************************************************************************************************/
irqreturn_t  __mciIrq (PVOID  pvArg, ULONG  iVector)
{
    UINT32        uiDStat, uiIMsk, uiDClr, uiCClr = 0;
    UINT32        uiCStat, uiFStat;
    PSDIO_DAT     psdio = (PSDIO_DAT)pvArg;
    INTREG        iRegInterLevel;

    INTER_CLR_PNDING(BIT_SDI);

    uiDStat = rSDIDSTA;
    uiIMsk  = rSDIIMSK;

    if (uiDStat & SDI_DSTA_SDI_OIRQDETECT) {
        if (uiIMsk & SDI_IMSK_SDI_OIRQ) {
            uiDClr = SDI_DSTA_SDI_OIRQDETECT;
            rSDIDSTA = uiDClr;
            sdioIntEnable(psdio, 0);

#if SDI_USE_SDIOINT_NOTIFY <= 0
            __mciWakeupQueue(psdio->SDIO_hSdioSync);
#else
            SDI_SDIOINT_NOTIFY(psdio);
#endif

            return  (LW_IRQ_HANDLED);
        }
    }

    LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);

    uiCStat = rSDICSTA;
    uiFStat = rSDIFSTA;
    uiDClr  = 0;

    if (psdio->SDIO_eCompleteWhat == COMPLETION_NONE ||
        psdio->SDIO_eCompleteWhat == COMPLETION_FINALIZE) {
        LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
        __mciClearMsk();
        goto __irq_out;
    }

    if (!(psdio->SDIO_psdcmd->SDCMD_uiFlag)) {
        LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
        __mciClearMsk();
        goto __irq_out;
    }

    /*
     *  FIFO 数据传输
     */
    if (!psdio->SDIO_ucDmaEn) {
        if (psdio->SDIO_iIoAct == XFER_WRITE &&
            (uiFStat & SDI_FSTA_TFDET)) {
            LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
            __mciDisableIntMask(SDI_IMSK_TXFIFOHALF);
            __mciWakeupQueue(psdio->SDIO_hTaskSync);
            LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);
        } else if (psdio->SDIO_iIoAct == XFER_READ &&
                   (uiFStat & SDI_FSTA_RFDET)) {
            LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
            __mciDisableIntMask(SDI_IMSK_RXFIFOHALF | SDI_IMSK_RXFIFOLAST);
            __mciWakeupQueue(psdio->SDIO_hTaskSync);
            LW_SPIN_LOCK_QUICK(&psdio->SDIO_slCompLock, &iRegInterLevel);
        }
    }

    if (uiCStat & SDI_CSTA_CMDTIMEOUT) {
        SDI_ERR("CmdStat: command timeout.\r\n");
        psdio->SDIO_uiCmdError = ETIMEDOUT;
        goto __fail_xfer;
    }

    if (uiCStat & SDI_CSTA_CMDSENT) {
        if (psdio->SDIO_eCompleteWhat == COMPLETION_CMDSENT) {
            SDI_DBG("CmdStat: command send.\r\n");
            psdio->SDIO_uiCmdError = ERROR_NONE;
            goto __close_xfer;
        }
        uiCClr |= SDI_CSTA_CMDSENT;
    }

    if (uiCStat & SDI_CSTA_CRCFAIL) {
        SDI_DBG("CmdStat: command CRC error[ignore].\r\n");
        uiCClr |= SDI_CSTA_CRCFAIL;
        /*
         * 经测试,这里应该是2440控制器硬件的问题, 实际上没有错误
         * 因此这里不跳转到结束, 继续进行下面的处理
         */
    }

    if (uiCStat & SDI_CSTA_RSPFIN) {
        if (psdio->SDIO_eCompleteWhat == COMPLETION_RSPFIN) {
            SDI_DBG("CmdStat: command response received.\r\n");
            psdio->SDIO_uiCmdError = ERROR_NONE;
            goto __close_xfer;
        }

        if (psdio->SDIO_eCompleteWhat == COMPLETION_XFERFINISH_RSPFIN) {
            SDI_DBG("Old complete state: %s, New complete state: %s",
                    "COMPLETION_XFERFINISH_RSPFIN",
                    "COMPLETION_XFERFINISH");
            psdio->SDIO_eCompleteWhat = COMPLETION_XFERFINISH;
        }

        uiCClr = SDI_CSTA_RSPFIN;
    }

    if (!psdio->SDIO_bData) {
        SDI_DBG("Have not more data need progress exit.\r\n");
        goto __clr_stabit;
    }

    /*
     *  Check for FIFO failure
     */
    if (uiFStat & SDI_FSTA_FIFOFAIL) {
        SDI_ERR("FifoSTat: FIFO state error.\r\n");
        psdio->SDIO_uiDataError = EILSEQ;
        goto __fail_xfer;
    }

    if (uiDStat & SDI_DSTA_RXCRCFAIL) {
        SDI_ERR("DataStata: Rx CRC fail.\r\n");
        psdio->SDIO_uiDataError = EILSEQ;
        goto __fail_xfer;
    }

    if (uiDStat & SDI_DSTA_CRCFAIL) {
        SDI_ERR("DataStata: Tx CRC fail.\r\n");
        psdio->SDIO_uiDataError = EILSEQ;
        goto __fail_xfer;
    }

    if (uiDStat & SDI_DSTA_DATATIMEOUT) {
        SDI_ERR("DataStata: data timeout.\r\n");
        psdio->SDIO_uiDataError = ETIMEDOUT;
        goto __fail_xfer;
    }

    if (uiDStat & SDI_DSTA_XFERFINISH) {
        if (psdio->SDIO_eCompleteWhat == COMPLETION_XFERFINISH) {
            SDI_DBG("Data transfer completed.\r\n");
            goto __close_xfer;
        }

        if (psdio->SDIO_eCompleteWhat == COMPLETION_XFERFINISH_RSPFIN) {
            SDI_DBG("DataStat: complete stata[%s->%s]",
                    "COMPLETION_XFERFINISH_RSPFIN",
                    "COMPLETION_RSPFIN");
            psdio->SDIO_eCompleteWhat = COMPLETION_RSPFIN;
        }

        uiDClr = SDI_DSTA_XFERFINISH;
    }

__clr_stabit:
    rSDICSTA = uiCClr;
    rSDIDSTA = uiDClr;
    LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
    goto __irq_out;

__fail_xfer:
    psdio->SDIO_iIoAct = XFER_NONE;

__close_xfer:
    psdio->SDIO_eCompleteWhat = COMPLETION_FINALIZE;
    LW_SPIN_UNLOCK_QUICK(&psdio->SDIO_slCompLock, iRegInterLevel);
    __mciClearMsk();
    __mciWakeupQueue(psdio->SDIO_hTaskSync);

__irq_out:

    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** 函数名称: __mciFifoCnt
** 功能描述: 获得 FIFO 中数据数量
** 输    入: NONE
** 输    出: NONE
** 返    回: 数据数量
*********************************************************************************************************/
static INT  __mciFifoCnt (VOID)
{
    volatile UINT32 uiFStat = rSDIFSTA;

    uiFStat &= SDI_FSTA_COUNTMASK;

    return  (uiFStat);
}
/*********************************************************************************************************
** 函数名称: __mciFifoFree
** 功能描述: 获得 FIFO 剩余空间数
** 输    入: NONE
** 输    出: NONE
** 返    回: FIFO 剩余空间数量
*********************************************************************************************************/
static INT  __mciFifoFree (VOID)
{
    volatile UINT32 uiFStat = rSDIFSTA;

    uiFStat &= SDI_FSTA_COUNTMASK;

    return  (63 - uiFStat);
}
/*********************************************************************************************************
** 函数名称: __mciFifoRead
** 功能描述: 从 SD 主控 FIFO 读入数据
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT __mciFifoRead (PSDIO_DAT  psdio)
{
    UINT8  uiFifo;
    UINT8 *puiTmp;
    UINT32 uiTmpLen;

    if (!psdio) {
        SDI_ERR("Error sdio struct.\r\n");
        return  (PX_ERROR);
    }

    uiTmpLen = (psdio->SDIO_psddata->SDDAT_uiBlkNum
             * psdio->SDIO_psddata->SDDAT_uiBlkSize)
             - psdio->SDIO_uiBytes;

    puiTmp   = psdio->SDIO_puiBuf + uiTmpLen;

    while ((uiFifo = __mciFifoCnt())) {
        if (!psdio->SDIO_uiBytes) {
            psdio->SDIO_iIoAct = XFER_NONE;
            psdio->SDIO_eCompleteWhat = COMPLETION_FINALIZE;
            return  (ERROR_NONE);
        }

        if (uiFifo >= psdio->SDIO_uiBytes) {
            uiFifo = psdio->SDIO_uiBytes;
        }

        psdio->SDIO_uiBytes -= uiFifo;

        while (uiFifo--) {
            *(puiTmp) = rSDIDAT;
            puiTmp++;
        }
    }

    if (!psdio->SDIO_uiBytes) {
        SDI_DBG("(FIFO READ)Data transfer complete.\r\n");
        psdio->SDIO_iIoAct = XFER_NONE;
        psdio->SDIO_eCompleteWhat = COMPLETION_FINALIZE;

        return  (ERROR_NONE);
    }

    __mciEnableIntMask(SDI_IMSK_RXFIFOLAST | SDI_IMSK_RXFIFOHALF);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciFifoWrite
** 功能描述: 向SD主控 FIFO 写入数据
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT __mciFifoWrite (PSDIO_DAT  psdio)
{
    UINT8 uiFifo;

    if (!psdio) {
        SDI_ERR("Error sdio struct.\r\n");
        return  (PX_ERROR);
    }

    while ((uiFifo = __mciFifoFree())) {
        if (!psdio->SDIO_uiBytes) {
            SDI_DBG("(FIFO WRITE) Data transfer complete.\r\n");
            psdio->SDIO_iIoAct = XFER_NONE;
            return  (ERROR_NONE);
        }

        if (uiFifo > psdio->SDIO_uiBytes) {
            uiFifo = psdio->SDIO_uiBytes;
        }

        psdio->SDIO_uiBytes -= uiFifo;

        while (uiFifo--) {
            rSDIDAT = *(psdio->SDIO_puiBuf);
            psdio->SDIO_puiBuf++;
        }
    }

    __mciEnableIntMask(SDI_IMSK_TXFIFOHALF);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciDmaCallback
** 功能描述: DMA 完成回调函数
** 输    入: iChan    通道号
**           pvArg    回调参数
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  __mciDmaCallback (UINT iChan, PVOID  pvArg)
{
    PSDIO_DAT  psdio = (PSDIO_DAT)pvArg;

    if (psdio->SDIO_hDmaSync != LW_OBJECT_HANDLE_INVALID) {
        API_SemaphoreBPost(psdio->SDIO_hDmaSync);
    }
}
/*********************************************************************************************************
** 函数名称: __mciDmaSetup
** 功能描述: MCI 建立 DMA 传输参数
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciDmaSetup (PSDIO_DAT  psdio, LW_DMA_TRANSACTION  *dmatData)
{
    INT     iRw     = psdio->SDIO_sdiopHeader.SDIOP_iRdWr;
    UINT8  *pucAddr = (UINT8 *)(SDIDAT);

    lib_bzero(dmatData, sizeof(LW_DMA_TRANSACTION));

    dmatData->DMAT_pucSrcAddress  = iRw ? psdio->SDIO_puiBuf : pucAddr;    /*  iRw == 1 为写操作        */
    dmatData->DMAT_pucDestAddress = iRw ? pucAddr : psdio->SDIO_puiBuf;
    dmatData->DMAT_bHwReqEn       = TRUE;
    dmatData->DMAT_stDataBytes    = psdio->SDIO_uiBytes;
    dmatData->DMAT_bHwHandshakeEn = TRUE;
    dmatData->DMAT_iHwReqNum      = 2;
    dmatData->DMAT_iTransMode     = DMA_TRANSMODE_SINGLE  |                /*  传输模式为单次传输       */
                                    DMA_TRANSMODE_CLKAPB  |                /*  使用 PCLK 时钟           */
                                    DMA_TRANSMODE_ONESHOT |                /*  完成后不自动重载         */
                                    DMA_TRANSMODE_BYTE;
    dmatData->DMAT_iSrcAddrCtl    = iRw ? LW_DMA_ADDR_INC : LW_DMA_ADDR_FIX;
    dmatData->DMAT_iDestAddrCtl   = iRw ? LW_DMA_ADDR_FIX : LW_DMA_ADDR_INC;
    dmatData->DMAT_ulOption       = iRw ? (DMA_OPTION_SRCBUS_AHB |         /*  源地址从 AHB             */
                                           DMA_OPTION_DSTBUS_APB)          /*  目的地址到 APB           */
                                        : (DMA_OPTION_DSTBUS_AHB |
                                           DMA_OPTION_SRCBUS_APB);

    dmatData->DMAT_pfuncCallback  = __mciDmaCallback;
    dmatData->DMAT_pvArg          = (PVOID)psdio;

    rSDICSTA  = 0xa00;

    psdio->SDIO_hDmaSync          = API_SemaphoreBCreate("Sem_sdio_dma",
                                                         LW_FALSE,
                                                         LW_OPTION_OBJECT_GLOBAL,
                                                         LW_NULL);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciDmaTransfer
** 功能描述: MCI DMA 传输开始
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciDmaTransfer (PSDIO_DAT  psdio, LW_DMA_TRANSACTION  *pdmatData)
{
    INT  iRet;

    if (!psdio || !pdmatData) {
        return  (PX_ERROR);
    }

    iRet = dmaJobAdd(0, pdmatData);                                      /*  通道0 做 DMA 传输          */
    if (iRet != ERROR_NONE) {
        SDI_ERR("DMA trans data error.\n");
        return  (PX_ERROR);
    }

    API_SemaphoreBPend(psdio->SDIO_hDmaSync, LW_OPTION_WAIT_INFINITE);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciRequestDma
** 功能描述: MCI 请求 DMA 作传输
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  __mciRequestDma (PSDIO_DAT  psdio)
{
    LW_DMA_TRANSACTION   dmatData;
    INT                  iRet;

    __mciDmaSetup(psdio, &dmatData);

    iRet = __mciDmaTransfer(psdio, &dmatData);
    if (iRet != ERROR_NONE) {
        SDI_ERR("Dma transfer error.\r\n");
    }
}
/*********************************************************************************************************
** 函数名称: __sendDataPrepare
** 功能描述: 发送数据前的配置
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __sendDataPrepare (PSDIO_DAT psdio)
{
    UINT32  uiMsk;
    volatile UINT32  uiDatCon = 0;
    UINT32  uiRetry  = 3;

    if (!psdio->SDIO_bData) {
        rSDIDCON = 0;
        return  (ERROR_NONE);
    }

    if ((psdio->SDIO_psddata->SDDAT_uiBlkSize & 3) != 0) {
        /*
         *  多块传输时，数据要字对齐
         */
        if (psdio->SDIO_psddata->SDDAT_uiBlkNum > 1) {
            SDI_DBG("Can't do non-word sized block transfers(blksiz %d)\n",
                    psdio->SDIO_psddata->SDDAT_uiBlkSize);
            return  (EINVAL);
        }
    }

    while (rSDIDSTA &
          (SDI_DSTA_RXDATAON |
           SDI_DSTA_TXDATAON)) {
        SDI_DBG("Data transfer still in progress.\r\n");

        __mciDataStop(psdio);
        sdioReset(psdio);

        if ((uiRetry--) == 0) {
            SDI_DBG("Data prepare timeout.\r\n");
            return  (EINVAL);
        }
    }

    psdio->SDIO_uiBytes = psdio->SDIO_psddata->SDDAT_uiBlkNum * psdio->SDIO_psddata->SDDAT_uiBlkSize;

    uiDatCon |= psdio->SDIO_psddata->SDDAT_uiBlkNum & SDI_DCON_BLKNUM_MASK;

    if (psdio->SDIO_ucDmaEn) {
        uiDatCon |= SDI_DCON_DMAEN;                                     /*  使能 DMA 模式               */
    }

    if (psdio->SDIO_iBusWidth == 4) {
        uiDatCon |= SDI_DCON_WIDEBUS;                                   /*  SDIO 宽总线模式             */
    }

    if (psdio->SDIO_sdiopHeader.SDIOP_iBlkMode) {
        uiDatCon |= SDI_DCON_BLOCKMODE;                                 /*  非流模式(块模式)            */
    }

    if ((psdio->SDIO_sdiopHeader.SDIOP_iRdWr) == 0) {
        uiDatCon |= SDI_DCON_RXAFTERCMD;
        uiDatCon |= SDI_DCON_XFER_RXSTART;
    } else {
        uiDatCon |= SDI_DCON_TXAFTERRESP;
        uiDatCon |= SDI_DCON_XFER_TXSTART;
    }

    uiDatCon |= SDI_DCON_DS_WORD;
    uiDatCon |= SDI_DCON_DATSTART;                                      /*  传输开始                    */

    rSDIDCON  = uiDatCon;                                               /*  写入数据控制寄存器          */

    rSDIBSIZE = psdio->SDIO_psddata->SDDAT_uiBlkSize;                     /*  块大小                      */

    uiMsk = SDI_IMSK_FIFOFAIL    |
            SDI_IMSK_DATACRC     |
            SDI_IMSK_DATATIMEOUT |
            SDI_IMSK_DATAFINISH  ;

    __mciEnableIntMask(uiMsk);

    rSDIDTIMER = SDI_SET_TIMEOUT;                                       /*  超时计数                    */

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __mciPrepareFifo
** 功能描述: MCI 准备 FIFO
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
VOID  __mciPrepareFifo (PSDIO_DAT  psdio)
{
    psdio->SDIO_iIoAct = psdio->SDIO_sdiopHeader.SDIOP_iRdWr ? XFER_WRITE : XFER_READ;

    if (psdio->SDIO_sdiopHeader.SDIOP_iRdWr) {
        __mciFifoWrite(psdio);
        __mciEnableIntMask(SDI_IMSK_TXFIFOHALF);
    } else {
        __mciEnableIntMask(SDI_IMSK_RXFIFOHALF | SDI_IMSK_RXFIFOLAST);
    }
}
/*********************************************************************************************************
** 函数名称: __mciSendRequest
** 功能描述: MCI 发送请求
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciSendRequest (PSDIO_DAT  psdio)
{
    INT  iRet;

    if (__mciCardPresent(psdio) == 0) {
        SDI_DBG("No medium present.\r\n");
        psdio->SDIO_uiCmdError = ENOMEDIUM;
        __mciRequestDone(psdio);
    } else {
        rSDICSTA = SDI_MSK;
        rSDIDSTA = SDI_MSK;
        rSDIFSTA = SDI_MSK;

        if (psdio->SDIO_bData) {
            iRet = __sendDataPrepare(psdio);
            if (iRet != ERROR_NONE) {
                SDI_ERR("prepare data error.\r\n");
                psdio->SDIO_uiCmdError = iRet;
                psdio->SDIO_uiDataError = iRet;
                __mciRequestDone(psdio);
                return;
            }

            if (psdio->SDIO_ucDmaEn) {
                /*
                 *  TODO DMA init
                 */
                SDI_DBG("Dma prepare...\r\n");
                __mciRequestDma(psdio);
            } else {
                SDI_DBG("FIFO prepare...\r\n");
                __mciPrepareFifo(psdio);
            }
        }
        __mciSendCmd(psdio);
        __mciEnableIrq(psdio, 1);
    }
}
/*********************************************************************************************************
** 函数名称: __mciWaitCmd
** 功能描述: MCI 等命令完成
** 输    入: psdio    SDIO 结构
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID  __mciWaitCmd (PSDIO_DAT  psdio)
{
    __mciSendRequest(psdio);
    /*
     *  等完成
     */
    __mciIoWaitQueue(psdio->SDIO_hComplete);
}
/*********************************************************************************************************
** 函数名称: __sdioInt
** 功能描述: 请求中断
** 输    入: NONE
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __mciIntConnect (PSDIO_DAT  psdio)
{
    if (API_InterVectorConnect(MCI_IRQ, __mciIrq, psdio, "sdi_isr") != ERROR_NONE) {
        SDI_ERR("sdio intr request failed.\n");
        return  (PX_ERROR);
    }

    psdio->SDIO_ulVector = MCI_IRQ;

    API_InterVectorDisable(MCI_IRQ);

    psdio->SDIO_bIrqState  = 0;

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: mciDriverInstall
** 功能描述: MCI 驱动函数安装
** 输    入: psdio
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  mciDriverInstall (PSDIO_DAT  psdio)
{
    if (psdio) {
        psdio->SDIO_request.MCIDRV_request = __mciWaitCmd;

        return  (ERROR_NONE);
    }

    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: sdioAlloc
** 功能描述: 分配 SDIO 空间
** 输    入: NONE
** 输    出: NONE
** 返    回: SDIO 结构指针
*********************************************************************************************************/
PSDIO_DAT  sdioAlloc (VOID)
{
    PSDIO_DAT  psdio;

    psdio = sys_malloc(sizeof(SDIO_DAT));
    if (!psdio) {
        SDI_ERR("Alloc memory error.\n");
        return  (LW_NULL);
    }

    lib_bzero(psdio, sizeof(SDIO_DAT));

    return  (psdio);
}
/*********************************************************************************************************
** 函数名称: sdioFree
** 功能描述: 释放 SDIO
** 输    入: psdio   SDIO 结构
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
VOID  sdioFree (PSDIO_DAT  psdio)
{
    if (psdio) {
        sys_free(psdio);
    }
}
/*********************************************************************************************************
** 函数名称: __mciThreadDestory
** 功能描述: MCI 线程回收资源
** 输    入: psdio   SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  __mciThreadDestory (PSDIO_DAT  psdio)
{
    if (psdio) {
        if (psdio->SDIO_hComplete != LW_HANDLE_INVALID) {
            API_MsgQueueDelete(&psdio->SDIO_hComplete);
        } else {
            goto __err;
        }

        if (psdio->SDIO_hDmaSync != LW_HANDLE_INVALID) {
            API_SemaphoreBDelete(&psdio->SDIO_hDmaSync);
        } else {
            goto __err;
        }

        if (psdio->SDIO_hTaskSync != LW_HANDLE_INVALID) {
            API_MsgQueueDelete(&psdio->SDIO_hTaskSync);
        } else {
            goto __err;
        }

        if (psdio->SDIO_hTaskSync != LW_HANDLE_INVALID) {
            kill(psdio->SDIO_hTaskSync, SIGTERM);
        } else {
            goto __err;
        }

        if (psdio->SDIO_hTaskId != LW_HANDLE_INVALID) {
            kill(psdio->SDIO_hTaskId, SIGTERM);
        } else {
            goto __err;
        }

        return  (ERROR_NONE);
    }

__err:
    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: sdioDeInit
** 功能描述: sdio 回收资源
** 输    入: psdio   SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  sdioDeInit (PSDIO_DAT  psdio)
{
    if (psdio) {
        /*
         *  做资源回收
         */

        if (__mciThreadDestory(psdio) != ERROR_NONE) {
            goto __err;
        }

        API_InterVectorDisconnect(psdio->SDIO_ulVector, __mciIrq, psdio);

        return  (ERROR_NONE);
    }

__err:
    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: sdioInit
** 功能描述: sdio 初始化
** 输    入: psdio   SDIO 结构
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
INT  sdioInit (PSDIO_DAT  psdio)
{
    INT       iRet;

    __mciHwInit();

    mciDriverInstall(psdio);

    sdioStartClock();

    LW_SPIN_INIT(&psdio->SDIO_slCompLock);

    iRet = __mciTaskletInit(psdio, __mciIoTasklet);
    if (iRet != ERROR_NONE) {
        SDI_ERR("Tasklet init failed.\n");
        return  (PX_ERROR);
    }

    iRet = __mciSdioThreadInit(psdio, __mciSdioThread);
    if (iRet != ERROR_NONE) {
        SDI_ERR("sdio thread create failed.\r\n");
        return  (PX_ERROR);
    }

    iRet = __mciIntConnect(psdio);
    if (iRet != ERROR_NONE) {
        SDI_ERR("Intr request failed.\n");
        return  (PX_ERROR);
    }

#ifdef SDIO_DMA_EN
    psdio->SDIO_ucDmaEn = 1;
#else
    psdio->SDIO_ucDmaEn = 0;
#endif

    return  (ERROR_NONE);
}

/*********************************************************************************************************
  END
*********************************************************************************************************/

