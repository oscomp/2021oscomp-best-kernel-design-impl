//
// driver for qemu's virtio disk device.
// uses qemu's mmio interface to virtio.
// qemu presents a "legacy" virtio interface.
//
// qemu ... -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
//

#ifndef __DEBUG_virtio_disk
#undef DEBUG
#endif 

#define __module_name__ 	"virtio_disk"

#include "types.h"
#include "hal/riscv.h"
#include "param.h"
#include "memlayout.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"
#include "fs/buf.h"
#include "hal/virtio.h"
#include "sched/proc.h"
#include "mm/vm.h"
#include "utils/string.h"
#include "printf.h"
#include "utils/debug.h"
#include "utils/dlist.h"
#include "sync/waitqueue.h"

// the address of virtio mmio register r.
#define R(r) ((volatile uint32 *)(VIRTIO0_V + (r)))

static struct disk {
	// the virtio driver and device mostly communicate through a set of
	// structures in RAM. pages[] allocates that memory. pages[] is a
	// global (instead of calls to kalloc()) because it must consist of
	// two contiguous pages of page-aligned physical memory.
	char pages[2 * PGSIZE];

	// pages[] is divided into three regions (descriptors, avail, and
	// used), as explained in Section 2.6 of the virtio specification
	// for the legacy interface.
	// https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.pdf
	
	// the first region of pages[] is a set (not a ring) of DMA
	// descriptors, with which the driver tells the device where to read
	// and write individual disk operations. there are NUM descriptors.
	// most commands consist of a "chain" (a linked list) of a couple of
	// these descriptors.
	// points into pages[].
	struct virtq_desc *desc;

	// next is a ring in which the driver writes descriptor numbers
	// that the driver would like the device to process.  it only
	// includes the head descriptor of each chain. the ring has
	// NUM elements.
	// points into pages[].
	struct virtq_avail *avail;

	// finally a ring in which the device writes descriptor numbers that
	// the device has finished processing (just the head of each chain).
	// there are NUM used ring entries.
	// points into pages[].
	struct virtq_used *used;

	// our own book-keeping.
	char free[NUM];  // is a descriptor free?
	uint16 used_idx; // we've looked this far in used[2..NUM].
	uint16 nwait;

	// track info about in-flight operations,
	// for use when completion interrupt arrives.
	// indexed by first descriptor index of chain.
	struct {
		struct buf *b;
		char status;
		int idx1;	// first index of a chain
	} info[NUM];

	// queue of buf to write (read doesn't need)
	// buf::dirty is protected by this queue's lock
	struct wait_queue queue;
	int queue_running;

	// disk command headers.
	// one-for-one with descriptors, for convenience.
	struct virtio_blk_req ops[NUM];
	
	struct spinlock vdisk_lock;
	
} __attribute__ ((aligned (PGSIZE))) disk;

void
virtio_disk_init(void)
{
	uint32 status = 0;

	initlock(&disk.vdisk_lock, "virtio_disk");
	wait_queue_init(&disk.queue, "vdisk_queue");
	disk.queue_running = 0;
	disk.nwait = 0;

	if (*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
		*R(VIRTIO_MMIO_VERSION) != 1 ||
		*R(VIRTIO_MMIO_DEVICE_ID) != 2 ||
		*R(VIRTIO_MMIO_VENDOR_ID) != 0x554d4551)
	{
		panic("could not find virtio disk");
	}
	
	status |= VIRTIO_CONFIG_S_ACKNOWLEDGE;
	*R(VIRTIO_MMIO_STATUS) = status;

	status |= VIRTIO_CONFIG_S_DRIVER;
	*R(VIRTIO_MMIO_STATUS) = status;

	// negotiate features
	uint64 features = *R(VIRTIO_MMIO_DEVICE_FEATURES);
	features &= ~(1 << VIRTIO_BLK_F_RO);
	features &= ~(1 << VIRTIO_BLK_F_SCSI);
	features &= ~(1 << VIRTIO_BLK_F_CONFIG_WCE);
	features &= ~(1 << VIRTIO_BLK_F_MQ);
	features &= ~(1 << VIRTIO_F_ANY_LAYOUT);
	features &= ~(1 << VIRTIO_RING_F_EVENT_IDX);
	features &= ~(1 << VIRTIO_RING_F_INDIRECT_DESC);
	*R(VIRTIO_MMIO_DRIVER_FEATURES) = features;

	// tell device that feature negotiation is complete.
	status |= VIRTIO_CONFIG_S_FEATURES_OK;
	*R(VIRTIO_MMIO_STATUS) = status;

	// tell device we're completely ready.
	status |= VIRTIO_CONFIG_S_DRIVER_OK;
	*R(VIRTIO_MMIO_STATUS) = status;

	*R(VIRTIO_MMIO_GUEST_PAGE_SIZE) = PGSIZE;

	// initialize queue 0.
	*R(VIRTIO_MMIO_QUEUE_SEL) = 0;
	uint32 max = *R(VIRTIO_MMIO_QUEUE_NUM_MAX);
	if (max == 0)
		panic("virtio disk has no queue 0");
	if (max < NUM)
		panic("virtio disk max queue too short");
	*R(VIRTIO_MMIO_QUEUE_NUM) = NUM;
	memset(disk.pages, 0, sizeof(disk.pages));
	*R(VIRTIO_MMIO_QUEUE_PFN) = ((uint64)disk.pages) >> PGSHIFT;

	// desc = pages -- num * virtq_desc
	// avail = pages + 0x40 -- 2 * uint16, then num * uint16
	// used = pages + 4096 -- 2 * uint16, then num * vRingUsedElem

	disk.desc = (struct virtq_desc *) disk.pages;
	disk.avail = (struct virtq_avail *)(disk.pages + NUM * sizeof(struct virtq_desc));
	disk.used = (struct virtq_used *) (disk.pages + PGSIZE);

	// all NUM descriptors start out unused.
	for (int i = 0; i < NUM; i++)
		disk.free[i] = 1;

	// plic.c and trap.c arrange for interrupts from VIRTIO0_IRQ.
}

