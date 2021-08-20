#ifndef __DEBUG_mmap
#undef DEBUG
#endif

#define __module_name__	"mmap"

#include "types.h"
#include "hal/riscv.h"
#include "memlayout.h"
#include "mm/vm.h"
#include "mm/pm.h"
#include "mm/kmalloc.h"
#include "mm/mmap.h"
#include "mm/usrmm.h"
#include "sched/proc.h"
#include "utils/string.h"
#include "utils/debug.h"
#include "errno.h"


/**
 * Simple implement of anonymous file.
 * The reason to apply this is that the lazy
 * mechanism requests. When a proc forking,
 * the shared anonymous might not be mapping
 * yet, so the parent and the children need
 * a file to keep those mmap pages.
 */

struct anonfile {
	struct spinlock	lock;
	struct rb_root	mapping;	// for mmap_page
	uint			ref;
};

static struct anonfile *alloc_anonfile(void)
{
	struct anonfile *f = kmalloc(sizeof(struct anonfile));
	if (!f)
		return NULL;

	__debug_info("alloc_anonfile", "allocate one: %p\n", f);
	initlock(&f->lock, "anonfile");
	f->mapping.rb_node = NULL;
	f->ref = 1;
	return f;
}

static struct anonfile *dup_anonfile(struct anonfile *fp)
{
	acquire(&fp->lock);
	fp->ref++;
	release(&fp->lock);
	return fp;
}

static void close_anonfile(struct anonfile *fp)
{
	acquire(&fp->lock);
	fp->ref--;
	if (fp->ref == 0) {
		if (fp->mapping.rb_node != NULL)
			panic("close_anonfile: rbtree not empty");
		release(&fp->lock);
		__debug_info("close_anonfile", "free one: %p\n", fp);
		kfree(fp);
		return;
	}
	release(&fp->lock);
}


/**
 * Red black tree approaches.
 */

static struct mmap_page *get_mmap_page(struct rb_root *root, uint64 off)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct mmap_page *map = rb_entry(node, struct mmap_page, rb);
		if (off < map->f_off)
			node = node->rb_left;
		else if (off > map->f_off)
			node = node->rb_right;
		else
			return map;
	}

	return NULL;
}

static void put_mmap_page(struct mmap_page *map, struct rb_root *root)
{
	map->ref--;
	if (map->ref == 0) {
		rb_erase(&map->rb, root);
		if (map->pa && pageput((uint64)map->pa) == 0)
			freepage(map->pa);
		kfree(map);
	}
}

static struct mmap_page *get_mmap_with_parent(struct rb_root *root,
								uint64 off, struct rb_node **pparent,
								struct rb_node ***pplink)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	
	while (*p) {
		parent = *p;
		struct mmap_page *map = rb_entry(parent, struct mmap_page, rb);
		if (off < map->f_off)
			p = &parent->rb_left;
		else if (off > map->f_off)
			p = &parent->rb_right;
		else
			return map;
	}

	*pparent = parent;
	*pplink = p;
	return NULL;
}

static struct mmap_page *get_adjacent_page(struct mmap_page *map, int prev)
{
	struct rb_node *node;
	node = prev ? rb_prev(&map->rb) : rb_next(&map->rb);
	if (!node)
		return NULL;

	return rb_entry(node, struct mmap_page, rb);
}

void imapdel(struct inode *ip)
{
	struct rb_node *parent = NULL;
	struct rb_node *node = ip->mapping.rb_node;
	struct mmap_page *map;

	if (node == NULL)
		return;

	acquire(&ip->ilock);
	while (node != NULL) {
		if (node->rb_left)
			node = node->rb_left;
		else if (node->rb_right)
			node = node->rb_right;
		else {
			parent = rb_parent(node);
			if (parent) {
				if (node == parent->rb_left)
					parent->rb_left = NULL;
				else
					parent->rb_right = NULL;
			}
			map = rb_entry(node, struct mmap_page, rb);
			if (map->ref != 0)
				panic("imapdel: mmap page ref");
			if (map->pa) {
				if (pageput((uint64)map->pa) != 0)
					panic("imapdel: page ref");
				freepage(map->pa);
			}
			kfree(map);
			node = parent;
		}
	}

	ip->mapping.rb_node = NULL;
	release(&ip->ilock);
}

