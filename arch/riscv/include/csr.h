/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 Regents of the University of California
 */

#ifndef CSR_H
#define CSR_H

/* Status register flags */
#define SR_SIE    0x00000002 /* Supervisor Interrupt Enable */
#define SR_SPIE   0x00000020 /* Previous Supervisor IE */
#define SR_SPP    0x00000100 /* Previously Supervisor */
#define SR_SUM    0x00040000 /* Supervisor User Memory Access */

#define SR_FS           0x00006000 /* Floating-point Status */
#define SR_FS_OFF       0x00000000
#define SR_FS_INITIAL   0x00002000
#define SR_FS_CLEAN     0x00004000
#define SR_FS_DIRTY     0x00006000

#define SR_XS           0x00018000 /* Extension Status */
#define SR_XS_OFF       0x00000000
#define SR_XS_INITIAL   0x00008000
#define SR_XS_CLEAN     0x00010000
#define SR_XS_DIRTY     0x00018000

#define SR_SD           0x8000000000000000 /* FS/XS dirty */

/* SATP flags */
#define SATP_PPN        0x00000FFFFFFFFFFF
#define SATP_MODE_39    0x8000000000000000
#define SATP_MODE       SATP_MODE_39

/* SCAUSE */
#define SCAUSE_IRQ_FLAG   (1 << 63)

#define IRQ_U_SOFT		0
#define IRQ_S_SOFT		1
#define IRQ_M_SOFT		3
#define IRQ_U_TIMER		4
#define IRQ_S_TIMER		5
#define IRQ_M_TIMER		7
#define IRQ_U_EXT		8
#define IRQ_S_EXT		9
#define IRQ_M_EXT		11

#define EXC_INST_MISALIGNED	0
#define EXC_INST_ACCESS		1
#define EXC_BREAKPOINT		3
#define EXC_LOAD_ACCESS		5
#define EXC_STORE_ACCESS	7
#define EXC_SYSCALL		8
#define EXC_INST_PAGE_FAULT	12
#define EXC_LOAD_PAGE_FAULT	13
#define EXC_STORE_PAGE_FAULT	15

/* SIE (Interrupt Enable) and SIP (Interrupt Pending) flags */
#define SIE_SSIE    (0x1 << IRQ_S_SOFT)
#define SIE_STIE    (0x1 << IRQ_S_TIMER)
#define SIE_SEIE    (0x1 << IRQ_S_EXT)

#define CSR_CYCLE   0xc00
#define CSR_TIME    0xc01
#define CSR_INSTRET   0xc02
#define CSR_SSTATUS   0x100
#define CSR_SIE     0x104
#define CSR_STVEC   0x105
#define CSR_SCOUNTEREN    0x106
#define CSR_SSCRATCH    0x140
#define CSR_SEPC    0x141
#define CSR_SCAUSE    0x142
#define CSR_STVAL   0x143
#define CSR_SIP     0x144
#define CSR_SATP    0x180
#define CSR_CYCLEH    0xc80
#define CSR_TIMEH   0xc81
#define CSR_INSTRETH    0xc82

#endif /* CSR_H */
