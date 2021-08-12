#include <os/syscall.h>
#include <stdio.h>
#include <os/sched.h>
#include <os/fat32.h>

long (*syscall[NUM_SYSCALLS])() = {0};

void handle_syscall(regs_context_t *regs, uint64_t interrupt, uint64_t cause)
{
    log(0, "\n\ncurrent pid is %d", current_running->pid);
    log(0, "current tid is %d", current_running->tid);
    log(0, "num is %d", regs->regs[17]);
    // log(0, "sp is %lx", current_running->user_sp);
    // log(0, "tp is %lx", *((uint64_t *)(current_running->user_sp - 8)));
    // log(0, "sepc is %lx", regs->sepc);
    // log(0, "ra is %lx", regs->regs[1]);
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
