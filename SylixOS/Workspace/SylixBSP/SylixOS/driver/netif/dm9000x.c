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
** 文   件   名: dm9000x.c
**
** 创   建   人: JiaoJinXing
**
** 文件创建日期: 2012 年 03 月 03 日
**
** 描        述: DM9000X 以太网芯片驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"
#include "SylixOS.h"
#include "driver/net/dm9000.h"
/*********************************************************************************************************
** 函数名称: dm9000IntIsr
** 功能描述: DM9000 中断处理
** 输　入  : dm9000
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static irqreturn_t  dm9000IntIsr (struct dm9000_netdev  *dm9000)
{
#define DM9000_INT_GPIO     1

    irqreturn_t ret;

    ret = API_GpioSvrIrq(DM9000_INT_GPIO);
    if (ret == LW_IRQ_HANDLED) {
        API_GpioClearIrq(DM9000_INT_GPIO);
        dm9000Isr(dm9000);
    }

    return  (ret);
}
/*********************************************************************************************************
** 函数名称: dm9000NetInit
** 功能描述: DM9000 总线, 中断初始化
** 输　入  : ip, netmask, gw   默认的网络地址配置
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void  dm9000NetInit (const char *ip, const char *netmask, const char *gw)
{
    static struct dm9000_netdev  dm9000;
    INT  ret;

    /*
     * 设置 GPA15 为 nGCS4
     */
    rGPACON  |= 1 << 15;

    /*
     * 设置 BANK4 总线宽度为 16 位, 使能等待
     */
    rBWSCON   = (rBWSCON & (~(0x07 << 16))) | (0x05 << 16);

    /*
     * 设置 BANK4 的访问时序以适合 DM9000 芯片
     */
    rBANKCON4 = (1 << 13) | (1 << 11) | (6 << 8) | (1 << 6) | (1 << 4) | (0 << 2) | (0 << 0);

    dm9000.init = NULL;
    dm9000.base = (addr_t)API_VmmIoRemapNocache((PVOID)0x20000000, LW_CFG_VMM_PAGE_SIZE);
    dm9000.io   = dm9000.base + 0x00000300;
    dm9000.data = dm9000.base + 0x00000304;

    dm9000.mac[0] = 0x08;
    dm9000.mac[1] = 0x09;
    dm9000.mac[2] = 0x0a;
    dm9000.mac[3] = 0x0b;
    dm9000.mac[4] = 0xaa;
    dm9000.mac[5] = 0x5a;

    ret = API_GpioRequestOne(DM9000_INT_GPIO, LW_GPIOF_OUT_INIT_LOW, "dm9000_eint");
    if (ret != ERROR_NONE) {
        return;
    }

    dm9000.irq = API_GpioSetupIrq(DM9000_INT_GPIO, LW_TRUE, 1);

    API_InterVectorConnect(dm9000.irq, (PINT_SVR_ROUTINE)dm9000IntIsr, (PVOID)&dm9000, "dm9000_isr");

    dm9000Init(&dm9000, ip, netmask, gw);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