/**
 * Functions applied when processes fork or exit.
 */

static void __anon_mmapdel(struct seg *seg)
{
	if (!MMAP_SHARE(seg->mmap))
		return;

	struct anonfile *fp = MMAP_FILE(seg->mmap);
	uint64 off = seg->f_off;
	uint64 end = off + seg->sz;

	acquire(&fp->lock);
	for (; off < end; off += PGSIZE) {
		struct mmap_page *map = get_mmap_page(&fp->mapping, off);
		if (map == NULL)
			panic("__anon_mmapdel: no map node\n");
		
		put_mmap_page(map, &fp->mapping);
	}
	release(&fp->lock);

	close_anonfile(fp);
	seg->mmap = NULL;
}

static void __file_mmapdel(struct seg *seg, int sync)
{
	__debug_info("__file_mmapdel", "enter seg=%p, sync=%d\n", seg, sync);

	struct file *fp = MMAP_FILE(seg->mmap);
	if (!MMAP_SHARE(seg->mmap))
		goto out;

	struct inode *ip = fp->ip;
	uint64 off = seg->f_off;
	uint64 end = off + seg->sz;
	struct mmap_page *map;

	acquire(&ip->ilock);
	
	while ((map = get_mmap_page(&ip->mapping, off)) == NULL) {
		off += PGSIZE;
		if (off >= end)
			break;
	}
	while (map && (off = map->f_off) < end) {
		if (sync && (seg->flag & PTE_W) && map->pa && off < ip->size) {
			// uint64 len = (ip->size - off < map->f_len) ?
			// 				ip->size - off : map->f_len;
			uint64 len = (ip->size - off < PGSIZE) ?
							ip->size - off : PGSIZE;
			release(&ip->ilock);
			ilock(ip);
		
			// This op may fail, but what can we do about it?
			if (map->valid) {
				__debug_info("__file_mmapdel", "writing: off=%p, len=%d\n", off, map->f_len);
				ip->fop->write(ip, 0, (uint64)map->pa, off, len);
			}
			iunlock(ip);
			acquire(&ip->ilock);
		}

		map->ref--;
		map = get_adjacent_page(map, 0);
	}
	release(&ip->ilock);

out:
	fileclose(fp);
}

void mmapdel(struct seg *seg, int sync)
{
	__debug_assert("mmapdel", seg->type == MMAP, "funny type %d\n", seg->type);
	__debug_info("mmapdel", "off=%p, sz=%d\n", seg->f_off, seg->sz);
	
	if (MMAP_ANONY(seg->mmap)) {
		__anon_mmapdel(seg);
	} else {
		__file_mmapdel(seg, sync);
	}
}

static void __anon_mmapdup(struct seg *seg)
{
	if (!MMAP_SHARE(seg->mmap))
		return;

	struct anonfile *fp = MMAP_FILE(seg->mmap);
	uint64 off = seg->f_off;
	uint64 end = off + seg->sz;

	acquire(&fp->lock);
	struct mmap_page *map = get_mmap_page(&fp->mapping, off);
	for (; off < end; off += PGSIZE) {
		if (map == NULL || map->f_off != off)
			panic("__anon_mmapdup: no map node\n");
		map->ref++;
		map = get_adjacent_page(map, 0);
	}
	release(&fp->lock);

	dup_anonfile(fp);
}

