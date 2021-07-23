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

#define BCACHE_TABLE_SIZE 233
#define __hash_idx(idx) ((idx) % BCACHE_TABLE_SIZE)

// struct {
static struct spinlock bcachelock;
static struct buf head;
static struct buf *bcache[BCACHE_TABLE_SIZE];
static struct buf bufs[BNUM];
// } bcache;

void
binit(void)
{
	initlock(&bcachelock, "bcache");
	for (int i = 0; i < BCACHE_TABLE_SIZE; i++) {
		bcache[i] = NULL;
	}
	head.next = &head;
	head.prev = &head;
	for (struct buf *b = bufs; b < bufs + BNUM; b++) {
		b->refcnt = 0;
		b->sectorno = ~0;
		b->dev = ~0;
		b->hash.pprev = NULL;
		b->hash.next = NULL;

		b->next = head.next;
		b->prev = &head;
		head.next->prev = b;
		head.next = b;
		initsleeplock(&b->lock, "buffer");
	}
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint sectorno)
{
	struct buf *b;
	int idx = __hash_idx(sectorno);

	__debug_info("bget", "search secno=%d, idx=%d\n", sectorno, idx);
	acquire(&bcachelock);
	// Is the block already cached?
	for (b = bcache[idx]; b != NULL; b = list_next(struct buf, b)) {
		if (b->dev == dev && b->sectorno == sectorno) {
			list_remove(b);
			if (bcache[idx]) {
				bcache[idx]->hash.pprev = &b->hash.next;
			}
			b->hash.next = (list_node_t *)bcache[idx];
			b->hash.pprev = (list_node_t **)&bcache[idx];
			bcache[idx] = b;
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
	for (b = head.prev; b != &head; b = b->prev){
		if (b->refcnt == 0) {
			b->dev = dev;
			b->sectorno = sectorno;
			b->valid = 0;
			b->refcnt = 1;
			if (b->hash.pprev) {
				list_remove(b);
			}
			if (bcache[idx]) {
				bcache[idx]->hash.pprev = &b->hash.next;
			}
			b->hash.next = (list_node_t *)bcache[idx];
			b->hash.pprev = (list_node_t **)&bcache[idx];
			bcache[idx] = b;
			release(&bcachelock);
			acquiresleep(&b->lock);
			return b;
		}
	}
	panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf* 
bread(uint dev, uint sectorno) {
	struct buf *b;

	b = bget(dev, sectorno);
	__debug_info("bread", "get buffer\n");
	if (!b->valid) {
		__debug_info("bread", "start reading sno %d\n", sectorno);
		disk_read(b);
		__debug_info("bread", "done sno %d\n", sectorno);
		b->valid = 1;
	}
	return b;
}

// Write b's contents to disk.  Must be locked.
void 
bwrite(struct buf *b) {
	if(!holdingsleep(&b->lock))
		panic("bwrite");
	disk_write(b);
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
		b->next->prev = b->prev;
		b->prev->next = b->next;
		b->next = head.next;
		b->prev = &head;
		head.next->prev = b;
		head.next = b;
	}
	
	release(&bcachelock);
}

void bprint()
{
	acquire(&bcachelock);
	printf("\nbuf cache:\n");
	for (int i = 0; i < BCACHE_TABLE_SIZE; i++) {
		struct buf *b = bcache[i];
		if (b == NULL) { continue; }
		printf(__INFO("%d")" ", i);
		for (; b != NULL; b = list_next(struct buf, b)) {
			printf("%d ", b->sectorno);
		}
		printf("\n");
	}
	release(&bcachelock);
}
