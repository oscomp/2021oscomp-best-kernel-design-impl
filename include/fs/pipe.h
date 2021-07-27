#ifndef __PIPE_H
#define __PIPE_H

#include "types.h"
#include "fs/file.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"

#define PIPESIZE 512

struct pipe {
	struct spinlock		lock;
	struct sleeplock	wlock;
	struct sleeplock	rlock;
	uint	nread;			// number of bytes read
	uint	nwrite;			// number of bytes written
	int		readopen;		// read fd is still open
	int		writeopen;		// write fd is still open
	char	data[PIPESIZE];
};

int pipealloc(struct file **f0, struct file **f1);
void pipeclose(struct pipe *pi, int writable);
int pipewrite(struct pipe *pi, uint64 addr, int n);
int piperead(struct pipe *pi, uint64 addr, int n);
int pipewritev(struct pipe *pi, struct iovec ioarr[], int count);
int pipereadv(struct pipe *pi, struct iovec ioarr[], int count);

#endif