static void __file_mmapdup(struct seg *seg)
{
	struct file *fp = MMAP_FILE(seg->mmap);
	if (!MMAP_SHARE(seg->mmap))
		goto out;

	struct inode *ip = fp->ip;
	uint64 off = seg->f_off;
	uint64 end = off + seg->sz;
	struct mmap_page *map;

	acquire(&ip->ilock);
	while ((map = get_mmap_page(&ip->mapping, off)) == NULL) {
		off += PGSIZE;
		if (off >= end)
			break;
	}
	while (map && map->f_off < end) {
		map->ref++;
		map = get_adjacent_page(map, 0);
	}
	release(&ip->ilock);

out:
	filedup(fp);
}

void mmapdup(struct seg *seg)
{
	__debug_assert("mmapdup", seg->type == MMAP,
				"bad type: %d\n", seg->type);
	__debug_info("mmapdup", "off=%p, sz=%d\n", seg->f_off, seg->sz);
	if (MMAP_ANONY(seg->mmap)) {
		__anon_mmapdup(seg);
	} else {
		__file_mmapdup(seg);
	}
}


/**
 * System call support.
 */

static void __mmap_filedup(uint64 mmap)
{
	if (MMAP_ANONY(mmap)) {
		struct anonfile *fp = MMAP_FILE(mmap);
		if (fp)
			dup_anonfile(fp);
	} else {
		filedup(MMAP_FILE(mmap));
	}
}

/**
 * Cut the given segment at start and end. There are three cases to do this:
 * 1. seg is covered by [start, end], then no split at all
 * 2. it's either start or end that falls in the range of seg, then split seg into two
 * 3. both start and end fall in the range of seg, then split it into three
 * In any case discussed above, this returns the spliting seg in [start, end].
 */
static struct seg *split_segment(struct seg *s, uint64 start, uint64 end)
{
	uint64 segend = s->addr + s->sz;
	if (end < start || (start <= PGROUNDDOWN(s->addr) && end >= PGROUNDUP(segend))) {
		__debug_warn("split_segment", "range [0x%x, 0x%x] is cover seg:{addr=%p, size=0x%x}\n",
						start, end, s->addr, s->sz);
		return s;
	}

	struct seg *s1, *s2 = NULL;
	start = PGROUNDDOWN(start);
	end = PGROUNDUP(end);
	s1 = kmalloc(sizeof(struct seg));
	if (s1 == NULL)
		return NULL;
	
	// split into three part
	if (start > s->addr && end < segend) {
		if ((s2 = kmalloc(sizeof(struct seg))) == NULL) {
			kfree(s1);
			return NULL;
		}
	}
	
	*s1 = *s;
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

		if (s->type == MMAP) {
			__mmap_filedup(s->mmap);
			s1->f_off += spliter - s->addr;
		}

		s1->next = s->next;
		s->next = s1;
	}
	else {
		*s2 = *s;
		s2->addr = start;
		s2->sz = end - start;
		s1->addr = end;
		s1->sz = segend - end;
		s->sz = start - s->addr;

		if (s->type == MMAP) {
			__mmap_filedup(s->mmap);	// twice for two segs
			__mmap_filedup(s->mmap);
			s1->f_off += end - s->addr;
			s2->f_off += start - s->addr;
		}

		s1->next = s->next;
		s2->next = s1;
		s->next = s2;
	}

	return s2;
}

// static int msync_file(struct seg *s, int close)
// {
// 	if (MMAP_ANONY(s->mmap)) {
// 		if (close) {
// 			__anonymous_del(myproc()->pagetable, s);
// 		}
// 		return 0;
// 	}

// 	struct file *fp = MMAP_FILE(s->mmap);
// 	int ret = 0;
// 	if (MMAP_SHARE(s->mmap) && (s->flag & PTE_W)) { // A shared map, carry through to the file.
// 		struct inode *ip = fp->ip;
// 		uint64 off = s->f_off;
// 		uint64 const end = s->f_off + s->sz;

// 		for (; off < end; off += PGSIZE) {
// 			acquire(&ip->ilock);
// 			struct mmap_page *map = get_mmap_page(&ip->mapping, off);
// 			release(&ip->ilock);
			
// 			/**
// 			 * Because of lazy mmap, the page might haven't been allocated yet.
// 			 * Just ignore this one.
// 			 */
// 			if (map == NULL)
// 				continue;

