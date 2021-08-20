#ifndef __DEBUG_usrmm
#undef  DEBUG
#endif

#define __module_name__ 	"usrmm"

#include "mm/usrmm.h"
#include "mm/kmalloc.h"
#include "sched/proc.h"
#include "mm/pm.h"
#include "mm/vm.h"
#include "utils/string.h"
#include "utils/debug.h"
#include "errno.h"

struct seg *newseg(pagetable_t pagetable, struct seg *head,
				enum segtype type, uint64 offset, uint64 sz, long flag)
{
	uint64 nstart, nend;
	nstart = PGROUNDDOWN(offset);
	nend = offset + sz;
	nend = PGROUNDUP(nend);

	// __debug_info("newseg", "type=%d nstart=%p nend=%p\n", type, nstart, nend);
	// __debug_info("newseg", "sizeof(struct seg): %d\n", sizeof(struct seg));
	struct seg *seg = NULL;
	for (struct seg *s = head; s != NULL; s = s->next) {
		// __debug_info("newseg", "type=%d start=%p end=%p\n", s->type, start, end);
		if (nend <= s->addr) {
			break;
		} else if (nstart >= s->addr + s->sz) {
			seg = s;
		} else {
			__debug_error("newseg", "segments overlap\n");
			return NULL;
		}
	}

	// __debug_info("newseg", "calling to kmalloc\n");
	struct seg *p;
	if ((p = (struct seg *)kmalloc(sizeof(struct seg))) == NULL) {
		__debug_error("newseg", "fail to kmalloc\n");
		return NULL;
	}
/*
	// __debug_info("newseg", "calling to uvmalloc\n");
	if(uvmalloc(pagetable, nstart, nend, flag) == 0)
	{
		kfree(p);
		__debug_error("newseg", "fail to uvmalloc\n");
		return NULL;
	}
*/
	if (seg == NULL) {
		p->next = head;
		head = p;
	} else {
		p->next = seg->next;
		seg->next = p;
	}
	p->addr = offset;
	p->sz = sz;
	p->type = type;
	p->flag = flag;
	p->mmap = NULL;

	// __debug_info("newseg", "done\n");
	// return p;
	return head;
}

struct seg *locateseg(struct seg *head, uint64 addr)
{
		// __debug_info("locateseg", "addr=%p\n", addr);
	while (head) {
		uint64 start, end;
		start = PGROUNDDOWN(head->addr);
		end = head->addr + head->sz;
		// __debug_info("locateseg", "type=%d start=%p end=%p\n", head->type, start, end);
		if (addr >= start && addr < end)
			return head;
		else if (addr >= end)
			head = head->next;
		else {
			__debug_warn("locateseg", "va=%p in no segment\n", addr);
			return NULL;
		}
	}

	return NULL;
}

struct seg *getseg(struct seg *head, enum segtype type)
{
	for (struct seg *seg = head; seg != NULL; seg = seg->next) {
		if (seg->type == type)
			return seg;
	}
	return NULL;
}

// end is not included
struct seg *partofseg(struct seg *head, uint64 start, uint64 end)
{
	// __debug_info("partofseg", "start=%p end=%p\n", start, end);
	struct seg *s = locateseg(head, start);
	struct seg *s2 = locateseg(s, end - 1);
	if (s == NULL || s2 == NULL)
		return NULL;

	if (s != s2 && PGROUNDUP(s->addr + s->sz) != PGROUNDDOWN(s2->addr))
		return NULL;

	return s;
}

void freeseg(pagetable_t pagetable, struct seg *p)
{
	uvmdealloc(pagetable, PGROUNDDOWN(p->addr), p->addr + p->sz);
	p->sz = 0;
}

struct seg *delseg(pagetable_t pagetable, struct seg *s)
{
	struct seg *next = s->next;
	// __debug_info("delseg", "s = %p\n", s);
	// __debug_info("delseg", "s->type: %d\n", s->type);
	uvmdealloc(pagetable, PGROUNDDOWN(s->addr), s->addr + s->sz);
	if (s->type == MMAP) {
		mmapdel(s, 1);
	}
	kfree(s);
	return next;
}

