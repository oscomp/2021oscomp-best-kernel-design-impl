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
** 文   件   名: symbol.h								
**														
** 创   建   人: makesymbol 工具						
**														
** 文件创建日期: 2021 年 05 月 18 日		
**														
** 描        述: 系统 sylixos 符号表. (此文件由 makesymbol 工具自动生成, 请勿修改)	
*********************************************************************************************************/	
														
#ifndef __SYMBOL_H										
#define __SYMBOL_H										
														
#include "SylixOS.h"									
#include "symboltools.h"								
														
#ifdef SYLIXOS_EXPORT_KSYMBOL							
#define SYM_TABLE_SIZE 7251							
extern  LW_STATIC_SYMBOL  _G_symLibSylixOS[SYM_TABLE_SIZE];					
															
static LW_INLINE  INT symbolAddAll (VOID)				
{														
    return  (symbolAddStatic((LW_SYMBOL *)_G_symLibSylixOS, SYM_TABLE_SIZE));	
}														
#else													
static LW_INLINE  INT symbolAddAll (VOID)				
{														
    return  (ERROR_NONE);								
}														
#endif													
														
#endif                                                                  /*  __SYMBOL_H                  */	
/*********************************************************************************************************	
  END													
*********************************************************************************************************/	
