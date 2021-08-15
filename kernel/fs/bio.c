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

#if BNUM >= 2000
#define BCACHE_TABLE_SIZE 233
#elif BNUM >= 1000
#define BCACHE_TABLE_SIZE 131
#else
#define BCACHE_TABLE_SIZE 47
#endif

#define __hash_idx(idx) ((idx) % BCACHE_TABLE_SIZE)

/**
 * Dirty buffer write back no-block mechanism.
 * 
 * When we need to write buf to disk, we don't wait.
 * Just throw it to the disk driver. Every time when a
 * disk interrupt comes, the driver will start the next
 * write task automatically.
 * 
 * Note that when those dirty bufs are sending to disk,
 * we don't hold their sleeplock, which might block us.
 * No matter whether there is another process who holds
 * some of them and reads/writes the data, we just carry
 * the data to disk without any damage of the data.
 * But if someone is writing the buf, we might carry the
 * unfinish work to disk. However, this seems to be
 * harmless.
 * 
 * Besides, we don't worry the eviction, because all
 * dirty bufs are not in the lru list and evictions occur
 * only on lru list in bget().
 */


// This lock protects @lru_head, @bcache, nwait
// and some other buf fields (see buf.h).
static struct spinlock bcachelock;

// @lru_head can only access unused bufs,
// while @bcache can get any valid ones.
static struct d_list lru_head;
static list_node_t *bcache[BCACHE_TABLE_SIZE];

// If we run out all the bufs, we have to wait.
// This is the number of waiting procs.
int nwait = 0;

// Bufs pool.
static struct buf bufs[BNUM];


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
		b->hash.pprev = NULL;
		b->hash.next = NULL;
		b->dirty = 0;
		b->disk = 0;

		dlist_add_before(&lru_head, &b->list);
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

	acquire(&bcachelock);

	/**
	 * Is the block already cached? Look up in the hash list at first.
	 * No matter which list the target buf stays now, we can always get
	 * it from the hash list (as long as it is cached).
	 */
rescan:
	for (list_node_t *l = bcache[idx]; l != NULL; l = l->next) {
		b = container_of(l, struct buf, hash);
		if (b->dev == dev && b->sectorno == sectorno) {
			if (l != bcache[idx]) {		// not the first node
				_list_remove(l);		// move it to the head (also lru)
				_list_push_front(&bcache[idx], l);
			}
			if (b->refcnt++ == 0) {
				/**
				 * We are the first visitor.
				 * This indicates that this buf is in the @lru_head list.
				 * We need to move it out in case of being evicted.
				 * The last one who keep the buf will push it back.
				 */
				dlist_del(&b->list);
			}
			release(&bcachelock);
			acquiresleep(&b->lock);
			return b;
		}
	}

	/**
	 * We are here due to cache miss.
	 * Just get one from the tail of @lru_head list.
	 * This may cause an eviction, but it is safe.
	 */
	struct d_list *dl = lru_head.prev;
	if (dl != &lru_head) {
		b = container_of(dl, struct buf, list);
		b->dev = dev;
		b->sectorno = sectorno;
		b->refcnt = 1;

		if (b->hash.pprev) {		// if we were in a hash list
			_list_remove(&b->hash);
		}
		_list_push_front(&bcache[idx], &b->hash);
		dlist_del(&b->list);

		acquiresleep(&b->lock);		// this should not block
		b->valid = 0;
		release(&bcachelock);		// or we will sched() with this spinlock
		return b;
	}

	/**
	 * We are here because the @lru_head list is
	 * empty, we have to go to sleep and wait.
	 * When we wake up, we need to re-scan both hash
	 * and lru list, in case of some procs who need
	 * the identical buf and wake up before us,
	 * taking a buf then insert into the hash list.
	 */
	nwait++;
	// printf("\n"__INFO("bget")" sleep\n");
	sleep(&nwait, &bcachelock);
	nwait--;
	goto rescan;
}

// Return a locked buf with the contents of the indicated block.
struct buf* 
bread(uint dev, uint sectorno)
{
	struct buf *b;
	b = bget(dev, sectorno);
	if (!b->valid) {
		disk_read(b);
		b->valid = 1;
	}
	return b;
}

// Write b's contents to disk.  Must be locked.
void 
bwrite(struct buf *b)
{
	/**
	 * This function doesn't behave the same as the old one
	 * any more. In our case, we only commit the buf to the
	 * disk driver and directly come back without waiting.
	 * Most importantly, we release the buf in this function
	 * instead of brelse(). For two reasons:
	 * 1. bread() and brelse() make a pair for read cases,
	 * while bread() (or bget() if we'll write cover the buf)
	 * and bwrite() make the other pair for write cases.
	 * 2. We need different operations.
	 */
	
	int res;
	/**
	 * Submit a write request to disk driver. If get positive
	 * result, it means the buf it new in driver's queue, so
	 * we need to keep a ref for the driver.
	 */
	res = disk_submit(b);
	b->valid = 1;
	releasesleep(&b->lock);
	if ((res & 0x1) == 0)
		bput(b);
	if ((res & 0x2) == 0)
		disk_write_start();
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
	releasesleep(&b->lock);
	bput(b);
}

void bsync(void)
{
	disk_write_start();
}

int breads(struct buf *bufs[], int nbuf)
{
	int i = 0, j, ret = 0;

	// find consecutive sectors
	while (i < nbuf) {
		if (!bufs[i]->valid) {
			for (j = i + 1;
				j < nbuf && !bufs[j]->valid &&
				bufs[j]->sectorno == bufs[j - 1]->sectorno + 1;
				j++);
			j--;
			if (i == j)
				ret = disk_read(bufs[i]);
			else
				ret = disk_multiple_read(&bufs[i], j - i + 1);
			if (ret < 0)
				break;
			while (i <= j)
				bufs[i++]->valid = 1;
		} else {
			i++;
		}
	}

	return ret;
}

void bput(struct buf *b)
{
	acquire(&bcachelock);
	b->refcnt--;
	if (b->refcnt == 0) {
		// no one is waiting for it.
		dlist_add_after(&lru_head, &b->list);

		// available buf for someone who's in sleep-wait
		if (nwait > 0)
			wakeup(&nwait);
	}
	release(&bcachelock);
}

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
