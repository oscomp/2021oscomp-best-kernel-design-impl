#ifndef __DEBUG_disk 
#undef DEBUG 
#endif 

#define __module_name__     "disk"

#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/debug.h"

#include "include/buf.h"

#ifndef QEMU
#include "include/sdcard.h"
#include "include/dmac.h"
#else
#include "include/virtio.h"
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
	virtio_disk_rw(b, 0);
	#else 
	sdcard_read_sector(b->data, b->sectorno);
	#endif

	__debug_info("disk_read", "leave\n");
}

void disk_write(struct buf *b)
{
	__debug_info("disk_write", "enter\n");

	#ifdef QEMU
	virtio_disk_rw(b, 1);
	#else 
	sdcard_write_sector(b->data, b->sectorno);
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
