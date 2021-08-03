#ifndef __POLL_H
#define __POLL_H

#include "types.h"
#include "time.h"
#include "sync/spinlock.h"
#include "mesg/signal.h"
#include "fs/file.h"
#include "utils/dlist.h"
#include "sync/waitqueue.h"


#define POLLIN      0x0001
#define POLLPRI     0x0002
#define POLLOUT     0x0004
#define POLLERR     0x0008
#define POLLHUP     0x0010
#define POLLNVAL    0x0020

#define POLLIN_SET	(POLLIN | POLLHUP | POLLERR)
#define POLLOUT_SET	(POLLOUT | POLLERR)
#define POLLEX_SET	POLLPRI


// Argument for poll().
struct pollfd {
	int32 fd;         /* file descriptor */
	int16 events;     /* requested events */
	int16 revents;    /* returned events */
};

// Argument for select(). Each bit represents an fd.
#define FDSET_SIZE		1024
#define NFDBIT			(sizeof(uint64) * 8)
#define NFDBITS			(FDSET_SIZE / NFDBIT)
struct fdset {
	uint64 fd_bits[NFDBITS];
};


struct poll_table;
struct file;

/**
 * This is to put all the observing files' wait_queue into the poll_wait_queue,
 * so that poll/select can remove the nodes from them before returning.
 */
typedef void (*poll_func)(struct file *, struct wait_queue *, struct poll_table *);

struct poll_table {
	poll_func	func;
	uint32		key;
};


// The files use this to wake process up.
struct poll_wait_node {
	struct wait_queue *queue;
	struct wait_node node;
};

#define ON_STACK_PWN_NUM	24

// Main struct in poll/select. All poll_wait_nodes are got from here.
struct poll_wait_queue {
	struct poll_table pt;
	uint64 error;
	int index;
	struct poll_wait_node nodes[ON_STACK_PWN_NUM];
};




static inline void
poll_wait(struct file * fp, struct wait_queue *wq, struct poll_table *pt)
{
	if (pt && pt->func && wq)
		pt->func(fp, wq, pt);
}


int ppoll(struct pollfd *pfds, int nfds, struct timespec *timeout, __sigset_t *sigmask);

int pselect(int nfds, struct fdset *readfds,
			struct fdset *writefds, struct fdset *exceptfds,
			struct timespec *timeout, __sigset_t *sigmask);

#endif