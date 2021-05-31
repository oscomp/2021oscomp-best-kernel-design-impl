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
** 文   件   名: hoitFsCmd.c
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 05 月 02 日
**
** 描        述: Hoit文件系统注册硬链接shell命令
*********************************************************************************************************/
#include "hoitFsCmd.h"
#include "hoitFs.h"
/*********************************************************************************************************
** 函数名称: hln_cmd_wrppaer
** 功能描述: 注册SylixOS ttinyShell命令函数
** 输　入  : iArgC         变量数
**           ppcArgV       变量内容
** 输　出  : 0
** 全局变量:
** 调用模块:
*********************************************************************************************************/
#include "./hoitFsTest.h"
PHOIT_VOLUME _G_Volumn;

#define DIVIDER                         "================="
#define GET_ARG(i)                      *(ppcArgV + i)
#define EQU_ARG(pcTargetArg, pcSrcArg)  lib_strcmp(pcTargetArg, pcSrcArg) == 0


#define NEXT_LINE                       "\n"

PHOIT_VOLUME _G_Volumn;

VOID __hoitShowSectorInfo(PHOIT_VOLUME pfs){
    PHOIT_ERASABLE_SECTOR       pErasableSectorTraverse;
    pErasableSectorTraverse = pfs->HOITFS_erasableSectorList;
    while (pErasableSectorTraverse)
    {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        if(hoitLogCheckIfLog(pfs, pErasableSectorTraverse)){
            printf(DIVIDER "SECTOR %d [*LOG] " DIVIDER NEXT_LINE, pErasableSectorTraverse->HOITS_bno);
        }
        else if(pErasableSectorTraverse == pfs->HOITFS_now_sector){
            printf(DIVIDER "SECTOR %d [*CUR] " DIVIDER NEXT_LINE, pErasableSectorTraverse->HOITS_bno);
        }
        else {
            printf(DIVIDER "SECTOR %d" DIVIDER NEXT_LINE, pErasableSectorTraverse->HOITS_bno);
        }
        printf("UsedSize: %d" NEXT_LINE, pErasableSectorTraverse->HOITS_uiUsedSize);
        printf("FreeSize: %d" NEXT_LINE, pErasableSectorTraverse->HOITS_uiFreeSize);
        pErasableSectorTraverse = pErasableSectorTraverse->HOITS_next;
        API_TShellColorEnd(STD_OUT);
    }
}


INT hln_cmd_wrapper(INT  iArgC, PCHAR  ppcArgV[]) {
    PCHAR pLinkDstName1 = GET_ARG(1);       /* 目标链接文件 */
    PCHAR pLinkSrcName2 = GET_ARG(2);
    
    __hoitFsHardlink(_G_Volumn, pLinkSrcName2, pLinkDstName1);
    return 0;
}

INT gc_cmd_wrapper(INT  iArgC, PCHAR  ppcArgV[]) {
    PCHAR pcGCOption;

    pcGCOption = GET_ARG(1);
    if(EQU_ARG("-c", pcGCOption)){
        hoitGCClose(_G_Volumn);
    }
    else if (EQU_ARG("-t", pcGCOption))
    {
        hoitTestGC(_G_Volumn);
        printf("\n");
    }
}


INT fs_cmd_wrapper(INT  iArgC, PCHAR  ppcArgV[]) {
    PCHAR       pcFSOption;

    pcFSOption = GET_ARG(1);
    if(EQU_ARG("-i", pcFSOption)){
        __hoitShowSectorInfo(_G_Volumn);
    }
    else if (EQU_ARG("-t", pcFSOption))
    {
        pcFSOption = GET_ARG(2);
        if(EQU_ARG("ftt", pcFSOption)){                                 /* hoit -t ftt 2 3 2 */
            hoitTestFileTree(iArgC - 2, ppcArgV + 2);
        }
        else if (EQU_ARG("fot", pcFSOption))                            /* hoit -t fot */
        {
            hoitTestFileOverWrite(iArgC - 2, ppcArgV + 2);
        }
        else if (EQU_ARG("ln", pcFSOption))
        {
            hoitTestLink(iArgC - 2, ppcArgV + 2);
        }
    }
}


VOID register_hoitfs_cmd(PHOIT_VOLUME pfs) {
    _G_Volumn = pfs;
    API_TShellKeywordAdd("hln", hln_cmd_wrapper);
    API_TShellKeywordAdd("gc", gc_cmd_wrapper);
    API_TShellKeywordAdd("hoit", fs_cmd_wrapper);
    // API_TShellKeywordAdd("ftt", hoitTestFileTree);
    // API_TShellKeywordAdd("fot", hoitTestFileOverWrite);
}
