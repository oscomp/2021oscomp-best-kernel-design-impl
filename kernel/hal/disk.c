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

void disk_read(struct buf *b)
{
	__debug_info("disk_read", "enter\n");

	#ifdef QEMU
	virtio_disk_rw(b, b->sectorno, 0);
	#else 
	sdcard_read_sector(b->data, b->sectorno);
	#endif

	__debug_info("disk_read", "leave\n");
}

void disk_write(struct buf *b, uint sector)
{
	__debug_info("disk_write", "enter\n");

	#ifdef QEMU
	virtio_disk_rw(b, sector, 1);
	#else 
	sdcard_write_sector(b->data, sector);
	#endif

	__debug_info("disk_write", "leave\n");
}

void disk_intr(void)
{
	__debug_info("disk_intr", "enter\n");

	#ifdef QEMU
	virtio_disk_intr();
	#else 
	dmac_intr(DMAC_CHANNEL0);
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
void disk_test(void)
{
	void *page = allocpage();
	if (page == NULL)
		return;

	memset(page, 0, PGSIZE);
	struct buf *bufs = page;
	struct buf *pbufs[2];

	bufs[0].sectorno = 10952;
	bufs[1].sectorno = 10953;
	// bufs[2].sectorno = 10866;

	pbufs[0] = &bufs[0];
	pbufs[1] = &bufs[1];
	// pbufs[2] = &bufs[2];

	disk_read_multi_blk(pbufs, 2);

	printf(__INFO("sec 1")"\n%s\n", bufs[0].data);
	printf(__INFO("sec 2")"\n%s\n", bufs[1].data);
	// printf(__INFO("sec 3")"\n%s\n", bufs[2].data);

	// for (int i = 0; i < 10; i++) {
	// 	// printf("%d\t", bufs[1].data[i]);
	// 	bufs[0].data[i] = '1';
	// 	bufs[1].data[i] = '2';
	// }

	// printf("writing sec %d\n", bufs[0].sectorno);
	// disk_write(&bufs[0], bufs[0].sectorno);
	// printf("writing sec %d\n", bufs[1].sectorno);
	// disk_write(&bufs[1], bufs[1].sectorno);

	freepage(page);
}
*/