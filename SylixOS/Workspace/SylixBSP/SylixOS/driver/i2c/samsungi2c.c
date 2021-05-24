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
** 文   件   名: samsungi2c.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 19 日
**
** 描        述: S3C2440 I2C 驱动(总线驱动部分)
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
/*********************************************************************************************************
  S3C2440A I2C 波特率参数 (约为100K)
*********************************************************************************************************/
#define __I2C_BPS_PARAM             (((PCLK / 512) / 100000) & 0xF)
/*********************************************************************************************************
  I2C CON 寄存器两种取值定义
*********************************************************************************************************/
#define __I2C_CON_DACK(iBpsParam)   ((1 << 7) | (1 << 6) | (1 << 5) | (iBpsParam))
#define __I2C_CON_DNACK(iBpsParam)  ((0 << 7) | (1 << 6) | (1 << 5) | (iBpsParam))
/*********************************************************************************************************
  I2C 物理控制器结构
*********************************************************************************************************/
typedef struct {
    int                 iStatus;                                        /*  状态                        */
    int                 iBpsParam;                                      /*  波特率参数                  */
    PLW_I2C_MESSAGE     pi2cmsg;                                        /*  需要处理的消息              */
    int                 iMsgPtr;                                        /*  消息内部指针                */
    int                 iMsgNum;                                        /*  消息数量                    */
    int                 iMsgIndex;                                      /*  当前处理的 msg 下标         */
} __SAMSUNGI2C_CHANNEL;
typedef __SAMSUNGI2C_CHANNEL        *__PSAMSUNGI2C_CHANNEL;
/*********************************************************************************************************
  I2C 消息数量判断
*********************************************************************************************************/
#define __I2C_BUS_IS_LASTMSG(psamsungi2c)   (psamsungi2c->iMsgIndex >= (psamsungi2c->iMsgNum - 1))
                                                                        /*  最后一个消息或没有消息      */
#define __I2C_BUS_IS_MSGLAST(psamsungi2c)   (psamsungi2c->iMsgPtr ==    \
                                             (psamsungi2c->pi2cmsg->I2CMSG_usLen - 1))
                                                                        /*  消息的最后一个字节          */
#define __I2C_BUS_IS_MSGEND(psamsungi2c)    (psamsungi2c->iMsgPtr >= psamsungi2c->pi2cmsg->I2CMSG_usLen)
                                                                        /*  消息结束                    */
