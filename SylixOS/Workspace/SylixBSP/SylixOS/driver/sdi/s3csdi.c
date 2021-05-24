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
** 文   件   名: s3csdi.c
**
** 创   建   人: Zeng.Bo(曾波)
**
** 文件创建日期: 2010 年 11 月 24 日
**
** 描        述: s3c2440a sd卡主控制器硬件驱动源文件

** BUG:
2010.12.01  发现 __SDI_REG_RSP0 地址定义错误,修改之.
2010.12.08  优化内部函数.
2010.03.31  修改__s3csdPrepareData(),对不同设备设置不同的总线位宽(主要是兼容MMC).
2014.10.31  修改为符合 SDM 接口的驱动
*********************************************************************************************************/
#define __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
#include "mciLib.h"
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define __S3CSDHOST_NAME            "/bus/sd/0"

#ifdef  SDIO_OPS_DEBUG
#define  SDIO_DBG(fmt, args...)     _DebugFormat(__LOGMESSAGE_LEVEL, fmt, ##args)
#else
#define  SDIO_DBG(fmt, args...)
#endif
#define  SDIO_ERR(fmt, args...)     _DebugFormat(__ERRORMESSAGE_LEVEL, fmt, ##args)
/*********************************************************************************************************
  中断引脚配置
*********************************************************************************************************/
#define __NCD_SD                    (rGPGDAT & (1u << 8))
#define __SDCARD_INSERT             (__NCD_SD == 0)

#define __PIN_SET_IO()              rGPGCON = rGPGCON & ~(3 << 16)
#define __PIN_SET_INT()             rGPGCON = (rGPGCON & ~(3 << 16)) | (2 << 16)
/*********************************************************************************************************
  控制器内部结构
*********************************************************************************************************/
typedef struct {
    SD_HOST       sdhost;
    SD_CALLBACK   callbackChkDev;
    PVOID         pvCallBackArg;
    PVOID         pvSdmHost;
} __S3CSD_HOST;
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
static INT __s3csdIoCtl(PLW_SD_ADAPTER          psdadapter,
                        INT                     iCmd,
                        LONG                    lArg);
static INT __s3csdTransfer(PLW_SD_ADAPTER       psdadapter,
                           PLW_SD_DEVICE        psddevice,
                           PLW_SD_MESSAGE       psdmsg,
                           INT                  iNum);
static INT  __s3csdCallBackInstall(SD_HOST     *pHost,
                                   INT          iCallbackType,
                                   SD_CALLBACK  callback,
                                   PVOID        pvCallbackArg);
static INT  __s3csdCallBackUnInstall(SD_HOST   *pHost,
                                    INT         iCallbackType);

static VOID __s3csdSdioIntEn(SD_HOST *pHost, BOOL bEnable);
static VOID __s3csdHotPlugInit(VOID);
static VOID __s3csdMciIrqHandle(ULONG ulArg);
static irqreturn_t __s3csdCdIntr(PVOID  pvArg, ULONG ulVec);
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
static LW_SD_FUNCS        _G_sdfuncS3cSd;
static UINT32             _G_s3csdOCR = SD_VDD_32_33 |                  /*  OCR中包含主控制器的电压支持 */
                                        SD_VDD_33_34 |                  /*  情况,还有设备容量支持情况   */
                                        SD_VDD_34_35 |
                                        SD_VDD_35_36;

