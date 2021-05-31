
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

void sys_write(char *);
void sys_move_cursor(int, int);
void sys_reflush();

long sys_get_timebase();
long sys_get_tick();
void sys_move_cursor(int x, int y);
void sys_exit();

void sys_testdisk();
pid_t sys_exec(const char* file_name, int argc, char* argv[], spawn_mode_t mode);

int8_t sys_test(const char* filename);

#endif
