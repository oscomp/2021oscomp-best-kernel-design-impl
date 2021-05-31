
#include <asm.h>
#include <csr.h>
#include <asm/sbidef.h>
#include <asm/sbiasm.h>

.section ".entry_function","ax"
ENTRY(_start)
  /* Load the global pointer */
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  fence
  fence.i

  /* Mask all interrupts */
  csrw CSR_SIE, zero
  csrw CSR_SIP, zero

  /*
   * Disable FPU to detect illegal usage of
   * floating point in kernel space
   */
  li t0, SR_FS
  csrc sstatus, t0

  /* setup C environment */
  la a3, __bss_start
  la a4, __BSS_END__
  ble a4, a3, clear_bss_done
clear_bss:
  sw zero, (a3)
  add a3, a3, 4
  blt a3, a4, clear_bss
clear_bss_done:  /* this should never return */
  la tp, pid0_pcb
  ld sp, pid0_stack
  csrw CSR_SSCRATCH, x0
  j main

secondary:
  la tp, pid0_pcb2
  ld sp, pid0_stack2
  csrw CSR_SSCRATCH, x0
  j main

  
loop:
  wfi
  j loop

END(_start)
