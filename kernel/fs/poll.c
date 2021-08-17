#ifndef __DEBUG_poll
#undef DEBUG 
#endif 

#define __module_name__ "poll"

#include "types.h"
#include "errno.h"
#include "hal/riscv.h"
#include "fs/poll.h"
#include "sched/proc.h"
#include "mm/vm.h"
#include "time.h"
#include "timer.h"
#include "utils/string.h"
#include "utils/debug.h"


static void __poll_func(struct file *f, struct wait_queue *wq, struct poll_table *pt)
{
	struct poll_wait_queue *pwq = container_of(pt, struct poll_wait_queue, pt);
	struct poll_wait_node *pwn;
	if (pwq->index >= ON_STACK_PWN_NUM)	// we can allocate pages
		return;

	pwn = &pwq->nodes[pwq->index++];
	pwn->node.chan = pwq;
	pwn->queue = wq;
	wait_queue_add_locked(wq, &pwn->node);
}


static void poll_init(struct poll_wait_queue *pwq)
{
	pwq->pt.func = __poll_func;
	pwq->pt.key = 0;
	pwq->error = 0;
	pwq->index = 0;
}


static void poll_end(struct poll_wait_queue *pwq)
{
	for (int i = 0; i < pwq->index; i++) {
		struct poll_wait_node *pwn = pwq->nodes + i;
		struct wait_queue *wq = pwn->queue;
		acquire(&wq->lock);
		wait_queue_del(&pwn->node);
		if (!wait_queue_empty(wq)) {
			struct wait_node *node = wait_queue_next(wq);
			wakeup(node->chan);
		}
		release(&wq->lock);
	}
}


static uint32 file_poll(struct file *fp, struct poll_table *pt)
{
	if (!fp->poll)
		return POLLIN|POLLOUT;
	return fp->poll(fp, pt);
}


static int poll_sched_timeout(struct poll_wait_queue *pwq, uint64 expire)
{
	struct proc *p = myproc();
	struct proc *volatile vp = p;
	int ret = 0;

	acquire(&p->lk);	// Hold this for sleep().
	
	p->sleep_expire = expire;
	sleep(pwq, &p->lk);
	
	// Check whether we are waken up by timeout.
	// If so, this field will be set to zero.
	if (vp->sleep_expire == 0)
		ret = 1;
	else
		p->sleep_expire = 0;
	release(&p->lk);

	__debug_info("poll_sched_timeout", "pwq=%p, ret=%d\n", pwq, ret);

	return ret;
}


int ppoll(struct pollfd *pfds, int nfds, struct timespec *timeout, __sigset_t *sigmask)
{
	// uint64 now = readtime();
	// uint64 expire;
	
	// if (timeout)
	// 	expire	= now + convert_from_timespec(timeout);

	for (int i = 0; i < nfds; i++) {
		pfds[i].revents = POLLIN|POLLOUT;
	}
	return nfds;
}


// Copy fdset from arguments, since we need to write results into them later.
static inline void fdset_work_init(struct fdset *rfds, struct fdset *wfds, struct fdset *exfds,
							struct fdset *rarg, struct fdset *warg, struct fdset *exarg)
{
	if (rarg)
		memmove(rfds, rarg, sizeof(struct fdset));
	else
		memset(rfds, 0, sizeof(struct fdset));

	if (warg)
		memmove(wfds, warg, sizeof(struct fdset));
	else
		memset(wfds, 0, sizeof(struct fdset));

	if (exarg)
		memmove(exfds, exarg, sizeof(struct fdset));
	else
		memset(exfds, 0, sizeof(struct fdset));
}


int pselect(int nfds, struct fdset *readfds, struct fdset *writefds, struct fdset *exceptfds,
			struct timespec *timeout, __sigset_t *sigmask)
{
	struct fdset rfds, wfds, exfds;
	uint64 expire;
	struct poll_wait_queue wait;
	// struct poll_wait_queue *pwait;
	int immediate = 0;
	
	
	__debug_info("pselect", "timeout={%ds, %dns}\n", 
				timeout ? timeout->sec : -1, timeout ? timeout->nsec : -1);

	if (timeout) {
		expire = convert_from_timespec(timeout);
		if (expire == 0)
			immediate = 1;			// don't sleep
		else 
			expire += readtime();	// overflow?
	} else
		expire = 0;					// infinity

	fdset_work_init(&rfds, &wfds, &exfds, readfds, writefds, exceptfds);

	// Since the node is on stack, if we want other process to get us, we should use pa.
	// pwait = (struct poll_wait_queue *)kwalkaddr(myproc()->pagetable, (uint64)&wait);
	// pwait = &wait;
	poll_init(&wait);
	if (immediate)
		wait.pt.func = NULL;		// we won't be inserted into any queue in later poll()s

	int ret = 0;
	for (;;)
	{
		int i = 0;	// fd iterator
		for (int idx = 0; idx <= (nfds - 1) / NFDBIT; idx++)
		{
			uint64 r, w, ex, all;
			uint64 rres = 0, wres = 0, exres = 0;

			r = rfds.fd_bits[idx];		// extract fd bits
			w = wfds.fd_bits[idx];
			ex = exfds.fd_bits[idx];
			all = r | w | ex;
			if (all == 0) {				// no fd in this part
				i += NFDBIT;
				continue;
			}

			for (int b = 0; b < NFDBIT; b++, i++) {
				if (i >= nfds)			// reach end
					break;
				uint64 bit = 1L << b;
				if (!(bit & all))		// no fd in this bit
					continue;

				struct file *fp = fd2file(i, 0);
				if (!fp)				// bad fd
					continue;
				
				__debug_info("pselect", "fd=%d\n", i);
				
				wait.pt.key = POLLEX_SET;
				if (r & bit)
					wait.pt.key |= POLLIN_SET;
				if (w & bit)
					wait.pt.key |= POLLOUT_SET;

				uint32 mask = file_poll(fp, &wait.pt);
				__debug_info("pselect", "mask=%d\n", mask);

				if ((mask & POLLIN_SET) && (r & bit)) {
					rres |= bit;
					ret++;
					wait.pt.func = NULL;
				}
				if ((mask & POLLOUT_SET) && (w & bit)) {
					wres |= bit;
					ret++;
					wait.pt.func = NULL;
				}
				if ((mask & POLLEX_SET) && (ex & bit)) {
					exres |= bit;
					ret++;
					wait.pt.func = NULL;
				}
			}
			if (readfds)
				readfds->fd_bits[idx] = rres;
			if (writefds)
				writefds->fd_bits[idx] = wres;
			if (exceptfds)
				exceptfds->fd_bits[idx] = exres;
		}
		wait.pt.func = NULL;		// only need to be called once for each file

		if (ret > 0 || immediate)	// got results or don't sleep-wait
			break;
		if (wait.error) {
			ret = wait.error;
			break;
		}

		// at this point, maybe we are already waken up by some
		if (poll_sched_timeout(&wait, expire))
			immediate = 1;
	}

	poll_end(&wait);

	__debug_info("pselect", "return %d\n", ret);
	return ret;
}
