/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */

#include <asm.h>
#include <csr.h>

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

  bnez a0, secondary

  /* Clear BSS for flat non-ELF images */
  la a3, __bss_start
  la a4, __BSS_END__
  ble a4, a3, clear_bss_done
clear_bss:
  sw zero, (a3)
  add a3, a3, 4
  blt a3, a4, clear_bss
clear_bss_done:
  li sp, 0x80280000
  j boot_kernel
secondary:
  li sp, 0x80288000
  j boot_kernel

loop:
  wfi
  j loop

END(_start)

ENTRY(get_s0)
  mv a0, s0
  ret
END(get_s0)