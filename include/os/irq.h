
#ifndef INCLUDE_INTERRUPT_H_
#define INCLUDE_INTERRUPT_H_

#include <type.h>
#include <os/sched.h>

/* ERROR code */
enum IrqCode
{
    IRQC_U_SOFT  = 0,
    IRQC_S_SOFT  = 1,
    IRQC_M_SOFT  = 3,
    IRQC_U_TIMER = 4,
    IRQC_S_TIMER = 5,
    IRQC_M_TIMER = 7,
    IRQC_U_EXT   = 8,
    IRQC_S_EXT   = 9,
    IRQC_M_EXT   = 11,
    IRQC_COUNT
};

enum ExcCode
{
    EXCC_INST_MISALIGNED  = 0,
    EXCC_INST_ACCESS      = 1,
    EXCC_BREAKPOINT       = 3,
    EXCC_LOAD_ACCESS      = 5,
    EXCC_STORE_ACCESS     = 7,
    EXCC_SYSCALL          = 8,
    EXCC_INST_PAGE_FAULT  = 12,
    EXCC_LOAD_PAGE_FAULT  = 13,
    EXCC_STORE_PAGE_FAULT = 15,
    EXCC_COUNT
};

#define TIMER_INTERVAL 150000
#define PREEMPT_FREQUENCY 50

typedef void (*handler_t)(regs_context_t*, uint64_t, uint64_t);

extern handler_t irq_table[IRQC_COUNT];
extern handler_t exc_table[EXCC_COUNT];

extern void interrupt_helper(regs_context_t *regs, uint64_t stval, uint64_t cause, uint64_t tp);

/* exception handler entery */
extern void exception_handler_entry(void);
extern void init_exception();
extern void setup_exception();

extern void reset_irq_timer();
extern void handle_software();
extern void handle_int(regs_context_t *regs, uint64_t interrupt, uint64_t cause);
extern void handle_other(regs_context_t *regs, uint64_t interrupt, uint64_t cause);
extern void handle_syscall(regs_context_t *regs, uint64_t interrupt, uint64_t cause);
extern void handle_pgfault(regs_context_t *regs, uint64_t interrupt, uint64_t cause);
extern void handle_sext(regs_context_t *regs, uint64_t interrupt, uint64_t cause);


extern void enable_interrupt(void);
extern void disable_interrupt(void);
extern void clear_interrupt(void);
extern void enable_preempt(void);
extern void disable_preempt(void);

extern uint64_t read_satp();

extern uintptr_t riscv_dtb;

#endif
