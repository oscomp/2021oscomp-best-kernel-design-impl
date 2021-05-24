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
** 文   件   名: touchscr.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 04 月 29 日
**
** 描        述: 2440 触摸屏接口
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "config.h"                                                     /*  工程配置 & 处理器相关       */
#include "SylixOS.h"
#include "touchscr.h"
#include "mouse.h"
/*********************************************************************************************************
  touch screen 设备结构
*********************************************************************************************************/
typedef struct {
    LW_DEV_HDR                 TS_devhdr;                               /*  设备头                      */
    touchscreen_event_notify   TS_tData;                                /*  采集到的数据                */
    BOOL                       TS_bIsReadRel;                           /*  是否读取的 release 操作     */
    LW_HANDLE                  TS_hThread;                              /*  扫描线程                    */
    LW_SEL_WAKEUPLIST          TS_selwulList;                           /*  select() 等待链             */
    LW_SPINLOCK_DEFINE        (TS_slLock);                              /*  自旋锁                      */
} TS_DEV;
/*********************************************************************************************************
  定义全局变量, 用于保存can驱动号
*********************************************************************************************************/
static INT                  touch_dev_num = PX_ERROR;
static UINT32               xp = 0;
static UINT32               yp = 0;
static UINT32               abs_xp = 0;
static UINT32               abs_yp = 0;
static int                  send_press = 0;
static LW_OBJECT_HANDLE     adc_sem;
static UINT32               down_start = 0;

#define S3C2440_ADCTSC_YM_SEN       (1 << 7)
#define S3C2440_ADCTSC_YP_SEN       (1 << 6)
#define S3C2440_ADCTSC_XP_SEN       (1 << 4)
#define S3C2440_ADCTSC_AUTO_PST     (1 << 2)
#define S3C2440_ADCTSC_XY_PST(x)    (x << 0)
/*********************************************************************************************************
  自动转换设置
*********************************************************************************************************/
#define AUTOPST     (S3C2440_ADCTSC_YM_SEN | S3C2440_ADCTSC_YP_SEN | S3C2440_ADCTSC_XP_SEN | \
                     S3C2440_ADCTSC_AUTO_PST | S3C2440_ADCTSC_XY_PST(0))
/*********************************************************************************************************
  等待触屏按下
*********************************************************************************************************/
#define WAIT4INT(x) (((x)<<8) | S3C2440_ADCTSC_YM_SEN | S3C2440_ADCTSC_YP_SEN | \
                     S3C2440_ADCTSC_XP_SEN | S3C2440_ADCTSC_XY_PST(3))

