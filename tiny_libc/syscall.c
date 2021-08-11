#include <sys/syscall.h>
#include <stdint.h>
#include <type.h>
#include <os/sched.h>

void sys_testdisk()
{
    invoke_syscall(SYSCALL_TESTDISK, IGNORE, IGNORE, IGNORE, IGNORE, IGNORE);
}

ssize_t sys_write(fd_num_t fd, char *buff, size_t count)
{
    return invoke_syscall(SYS_write, fd, buff, count, IGNORE, IGNORE);
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

pid_t sys_clone(uint32_t flag, uint64_t stack, pid_t ptid, void *tls, pid_t ctid)
{
    return invoke_syscall(SYS_clone, flag, stack, ptid, tls, ctid);
}

pid_t sys_wait4(pid_t pid, uint16_t *status, int32_t options)
{
    return invoke_syscall(SYS_wait4, pid, status, options, IGNORE, IGNORE);
}