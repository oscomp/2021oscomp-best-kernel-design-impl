#ifndef __DEBUG_sysmem
#undef DEBUG
#endif

#define __module_name__ "sysmem"

#include "types.h"
#include "hal/riscv.h"
#include "param.h"
#include "errno.h"
#include "syscall.h"
#include "sched/proc.h"
#include "mm/vm.h"
#include "mm/usrmm.h"
#include "mm/mmap.h"
#include "printf.h"
#include "utils/debug.h"


uint64
sys_sbrk(void)
{
	int n;
	if(argint(0, &n) < 0)
		return -1;
	
	struct proc *p = myproc();
	uint64 addr = p->pbrk;

	if (growproc(addr + n) < 0)
		return -1;

	return addr;
}

uint64
sys_brk(void)
{
	uint64 addr;
	if(argaddr(0, &addr) < 0)
		return -1;
	
	struct proc *p = myproc();
	if (addr == 0)
		return p->pbrk;

	uint64 old = p->pbrk;
	if (growproc(addr) < 0)
		return old;

	return addr;
}

uint64
sys_mprotect(void)
{
	uint64 addr, len;
	int prot;

	argaddr(0, &addr);
	argaddr(1, &len);
	argint(2, &prot);
	if (addr % PGSIZE || (prot & ~PROT_ALL))
		return -EINVAL;

	prot = (prot << 1) & (PTE_X|PTE_W|PTE_R); // convert to PTE_attr
	struct proc *p = myproc();
	struct seg *s = partofseg(p->segment, addr, addr + len);
	if (s == NULL ||
		(s->type == MMAP && !(s->flag & PTE_W) && (prot & PTE_W))
	)
	return -EINVAL;

	return uvmprotect(p->pagetable, addr, len, prot);
}

extern int argfd(int n, int *pfd, struct file **pf);

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
		if (off != 0)
			return -EINVAL;
		f = NULL;
	} else if (f->type != FD_INODE) {
		return -EPERM;
	}

	// Must provide one of them.
	if (!(flags & (MAP_SHARED|MAP_PRIVATE))) {
		return -EINVAL;
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
