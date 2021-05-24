;/*********************************************************************************************************
;**
;**                                    中国软件开源组织
;**
;**                                   嵌入式实时操作系统
;**
;**                                SylixOS(TM)  LW : long wing
;**
;**                               Copyright All Rights Reserved
;**
;**--------------文件信息--------------------------------------------------------------------------------
;**
;** 文   件   名: startup.S
;**
;** 创   建   人: Han.Hui (韩辉)
;**
;** 文件创建日期: 2007 年 12 月 09 日
;**
;** 描        述: S3C2440 系统启动.
;*********************************************************************************************************/
    
#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include "config.h"
#include <arch/assembler.h>

    FILE_BEGIN()

;/*********************************************************************************************************
;  配置各处理器模式堆栈空间的大小
;*********************************************************************************************************/

#define SVC_STACK_SIZE  0x00002000
#define SYS_STACK_SIZE  0x00001000
#define FIQ_STACK_SIZE  0x00001000
#define UND_STACK_SIZE  0x00001000
#define ABT_STACK_SIZE  0x00001000
#define IRQ_STACK_SIZE  0x00001000

#define CPU_STACKS_SIZE     (SVC_STACK_SIZE + \
                             SYS_STACK_SIZE + \
                             FIQ_STACK_SIZE + \
                             IRQ_STACK_SIZE + \
                             UND_STACK_SIZE + \
                             ABT_STACK_SIZE)

;/*********************************************************************************************************
;  声明外部函数和导出全局函数
;*********************************************************************************************************/

    IMPORT_LABEL(archIntEntry)
    IMPORT_LABEL(archAbtEntry)
    IMPORT_LABEL(archPreEntry)
    IMPORT_LABEL(archUndEntry)
    IMPORT_LABEL(archSwiEntry)
    IMPORT_LABEL(sdramInit)
    IMPORT_LABEL(targetInit)
    IMPORT_LABEL(bspInit)

;/*********************************************************************************************************
;  异常向量表
;*********************************************************************************************************/

    SECTION(.vector)

FUNC_DEF(vector)
	    LDR     PC, resetEntry
    LDR     PC, undefineEntry
    LDR     PC, swiEntry
    LDR     PC, prefetchEntry
    LDR     PC, abortEntry
    LDR     PC, reserveEntry
    LDR     PC, irqEntry
    LDR     PC, fiqEntry
    FUNC_END()

FUNC_LABEL(resetEntry)
    .word   reset

FUNC_LABEL(undefineEntry)
    .word   archUndEntry

FUNC_LABEL(swiEntry)
    .word   archSwiEntry

FUNC_LABEL(prefetchEntry)
    .word   archPreEntry

FUNC_LABEL(abortEntry)
    .word   archAbtEntry

FUNC_LABEL(reserveEntry)
    .word   0

FUNC_LABEL(irqEntry)
    .word   archIntEntry

FUNC_LABEL(fiqEntry)
    .word   0

;/*********************************************************************************************************
;  复位入口
;*********************************************************************************************************/

#define WTCON   0x53000000

    SECTION(.text)

FUNC_DEF(reset)
    LDR     R0 , =WTCON                                                ;/*  watch dog disable            */
    LDR     R1 , =0x0                                                  ;/*  复位时，看门狗是打开的       */
    STR     R1 , [R0]

;/*********************************************************************************************************
;  初始化堆栈
;*********************************************************************************************************/

    LDR     R0 , =__stack_end                                          ;/*  栈区顶端地址                 */

    MSR     CPSR_c, #(SVC32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #SVC_STACK_SIZE
    
    MSR     CPSR_c, #(SYS32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #SYS_STACK_SIZE
    
    MSR     CPSR_c, #(FIQ32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #FIQ_STACK_SIZE
    
    MSR     CPSR_c, #(IRQ32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #IRQ_STACK_SIZE
    
    MSR     CPSR_c, #(UND32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #UND_STACK_SIZE

    MSR     CPSR_c, #(ABT32_MODE | DIS_INT)
    MOV     SP , R0
    SUB     R0 , R0, #ABT_STACK_SIZE

    MSR     CPSR_c, #(SVC32_MODE | DIS_INT)

;/*********************************************************************************************************
;  初始化最核心硬件接口 (有 BOOTLOADER 就不需要初始化 SDRAM 与 PLL)
;*********************************************************************************************************/

    BL      sdramInit
    BL      targetInit

;/*********************************************************************************************************
;  初始化 DATA 段
;*********************************************************************************************************/

    LDR     R1 , =_etext                                                ;/*  -> ROM data end             */
    LDR     R2 , =_data                                                 ;/*  -> data start               */
    LDR     R3 , =_edata                                                ;/*  -> end of data              */
LINE_LABEL(1)
    CMP     R2 , R3                                                     ;/*  check if data to move       */
    LDRLO   R0 , [R1] , #4                                              ;/*  copy it                     */
    STRLO   R0 , [R2] , #4
    BLO     1b                                                          ;/*  loop until done             */

;/*********************************************************************************************************
;  清零 BSS 段
;*********************************************************************************************************/

    MOV     R0 , #0                                                     ;/*  get a zero                  */
    LDR     R1 , =__bss_start                                           ;/*  -> bss start                */
    LDR     R2 , =__bss_end                                             ;/*  -> bss end                  */
LINE_LABEL(2)
    CMP     R1 , R2                                                     ;/*  check if data to clear      */
    STRLO   R0 , [R1],   #4                                             ;/*  clear 4 bytes               */
    BLO     2b                                                          ;/*  loop until done             */

;/*********************************************************************************************************
;  进入 bspInit 函数 (argc = 0, argv = NULL, frame pointer = NULL)
;*********************************************************************************************************/

    MOV     R0 , #0
    MOV     R1 , #0
    MOV     R2 , #0
    MOV     FP , #0

    LDR     R10, =bspInit
    MOV     LR , PC
    BX      R10

    B       .
    FUNC_END()

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
