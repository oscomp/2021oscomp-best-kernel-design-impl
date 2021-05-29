#ifndef __DEBUG_mmap
#undef DEBUG
#endif

#define __module_name__	"mmap"

#include "include/types.h"
#include "include/riscv.h"
#include "include/memlayout.h"
#include "include/vm.h"
#include "include/pm.h"
#include "include/kmalloc.h"
#include "include/mmap.h"
#include "include/usrmm.h"
#include "include/proc.h"
#include "include/string.h"
#include "include/debug.h"

struct mapped*
add_map(struct mapped **phead, int off, int share)
{
	// struct mapped *p = *phead;
	// while(p && p->offset <= off){
	// 	if(p->offset == off){
	// 		if (share)
	// 			p->n_ref++;
	// 		return p;
	// 	}
	// 	phead = &(p->next);
	// 	p = p->next;
	// }

	// if(share){
		struct mapped* new_head = (struct mapped*)kmalloc(sizeof(struct mapped));
		if(!new_head){
			// panic("kmalloc failed in add_map!\n");
			return NULL;
		}
		new_head->ph_addr = NULL;
		new_head->next = *phead;
		new_head->pprev = phead;
		*phead = new_head;
		new_head->n_ref = 1;
		return new_head;
	// }
	// return NULL;
}

// return the **pprev field in the pre-node, whether ever it's in an inode or what
static struct mapped **
get_prevmap(struct mapped **phead, int off)
{
	struct mapped *p = *phead;
	while(p && p->offset <= off){
		if(p->offset == off){
			return phead;
		}
		phead = &(p->next);
		p = p->next;
	}
	return phead;
}

void
del_map(struct mapped *p, int off, int npages)
{
	struct mapped *next = NULL;
	while (npages--) {
		__debug_assert("del_map", p->offset == off,
						"p->off=%p off=%p\n", p->offset, off);
		next = p->next;
		p->n_ref--;
		if (p->n_ref == 0) {
			*(p->pprev) = p->next;
			if (p->next != NULL) {
				p->next->pprev = p->pprev;
			}
			__debug_info("del_map", "freeing %p\n", p);
			kfree(p);
		}
		p = next;
		off += PGSIZE;
	}
}

void del_segmap(struct seg *seg)
{
	__debug_assert("del_segmap", seg->type == MMAP, "funny type %d\n", seg->type);
	// if (!seg->mmap)
	// 	return;
	struct inode *ip = seg->mmap->ip;
	ilock(ip);
	struct mapped *map = *get_prevmap(&ip->maphead, seg->f_off);
	del_map(map, map->offset, seg->sz / PGSIZE);
	iunlock(ip);
	fileclose(seg->mmap);
}


uint64
do_mmap(uint64 start, int len, int prot, int flags, struct file *f, int off)
{
	struct inode *ip = f->ip;
	if(off + len > ip->size) {
		__debug_warn("mmap", "length overflows!\n");
		return -1;
	}
	if (ip->mode & I_MODE_DIR) {
		return -1;
	}
	// Must provide one of them.
	if (!(flags & (MAP_SHARED|MAP_PRIVATE))) {
		return -1;
	}
	if((f->readable ^ (prot & PROT_READ)) || (f->writable ^ ((prot & PROT_WRITE) >> 1)))
	{
		__debug_warn("mmap", "unmatched priviledge");
		return -1;
	}

	struct proc *p = myproc();

	int sz = PGROUNDUP(len);

	struct seg* s = getseg(p->segment, STACK);
	uint64 base = VUMMAP;
	for (struct seg *ss = s->next; ss != NULL; ss = ss->next) {
		if (ss->addr >= base + sz) {
			break;
		}
		s = ss;
		base = PGROUNDUP(ss->addr + ss->sz);
	}

	if(base + sz > MAXUVA)
	{
		__debug_warn("mmap", "Not enough user space in mmap!\n");
		return -1;
	}

	struct seg* new_seg;
	if((new_seg = (struct seg*) kmalloc(sizeof(struct seg))))
	{
		new_seg->type = MMAP;
		new_seg->addr = base;
		new_seg->sz = sz;
		new_seg->flag = (prot << 1) & 0xE;
	}
	else
	{
		__debug_warn("mmap", "newseg failed in mmap!\n");
		return -1;
	}

	int n = sz / PGSIZE;
	
	int i;
	struct mapped **pmap = &(ip->maphead);
	struct mapped *newhead = NULL, *map;
	ilock(ip);
	for(i = 0; i < n; i++)
	{
		int len2 = len >= PGSIZE ? PGSIZE : len; 
		len -= len2;
		if(flags & MAP_SHARED)
		{ // share
			pmap = get_prevmap(pmap, off);
			int isnew = *pmap == NULL || (*pmap)->offset > off;
			if (isnew) { // the first one to share
				map = kmalloc(sizeof(struct mapped));
				if (map == NULL)
					goto fail;
				map->offset = off;
				map->len = len2;
				map->n_ref = 0;
				map->ph_addr = (uint64) allocpage();

				if (!map->ph_addr) {
					kfree(map);
					goto fail;
				}

				if((ip->fop->read(ip, 0, map->ph_addr, off + i * PGSIZE, len2)) != len2) {
					freepage((void*)map->ph_addr);
					kfree(map);
					goto fail;
				}
				if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
					memset((void*)(map->ph_addr + len2), 0, PGSIZE - len2);
				}
			} else {
				map = *pmap;
			}

			if(mappages(p->pagetable, base + i * PGSIZE, PGSIZE, map->ph_addr, new_seg->flag|PTE_U))
			{
				__debug_warn("mmap", "allocpage failed!\n");
				if (isnew) {
					freepage((void*)map->ph_addr);
					kfree(map);
				}
				goto fail;
			}
			if (newhead == NULL) {	// keep the first mapped list, in case of failure
				newhead = map;
			}
			if (isnew) {
				map->next = *pmap;
				map->pprev = pmap;
				*pmap = map;
				if (map->next)
					map->next->pprev = &(map->next);
			}
			map->n_ref++;
			pmap = &(map->next);	// hook at the present mapped node, start searching from it in the next loop
		}
		else
		{ // not share
			uint64 ph_addr = (uint64) allocpage();
			if (ph_addr == NULL)
				goto fail;

			pmap = get_prevmap(pmap, off);
			map = *pmap;
			if (map && map->offset == off) { // found a mapped page
				memmove((void*)ph_addr, (void*)map->ph_addr, PGSIZE);
			} else {
				if((ip->fop->read(ip, 0, ph_addr, off + i * PGSIZE, len2)) != len2) {
					freepage((void*)ph_addr);
					goto fail;
				}
				if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
					memset((void*)(ph_addr + len2), 0, PGSIZE - len2);
				}
			}

			if(mappages(p->pagetable, base + i * PGSIZE, PGSIZE, ph_addr, new_seg->flag|PTE_U))
			{
				__debug_warn("mmap", "allocpage failed!\n");
				freepage((void*)ph_addr);
				goto fail;
			}
		}
	}

	iunlock(ip);
	new_seg->next = s->next;
	s->next = new_seg;
	new_seg->f_off = off;
	if (flags & MAP_SHARED) {
		new_seg->mmap = filedup(f);
	} else {
		new_seg->mmap = NULL;
	}

	// sfence_vma();

	return base;