/*********************************************************************************************************
  I2C 总线状态
*********************************************************************************************************/
#define __I2C_BUS_STATE_IDLE        0                                   /*  总线空闲                    */
#define __I2C_BUS_STATE_START       1                                   /*  总线启动                    */
#define __I2C_BUS_STATE_READ        2                                   /*  读数据                      */
#define __I2C_BUS_STATE_WRITE       3                                   /*  写数据                      */
#define __I2C_BUS_STATE_STOP        4                                   /*  总线结束                    */
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
static LW_OBJECT_HANDLE             __GhI2cSignal;                      /*  i2c 中断                    */
#define __I2C_BUS_SIGNAL()          API_SemaphoreBPost(__GhI2cSignal)
#define __I2C_BUS_WAIT(ulTimeout)   API_SemaphoreBPend(__GhI2cSignal, ulTimeout)
static LW_I2C_FUNCS                 __Gi2cfuncSamsung;                  /*  i2c 总线函数                */
static __SAMSUNGI2C_CHANNEL         __Gsamsungi2c;                      /*  i2c 控制器                  */
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
static VOID  __samsungI2cStart(UINT16  usAddr, UINT16  usFlag);
static VOID  __samsungI2cStop(UINT16  usFlag);
/*********************************************************************************************************
** Function name:           __samsungI2cIsr
** Descriptions:            i2c 控制器中断处理函数
** input parameters:        psamsungi2c     控制器
** output parameters:       NONE
** Returned value:          中断服务返回值
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static irqreturn_t  __samsungI2cIsr (__PSAMSUNGI2C_CHANNEL   psamsungi2c)
{
    BYTE        ucByte;
    ULONG       ulStatus = rIICSTAT;                                    /*  i2c 状态                    */

    INTER_CLR_PNDING(BIT_IIC);                                          /*  清除中断标志                */

    switch (psamsungi2c->iStatus) {                                     /*  处理不同状态                */
    
    case __I2C_BUS_STATE_IDLE:
        break;                                                          /*  直接退出                    */
    
    case __I2C_BUS_STATE_START:
        if ((ulStatus & 0x09) ||                                        /*  出现错误                    */
            (__I2C_BUS_IS_LASTMSG(psamsungi2c) &&                       /*  没有待处理的消息            */
             (psamsungi2c->pi2cmsg->I2CMSG_usLen == 0))) {
            errno = ENXIO;
            __samsungI2cStop(0);                                        /*  停止总线                    */
            break;
        }

        if (psamsungi2c->pi2cmsg->I2CMSG_usFlag & LW_I2C_M_RD) {        /*  进入读状态                  */
            psamsungi2c->iStatus = __I2C_BUS_STATE_READ;
            goto    __prepare_read;
        } else {
            psamsungi2c->iStatus = __I2C_BUS_STATE_WRITE;               /*  进入写状态                  */
            goto    __prepare_write;
        }
        break;
    
    case __I2C_BUS_STATE_READ:
        ucByte = (BYTE)rIICDS;                                          /*  读取数据                    */
        psamsungi2c->pi2cmsg->I2CMSG_pucBuffer[psamsungi2c->iMsgPtr] = ucByte;
        psamsungi2c->iMsgPtr++;
        
__prepare_read:
        if (__I2C_BUS_IS_MSGLAST(psamsungi2c)) {                        /*  这是当前消息的最后一个字节  */
            if (__I2C_BUS_IS_LASTMSG(psamsungi2c)) {                    /*  这是最后一个消息            */
                rIICCON = __I2C_CON_DNACK(psamsungi2c->iBpsParam);      /*  启动总线不需要 ACK          */
            }
        } else if (__I2C_BUS_IS_MSGEND(psamsungi2c)) {                  /*  当前消息已经结束            */
            if (__I2C_BUS_IS_LASTMSG(psamsungi2c)) {                    /*  这是最后一个消息            */
                psamsungi2c->iMsgPtr = 0;
                psamsungi2c->iMsgIndex++;                               /*  保证与消息数量相同          */
                __samsungI2cStop(LW_I2C_M_RD);
            } else {
                psamsungi2c->iMsgPtr = 0;
                psamsungi2c->iMsgIndex++;
                psamsungi2c->pi2cmsg++;                                 /*  开始接收下一个消息          */
                rIICCON = __I2C_CON_DACK(psamsungi2c->iBpsParam);       /*  启动接收允许 ACK            */
            }
        } else {
            rIICCON = __I2C_CON_DACK(psamsungi2c->iBpsParam);           /*  启动接收允许 ACK            */
        }
        break;
    
    case __I2C_BUS_STATE_WRITE:
        if ((ulStatus & 0x01) &&                                        /*  需要 ACK 但没有接收到 ACK   */
            !(psamsungi2c->pi2cmsg->I2CMSG_usFlag & 
              LW_I2C_M_IGNORE_NAK)) {
            errno = ECONNREFUSED;
            __samsungI2cStop(0);                                        /*  停止总线                    */
            break;
        }
        
__prepare_write:
        if (!__I2C_BUS_IS_MSGEND(psamsungi2c)) {                        /*  当前消息还没有发送完        */
            ucByte = psamsungi2c->pi2cmsg->I2CMSG_pucBuffer[psamsungi2c->iMsgPtr];
            psamsungi2c->iMsgPtr++;
            rIICDS  = ucByte;
            rIICCON = __I2C_CON_DACK(psamsungi2c->iBpsParam);           /*  发送                        */
        
        } else if (!__I2C_BUS_IS_LASTMSG(psamsungi2c)) {                /*  还有剩余的消息没有发送完    */
            psamsungi2c->iMsgPtr = 0;
            psamsungi2c->iMsgIndex++;
            psamsungi2c->pi2cmsg++;                                     /*  开始发送下一个消息          */
            
            if (psamsungi2c->pi2cmsg->I2CMSG_usFlag & 
                LW_I2C_M_NOSTART) {                                     /*  不需要起始位                */
                if (psamsungi2c->pi2cmsg->I2CMSG_usFlag &
                    LW_I2C_M_RD) {                                      /*  读操作                      */
                    /*
                     *  控制器换向, 必须重启总线
                     */
                    __samsungI2cStop(0);                                /*  不能进行读操作              */
                }
                goto    __prepare_write;
                
            } else {
                __samsungI2cStart(psamsungi2c->pi2cmsg->I2CMSG_usAddr, 
                                  psamsungi2c->pi2cmsg->I2CMSG_usFlag); /*  重启总线                    */
            }
        } else {
            psamsungi2c->iMsgPtr = 0;
            psamsungi2c->iMsgIndex++;                                   /*  保证与消息数量相同          */
            __samsungI2cStop(0);                                        /*  发送结束                    */
        }
        break;
    
    case __I2C_BUS_STATE_STOP:
        API_InterVectorDisable(VIC_CHANNEL_IIC);                        /*  关闭总线中断                */
        break;
    }
    
    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** Function name:           __samsungI2cInit
