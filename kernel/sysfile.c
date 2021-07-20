//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#ifndef __DEBUG_sysfile
#undef DEBUG
#endif

#define __module_name__ 	"sysfile"

#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/stat.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/sleeplock.h"
#include "include/file.h"
#include "include/pipe.h"
#include "include/fcntl.h"
#include "include/fs.h"
#include "include/syscall.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/vm.h"
#include "include/debug.h"
#include "include/poll.h"
#include "include/kmalloc.h"
#include "include/errno.h"

// Fetch the nth word-sized system call argument as a file descriptor
// and return both the descriptor and the corresponding struct file.
static int
argfd(int n, int *pfd, struct file **pf)
{
	int fd;
	struct file *f;

	if(argint(n, &fd) < 0)
		return -1;
	if(pfd)
		*pfd = fd;
	if(fd < 0)
		return -1;
	if ((f = fd2file(fd, 0)) == NULL)
		return -1;
	if(pf)
		*pf = f;
	return 0;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
// static int
// fdalloc(struct file *f)
// {
//   int fd;
//   struct proc *p = myproc();

//   for(fd = 0; fd < NOFILE; fd++){
//     if(p->ofile[fd] == 0){
//       p->ofile[fd] = f;
//       return fd;
//     }
//   }
//   return -1;
// }

uint64
sys_dup(void)
{
	struct file *f;
	int fd;

	if(argfd(0, 0, &f) < 0)
		return -1;
	if((fd=fdalloc(f, 0)) < 0)
		return -1;
	filedup(f);
	return fd;
}

uint64
sys_dup3(void)
{
	struct file *f;
	int old, new, flag;

	if (argfd(0, &old, &f) < 0 || argint(1, &new) < 0 || argint(2, &flag) < 0)
		return -1;
	if (old == new || new < 0)
		return -1;

	if (fdalloc3(f, new, flag) < 0)
		return -1;
	
	filedup(f);
	return new;
}

uint64
sys_read(void)
{
	struct file *f;
	int n;
	uint64 p;

	if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0 || argint(2, &n) < 0)
		return -1;
	return fileread(f, p, n);
}

uint64
sys_write(void)
{
	struct file *f;
	int n;
	uint64 p;

	if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0 || argint(2, &n) < 0)
		return -1;

	return filewrite(f, p, n);
}

uint64
sys_close(void)
{
	int fd;
	struct file *f;

	if(argfd(0, &fd, &f) < 0)
		return -1;
	f = fd2file(fd, 1);
	fileclose(f);
	return 0;
}

uint64
sys_fstat(void)
{
	struct file *f;
	uint64 st; // user pointer to struct stat

	if(argfd(0, 0, &f) < 0 || argaddr(1, &st) < 0)
		return -1;
	return filestat(f, st);
}

uint64
sys_fstatat(void)
{
	int fd;
	struct file *f;
	struct inode *ip, *dp;
	char path[MAXPATH];
	uint64 staddr; // user pointer to struct stat
	int flag;

	if (argfd(0, &fd, &f) < 0) {
		if (fd != AT_FDCWD)
			return -EBADF;
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
	}

	if (argstr(1, path, MAXPATH) < 0 || argaddr(2, &staddr) < 0 || argint(3, &flag) < 0)
		return -ENAMETOOLONG;
	if ((ip = nameifrom(dp, path)) == NULL)
		return -ENOENT;

	struct kstat st;
	ilock(ip);
	ip->op->getattr(ip, &st);
	iunlockput(ip);
	
	if (copyout2(staddr, (char *)&st, sizeof(st)) < 0)
		return -EFAULT;

	return 0;
}

