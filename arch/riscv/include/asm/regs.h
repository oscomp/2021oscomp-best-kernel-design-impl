
#ifndef INCLUDE_REGS_H_
#define INCLUDE_REGS_H_

#define OFFSET_REG_ZERO         0

/* return address */
#define OFFSET_REG_RA           8

/* pointers */
#define OFFSET_REG_SP           16 // stack
#define OFFSET_REG_GP           24 // global
#define OFFSET_REG_TP           32 // thread

/* temporary */
#define OFFSET_REG_T0           40
#define OFFSET_REG_T1           48
#define OFFSET_REG_T2           56

/* saved register */
#define OFFSET_REG_S0           64
#define OFFSET_REG_S1           72

/* args */
#define OFFSET_REG_A0           80
#define OFFSET_REG_A1           88
#define OFFSET_REG_A2           96
#define OFFSET_REG_A3           104
#define OFFSET_REG_A4           112
#define OFFSET_REG_A5           120
#define OFFSET_REG_A6           128
#define OFFSET_REG_A7           136

/* saved register */
#define OFFSET_REG_S2           144
#define OFFSET_REG_S3           152
#define OFFSET_REG_S4           160
#define OFFSET_REG_S5           168
#define OFFSET_REG_S6           176
#define OFFSET_REG_S7           184
#define OFFSET_REG_S8           192
#define OFFSET_REG_S9           200
#define OFFSET_REG_S10          208
#define OFFSET_REG_S11          216

/* temporary register */
#define OFFSET_REG_T3           224
#define OFFSET_REG_T4           232
#define OFFSET_REG_T5           240
#define OFFSET_REG_T6           248

/* privileged register */
#define OFFSET_REG_SSTATUS      256
#define OFFSET_REG_SEPC         264
#define OFFSET_REG_SBADADDR     272
#define OFFSET_REG_SCAUSE       280
#define OFFSET_REG_SATP         288

/* Size of stack frame, word/double word alignment */
#define OFFSET_SIZE             296
#ifndef KERNEL_STACK_SIZE
    #define KERNEL_STACK_SIZE 4096
#endif
#ifndef KERNEL_STACK_ALIGN
    #define KERNEL_STACK_ALIGN 0xfffffffffffff000lu
#endif

#define PCB_KERNEL_SP          0
#define PCB_USER_SP            8
#define PCB_PREEMPT_COUNT      16

/* offset in switch_to */
#define SWITCH_TO_RA     0
#define SWITCH_TO_SP     8
#define SWITCH_TO_S0     16
#define SWITCH_TO_S1     24
#define SWITCH_TO_S2     32
#define SWITCH_TO_S3     40
#define SWITCH_TO_S4     48
#define SWITCH_TO_S5     56
#define SWITCH_TO_S6     64
#define SWITCH_TO_S7     72
#define SWITCH_TO_S8     80
#define SWITCH_TO_S9     88
#define SWITCH_TO_S10    96
#define SWITCH_TO_S11    104
#define SWITCH_TO_SATP   112

#define SWITCH_TO_SIZE   120

/* offset in signal */
#define SIGNAL_CONTEXT_RA     0
#define SIGNAL_CONTEXT_SP     8
#define SIGNAL_CONTEXT_S0     16
#define SIGNAL_CONTEXT_S1     24
#define SIGNAL_CONTEXT_S2     32
#define SIGNAL_CONTEXT_S3     40
#define SIGNAL_CONTEXT_S4     48
#define SIGNAL_CONTEXT_S5     56
#define SIGNAL_CONTEXT_S6     64
#define SIGNAL_CONTEXT_S7     72
#define SIGNAL_CONTEXT_S8     80
#define SIGNAL_CONTEXT_S9     88
#define SIGNAL_CONTEXT_S10    96
#define SIGNAL_CONTEXT_S11    104
#define SIGNAL_CONTEXT_KERNEL_SP 112
#define SIGNAL_CONTEXT_USER_SP 120
#define SIGNAL_CONTEXT_MASK   128

#define SIGNAL_CONTEXT_SIZE 136

#endif
