/*
 * Copyright (C) 2015 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 */

#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H

#include <asm/sbidef.h>
#include <type.h>

#define SBI_CALL(which, arg0, arg1, arg2)                     \
    ({                                                        \
        register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);  \
        register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);  \
        register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);  \
        register uintptr_t a7 asm("a7") = (uintptr_t)(which); \
        asm volatile("ecall"                                  \
                     : "+r"(a0)                               \
                     : "r"(a1), "r"(a2), "r"(a7)              \
                     : "memory");                             \
        a0;                                                   \
    })

/* Lazy implementations until SBI is finalized */
#define SBI_CALL_0(which) SBI_CALL(which, 0, 0, 0)
#define SBI_CALL_1(which, arg0) SBI_CALL(which, arg0, 0, 0)
#define SBI_CALL_2(which, arg0, arg1) \
    SBI_CALL(which, arg0, arg1, 0)
#define SBI_CALL_3(which, arg0, arg1, arg2) \
    SBI_CALL(which, arg0, arg1, arg2)

static inline void sbi_console_putstr(char *str)
{
    SBI_CALL_1(SBI_CONSOLE_PUTSTR, str);
}

static inline uintptr_t sbi_sd_write(
    unsigned int mem_address, unsigned int num_of_blocks,
    unsigned int block_id)
{
    return SBI_CALL_3(
        SBI_SD_WRITE, mem_address, num_of_blocks, block_id);
}

static inline uintptr_t sbi_sd_read(
    unsigned int mem_address, unsigned int num_of_blocks,
    unsigned int block_id)
{
    return SBI_CALL_3(
        SBI_SD_READ, mem_address, num_of_blocks, block_id);
}

static inline void sbi_console_putchar(int ch)
{
    SBI_CALL_1(SBI_CONSOLE_PUTCHAR, ch);
}

static inline int sbi_console_getchar(void)
{
    return SBI_CALL_0(SBI_CONSOLE_GETCHAR);
}

static inline void sbi_set_timer(uint64_t stime_value)
{
#if __riscv_xlen == 32
    SBI_CALL_2(SBI_SET_TIMER, stime_value, stime_value >> 32);
#else
    SBI_CALL_1(SBI_SET_TIMER, stime_value);
#endif
}

static inline void sbi_shutdown(void) { SBI_CALL_0(SBI_SHUTDOWN); }

static inline void sbi_clear_ipi(void)
{
    SBI_CALL_0(SBI_CLEAR_IPI);
}

static inline void sbi_send_ipi(const unsigned long *hart_mask)
{
    SBI_CALL_1(SBI_SEND_IPI, hart_mask);
}

static inline void sbi_remote_fence_i(
    const unsigned long *hart_mask)
{
    SBI_CALL_1(SBI_REMOTE_FENCE_I, hart_mask);
}

static inline void sbi_remote_sfence_vma(
    const unsigned long *hart_mask, unsigned long start,
    unsigned long size)
{
    SBI_CALL_1(SBI_REMOTE_SFENCE_VMA, hart_mask);
}

static inline void sbi_remote_sfence_vma_asid(
    const unsigned long *hart_mask, unsigned long start,
    unsigned long size, unsigned long asid)
{
    SBI_CALL_1(SBI_REMOTE_SFENCE_VMA_ASID, hart_mask);
}

enum SBI_READY_FDT_TYPE {
    TIMEBASE,
    SLCR_BADE_ADDR,
    ETHERNET_ADDR,
    PLIC_ADDR,
    NR_IRQS
};
static inline uint32_t sbi_read_fdt(enum SBI_READY_FDT_TYPE type)
{
    return SBI_CALL_1(SBI_FDT_READ, (int)type);
}

#endif
