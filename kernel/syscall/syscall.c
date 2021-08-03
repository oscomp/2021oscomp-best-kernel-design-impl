#include <os/syscall.h>
#include <stdio.h>
#include <os/sched.h>

long (*syscall[NUM_SYSCALLS])();

void handle_syscall(regs_context_t *regs, uint64_t interrupt, uint64_t cause)
{
    printk_port("num is %d\n", regs->regs[17]);
    // printk_port("sp is %lx\n", current_running->user_sp);
    // syscall[fn](arg1, arg2, arg3)
    regs->sepc = regs->sepc + 4;
    regs->regs[10] = syscall[regs->regs[17]](regs->regs[10],
                                              regs->regs[11],
                                              regs->regs[12],
                                              regs->regs[13],
                                              regs->regs[14],
                                              regs->regs[15],
                                              regs->regs[16]);
}
