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
#include "include/errno.h"


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
	if (seg->mmap == (void *)MAP_ANONYMOUS)
		return;

	struct inode *ip = seg->mmap->ip;
	ilock(ip);
	// acquire(&ip->lock.lk);
	struct mapped *map = *get_prevmap(&ip->maphead, seg->f_off);
	del_map(map, map->offset, seg->sz / PGSIZE);
	// release(&ip->lock.lk);
	iunlock(ip);
	fileclose(seg->mmap);
}

void dup_segmap(struct seg *seg)
{
	__debug_assert("del_segmap", seg->type == MMAP && seg->mmap, "funny seg\n");
	struct file *fp = seg->mmap;
	if (fp == (void *)MAP_ANONYMOUS)
		return;

	struct inode *ip = fp->ip;
	// acquire(&ip->lock.lk);
	ilock(ip);
	struct mapped *map = *get_prevmap(&ip->maphead, seg->f_off);
	int npages = PGROUNDUP(seg->sz) / PGSIZE;
	while (npages--) {
		map->n_ref++;
		map = map->next;
	}
	iunlock(ip);
	// release(&ip->lock.lk);
	filedup(fp);
}

static struct seg *split_segment(struct seg *s, uint64 start, uint64 end)
{
	uint64 segend = s->addr + s->sz;
	if (end < start || (start <= PGROUNDDOWN(s->addr) && end >= PGROUNDUP(segend))) {
		__debug_warn("split_segment", "range [0x%x, 0x%x] is cover seg:{addr=%p, size=0x%x\n",
						start, end, s->addr, s->sz);
		return s;
	}

	struct seg *s1, *s2 = NULL;
	start = PGROUNDDOWN(start);
	end = PGROUNDUP(end);
	s1 = kmalloc(sizeof(struct seg));
	if (s1 == NULL)
		return NULL;
	
	if (start > s->addr && end < segend) {
		if ((s2 = kmalloc(sizeof(struct seg))) == NULL) {
			kfree(s1);
			return NULL;
		}
	}
	
	*s1 = *s;
	// memmove(s1, s, sizeof(struct seg));
	if (s2) 
		*s2 = *s;
		// memmove(s2, s, sizeof(struct seg));
	if (s->mmap && s->mmap != (void *)MAP_ANONYMOUS) {
		filedup(s1->mmap);
		if (s2)
			filedup(s2->mmap);
	}
	
	if (!s2) {
		uint64 spliter;
		if (start <= s->addr) {
			spliter = end;
			s2 = s;
		} else if (end >= s->addr + s->sz) {
			spliter = start;
			s2 = s1;
		} else
			panic("split_segment: funny case");

		s1->addr = spliter;
		s1->sz = segend - spliter;
		s->sz -= s1->sz;
		s1->next = s->next;
		s->next = s1;
	}
	else {
		s2->addr = start;
		s2->sz = end - start;
		s1->addr = end;
		s1->sz = segend - end;
		s->sz = start - s->addr;
		s1->next = s->next;
		s2->next = s1;
		s->next = s2;
	}

	return s2;
}

static int msync_file(struct seg *s, int del)
{
	uint64 off = s->f_off;
	struct file *fp = s->mmap;
	if (fp && (s->flag & PTE_W)) { // A shared map, carry through to the file.
		struct inode *ip = fp->ip;
		ilock(ip);
		struct mapped *map = *get_prevmap(&ip->maphead, off);
		__debug_assert("do_munmap", off == map->offset,
						"funny mmap: s->off=%d map->off=%d\n", off, map->offset);

		uint64 sz2 = s->sz;
		for (struct mapped *m = map; sz2 > 0 && m; sz2 -= PGSIZE, off += PGSIZE, m = m->next) {
			__debug_assert("do_munmap", off == m->offset,
							"funny mmap: s->off=%d map->off=%d\n", off, m->offset);
			
			// This op may fail, but what can we do about it?
			ip->fop->write(ip, 0, m->ph_addr, off, m->len);
		}
		iunlock(ip);
		if (del) {
			del_map(map, map->offset, s->sz / PGSIZE);
			fileclose(fp);
			s->mmap = NULL;
		}
	}
	return 0;
}

