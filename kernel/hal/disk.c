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
