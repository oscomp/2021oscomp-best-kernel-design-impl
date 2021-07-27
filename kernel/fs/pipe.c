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

int
pipealloc(struct file **f0, struct file **f1)
{
	struct pipe *pi;

	pi = 0;
	*f0 = *f1 = 0;
	if((*f0 = filealloc()) == NULL || (*f1 = filealloc()) == NULL)
		goto bad;
	if((pi = kmalloc(sizeof(struct pipe))) == NULL)
		goto bad;
	pi->readopen = 1;
	pi->writeopen = 1;
	pi->nwrite = 0;
	pi->nread = 0;

	initlock(&pi->lock, "pipe");
	initsleeplock(&pi->wlock, "pipewriter");
	initsleeplock(&pi->rlock, "pipereader");

	(*f0)->type = FD_PIPE;
	(*f0)->readable = 1;
	(*f0)->writable = 0;
	(*f0)->pipe = pi;
	(*f1)->type = FD_PIPE;
	(*f1)->readable = 0;
	(*f1)->writable = 1;
	(*f1)->pipe = pi;
	return 0;

 bad:
	if(pi)
		kfree(pi);
	if(*f0)
		fileclose(*f0);
	if(*f1)
		fileclose(*f1);
	return -1;
}

void
pipeclose(struct pipe *pi, int writable)
{
	acquire(&pi->lock);
	if(writable){
		pi->writeopen = 0;
		wakeup(&pi->nread);
	} else {
		pi->readopen = 0;
		wakeup(&pi->nwrite);
	}
	if(pi->readopen == 0 && pi->writeopen == 0){
		release(&pi->lock);
		kfree(pi);
	} else
		release(&pi->lock);
}

// caller must hold pi->lock
static int pipewritable(struct pipe *pi)
{
	struct proc *pr = myproc();
	int m;
	while ((m = pi->nwrite - pi->nread) == PIPESIZE) {		// pipe is full
		if (pi->readopen == 0 || pr->killed) {
			return -1;
		}
		wakeup(&pi->nread);
		sleep(&pi->nwrite, &pi->lock);
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
		sleep(&pi->nread, &pi->lock); //DOC: piperead-sleep
	}
	return m;
}

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
	int i, m;
	char const *pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->wlock);	// block other writers
	acquire(&pi->lock);			// avoid races between writers and readers
	for (i = 0; i < n;) {
		if ((m = pipewritable(pi)) < 0) {
			// release(&pi->lock);
			// releasesleep(&pi->wlock);
			// return -1;
			i = -1;
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
			release(&pi->lock);
			int res = copyin2(paddr, addr + i, count);
			acquire(&pi->lock);

			if (res < 0)
				break;
			i += count;
			pi->nwrite += count;
			m -= count;
		}
		if (m > 0)
			break;
	}
	wakeup(&pi->nread);
out:
	release(&pi->lock);
	releasesleep(&pi->wlock);
	// __debug_info("pipewrite", "written %d/%d\n", i, n);
	return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
	int i = -1, m;
	char const *pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->rlock);	// block other readers
	acquire(&pi->lock);
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
		release(&pi->lock);
		res = copyout2(addr + i, paddr, count);
		acquire(&pi->lock);

		if (res < 0)
			break;
		pi->nread += count;
		i += count;
	}
	wakeup(&pi->nwrite);  //DOC: piperead-wakeup
out:
	release(&pi->lock);
	releasesleep(&pi->rlock);
	// __debug_info("piperead", "read %d\n", i);
	return i;
}

int pipewritev(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ret = 0;
	char const *pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->wlock);	// block other writers
	acquire(&pi->lock);
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

				release(&pi->lock);
				res = copyin2(paddr, (uint64)ioarr[i].iov_base + j, cnt);
				acquire(&pi->lock);

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
	wakeup(&pi->nread);
out2:
	release(&pi->lock);
	releasesleep(&pi->wlock);
	return ret;
}

int pipereadv(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ndata, ret = 0;
	char const *pipebound = pi->data + PIPESIZE;

	acquiresleep(&pi->rlock);	// block other readers
	acquire(&pi->lock);
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

			release(&pi->lock);
			res = copyout2((uint64)ioarr[i].iov_base + j, paddr, cnt);
			acquire(&pi->lock);

			if (res < 0)
				goto out1;
			pi->nread += cnt;
			j += cnt;
			ret += cnt;
			ndata -= cnt;
		}
	}
out1:
	wakeup(&pi->nwrite);  //DOC: piperead-wakeup
out2:
	release(&pi->lock);
	releasesleep(&pi->rlock);
	return ret;
}