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
# 文   件   名: Makefile
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2021 年 03 月 25 日
#
# 描        述: 本文件由 RealEvo-IDE 生成，用于配置 Makefile 功能，请勿手动修改
#*********************************************************************************************************

#*********************************************************************************************************
# Include config.mk
#*********************************************************************************************************
CONFIG_MK_EXIST = $(shell if [ -f ../config.mk ]; then echo exist; else echo notexist; fi;)
ifeq ($(CONFIG_MK_EXIST), exist)
include ../config.mk
else
CONFIG_MK_EXIST = $(shell if [ -f config.mk ]; then echo exist; else echo notexist; fi;)
ifeq ($(CONFIG_MK_EXIST), exist)
include config.mk
else
CONFIG_MK_EXIST =
endif
endif

#*********************************************************************************************************
# Include sylixos base config.mk
#*********************************************************************************************************
EMPTY =
SPACE = $(EMPTY) $(EMPTY)

SYLIXOS_BASE_PATH_BAK := $(SYLIXOS_BASE_PATH)
TOOLCHAIN_PREFIX_BAK  := $(TOOLCHAIN_PREFIX)
DEBUG_LEVEL_BAK       := $(DEBUG_LEVEL)
CPU_TYPE_BAK          := $(CPU_TYPE)
FPU_TYPE_BAK          := $(FPU_TYPE)

SYLIXOS_BASE_CONFIGMK = $(subst $(SPACE),\ ,$(SYLIXOS_BASE_PATH))/config.mk
include $(SYLIXOS_BASE_CONFIGMK)

SYLIXOS_BASE_PATH := $(SYLIXOS_BASE_PATH_BAK)
DEBUG_LEVEL       := $(DEBUG_LEVEL_BAK)

ifneq ($(TOOLCHAIN_PREFIX_BAK),)
TOOLCHAIN_PREFIX := $(TOOLCHAIN_PREFIX_BAK)
endif

ifneq ($(CPU_TYPE_BAK),)
CPU_TYPE := $(CPU_TYPE_BAK)
endif

ifneq ($(FPU_TYPE_BAK),)
FPU_TYPE := $(FPU_TYPE_BAK)
endif

#*********************************************************************************************************
# Include header.mk
#*********************************************************************************************************
MKTEMP = $(subst $(SPACE),\ ,$(SYLIXOS_BASE_PATH))/libsylixos/SylixOS/mktemp

include $(MKTEMP)/header.mk

#*********************************************************************************************************
# Include targets makefiles
#*********************************************************************************************************
include SylixBSP.mk

#*********************************************************************************************************
# Include end.mk
#*********************************************************************************************************
include $(END_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************
