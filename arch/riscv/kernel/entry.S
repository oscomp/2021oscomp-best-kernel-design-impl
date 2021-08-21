#include <asm.h>
#include <csr.h>

.macro SAVE_CONTEXT
  .local _restore_kernel_tpsp
  .local _save_context
  .local _from_kernel
# csrrw tp, CSR_SSCRATCH, tp
# bnez tp, _save_context
# _restore_kernel_tpsp:
#   csrr tp, CSR_SSCRATCH
#   sd sp, PCB_KERNEL_SP(tp)
_save_context:
  /* HappyEric uses tp as current_running */
  /*
   * If coming from userspace, tp is greater or equal to 0
   * If coming from kernel, tp is less than 0xffffffff00000000
   */
  bltz tp, _from_kernel
  csrw CSR_SSCRATCH, tp
  ld tp, current_running
  sd sp, PCB_USER_SP(tp) /* user sp is real */
  ld sp, PCB_KERNEL_SP(tp)
  csrr tp, CSR_SSCRATCH
_from_kernel:
  addi sp, sp, -(OFFSET_SIZE)

  /* TODO: save all general purpose registers here! */
  sd zero, OFFSET_REG_ZERO(sp)
  sd ra, OFFSET_REG_RA(sp)
  sd gp, OFFSET_REG_GP(sp)

  /* push tp */
  sd tp, OFFSET_REG_TP(sp)
  
  ld tp, current_running
  sd sp, PCB_KERNEL_SP(tp) /* very important */

  sd t0, OFFSET_REG_T0(sp)
  sd t1, OFFSET_REG_T1(sp)
  sd t2, OFFSET_REG_T2(sp)

  sd s0, OFFSET_REG_S0(sp)
  sd s1, OFFSET_REG_S1(sp)

  sd a0, OFFSET_REG_A0(sp)
  sd a1, OFFSET_REG_A1(sp)
  sd a2, OFFSET_REG_A2(sp)
  sd a3, OFFSET_REG_A3(sp)
  sd a4, OFFSET_REG_A4(sp)
  sd a5, OFFSET_REG_A5(sp)
  sd a6, OFFSET_REG_A6(sp)
  sd a7, OFFSET_REG_A7(sp)
  
  sd s2, OFFSET_REG_S2(sp)
  sd s3, OFFSET_REG_S3(sp)
  sd s4, OFFSET_REG_S4(sp)
  sd s5, OFFSET_REG_S5(sp)
  sd s6, OFFSET_REG_S6(sp)
  sd s7, OFFSET_REG_S7(sp)
  sd s8, OFFSET_REG_S8(sp)
  sd s9, OFFSET_REG_S9(sp)
  sd s10, OFFSET_REG_S10(sp)
  sd s11, OFFSET_REG_S11(sp)
  
  sd t3, OFFSET_REG_T3(sp)
  sd t4, OFFSET_REG_T4(sp)
  sd t5, OFFSET_REG_T5(sp)
  sd t6, OFFSET_REG_T6(sp)
  
  /*
   * Disable user-mode memory access as it should only be set in the
   * actual user copy routines.
   *
   * Disable the FPU to detect illegal usage of floating point in kernel
   * space.
   */
  li t0, SR_SUM | SR_FS

  /* TODO: save sstatus, sepc, stval, scause and sscratch on kernel stack */
  csrr s0, CSR_SSTATUS
  sd s0, OFFSET_REG_SSTATUS(sp)
  csrr s0, CSR_SEPC
  sd s0, OFFSET_REG_SEPC(sp)
  csrr s0, CSR_STVAL
  sd s0, OFFSET_REG_SBADADDR(sp)
  csrr s0, CSR_SCAUSE
  sd s0, OFFSET_REG_SCAUSE(sp)

  fsd ft0, OFFSET_FREG_FT0(sp)
  fsd ft1, OFFSET_FREG_FT1(sp)
  fsd ft2, OFFSET_FREG_FT2(sp)
  fsd ft3, OFFSET_FREG_FT3(sp)
  fsd ft4, OFFSET_FREG_FT4(sp)
  fsd ft5, OFFSET_FREG_FT5(sp)
  fsd ft6, OFFSET_FREG_FT6(sp)
  fsd ft7, OFFSET_FREG_FT7(sp)

  fsd fs0, OFFSET_FREG_FS0(sp)
  fsd fs1, OFFSET_FREG_FS1(sp)

  fsd fa0, OFFSET_FREG_FA0(sp)
  fsd fa1, OFFSET_FREG_FA1(sp)
  fsd fa2, OFFSET_FREG_FA2(sp)
  fsd fa3, OFFSET_FREG_FA3(sp)
  fsd fa4, OFFSET_FREG_FA4(sp)
  fsd fa5, OFFSET_FREG_FA5(sp)
  fsd fa6, OFFSET_FREG_FA6(sp)
  fsd fa7, OFFSET_FREG_FA7(sp)

  fsd fs2, OFFSET_FREG_FS2(sp)
  fsd fs3, OFFSET_FREG_FS3(sp)
  fsd fs4, OFFSET_FREG_FS4(sp)
  fsd fs5, OFFSET_FREG_FS5(sp)
  fsd fs6, OFFSET_FREG_FS6(sp)
  fsd fs7, OFFSET_FREG_FS7(sp)
  fsd fs8, OFFSET_FREG_FS8(sp)
  fsd fs9, OFFSET_FREG_FS9(sp)
  fsd fs10, OFFSET_FREG_FS10(sp)
  fsd fs11, OFFSET_FREG_FS11(sp)

  fsd f28, OFFSET_FREG_FT8(sp)
  fsd f29, OFFSET_FREG_FT9(sp)
  fsd f30, OFFSET_FREG_FT10(sp)
  fsd f31, OFFSET_FREG_FT11(sp)

