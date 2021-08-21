#include <asm.h>
#include <csr.h>

ENTRY(set_signal_trampoline)
    .local copy_kernel_stack
    .local copy_kernel_stack_finish

    /* arg0 is pointer to new_pt_regs, arg1 is previous signal mask */
    /* 1. create signal context */
    ld a3, PCB_KERNEL_SP(tp) /* a3 == pt_regs */
    sub a3, a3, sp /* a3 == stack size */
    mv a4, sp /* a4 = enter sp */

    addi sp, sp, -(SIGNAL_CONTEXT_SIZE)
    sd ra, SIGNAL_CONTEXT_RA(sp)
    # sd sp, SIGNAL_CONTEXT_SP(sp)
    sd s0, SIGNAL_CONTEXT_S0(sp)
    sd s1, SIGNAL_CONTEXT_S1(sp)
    sd s2, SIGNAL_CONTEXT_S2(sp)
    sd s3, SIGNAL_CONTEXT_S3(sp)
    sd s4, SIGNAL_CONTEXT_S4(sp)
    sd s5, SIGNAL_CONTEXT_S5(sp)
    sd s6, SIGNAL_CONTEXT_S6(sp)
    sd s7, SIGNAL_CONTEXT_S7(sp)
    sd s8, SIGNAL_CONTEXT_S8(sp)
    sd s9, SIGNAL_CONTEXT_S9(sp)
    sd s10, SIGNAL_CONTEXT_S10(sp)
    sd s11, SIGNAL_CONTEXT_S11(sp)
    ld a5, PCB_KERNEL_SP(tp)
    sd a5, SIGNAL_CONTEXT_KERNEL_SP(sp)
    ld a5, PCB_USER_SP(tp)
    sd a5, SIGNAL_CONTEXT_USER_SP(sp)
    sd a1, SIGNAL_CONTEXT_MASK(sp)

    /* 2. copy new_pt_regs */
    addi sp, sp, -(OFFSET_SIZE)
    /* use a2 */
    ld a2, OFFSET_REG_ZERO(a0)
    sd a2, OFFSET_REG_ZERO(sp)

    ld a2, OFFSET_REG_RA(a0)
    sd a2, OFFSET_REG_RA(sp)

    ld a2, OFFSET_REG_SP(a0)
    sd a2, OFFSET_REG_SP(sp)

    ld a2, OFFSET_REG_GP(a0)
    sd a2, OFFSET_REG_GP(sp)

    ld a2, OFFSET_REG_TP(a0)
    sd a2, OFFSET_REG_TP(sp)

    ld a2, OFFSET_REG_T0(a0)
    sd a2, OFFSET_REG_T0(sp)

    ld a2, OFFSET_REG_T1(a0)
    sd a2, OFFSET_REG_T1(sp)

    ld a2, OFFSET_REG_T2(a0)
    sd a2, OFFSET_REG_T2(sp)

    ld a2, OFFSET_REG_S0(a0)
    sd a2, OFFSET_REG_S0(sp)

    ld a2, OFFSET_REG_S1(a0)
    sd a2, OFFSET_REG_S1(sp)
    
    ld a2, OFFSET_REG_A0(a0)
    sd a2, OFFSET_REG_A0(sp)

    ld a2, OFFSET_REG_A1(a0)
    sd a2, OFFSET_REG_A1(sp)

    ld a2, OFFSET_REG_A2(a0)
    sd a2, OFFSET_REG_A2(sp)

    ld a2, OFFSET_REG_A3(a0)
    sd a2, OFFSET_REG_A3(sp)

    ld a2, OFFSET_REG_A4(a0)
    sd a2, OFFSET_REG_A4(sp)

    ld a2, OFFSET_REG_A5(a0)
    sd a2, OFFSET_REG_A5(sp)

    ld a2, OFFSET_REG_A6(a0)
    sd a2, OFFSET_REG_A6(sp)

    ld a2, OFFSET_REG_A7(a0)
    sd a2, OFFSET_REG_A7(sp)

    ld a2, OFFSET_REG_S2(a0)
    sd a2, OFFSET_REG_S2(sp)

    ld a2, OFFSET_REG_S3(a0)
    sd a2, OFFSET_REG_S3(sp)

    ld a2, OFFSET_REG_S4(a0)
    sd a2, OFFSET_REG_S4(sp)

    ld a2, OFFSET_REG_S5(a0)
    sd a2, OFFSET_REG_S5(sp)

    ld a2, OFFSET_REG_S6(a0)
    sd a2, OFFSET_REG_S6(sp)

    ld a2, OFFSET_REG_S7(a0)
    sd a2, OFFSET_REG_S7(sp)

    ld a2, OFFSET_REG_S8(a0)
    sd a2, OFFSET_REG_S8(sp)

    ld a2, OFFSET_REG_S9(a0)
    sd a2, OFFSET_REG_S9(sp)

    ld a2, OFFSET_REG_S10(a0)
    sd a2, OFFSET_REG_S10(sp)

    ld a2, OFFSET_REG_S11(a0)
    sd a2, OFFSET_REG_S11(sp)

    ld a2, OFFSET_REG_T3(a0)
    sd a2, OFFSET_REG_T3(sp)

    ld a2, OFFSET_REG_T4(a0)
    sd a2, OFFSET_REG_T4(sp)

    ld a2, OFFSET_REG_T5(a0)
    sd a2, OFFSET_REG_T5(sp)

    ld a2, OFFSET_REG_T6(a0)
    sd a2, OFFSET_REG_T6(sp)

    ld a2, OFFSET_REG_SSTATUS(a0)
    sd a2, OFFSET_REG_SSTATUS(sp)

    ld a2, OFFSET_REG_SEPC(a0)
    sd a2, OFFSET_REG_SEPC(sp)

    ld a2, OFFSET_REG_SBADADDR(a0)
    sd a2, OFFSET_REG_SBADADDR(sp)

    ld a2, OFFSET_REG_SCAUSE(a0)
    sd a2, OFFSET_REG_SCAUSE(sp)

    sd sp, PCB_KERNEL_SP(tp)

    /* 3. copy kernel stack */
    sub sp, sp, a3
    mv a5, sp
    beqz a3, copy_kernel_stack_finish