static int lookup_fixed_segment(uint64 start, uint64 end, struct seg **pprev, struct seg **pnew)
{
	if (end > MAXUVA) {
		__debug_warn("lookup_fixed_segment", "Not enough user space in mmap!\n");
		return -EINVAL;
	}

	struct proc *p = myproc();
	struct seg *s, *s2;
	struct seg *prev = NULL;

	for (s = p->segment; s != NULL;)
	{
		if (start >= s->addr + s->sz) {
			prev = s;
			s = s->next;
		} else if (s->addr >= end) {
			break;
		}
		else {
			s2 = split_segment(s, start, end);
			if (s2 == NULL)
				return -ENOMEM;
			if (s2 != s) {	// after spliting, s2 is not the first part
				for (prev = s; prev->next != s2; prev = prev->next);
			}
			if (s->mmap && s->mmap != (void *)MAP_ANONYMOUS) {
				msync_file(s, 1);
			}
			uvmdealloc(p->pagetable, PGROUNDDOWN(s->addr), s->addr + s->sz, s->type);
			s2 = s->next;
			if (prev)
				prev->next = s2;
			kfree(s);
			s = s2;
		}
	}

	s2 = kmalloc(sizeof(struct seg));
	if (s2 == NULL)
		return -ENOMEM;

	memset(s2, 0, sizeof(struct seg));
	s2->addr = start;
	s2->sz = end - start;
	s2->type = MMAP;

	*pprev = prev;
	*pnew = s2;
	return 0;
}


static int lookup_segment(uint64 sz, struct seg **pprevseg, struct seg **pnewseg)
{
	struct proc *p = myproc();
	struct seg *s = getseg(p->segment, STACK);
	uint64 base = VUMMAP;
	for (struct seg *ss = s->next; ss != NULL; ss = ss->next) {
		if (ss->addr >= base + sz)
			break;
		s = ss;
		base = PGROUNDUP(ss->addr + ss->sz);
	}
	if (base + sz > MAXUVA) {
		__debug_warn("lookup_segment", "Not enough user space in mmap!\n");
		return -EINVAL;
	}

	struct seg* newseg = kmalloc(sizeof(struct seg));
	if (newseg == NULL) {
		__debug_warn("lookup_segment", "newseg failed in mmap!\n");
		return -ENOMEM;
	}

	newseg->type = MMAP;
	newseg->addr = base;
	newseg->sz = sz;
	newseg->mmap = NULL;
	*pprevseg = s;
	*pnewseg = newseg;
	return 0;
}

static int mmap_file(struct seg *s, uint64 len, int flags, struct file *f, int64 off)
{
	uint64 sz = PGROUNDUP(len);
	uint64 base = s->addr;
	int n = sz / PGSIZE, i, ret = 0;
	struct inode *ip = f->ip;
	struct mapped **pmap = &(ip->maphead);
	struct mapped *newhead = NULL, *map;
	struct proc *p = myproc();

	ilock(ip);
	for (i = 0; i < n; i++)
	{
		uint64 len2 = len >= PGSIZE ? PGSIZE : len; 
		len -= len2;
		if (flags & MAP_SHARED)
		{ // share
			pmap = get_prevmap(pmap, off);
			int isnew = *pmap == NULL || (*pmap)->offset > off;
			if (isnew) { // the first one to share
				map = kmalloc(sizeof(struct mapped));
				if (map == NULL) {
					ret = -ENOMEM;
					goto fail;
				}
				map->offset = off;
				map->len = len2;
				map->n_ref = 0;
				map->ph_addr = (uint64) allocpage();
				if (!map->ph_addr) {
					kfree(map);
					ret = -ENOMEM;
					goto fail;
				}

				if ((ip->fop->read(ip, 0, map->ph_addr, off + i * PGSIZE, len2)) != len2) {
					freepage((void*)map->ph_addr);
					kfree(map);
					ret = -EIO;
					goto fail;
				}
				if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
					memset((void*)(map->ph_addr + len2), 0, PGSIZE - len2);
				}
			} else {
				map = *pmap;
			}

			if (mappages(p->pagetable, base + i * PGSIZE, PGSIZE, map->ph_addr, s->flag|PTE_U)) {
				__debug_warn("do_mmap_file", "allocpage failed!\n");
				if (isnew) {
					freepage((void*)map->ph_addr);
					kfree(map);
				}
				ret = -ENOMEM;
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
			if (ph_addr == NULL) {
				ret = -ENOMEM;
				goto fail;
			}

			pmap = get_prevmap(pmap, off);
			map = *pmap;
			if (map && map->offset == off) { // found a mapped page
				memmove((void*)ph_addr, (void*)map->ph_addr, PGSIZE);
			} else {
				if ((ip->fop->read(ip, 0, ph_addr, off + i * PGSIZE, len2)) != len2) {
					freepage((void*)ph_addr);
					ret = -EIO;
					goto fail;
				}
				if (i == n - 1) {	// last part of the file, fill with 0 in the last empty part
					memset((void*)(ph_addr + len2), 0, PGSIZE - len2);
				}
			}
			if (mappages(p->pagetable, base + i * PGSIZE, PGSIZE, ph_addr, s->flag|PTE_U)) {
				__debug_warn("do_mmap_file", "allocpage failed!\n");
				freepage((void*)ph_addr);
				ret = -ENOMEM;
				goto fail;
			}
		}
	}

	iunlock(ip);
	s->f_off = off;
	if (flags & MAP_SHARED) {
		s->mmap = filedup(f);
	} else {
		s->mmap = NULL;
	}
	return 0;

fail:
	if (newhead)
		del_map(newhead, off, i);
	iunlock(ip);
	uvmdealloc(p->pagetable, base, base + i * PGSIZE, MMAP);
	return -ret;
}