.endm

.macro RESTORE_CONTEXT
  .local _back_to_kernel
  /* TODO: Restore all registers and sepc,sstatus */
  ld zero, OFFSET_REG_ZERO(sp)
  ld ra, OFFSET_REG_RA(sp)
    
  ld gp, OFFSET_REG_GP(sp)
  ld tp, OFFSET_REG_TP(sp)

  ld t0, OFFSET_REG_T0(sp)
  ld t1, OFFSET_REG_T1(sp)
  ld t2, OFFSET_REG_T2(sp)

  ld s1, OFFSET_REG_S1(sp)

  ld a0, OFFSET_REG_A0(sp)
  ld a1, OFFSET_REG_A1(sp)
  ld a2, OFFSET_REG_A2(sp)
  ld a3, OFFSET_REG_A3(sp)
  ld a4, OFFSET_REG_A4(sp)
  ld a5, OFFSET_REG_A5(sp)
  ld a6, OFFSET_REG_A6(sp)
  ld a7, OFFSET_REG_A7(sp)
  
  ld s2, OFFSET_REG_S2(sp)
  ld s3, OFFSET_REG_S3(sp)
  ld s4, OFFSET_REG_S4(sp)
  ld s5, OFFSET_REG_S5(sp)
  ld s6, OFFSET_REG_S6(sp)
  ld s7, OFFSET_REG_S7(sp)
  ld s8, OFFSET_REG_S8(sp)
  ld s9, OFFSET_REG_S9(sp)
  ld s10, OFFSET_REG_S10(sp)
  ld s11, OFFSET_REG_S11(sp)
  
  ld t3, OFFSET_REG_T3(sp)
  ld t4, OFFSET_REG_T4(sp)
  ld t5, OFFSET_REG_T5(sp)
  ld t6, OFFSET_REG_T6(sp)

  fld ft0, OFFSET_FREG_FT0(sp)
  fld ft1, OFFSET_FREG_FT1(sp)
  fld ft2, OFFSET_FREG_FT2(sp)
  fld ft3, OFFSET_FREG_FT3(sp)
  fld ft4, OFFSET_FREG_FT4(sp)
  fld ft5, OFFSET_FREG_FT5(sp)
  fld ft6, OFFSET_FREG_FT6(sp)
  fld ft7, OFFSET_FREG_FT7(sp)

  fld fs0, OFFSET_FREG_FS0(sp)
  fld fs1, OFFSET_FREG_FS1(sp)

  fld fa0, OFFSET_FREG_FA0(sp)
  fld fa1, OFFSET_FREG_FA1(sp)
  fld fa2, OFFSET_FREG_FA2(sp)
  fld fa3, OFFSET_FREG_FA3(sp)
  fld fa4, OFFSET_FREG_FA4(sp)
  fld fa5, OFFSET_FREG_FA5(sp)
  fld fa6, OFFSET_FREG_FA6(sp)
  fld fa7, OFFSET_FREG_FA7(sp)

  fld fs2, OFFSET_FREG_FS2(sp)
  fld fs3, OFFSET_FREG_FS3(sp)
  fld fs4, OFFSET_FREG_FS4(sp)
  fld fs5, OFFSET_FREG_FS5(sp)
  fld fs6, OFFSET_FREG_FS6(sp)
  fld fs7, OFFSET_FREG_FS7(sp)
  fld fs8, OFFSET_FREG_FS8(sp)
  fld fs9, OFFSET_FREG_FS9(sp)
  fld fs10, OFFSET_FREG_FS10(sp)
  fld fs11, OFFSET_FREG_FS11(sp)

  fld f28, OFFSET_FREG_FT8(sp)
  fld f29, OFFSET_FREG_FT9(sp)
  fld f30, OFFSET_FREG_FT10(sp)
  fld f31, OFFSET_FREG_FT11(sp)

  ld s0, OFFSET_REG_SSTATUS(sp)
  csrw CSR_SSTATUS, s0
  ld s0, OFFSET_REG_SEPC(sp)
  csrw CSR_SEPC, s0

  ld s0, OFFSET_REG_S0(sp)

  addi sp, sp, OFFSET_SIZE
  bltz tp, _back_to_kernel
  csrw CSR_SSCRATCH, tp
  ld tp, current_running
  sd sp, PCB_KERNEL_SP(tp)
  ld sp, PCB_USER_SP(tp)
  csrr tp, CSR_SSCRATCH
