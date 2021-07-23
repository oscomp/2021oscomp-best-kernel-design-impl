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

	acquire(&pi->lock);
	for (i = 0; i < n;) {
		if ((m = pipewritable(pi)) < 0) {
			release(&pi->lock);
			return -1;
		}
		m = (PIPESIZE - m < n - i) ? PIPESIZE - m : n - i;		// amount of bytes to write
		while (m > 0) {							// pipe is a loop in a buf
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
	wakeup(&pi->nread);
	release(&pi->lock);
	return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
	int i, m;
	char const *pipebound = pi->data + PIPESIZE;

	acquire(&pi->lock);
	if ((m = pipereadable(pi)) < 0) {
		release(&pi->lock);
		return -1;
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
	wakeup(&pi->nwrite);  //DOC: piperead-wakeup
	release(&pi->lock);
	return i;
}

int pipewritev(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ret = 0;
	char const *pipebound = pi->data + PIPESIZE;

	acquire(&pi->lock);
	for (int i = 0; i < count; i++) {
		uint64 n = ioarr[i].iov_len;
		int j;
		for (j = 0; j < n;) {
			int m = pipewritable(pi);
			if (m < 0) {
				release(&pi->lock);
				return -EPIPE;
			}
			m = (PIPESIZE - m < n - j) ? PIPESIZE - m : n - j;		// amount of bytes to write
			while (m > 0) {							// pipe is a loop in a buf
				char *paddr = pi->data + pi->nwrite % PIPESIZE;
				int cnt = (pipebound - paddr < m) ? pipebound - paddr : m;
				if (copyin2(paddr, (uint64)ioarr[i].iov_base + j, cnt) < 0)
					break;
				m -= cnt;
				j += cnt;
				pi->nwrite += cnt;
				ret += cnt;
			}
			if (m > 0)
				break;
		}
		if (j < n)
			break;
	}
	wakeup(&pi->nread);
	release(&pi->lock);
	return ret;
}

int pipereadv(struct pipe *pi, struct iovec ioarr[], int count)
{
	int ndata, ret = 0;
	char const *pipebound = pi->data + PIPESIZE;

	acquire(&pi->lock);
	if ((ndata = pipereadable(pi)) < 0) {
		release(&pi->lock);
		return -EPIPE;
	}
	for (int i = 0; i < count && ndata > 0; i++) {
		uint64 n = ioarr[i].iov_len;
		int m = n < ndata ? n : ndata;
		int j;
		for (j = 0; j < m;) {
			char *paddr = pi->data + pi->nread % PIPESIZE;
			int cnt = (pipebound - paddr < m - j) ? pipebound - paddr : m - j;
			if (copyout2((uint64)ioarr[i].iov_base + j, paddr, cnt) < 0)
				break;
			pi->nread += cnt;
			j += cnt;
			ret += cnt;
			ndata -= cnt;
		}
		if (j < m)
			break;
	}
	wakeup(&pi->nwrite);  //DOC: piperead-wakeup
	release(&pi->lock);
	return ret;
}