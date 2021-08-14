#ifndef __DEBUG_disk 
#undef DEBUG 
#endif 

#define __module_name__     "disk"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "utils/debug.h"

#include "fs/buf.h"

#ifndef QEMU
#include "hal/sdcard.h"
#include "hal/dmac.h"
#else
#include "hal/virtio.h"
#endif 

void disk_init(void)
{
	__debug_info("disk_init", "enter\n");

	#ifdef QEMU
	virtio_disk_init();
	#else 
	sdcard_init();
	#endif

	__debug_info("disk_init", "leave\n");
}

int disk_read(struct buf *b)
{
	__debug_info("disk_read", "enter\n");

	#ifdef QEMU
	return virtio_disk_rw(b, 0);
	#else 
	return sdcard_read(b);
	#endif

	__debug_info("disk_read", "leave\n");
}

void disk_write(struct buf *b)
{
	__debug_info("disk_write", "enter\n");

	#ifdef QEMU
	// virtio_disk_rw(b, 1);
	#else 
	// sdcard_write_sector(b->data, sector);
	#endif

	__debug_info("disk_write", "leave\n");
}

int disk_submit(struct buf *b)
{
	#ifdef QEMU
	return virtio_disk_submit(b);
	#else 
	return sdcard_submit(b);
	return 0;
	#endif
}

void disk_write_start(void)
{
	#ifdef QEMU
	virtio_disk_write_start();
	#else 
	sdcard_write_start();
	#endif
}

void disk_intr(void)
{
	__debug_info("disk_intr", "enter\n");

	#ifdef QEMU
	virtio_disk_intr();
	#else
	sdcard_intr();
	#endif

	__debug_info("disk_intr", "leave\n");
}

// int disk_read_multi_blk(struct buf *bufs[], int nbuf)
// {
// 	#ifdef QEMU
// 	return virtio_disk_multi_rw(bufs, nbuf, 0);
// 	#else
// 	return sdcard_read_sectors(bufs, nbuf);
// 	#endif
// }

// int disk_write_multi_blk(struct buf *bufs[], int nbuf)
// {
// 	#ifdef QEMU
// 	return virtio_disk_multi_rw(bufs, nbuf, 1);
// 	#else
// 	return sdcard_write_sectors(bufs, nbuf);
// 	#endif
// }
/*
#include "time.h"
#include "mm/pm.h"

#define TEST_NBUF	500
#define CONSECUTIVE	1

void disk_test(void)
{
	// void *page = allocpage();
	// struct buf **bufs = page;
	struct buf *b;
	int setcorno = 10000, i;
	
	uint64 t1 = readtime();

	for (i = 0; i < BNUM; i++) {
		b = bget(ROOTDEV, setcorno + i * CONSECUTIVE);
		b->data[0] = b->data[BSIZE - 1] = i + t1;
		bwrite(b);
	}

	// race
	for (i = 0; i < BNUM; i++) {
		b = bget(ROOTDEV, setcorno + i * CONSECUTIVE);
		while (b->dirty || b->disk) {	// not done
			asm volatile("wfi");
		}
		b->valid = 0;
		brelse(b);
	}

	// for (i = 0; i < TEST_NBUF; i++) {
	// 	bufs[i] = bget(ROOTDEV, setcorno + i);
	// 	bufs[i]->data[0] = i + random;
	// 	bufs[i]->data[BSIZE - 1] = i + random;
	// }
	
	// for (i = 0; i < TEST_NBUF; i++)
	// 	bwrite(bufs[i]);
	
	// for (i = 0; i < TEST_NBUF; i++) {
	// 	while (bufs[i]->dirty || bufs[i]->disk) {	// not done
	// 		asm volatile("wfi");
	// 	}
	// 	bufs[i]->valid = 0;
	// }

	uint64 t2 = readtime();

	struct timeval t;
	convert_to_timeval(t2 - t1, &t);
	printf("%d KB cost %d s %d us\n", BSIZE * BNUM / 1024, t.sec, t.usec);

	for (i = 0; i < BNUM; i++) {
		b = bread(ROOTDEV, setcorno + i * CONSECUTIVE);
		uchar ans = i + t1;
		if (b->data[0] != ans || b->data[BSIZE - 1] != ans)
			printf(__WARN("read")" sec %d get wrong data\n", b->sectorno);
		brelse(b);
	}
	printf(__INFO("read")" write correct\n");

	// freepage(page);

	for(;;) {
		asm volatile("wfi");
	}
}
*/