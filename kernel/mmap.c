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
	while (npages--) {
		__debug_assert("del_map", p->offset == off,
						"p->off=%p off=%p\n", p->offset, off);
		p->n_ref--;
		if (p->n_ref == 0) {
			*(p->pprev) = p->next;
			if (p->next != NULL) {
				p->next->pprev = p->pprev;
			}
		}
		off += PGSIZE;
	}
}

uint64
do_mmap(uint64 start, int len, int prot, int flags, struct file *f, int off)
{
	struct inode *ip = f->ip;
	if(off + len > ip->size)
	{
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
		new_seg->flag = (flags << 1) & 0xE;
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
		if(flags & MAP_SHARED)
		{ // share
			pmap = get_prevmap(pmap, off);
			int isnew = *pmap == NULL || (*pmap)->offset > off;
			if (isnew) { // the first one to share
				map = kmalloc(sizeof(struct mapped));
				if (map == NULL)
					goto fail;
				map->offset = off;
				map->n_ref = 0;
				map->ph_addr = (uint64) allocpage();

				if (!map->ph_addr) {
					kfree(map);
					goto fail;
				}
				if((ip->fop->read(ip, 0, map->ph_addr, off + i * PGSIZE, PGSIZE)) != PGSIZE) {
					if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
						memset((void*)(map->ph_addr + len % PGSIZE), 0, PGSIZE - (len % PGSIZE));
					} else {	// not last part, fail to read
						freepage((void*)map->ph_addr);
						kfree(map);
						goto fail;
					}
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
				if((ip->fop->read(ip, 0, ph_addr, off + i * PGSIZE, PGSIZE)) != PGSIZE) {
					if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
						memset((void*)(ph_addr + len % PGSIZE), 0, PGSIZE - (len % PGSIZE));
					} else {	// not last part, fail to read
						freepage((void*)ph_addr);
						goto fail;
					}
				}
			}

			if(mappages(p->pagetable, base + i * PGSIZE, PGSIZE, ph_addr, new_seg->flag|PTE_U))
			{
				__debug_warn("mmap", "allocpage failed!\n");
				freepage((void*)ph_addr);
				goto fail;
			}
			// if(map)
			// { // mapped by others
			// 	memmove((void *)ph_addr, (void *)map->ph_addr, PGSIZE);
			// }
			// else
			// { // not mapped by others
			// 	if(i == n - 1)
			// 	memset((void *) (base + i * PGSIZE), 0, PGSIZE);
			// 	if((ip->fop->read(ip, 1, base + i * PGSIZE, off + i * PGSIZE, PGSIZE)) > 0)
			// 	;
			// }
		}
	}

	iunlock(ip);
	new_seg->next = s->next;
	s->next = new_seg;

	sfence_vma();

	return base;

fail:
	if (newhead)
		del_map(newhead, off, i);
	iunlock(ip);
	uvmdealloc(p->pagetable, base, base + i * PGSIZE, MMAP);
	kfree(new_seg);
	return -1;
}