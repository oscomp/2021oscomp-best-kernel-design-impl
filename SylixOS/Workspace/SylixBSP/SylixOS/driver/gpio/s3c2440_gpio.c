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
** 文   件   名: s3c2440_gpio.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2014 年 05 月 17 日
**
** 描        述: S3C2440 GPIO 驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"
#include "SylixOS.h"                                                    /*  操作系统                    */
#include "s3c2440_gpio.h"
/*********************************************************************************************************
** 函数名称: s3c2440GpioGetDirection
** 功能描述: 获得指定 GPIO 方向
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
** 输  出  : 0: 输入 1:输出
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  s3c2440GpioGetDirection (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset)
{
    UINT32  uiTemp;

    if (uiOffset == 0) {
        uiTemp = (rGPBCON >> 2) & 0x3;
        if (uiTemp == 0) {
            return  (0);
        } else if (uiTemp == 1) {
            return  (1);
        } else {
            return  (-1);
        }
    } else if (uiOffset == 1) {
        return  (0);
    } else {
        return  (-1);
    }
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioDirectionInput
** 功能描述: 设置指定 GPIO 为输入模式
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
** 输  出  : 0: 正确 -1:错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  s3c2440GpioDirectionInput (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset)
{
    UINT32  uiTemp;

    if (uiOffset == 0) {
        uiTemp = rGPBCON;
        uiTemp &= ~(3 << 2);
        rGPBCON = uiTemp;

        return  (0);
    } else if (uiOffset == 1) {
        return  (0);
    } else {
        return  (-1);
    }
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioGet
** 功能描述: 获得指定 GPIO 电平
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
** 输  出  : 0: 低电平 1:高电平
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  s3c2440GpioGet (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset)
{
    UINT32  uiValue;
    UINT32  uiTemp;

    if (uiOffset == 0) {
        uiTemp = rGPBDAT;
        uiValue = (uiTemp & (1 << 1));
        if (uiValue) {
            return  (1);
        } else {
            return  (0);
        }
    } else if (uiOffset == 1) {
        return  (0);
    } else {
        return  (-1);
    }
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioDirectionOutput
** 功能描述: 设置指定 GPIO 为输出模式
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
**           iValue      输出电平
** 输  出  : 0: 正确 -1:错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  s3c2440GpioDirectionOutput (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset, INT  iValue)
{
    UINT32  uiTemp;

    if (uiOffset == 0) {
        uiTemp = rGPBCON;
        uiTemp = (uiTemp & ~(3U << 2)) | (1U << 2);
        rGPBCON = uiTemp;

        uiTemp = rGPBDAT;
        if (iValue) {
            uiTemp |= (1 << 1);
        } else {
            uiTemp &= ~(1 << 1);
        }
        rGPBDAT = uiTemp;

        return  (0);
    } else if (uiOffset == 1) {
        return  (0);
    } else {
        return  (-1);
    }
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioSet
** 功能描述: 设置指定 GPIO 电平
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
**           iValue      输出电平
** 输  出  : 0: 正确 -1:错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID  s3c2440GpioSet (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset, INT  iValue)
{
    UINT32  uiTemp;

    if (uiOffset != 0) {
        return;
    }

    uiTemp = rGPBDAT;
    if (iValue) {
        uiTemp |= (1 << 1);
    } else {
        uiTemp &= ~(1 << 1);
    }
    rGPBDAT = uiTemp;
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioSetupIrq
** 功能描述: 设置指定 GPIO 为外部中断输入管脚
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
**           bIsLevel    是否为电平触发
**           uiType      如果为电平触发, 1 表示高电平触发, 0 表示低电平触发
**                       如果为边沿触发, 1 表示上升沿触发, 0 表示下降沿触发, 2 双边沿触发
** 输  出  : IRQ 向量号 -1:错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ULONG  s3c2440GpioSetupIrq (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset, BOOL  bIsLevel, UINT  uiType)
{
    UINT32  uiValue;

    if (uiOffset != 1) {
        return  (-1);
    }

    rGPFCON   = (rGPFCON & (~(0x03 << 14))) | (0x02 << 14);             /*  设置 GPF7 为 EINT7          */

    rGPFUP    = rGPFUP | (1 << 7);                                      /*  关闭 GPF7 的上拉电阻        */

    if (bIsLevel) {
        if (uiType) {
            uiValue = 0x01;
        } else {
            uiValue = 0x00;
        }
    } else {
        if (uiType == 0) {
            uiValue = 0x02;
        } else if (uiType == 1) {
            uiValue = 0x04;
        } else {
            uiValue = 0x06;
        }
    }

    rEXTINT0  = (rEXTINT0 & (~(0x07 << 28))) | (uiValue << 28);         /*  设置 EINT7 中断触发方式     */

    rEINTMASK = rEINTMASK & (~(1 << 7));                                /*  使能 EINT7                  */

    return  (VIC_CHANNEL_EINT4_7);
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioClearIrq
** 功能描述: 清除指定 GPIO 中断标志
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
** 输  出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID  s3c2440GpioClearIrq (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset)
{
    UINT32  uiTemp;

    if (uiOffset != 1) {
        return;
    }

    uiTemp = rEINTPEND;
    rEINTPEND = uiTemp;

    INTER_CLR_PNDING(BIT_EINT4_7);
}
/*********************************************************************************************************
** 函数名称: s3c2440GpioSvrIrq
** 功能描述: 判断 GPIO 中断标志
** 输  入  : pGpioChip   GPIO 芯片
**           uiOffset    GPIO 针对 BASE 的偏移量
** 输  出  : 中断返回值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static irqreturn_t  s3c2440GpioSvrIrq (PLW_GPIO_CHIP  pGpioChip, UINT  uiOffset)
{
    UINT32  uiTemp;

    if (uiOffset != 1) {
        return  (LW_IRQ_NONE);
    }

    uiTemp = rEINTPEND;

    if (uiTemp & (1 << 7)) {
        return  (LW_IRQ_HANDLED);
    } else {
        return  (LW_IRQ_NONE);
    }
}
/*********************************************************************************************************
  GPIO 驱动程序
*********************************************************************************************************/
static LW_GPIO_CHIP  _G_s3c2440GpioChip = {
        .GC_pcLabel              = "S3C2440 GPIO",
        .GC_ulVerMagic           = LW_GPIO_VER_MAGIC,

        .GC_pfuncRequest         = LW_NULL,
        .GC_pfuncFree            = LW_NULL,

        .GC_pfuncGetDirection    = s3c2440GpioGetDirection,
        .GC_pfuncDirectionInput  = s3c2440GpioDirectionInput,
        .GC_pfuncGet             = s3c2440GpioGet,
        .GC_pfuncDirectionOutput = s3c2440GpioDirectionOutput,
        .GC_pfuncSetDebounce     = LW_NULL,
        .GC_pfuncSetPull         = LW_NULL,
        .GC_pfuncSet             = s3c2440GpioSet,
        .GC_pfuncSetupIrq        = s3c2440GpioSetupIrq,
        .GC_pfuncClearIrq        = s3c2440GpioClearIrq,
        .GC_pfuncSvrIrq          = s3c2440GpioSvrIrq,

        .GC_uiBase               = S3C2440_GPIO_BASE,
        .GC_uiNGpios             = 2,
};
/*********************************************************************************************************
** 函数名称: s3c2440GpioDrv
** 功能描述: 创建 S3C2440 GPIO 驱动
** 输  入  : NONE
** 输  出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  s3c2440GpioDrv (VOID)
{
    gpioChipAdd(&_G_s3c2440GpioChip);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