// 			/**
// 			 * The mapping file might be changed by other syscall like write(),
// 			 * so when synchronizing the file, something strange might happen.
// 			 * Similar situation would occur while handling the lazy loading.
// 			 */
// 			if (off + map->f_len > ip->size) {
// 				ret = -EIO;
// 			} else if (ret >= 0) {
// 				// This op may fail, but what can we do about it?
// 				ilock(ip);
// 				ip->fop->write(ip, 0, map->pa, off, map->f_len);
// 				iunlock(ip);
// 			}
		
// 			if (close) {
// 				acquire(&ip->ilock);
// 				put_mmap_page(map, &ip->mapping);
// 				release(&ip->ilock);
// 			}
// 		}
	
// 	}

// 	if (close) {
// 		fileclose(fp);
// 		s->mmap = NULL;
// 	}

// 	return ret;
// }

/**
 * Fixed mapping needs to delete existing mapping.
 * This function only creates new segment, for the list of segs
 * that need to delete, we store the previous seg of the first
 * of them in pprev, and store the new seg in pnew. Besides,
 * new seg's next is the seg that we don't delete. Like this:
 *         delete
 *         /     \
 * s0 --> s1 --> s2 --> s3
 *  \                   /
 *  prev            new->next
 */
static int lookup_fixed_segment(uint64 start, uint64 end,
		struct seg **pprev, struct seg **pnew)
{
	if (end > MAXUVA) {
		__debug_warn("lookup_fixed_segment", "Not enough user space in mmap!\n");
		return -EINVAL;
	}
	if (end < start)
		return -EOVERFLOW;


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

			if (s2 != s) {
				/**
				 * After spliting, s2 is not the first part.
				 * Adjust prev and s. Once this happened,
				 * if we still have some segs to split,
				 * s2 should always be s, because we maintain
				 * seg list in order. In other words, prev is settled.
				 */
				for (prev = s; prev->next != s2; prev = prev->next);
				s = s2;
			}
			s = s -> next;
		}
	}

	*pprev = prev;

	s2 = kmalloc(sizeof(struct seg));
	if (s2 == NULL)
		return -ENOMEM;

	memset(s2, 0, sizeof(struct seg));
	s2->addr = start;
	s2->sz = end - start;
	s2->type = MMAP;
	s2->next = s;
	*pnew = s2;

	return 0;
}


static int lookup_segment(uint64 sz, struct seg **pprev, struct seg **pnewseg)
{
	struct proc *p = myproc();
	struct seg *s = getseg(p->segment, STACK);
	uint64 base = VUMMAP;
	if (base + sz > MAXUVA) {
		__debug_warn("lookup_segment", "Not enough user space in mmap!\n");
		return -EINVAL;
	}
	if (base + sz < base)
		return -EOVERFLOW;

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
	newseg->next = s->next;
	*pprev = s;
	*pnewseg = newseg;
	return 0;
}

static int mmap_file(struct seg *s, uint64 len, int flags, struct file *f, int64 off)
{
	// struct inode *ip = f->ip;

	s->f_off = off;
	s->f_sz = 0;	// this is for elf, we don't care about it here
	s->mmap = (uint64) filedup(f);

	if (!(flags & MAP_SHARED))
		return 0;
/*
	uint64 n;
	struct mmap_page *map;
	struct rb_node *parent;
	struct rb_node **plink;

	acquire(&ip->ilock);
	for (n = 0; n < len; n += PGSIZE) {
		map = get_mmap_with_parent(&ip->mapping, off + n, &parent, &plink);
		if (map) {
			map->ref++;
		} else {
			map = kmalloc(sizeof(struct mmap_page));
			if (map == NULL)
				goto fail;

			map->f_off = off + n;
			map->f_len = (PGSIZE < len - n) ? PGSIZE : len - n;
			map->pa = NULL;
			map->ref = 1;
			map->valid = 0;
			rb_link_node(&map->rb, parent, plink);
			rb_insert_color(&map->rb, &ip->mapping);
		}
	}
	release(&ip->ilock);
*/
	s->mmap |= MMAP_SHARE_FLAG;
	return 0;

/*
fail:
	for (n -= PGSIZE; n >= 0; n -= PGSIZE) {
		map = get_mmap_page(&ip->mapping, off + n);
		put_mmap_page(map, &ip->mapping);
	}
	release(&ip->ilock);
	fileclose(f);
	return -ENOMEM;
*/
}

