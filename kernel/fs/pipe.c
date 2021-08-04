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
	initsleeplock(&pi->wlock, "pipewriter");
	initsleeplock(&pi->rlock, "pipereader");
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

static void pipewakeup(struct pipe *pi, int who)
{
	struct wait_queue *queue;
	struct d_list *l;
	void *chan;

	if (who == PIPE_READER) {
		chan = &pi->nread;
		queue = &pi->rqueue;
	} else if (who == PIPE_WRITER) {
		chan = &pi->nwrite;
		queue = &pi->wqueue;
	} else
		panic("pipewakeup");

	wakeup(chan);
	acquire(&queue->lock);
	for (l = queue->head.next; l != &queue->head; l = l->next) {
		struct poll_wait_node *pwn = dlist_entry(l, struct poll_wait_node, node);
		wakeup(pwn->chan);
		__debug_info("pipewakeup", "pwn=%p, chan=%p\n", pwn, pwn->chan);
	}
	release(&queue->lock);
}

// Only when the file's ref decreases down to 0 can we call to this.
// So we don't need to hold locks.
void
pipeclose(struct pipe *pi, int writable)
{
	if (writable) {
		pi->writeopen = 0;
		pipewakeup(pi, PIPE_READER);
	} else {
		pi->readopen = 0;
		pipewakeup(pi, PIPE_WRITER);
	}
	if (pi->readopen == 0 && pi->writeopen == 0)
		kfree(pi);
}

static int pipewritable(struct pipe *pi)
{
	struct proc *pr = myproc();
	int m;
	while ((m = pi->nwrite - pi->nread) == PIPESIZE) {		// pipe is full
		if (pi->readopen == 0 || pr->killed) {
			return -1;
		}
		pipewakeup(pi, PIPE_READER);
		acquire(&pi->lock);		// Hold the lock for sleep().
		sleep(&pi->nwrite, &pi->lock);
		release(&pi->lock);
	}
	return m;
}

static int pipereadable(struct pipe *pi)
{
	struct proc *pr = myproc();
	int m;
	// __debug_info("pipereadable", "nr/nw=%d/%d, r=%d, w=%d\n",
	// 			pi->nread, pi->nwrite, pi->readopen, pi->writeopen);
	while ((m = pi->nwrite - pi->nread) == 0 && pi->writeopen) {	// pipe is empty
		if (pr->killed) {
			return -1;
		}
		acquire(&pi->lock);
		sleep(&pi->nread, &pi->lock); //DOC: piperead-sleep
		release(&pi->lock);
	}
	return m;
}

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
	int i, m;
	char *const pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->wlock);	// block other writers
	// acquire(&pi->lock);			// avoid races between writers and readers
	for (i = 0; i < n;) {
		if ((m = pipewritable(pi)) < 0) {
			// release(&pi->lock);
			// releasesleep(&pi->wlock);
			// return -1;
			i = -EPIPE;
			goto out;
		}
		m = (PIPESIZE - m < n - i) ?	// amount of bytes to write
			PIPESIZE - m : n - i;

		while (m > 0) {					// pipe is a loop in a buf
			char *paddr = pi->data + pi->nwrite % PIPESIZE;
			int count = (pipebound - paddr < m) ? pipebound - paddr : m;

			/**
			 * This might lead to a page fault due to lazy-elf-load,
			 * which requests a file reading and then schedules.
			 * Must let go the spinlock, then re-acquire it after
			 * copying done. Since we use wlock to block other writers,
			 * it's OK to release the spinlock.
			 */
			// release(&pi->lock);
			int res = copyin2(paddr, addr + i, count);
			// acquire(&pi->lock);

			if (res < 0)
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
	// release(&pi->lock);
	releasesleep(&pi->wlock);
	// __debug_info("pipewrite", "written %d/%d\n", i, n);
	return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
	int i = -1, m;
	char *const pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->rlock);	// block other readers
	// acquire(&pi->lock);
	if ((m = pipereadable(pi)) < 0) {
		// release(&pi->lock);
		// return -1;
		goto out;
	}
	if (m > n)
		m = n;
	for (i = 0; i < m;) {
		char *paddr = pi->data + pi->nread % PIPESIZE;
		int count = (pipebound - paddr < m - i) ? pipebound - paddr : m - i;
		int res;

		/**
		 * The very same problem as described in pipewrite().
		 */
		// release(&pi->lock);
		res = copyout2(addr + i, paddr, count);
		// acquire(&pi->lock);

		if (res < 0)
			break;
		pi->nread += count;
		i += count;
	}
	pipewakeup(pi, PIPE_WRITER);
out:
	// release(&pi->lock);
	releasesleep(&pi->rlock);
	// __debug_info("piperead", "read %d\n", i);
	return i;
}

int pipewritev(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ret = 0;
	char *const pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->wlock);	// block other writers
	// acquire(&pi->lock);
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
				int res;

				// release(&pi->lock);
				res = copyin2(paddr, (uint64)ioarr[i].iov_base + j, cnt);
				// acquire(&pi->lock);

				if (res < 0)
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
	// release(&pi->lock);
	releasesleep(&pi->wlock);
	return ret;
}

int pipereadv(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ndata, ret = 0;
	char *const pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->rlock);	// block other readers
	// acquire(&pi->lock);
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
			int res;

			// release(&pi->lock);
			res = copyout2((uint64)ioarr[i].iov_base + j, paddr, cnt);
			// acquire(&pi->lock);

			if (res < 0)
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
	// release(&pi->lock);
	releasesleep(&pi->rlock);
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
