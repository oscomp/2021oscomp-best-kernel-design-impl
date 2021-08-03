#include <sys/syscall.h>
#include <stdint.h>
#include <type.h>
#include <os/sched.h>

void sys_testdisk()
{
    invoke_syscall(SYSCALL_TESTDISK, IGNORE, IGNORE, IGNORE, IGNORE, IGNORE);
}

void sys_write(char *buff)
{
    invoke_syscall(SYSCALL_WRITE, (uintptr_t)buff, IGNORE, IGNORE, IGNORE, IGNORE);
}

void sys_move_cursor(int x, int y)
{
    invoke_syscall(SYSCALL_CURSOR, x, y, IGNORE, IGNORE, IGNORE);
}

void sys_exit()
{
    invoke_syscall(SYS_exit, IGNORE, IGNORE, IGNORE, IGNORE, IGNORE);
}

int8_t sys_test(const char* filename)
{
    return invoke_syscall(SYSCALL_TEST, filename, IGNORE, IGNORE, IGNORE, IGNORE);
}

int8_t sys_exec(const char* file_name, char* argv[], char *const envp[])
{
    return invoke_syscall(SYS_execve, file_name, argv, envp, IGNORE, IGNORE);
}

