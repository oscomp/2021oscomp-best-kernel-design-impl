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
** 文   件   名: samsungtty.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 20 日
**
** 描        述: tty 驱动.
**
** BUG:
2009.05.31  串口开中断的时机有错误.
2009.07.21  修正 485 方向角控制错误.
2012.12.14  修正 FIFO 满判断错误.
2014.05.27  startup 仅仅打开 Tx FIFO 空中断即可.
2014.07.20  加入设备电源管理.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
#include "uart.h"
#include "samsungtty.h"
/*********************************************************************************************************
  对指定管脚进行操作
*********************************************************************************************************/
#define __COM1_RS485_SEND_START()       BIT_SET(rGPGDAT, 0)
#define __COM1_RS485_SEND_END()         BIT_CLR(rGPGDAT, 0)
/*********************************************************************************************************
  判断 FIFO 有效个数
*********************************************************************************************************/
#define __COM_FIFO_CNT                  64
#define __COM_FIFO_CNT_MASK             0x7F
/*********************************************************************************************************
  STREAM UART CHANNEL (SIO CHANNEL)
*********************************************************************************************************/
typedef struct {
    SIO_DRV_FUNCS             *pdrvFuncs;                               /*  SIO 驱动程序组              */
    LW_PM_DEV                  pmdev;                                   /*  电源管理节点                */
    
    INT                      (*pcbGetTxChar)();                         /*  中断回调函数                */
    INT                      (*pcbPutRcvChar)();
    
    PVOID                      pvGetTxArg;                              /*  回调函数句柄                */
    PVOID                      pvPutRcvArg;
    
    INT                        iChannelMode;                            /*  同步 IO 通道模式            */
    
    UCHAR                    (*pfuncHwInByte)(INT);                     /*  物理硬件接收一个字节        */
    VOID                     (*pfuncHwOutByte)(INT, CHAR);              /*  物理硬件发送一个字节        */
    
    INT                        iChannelNum;                             /*  通道号                      */
    INT                        iBaud;                                   /*  波特率                      */
    
    INT                        iHwOption;                               /*  硬件选项                    */
    INT                        iRs485Flag;                              /*  对串口是否要使能485功能     */

} __SAMSUNGSIO_CHANNEL;
typedef __SAMSUNGSIO_CHANNEL  *__PSAMSUNGSIO_CHANNEL;                   /*  指针类型                    */
/*********************************************************************************************************
  SIO 通道控制块
*********************************************************************************************************/
static __SAMSUNGSIO_CHANNEL     __GsamsungsiochanUart0;
static __SAMSUNGSIO_CHANNEL     __GsamsungsiochanUart1;
static __SAMSUNGSIO_CHANNEL     __GsamsungsiochanUart2;
/*********************************************************************************************************
  UART 驱动程序
*********************************************************************************************************/
static INT   __uartIoctl(SIO_CHAN  *psiochanChan, INT  iCmd, LONG lArg);/*  端口控制                    */
static INT   __uartStartup(SIO_CHAN    *psiochanChan);                  /*  发送                        */
static INT   __uartcbInstall(SIO_CHAN          *psiochanChan,           /*  安装回调                    */
                             INT                iCallbackType,
                             VX_SIO_CALLBACK    callbackRoute,
                             PVOID              pvCallbackArg);
static INT   __uartPollRxChar(SIO_CHAN    *psiochanChan,
                              PCHAR        pcInChar);                   /*  轮询接收                    */
static INT   __uartPollTxChar(SIO_CHAN *, CHAR);                        /*  轮询发送                    */
static irqreturn_t  __uartIsr(SIO_CHAN  *psiochanChan);                 /*  接收中断                    */
static VOID  __uartHwOptionAnalyse(INT     iHwOption, 
                                   PUCHAR  pucDataBits, 
                                   PUCHAR  pucStopBits, 
                                   PUCHAR  pucParity);                  /*  分析硬件参数                */
