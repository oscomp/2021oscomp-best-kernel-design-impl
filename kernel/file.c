//
// Support functions for system calls that involve file descriptors.
//

#ifndef __DEBUG_file 
#undef DEBUG 
#endif 

#define __module_name__ 	"file"

#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/spinlock.h"
#include "include/sleeplock.h"
#include "include/fs.h"
#include "include/file.h"
#include "include/pipe.h"
#include "include/stat.h"
#include "include/proc.h"
#include "include/printf.h"
#include "include/string.h"
#include "include/vm.h"
#include "include/kmalloc.h"
#include "include/debug.h"

struct devsw devsw[NDEV];
// struct {
//   struct spinlock lock;
//   struct file file[NFILE];
// } ftable;

// void
// fileinit(void)
// {
//   initlock(&ftable.lock, "ftable");
//   struct file *f;
//   for(f = ftable.file; f < ftable.file + NFILE; f++){
//     memset(f, 0, sizeof(struct file));
//   }
//   #ifdef DEBUG
//   printf("fileinit\n");
//   #endif
// }

// Allocate a file structure.
struct file*
filealloc(void)
{
  struct file *f;
  f = (struct file *)kmalloc(sizeof(struct file));
  if (f == NULL) {
    return NULL;
  }
  initlock(&f->lock, "f->lock");

  f->ref = 1;
  return f;
}
// struct file*
// filealloc(void)
// {
//   struct file *f;

//   acquire(&ftable.lock);
//   for(f = ftable.file; f < ftable.file + NFILE; f++){
//     if(f->ref == 0){
//       f->ref = 1;
//       release(&ftable.lock);
//       return f;
//     }
//   }
//   release(&ftable.lock);
//   return NULL;
// }

// Increment ref count for file f.
struct file*
filedup(struct file *f)
{
  acquire(&f->lock);
  if(f->ref < 1)
    panic("filedup");
  f->ref++;
  release(&f->lock);
  return f;
}

// Close file f.  (Decrement ref count, close when reaches 0.)
void
fileclose(struct file *f)
{
  // struct file ff;

   acquire(&f->lock);
  if(f->ref < 1)
    panic("fileclose");
  if(--f->ref > 0){
	 release(&f->lock);
    return;
  }
  // ff = *f;
  // f->ref = 0;
  // f->type = FD_NONE;
  // release(&ftable.lock);

  // NOT SURE HERE!
  release(&f->lock);
  if(f->type == FD_PIPE){
    pipeclose(f->pipe, f->writable);
  } else if (f->type == FD_INODE) {
    iput(f->ip);
  } else if (f->type == FD_DEVICE) {

  }
  kfree(f);
}

// Get metadata about file f.
// addr is a user virtual address, pointing to a struct stat.
int
filestat(struct file *f, uint64 addr)
{
  // struct proc *p = myproc();
  struct kstat st;
  
  if(f->type == FD_INODE){
    ilock(f->ip);
    f->ip->op->getattr(f->ip, &st);
    iunlock(f->ip);
    // if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    if(copyout2(addr, (char *)&st, sizeof(st)) < 0)
      return -1;
    return 0;
  }
  return -1;
}

// Read from file f.
// addr is a user virtual address.
int
fileread(struct file *f, uint64 addr, int n)
{
  int r = 0;
  struct inode *ip;

  if(f->readable == 0)
    return -1;

  switch (f->type) {
    case FD_PIPE:
        r = piperead(f->pipe, addr, n);
        break;
    case FD_DEVICE:
        if(f->major < 0 || f->major >= NDEV || !devsw[f->major].read)
          return -1;
        r = devsw[f->major].read(1, addr, n);
        break;
    case FD_INODE:
        ip = f->ip;
        ilock(ip);
          if((r = ip->fop->read(ip, 1, addr, f->off, n)) > 0)
            f->off += r;
        iunlock(ip);
        break;
    default:
      panic("fileread");
  }

  return r;
}

// Write to file f.
// addr is a user virtual address.
int
filewrite(struct file *f, uint64 addr, int n)
{
  int ret = 0;

  if(f->writable == 0)
    return -1;

  if(f->type == FD_PIPE){
    ret = pipewrite(f->pipe, addr, n);
  } else if(f->type == FD_DEVICE){
    if(f->major < 0 || f->major >= NDEV || !devsw[f->major].write)
      return -1;
    ret = devsw[f->major].write(1, addr, n);
  } else if(f->type == FD_INODE){
    struct inode *ip = f->ip;
    ilock(ip);
    if (ip->fop->write(ip, 1, addr, f->off, n) == n) {
      ret = n;
      f->off += n;
    } else {
      ret = -1;
    }
    iunlock(ip);
  } else {
    panic("filewrite");
  }

  return ret;
}

// Read from dir f.
// addr is a user virtual address.
int
filereaddir(struct file *f, uint64 addr, uint64 n)
{
  // struct proc *p = myproc();
  if (f->type != FD_INODE || f->readable == 0)
    return -1;

  struct dirent dent;
  struct inode *ip = f->ip;

  if(!(ip->mode & I_MODE_DIR))
    return -1;

  int ret;
  uint64 old = addr;
  ilock(ip);
  for (;;) {
    ret = ip->fop->readdir(ip, &dent, f->off);
    if (ret <= 0 || dent.reclen > n) // 0 is end, -1 is err
      break;

    if(copyout2(addr, (char *)&dent, dent.reclen) < 0) {
      iunlock(ip);
      return -1;
    }

    f->off += ret;
    addr += dent.reclen;
    n -= dent.reclen;

  }
  iunlock(ip);

  return addr - old;
}