uint64
sys_openat(void)
{
	char path[MAXPATH];
	int dirfd, fd, omode, fmode;
	struct file *f = NULL;
	struct inode *dp = NULL, *ip;

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -EBADF;
		}
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
	}
	if (argstr(1, path, MAXPATH) < 0 || argint(2, &omode) < 0 || argint(3, &fmode) < 0)
		return -ENAMETOOLONG;

	if(omode & O_CREATE){
		ip = create(dp, path, fmode & (~I_MODE_DIR));
		if(ip == NULL){
			return -1;
		}
	} else {
		if ((ip = nameifrom(dp, path)) == NULL) {
			__debug_warn("sys_openat", "can find %s\n", path);
			return -ENOENT;
		}
		ilock(ip);
		if ((ip->mode & I_MODE_DIR) && (omode & (O_WRONLY|O_RDWR))) {
			iunlockput(ip);
			__debug_warn("sys_openat", "illegel mode 0x%x\n", omode);
			return -EISDIR;
		}
		if ((omode & O_DIRECTORY) && !(ip->mode & I_MODE_DIR)) {
			iunlockput(ip);
			__debug_warn("sys_openat", "o_dir\n");
			return -ENOTDIR;
		}
	}

	if((f = filealloc()) == NULL || (fd = fdalloc(f, omode & O_CLOEXEC)) < 0){
		if (f) {
			fileclose(f);
		}
		iunlockput(ip);
		return -ENOMEM;
	}

	if (!(ip->mode & I_MODE_DIR) && (omode & O_TRUNC) && (omode & (O_WRONLY|O_RDWR))) {
		ip->op->truncate(ip);
	}

	if (ip->dev) {
		f->type = FD_DEVICE;
	} else {
		f->type = FD_INODE;
		f->off = (omode & O_APPEND) ? ip->size : 0;
	}

	f->ip = ip;
	f->readable = !(omode & O_WRONLY);
	f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

	iunlock(ip);

	return fd;
}

uint64
sys_mkdirat(void)
{
	char path[MAXPATH];
	int dirfd, mode;
	struct file *f = NULL;
	struct inode *dp = NULL, *ip;

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -EBADF;
		}
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
	}
	if (argstr(1, path, MAXPATH) < 0 || argint(2, &mode) < 0) {
		return -ENAMETOOLONG;
	}
	__debug_info("mkdirat", "create dir %s\n", path);
	if ((ip = create(dp, path, mode | I_MODE_DIR)) == NULL) {
		__debug_warn("mkdirat", "create fail\n");
		return -1;
	}
	iunlockput(ip);
	return 0;
}

uint64
sys_chdir(void)
{
	char path[MAXPATH];
	struct inode *ip;
	struct proc *p = myproc();
	
	if(argstr(0, path, MAXPATH) < 0 || (ip = namei(path)) == NULL){
		return -1;
	}
	if (!(ip->mode & I_MODE_DIR)) {
		iput(ip);
		return -1;
	}
	iput(p->cwd);
	p->cwd = ip;
	return 0;
}