void delsegs(pagetable_t pagetable, struct seg *head)
{
	while (head) {
		head = delseg(pagetable, head);
	}
}

struct seg *copysegs(pagetable_t pt, struct seg *seg, pagetable_t pt2)
{
	struct seg *head = NULL, *tail = NULL;
	for (; seg != NULL; seg = seg->next) {
		struct seg *s = kmalloc(sizeof(struct seg));
		if (s == NULL) {
			__debug_warn("copysegs", "seg kmalloc fail\n");
			goto bad;
		}

		// This means that the mmap segment is shared, shouldn't do COW.
		// If seg->mmap is NULL, it means the mmap is private.
		int cow = (seg->type == MMAP && MMAP_SHARE(seg->mmap)) ? 0 : 1;
		// Copy user memory from parent to child.
		if (uvmcopy(pt, pt2, PGROUNDDOWN(seg->addr), seg->addr + seg->sz, cow) < 0) 
		{
			__debug_warn("copysegs", "uvmcopy_cow fails\n");
			__debug_warn("copysegs", "%p, %p, %d\n", 
					seg->addr, seg->addr + seg->sz, seg->type);
			kfree(s);
			__debug_warn("copysegs", "exit kfree\n");
			goto bad;
		}
		memmove(s, seg, sizeof(struct seg));
		if (seg->type == MMAP) {
			mmapdup(seg);
		}
		s->next = NULL;
		if (tail == NULL) {
			head = tail = s;
		} else {
			tail->next = s;
			tail = s;
		}
	}
	return head;

bad:
	while (head) {
		tail = head->next;
		if (head->type == MMAP)
			mmapdel(head, 0);
		uvmdealloc(pt2, PGROUNDDOWN(head->addr), head->addr + head->sz);
		kfree(head);
		head = tail;
	}
	return NULL;
}

// caller must hold ip->lock
int loadseg(pagetable_t pagetable, uint64 va, struct seg *s, struct inode *ip)
{
	va = PGROUNDDOWN(va) >= s->addr ? PGROUNDDOWN(va) : s->addr;
	uint64 off = s->f_off + (va - s->addr);
	int64 size = s->f_sz - (va - s->addr);
	uint64 end = va + PGSIZE;
	end = PGROUNDDOWN(end);
	if ((int64)(end - va) < size)
		size = end - va;

	__debug_info("loadseg", "base=0x%x, off=0x%x, size=0x%x, end=0x%x\n", va, off, size, end);
	__debug_info("loadseg", "saddr=0x%x, sz=0x%x, f_off=0x%x, f_zs=0x%x, flag=0x%x\n", s->addr, s->sz, s->f_off, s->f_sz, s->flag);

	void *pa = allocpage();
	if (pa == NULL)
		return -ENOMEM;

	memset(pa, 0, PGSIZE);
	__debug_info("loadseg", "pa=%p, base=0x%x, f_off=0x%x, size=%d\n", pa, va, off, size);
	if (size > 0 && ip->fop->read(ip, 0, (uint64)pa + va % PGSIZE, off, size) != size) {
		__debug_warn("loadseg", "fail to load! off=0x%x, size=%d\n", off, size);
		goto bad;
	}
	pagereg((uint64)pa, 0);
	if (mappages(pagetable, va, end - va, (uint64)pa, s->flag|PTE_U) < 0) {
		__debug_warn("loadseg", "fail to map! va=0x%x, end=0x%x\n", va, end);
		goto bad;
	}

	return 0;

bad:
	freepage(pa);
	return -1;
}

// Test whether the given range is in any part of the current proc's v-space
int rangeinseg(uint64 start, uint64 end)
{
	return partofseg(myproc()->segment, start, end) ? 1 : 0;
}