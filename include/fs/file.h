#ifndef __FILE_H
#define __FILE_H

#include "sync/spinlock.h"
#include "param.h"
#include "types.h"
#include "fs/poll.h"

typedef enum {
	FD_NONE,
	FD_PIPE,
	FD_INODE,
	FD_DEVICE,
} file_type_e;

struct poll_table;

struct file {
	struct spinlock	lock;
	file_type_e		type;
	int				ref;		// reference count
	char			readable;
	char			writable;
	short			major;		// FD_DEVICE
	uint			off;		// FD_ENTRY
	struct pipe		*pipe;		// FD_PIPE
	struct inode	*ip;
	uint32 (*poll)(struct file *, struct poll_table *);
};


struct fdtable {
	uint16 basefd;
	uint16 nextfd;
	uint16 used;
	uint16 exec_close;
	struct file *arr[NOFILE];
	struct fdtable *next;
};

#define CONSOLE 1

struct iovec {
	void *iov_base; /* Starting address */
	uint64 iov_len; /* Number of bytes to transfer */
};

struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
// void            fileinit(void);
int             fileread(struct file*, uint64, int n);
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);
int             filereaddir(struct file *f, uint64 addr, uint64 n);
int             filereadv(struct file *f, struct iovec ioarr[], int count);
int             filewritev(struct file *f, struct iovec ioarr[], int count);

int             copyfdtable(struct fdtable *fdt1, struct fdtable *fdt2);
void            dropfdtable(struct fdtable *fdt);
struct file*    fd2file(int fd, int free);
int             fdalloc(struct file *f, int flag);
int             fdalloc3(struct file *f, int fd, int flag);
void            fdcloexec(struct fdtable *fdt);
int             fcntldup(struct file *f, int minfd, int cloexec);

#endif