static int mmap_anonymous(struct seg *s, int flags)
{
	/**
	 * We can map the first page referring to locality.
	 * Other pages will be mapped in page fault handler.
	 * But there may be some original mappings in the pagetable,
	 * We'd better leave all the work for page fault handler.
	 */

	if (!(flags & MAP_SHARED)) {
		s->mmap = NULL;
		goto out;
	}

	struct anonfile *fp = alloc_anonfile();
	if (!fp)
		return -ENOMEM;

	uint64 off;
	struct mmap_page *map;
	struct rb_node *parent;
	struct rb_node **plink;

	/**
	 * Build a mmap pages tree at the beginning, the tree nodes
	 * will keep references of the pages (not now), in case of
	 * those shared mapping pages are free due to some processes
	 * exit(). There may be still other processes who are mapping
	 * with those pages, but haven't really mapped them into the
	 * pagetable yet.
	 */
	for (off = 0; off < s->sz; off += PGSIZE) {
		map = kmalloc(sizeof(struct mmap_page));
		if (map == NULL)
			goto fail;

		map->f_off = off;
		map->f_len = PGSIZE;
		map->pa = NULL;
		map->ref = 1;
		map->valid = 0;

		// shouldn't get result since we haven't put "off" into it
		if (get_mmap_with_parent(&fp->mapping, off, &parent, &plink))
			panic("mmap_anonymous: rbtree");
		rb_link_node(&map->rb, parent, plink);
		rb_insert_color(&map->rb, &fp->mapping);
	}

	s->mmap = MMAP_SHARE_FLAG | (uint64)fp;

out:
	s->mmap |= MMAP_ANONY_FLAG;
	s->f_off = 0;
	s->f_sz = 0;

	return 0;

fail:
	for (off -= PGSIZE; off >= 0; off -= PGSIZE) {
		map = get_mmap_page(&fp->mapping, off);
		put_mmap_page(map, &fp->mapping);
	}
	close_anonfile(fp);
	return -ENOMEM;
}

uint64 do_mmap(uint64 start, uint64 len, int prot, int flags, struct file *f, int64 off)
{
	if (f) {
		struct inode *ip = f->ip;
		if (off >= ip->size) {
			__debug_warn("mmap", "length overflows!\n");
			return -EINVAL;
		}
		if (S_ISDIR(ip->mode))
			return -EISDIR;
		if ((f->readable ^ (prot & PROT_READ)) || (f->writable ^ ((prot & PROT_WRITE) >> 1))) {
			__debug_warn("mmap", "unmatched priviledge: w/r=%d/%d, prot x/w/r=%d/%d/%d\n",
						f->writable, f->readable,
						!!(prot & PROT_EXEC), !!(prot & PROT_WRITE), !!(prot & PROT_READ));
			return -EPERM;
		}
	} else if (!(flags & MAP_ANONYMOUS))
		panic("do_mmap: bad file");	// sys_mmap should check this

	uint64 sz = PGROUNDUP(len);
	struct seg *prev, *new;
	int ret = 0;
	if (flags & MAP_FIXED)
		ret = lookup_fixed_segment(start, start + sz, &prev, &new);
	else
		ret = lookup_segment(sz, &prev, &new);

	if (ret < 0)
		return ret;

	new->flag = (prot << 1) & (PTE_X|PTE_W|PTE_R);

	if (f)
		ret = mmap_file(new, len, flags, f, off);
	else
		ret = mmap_anonymous(new, flags);

	if (ret < 0) {
		kfree(new);
		return ret;
	}

	/**
	 * Clean up existing mapping if it is fixed mapping.
	 * Otherwise, we have prev->next == new->next, so the
	 * deletion below won't be carried.
	 */
	struct proc *p = myproc();
	struct seg *del = prev ? prev->next : p->segment;
	while (del != new->next) {
		del = delseg(p->pagetable, del);
	}

	if (prev) {
		prev->next = new;
	} else {
		p->segment = new;
	}
	sfence_vma();
	return new->addr;
}

