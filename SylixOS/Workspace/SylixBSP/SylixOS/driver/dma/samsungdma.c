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
** 文   件   名: samsungdma.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 19 日
**
** 描        述: DMA 驱动程序
**
** BUG:
2009.09.16  DMA auto reload 错误修正.
2009.12.12  操作系统升级了相关的 DMA 驱动接口, 这里符合新 DMA 接口需求.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
#include "samsungdma.h"
/*********************************************************************************************************
  2440 DMA 通道数量定义
*********************************************************************************************************/
#define __2440_DMA_CHAN_NUM     4
/*********************************************************************************************************
  2440 DMA 通道结构
*********************************************************************************************************/
typedef struct {

    volatile unsigned int       uiSrcAddr;                              /*  源端地址                    */
    volatile unsigned int       uiSrcCtl;                               /*  源端地址控制                */
    
    volatile unsigned int       uiDstAddr;                              /*  目的端地址                  */
    volatile unsigned int       uiDstCtl;                               /*  目的端地址控制              */
    
    volatile unsigned int       uiDMACtl;                               /*  DMA 通道控制                */
    volatile unsigned int       uiDMAStat;                              /*  DMA 通道状态                */
    
    volatile unsigned int       uiCurScr;                               /*  DMA 当前源端地址            */
    volatile unsigned int       uiCurDst;                               /*  DMA 当前目的端地址          */
    
    volatile unsigned int       uiMaskTigger;                           /*  触发控制寄存器              */
    
} __DMA_PHY_CHANNEL;
/*********************************************************************************************************
  2410 DMA 地址定义
*********************************************************************************************************/
#define __2440_DMA_PHY_ADDR    {(__DMA_PHY_CHANNEL *)0x4b000000,        \
                                (__DMA_PHY_CHANNEL *)0x4b000040,        \
                                (__DMA_PHY_CHANNEL *)0x4b000080,        \
                                (__DMA_PHY_CHANNEL *)0x4b0000c0}
/*********************************************************************************************************
  四个通道的 DMA 控制器
*********************************************************************************************************/
__DMA_PHY_CHANNEL              *__GpdmaphychanTbl[__2440_DMA_CHAN_NUM] = __2440_DMA_PHY_ADDR;
/*********************************************************************************************************
  2440 DMA 关键性的操作参数 (DMASKTRIG)
*********************************************************************************************************/
#define __DMA_PHY_STOP          (1 << 2)                                /*  DMASKTRIG [2]               */
#define __DMA_PHY_ON            (1 << 1)                                /*  DMASKTRIG [1]               */
#define __DMA_SW_TRIGGER        (1 << 0)                                /*  DMASKTRIG [0]               */
/*********************************************************************************************************
  2440 DMA 关键性的操作参数 (DMASKTRIG)
*********************************************************************************************************/
#define __DMA_PHY_STAT          (0x3 << 20)                             /*  DSTAT [21:20]               */
/*********************************************************************************************************
** Function name:           __dmaHwIsr
** Descriptions:            DMA 中断处理例程
** input parameters:        iChannel    通道号
** output parameters:       NONE
** Returned value:          中断服务返回值
** Created by:              Hanhui
** Created Date:            2007/10/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static irqreturn_t  __dmaHwIsr (int  iChannel)
{
    switch (iChannel) {                                                 /*  清除指定中断标志            */
    
    case LW_DMA_CHANNEL0:
        INTER_CLR_PNDING(BIT_DMA0);
        break;
    
    case LW_DMA_CHANNEL1:
        INTER_CLR_PNDING(BIT_DMA1);
        break;
        
    case LW_DMA_CHANNEL2:
        INTER_CLR_PNDING(BIT_DMA2);
        break;
        
    case LW_DMA_CHANNEL3:
        INTER_CLR_PNDING(BIT_DMA3);
        break;
        
    default:
        return  (LW_IRQ_HANDLED);
    }
    
    API_DmaContext(iChannel);                                           /*  调用 DMA 处理函数           */
    
    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** Function name:           __dmaHwReset
