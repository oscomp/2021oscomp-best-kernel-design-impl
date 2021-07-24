#ifndef __DEBUG_exec
#undef  DEBUG
#endif

#define __module_name__ 	"exec"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sched/proc.h"
#include "elf.h"
#include "fs/fs.h"
#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/usrmm.h"
#include "printf.h"
#include "utils/string.h"
#include "syscall.h"
#include "utils/debug.h"
#include "errno.h"

// Load a program segment into pagetable at virtual address va.
// va must be page-aligned (not necessary)
// and the pages from va to va+sz must already be mapped.
// Returns 0 on success, -1 on failure.
/*
static int
loadseg(pagetable_t pagetable, uint64 va, struct inode *ip, uint offset, uint sz)
{
	uint i, n;
	uint64 pa;
	// if((va % PGSIZE) != 0)
	//   panic("loadseg: va must be page aligned");

	for (i = 0; i < sz; i += n) {
		pa = walkaddr(pagetable, va + i);
		if (pa == NULL)
			panic("loadseg: address should exist");

		uint m = (va + i) % PGSIZE; 
		if (sz - i < PGSIZE)
			n = sz - i;
		else
			n = PGSIZE - m;

		// __debug_info("loadseg", "pa=%p, n=%d\n", pa + m, n);
		if(ip->fop->read(ip, 0, pa + m, offset + i, n) != n)
			return -1;
	}

	return 0;
}
*/

// push arg or env strings into user stack, return strings count
static int pushstack(pagetable_t pt, uint64 table[], char **utable, int maxc, uint64 *spp)
{
	uint64 sp = *spp;
	uint64 stackbase = PGROUNDDOWN(sp);
	uint64 argc, argp;
	char *buf;

	if ((buf = allocpage()) == NULL) {
		return -ENOMEM;
	}
	for (argc = 0; utable; argc++) {
		if (argc >= maxc || fetchaddr((uint64)(utable + argc), &argp) < 0)
			goto bad;
		__debug_info("pushstack", "argp=%p\n", argp);
		if (argp == 0)
			break;
		int arglen = fetchstr(argp, buf, PGSIZE);   // '\0' included in PGSIZE, but not in envlen
		__debug_info("pushstack", "arglen = %d\n", arglen);
		if (arglen++ < 0) {                               // including '\0'
			__debug_warn("pushstack", "didn't get null\n");
			goto bad;
		}
		sp -= arglen;
		if (sp < stackbase || copyout(pt, sp, buf, arglen) < 0)
			goto bad;
		table[argc] = sp;
	}
	table[argc] = 0;
	*spp = sp;
	freepage(buf);
	return argc;

bad:
	if (buf)
		freepage(buf);
	return -EFAULT;
}