static __S3CSD_HOST       _G_s3csdHost = {

        {
            .SDHOST_cpcName                = __S3CSDHOST_NAME,
            .SDHOST_iType                  = SDHOST_TYPE_SD,
            .SDHOST_iCapbility             = SDHOST_CAP_DATA_4BIT,
            .SDHOST_pfuncCallbackInstall   = __s3csdCallBackInstall,
            .SDHOST_pfuncCallbackUnInstall = __s3csdCallBackUnInstall,
            .SDHOST_pfuncSdioIntEn         = __s3csdSdioIntEn,
        },
};
/*********************************************************************************************************
  mciLib通信对象
*********************************************************************************************************/
static PSDIO_DAT            _G_pmciXfer = NULL;                         /*  mci 库使用的传输对象        */
/*********************************************************************************************************
** 函数名称: __s3csdIoCtl
** 功能描述: SD主控IO控制
** 输    入: psdadapter     适配器
**           iCmd           控制命令
**           lArg           参数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT __s3csdIoCtl (PLW_SD_ADAPTER  psdadapter,
                         INT             iCmd,
                         LONG            lArg)
{
    INT iError = ERROR_NONE;

    switch (iCmd) {
    case SDBUS_CTRL_POWEROFF:
        break;

    case SDBUS_CTRL_POWERUP:
    case SDBUS_CTRL_POWERON:
        sdioReset(_G_pmciXfer);
        bspDelayUs(50000);
        break;

    case SDBUS_CTRL_SETBUSWIDTH:
        iError = sdioSetBusWidth((INT)lArg);
        _G_pmciXfer->SDIO_iBusWidth = 1 << lArg;
        break;

    case SDBUS_CTRL_SETCLK:
        sdioSetClock((UINT32)lArg);
        break;

    case SDBUS_CTRL_DELAYCLK:
        break;

    case SDBUS_CTRL_GETOCR:
        *(UINT32 *)lArg = _G_s3csdOCR;
        iError = ERROR_NONE;
        break;

    default:
        _DebugHandle(__ERRORMESSAGE_LEVEL,"__s3csdIoCtl() error : can't support this cmd.\r\n");
        iError = PX_ERROR;
        break;
    }

    return  (iError);
}
/*********************************************************************************************************
** 函数名称: __s3csdTransfer
** 功能描述: SD主控传输函数
** 输    入: psdadapter   适配器
**           psddevice    设备
**           psdmsg       请求消息
**           iNum         消息个数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT __s3csdTransfer (PLW_SD_ADAPTER      psdadapter,
                            PLW_SD_DEVICE       psddevice,
                            PLW_SD_MESSAGE      psdmsg,
                            INT                 iNum)
{
    PSDIO_DAT       pMciXfer = _G_pmciXfer;
    INT             i        = 0;

    while (i < iNum && psdmsg != NULL) {

        pMciXfer->SDIO_psdcmd  = psdmsg->SDMSG_psdcmdCmd;
        pMciXfer->SDIO_psddata = psdmsg->SDMSG_psddata;

        if (pMciXfer->SDIO_psddata) {
            BOOL bWrite;
            bWrite = SD_DAT_IS_WRITE(psdmsg->SDMSG_psddata) ? TRUE : FALSE;

            pMciXfer->SDIO_sdiopHeader.SDIOP_iBlkMode = SD_DAT_IS_STREAM(psdmsg->SDMSG_psddata) ?
                                                        FALSE : TRUE;
            pMciXfer->SDIO_sdiopHeader.SDIOP_iRdWr    = bWrite;

            pMciXfer->SDIO_puiBuf = bWrite ?  psdmsg->SDMSG_pucWrtBuffer : psdmsg->SDMSG_pucRdBuffer;
            pMciXfer->SDIO_bData  = TRUE;

        } else {
            pMciXfer->SDIO_puiBuf = NULL;
            pMciXfer->SDIO_bData  = FALSE;
        }

        if (pMciXfer->SDIO_request.MCIDRV_request) {
            pMciXfer->SDIO_request.MCIDRV_request(pMciXfer);
            if (pMciXfer->SDIO_uiCmdError != ERROR_NONE) {
                SDIO_DBG(" cmd(%d) error.\r\n", psdmsg->SDMSG_psdcmdCmd->SDCMD_uiOpcode);
                return  (PX_ERROR);
            }

            if (pMciXfer->SDIO_psddata && pMciXfer->SDIO_uiDataError != ERROR_NONE) {
                SDIO_DBG("data error.\r\n");
                return  (PX_ERROR);
            }

        } else {
            return  (PX_ERROR);
        }


        /*
         * 如果有停止命令,则随后发送
         */
        if (psdmsg->SDMSG_psdcmdStop) {
            pMciXfer->SDIO_psdcmd  = psdmsg->SDMSG_psdcmdStop;
            pMciXfer->SDIO_psddata = NULL;
            pMciXfer->SDIO_bData   = FALSE;
            pMciXfer->SDIO_request.MCIDRV_request(pMciXfer);
        }

        i++;
        psdmsg++;
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __s3csdCallBackInstall
** 功能描述: 安装回调函数
** 输    入: pHost            控制器对象
**           iCallbackType    回调函数类型
**           callback         回调函数指针
**           pvCallbackArg    回调函数参数
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __s3csdCallBackInstall (SD_HOST          *pHost,
                                    INT               iCallbackType,
                                    SD_CALLBACK       callback,
                                    PVOID             pvCallbackArg)
{
    __S3CSD_HOST *pS3cHost = (__S3CSD_HOST *)pHost;
    if (!pS3cHost) {
        return  (PX_ERROR);
    }

    if (iCallbackType == SDHOST_CALLBACK_CHECK_DEV) {
        pS3cHost->callbackChkDev = callback;
        pS3cHost->pvCallBackArg  = pvCallbackArg;
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __s3csdCallBackUnInstall
** 功能描述: 卸载安装的回调函数
** 输    入: NONE
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static INT  __s3csdCallBackUnInstall (SD_HOST          *pHost,
                                     INT               iCallbackType)
{
    __S3CSD_HOST *pS3cHost = (__S3CSD_HOST *)pHost;
    if (!pS3cHost) {
        return  (PX_ERROR);
    }

    if (iCallbackType == SDHOST_CALLBACK_CHECK_DEV) {
        pS3cHost->callbackChkDev = NULL;
        pS3cHost->pvCallBackArg  = NULL;
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __s3csdSdioIntEn
** 功能描述: 使能 SDIO 中断
** 输    入: pHost      SD 控制器对象
**           bEnable    是否使能
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID __s3csdSdioIntEn (SD_HOST *pHost, BOOL bEnable)
{
    sdioIntEnable(_G_pmciXfer, bEnable);
}
/*********************************************************************************************************
** 函数名称: __s3csdHotPlugInit
** 功能描述:  热插拔支持初始化
** 输    入: NONE
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID __s3csdHotPlugInit (VOID)
{
    __PIN_SET_INT();

    rEXTINT2  = (rEXTINT2 | 7);                                         /*  外部中断16为双边沿触发中断  */
    rEINTPEND =  rEINTPEND | (1u << 16);                                /*  清除外部中断16              */
    rEINTMASK =  rEINTMASK & ~(1u << 16);                               /*  使能外部中断16              */

    ClearPending(VIC_CHANNEL_EINT8_23);                                 /*  清除外部中断2标志位         */

    API_InterVectorConnect(VIC_CHANNEL_EINT8_23,
                           __s3csdCdIntr,
                           NULL,
                           "sdcd_isr");                                 /*  设置中断子函数              */

    API_InterVectorEnable(VIC_CHANNEL_EINT8_23);                        /*  使能外部中断2               */
}
/*********************************************************************************************************
** 函数名称: __s3csdCdIntr
** 功能描述: 热插拔引脚中断服务
** 输    入: pvArg     参数
**           ulVec     中断向量号
** 输    出: NONE
** 返    回: ERROR CODE
*********************************************************************************************************/
static irqreturn_t __s3csdCdIntr (PVOID  pvArg, ULONG ulVec)
{
    API_InterVectorDisable(VIC_CHANNEL_EINT8_23);

    rEINTPEND =  rEINTPEND | (1u << 16);                                /*  清除外部中断16              */

    ClearPending(BIT_EINT8_23);                                         /*  清除外部中断16标志位        */

    __PIN_SET_IO();                                                     /*  置为IO口                    */

    if (__SDCARD_INSERT) {
        API_SdmEventNotify(_G_s3csdHost.pvSdmHost, SDM_EVENT_DEV_INSERT);

    } else {
        if (_G_s3csdHost.callbackChkDev) {
            _G_s3csdHost.callbackChkDev(_G_s3csdHost.pvCallBackArg, SDHOST_DEVSTA_UNEXIST);
        }

        API_SdmEventNotify(_G_s3csdHost.pvSdmHost, SDM_EVENT_DEV_REMOVE);
    }

    __PIN_SET_INT();                                                    /*  置为中断口                  */

    API_InterVectorEnable(VIC_CHANNEL_EINT8_23);                        /*  使能外部中断2               */

    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** 函数名称: __s3csdMciIrqHandle
** 功能描述: 供mci库使用
** 输    入: ulArg  参数
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
static VOID __s3csdMciIrqHandle (ULONG ulArg)
{
    PVOID pvSdmHost = (PVOID)ulArg;

    API_SdmEventNotify(pvSdmHost, SDM_EVENT_SDIO_INTERRUPT);
}
/*********************************************************************************************************
** 函数名称: s3csdiDrvInstall
** 功能描述: 初始化 SD 总线, 获得适配器操作函数集
** 输    入: NONE
** 输    出: NONE
** 返    回: NONE
*********************************************************************************************************/
INT s3csdiDrvInstall (VOID)
{
    static UINT  uiCdDat;
           INT   iRet;
           PVOID pvSdmHost;

    (VOID)uiCdDat;
    /*
     * 初始化MCI相关数据结构
     */
    _G_pmciXfer = sdioAlloc();
    if (!_G_pmciXfer) {
        return  (PX_ERROR);
    }

    _G_pmciXfer->SDIO_userFunc = __s3csdMciIrqHandle;

    iRet = sdioInit(_G_pmciXfer);
    if (iRet != ERROR_NONE) {
        goto __err1;
    }

    /*
     * 创建SD总线适配器
     */
    _G_sdfuncS3cSd.SDFUNC_pfuncMasterXfer = __s3csdTransfer;
    _G_sdfuncS3cSd.SDFUNC_pfuncMasterCtl  = __s3csdIoCtl;
    iRet = API_SdAdapterCreate(__S3CSDHOST_NAME, &_G_sdfuncS3cSd);
    if (iRet != ERROR_NONE) {
        goto __err2;
    }

    /*
     * 向SDM注册HOST信息
     */
    pvSdmHost = API_SdmHostRegister(&_G_s3csdHost.sdhost);
    if (!pvSdmHost) {
        goto __err3;
    }
    _G_s3csdHost.pvSdmHost      = pvSdmHost;
    _G_pmciXfer->SDIO_ulFuncArg = (ULONG)pvSdmHost;

    /*
     * 处理卡已经插入的情况
     */
    __PIN_SET_IO();
    uiCdDat = rGPGDAT;
    if (__SDCARD_INSERT) {
        API_SdmEventNotify(pvSdmHost, SDM_EVENT_DEV_INSERT);
    }

    __s3csdHotPlugInit();

    return  (ERROR_NONE);

__err3:
    API_SdAdapterDelete(__S3CSDHOST_NAME);

__err2:
    sdioDeInit(_G_pmciXfer);

__err1:
    sdioFree(_G_pmciXfer);

    return  (PX_ERROR);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