// find a free descriptor, mark it non-free, return its index.
static int
alloc_desc()
{
	for (int i = 0; i < NUM; i++) {
		if (disk.free[i]) {
			disk.free[i] = 0;
			return i;
		}
	}
	return -1;
}

// mark a descriptor as free.
static void
free_desc(int i)
{
	if (i >= NUM)
		panic("free_desc 1");
	if (disk.free[i])
		panic("free_desc 2");
	disk.desc[i].addr = 0;
	disk.desc[i].len = 0;
	disk.desc[i].flags = 0;
	disk.desc[i].next = 0;
	disk.free[i] = 1;
	if (disk.nwait > 0)
		wakeup(&disk.free[0]);
}

// free a chain of descriptors.
static void
free_chain(int i)
{
	while (1) {
		int flag = disk.desc[i].flags;
		int nxt = disk.desc[i].next;
		free_desc(i);
		if (flag & VRING_DESC_F_NEXT)
			i = nxt;
		else
			break;
	}
}

// allocate three descriptors (they need not be contiguous).
// disk transfers always use three descriptors.
static int
alloc3_desc(int * restrict idx)
{
	for (int i = 0; i < 3; i++) {
		idx[i] = alloc_desc();
		if (idx[i] < 0) {
			for (int j = 0; j < i; j++)
				free_desc(idx[j]);
			return -1;
		}
	}
	return 0;
}

int
virtio_disk_rw(struct buf *b, int write)
{
	acquire(&disk.vdisk_lock);
	// the spec's Section 5.2 says that legacy block operations use
	// three descriptors: one for type/reserved/sector, one for the
	// data, one for a 1-byte status result.

	// allocate the three descriptors.
	int idx[3];
	while (1) {
		if (alloc3_desc(idx) == 0)
			break;
		if (write) {	// write don't wait
			release(&disk.vdisk_lock);
			return -1;
		}
		disk.nwait++;
		sleep(&disk.free[0], &disk.vdisk_lock);
		disk.nwait--;
	}

	// format the three descriptors.
	// qemu's virtio-blk.c reads them.
	
	struct virtio_blk_req *buf0 = &disk.ops[idx[0]];
	struct virtq_desc *desc;

	buf0->type = write ? VIRTIO_BLK_T_OUT :	// write the disk
						VIRTIO_BLK_T_IN;	// read the disk
	buf0->reserved = 0;
	buf0->sector = b->sectorno;

	// first
	desc = &disk.desc[idx[0]];
	desc->addr = (uint64) buf0;
	desc->len = sizeof(struct virtio_blk_req);
	desc->flags = VRING_DESC_F_NEXT;
	desc->next = idx[1];

	// second
	desc = &disk.desc[idx[1]];
	desc->addr = (uint64) b->data;
	desc->len = BSIZE;
	desc->flags = write ? 0 :				// device reads b->data
						VRING_DESC_F_WRITE;	// device writes b->data
	desc->flags |= VRING_DESC_F_NEXT;
	desc->next = idx[2];

	// last
	desc = &disk.desc[idx[2]];
	desc->addr = (uint64) &disk.info[idx[0]].status;
	desc->len = 1;
	desc->flags = VRING_DESC_F_WRITE;		// device writes the status
	desc->next = 0;

	/**
	 * Record struct buf for virtio_disk_intr().
	 * If we are committing a wirte, we are not going to wait here.
	 * After the write is done, the interrupt handler will use idx1
	 * to free the desc chain that we allocated. In other way, this
	 * is also a hint that this is a write buf, the handler needs to
	 * do something about it.
	 */
	disk.info[idx[0]].idx1 = write ? idx[0] : -1;
	disk.info[idx[0]].status = 0xff;		// device writes 0 on success
	disk.info[idx[0]].b = b;

	// tell the device the first index in our chain of descriptors.
	disk.avail->ring[disk.avail->idx % NUM] = idx[0];

	__sync_synchronize();

	// tell the device another avail ring entry is available.
	disk.avail->idx += 1; // not % NUM ...

	__sync_synchronize();

	*R(VIRTIO_MMIO_QUEUE_NOTIFY) = 0; // value is queue number

	int res = 0;
	if (!write) {
		b->disk = 1;
		// Wait for virtio_disk_intr() to say request has finished.
		while (b->disk == 1) {
			// printf(__INFO("virtio_disk_rw")" sleep on %d\n", b->sectorno);
			sleep(b, &disk.vdisk_lock);
		}
		res = -(disk.info[idx[0]].status != 0);
		free_chain(idx[0]);
	}

	release(&disk.vdisk_lock);
	return res;
}

