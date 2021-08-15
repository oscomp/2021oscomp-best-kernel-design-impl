#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <os/fat32.h>
#include <os/sched.h>
#include <os/ring_buffer.h>

typedef uint32_t fd_set;

int64_t do_ioctl(fd_num_t fd, uint64_t request, const char *argp[]);
int do_pselect6(int nfds, fd_set *readfds, fd_set *writefds,
                   fd_set *exceptfds, const struct timespec *timeout,
                   const sigset_t *sigmask);
#endif