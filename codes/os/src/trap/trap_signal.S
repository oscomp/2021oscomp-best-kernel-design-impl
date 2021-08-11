.altmacro
.macro SAVE_GP n
    sd x\n, \n*8(sp)
.endm
.macro LOAD_GP n
    ld x\n, \n*8(sp)
.endm
    .section .text.ssignaltrampoline
    .globl __signal_trapoline
    .align 2
__signal_trapoline:
    addi	a7, zero, 139 #sys_sigreturn
    ecall
