#ifndef __POLL_H
#define __POLL_H

#include "types.h"
#include "time.h"

#define POLLIN      0x0001
#define POLLPRI     0x0002
#define POLLOUT     0x0004
#define POLLERR     0x0008
#define POLLHUP     0x0010
#define POLLNVAL    0x0020


struct pollfd {
	int32 fd;         /* file descriptor */
	int16 events;     /* requested events */
	int16 revents;    /* returned events */
};

int do_ppoll(struct pollfd *pfds, int nfds /*, struct timespec *timeout, struct sigset *sigmask*/);

#endif