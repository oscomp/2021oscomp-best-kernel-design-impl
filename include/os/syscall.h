
#ifndef OS_SYSCALL_H_
#define OS_SYSCALL_H_

#include <os/sched.h>
#include <os/syscall_number.h>

/* syscall function pointer */
extern long (*syscall[NUM_SYSCALLS])();
extern void handle_syscall(regs_context_t *regs, uint64_t interrupt, uint64_t cause);

/* some syscall func */

#endif
