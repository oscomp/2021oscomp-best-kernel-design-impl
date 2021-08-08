#ifndef __ASM_GENERIC_POLL_H
#define __ASM_GENERIC_POLL_H

#include <os/fat32.h>

#define POLLIN 0x0001
#define POLLPRI 0x0002
#define POLLOUT 0x0004
#define POLLERR 0x0008
#define POLLHUP 0x0010
#define POLLNVAL 0x0020

struct pollfd {
    fd_num_t   fd;         /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};

typedef uint32_t nfds_t;

int32_t do_ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *tmo_p, const sigset_t *sigmask);

#endif