void
virtio_disk_intr()
{
	struct buf *b;
	struct buf *bufs[NUM];	// bufs that finished writing
	int cnt = 0, i;

	acquire(&disk.vdisk_lock);

	// the device won't raise another interrupt until we tell it
	// we've seen this interrupt, which the following line does.
	// this may race with the device writing new entries to
	// the "used" ring, in which case we may process the new
	// completion entries in this interrupt, and have nothing to do
	// in the next interrupt, which is harmless.
	*R(VIRTIO_MMIO_INTERRUPT_ACK) = *R(VIRTIO_MMIO_INTERRUPT_STATUS) & 0x3;

	__sync_synchronize();

	// the device increments disk.used->idx when it
	// adds an entry to the used ring.
	while (disk.used_idx != disk.used->idx) {
		__sync_synchronize();
		int id = disk.used->ring[disk.used_idx % NUM].id;
		disk.used_idx += 1;

		if (disk.info[id].status != 0)
			panic("virtio_disk_intr status");

		b = disk.info[id].b;
		// disk.info[id].b = 0;
		if (disk.info[id].idx1 < 0) {	// read interrupt
			b->disk = 0;   				// disk is done with buf
			wakeup(b);
		} else {						// write interrupt
			free_chain(disk.info[id].idx1);
			bufs[cnt++] = b;			// collect for convenience
		}
	}
	release(&disk.vdisk_lock);

	if (cnt > 0) {
		acquire(&disk.queue.lock);
		for (i = 0; i < cnt; i++) {			// clean write bufs
			bufs[i]->disk = 0;				// disk is done with buf
			disk.queue_running--;
			if (bufs[i]->dirty)				// some one has touched it again
				bufs[i] = NULL;				// we have to keep it and write again
			else
				dlist_del(&bufs[i]->list);	// done, let go
		}
		release(&disk.queue.lock);

		for (i = 0; i < cnt; i++) {
			if (bufs[i])
				bput(bufs[i]);
		}
	}

	acquire(&disk.queue.lock);
	struct d_list *l = disk.queue.head.next;
	if (l == &disk.queue.head) {			// nothing to write
		release(&disk.queue.lock);
		return;
	}

	for (; l != &disk.queue.head; l = l->next) {
		b = container_of(l, struct buf, list);
		if (!b->disk) {
			b->disk = 1;
			b->dirty = 0;
			disk.queue_running++;
			release(&disk.queue.lock);
			if (virtio_disk_rw(b, 1) < 0) {
				acquire(&disk.queue.lock);
				b->disk = 0;
				b->dirty = 1;
				disk.queue_running--;
				release(&disk.queue.lock);
			}
			return;
		}
	}
	release(&disk.queue.lock);
}

