#ifndef __DEBUG_pipe 
#undef DEBUG 
#endif 

#define __module_name__ 	"pipe"

#include "types.h"
#include "hal/riscv.h"
#include "param.h"
#include "sync/spinlock.h"
#include "sched/proc.h"
#include "sync/sleeplock.h"
#include "fs/file.h"
#include "fs/pipe.h"
#include "mm/kmalloc.h"
#include "mm/vm.h"
#include "mm/pm.h"
#include "utils/debug.h"
#include "errno.h"
#include "fs/poll.h"


/**
 * The wait_node is on stack, which is accessed via va.
 * If we want other process to wake us up, we need to
 * transplant va to pa. (In fact, it's still va, but it's
 * equal to pa since we use direct-mapping.
 */
// static inline struct wait_node *waitinit(struct wait_node *onstacknode)
// {
// 	struct wait_node *pwait;
// 	pwait = (struct wait_node *)kwalkaddr(myproc()->pagetable, (uint64)onstacknode);
// 	pwait->chan = pwait;
// 	return pwait;
// }

static uint32 pipepoll(struct file *, struct poll_table *);

int
pipealloc(struct file **pf0, struct file **pf1)
{
	struct pipe *pi = NULL;
	struct file *f0, *f1 = NULL;

	if ((f0 = filealloc()) == NULL ||
		(f1 = filealloc()) == NULL ||
		// (pi = kmalloc(sizeof(struct pipe))) == NULL)
		(pi = allocpage()) == NULL)
	{
		goto bad;
	}

	pi->readopen = 1;
	pi->writeopen = 1;
	pi->nwrite = 0;
	pi->nread = 0;
	pi->writing = 0;

	initlock(&pi->lock, "pipe");
	wait_queue_init(&pi->wqueue, "pipewritequeue");
	wait_queue_init(&pi->rqueue, "pipereadqueue");

	f0->type = FD_PIPE;
	f0->readable = 1;
	f0->writable = 0;
	f0->pipe = pi;
	f0->poll = pipepoll;

	f1->type = FD_PIPE;
	f1->readable = 0;
	f1->writable = 1;
	f1->pipe = pi;
	f1->poll = pipepoll;

	*pf0 = f0;
	*pf1 = f1;
	return 0;

 bad:
	if (pi)
		freepage(pi);
		// kfree(pi);
	if (f0)
		fileclose(f0);
	if (f1)
		fileclose(f1);
	*pf0 = *pf1 = NULL;
	return -ENOMEM;
}

#define PIPE_READER	0
#define PIPE_WRITER	1

static void pipelock(struct pipe *pi, struct wait_node *wait, int who)
{
	struct wait_queue *q;
	q = (who == PIPE_READER) ? &pi->rqueue : &pi->wqueue;

	acquire(&q->lock);
	wait_queue_add(q, wait);	// stay in line

	// whether we are the first arrival
	while (!wait_queue_is_first(q, wait)) {
		sleep(wait->chan, &q->lock);
	}
	release(&q->lock);
}

static void pipeunlock(struct pipe *pi, struct wait_node *wait, int who)
{
	struct wait_queue *q;
	q = (who == PIPE_READER) ? &pi->rqueue : &pi->wqueue;

	acquire(&q->lock);
	// if (wait_queue_empty(q))
	// 	panic("pipeunlock: empty queue");
	// wait_queue_empty(q);

	// if (wait != wait_queue_next(q))
	// 	panic("pipeunlock: not next");
	
	wait_queue_del(wait);			// walk out the queue
	if (!wait_queue_empty(q)) {		// wake up the next one
		wait = wait_queue_next(q);
		wakeup(wait->chan);
	}
	release(&q->lock);
}

static void pipewakeup(struct pipe *pi, int who)
{
	struct wait_queue *queue;

	queue = (who == PIPE_READER) ? &pi->rqueue : &pi->wqueue;
	acquire(&queue->lock);
	if (!wait_queue_empty(queue)) {
		struct wait_node *wno = wait_queue_next(queue);
		wakeup(wno->chan);
	}
	release(&queue->lock);
}

