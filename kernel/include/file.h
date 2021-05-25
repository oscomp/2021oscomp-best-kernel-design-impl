#ifndef __FILE_H
#define __FILE_H

#include "spinlock.h"
#include "param.h"

struct file {
  struct spinlock lock;
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;
  uint off;          // FD_ENTRY
  short major;       // FD_DEVICE
};

// #define major(dev)  ((dev) >> 16 & 0xFFFF)
// #define minor(dev)  ((dev) & 0xFFFF)
// #define	mkdev(m,n)  ((uint)((m)<<16| (n)))

// map major device number to device functions.
struct devsw {
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];

struct fdtable {
  uint16 basefd;
  uint16 nextfd;
  uint16 used;
  uint16 exec_close;
  struct file *arr[NOFILE];
  struct fdtable *next;
};

#define CONSOLE 1

struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
// void            fileinit(void);
int             fileread(struct file*, uint64, int n);
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);
int             filereaddir(struct file *f, uint64 addr, uint64 n);

int             copyfdtable(struct fdtable *fdt1, struct fdtable *fdt2);
void            dropfdtable(struct fdtable *fdt);
struct file*    fd2file(int fd, int free);
int             fdalloc(struct file *f);
int             fdalloc3(struct file *f, int fd, int flag);

#endif