_back_to_kernel:
.endm

ENTRY(enable_preempt)
  ld t1, current_running
  ld t0, PCB_PREEMPT_COUNT(t1)
  beq t0, zero, do_enable
  addi t0, t0, -1
  sd t0, PCB_PREEMPT_COUNT(t1)
  beq t0, zero, do_enable
  jr ra
do_enable:
  not t0, x0
  csrs CSR_SIE, t0
  jr ra
ENDPROC(enable_preempt)

ENTRY(disable_preempt)
  csrw CSR_SIE, zero
  ld t1, current_running
  ld t0, PCB_PREEMPT_COUNT(t1)
  addi t0, t0, 1
  sd t0, PCB_PREEMPT_COUNT(t1)
  jr ra
ENDPROC(disable_preempt)

ENTRY(enable_interrupt)
  li t0, SR_SIE
  csrs CSR_SSTATUS, t0
  jr ra
ENDPROC(enable_interrupt)

ENTRY(disable_interrupt)
  li t0, SR_SIE
  csrc CSR_SSTATUS, t0
  jr ra
ENDPROC(disable_interrupt)

ENTRY(enable_float_point_inst)
  li t0, SR_FS
  csrs CSR_SSTATUS, t0
  jr ra
ENDPROC(enable_float_point_inst)

// the address of previous pcb in a0
// the address of next pcb in a1
ENTRY(switch_to)
  beqz a0, switch_to_restore /* if previous_running is NULL, just switch to current_running */
  beq a0, a1, switch_to_ret /* if a0 is equal to a1, no need to switch */
  addi tp, a0, 0
  // save all callee save registers on kernel stack
  addi sp, sp, -(SWITCH_TO_SIZE)
  sd ra, SWITCH_TO_RA(sp)
    
  sd s0, SWITCH_TO_S0(sp)
  sd s1, SWITCH_TO_S1(sp)
  
  sd s2, SWITCH_TO_S2(sp)
  sd s3, SWITCH_TO_S3(sp)
  sd s4, SWITCH_TO_S4(sp)
  sd s5, SWITCH_TO_S5(sp)
  sd s6, SWITCH_TO_S6(sp)
  sd s7, SWITCH_TO_S7(sp)
  sd s8, SWITCH_TO_S8(sp)
  sd s9, SWITCH_TO_S9(sp)
  sd s10, SWITCH_TO_S10(sp)
  sd s11, SWITCH_TO_S11(sp)
  csrr s11, CSR_SATP
  sd s11, SWITCH_TO_SATP(sp)

  sd sp, PCB_KERNEL_SP(tp)

switch_to_restore:
  // restore next
  addi tp, a1, 0
  ld sp, PCB_KERNEL_SP(tp)  

  ld s0, SWITCH_TO_SATP(sp)
  csrw CSR_SATP, s0
  fence
  fence.i
  sfence.vma
  
  ld s0, SWITCH_TO_S0(sp)
  ld s1, SWITCH_TO_S1(sp)
  
  ld s2, SWITCH_TO_S2(sp)
  ld s3, SWITCH_TO_S3(sp)
  ld s4, SWITCH_TO_S4(sp)
  ld s5, SWITCH_TO_S5(sp)
  ld s6, SWITCH_TO_S6(sp)
  ld s7, SWITCH_TO_S7(sp)
  ld s8, SWITCH_TO_S8(sp)
  ld s9, SWITCH_TO_S9(sp)
  ld s10, SWITCH_TO_S10(sp)
  ld s11, SWITCH_TO_S11(sp)  

  ld ra, SWITCH_TO_RA(sp)
  addi sp, sp, SWITCH_TO_SIZE
switch_to_ret:
  jr ra
ENDPROC(switch_to)

ENTRY(ret_from_exception)
  sd sp, PCB_KERNEL_SP(tp)
  call handle_signal
  ld sp, PCB_KERNEL_SP(tp)
  RESTORE_CONTEXT
  sret
ENDPROC(ret_from_exception)

ENTRY(exception_handler_entry)
  .align 4
  
  SAVE_CONTEXT

  /* Load the global pointer */
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  csrw CSR_SSCRATCH, x0

  /* TODO: load ret_from_exception into $ra
   * so that we can return to ret_from_exception
   * when interrupt_help complete.
   */
  
  la ra, ret_from_exception

  /* TODO: call interrupt_helper
   * note: dont forget to pass parameters for it.
   */
  ld a0, PCB_KERNEL_SP(tp)
  csrr a1, CSR_STVAL
  csrr a2, CSR_SCAUSE

  j interrupt_helper

ENDPROC(exception_handler_entry)

ENTRY(do_sret)
  csrw CSR_SEPC, ra
  sret
ENDPROC(do_sret)