** Descriptions:            初始化 DMA 并停止
** input parameters:        iChannel:   DMA 通道号
**                          pdmafuncs   没有使用
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2007/10/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void  __dmaHwReset (UINT   uiChannel, PLW_DMA_FUNCS  pdmafuncs)
{
    __DMA_PHY_CHANNEL   *pdmaphychanCtl = __GpdmaphychanTbl[uiChannel];
    
    if (uiChannel >= __2440_DMA_CHAN_NUM) {
        return;                                                         /*  通道出错                    */
    }
    
    pdmaphychanCtl->uiMaskTigger = (__DMA_PHY_STOP | __DMA_PHY_ON);     /*  停止 DMA                    */
    
    /*
     *  安装中断处理例程
     */
    switch (uiChannel) {
    
    case LW_DMA_CHANNEL0:
        INTER_CLR_PNDING(BIT_DMA0);                                     /*  清除悬挂中断标志            */
        API_InterVectorConnect(VIC_CHANNEL_DMA0, 
                               (PINT_SVR_ROUTINE)__dmaHwIsr,
                               (PVOID)uiChannel,
                               "dma0_isr");                             /*  安装中断处理例程            */
        INTER_CLR_MSK((1u << VIC_CHANNEL_DMA0));
        break;
        
    case LW_DMA_CHANNEL1:
        INTER_CLR_PNDING(BIT_DMA1);                                     /*  清除悬挂中断标志            */
        API_InterVectorConnect(VIC_CHANNEL_DMA1, 
                               (PINT_SVR_ROUTINE)__dmaHwIsr,
                               (PVOID)uiChannel,
                               "dma1_isr");                             /*  安装中断处理例程            */
        INTER_CLR_MSK((1u << VIC_CHANNEL_DMA1));
        break;
        
    case LW_DMA_CHANNEL2:
        INTER_CLR_PNDING(BIT_DMA2);                                     /*  清除悬挂中断标志            */
        API_InterVectorConnect(VIC_CHANNEL_DMA2, 
                               (PINT_SVR_ROUTINE)__dmaHwIsr,
                               (PVOID)uiChannel,
                               "dma2_isr");                             /*  安装中断处理例程            */
        INTER_CLR_MSK((1u << VIC_CHANNEL_DMA2));
        break;
    
    case LW_DMA_CHANNEL3:
        INTER_CLR_PNDING(BIT_DMA3);                                     /*  清除悬挂中断标志            */
        API_InterVectorConnect(VIC_CHANNEL_DMA3, 
                               (PINT_SVR_ROUTINE)__dmaHwIsr,
                               (PVOID)uiChannel,
                               "dma3_isr");                             /*  安装中断处理例程            */
        INTER_CLR_MSK((1u << VIC_CHANNEL_DMA3));
        break;
        
    }
}
/*********************************************************************************************************
** Function name:           __dmaHwGetStatus
** Descriptions:            获得 DMA 当前状态
** input parameters:        iChannel:               DMA 通道号
**                          pdmafuncs               没有使用
** output parameters:       NONE
** Returned value:          0:空闲  1:忙  -1:错误
** Created by:              Hanhui
** Created Date:            2007/10/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static int  __dmaHwGetStatus (UINT   uiChannel, PLW_DMA_FUNCS  pdmafuncs)
{
    __DMA_PHY_CHANNEL   *pdmaphychanCtl = __GpdmaphychanTbl[uiChannel];
    
    if (uiChannel >= __2440_DMA_CHAN_NUM) {
        return  (PX_ERROR);                                             /*  通道出错                    */
    }
    
    if (pdmaphychanCtl->uiDMAStat & __DMA_PHY_STAT) {                   /*  检测状态                    */
        return  (LW_DMA_STATUS_BUSY);
    } else {
        return  (LW_DMA_STATUS_IDLE);
    }
}
/*********************************************************************************************************
** Function name:           __dmaHwTransact
** Descriptions:            初始化一次 DMA 传输
** input parameters:        iChannel:                 DMA 通道号
**                          pdmafuncs                 没有使用
**                          pdmatMsg:                 DMA 装载参数
** output parameters:       NONE
** Returned value:          -1:错误   0:正常
** Created by:              Hanhui
** Created Date:            2007/10/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static int  __dmaHwTransact (UINT   uiChannel, PLW_DMA_FUNCS  pdmafuncs, PLW_DMA_TRANSACTION  pdmatMsg)
{
    __DMA_PHY_CHANNEL   *pdmaphychanCtl = __GpdmaphychanTbl[uiChannel];
    
    if (uiChannel >= __2440_DMA_CHAN_NUM) {
        return  (-1);                                                   /*  通道出错                    */
    }
    
    if (!pdmatMsg) {
        return  (-1);                                                   /*  消息指针错误                */
    }
    
    /*
     *  保存地址信息
     */
    pdmaphychanCtl->uiSrcAddr = (unsigned int)pdmatMsg->DMAT_pucSrcAddress;
    pdmaphychanCtl->uiDstAddr = (unsigned int)pdmatMsg->DMAT_pucDestAddress;
    
    /*
     *  设置源端地址控制信息
     */
    switch (pdmatMsg->DMAT_iSrcAddrCtl) {
    
    case LW_DMA_ADDR_INC:
        if (pdmatMsg->DMAT_ulOption & DMA_OPTION_SRCBUS_APB) {          /*  源端为 APB 总线             */
            pdmaphychanCtl->uiSrcCtl = (1 << 1);
        } else {                                                        /*  源端为 AHB 总线             */
            pdmaphychanCtl->uiSrcCtl = 0;
        }
        break;
        
    case LW_DMA_ADDR_FIX:
        if (pdmatMsg->DMAT_ulOption & DMA_OPTION_SRCBUS_APB) {          /*  源端为 APB 总线             */
            pdmaphychanCtl->uiSrcCtl = ((1 << 1) | 1);
        } else {                                                        /*  源端为 AHB 总线             */
            pdmaphychanCtl->uiSrcCtl = 1;
        }
        break;
        
    default:
        return  (PX_ERROR);                                             /*  不支持                      */
    }
    
    /*
     *  设置目的端地址控制信息
     */
    switch (pdmatMsg->DMAT_iDestAddrCtl) {
    
    case LW_DMA_ADDR_INC:
        if (pdmatMsg->DMAT_ulOption & DMA_OPTION_DSTBUS_APB) {          /*  目的端为 APB 总线           */
            pdmaphychanCtl->uiDstCtl = (1 << 1);
        } else {                                                        /*  目的端为 AHB 总线           */
            pdmaphychanCtl->uiDstCtl = 0;
        }
        break;
        
    case LW_DMA_ADDR_FIX:
        if (pdmatMsg->DMAT_ulOption & DMA_OPTION_DSTBUS_APB) {          /*  目的端为 APB 总线           */
            pdmaphychanCtl->uiDstCtl = ((1 << 1) | 1);
        } else {                                                        /*  目的端为 AHB 总线           */
            pdmaphychanCtl->uiDstCtl = 1;
        }
        break;
        
    default:
        return  (PX_ERROR);                                             /*  不支持                      */
    }
    
    /*
     *  设置 DMA 址控制信息
     */
    {
        int     iHandshake     = 0;                                     /*  非握手模式，测试用          */
        int     iSyncClk       = 0;                                     /*  请求段同步时钟              */
        int     iInterEn       = 1;                                     /*  允许中断                    */
        int     iTransferMode  = 0;                                     /*  卒发或单字传输              */
        int     iServiceMode   = 0;                                     /*  完全 or 单次传输模式        */
        int     iReqScr        = 0;                                     /*  请求源                      */
        int     iSwOrHwReg     = 0;                                     /*  请求启动方式                */
        int     iAutoReloadDis = 1;                                     /*  是否禁能自动加载            */
        int     iDataSizeOnce  = 0;                                     /*  一次传输的数据宽度          */
        int     iLength;                                                /*  传输的长度                  */
        
        if (pdmatMsg->DMAT_bHwHandshakeEn) {
            iHandshake = 1;                                             /*  使用硬件握手                */
        }
        
        if (pdmatMsg->DMAT_iTransMode & DMA_TRANSMODE_WHOLE) {
            iServiceMode = 1;                                           /*  完全传输模式                */
        }
        
        if (pdmatMsg->DMAT_iTransMode & DMA_TRANSMODE_CLKAHB) {
            iSyncClk = 1;                                               /*  AHB 时钟源                  */
        }
        
        if (pdmatMsg->DMAT_iTransMode & DMA_TRANSMODE_BURST) {
            iTransferMode = 1;                                          /*  卒发模式                    */
        }
        
        iReqScr = pdmatMsg->DMAT_iHwReqNum;                             /*  请求源编号                  */
        
        if (pdmatMsg->DMAT_bHwReqEn) {
            iSwOrHwReg = 1;                                             /*  硬件请求启动                */
        }
        
        if (pdmatMsg->DMAT_iTransMode & DMA_TRANSMODE_DBYTE) {
            iDataSizeOnce = 1;                                          /*  半字传输                    */
        } else if (pdmatMsg->DMAT_iTransMode & DMA_TRANSMODE_4BYTE) {
            iDataSizeOnce = 2;                                          /*  字传输                      */
        }
        
        switch (iDataSizeOnce) {                                        /*  确定传输长度                */
        
        case 0:
            iLength = (INT)pdmatMsg->DMAT_stDataBytes;
            break;
            
        case 1:
            iLength = (INT)pdmatMsg->DMAT_stDataBytes / 2;
            break;
            
        case 2:
            iLength = (INT)pdmatMsg->DMAT_stDataBytes / 4;
            break;
        }
        
        pdmaphychanCtl->uiDMACtl = ((unsigned)iHandshake << 31)
                                 | (iSyncClk       << 30)
                                 | (iInterEn       << 29)
                                 | (iTransferMode  << 28)
                                 | (iServiceMode   << 27)
                                 | (iReqScr        << 24)
                                 | (iSwOrHwReg     << 23)
                                 | (iAutoReloadDis << 22)
                                 | (iDataSizeOnce  << 20)
                                 | (iLength);                           /*  设置控制寄存器              */
                                 
        /*
         *  启动 DMA 
         */
        if (iSwOrHwReg == 0) {                                          /*  选择软件启动方式            */
            pdmaphychanCtl->uiMaskTigger = 
                            (__DMA_PHY_ON | __DMA_SW_TRIGGER);          /*  软件启动传输                */
        } else {
            pdmaphychanCtl->uiMaskTigger = __DMA_PHY_ON;                /*  进打开通道，等待硬件触发    */
        }
    }

    return  (0);
}
/*********************************************************************************************************
** Function name:           dmaGetFuncs
** Descriptions:            获取 2440 DMA 驱动
** input parameters:        iChannel        DMA 通道号
** output parameters:       pulMaxBytes     最大传输字节数
** Returned value:          指定通道 DMA 控制器操作函数集
** Created by:              Hanhui
** Created Date:            2007/10/18
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
PLW_DMA_FUNCS  dmaGetFuncs (int   iChannel, ULONG   *pulMaxBytes)
{
    static LW_DMA_FUNCS     pdmafuncsS3c2440a;
    
    if (pdmafuncsS3c2440a.DMAF_pfuncReset == LW_NULL) { 
        pdmafuncsS3c2440a.DMAF_pfuncReset  = __dmaHwReset;
        pdmafuncsS3c2440a.DMAF_pfuncTrans  = __dmaHwTransact;
        pdmafuncsS3c2440a.DMAF_pfuncStatus = __dmaHwGetStatus;
    }
    
    if (pulMaxBytes) {
        *pulMaxBytes = (1 * LW_CFG_MB_SIZE);
    }
    
    return  (&pdmafuncsS3c2440a);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