/*********************************************************************************************************
  SIO 驱动程序
*********************************************************************************************************/
static SIO_DRV_FUNCS    __GsiodrvUartDrvFunc = {
             (INT (*)(SIO_CHAN *,INT, PVOID))__uartIoctl,
             __uartStartup,
             __uartcbInstall,
             __uartPollRxChar,
             (INT (*)(SIO_CHAN *, CHAR))__uartPollTxChar
};
/*********************************************************************************************************
** Function name:           __uartHwOptionAnalyse
** Descriptions:            分析 SIO 通道硬件参数
** input parameters:        iHwOption                   硬件参数
** output parameters:       pucDataBits,                数据位数
                            pucStopBits,                结束位
                            pucParity                   校验位
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static VOID   __uartHwOptionAnalyse (INT     iHwOption, 
                                     PUCHAR  pucDataBits, 
                                     PUCHAR  pucStopBits, 
                                     PUCHAR  pucParity)
{
    if ((iHwOption & CS8) == CS8) {                                     /*  确定数据位数                */
        *pucDataBits = 8;
    } else if (iHwOption & CS7) {
        *pucDataBits = 7;
    } else if (iHwOption & CS6) {
        *pucDataBits = 6;
    } else {
        *pucDataBits = 5;
    }
    
    if (iHwOption & STOPB) {                                            /*  确定结束位                  */
        *pucStopBits = TWO_STOPBIT;
    } else {
        *pucStopBits = ONE_STOPBIT;
    }
    
    if (iHwOption & PARENB) {                                           /*  确定校验位                  */
        if (iHwOption & PARODD) {
            *pucParity = CHK_ODD;
        } else {
            *pucParity = CHK_EVEN;
        }
    } else {
        *pucParity = CHK_NONE;
    }
}
/*********************************************************************************************************
** Function name:           sioChanCreate
** Descriptions:            创建一个 sio 通道
** input parameters:        iChannelNum     硬件通道号
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/12/20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
SIO_CHAN    *sioChanCreate (INT   iChannelNum)
{
    static PLW_PM_ADAPTER    pmadapter = LW_NULL;
    __PSAMSUNGSIO_CHANNEL    psamsungsiochanUart;
    SIO_CHAN                *psiochan;
    
    UCHAR                    ucDataBits;
    UCHAR                    ucStopBits;
    UCHAR                    ucParity;
    
    if (pmadapter == LW_NULL) {
        pmadapter =  pmAdapterFind("inner_pm");
        if (pmadapter == LW_NULL) {
            printk(KERN_ERR "can not find power manager.\n");
        }
    }

    switch (iChannelNum) {
    
    case COM0:
        pmDevInit(&__GsamsungsiochanUart0.pmdev, pmadapter, 10, LW_NULL);
        __GsamsungsiochanUart0.pmdev.PMD_pcName = "uart0";
        psamsungsiochanUart              = &__GsamsungsiochanUart0;
        psamsungsiochanUart->pdrvFuncs   = &__GsiodrvUartDrvFunc;       /*  SIO FUNC                    */
        psamsungsiochanUart->iChannelNum = COM0;
        
        API_InterVectorConnect(VIC_CHANNEL_UART0, 
                               (PINT_SVR_ROUTINE)__uartIsr, 
                               (PVOID)&__GsamsungsiochanUart0,
                               "uart0_isr");                            /*  安装操作系统中断向量表      */
        break;
    
    case COM1:
        pmDevInit(&__GsamsungsiochanUart1.pmdev, pmadapter, 11, LW_NULL);
        __GsamsungsiochanUart1.pmdev.PMD_pcName = "uart1";
        psamsungsiochanUart              = &__GsamsungsiochanUart1;
        psamsungsiochanUart->pdrvFuncs   = &__GsiodrvUartDrvFunc;       /*  SIO FUNC                    */
        psamsungsiochanUart->iChannelNum = COM1;
        
        API_InterVectorConnect(VIC_CHANNEL_UART1, 
                               (PINT_SVR_ROUTINE)__uartIsr, 
                               (PVOID)&__GsamsungsiochanUart1,
                               "uart1_isr");                            /*  安装操作系统中断向量表      */
        break;
    
    case COM2:
        pmDevInit(&__GsamsungsiochanUart2.pmdev, pmadapter, 12, LW_NULL);
        __GsamsungsiochanUart2.pmdev.PMD_pcName = "uart2";
        psamsungsiochanUart              = &__GsamsungsiochanUart2;
        psamsungsiochanUart->pdrvFuncs   = &__GsiodrvUartDrvFunc;       /*  SIO FUNC                    */
        psamsungsiochanUart->iChannelNum = COM2;
        
        API_InterVectorConnect(VIC_CHANNEL_UART2, 
                               (PINT_SVR_ROUTINE)__uartIsr, 
                               (PVOID)&__GsamsungsiochanUart2,
                               "uart2_isr");                            /*  安装操作系统中断向量表      */
        break;
    
    default:
        return  (LW_NULL);                                              /*  通道号错误                  */
    }
    
    psiochan = (SIO_CHAN *)psamsungsiochanUart;
    
    psamsungsiochanUart->iChannelMode = SIO_MODE_INT;                   /*  使用中断模式                */
    psamsungsiochanUart->iBaud        = UART_DEFAULT_BAUD;              /*  初始化波特率                */
    psamsungsiochanUart->iHwOption    = UART_DEFAULT_OPT;               /*  硬件状态                    */
    psamsungsiochanUart->iRs485Flag   = RS485_DIS;                      /*  默认不使用485功能           */
    
    __uartHwOptionAnalyse(UART_DEFAULT_OPT,
                          &ucDataBits,
                          &ucStopBits,
                          &ucParity);                                   /*  获得具体参数                */
                          
    uartInit(iChannelNum, UNUSE_INF, ucDataBits, ucStopBits, 
             ucParity, UART_DEFAULT_BAUD, LW_NULL);
             
    return  (psiochan);
}
/*********************************************************************************************************
** Function name:           __uartcbInstall
** Descriptions:            SIO 通道安装回调函数
** input parameters:        psiochanChan                 通道
**                          iCallbackType                回调类型
**                          callbackRoute                回调函数
**                          pvCallbackArg                回调参数
** output parameters:       NONE
** Returned value:          错误号
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT   __uartcbInstall(SIO_CHAN          *psiochanChan,
                             INT                iCallbackType,
                             VX_SIO_CALLBACK    callbackRoute,
                             PVOID              pvCallbackArg)
{
    __PSAMSUNGSIO_CHANNEL     psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    
    switch (iCallbackType) {
    
    case SIO_CALLBACK_GET_TX_CHAR:                                      /*  发送回电函数                */
        psamsungsiochanUart->pcbGetTxChar = callbackRoute;
        psamsungsiochanUart->pvGetTxArg   = pvCallbackArg;
        return    (ERROR_NONE);
        
    case SIO_CALLBACK_PUT_RCV_CHAR:                                     /*  接收回电函数                */
        psamsungsiochanUart->pcbPutRcvChar = callbackRoute;
        psamsungsiochanUart->pvPutRcvArg   = pvCallbackArg;
        return    (ERROR_NONE);
        
    default:
        _ErrorHandle(ENOSYS);
        return    (PX_ERROR);
    }
}
/*********************************************************************************************************
** Function name:           __uart485Delay
** Descriptions:            uart1 485 模式软延迟, 由于 485 收发器换向频率限制, 所以在发送前后, 必须加入
                            软件延迟, 此程序在 gcc -O3 -Os 测试通过
** input parameters:        iTimes          延迟循环数量 (一般为 40)
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static VOID  __uart485Delay (INT  iTimes)
{
    volatile int    i;
    
    for (; iTimes > 0; iTimes--) {
        for (i = 0; i < 15; i++);
    }
}
/*********************************************************************************************************
** Function name:           __uartStartup
** Descriptions:            SIO 通道发送(没有使用中断)
** input parameters:        psiochanChan                 通道
** output parameters:       NONE
** Returned value:          SIO 通道控制块指针
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT   __uartStartup (SIO_CHAN    *psiochanChan)
{
    __PSAMSUNGSIO_CHANNEL     psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    CHAR                      cTx;
    
    switch (psamsungsiochanUart->iChannelNum) {
    
    case COM0:
        INTER_CLR_SUBMSK(BIT_SUB_TXD0);                                 /*  使能 FIFO 空中断            */
        break;
    
    case COM1:
        if (psamsungsiochanUart->iRs485Flag == RS485_EN) {              /*  485 模式使用查询发送方式    */
            __COM1_RS485_SEND_START();                                  /*  485 换向(发送模式)          */
            __uart485Delay(40);                                         /*  短延迟                      */
            do {
                if (psamsungsiochanUart->pcbGetTxChar(psamsungsiochanUart->pvGetTxArg, &cTx)
                    != ERROR_NONE) {
                    break;                                              /*  发送完毕                    */
                }
                while (rUFSTAT1 & (1 << 14));                           /*  FIFO 未满                   */
                WrUTXH1(cTx);                                           /*  发送数据                    */
            } while (1);
            while ((rUTRSTAT1 & (1 << 2)) == 0);                        /*  等待发送缓冲器为空          */
            __COM1_RS485_SEND_END();                                    /*  485 换向(接收模式)          */
            __uart485Delay(40);                                         /*  短延迟                      */
        
        } else {                                                        /*  全双工模式采用中断发送      */
            INTER_CLR_SUBMSK(BIT_SUB_TXD1);                             /*  使能 FIFO 空中断            */
        }
        break;
        
    case COM2:
        INTER_CLR_SUBMSK(BIT_SUB_TXD2);                                 /*  使能 FIFO 空中断            */
        break;
    }
    
    return    (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __uartPollRxChar
** Descriptions:            SIO 通道轮询接收
** input parameters:        psiochanChan                 通道
** output parameters:       pcInChar                     接收的字节
** Returned value:          接收的个数
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT   __uartPollRxChar (SIO_CHAN    *psiochanChan, PCHAR  pcInChar)
{
    __PSAMSUNGSIO_CHANNEL     psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    
    switch (psamsungsiochanUart->iChannelNum) {
    
    case COM0:
        if (rUFSTAT0 & __COM_FIFO_CNT_MASK) {
            *pcInChar = RdURXH0();
        } else {
            _ErrorHandle(EAGAIN);
            return  (PX_ERROR);
        }
        break;
        
    case COM1:
        if (rUFSTAT1 & __COM_FIFO_CNT_MASK) {
            *pcInChar = RdURXH1();
        } else {
            _ErrorHandle(EAGAIN);
            return  (PX_ERROR);
        }
        break;
        
    case COM2:
        if (rUFSTAT2 & __COM_FIFO_CNT_MASK) {
            *pcInChar = RdURXH2();
        } else {
            _ErrorHandle(EAGAIN);
            return  (PX_ERROR);
        }
        break;
    }
    
    return    (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __uartPollTxChar
** Descriptions:            SIO 通道轮询发送
** input parameters:        psiochanChan                 通道
**                          cOutChar                     发送的字节
** output parameters:       NONE
** Returned value:          发送的个数
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT   __uartPollTxChar (SIO_CHAN   *psiochanChan, CHAR  cOutChar)
{
    __PSAMSUNGSIO_CHANNEL     psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    
    switch (psamsungsiochanUart->iChannelNum) {
    
    case COM0:
        while (rUFSTAT0 & (1 << 14));                                   /*  FIFO 未满                   */
        WrUTXH0(cOutChar);                                              /*  发送数据                    */
        break;
        
    case COM1:
        while (rUFSTAT1 & (1 << 14));                                   /*  FIFO 未满                   */
        WrUTXH1(cOutChar);                                              /*  发送数据                    */
        break;
        
    case COM2:
        while (rUFSTAT2 & (1 << 14));                                   /*  FIFO 未满                   */
        WrUTXH2(cOutChar);                                              /*  发送数据                    */
        break;
    }
    
    return    (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           __uartIsr
** Descriptions:            SIO 通道中断处理函数
** input parameters:        psiochanChan                 通道
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static irqreturn_t  __uartIsr (SIO_CHAN  *psiochanChan)
{
    __PSAMSUNGSIO_CHANNEL     psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    UCHAR                     ucData;
    
    switch (psamsungsiochanUart->iChannelNum) {
    
    case COM0:
        if (rSUBSRCPND & BIT_SUB_TXD0) {
            INTER_CLR_SUBSRCPND(BIT_SUB_TXD0);                          /*  清除发送中断                */
        }
        if (rSUBSRCPND & BIT_SUB_RXD0) {
            INTER_CLR_SUBSRCPND(BIT_SUB_RXD0);                          /*  清除接收中断                */
        }
        INTER_CLR_PNDING(BIT_UART0);                                    /*  清除中断标志                */
        while (((rUFSTAT0 >> 8) & __COM_FIFO_CNT_MASK) < __COM_FIFO_CNT) {
                                                                        /*  TxFIFO 未满                 */
            if (psamsungsiochanUart->pcbGetTxChar(psamsungsiochanUart->pvGetTxArg, 
                (PCHAR)&ucData) != ERROR_NONE) {                        /*  发送结束                    */
                INTER_SET_SUBMSK(BIT_SUB_TXD0);                         /*  发送结束                    */
                break;
            } else {
                WrUTXH0(ucData);                                        /*  发送数据                    */
            }
        }
        while (rUFSTAT0 & __COM_FIFO_CNT_MASK)  {                       /*  需要接收数据                */
            ucData = RdURXH0();
            psamsungsiochanUart->pcbPutRcvChar(psamsungsiochanUart->pvPutRcvArg, ucData);
        }
        break;
        
    case COM1:
        if (rSUBSRCPND & BIT_SUB_TXD1) {
            INTER_CLR_SUBSRCPND(BIT_SUB_TXD1);                          /*  清除发送中断                */
        }
        if (rSUBSRCPND & BIT_SUB_RXD1) {
            INTER_CLR_SUBSRCPND(BIT_SUB_RXD1);                          /*  清除接收中断                */
        }
        INTER_CLR_PNDING(BIT_UART1);                                    /*  清除中断标志                */
        while (((rUFSTAT1 >> 8) & __COM_FIFO_CNT_MASK) < __COM_FIFO_CNT) {
                                                                        /*  TxFIFO 未满                 */
            if (psamsungsiochanUart->pcbGetTxChar(psamsungsiochanUart->pvGetTxArg, 
                (PCHAR)&ucData) != ERROR_NONE) {                        /*  发送结束                    */
                INTER_SET_SUBMSK(BIT_SUB_TXD1);                         /*  发送结束                    */
                break;
            } else {
                WrUTXH1(ucData);                                        /*  发送数据                    */
            }
        }
        while (rUFSTAT1 & __COM_FIFO_CNT_MASK)  {                       /*  需要接收数据                */
            ucData = RdURXH1();
            psamsungsiochanUart->pcbPutRcvChar(psamsungsiochanUart->pvPutRcvArg, ucData);
        }
        break;
        
    case COM2:
        if (rSUBSRCPND & BIT_SUB_TXD2) {
            INTER_CLR_SUBSRCPND(BIT_SUB_TXD2);                          /*  清除发送中断                */
        }
        if (rSUBSRCPND & BIT_SUB_RXD2) {
            INTER_CLR_SUBSRCPND(BIT_SUB_RXD2);                          /*  清除接收中断                */
        }
        INTER_CLR_PNDING(BIT_UART2);                                    /*  清除中断标志                */
        while (((rUFSTAT2 >> 8) & __COM_FIFO_CNT_MASK) < __COM_FIFO_CNT) {
                                                                        /*  TxFIFO 未满                 */
            if (psamsungsiochanUart->pcbGetTxChar(psamsungsiochanUart->pvGetTxArg, 
                (PCHAR)&ucData) != ERROR_NONE) {                        /*  发送结束                    */
                INTER_SET_SUBMSK(BIT_SUB_TXD2);                         /*  发送结束                    */
                break;
            } else {
                WrUTXH2(ucData);                                        /*  发送数据                    */
            }
        }
        while (rUFSTAT2 & __COM_FIFO_CNT_MASK) {                        /*  需要接收数据                */
            ucData = RdURXH2();
            psamsungsiochanUart->pcbPutRcvChar(psamsungsiochanUart->pvPutRcvArg, ucData);
        }
        break;
    }
    
    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** Function name:           __uartIoctl
** Descriptions:            SIO 通道控制
** input parameters:        psiochanChan                 通道
**                          iCmd                         命令
**                          lArg                         参数
** output parameters:       NONE
** Returned value:          命令执行结果
** Created by:              Hanhui
** Created Date:            2007/09/24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT   __uartIoctl (SIO_CHAN  *psiochanChan, INT  iCmd, LONG  lArg)
{
    __PSAMSUNGSIO_CHANNEL   psamsungsiochanUart = (__PSAMSUNGSIO_CHANNEL)psiochanChan;
    
    UCHAR                   ucDataBits;
    UCHAR                   ucStopBits;
    UCHAR                   ucParity;
    
    switch (iCmd) {
    
    case SIO_BAUD_SET:                                                  /*  设置模特率                  */
        __uartHwOptionAnalyse(psamsungsiochanUart->iHwOption,
                              &ucDataBits,
                              &ucStopBits,
                              &ucParity);                               /*  获得具体参数                */
        uartInit(psamsungsiochanUart->iChannelNum, UNUSE_INF, 
                 ucDataBits, ucStopBits, 
                 ucParity, (INT)lArg, LW_NULL);                         /*  初始化串口                  */
        psamsungsiochanUart->iBaud = (INT)lArg;
        break;
        
    case SIO_BAUD_GET:                                                  /*  获得波特率                  */
        *((LONG *)lArg) = psamsungsiochanUart->iBaud;
        break;
    
    case SIO_HW_OPTS_SET:                                               /*  设置硬件参数                */
        __uartHwOptionAnalyse((INT)lArg,
                              &ucDataBits,
                              &ucStopBits,
                              &ucParity);                               /*  获得具体参数                */
        uartInit(psamsungsiochanUart->iChannelNum, UNUSE_INF, 
                 ucDataBits, ucStopBits, 
                 ucParity, psamsungsiochanUart->iBaud, LW_NULL);        /*  初始化串口                  */
        psamsungsiochanUart->iHwOption = (INT)lArg;                     /*  记录信息                    */
        break;
        
    case SIO_HW_OPTS_GET:                                               /*  获取硬件参数                */
        *((INT *)lArg) = psamsungsiochanUart->iHwOption;
        break;
    
    case SIO_OPEN:                                                      
        switch (psamsungsiochanUart->iChannelNum) {
        
        case COM0:
            pmDevOn(&psamsungsiochanUart->pmdev);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART0));
            INTER_CLR_SUBMSK(BIT_SUB_RXD0);                             /*  打开接收中断                */
            API_InterVectorEnable(VIC_CHANNEL_UART0);                   /*  使能串口中断                */
            break;
            
        case COM1:
            pmDevOn(&psamsungsiochanUart->pmdev);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART1));
            INTER_CLR_SUBMSK(BIT_SUB_RXD1);                             /*  打开接收中断                */
            API_InterVectorEnable(VIC_CHANNEL_UART1);                   /*  使能串口中断                */
            break;
            
        case COM2:
            pmDevOn(&psamsungsiochanUart->pmdev);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART2));
            INTER_CLR_SUBMSK(BIT_SUB_RXD2);                             /*  打开接收中断                */
            API_InterVectorEnable(VIC_CHANNEL_UART2);                   /*  使能串口中断                */
            break;
        }
        break;
        
    case SIO_HUP:                                                       /*  关闭串口                    */
        switch (psamsungsiochanUart->iChannelNum) {
        
        case COM0:
            pmDevOff(&psamsungsiochanUart->pmdev);
            API_InterVectorDisable(VIC_CHANNEL_UART0);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART0));
            break;
            
        case COM1:
            pmDevOff(&psamsungsiochanUart->pmdev);
            API_InterVectorDisable(VIC_CHANNEL_UART1);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART1));
            break;
            
        case COM2:
            pmDevOff(&psamsungsiochanUart->pmdev);
            API_InterVectorDisable(VIC_CHANNEL_UART2);
            INTER_CLR_PNDING((1u << VIC_CHANNEL_UART2));
            break;
        }
        break;
        
    case RS485_SET:                                                     /*  设置 485 状态               */
        if (psamsungsiochanUart->iChannelNum == COM1) {
            psamsungsiochanUart->iRs485Flag  =  (INT)lArg;
            if (lArg) {
                rGPGCON = (rGPGCON & ~(0x3)) | (0x1);                   /*  GPG0 输出                   */
                BIT_CLR(rGPGUP, 0);                                     /*  使能上拉                    */
                __COM1_RS485_SEND_END();                                /*  结束发送状态                */
            }
        }
        break;
        
    case RS485_GET:                                                     /*  获取 485 状态               */
        *((LONG *)lArg) = psamsungsiochanUart->iRs485Flag;
        break;
        
    default:
        _ErrorHandle(ENOSYS);
        return    (ENOSYS);
    }
    
    return    (ERROR_NONE);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
