
#ifndef INCLUDE_SYSCALL_H_
#define INCLUDE_SYSCALL_H_

#include <os/syscall_number.h>
#include <stdint.h>
#include <os/sched.h>
#include <type.h>

extern uint64_t invoke_syscall(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void sys_sleep(uint32_t);

void sys_futex_wait(volatile uint64_t *val_addr, uint64_t val);
void sys_futex_wakeup(volatile uint64_t *val_addr, int num_wakeup);

ssize_t sys_write(fd_num_t fd, char *buff, size_t count);
void sys_move_cursor(int, int);
void sys_reflush();

long sys_get_timebase();
long sys_get_tick();
void sys_move_cursor(int x, int y);
void sys_exit();

void sys_testdisk();
int8_t sys_exec(const char* file_name, char* argv[], char *const envp[]);
pid_t sys_clone(uint32_t flag, uint64_t stack, pid_t ptid, void *tls, pid_t ctid);
int8_t sys_test(const char* filename);
pid_t sys_wait4(pid_t pid, uint16_t *status, int32_t options);

#endif