copy_kernel_stack:    
    ld a2, 0(a4)
    sd a2, 0(a5)
    addi a4, a4, 8
    addi a5, a5, 8
    addi a3, a3, -8
    bnez a3, copy_kernel_stack
copy_kernel_stack_finish:
    ret

ENDPROC(set_signal_trampoline)

ENTRY(exit_signal_trampoline)

    ld sp, PCB_KERNEL_SP(tp) /* sp == trampoline user context */
    addi sp, sp, OFFSET_SIZE    /* sp = signal context */

    ld ra, SIGNAL_CONTEXT_RA(sp)
    # sd sp, SIGNAL_CONTEXT_SP(sp)
    ld s0, SIGNAL_CONTEXT_S0(sp)
    ld s1, SIGNAL_CONTEXT_S1(sp)
    ld s2, SIGNAL_CONTEXT_S2(sp)
    ld s3, SIGNAL_CONTEXT_S3(sp)
    ld s4, SIGNAL_CONTEXT_S4(sp)
    ld s5, SIGNAL_CONTEXT_S5(sp)
    ld s6, SIGNAL_CONTEXT_S6(sp)
    ld s7, SIGNAL_CONTEXT_S7(sp)
    ld s8, SIGNAL_CONTEXT_S8(sp)
    ld s9, SIGNAL_CONTEXT_S9(sp)
    ld s10, SIGNAL_CONTEXT_S10(sp)
    ld s11, SIGNAL_CONTEXT_S11(sp)

    ld a5, SIGNAL_CONTEXT_KERNEL_SP(sp)
    sd a5, PCB_KERNEL_SP(tp)
    ld a5, SIGNAL_CONTEXT_USER_SP(sp)
    sd a5, PCB_USER_SP(tp)

    addi sp, sp, SIGNAL_CONTEXT_SIZE
    ret

ENDPROC(exit_signal_trampoline)