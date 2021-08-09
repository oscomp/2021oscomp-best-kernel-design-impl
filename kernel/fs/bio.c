// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.

#ifndef __DEBUG_bio
#undef DEBUG
#endif

#define __module_name__ "bio"

#include "types.h"
#include "param.h"
#include "printf.h"
#include "fs/buf.h"
#include "hal/disk.h"
#include "hal/riscv.h"
#include "hal/sdcard.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"
#include "utils/debug.h"
#include "sched/proc.h"

// #define BCACHE_TABLE_SIZE 233
#define BCACHE_TABLE_SIZE 47
#define __hash_idx(idx) ((idx) % BCACHE_TABLE_SIZE)

static struct spinlock bcachelock;
static struct d_list lru_head;
static list_node_t *bcache[BCACHE_TABLE_SIZE];
static struct buf bufs[BNUM];
int nwait = 0;

void
binit(void)
{
	initlock(&bcachelock, "bcache");
	dlist_init(&lru_head);
	nwait = 0;
	for (int i = 0; i < BCACHE_TABLE_SIZE; i++)
		bcache[i] = NULL;

	for (struct buf *b = bufs; b < bufs + BNUM; b++) {
		b->refcnt = 0;
		b->sectorno = ~0;
		b->dev = ~0;
		b->hash.pprev = NULL;
		b->hash.next = NULL;
		b->dirty = 0;

		dlist_add_before(&lru_head, &b->lru);
		initsleeplock(&b->lock, "buffer");
	}
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
struct buf*
bget(uint dev, uint sectorno)
{
	struct buf *b;
	int idx = __hash_idx(sectorno);

	__debug_info("bget", "search secno=%d, idx=%d\n", sectorno, idx);
	acquire(&bcachelock);

	// Is the block already cached?
	for (list_node_t *l = bcache[idx]; l != NULL; l = l->next) {
		b = container_of(l, struct buf, hash);
		if (b->dev == dev && b->sectorno == sectorno) {
			if (l != bcache[idx]) {		// not the first node
				_list_remove(l);		// move it to the head
				_list_push_front(&bcache[idx], l);
			}
			b->refcnt++;
			release(&bcachelock);
			acquiresleep(&b->lock);
			__debug_info("bget", "buffered, secno=%d\n", sectorno);
			return b;
		}
	}

	// Not cached.
	// Recycle the least recently used (LRU) unused buffer.
	__debug_info("bget", "not buffered, secno=%d\n", sectorno);
rescan:
	for (struct d_list *dl = lru_head.prev; dl != &lru_head; dl = dlist_prev(dl)) {
		b = container_of(dl, struct buf, lru);
		if (b->refcnt == 0) {
			uint oldsec = b->sectorno;
			b->dev = dev;
			b->sectorno = sectorno;
			b->valid = 0;
			b->refcnt = 1;

			if (b->hash.pprev) {			// we were in a hash list
				_list_remove(&b->hash);
			}
			_list_push_front(&bcache[idx], &b->hash);

			release(&bcachelock);
			acquiresleep(&b->lock);
			if (b->dirty) {					// ah oh, we got a dirty one...
				disk_write(b, oldsec);		// write to its original sector
				b->dirty = 0;
			}
			return b;
		}
	}
	nwait++;
	sleep(&nwait, &bcachelock);
	nwait--;
	goto rescan;

	panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf* 
bread(uint dev, uint sectorno)
{
	struct buf *b;

	b = bget(dev, sectorno);
	// __debug_info("bread", "get buffer\n");
	if (!b->valid) {
		__debug_info("bread", "start reading sno %d\n", sectorno);
		disk_read(b);
		// __debug_info("bread", "done sno %d\n", sectorno);
		b->valid = 1;
	}
	return b;
}

// Write b's contents to disk.  Must be locked.
void 
bwrite(struct buf *b, int through)
{
	if(!holdingsleep(&b->lock))
		panic("bwrite");

	__debug_info("bwrite", "sectorno %d\n", b->sectorno);
	if (through == BWRITE_THROUGH) {
		disk_write(b, b->sectorno);
		b->dirty = 0;
	} else
		b->dirty = 1;

	b->valid = 1;
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
	if(!holdingsleep(&b->lock))
		panic("brelse");

	releasesleep(&b->lock);

	acquire(&bcachelock);
	b->refcnt--;
	if (b->refcnt == 0) {
		// no one is waiting for it.
		dlist_del(&b->lru);
		dlist_add_after(&lru_head, &b->lru);

		// available buf for someone who's in sleep-wait
		if (nwait > 0)
			wakeup(&nwait);
	}
	
	release(&bcachelock);
}

void bsync(void)
{
	struct buf *b;

	acquire(&bcachelock);
	for (b = bufs; b < bufs + BNUM; b++) {
		
		/**
		 * Pin a ref here in case of the eviction in bget().
		 * If a buf's refcnt is 0, it might be evicted and got
		 * its sectorno field changed in bget(), which will
		 * make our next step wrong.
		 */
		b->refcnt++;
		release(&bcachelock);
		
		/**
		 * We are good if under the protection of the sleeping lock.
		 * We can safely read the fields we want. If the buf is busy,
		 * ignore it.
		 */
		if (trysleeplock(&b->lock) == 0) {
			if (b->dirty) {
				if (!b->valid)
					panic("bsync: dirty but not valid buf");

				disk_write(b, b->sectorno);
				b->dirty = 0;
			}
			releasesleep(&b->lock);
		}

		acquire(&bcachelock);
		b->refcnt--;
	}
	release(&bcachelock);
}

/*
int breads(struct buf *bufs[], int nbuf)
{
	int i, j;
	for (i = 0; i < nbuf && bufs[i]->valid; i++);
	for (j = nbuf - 1; j > i && bufs[j]->valid; j--);

	int ret = 0;
	if (i <= j) {
		// for (int k = i; k <= j; k++)
		// 	printf("the %d buf: sec %d\n", k, bufs[k]->sectorno);
		ret = disk_read_multi_blk(&bufs[i], j - i + 1);
		if (ret >= 0)
			for (; i <= j; i++)
				bufs[i]->valid = 1;
	}
	return ret;
}
*/

void bprint()
{
	acquire(&bcachelock);
	printf("\nbuf cache:\n");
	for (int i = 0; i < BCACHE_TABLE_SIZE; i++) {
		list_node_t *l = bcache[i];
		if (l == NULL) { continue; }
		printf(__INFO("%d")" ", i);
		for (; l != NULL; l = l->next) {
			struct buf *b = container_of(l, struct buf, hash);
			printf("%d ", b->sectorno);
		}
		printf("\n");
	}
	release(&bcachelock);
}