int do_munmap(uint64 start, uint64 len)
{
	uint64 sz = PGROUNDUP(len);
	uint64 end = start + sz;
	struct proc *p;
	struct seg *prev = NULL;
	struct seg *s, *del;
	int ret;

	if (end < start)
		return -EOVERFLOW;

	/**
	 * This function takes care of the seg-split problem,
	 * which allows us to take advantage of it: simply free
	 * the segs it returns to us. But it also returns a new
	 * seg we don't use at all, just free it at last.
	 */
	ret = lookup_fixed_segment(start, end, &prev, &s);
	if (ret < 0)
		return ret;

	p = myproc();
	del = prev ? prev->next : p->segment;
	ret = 0;
	if (del != s->next) {
		ret = 1;
		while (del != s->next) {
			del = delseg(p->pagetable, del);
		}
	}

	if (ret) {	// it did delete some segs
		if (prev) {
			prev->next = s->next;
		} else {
			p->segment = s->next;
		}
		sfence_vma();
	}

	kfree(s);
	return 0;
}


int do_msync(uint64 addr, uint64 len, int flags)
{
	struct proc *p = myproc();
	struct seg *s = partofseg(p->segment, addr, addr + len);
	if (s == NULL)
		return -EFAULT;

	// private mapping and anonymous mapping can not be synchronized
	if (s->type != MMAP || MMAP_ANONY(s->mmap) || !MMAP_SHARE(s->mmap))
		return 0;

	// MS_INVALIDATE does nothing on our system
	if (!(flags & (MS_ASYNC|MS_SYNC))) {
		return -EPERM;
		// goto out;
	}

	struct file *f = MMAP_FILE(s->mmap);
	struct inode *ip = f->ip;
	int ret = 0;

	uint64 foff = s->f_off + (addr - s->addr);
	uint64 sz = s->sz < len ? s->sz : len;
	uint64 end = foff + sz;

	acquire(&ip->ilock);
	for (; foff < end; foff += PGSIZE) {
		struct mmap_page *map = get_mmap_page(&ip->mapping, foff);
		if (map == NULL)
			panic("__file_mmapdel: no map node\n");
		
		if (map->pa && foff < ip->size) {
			uint64 flen = (ip->size - foff < map->f_len) ?
							ip->size - foff : map->f_len;
			release(&ip->ilock);
			ilock(ip);
		
			// This op may fail, but what can we do about it?
			if (map->valid) {
				ip->fop->write(ip, 0, (uint64)map->pa, foff, flen);
			}
			iunlock(ip);
			acquire(&ip->ilock);
		}
	}
	release(&ip->ilock);

// out:
	return ret;
}


/**
 * Page fault handler, the real allocator.
 */

