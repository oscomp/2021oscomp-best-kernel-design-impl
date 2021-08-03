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
#include "utils/debug.h"
#include "errno.h"
#include "fs/poll.h"


/**
 * The wait_node is on stack, which is accessed via va.
 * If we want other process to wake us up, we need to
 * transplant va to pa. (In fact, it's still va, but it's
 * equal to pa since we use direct-mapping.
 */
static inline struct wait_node *waitinit(struct wait_node *onstacknode)
{
	struct wait_node *pwait;
	pwait = (struct wait_node *)kwalkaddr(myproc()->pagetable, (uint64)onstacknode);
	pwait->chan = pwait;
	return pwait;
}

static uint32 pipepoll(struct file *, struct poll_table *);

int
pipealloc(struct file **pf0, struct file **pf1)
{
	struct pipe *pi = NULL;
	struct file *f0, *f1 = NULL;

	if ((f0 = filealloc()) == NULL ||
		(f1 = filealloc()) == NULL ||
		(pi = kmalloc(sizeof(struct pipe))) == NULL)
	{
		goto bad;
	}

	pi->readopen = 1;
	pi->writeopen = 1;
	pi->nwrite = 0;
	pi->nread = 0;

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
		kfree(pi);
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
	if (wait_queue_empty(q))
		panic("pipeunlock: empty queue");

	if (wait != wait_queue_next(q))
		panic("pipeunlock: not next");
	
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

// Only when the file's ref decreases down to 0 can we call to this.
// So we don't need to hold locks.
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
		kfree(pi);
	} else
		release(&pi->lock);
}

static int pipewritable(struct pipe *pi)
{
	struct proc *pr = myproc();
	struct wait_queue *wq = &pi->wqueue;
	struct wait_node *wait;
	int m;
	while ((m = pi->nwrite - pi->nread) == PIPESIZE) {		// pipe is full
		if (pi->readopen == 0 || pr->killed) {
			return -1;
		}
		pipewakeup(pi, PIPE_READER);
		acquire(&wq->lock);
		wait = wait_queue_next(wq);
		sleep(wait->chan, &wq->lock);
		release(&wq->lock);
	}
	return m;
}

static int pipereadable(struct pipe *pi)
{
	struct proc *pr = myproc();
	struct wait_queue *rq = &pi->rqueue;
	struct wait_node *wait;
	int m;
	// __debug_info("pipereadable", "nr/nw=%d/%d, r=%d, w=%d\n",
	// 			pi->nread, pi->nwrite, pi->readopen, pi->writeopen);
	while ((m = pi->nwrite - pi->nread) == 0 && pi->writeopen) {	// pipe is empty
		if (pr->killed) {
			return -1;
		}
		acquire(&rq->lock);
		wait = wait_queue_next(rq);
		sleep(wait->chan, &rq->lock);
		release(&rq->lock);
	}
	return m;
}

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
	int i, m;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait, *pwait;

	pwait = waitinit(&wait);
	pipelock(pi, pwait, PIPE_WRITER);		// block other writers
	for (i = 0; i < n;) {
		if ((m = pipewritable(pi)) < 0) {
			i = -EPIPE;
			goto out;
		}
		m = (PIPESIZE - m < n - i) ?	// amount of bytes to write
			PIPESIZE - m : n - i;

		while (m > 0) {					// pipe is a loop in a buf
			char *paddr = pi->data + pi->nwrite % PIPESIZE;
			int count = (pipebound - paddr < m) ? pipebound - paddr : m;

			if (copyin2(paddr, addr + i, count) < 0)
				break;
			i += count;
			pi->nwrite += count;
			m -= count;
		}
		if (m > 0)
			break;
	}
	pipewakeup(pi, PIPE_READER);
out:
	pipeunlock(pi, pwait, PIPE_WRITER);
	// __debug_info("pipewrite", "written %d/%d\n", i, n);
	return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
	int i = -1, m;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait, *pwait;

	pwait = waitinit(&wait);
	pipelock(pi, pwait, PIPE_READER);	// block other readers

	if ((m = pipereadable(pi)) < 0) {
		goto out;
	}
	if (m > n)
		m = n;
	for (i = 0; i < m;) {
		char *paddr = pi->data + pi->nread % PIPESIZE;
		int count = (pipebound - paddr < m - i) ? pipebound - paddr : m - i;

		if (copyout2(addr + i, paddr, count) < 0)
			break;
		pi->nread += count;
		i += count;
	}
	pipewakeup(pi, PIPE_WRITER);
out:
	pipeunlock(pi, pwait, PIPE_READER);
	// __debug_info("piperead", "read %d\n", i);
	return i;
}

int pipewritev(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ret = 0;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait, *pwait;

	pwait = waitinit(&wait);
	pipelock(pi, pwait, PIPE_WRITER);	// block other writers

	for (int i = 0; i < count; i++) {
		uint64 n = ioarr[i].iov_len;
		int j;
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

				if (copyin2(paddr, (uint64)ioarr[i].iov_base + j, cnt) < 0)
					goto out1;
				m -= cnt;
				j += cnt;
				pi->nwrite += cnt;
				ret += cnt;
			}
		}
	}
out1:
	pipewakeup(pi, PIPE_READER);
out2:
	pipeunlock(pi, pwait, PIPE_WRITER);
	return ret;
}

int pipereadv(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ndata, ret = 0;
	char *const pipebound = pi->data + PIPESIZE;
	struct wait_node wait, *pwait;

	pwait = waitinit(&wait);
	pipelock(pi, pwait, PIPE_READER);	// block other readers

	if ((ndata = pipereadable(pi)) < 0) {
		ret = -EPIPE;
		goto out2;
	}
	for (int i = 0; i < count && ndata > 0; i++) {
		uint64 n = ioarr[i].iov_len;
		int m = n < ndata ? n : ndata;
		int j;
		for (j = 0; j < m;) {
			char *paddr = pi->data + pi->nread % PIPESIZE;
			int cnt = (pipebound - paddr < m - j) ? pipebound - paddr : m - j;

			if (copyout2((uint64)ioarr[i].iov_base + j, paddr, cnt) < 0)
				goto out1;
			pi->nread += cnt;
			j += cnt;
			ret += cnt;
			ndata -= cnt;
		}
	}
out1:
	pipewakeup(pi, PIPE_WRITER);
out2:
	pipeunlock(pi, pwait, PIPE_READER);
	return ret;
}


static
uint32 pipepoll(struct file *fp, struct poll_table *pt)
{
	uint32 mask = 0;
	struct pipe *pi = fp->pipe;

	if (fp->readable)
		poll_wait(fp, &pi->rqueue, pt);
	if (fp->writable)
		poll_wait(fp, &pi->wqueue, pt);

	if (fp->readable) {
		if (pi->nwrite - pi->nread > 0)			// has something to read
			mask |= POLLIN;
		if (!pi->writeopen)
			mask |= POLLHUP;
	}

	if (fp->writable) {
		if (pi->nwrite - pi->nread < PIPESIZE)	// has room to write
			mask |= POLLOUT;
		if (!pi->readopen)
			mask |= POLLERR;
	}

	return mask;
}