void virtio_disk_write_start(void)
{
	acquire(&disk.queue.lock);
	if (disk.queue_running > 0) {
		release(&disk.queue.lock);
		return;
	} else if (disk.queue_running < 0)
		panic("virtio_disk_write_start: bad counter");

	struct d_list *l = disk.queue.head.next;
	if (l == &disk.queue.head) {
		release(&disk.queue.lock);
		return;
	}
	
	struct buf *b;
	b = container_of(l, struct buf, list);
	if (b->disk)
		panic("virtio_disk_write_start: on-flight buf");

	b->disk = 1;
	b->dirty = 0;
	disk.queue_running = 1;
	release(&disk.queue.lock);

	if (virtio_disk_rw(b, 1) < 0) {
		/**
		 * This happens when lacking of descs,
		 * but don't worry, since lacking of
		 * descs means some one is reading disk,
		 * so when its interrupt comes, it will
		 * start us.
		 */
		acquire(&disk.queue.lock);
		b->disk = 0;
		b->dirty = 1;
		disk.queue_running--;
		release(&disk.queue.lock);
	}

}

int virtio_disk_submit(struct buf *b)
{
	int res = 0;
	acquire(&disk.queue.lock);

	/**
	 * Test whether this is a new coming member.
	 * When carrying the buf to disk, b->disk
	 * will be set and b->dirty will be cleared.
	 * One of them is non-zero means that this
	 * buf is already in queue.
	 */
	if (b->dirty == 0 && b->disk == 0) {
		dlist_add_before(&disk.queue.head, &b->list);
		res = 0x1;
	}
	b->dirty = 1;
	if (disk.queue_running > 0)	// inform that the queue is on
		res |= 0x2;
	release(&disk.queue.lock);
	return res;
}

/*
static int alloc_descs(int *idx, int n)
{
	for (int i = 0; i < n; i++) {
		idx[i] = alloc_desc();
		if (idx[i] < 0) {
			for (int j = 0; j < i; j++)
				free_desc(idx[j]);
			return -1;
		}
	}
	return 0;
}

int virtio_disk_multi_rw(struct buf *bufs[], int nbuf, int write)
{
	int ndesc = nbuf + 2;
	if (nbuf <= 0 || ndesc > NUM)
		panic("virtio_disk_multi_rw: bad nbuf");

	acquire(&disk.vdisk_lock);

	int idx[NUM];
	while (1) {
		if (alloc_descs(idx, ndesc) == 0)
			break;
		sleep(&disk.free[0], &disk.vdisk_lock);
	}
	
	struct virtio_blk_outhdr {
		uint32 type;
		uint32 reserved;
		uint64 sector;
	};
	struct virtio_blk_outhdr buf0;
	struct virtio_blk_outhdr *pbuf0;
	uint64 sector = bufs[0]->sectorno;
	
	pbuf0 = (struct virtio_blk_outhdr*)kwalkaddr(myproc()->pagetable, (uint64)&buf0);


	buf0.type = write ? VIRTIO_BLK_T_OUT : VIRTIO_BLK_T_IN;
	buf0.reserved = 0;
	buf0.sector = sector;

	disk.desc[idx[0]].addr = (uint64)pbuf0;
	disk.desc[idx[0]].len = sizeof(struct virtio_blk_outhdr);
	disk.desc[idx[0]].flags = VRING_DESC_F_NEXT;
	disk.desc[idx[0]].next = idx[1];

	for (int i = 1; i <= nbuf; i++) {
		if (sector++ != bufs[i - 1]->sectorno)
			panic("inconsecutive sector number");

		disk.desc[idx[i]].addr = (uint64)bufs[i - 1]->data;
		disk.desc[idx[i]].len = BSIZE;
		disk.desc[idx[i]].flags = write ? 0 : VRING_DESC_F_WRITE;
		disk.desc[idx[i]].flags |= VRING_DESC_F_NEXT;
		disk.desc[idx[i]].next = idx[i + 1];
	}

	disk.info[idx[0]].status = -1;
	disk.desc[idx[ndesc - 1]].addr = (uint64)&disk.info[idx[0]].status;
	disk.desc[idx[ndesc - 1]].len = 1;
	disk.desc[idx[ndesc - 1]].flags = VRING_DESC_F_WRITE;
	disk.desc[idx[ndesc - 1]].next = 0;

	bufs[0]->disk = 1;
	disk.info[idx[0]].b = bufs[0];

	disk.avail[2 + (disk.avail[1] % NUM)] = idx[0];
	__sync_synchronize();
	disk.avail[1] = disk.avail[1] + 1;

	*R(VIRTIO_MMIO_QUEUE_NOTIFY) = 0; // queue number

	while (bufs[0]->disk == 1) {
		sleep(bufs[0], &disk.vdisk_lock);
	}

	int ret = disk.info[idx[0]].status == 0 ? 0 : -1;

	disk.info[idx[0]].b = 0;
	free_chain(idx[0]);

	release(&disk.vdisk_lock);

	return ret;
}
*/