#define S3C2440_ADCDAT0_UPDOWN      (1 << 15)
/*********************************************************************************************************
** 函数名称: getAbsX
** 功能描述: 获取压力
** 输　入  : uiX     X 轴测量值
** 输　出  : 计算出的压力值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static unsigned int getAbsX (unsigned int uiX)
{
    unsigned int    iPressure;

    iPressure = uiX;

    return  (iPressure);
}
/*********************************************************************************************************
** 函数名称: getAbsY
** 功能描述: 获取压力
** 输　入  : uiY     Y 轴测量值
** 输　出  : 计算出的压力值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static unsigned int getAbsY (unsigned int uiY)
{
    unsigned int    iPressure;

    iPressure = uiY;

    return  (iPressure);
}
/*********************************************************************************************************
** 函数名称: touchIsr
** 功能描述: adc的中断服务程序
** 输　入  : NONE
** 输　出  : 中断返回值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static irqreturn_t touchIsr (void *arg)
{
    UINT32  x_data,y_data;
    INT     updown = 0;

    /*
     * 读取当前是否是触摸屏按下导致的中断
     */

    /*
     * 读取当前是否是按下状态
     */
    INTER_CLR_SUBSRCPND(BIT_SUB_ADC);
    INTER_CLR_SUBSRCPND(BIT_SUB_TC);
    INTER_CLR_PNDING(BIT_ADC);

    x_data = rADCDAT0;
    y_data = rADCDAT1;

    updown = (!(x_data & S3C2440_ADCDAT0_UPDOWN)) && (!(y_data & S3C2440_ADCDAT0_UPDOWN));

    if (updown) {
        if (down_start == 0) {
            rADCTSC  = AUTOPST;
            rADCCON |= 0x01;                                            /* 启动AD转换                   */
            down_start++;

        } else {
            xp = (x_data & 0x3ff);
            yp = (y_data & 0x3ff);

                                                                        /* 计算abs_x,abs_y值            */
            abs_xp = getAbsX(xp);
            abs_yp = getAbsY(yp);
            send_press = 1;
                                                                        /* 释放一个信号给采集任务       */
            API_SemaphoreBFlush(adc_sem, NULL);
            xp = 0;
            yp = 0;

                                                                        /* 启动触屏按下判定             */
            down_start = 0;
            rADCTSC = WAIT4INT(0);
        }
    } else {                                                            /* 标识当前没有被按下的标准     */
        send_press = 0;
        down_start = 0;
        xp = 0;
        yp = 0;

                                                                        /* 释放一个信号给等待任务       */
        API_SemaphoreBFlush(adc_sem, NULL);
        rADCTSC = WAIT4INT(0);
    }

    return  (LW_IRQ_HANDLED);
}
/*********************************************************************************************************
** 函数名称: touchInit
** 功能描述: 初始化 触摸屏
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static void  touchInit (void)
{
    API_SemaphoreBCreate("ad_thread_sem", 0, LW_OPTION_WAIT_FIFO, &adc_sem);

    rADCCON = ((1<<14) | (49 << 6));
    rADCDLY = 40000;

    /*
     * 安装ADC 中断服务程序,
     */
    API_InterVectorConnect(VIC_CHANNEL_ADC,
                           (PINT_SVR_ROUTINE)touchIsr,
                           (PVOID)NULL,
                           "touchscr");
    API_InterVectorEnable(VIC_CHANNEL_ADC);


    rINTSUBMSK     &= ~(BIT_SUB_TC) ;
    rINTSUBMSK     &= ~(BIT_SUB_ADC) ;

    /*
     * 设置成中断等待模式，等待触摸屏被按下
     */
    rADCTSC = WAIT4INT(0);
}
/*********************************************************************************************************
** 函数名称: touchGetXY
** 功能描述: 获得触摸屏物理电压 X Y
** 输　入  : pX              X 轴方向电压
             pY              Y 轴方向电压
** 输　出  : 1: 表示点有效   0: 表示点无效
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static int  touchGetXY (int  *pX, int  *pY)
{
    API_SemaphoreBPend(adc_sem, LW_OPTION_WAIT_INFINITE);

    /*
     * qemu_2440 虚拟机 x y 是相反的.
     */
    *pX = abs_yp;
    *pY = abs_xp;
    return send_press;
}
/*********************************************************************************************************
** 函数名称: touchThread
** 功能描述: 触摸屏扫描线程
** 输　入  : ptsDev        设备
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static void touchThread (TS_DEV  *ptsDev)
{
    int                  ix, iy;
    INTREG               iregInterLevel;

    while (1) {
        if (touchGetXY(&ix, &iy)) {
            /*
             *  当前有点击操作.
             */
            LW_SPIN_LOCK_QUICK(&ptsDev->TS_slLock, &iregInterLevel);
            ptsDev->TS_tData.kstat |= MOUSE_LEFT;
            ptsDev->TS_tData.xmovement = ix;
            ptsDev->TS_tData.ymovement = iy;
            LW_SPIN_UNLOCK_QUICK(&ptsDev->TS_slLock, iregInterLevel);
            SEL_WAKE_UP_ALL(&ptsDev->TS_selwulList,
                            SELREAD);                                   /*  释放所有等待读的线程        */
        } else {
            /*
             *  当前没有点击操作.
             */
            if (ptsDev->TS_tData.kstat & MOUSE_LEFT) {
                LW_SPIN_LOCK_QUICK(&ptsDev->TS_slLock, &iregInterLevel);
                ptsDev->TS_tData.kstat &= (~MOUSE_LEFT);
                LW_SPIN_UNLOCK_QUICK(&ptsDev->TS_slLock, iregInterLevel);
            }

            if (ptsDev->TS_bIsReadRel == LW_FALSE) {                    /*  没有读取到释放操作          */
                SEL_WAKE_UP_ALL(&ptsDev->TS_selwulList,
                                SELREAD);                               /*  释放所有等待读的线程        */
            }
        }
    }
}
/*********************************************************************************************************
** 函数名称: touchIoctl
** 功能描述: 触摸屏 ioctl
** 输　入  : ptsDev           设备
**           iCmd             控制命令
**           lArg             参数
** 输　出  : ERROR_NONE or PX_ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT touchIoctl (TS_DEV  *ptsDev, INT  iCmd, LONG  lArg)
{
    PLW_SEL_WAKEUPNODE   pselwunNode;
    INT                  iError = ERROR_NONE;

    switch (iCmd) {

    case FIOSELECT:
        pselwunNode = (PLW_SEL_WAKEUPNODE)lArg;
        SEL_WAKE_NODE_ADD(&ptsDev->TS_selwulList, pselwunNode);
        break;

    case FIOUNSELECT:
        SEL_WAKE_NODE_DELETE(&ptsDev->TS_selwulList, (PLW_SEL_WAKEUPNODE)lArg);
        break;

    default:
        iError = PX_ERROR;
        errno  = ENOSYS;
        break;
    }

     return  (iError);
}
/*********************************************************************************************************
** 函数名称: touchOpen
** 功能描述: 触摸屏 open
** 输　入  : ptsDev               设备
**           pcName               设备名称
**           iFlags               打开设备时使用的标志
**           iMode                打开的方式，保留
** 输　出  : ERROR_NONE or PX_ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static LONG  touchOpen (TS_DEV  *ptsDev,
                        PCHAR    pcName,
                        INT      iFlags,
                        INT      iMode)
{
    LW_CLASS_THREADATTR  threadattr;

    if (pcName == LW_NULL) {
        _ErrorHandle(ERROR_IO_NO_DEVICE_NAME_IN_PATH);
        return    (PX_ERROR);
    }

    if (LW_DEV_INC_USE_COUNT(&ptsDev->TS_devhdr) == 1) {
        touchInit();

        ptsDev->TS_tData.ctype   = MOUSE_CTYPE_ABS;
        ptsDev->TS_tData.kstat   = 0;
        ptsDev->TS_tData.xanalog = 0;
        ptsDev->TS_tData.yanalog = 0;
        ptsDev->TS_bIsReadRel    = LW_TRUE;

        threadattr = API_ThreadAttrGetDefault();
        threadattr.THREADATTR_pvArg      = (void *)ptsDev;
        threadattr.THREADATTR_ucPriority = LW_PRIO_T_SERVICE;
        threadattr.THREADATTR_ulOption  |= LW_OPTION_OBJECT_GLOBAL;

        ptsDev->TS_hThread = API_ThreadCreate("t_touch",
                                              (PTHREAD_START_ROUTINE)touchThread,
                                              &threadattr, LW_NULL);
        return  ((LONG)ptsDev);
    }

    _ErrorHandle(ERROR_IO_FILE_EXIST);
    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: touchClose
** 功能描述: 触摸屏 close
** 输　入  : ptsDev               设备
** 输　出  : ERROR_NONE or PX_ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT touchClose (TS_DEV  *ptsDev)
{

    if (LW_DEV_GET_USE_COUNT(&ptsDev->TS_devhdr)) {
        if (!LW_DEV_DEC_USE_COUNT(&ptsDev->TS_devhdr)) {
            SEL_WAKE_UP_ALL(&ptsDev->TS_selwulList,
                            SELEXCEPT);                                 /*  激活异常等待                */
            SEL_WAKE_UP_ALL(&ptsDev->TS_selwulList,
                            SELWRITE);                                  /*  激活异常等待                */
            SEL_WAKE_UP_ALL(&ptsDev->TS_selwulList,
                            SELREAD);                                   /*  激活异常等待                */
            API_ThreadDelete(&ptsDev->TS_hThread, LW_NULL);
        }
    }
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: touchClose
** 功能描述: 触摸屏 read
** 输　入  : ptsDev           设备
**           pnotify          缓冲区指针
**           stNbyte          缓冲区大小字节数
** 输　出  : 读取字节数
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t touchRead (TS_DEV                      *ptsDev,
                          touchscreen_event_notify    *pnotify,
                          size_t                       stNbyte)
{
    INTREG  iregInterLevel;

    if (stNbyte == 0) {
        return  (0);
    }

    LW_SPIN_LOCK_QUICK(&ptsDev->TS_slLock, &iregInterLevel);

    pnotify->ctype   = ptsDev->TS_tData.ctype;
    pnotify->kstat   = ptsDev->TS_tData.kstat;
    pnotify->xanalog = ptsDev->TS_tData.xanalog;
    pnotify->yanalog = ptsDev->TS_tData.yanalog;

    if (ptsDev->TS_tData.kstat & MOUSE_LEFT) {                          /*  读取到点击事件              */
        ptsDev->TS_bIsReadRel = LW_FALSE;                               /*  需要确保应用读到释放操作    */
    } else {
        ptsDev->TS_bIsReadRel = LW_TRUE;                                /*  已经读取到释放操作          */
    }

    LW_SPIN_UNLOCK_QUICK(&ptsDev->TS_slLock, iregInterLevel);

    return  (sizeof(touchscreen_event_notify));
}
/*********************************************************************************************************
** 函数名称: tsDrv
** 功能描述: 安装触摸屏驱动
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  tsDrv (void)
{
    if (touch_dev_num > 0) {
        return  (ERROR_NONE);
    }

    touch_dev_num = iosDrvInstall(touchOpen, LW_NULL, touchOpen, touchClose,
                                  touchRead, LW_NULL, touchIoctl);

    DRIVER_LICENSE(touch_dev_num,     "Dual BSD/GPL->Ver 1.0");
    DRIVER_AUTHOR(touch_dev_num,      "Li.xiaocheng");
    DRIVER_DESCRIPTION(touch_dev_num, "touch screen driver.");

    return  (touch_dev_num > 0) ? (ERROR_NONE) : (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: tsDevCreate
** 功能描述: 创建触摸屏设备
** 输　入  : pcName    设备名
** 输　出  : 创建是否成功
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  tsDevCreate (PCHAR     pcName)
{
    TS_DEV   *ptsDev;
    INT       iTemp;

    if (!pcName) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "parameter error.\r\n");
        _ErrorHandle(EINVAL);
        return    (PX_ERROR);
    }

    if (touch_dev_num <= 0) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "can Driver invalidate.\r\n");
        _ErrorHandle(ERROR_IO_NO_DRIVER);
        return    (PX_ERROR);
    }

    ptsDev = (TS_DEV *)sys_malloc(sizeof(TS_DEV));
    if (!ptsDev) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return    (PX_ERROR);
    }
    lib_bzero(ptsDev, sizeof(TS_DEV));

    SEL_WAKE_UP_LIST_INIT(&ptsDev->TS_selwulList);                      /*  初始化 select 等待链        */
    LW_SPIN_INIT(&ptsDev->TS_slLock);                                   /*  初始化自旋锁                */

    iTemp = (INT)iosDevAdd(&ptsDev->TS_devhdr,
                           pcName,
                           touch_dev_num);
    if (iTemp) {
        sys_free(ptsDev);
        _DebugHandle(__ERRORMESSAGE_LEVEL, "add device error.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return    (PX_ERROR);

    } else {
        _ErrorHandle(ERROR_NONE);
        return    (ERROR_NONE);
    }
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