static int handle_anonymous_shared(uint64 badaddr, struct seg *s)
{
	struct anonfile *fp = MMAP_FILE(s->mmap);
	
	uint64 off = s->f_off + (PGROUNDDOWN(badaddr) - s->addr);
	struct mmap_page *map;
	
	acquire(&fp->lock);
	map = get_mmap_page(&fp->mapping, off);
	if (map == NULL)
		panic("handle_anonymous_shared: no map node");

	if (map->pa == NULL) {
		map->pa = allocpage();
		if (map->pa == NULL) {
			release(&fp->lock);
			return -ENOMEM;
		}
		memset(map->pa, 0, PGSIZE);
		pagereg((uint64)map->pa, 1);	// keep a reference on the page
	}
	release(&fp->lock);

	struct proc *p = myproc();
	if (mappages(p->pagetable, PGROUNDDOWN(badaddr), PGSIZE,
				(uint64)map->pa, s->flag|PTE_U) < 0)
		return -ENOMEM;

	sfence_vma();
	return 0;
}
/*
static void *__page_file_swap(struct inode *ip, uint64 foff, uint64 badaddr)
{
	struct proc *p = myproc();
	struct seg *s = locateseg(p->segment, badaddr);
	uint64 start = s->f_off;	// start offset
	uint64 end = start + s->sz;	// end offset
	void *page = NULL;

	if (s->sz == PGSIZE)		// can't spare any page
		return NULL;

	acquire(&ip->ilock);

	uint64 off = (foff == start) ? end - PGSIZE : foff - PGSIZE;
	struct mmap_page *map = get_mmap_page(&ip->mapping, off);
	int miss = 0, free = 0;

	while (off != foff)
	{
		if (map->ref == 1 && map->pa) {	// only us
			void *victim = map->pa;
			map->pa = NULL;
			release(&ip->ilock);

			ilock(ip);
			if ((s->flag & PTE_W) && map->valid && off < ip->size) {
				uint64 len = (ip->size - off < map->f_len) ?
								ip->size - off : map->f_len;
				ip->fop->write(ip, 0, (uint64)victim, off, len);
			}
			map->valid = 0;
			iunlock(ip);

			unmappages(p->pagetable, s->addr + (off - s->f_off), 1, VM_USER|VM_FREE);
			
			if (pageput((uint64)victim) != 0)
				panic("__page_file_swap: bad page ref");
			if (!page)
				page = victim;
			else
				freepage(victim);

			free++;
			miss = 0;

			acquire(&ip->ilock);
		} else {
			miss++;
			if (free >= 10 && miss >= 5) {		// enough
				printf(__INFO("mmap file swap")" evict enough %d\n", free);
				break;
			}
		}
		if (off == start) {
			off = end - PGSIZE;
			map = get_mmap_page(&ip->mapping, off);
		} else {
			off -= PGSIZE;
			map = get_adjacent_page(map, 1);
		}
		// if (!map || map->f_off != off)
		// 	panic("__page_file_swap: bad map node");
	}
	release(&ip->ilock);

	return page;
}
*/
static int __page_file_read(struct inode *ip, uint64 off, uint64 page)
{
	if (off >= ip->size) {	// offset out of file size
		return -EIO;				
	}
	uint64 len = (PGSIZE < ip->size - off) ? PGSIZE : ip->size - off;

	/**
	 * This read could meet some problem. If the result len is not
	 * equal to the expected len, should we sail that's a fault?
	 * or just ignore this? We choose the later at present.
	 */
	len = ip->fop->read(ip, 0, page, off, len);
	if (len < PGSIZE)
		memset((void *)(page + len), 0, PGSIZE - len);

	return 0;
}

