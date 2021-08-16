#*********************************************************************************************************
#
#                                    中国软件开源组织
#
#                                   嵌入式实时操作系统
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------文件信息--------------------------------------------------------------------------------
#
# 文   件   名: SylixBSP.mk
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2021 年 03 月 25 日
#
# 描        述: 本文件由 RealEvo-IDE 生成，用于配置 Makefile 功能，请勿手动修改
#*********************************************************************************************************

#*********************************************************************************************************
# Clear setting
#*********************************************************************************************************
include $(CLEAR_VARS_MK)

#*********************************************************************************************************
# Target
#*********************************************************************************************************
LOCAL_TARGET_NAME := SylixBSP.elf

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS :=  \
SylixOS/bsp/startup.S \
SylixOS/bsp/bspInit.c \
SylixOS/bsp/bspLib.c \
SylixOS/bsp/targetInit.c \
SylixOS/driver/dma/samsungdma.c \
SylixOS/driver/gpio/s3c2440_gpio.c \
SylixOS/driver/i2c/samsungi2c.c \
SylixOS/driver/lcd/s3c2440a_lcd.c \
SylixOS/driver/mtd/nand/k9f1g08.c \
SylixOS/driver/mtd/nand/nand.c \
SylixOS/driver/mtd/nand/s3c24xx_nand.c \
SylixOS/driver/mtd/nor/fake_nor.c \
SylixOS/driver/mtd/nor/nor.c \
SylixOS/driver/mtd/nor/nor_cmd.c \
SylixOS/driver/mtd/nor/nor_util.c \
SylixOS/driver/netif/dm9000x.c \
SylixOS/driver/pm/s3c2440a_pm.c \
SylixOS/driver/rtc/rtc.c \
SylixOS/driver/sdi/mciLib.c \
SylixOS/driver/sdi/s3csdi.c \
SylixOS/driver/sdi/sdInit.c \
SylixOS/driver/timer/timer.c \
SylixOS/driver/touchscr/s3c_onewire.c \
SylixOS/driver/touchscr/touchscr.c \
SylixOS/driver/tty/samsungtty.c \
SylixOS/driver/tty/uart.c \
SylixOS/extfs/hoitFs/hoitFs.c \
SylixOS/extfs/hoitFs/hoitFsCache.c \
SylixOS/extfs/hoitFs/hoitFsCmd.c \
SylixOS/extfs/hoitFs/hoitFsFDLib.c \
SylixOS/extfs/hoitFs/hoitFsGC.c \
SylixOS/extfs/hoitFs/hoitFsLib.c \
SylixOS/extfs/hoitFs/hoitFsLog.c \
SylixOS/extfs/hoitFs/hoitFsTest.c \
SylixOS/extfs/hoitFs/hoitFsTree.c \
SylixOS/extfs/hoitFs/hoitFsTreeUtil.c \
SylixOS/extfs/hoitFs/hoitMergeBuffer.c \
SylixOS/extfs/spifFs/spifFs.c \
SylixOS/extfs/spifFs/spifFsCache.c \
SylixOS/extfs/spifFs/spifFsCmd.c \
SylixOS/extfs/spifFs/spifFsFDLib.c \
SylixOS/extfs/spifFs/spifFsGC.c \
SylixOS/extfs/spifFs/spifFsGlue.c \
SylixOS/extfs/spifFs/spifFsLib.c \
SylixOS/extfs/spifFs/spifFsVerify.c \
SylixOS/extfs/tools/fstester/fstester.c \
SylixOS/extfs/tools/fstester/functionality.c \
SylixOS/extfs/tools/list/iter.c \
SylixOS/extfs/tools/list/list.c \
SylixOS/extfs/tools/list/list_test.c \
SylixOS/user/main.c


#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"./SylixOS" \
-I"./SylixOS/bsp"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := \
-D__BOOT_INRAM=1

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS   := 
LOCAL_CXXFLAGS := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := 
LOCAL_DEPEND_LIB_PATH := 

#*********************************************************************************************************
# Link script file
#*********************************************************************************************************
LOCAL_LD_SCRIPT := SylixOSBSP.ld

#*********************************************************************************************************
# C++ config
#*********************************************************************************************************
LOCAL_USE_CXX        := no
LOCAL_USE_CXX_EXCEPT := no

#*********************************************************************************************************
# Code coverage config
#*********************************************************************************************************
LOCAL_USE_GCOV := no

#*********************************************************************************************************
# OpenMP config
#*********************************************************************************************************
LOCAL_USE_OMP := no

#*********************************************************************************************************
# User link command
#*********************************************************************************************************
LOCAL_PRE_LINK_CMD   := 
LOCAL_POST_LINK_CMD  := 
LOCAL_PRE_STRIP_CMD  := 
LOCAL_POST_STRIP_CMD := 

include $(BSP_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************