/**
 * File descriptors operations.
 * 
 */

// Fork calls this.
int copyfdtable(struct fdtable *fdt1, struct fdtable *fdt2)
{
  struct fdtable *fd = fdt2;

  for (;;) {
    fd->basefd = fdt1->basefd;
    fd->nextfd = fdt1->nextfd;
    fd->used = fdt1->used;
    fd->exec_close = fdt1->exec_close;
    for (int i = 0; i < NOFILE; i++) {
      if (fdt1->arr[i]) {
        fdt2->arr[i] = filedup(fdt1->arr[i]);
      } else {
        fdt2->arr[i] = NULL;
      }
    }
    fdt1 = fdt1->next;
    if (fdt1) {
      if ((fd->next = kmalloc(sizeof(struct fdtable))) == NULL) {
        goto bad;
      }
      __debug_info("copyfdtable", "alloc\n");
      fd = fd->next;
    } else {
      fd->next = NULL;
      break;
    }
  }
  return 0;
bad:
  dropfdtable(fdt2);
  return -1;
}

void dropfdtable(struct fdtable *fdt)
{
  for (int i = 0; i < NOFILE; i++) {
    if (fdt->arr[i]) {
      fileclose(fdt->arr[i]);
    }
  }
  if (fdt->next) {
    dropfdtable(fdt->next);
    kfree(fdt->next);
      __debug_info("dropfdtable", "free\n");
  }
}

struct file *fd2file(int fd, int free)
{
  if (fd < 0)
    panic("fd2file");

  struct proc *p = myproc();
  struct fdtable *head = &p->fds, *prev = NULL;
  struct file *f = NULL;

  while (head && fd >= head->basefd + NOFILE) {
    prev = head;
    head = head->next;
  }
  if (!head || fd < head->basefd) // not found
    return NULL;

  fd %= NOFILE;
  f = head->arr[fd];
  if (free) {
    head->arr[fd] = NULL; // the file should be closed by caller
    if (fd < head->nextfd) {  // make nextfd the smallest
      head->nextfd = fd;
    }
    if (--head->used == 0 && prev) {  // if table is empty and is not the head, free it
      prev->next = head->next;
      kfree(head);
      __debug_info("fd2file", "free %p\n", head);
    }
  }
  return f;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
int fdalloc(struct file *f)
{
  int fd = 0;
  struct proc *p = myproc();
  struct fdtable *fdt = &p->fds;
  
  while (fdt->nextfd == NOFILE) { // table full
    if (!fdt->next ||                               // no next table
        fdt->basefd + NOFILE != fdt->next->basefd)  // or next table is not continuous
    {
      struct fdtable *fdnew = kmalloc(sizeof(struct fdtable));
      if (fdnew == NULL) {
        return -1;
      }
      __debug_info("fdalloc", "alloc %p\n", fdnew);
      fdnew->basefd = fdt->basefd + NOFILE;
      fdnew->exec_close = 0;
      fdnew->nextfd = 0;
      fdnew->used = 0;
      memset(fdnew->arr, 0, sizeof(fdnew->arr));
      fdnew->next = fdt->next;
      fdt->next = fdnew;
    }
    fdt = fdt->next;
  }

  fd = fdt->nextfd;
  fdt->arr[fd] = f;
  fdt->used++;
  // locate the next smallest free fd
  // if not found, nextfd will stop at NOFILE, which means this table is full
  while (++fdt->nextfd < NOFILE) {
    if (fdt->arr[fdt->nextfd] == NULL) {
      break;
    }
  }

  return fd + fdt->basefd;
}

int fdalloc3(struct file *f, int fd, int flag)
{
  if (fd < 0)
    panic("fdalloc3");

  __debug_info("fdalloc3", "in fd=%d flag=%d\n", fd, flag);
  struct proc *p = myproc();
  struct fdtable *fdt = &p->fds, *prev = NULL;

  while (fdt && fdt->basefd + NOFILE <= fd) {
    prev = fdt;
    fdt = fdt->next;
  }
  if (!fdt || fdt->basefd > fd) {  // no next table, or next table is not continuous
    struct fdtable *fdnew = kmalloc(sizeof(struct fdtable));
    if (fdnew == NULL) {
      return -1;
    }
    fdnew->basefd = fd - fd % NOFILE;
    fdnew->used = 0;
    fdnew->nextfd = 0;
    fdnew->exec_close = 0;
    memset(fdnew->arr, 0, sizeof(fdnew->arr));
    fdnew->next = fdt;
    prev->next = fdnew;
    fdt = fdnew;
    __debug_info("fdalloc3", "alloc %p base=%d\n",
                  fdnew, fdnew->basefd);
  }

  int fd2 = fd % NOFILE;
  if (fdt->arr[fd2] != NULL) {
    fileclose(fdt->arr[fd2]);
    fdt->arr[fd2] = f;
  } else {
    fdt->arr[fd2] = f;
    fdt->used++;
    if (fd2 == fdt->nextfd) { // unfortunately, we need to update nextfd
      while (++fdt->nextfd < NOFILE) {
        if (fdt->arr[fdt->nextfd] == NULL) {
          break;
        }
      }
    }
  }
  __debug_info("fdalloc3", "table %p base=%d, nextfd=%d, fd2=%d, fd=%d\n",
                fdt, fdt->basefd, fdt->nextfd, fd2, fd);

  if (flag)
    fdt->exec_close |= (1 << fd2);

  return fd;
}
