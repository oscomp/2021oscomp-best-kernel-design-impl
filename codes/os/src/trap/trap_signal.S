.altmacro
    .section .text.signaltrampoline
    .globl __signal_trampoline
    .align 2
__signal_trampoline:
    # ecall sys_sigreturn
    addi	a7, zero, 139
    ecall
