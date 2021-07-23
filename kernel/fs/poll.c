
#include "types.h"
#include "fs/poll.h"


int do_ppoll(struct pollfd *pfds, int nfds /*, struct timespec *timeout, struct sigset *sigmask*/)
{
	for (int i = 0; i < nfds; i++) {
		pfds[i].revents = POLLIN|POLLOUT;
	}
	return nfds;
}