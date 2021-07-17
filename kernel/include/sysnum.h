#ifndef __SYSNUM_H
#define __SYSNUM_H

// System call numbers
#define SYS_fork         1
#define SYS_wait         3
#define SYS_exec         7
#define SYS_sbrk        12
#define SYS_sleep       13
#define SYS_uptime      14
#define SYS_trace       18
#define SYS_test_proc   22


#define SYS_getcwd      17
#define SYS_dup         23
#define SYS_dup3        24
#define SYS_fcntl       25
#define SYS_ioctl       29
#define SYS_mkdirat     34
#define SYS_unlinkat    35
#define SYS_umount      39
#define SYS_mount       40
#define SYS_faccessat   48
#define SYS_chdir       49
#define SYS_openat      56
#define SYS_close       57
#define SYS_pipe2       59
#define SYS_getdents    61
#define SYS_lseek       62
#define SYS_read        63
#define SYS_write       64
#define SYS_writev      66
#define SYS_ppoll       73
#define SYS_readlinkat  78
#define SYS_fstatat     79
#define SYS_fstat       80
#define SYS_utimensat   88
#define SYS_exit        93
#define SYS_exit_group  94
#define SYS_nanosleep  101
#define SYS_sched_yield 124
#define SYS_kill       129
#define SYS_times      153
#define SYS_uname      160
#define SYS_gettimeofday 169
#define SYS_getpid     172
#define SYS_getppid    173
#define SYS_getuid     174
#define SYS_geteuid    175
#define SYS_getgid     176
#define SYS_getegid    177
#define SYS_sysinfo    179
#define SYS_brk        214
#define SYS_munmap     215
#define SYS_clone      220
#define SYS_execve     221
#define SYS_mmap       222
#define SYS_mprotect   226
#define SYS_wait4      260
#define SYS_renameat2  276

#endif