static int handle_file_mmap(uint64 badaddr, struct seg *s)
{
	int share = MMAP_SHARE(s->mmap);
	struct file *fp = MMAP_FILE(s->mmap);
	struct inode *ip = fp->ip;
	uint64 off = s->f_off + (PGROUNDDOWN(badaddr) - s->addr);
	struct mmap_page *map;
	struct rb_node *parent = NULL;
	struct rb_node **plink = NULL;
	void *pa;

	acquire(&ip->ilock);
	map = get_mmap_with_parent(&ip->mapping, off, &parent, &plink);

	if (share) {
		if (map) {
			map->ref++;
		} else {
			if (off >= ip->size) {
				release(&ip->ilock);
				return -EIO;
			}
			map = kmalloc(sizeof(struct mmap_page));
			if (map == NULL) {
				release(&ip->ilock);
				return -ENOMEM;
			}

			map->f_off =off;
			// map->f_len = (PGSIZE < len - n) ? PGSIZE : len - n;
			map->f_len = PGSIZE;
			map->pa = NULL;
			map->ref = 1;
			map->valid = 0;
			rb_link_node(&map->rb, parent, plink);
			rb_insert_color(&map->rb, &ip->mapping);
		}
		/**
		 * Here is another problem. The first fault allocates a page here,
		 * then releases the ilock to read the file. At this moment, the
		 * second fault (by other process) arrives, sees that the page is
		 * ready, then maps and leaves, which could totally happen before
		 * the first one finishing its reading.
		 * So we use a 'valid' field and the sleeplock to sync this.
		 */
		if (map->pa == NULL) {
			pa = allocpage();
			if (pa == NULL) {
				release(&ip->ilock);
				// pa = __page_file_swap(ip, off, badaddr);
				// if (pa == NULL)
					return -ENOMEM;

				// acquire(&ip->ilock);
				// if (map->pa == NULL) {
				// 	pagereg((uint64)pa, 1);
				// 	map->pa = pa;
				// } else {
				// 	freepage(pa);
				// 	pa = map->pa;
				// }
			} else {
				pagereg((uint64)pa, 1);		// keep refs on shared mappings
				map->pa = pa;
			}
		} else {
			pa = map->pa;
		}
		release(&ip->ilock);
		
		/**
		 * There is the page, but is it valid? That means the contents of the
		 * file may be still on disk. Or aren't they? That's hard to answer.
		 * The reading might fail due to the exceeding offset or other reasons,
		 * leaving an empty page without proper file contents.
		 * The 'valid' flag is to deal with this case. Besides, the sleeplock
		 * solves the problem we named above.
		 */
		ilock(ip);
		if (!map->valid) {
			if (__page_file_read(ip, off, (uint64)pa) < 0) {
				iunlock(ip);
				return -EIO;
			}
			map->valid = 1;
		}
		iunlock(ip);
	}
	else {
		pa = allocpage();
		if (!pa) {
			release(&ip->ilock);
			return -ENOMEM;
		}
		pagereg((uint64)pa, 0);				// don't keep refs on private mappings

		if (map && map->pa && map->valid) {	// copy from shared page if existing
			memmove(pa, map->pa, PGSIZE);
			release(&ip->ilock);
		} else {
			release(&ip->ilock);
			ilock(ip);
			if (__page_file_read(ip, off, (uint64)pa) < 0) {
				iunlock(ip);
				freepage(pa);
				return -EIO;
			}
			iunlock(ip);
		}
	}
	
	struct proc *p = myproc();
	if (mappages(p->pagetable, PGROUNDDOWN(badaddr), PGSIZE,
				(uint64)pa, s->flag|PTE_U) < 0)
	{
		if (!share)
			freepage(pa);	
		return -ENOMEM;
	}

	sfence_vma();
	return 0;
}

/**
 * @param	kind	load-0 | store-1 | execute-2
 */
int handle_page_fault_mmap(int kind, uint64 badaddr, struct seg *s)
{
	int illegel;
	switch (kind) {
		case 0: illegel = !(s->flag & PTE_R); break;
		case 1: illegel = !(s->flag & PTE_W); break;
		case 2: illegel = !(s->flag & PTE_X); break;
		default:
			illegel = 0;
			panic("handle_page_fault_mmap: kind");
	}
	if (illegel) {	// the access broke the protection
		__debug_warn("handle_page_fault_mmap", "illegal access: kind=%d, prot=0x%x\n",
					kind, s->flag);
		return -EFAULT;
	}

	if (MMAP_ANONY(s->mmap)) {
		if (!MMAP_SHARE(s->mmap)) {		// private mapping, just like heap
			struct proc *p = myproc();
			uint64 pa = PGROUNDDOWN(badaddr);

			if (uvmalloc(p->pagetable, pa, pa + PGSIZE, s->flag) == 0)
				return -ENOMEM;

			sfence_vma();
			return 0;
		}
		else
			return handle_anonymous_shared(badaddr, s);
	}

	return handle_file_mmap(badaddr, s);
}
