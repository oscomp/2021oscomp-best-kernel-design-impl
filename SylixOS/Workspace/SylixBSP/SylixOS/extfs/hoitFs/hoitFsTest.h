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
** 文   件   名: hoitFsTest.h
**
** 创   建   人: 张楠
**
** 文件创建日期: 2021 年 05 月 15 日
**
** 描        述: 测试函数，用于测试hoitfs文件系统
*********************************************************************************************************/
#include "hoitType.h"

INT     hoitTestFileTree (INT  iArgC, PCHAR  ppcArgV[]);
INT     hoitTestFileOverWrite (INT  iArgC, PCHAR  ppcArgV[]);
INT     hoitTestLink (INT  iArgC, PCHAR  ppcArgV[]);
INT     hoitTestGC(PHOIT_VOLUME pfs);