uint64
sys_pipe(void)
{
	uint64 fdarray; // user pointer to array of two integers
	struct file *rf, *wf;
	int fd0, fd1;
	int flags;
	// struct proc *p = myproc();

	if(argaddr(0, &fdarray) < 0)
		return -1;
	if (argint(1, &flags) < 0)
		return -1;
	if(pipealloc(&rf, &wf) < 0)
		return -ENOMEM;
	fd0 = -1;
	if ((fd0 = fdalloc(rf, 0)) < 0 || (fd1 = fdalloc(wf, 0)) < 0) {
		if(fd0 >= 0)
			fd2file(fd0, 1);
		fileclose(rf);
		fileclose(wf);
		return -ENOMEM;
	}
	// if(copyout(p->pagetable, fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
	//    copyout(p->pagetable, fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0){
	if (copyout2(fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
		copyout2(fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0)
	{
		fd2file(fd0, 1);
		fd2file(fd1, 1);
		fileclose(rf);
		fileclose(wf);
		return -EFAULT;
	}
	return 0;
}

// To support ls command
uint64
sys_getdents(void)
{
	struct file *f;
	uint64 p, len;

	if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0 || argint(2, (int*)&len) < 0)
		return -1;
	return filereaddir(f, p, len);
}

// get absolute cwd string
uint64
sys_getcwd(void)
{
	uint64 addr;
	int size;
	if (argaddr(0, &addr) < 0 || argint(1, (int*)&size) < 0)
		return -1;

	if (size < 2)
		return -1;

	char buf[MAXPATH];

	int max = MAXPATH < size ? MAXPATH : size;
	if ((size = namepath(myproc()->cwd, buf, max)) < 0)
		return -1;

	if (copyout2(addr, buf, size) < 0)
		return -1;

	return size;
}

uint64
sys_unlinkat(void)
{
	char path[MAXPATH];
	int dirfd, mode;
	struct file *f = NULL;
	struct inode *dp = NULL, *ip;

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -EBADF;
		}
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
		if (!(dp->mode & I_MODE_DIR)) {
			return -ENOTDIR;
		}
	}

	int len;
	if((len = argstr(1, path, MAXPATH)) <= 0 || argint(2, &mode) < 0)
		return -ENAMETOOLONG;

	char *s = path + len - 1;
	while (s >= path && *s == '/') {
		s--;
	}
	if (s >= path && *s == '.' && (s == path || *--s == '/')) {
		__debug_warn("sys_unlinkat", "illegel path %s\n", path);
		return -1;
	}
	
	if((ip = nameifrom(dp, path)) == NULL){
		__debug_warn("sys_unlinkat", "can namei %s\n", path);
		return -1;
	}
	if ((ip->mode & I_MODE_DIR) && mode != AT_REMOVEDIR) {
		iput(ip);
		__debug_warn("sys_unlinkat", "illegel mode 0x%x against 0x%x\n", mode, ip->mode);
		return -EISDIR;
	} else if (!(ip->mode & I_MODE_DIR) && mode == AT_REMOVEDIR) {
		iput(ip);
		return -ENOTDIR;
	}
	ilock(ip);
	if ((ip->mode & I_MODE_DIR) && isdirempty(ip) != 1) {
		iunlockput(ip);
		__debug_warn("sys_unlinkat", "dir isn't empty\n");
		return -1;
	}
	int ret = unlink(ip);
	iunlockput(ip);

	return ret;
}

// Must hold too many locks at a time! It's possible to raise a deadlock.
// Because this op takes some steps, we can't promise
uint64
sys_renameat(void)
{
	int fd1, fd2, flags;
	struct file *f1, *f2;
	char name1[MAXPATH], name2[MAXPATH];
	struct inode *ip1, *ip2;
	struct proc *p = myproc();

	if (argfd(0, &fd1, &f1) < 0) {
		if (fd1 != AT_FDCWD) return -EBADF;
		ip1 = p->cwd;
	} else
		ip1 = f1->ip;
	if (argfd(2, &fd2, &f2) < 0) {
		if (fd2 != AT_FDCWD) return -EBADF;
		ip2 = p->cwd;
	} else
		ip2 = f2->ip;

	if (!(ip1->mode & ip2->mode & I_MODE_DIR))
		return -ENOTDIR;

	if (argstr(1, name1, MAXPATH) < 0 || argstr(3, name2, MAXPATH) < 0 || argint(4, &flags))
		return -ENAMETOOLONG;

	int ret = 0;
	if ((ip1 = nameifrom(ip1, name1)) == NULL ||
		(ip2 = nameiparentfrom(ip2, name2, name1)) == NULL) // name1 stores the new name
	{
		if (ip1)
			iput(ip1);
		return -ENOENT;
	}
	
	ret = do_rename(ip1, ip2, name1);

	iput(ip1);
	iput(ip2);
	return ret;
}

uint64
sys_mount(void)
{
	char buf[MAXPATH];
	struct inode *dev = NULL, *imnt = NULL;
	uint64 flag;

	if (argstr(0, buf, MAXPATH) < 0 || (dev = namei(buf)) == NULL) {
		return -1;
	}
	if (argstr(1, buf, MAXPATH) < 0 || (imnt = namei(buf)) == NULL) {
		goto fail;
	}
	// Type of fs to mount and flag.
	if (argstr(2, buf, MAXPATH) < 0 || argint(3, (int*)&flag) < 0) {
		goto fail;
	}

	// We don't plan to support this
	// char data[512];
	// if (argstr(4, data, sizeof(data)) < 0) {
	//   goto fail;
	// }

	ilock(dev);
	ilock(imnt);
	if (do_mount(dev, imnt, buf, flag, NULL) < 0) {
		iunlock(imnt);
		iunlock(dev);
		goto fail;
	}
	iunlockput(imnt);
	iunlockput(dev);

	return 0;

fail:
	if (dev) {
		iput(dev);
	}
	if (imnt) {
		iput(imnt);
	}

	return -1;
}