** Descriptions:            i2c 控制器初始化
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static VOID  __samsungI2cInit (VOID)
{
    rGPECON &= ~(0xFu << 28);
    rGPECON |=  (0xAu << 28);                                           /*  设置管脚链接                */

    rGPEUP   = rGPEUP | 0xC000;                                         /*  禁止内部上拉电阻            */

    rIICCON  = ((1 << 7) | (1 << 6) | (1 << 5) | (0));                  /*  初始化                      */
    rIICADD  = 0x10;                                                    /*  本地从机地址                */
    rIICSTAT =  (3 << 6) | (1 << 4);                                    /*  使能 I2C 总线               */
    rIICLC   =  (1 << 2) | (1);                                         /*  Filter enable 15 clocks     */
    
    __GhI2cSignal = API_SemaphoreBCreate("i2c_signal", LW_FALSE, LW_OPTION_OBJECT_GLOBAL, LW_NULL);
    
    API_InterVectorConnect(VIC_CHANNEL_IIC, (PINT_SVR_ROUTINE)__samsungI2cIsr, 
                           (PVOID)&__Gsamsungi2c, "i2c_isr");
}
/*********************************************************************************************************
** Function name:           __samsungI2cStart
** Descriptions:            i2c 控制器发送启动字节
** input parameters:        usAddr      地址
**                          usFlag      标志
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static VOID  __samsungI2cStart (UINT16  usAddr, UINT16  usFlag)
{
    __Gsamsungi2c.iStatus = __I2C_BUS_STATE_START;                      /*  启动总线                    */

    API_InterVectorEnable(VIC_CHANNEL_IIC);                             /*  打开总线中断                */

    if (usFlag & LW_I2C_M_RD) {                                         /*  读操作                      */
        rIICDS    = (BYTE)(usAddr + 1);                                 /*  设置器件读地址              */
        rIICSTAT  = (2 << 6) | (1 << 5) | (1 << 4);                     /*  主收模式启动                */
        rIICCON   = __I2C_CON_DACK(__Gsamsungi2c.iBpsParam);            /*  重启总线 需要有此操作       */
    } else {
        rIICDS    = (BYTE)usAddr;
        rIICSTAT  = (3 << 6) | (1 << 5) | (1 << 4);                     /*  主发模式启动                */
    }
}
/*********************************************************************************************************
** Function name:           __samsungI2cStop
** Descriptions:            i2c 控制器发送停止总线
** input parameters:        usFlag      标志
** output parameters:       NONE
** Returned value:          NONE
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static VOID  __samsungI2cStop (UINT16  usFlag)
{
    if (usFlag & LW_I2C_M_RD) {                                         /*  读操作                      */
        rIICSTAT  = (2 << 6) | (0 << 5) | (1 << 4);                     /*  主收模式停止                */
    } else {                                                            /*  写操作                      */
        rIICSTAT  = (3 << 6) | (0 << 5) | (1 << 4);                     /*  主发模式停止                */
    }

    rIICCON = __I2C_CON_DACK(__I2C_BPS_PARAM);                          /*  允许 ACK                    */
    
    __Gsamsungi2c.iStatus = __I2C_BUS_STATE_STOP;                       /*  停止总线                    */
    
    __I2C_BUS_SIGNAL();                                                 /*  激活等待任务                */
}
/*********************************************************************************************************
** Function name:           __samsungI2cTransferOne
** Descriptions:            i2c 传输函数
** input parameters:        pi2cadapter     i2c 适配器
**                          pi2cmsg         i2c 传输消息
**                          iNum            需要传输的 msg 数量
** output parameters:       NONE
** Returned value:          完成传输的消息数量
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT  __samsungI2cDoTransfer (PLW_I2C_ADAPTER   pi2cadapter,
                                    PLW_I2C_MESSAGE   pi2cmsg,
                                    INT               iNum)
{
    __Gsamsungi2c.iStatus    = __I2C_BUS_STATE_START;
    __Gsamsungi2c.iBpsParam  = __I2C_BPS_PARAM;
    __Gsamsungi2c.pi2cmsg    = pi2cmsg;
    __Gsamsungi2c.iMsgPtr    = 0;
    __Gsamsungi2c.iMsgNum    = iNum;
    __Gsamsungi2c.iMsgIndex  = 0;                                       /*  从第一个开始发送            */

    __samsungI2cStart(pi2cmsg->I2CMSG_usAddr, pi2cmsg->I2CMSG_usFlag);  /*  启动总线                    */
    
    __I2C_BUS_WAIT(LW_OPTION_WAIT_A_SECOND * 3);                        /*  最多等待 3 秒钟             */
    
    /*
     *  此函数退出后如果还有 i2c 中断, 可能导致 msg[] 局部数组无效. 程序崩溃.
     */
    API_InterVectorDisable(VIC_CHANNEL_IIC);                            /*  关闭总线中断                */
    
    return  (__Gsamsungi2c.iMsgIndex);                                  /*  返回传输成功的数量          */
}
/*********************************************************************************************************
** Function name:           __samsungI2cTransfer
** Descriptions:            i2c 传输函数
** input parameters:        pi2cadapter     i2c 适配器
**                          pi2cmsg         i2c 传输消息组
**                          iNum            消息数量
** output parameters:       NONE
** Returned value:          完成传输的消息数量
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT  __samsungI2cTransfer (PLW_I2C_ADAPTER   pi2cadapter,
                                  PLW_I2C_MESSAGE   pi2cmsg,
                                  INT               iNum)
{
    REGISTER INT        i;
    
    for (i = 0; i < pi2cadapter->I2CADAPTER_iRetry; i++) {
        if (__samsungI2cDoTransfer(pi2cadapter, pi2cmsg, iNum) == iNum) {
            return  (iNum);
        } else {
            API_TimeSleep(LW_OPTION_WAIT_A_TICK);                       /*  等待一个机器周期重试        */
        }
    }
    
    return  (PX_ERROR);
}
/*********************************************************************************************************
** Function name:           i2cBusFuns
** Descriptions:            初始化 i2c 总线并获取操作函数集
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          总线操作函数集
** Created by:              Hanhui
** Created Date:            2009-10-20
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
PLW_I2C_FUNCS  i2cBusFuns (VOID)
{
    __Gi2cfuncSamsung.I2CFUNC_pfuncMasterXfer = __samsungI2cTransfer;
    
    __samsungI2cInit();
    
    return  (&__Gi2cfuncSamsung);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