static int mmap_anonymous(struct seg *s, uint64 sz, int flags)
{
	struct proc *p = myproc();
	uint64 base = s->addr;

	if (uvmalloc(p->pagetable, base, base + sz, s->flag) == 0)
		return -ENOMEM;

	if (flags & MAP_SHARED)
		s->mmap = (void *)MAP_ANONYMOUS;

	return 0;
}

uint64 do_mmap(uint64 start, uint64 len, int prot, int flags, struct file *f, int64 off)
{
	// Must provide one of them.
	if (!(flags & (MAP_SHARED|MAP_PRIVATE))) {
		return -EINVAL;
	}

	if (f) {
		struct inode *ip = f->ip;
		if (off + len > ip->size) {
			__debug_warn("mmap", "length overflows!\n");
			return -EINVAL;
		}
		if (ip->mode & I_MODE_DIR)
			return -EISDIR;
		if ((f->readable ^ (prot & PROT_READ)) || (f->writable ^ ((prot & PROT_WRITE) >> 1))) {
			__debug_warn("mmap", "unmatched priviledge");
			return -EPERM;
		}
	} else if (!(flags & MAP_ANONYMOUS))
		panic("do_mmap: bad file");	// sys_mmap should check this

	uint64 sz = PGROUNDUP(len);
	struct seg *s, *new;
	int ret = 0;
	if (flags & MAP_FIXED) {	// if this fails, the original pages will be lost at present implement
		ret = lookup_fixed_segment(start, start + sz, &s, &new);
	} else {
		ret = lookup_segment(sz, &s, &new);
	}
	if (ret < 0)
		return ret;

	new->flag = (prot << 1) & (PTE_X|PTE_W|PTE_R);

	if (f) {
		ret = mmap_file(new, len, flags, f, off);
	} else {
		ret = mmap_anonymous(new, sz, flags);
	}

	if (ret < 0) {
		// TODO: MAP_FIXED failure rescue
		kfree(new);
		return ret;
	}

	if (s) {
		new->next = s->next;
		s->next = new;
	} else {
		__debug_assert("do_mmap", flags & MAP_FIXED, "MAP_FIXED ouch!\n");
		struct proc *p = myproc();
		new->next = p->segment;
		p->segment = new;
	}
	sfence_vma();
	return new->addr;
}

int do_munmap(uint64 start, uint64 len)
{
	uint64 sz = PGROUNDUP(len);
	uint64 end = start + sz;
	struct proc *p = myproc();
	struct seg *prev = NULL;
	struct seg *s, *s2;

	int flag = 0;
	for (s = p->segment; s != NULL; ) {
		uint64 saddr = s->addr, eaddr = s->addr + s->sz;
		if (eaddr <= start) {
			prev = s;
			s = s->next;
			continue;
		}
		if (saddr >= end)
			break;

		int overlap = 0;
		if (start > saddr) {
			saddr = start;
			overlap = 1;
		}
		if (end < eaddr) {
			eaddr = end;
			overlap = 1;
		}
		if (overlap) {
			s2 = split_segment(s, saddr, eaddr);
			if (s2 == NULL)
				panic("do_munmap: split");

			if (s2 != s) {	// after spliting, s2 is not the first part
				for (prev = s; prev->next != s2; prev = prev->next);
			}
		}

		if (s->mmap && s->mmap != (void *)MAP_ANONYMOUS) {
			__debug_info("do_munmap", "sync file: s->mmap=%p\n", s->mmap);
			msync_file(s, 1);
		}

		flag = 1;
		uvmdealloc(p->pagetable, PGROUNDDOWN(s->addr), s->addr + s->sz, s->type);
		s2 = s->next;
		if (prev)
			prev->next = s2;
		else
			p->segment = s2;

		kfree(s);
		s = s2;
	}

	if (flag)
		sfence_vma();

	return 0;
}