uint64
sys_umount(void)
{
	char buf[MAXPATH];
	struct inode *mnt = NULL;
	uint64 flag;

	if (argstr(0, buf, MAXPATH) < 0 || (mnt = namei(buf)) == NULL) {
		return -1;
	}
	if (argint(1, (int*)&flag) < 0 ||
			mnt->ref > 1) // Is there anyone else holding this inode?
	{                 // If a syscall try to umount the same mntpoint, he won't pass this.
		iput(mnt);
		return -1;
	}

	ilock(mnt);
	if (do_umount(mnt, flag) < 0) {
		iunlockput(mnt);
		return -1;
	}

	// If umount successfully, mnt is no longer available,
	// we shouldn't unlock it, or put it.

	return 0;
}

uint64
sys_mmap(void)
{
	uint64 start, len;
	int prot, flags, fd;
	int64 off;
	struct file *f = NULL;

	argaddr(0, &start);
	argaddr(1, &len);
	argint(2, &prot);
	argint(3, &flags);
	argfd(4, &fd, &f);
	argaddr(5, (uint64*)&off);
	
	if (off % PGSIZE || len == 0)
		return -EINVAL;
	
	if ((fd < 0 || f == NULL) && !(flags & MAP_ANONYMOUS)) {
		return -EBADF;
	} else if (flags & MAP_ANONYMOUS) {
		f = NULL;
	} else if (f->type != FD_INODE) {
		return -EPERM;
	}

	return do_mmap(start, len, prot, flags, f, off);
}

uint64
sys_munmap(void)
{
	uint64 start, len;
	if (argaddr(0, &start) < 0 || argaddr(1, &len) < 0) {
		return -1;
	}

	if (start % PGSIZE || len == 0) {
		__debug_info("sys_munmap", "start=%p not aligned\n", start);
		return -EINVAL;
	}

	return do_munmap(start, len);
}

uint64
sys_readv(void)
{
	int fd, count;
	uint64 iovp;
	struct file *f = NULL;
	if (argfd(0, &fd, &f) < 0 || argaddr(1, &iovp) < 0 || argint(2, &count) < 0)
		return -EBADF;

	struct iovec *ioarr = kmalloc(count * sizeof(struct iovec));
	if (ioarr == NULL)
		return -ENOMEM;

	if (copyin2((char *)ioarr, iovp, count * sizeof(struct iovec)) < 0) {
		kfree(ioarr);
		return -EFAULT;
	}

	int ret = filereadv(f, ioarr, count);
	
	kfree(ioarr);
	return ret;
}

uint64
sys_writev(void)
{
	int fd, count;
	uint64 iovp;
	struct file *f = NULL;
	if (argfd(0, &fd, &f) < 0 || argaddr(1, &iovp) < 0 || argint(2, &count) < 0)
		return -EBADF;

	struct iovec *ioarr = kmalloc(count * sizeof(struct iovec));
	if (ioarr == NULL)
		return -ENOMEM;

	if (copyin2((char *)ioarr, iovp, count * sizeof(struct iovec)) < 0) {
		kfree(ioarr);
		return -EFAULT;
	}

	int ret = filewritev(f, ioarr, count);
	
	kfree(ioarr);
	return ret;
}

uint64
sys_readlinkat(void)
{
	int dirfd;
	uint64 ubuf, size;
	char path[MAXPATH];
	struct file *f;
	struct inode *dp, *ip;

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -EBADF;
		}
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
	}

	if (argstr(1, path, MAXPATH) < 0 || argaddr(2, &ubuf) < 0 || argaddr(3, &size) < 0)
		return -ENAMETOOLONG;

	__debug_info("sys_readlinkat", "path=%s\n", path);

	ip = nameifrom(dp, path);
	if (ip == NULL)
		return -ENOENT;

	int len;
	len = namepath(ip, path, MAXPATH);
	iput(ip);

	if (len > size)
		len = size;
	if (copyout2(ubuf, path, len) < 0)
		return -EFAULT;

	return len;
}