fail:
	if (newhead)
		del_map(newhead, off, i);
	iunlock(ip);
	uvmdealloc(p->pagetable, base, base + i * PGSIZE, MMAP);
	kfree(new_seg);
	return -1;
}


int do_munmap(uint64 start, uint64 len)
{
	struct proc *p = myproc();
	struct seg *stack = getseg(p->segment, STACK);
	struct seg *s = locateseg(stack, start);
	struct seg *s2 = NULL;
	uint64 sz = PGROUNDUP(len);

	if (s == NULL || s->type != MMAP) {
		__debug_warn("do_munmap", "no such seg or not a mmap seg at %p\n", start);
		return -1;
	}
	if (s->sz > sz) {
		__debug_warn("do_munmap", "seg->sz=%d is less than the given pgsz=%d\n", s->sz, sz);
		return -1;
	}

	uint64 end = start + sz;
	// In this case, the segment will split into two part
	if (start > s->addr && end < s->addr + s->sz) {
		s2 = kmalloc(sizeof(struct seg));
		if (s2 == NULL) {
			__debug_warn("do_munmap", "segment splits fail\n");
			return -1;
		}
	}

	// cal the off of file corresponding to start
	uint64 off = s->f_off + (start - s->addr);

	struct file *fp = s->mmap;
	if (fp && (s->flag & PTE_W)) { // A shared map, carry through to the file.
		struct inode *ip = s->mmap->ip;
		ilock(ip);
		struct mapped *map = *get_prevmap(&ip->maphead, off);

		__debug_assert("do_munmap", off == map->offset,
						"funny mmap: s->off=%d map->off=%d\n", off, map->offset);

		uint64 sz2 = sz;
		for (struct mapped *m = map; sz2 > 0 && m; sz2 -= PGSIZE, off += PGSIZE, m = m->next) {
			
			__debug_assert("do_munmap", off == m->offset,
							"funny mmap: s->off=%d map->off=%d\n", off, m->offset);
			
			// This op may fail, but what can we do about it?
			ip->fop->write(ip, 0, m->ph_addr, off, m->len);
		}
		del_map(map, map->offset, sz / PGSIZE);
		iunlock(ip);
	}

	// Now handle segment
	uvmdealloc(p->pagetable, start, end, s->type);
	sfence_vma();

	// start end sz
	if (s2) { // the segment splits
		s2->addr = end;
		s2->sz = s->sz - (end - s->addr);
		s2->flag = s->flag;
		s2->type = s->type;
		s2->f_off = off;
		s2->next = s->next;
		s2->mmap = fp;
		if (fp)
			filedup(fp);
		s->sz = start - s->addr;
		s->next = s2;
	} else {
		if (start == s->addr && sz == s->sz) { // the segment is all free
			while (stack->next != s) {
				stack = stack->next;
			}
			stack->next = s->next;
			if (fp)
				fileclose(fp);
			kfree(s);
		} else if (start == s->addr) {
			s->addr = end;
		} else if (end == s->addr + s->sz) {
			s->sz -= sz;
		} else {
			panic("do_munmap: funny case");
		}
	}

	return 0;
}