void
pipeclose(struct pipe *pi, int writable)
{
	acquire(&pi->lock);
	if (writable) {
		pi->writeopen = 0;
		pipewakeup(pi, PIPE_READER);
	} else {
		pi->readopen = 0;
		pipewakeup(pi, PIPE_WRITER);
	}
	if (pi->readopen == 0 && pi->writeopen == 0) {
		release(&pi->lock);
		// kfree(pi);
		freepage(pi);
	} else
		release(&pi->lock);
}

static int pipewritable(struct pipe *pi)
{
	struct proc *pr = myproc();
	struct wait_node *wait;
	int m;
	
	/**
	 * We are the first one in the queue, and that's why
	 * we can be in this function, so we are safe to read
	 * our wait_node without locking.
	 */
	wait = wait_queue_next(&pi->wqueue);
	
	/**
	 * It is not atomic visit that we hold this lock for.
	 * The writer and reader will not change the opposite
	 * fields, we don't worry about it.
	 * First, this lock helps us get through the sleep().
	 * Second, if it is the moment when we are on our way
	 * to sleep() that the other end is trying to wake us
	 * up, then we might miss this wakeup().
	 * In a word, when waking up the other end, hold pi->lock.
	 */
	acquire(&pi->lock);
	pi->writing = 1;
	while ((m = pi->nwrite - pi->nread) == PIPESIZE) {		// pipe is full
		if (pr->killed) {
			pi->writing = 0;
			m = -EINTR;
			break;
		}
		if (pi->readopen == 0) {
			pi->writing = 0;
			m = -EPIPE;
			break;
		}
		pipewakeup(pi, PIPE_READER);
		sleep(wait->chan, &pi->lock);
	}
	release(&pi->lock);
	return m;
}

static int pipereadable(struct pipe *pi, int immediate)
{
	struct proc *pr = myproc();
	struct wait_node *wait;
	int m;

	// Need no lock here. See pipewritable().
	wait = wait_queue_next(&pi->rqueue);

	acquire(&pi->lock);
	while ((m = pi->nwrite - pi->nread) == 0) {	// pipe is empty
		if (pr->killed) {
			m = -EINTR;
			break;
		}
		if (pi->writeopen == 0) {
			m = -EPIPE;
			break;
		}
		pipewakeup(pi, PIPE_WRITER);
		if (!pi->writing && immediate) {
			m = -1;
			break;
		} else {
			sleep(wait->chan, &pi->lock);
		}
	}
	release(&pi->lock);
	return m;
}

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
	int i, m;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait;
	wait.chan = &wait;
	pipelock(pi, &wait, PIPE_WRITER);		// block other writers
	for (i = 0; i < n;) {
		if ((m = pipewritable(pi)) < 0) {
			i = m;
			goto out;
		}
		m = (PIPESIZE - m < n - i) ?	// amount of bytes to write
			PIPESIZE - m : n - i;

		while (m > 0) {					// pipe is a loop in a buf
			char *paddr = pi->data + pi->nwrite % PIPESIZE;
			int count = (pipebound - paddr < m) ? pipebound - paddr : m;

			if (copyin_nocheck(paddr, addr + i, count) < 0) {
				n = 0;
				break;
			}
			i += count;
			pi->nwrite += count;
			m -= count;
		}
	}
	acquire(&pi->lock);		// see pipewritable()
	pipewakeup(pi, PIPE_READER);
	pi->writing = 0;
	release(&pi->lock);
out:
	pipeunlock(pi, &wait, PIPE_WRITER);
	// __debug_info("pipewrite", "written %d/%d\n", i, n);
	return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
	int tot = 0, m;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait;
	wait.chan = &wait;
	pipelock(pi, &wait, PIPE_READER);	// block other readers

	while (tot < n) {
		if ((m = pipereadable(pi, tot > 0)) < 0) {
			if (tot == 0)
				tot = m;
			goto out;
		}
		if (m > n - tot)
			m = n - tot;
		for (int i = 0; i < m;) {
			char *paddr = pi->data + pi->nread % PIPESIZE;
			int count = (pipebound - paddr < m - i) ? pipebound - paddr : m - i;

			if (copyout_nocheck(addr + i, paddr, count) < 0) {
				n = 0;			// halt the do-while loop
				break;
			}
			pi->nread += count;
			i += count;
			tot += count;
		}
	}
	acquire(&pi->lock);		// see pipewritable()
	pipewakeup(pi, PIPE_WRITER);
	release(&pi->lock);