uint64
sys_fcntl(void)
{
	int fd, cmd;
	struct file *f;
	uint64 arg;
	if (argfd(0, &fd, &f) < 0 || argint(1, &cmd) < 0 || argaddr(2, &arg) < 0)
		return -1;

	if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC) {
		int minfd = (int) arg;
		if (minfd < 0)
			return -1;
		minfd = fcntldup(f, minfd, cmd == F_DUPFD_CLOEXEC);
		if (minfd >= 0)
			filedup(f);
		return minfd;
	}

	__debug_error("sys_fcntl", "unsupported cmd %d\n", cmd);

	return -1;
}

uint64
sys_ppoll(void)
{
	uint64 addr;
	uint32 nfds;
	uint64 timeoutaddr;
	uint64 sigmaskaddr;
	if (argaddr(0, &addr) < 0 || argint(1, (int*)&nfds) < 0 || argaddr(2, &timeoutaddr) < 0 || argaddr(3, &sigmaskaddr) < 0)
		return -1;

	if (timeoutaddr || sigmaskaddr) {
		__debug_error("sys_ppoll", "not supported yet!\n");
		return -1;
	}
	if (nfds < 0) return -1;

	struct pollfd *pfds = kmalloc(nfds * sizeof(struct pollfd));
	if (pfds == NULL) return -1;

	for (int i = 0; i < nfds; i++) {
		if (copyin2((char *)(pfds + i), addr + i * sizeof(struct pollfd), sizeof(struct pollfd)) < 0) {
			kfree(pfds);
			return -1;
		}
	}

	int ret = do_ppoll(pfds, nfds); // should accept more arguments
	copyout2(addr, (char *)pfds, sizeof(struct pollfd) * nfds);
	kfree(pfds);
	return ret;
}

#define R_OK	4		/* Test for read permission.  */
#define W_OK	2		/* Test for write permission.  */
#define X_OK	1		/* Test for execute permission.  */
#define F_OK	0		/* Test for existence.  */              

uint64
sys_faccessat(void)
{
	char path[MAXPATH];
	int dirfd, mode, flags;
	struct file *f = NULL;
	struct inode *dp, *ip;

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -1;
		}
		dp = myproc()->cwd;
	} else {
		dp = f->ip;
	}
	if (argstr(1, path, MAXPATH) < 0 || argint(2, &mode) < 0 || argint(3, &flags) < 0)
		return -1;

	ip = nameifrom(dp, path);
	if (ip != NULL) {
		if (mode == F_OK) {
			iput(ip);
			return 0;
		}
	} else return -1;

	// assume user as root
	int imode = (ip->mode >> 6) & 0x7;
	iput(ip);

	if ((imode & mode) != mode)
		return -1;

	return 0;
}

#define SEEK_SET	0	/* Seek from beginning of file.  */                                                                                      
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

uint64
sys_lseek(void)
{
	int fd;
	struct file *f;
	int64 off;
	int whence;

	if (argfd(0, &fd, &f) < 0)
		return -EBADF;
	argaddr(1, (uint64 *)&off);
	argint(2, &whence);

	if (f->type != FD_INODE)
		return -ESPIPE;

	uint64 cur = f->off;	
	uint64 size = f->ip->size;
	switch (whence)
	{
		case SEEK_SET:
			cur = off;
			break;
		case SEEK_CUR:
			cur += off;
			break;
		case SEEK_END:
			cur = size + off;
			break;
		default:
			return -EINVAL;
	}

	// Doesn't support offset out of size
	if (cur > 0xffffffff)
		return -EFBIG;

	acquire(&f->lock);
	f->off = cur;
	release(&f->lock);

	return cur;
}

