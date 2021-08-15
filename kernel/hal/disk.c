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
	return virtio_disk_read(b);
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

int disk_multiple_read(struct buf *bufs[], int nbuf)
{
	#ifdef QEMU
	return virtio_disk_multiple_read(bufs, nbuf);
	#else
	return sdcard_read_sectors(bufs, nbuf);
	#endif
}

/*
#include "time.h"

#define TEST_NBUF	10000
#define CONSECUTIVE	1
#define NBUFPRT		8

void disk_test(void)
{
	struct buf *bufs[NBUFPRT];
	struct buf *b;
	int setcorno = 60000000, i;
	struct timeval t;
	
	uint64 t1 = readtime();

	for (i = 0; i < TEST_NBUF; i++) {
		b = bget(ROOTDEV, setcorno + i * CONSECUTIVE);
		b->data[0] = b->data[BSIZE - 1] = i + t1;
		bwrite(b);
		if ((readtime() & 0x1ff) < 0x3) {
			// printf("break!\n");
			brelse(bread(ROOTDEV, setcorno + i - 500));
		}
	}

	uint64 t2 = readtime();
	convert_to_timeval(t2 - t1, &t);
	printf("submit done! cost %d s %d us\n", t.sec, t.usec);

	// race
	extern int sdcard_wqueue_empty(void);
	while (!sdcard_wqueue_empty()) {
		asm volatile("wfi");
		// if ((readtime() & 0xff) < 0x3) {
		// 	// printf("break!\n");
		// 	brelse(bread(ROOTDEV, setcorno + i - 500));
		// }
	}

	uint64 t3 = readtime();

	convert_to_timeval(t3 - t1, &t);
	printf("%d KB cost %d s %d us\n", BSIZE * TEST_NBUF / 1024, t.sec, t.usec);

	t2 = readtime();
	for (i = 0; i < TEST_NBUF; i += NBUFPRT) {
		for (int j = 0; j < NBUFPRT; j++) {
			bufs[j] = bget(ROOTDEV, setcorno + (j + i) * CONSECUTIVE);
			bufs[j]->data[0] = bufs[j]->data[BSIZE - 1] = 0;
		}
		int ret = breads(bufs, NBUFPRT);
		if (ret < 0)
			printf(__WARN("breads")" fail! errno %d\n", ret);
		for (int j = 0; j < NBUFPRT; j++) {
			uchar ans = i + j + t1;
			b = bufs[j];
			if (b->data[0] != ans || b->data[BSIZE - 1] != ans)
				printf(__WARN("read")" sec %d get wrong data\n", b->sectorno);
			brelse(b);
		}
	}

	t3 = readtime();
	convert_to_timeval(t3 - t2, &t);
	printf(__INFO("read")" read done! cost %d s %d us\n", t.sec, t.usec);

	for(;;) {
		asm volatile("wfi");
	}
}
*/