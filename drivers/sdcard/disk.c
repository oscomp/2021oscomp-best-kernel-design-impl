#include "type.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "qemu.h"
#include "buf.h"

void disk_init(void)
{
    #ifndef K210
    // virtio_disk_init();
    #endif
}

void disk_read(struct buf *b)
{
    #ifndef K210
	// virtio_disk_rw(b, 0);
    #endif
}

void disk_write(struct buf *b)
{
    #ifndef K210
	// virtio_disk_rw(b, 1);
    #endif
}

void disk_intr(void)
{
    #ifndef K210
    // virtio_disk_intr();
    #endif
}

void do_testdisk()
{
    struct buf test;
    disk_read(&test);
}