uint64
sys_utimensat(void)
{
	char path[MAXPATH];
	int dirfd, flags;
	uint64 addr;
	struct file *f = NULL;
	struct inode *ip;
	struct timespec times[2];

	if (argfd(0, &dirfd, &f) < 0) {
		if (dirfd != AT_FDCWD) {
			return -EBADF;
		}
		ip = myproc()->cwd;
	} else {
		ip = f->ip;
	}
	if (argstr(1, path, MAXPATH) < 0 || argaddr(2, &addr) < 0 || argint(3, &flags) < 0)
		return -ENAMETOOLONG;

	if (addr != NULL && copyin2((char *)times, addr, sizeof(times)) < 0)
		return -EFAULT;
	else if (addr == NULL) {
		// memset(times, 0, sizeof(times));
	
	}

	ip = nameifrom(ip, path);
	if (ip == NULL)
		return -ENOENT;

	int ret = 0;
	struct kstat st;
	memset(&st, 0, sizeof(st));

	ilock(ip);
	ip->op->getattr(ip, &st);
	st.atime_sec = times[0].sec;
	st.atime_nsec = times[0].nsec;
	st.mtime_sec = times[1].sec;
	st.mtime_nsec = times[1].nsec;
	if (!ip->op->setattr || ip->op->setattr(ip, &st) < 0) {
		ret = -EACCES;
	}
	iunlockput(ip);

	return ret;
}

// Temporarily
uint64
sys_ioctl(void)
{
	#define TIOCGWINSZ	0x5413
	#define TCGETS		0x5401

	struct winsize {
		uint16 ws_row;		/* rows， in character */
		uint16 ws_col; 		/* columns, in characters */
		uint16 ws_xpixel;	/* horizontal size, pixels (unused) */
		uint16 ws_ypixel;	/* vertical size, pixels (unused) */
	};

	#define ICRNL		0000400
	#define OPOST		0000001
	#define ONLCR		0000004
	#define ICANON		0000002
	#define ECHO		0000010


	struct termios {
		uint16 c_iflag; /* 输入模式标志*/
		uint16 c_oflag; /* 输出模式标志*/
		uint16 c_cflag; /* 控制模式标志*/
		uint16 c_lflag; /*区域模式标志或本地模式标志或局部模式*/
		uint8 c_line; /*行控制line discipline */
		uint8 c_cc[8]; /* 控制字符特性*/
	};

	int fd;
	struct file *f;
	uint64 request;
	uint64 argp;

	if (argfd(0, &fd, &f) < 0 || argaddr(1, &request) < 0 || argaddr(2, &argp) < 0)
		return -EBADF;
	
	if (f->type != FD_DEVICE)
		return -EPERM;

	switch (request) {
	case TIOCGWINSZ: {
		struct winsize win = {
			.ws_row = 24,
			.ws_col = 80,
		};
		if (copyout2(argp, (char*)&win, sizeof(win)) < 0)
			return -EFAULT;
		break;
	}
	case TCGETS: {
		struct termios terminfo = {
			.c_iflag = ICRNL,
			.c_oflag = OPOST|ONLCR,
			.c_cflag = 0,
			.c_lflag = ICANON|ECHO,
			.c_line = 0,
			.c_cc = {0},
		};
		if (copyout2(argp, (char*)&terminfo, sizeof(terminfo)) < 0)
			return -EFAULT;
		break;
	}
	default:
		return -EPERM;
	}

	return 0;
}

uint64
sys_statfs(void)
{
	char path[MAXPATH];
	uint64 addr;
	if (argstr(0, path, MAXPATH) < 0 || argaddr(1, &addr) < 0)
		return -ENAMETOOLONG;

	struct inode *ip = namei(path);
	if (ip == NULL)
		return -ENOENT;
	
	struct superblock *sb = ip->sb;
	iput(ip);
	
	struct statfs stat;
	if (!sb->op.statfs)
		return -EPERM;
	if (sb->op.statfs(sb, &stat) < 0)
		return -EIO;
	if (copyout2(addr, (char *)&stat, sizeof(stat)) < 0)
		return -EFAULT;
	
	return 0;
}