out:
	pipeunlock(pi, &wait, PIPE_READER);
	// __debug_info("piperead", "read %d\n", i);
	return tot;
}

int pipewritev(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ret = 0;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait;

	// pwait = waitinit(&wait);
	wait.chan = &wait;
	pipelock(pi, &wait, PIPE_WRITER);	// block other writers

	for (int i = 0; i < count; i++) {
		uint64 addr = (uint64)ioarr[i].iov_base;
		uint64 n = ioarr[i].iov_len;
		int j;
		
		if (!rangeinseg(addr, addr + n))
			break;
		for (j = 0; j < n;) {
			int m = pipewritable(pi);
			if (m < 0) {
				ret = -EPIPE;
				goto out2;
			}
			m = (PIPESIZE - m < n - j) ? PIPESIZE - m : n - j;		// amount of bytes to write
			while (m > 0) {							// pipe is a loop in a buf
				char *paddr = pi->data + pi->nwrite % PIPESIZE;
				int cnt = (pipebound - paddr < m) ? pipebound - paddr : m;

				if (copyin_nocheck(paddr, addr + j, cnt) < 0)
					goto out1;
				m -= cnt;
				j += cnt;
				pi->nwrite += cnt;
				ret += cnt;
			}
		}
	}
out1:
	acquire(&pi->lock);		// see pipewritable()
	pipewakeup(pi, PIPE_READER);
	pi->writing = 0;
	release(&pi->lock);
out2:
	pipeunlock(pi, &wait, PIPE_WRITER);
	return ret;
}

int pipereadv(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ndata, ret = 0;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait;

	// pwait = waitinit(&wait);
	wait.chan = &wait;
	pipelock(pi, &wait, PIPE_READER);	// block other readers

	if ((ndata = pipereadable(pi, 0)) < 0) {
		ret = -EPIPE;
		goto out2;
	}
	for (int i = 0; i < count && ndata > 0; i++) {
		uint64 addr = (uint64)ioarr[i].iov_base;
		uint64 n = ioarr[i].iov_len;
		int m = n < ndata ? n : ndata;
		int j;

		if (!rangeinseg(addr, addr + n))
			break;
		for (j = 0; j < m;) {
			char *paddr = pi->data + pi->nread % PIPESIZE;
			int cnt = (pipebound - paddr < m - j) ? pipebound - paddr : m - j;

			if (copyout_nocheck(addr + j, paddr, cnt) < 0)
				goto out1;
			pi->nread += cnt;
			j += cnt;
			ret += cnt;
			ndata -= cnt;
		}
	}
out1:
	acquire(&pi->lock);		// see pipewritable()
	pipewakeup(pi, PIPE_WRITER);
	release(&pi->lock);
out2:
	pipeunlock(pi, &wait, PIPE_READER);
	return ret;
}


static
uint32 pipepoll(struct file *fp, struct poll_table *pt)
{
	uint32 mask = 0;
	struct pipe *pi = fp->pipe;

	__debug_info("pipepoll", "r/w=%d/%d | ro/wo=%d/%d | rq/wq=%d/%d rn/wn=%d/%d\n",
				fp->readable, fp->writable, pi->readopen, pi->writeopen,
				!wait_queue_empty(&pi->rqueue), !wait_queue_empty(&pi->wqueue),
				pi->nread, pi->nwrite);

	if (fp->readable)
		poll_wait(fp, &pi->rqueue, pt);
	if (fp->writable)
		poll_wait(fp, &pi->wqueue, pt);

	if (fp->readable) {
		if (pi->nwrite - pi->nread > 0)			// has something to read
			mask |= POLLIN;
		if (!pi->writeopen) {
			// if (pi->nwrite - pi->nread == 0)
			// 	mask |= POLLPRI;
			// else
				mask |= POLLHUP;
		}
	}

	if (fp->writable) {
		if (pi->nwrite - pi->nread < PIPESIZE)	// has room to write
			mask |= POLLOUT;
		if (!pi->readopen)
			mask |= POLLERR;
	}

	return mask;
}