// All argvs are pointers came from user space, and should be checked by sys_caller
int execve(char *path, char **argv, char **envp)
{
	__debug_info("exeve", "path = %s\n", path);
	int ret;
	struct inode *ip = NULL;
	pagetable_t pagetable = NULL;
	struct seg *seghead = NULL, *seg;
	struct proc *p = myproc();

	__debug_info("execve", "in\n");
	if ((ip = namei(path)) == NULL) {
		__debug_warn("execve", "can't open %s\n", path);
		ret = -ENOENT;
		goto bad;
	}

	// Make a copy of p->pagetable without old user space, 
	// but with the same kstack we are using now, which can't be changed.
	if ((pagetable = (pagetable_t)allocpage()) == NULL) {
		ret = -ENOMEM;
		goto bad;
	}
	memmove(pagetable, p->pagetable, PGSIZE);
	// Remove old u-map from the new pt, but don't free since later op might fail.
	for (int i = 0; i < PX(2, MAXUVA); i++) {
		pagetable[i] = 0;
	}

	__debug_info("execve", "get elf\n");
	// Check ELF header
	ilock(ip);
	struct elfhdr elf;
	if (ip->fop->read(ip, 0, (uint64) &elf, 0, sizeof(elf)) != sizeof(elf) || elf.magic != ELF_MAGIC) {
		iunlock(ip);
		ret = -ENOEXEC;
		goto bad;
	}

	__debug_info("execve", "load from elf\n");
	// Load program into memory.
	struct proghdr ph;
	uint64 elfaddr = 0;
	int flags;
	for (int i = 0, off = elf.phoff; i < elf.phnum; i++, off += sizeof(ph)) {
		if (ip->fop->read(ip, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph)) {
			__debug_warn("execve", "fail to read ELF file\n");
			iunlock(ip);
			ret = -EIO;
			goto bad;
		}
		if (ph.type != ELF_PROG_LOAD)
			continue;
		// if (ph.vaddr % PGSIZE != 0 || ph.memsz < ph.filesz || ph.vaddr + ph.memsz < ph.vaddr) {
		if (ph.memsz < ph.filesz || ph.vaddr + ph.memsz < ph.vaddr) {
			__debug_warn("execve", "funny ELF file: va=%p msz=0x%x fsz=0x%x\n", ph.vaddr, ph.memsz, ph.filesz);
				iunlock(ip);
			ret = -ENOEXEC;
			goto bad;
		}

		// turn ELF flags to PTE flags
		flags = 0;
		flags |= (ph.flags & ELF_PROG_FLAG_EXEC) ? PTE_X : 0;
		flags |= (ph.flags & ELF_PROG_FLAG_WRITE) ? PTE_W : 0;
		flags |= (ph.flags & ELF_PROG_FLAG_READ) ? PTE_R : 0;
		seg = newseg(pagetable, seghead, LOAD, ph.vaddr, ph.memsz, flags);
		if(seg == NULL) {
			__debug_warn("execve", "newseg fail: vaddr=%p, memsz=%d\n", ph.vaddr, ph.memsz);
			iunlock(ip);
			ret = -ENOMEM;
			goto bad;
		}
		seghead = seg;

		if (ph.off == 0)
			elfaddr = ph.vaddr;

		seg = locateseg(seghead, ph.vaddr);
		seg->f_off = ph.off;
		seg->f_sz = ph.filesz;
		if (ph.vaddr <= elf.entry && elf.entry < ph.vaddr + ph.filesz) {
			__debug_info("execve", "meet entry segment: entry=0x%x, va=0x%x, sz=0x%x\n", elf.entry, ph.vaddr, ph.filesz);
			if ((ret = loadseg(pagetable, elf.entry, seg, ip)) < 0) {
				__debug_warn("execve", "load segment\n");
				iunlock(ip);
				goto bad;
			}
		}
	}
	iunlock(ip);

	// Heap
	flags = PTE_R | PTE_W;
	for (seg = seghead; seg->next != NULL; seg = seg->next);
	__debug_info("execve", "making heap, start=%p\n", PGROUNDUP(seg->addr + seg->sz));
	seg = newseg(pagetable, seghead, HEAP, PGROUNDUP(seg->addr + seg->sz), 0, flags);
	if(seg == NULL) {
		__debug_warn("execve", "new heap fail\n");
		ret = -ENOMEM;
		goto bad;
	}
	seghead = seg;

	__debug_info("execve", "making stack\n");
	// Stack
	uint64 sp = VUSTACK;
	// uint64 stackbase = VUSTACK - PGSIZE;
	uint64 stackbase = VUSTACK - PGSIZE * 4;
	seg = newseg(pagetable, seghead, STACK, stackbase, sp - stackbase, flags);
	stackbase += PGSIZE * 3;
	if(seg == NULL || uvmalloc(pagetable, stackbase, sp, flags) == 0) {
		__debug_warn("execve", "new stack fail\n");
		ret = -ENOMEM;
		goto bad;
	}
	seghead = seg;

	// prepare a NULL on the bottom of stack
	sp -= sizeof(uint64);
	__debug_info("execve", "pushing argv/envp\n");
	// arguments to user main(argc, argv, envp)
	// argc is returned via the system call return
	// value, which goes in a0.
	int64 argc, envc;
	uint64 uargv[MAXARG + 1], uenvp[MAXENV + 1];
	if ((envc = pushstack(pagetable, uenvp, envp, MAXENV, &sp)) < 0 ||
		(argc = pushstack(pagetable, uargv, argv, MAXARG, &sp)) < 0) {
		__debug_warn("execve", "fail to push argv or envp into user stack\n");
		ret = envc < 0 ? envc : argc;
		goto bad;
	}
	uint64 random[2] = { 0xcde142a16cb93072, 0x128a39c127d8bbf2 };
	sp -= 16;
	if (sp < stackbase || copyout(pagetable, sp, (char *)random, 16) < 0) {
		ret = -EINVAL;
		goto bad;
	}

	uint64 auxvec[][2] = {
		// {AT_HWCAP, 0x112d},
		{AT_PAGESZ, PGSIZE},
		{AT_PHDR, elf.phoff + elfaddr},
		{AT_PHENT, elf.phentsize},
		{AT_PHNUM, elf.phnum},
		{AT_UID, 0},
		{AT_EUID, 0},
		{AT_GID, 0},
		{AT_EGID, 0},
		{AT_SECURE, 0},
		{AT_RANDOM, sp},
		{AT_NULL, 0}
	};

	sp -= 16 + sizeof(auxvec);
	sp -= (envc + 1) * sizeof(uint64);
	uint64 a2 = sp;
	sp -= (argc + 1) * sizeof(uint64);
	uint64 a1 = sp;
	sp -= sizeof(uint64); // argc
	flags = sp % 16;      // on risc-v, sp should be 16-byte aligned
	sp -= flags;
	a1 -= flags;
	a2 -= flags;

	__debug_info("execve", "pushing argv/envp table\n");
	if (sp < stackbase || 
		copyout(pagetable, a2 + (envc + 1) * sizeof(uint64), (char *)auxvec, sizeof(auxvec)) < 0 ||
		copyout(pagetable, a2, (char *)uenvp, (envc + 1) * sizeof(uint64)) < 0 ||
		copyout(pagetable, a1, (char *)uargv, (argc + 1) * sizeof(uint64)) < 0 ||
		copyout(pagetable, sp, (char *)&argc, sizeof(uint64)) < 0)
	{
		__debug_warn("execve", "fail to copy argv/envp table into user stack\n");
		ret = -EINVAL;
		goto bad;
	}
	p->trapframe->a1 = a1;
	p->trapframe->a2 = a2;

	// Save program name for debugging.
	safestrcpy(p->name, ip->entry->filename, sizeof(p->name));

	// Commit to the user image.
	pagetable_t oldpagetable = p->pagetable;
	seg = p->segment;
	p->pagetable = pagetable;
	p->segment = seghead;
	p->trapframe->epc = elf.entry;  // initial program counter = main
	p->trapframe->sp = sp; // initial stack pointer
	if (p->elf)
		iput(p->elf);
	p->elf = ip;

	__debug_info("execve", "sp=%p, stackbase=%p, entry=%p\n", sp, stackbase, elf.entry);
	fdcloexec(&p->fds);
	w_satp(MAKE_SATP(p->pagetable));
	sfence_vma();

	delsegs(oldpagetable, seg);
	uvmfree(oldpagetable);
	freepage(oldpagetable);

	return 0;
	// return argc; // this ends up in a0, the first argument to main(argc, argv)

	bad:
	__debug_warn("execve", "reach bad: seg=%p, pt=%p, ip=%p\n", seghead, pagetable, ip);
	if (seghead) {
		delsegs(pagetable, seghead);
	}
	if (pagetable) {
		uvmfree(pagetable);
		freepage(pagetable);
	}
	if (ip) {
		iput(ip);
	}
	return ret;
}
