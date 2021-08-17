// Copyright (c) 2006-2019 Frans Kaashoek, Robert Morris, Russ Cox,
//                         Massachusetts Institute of Technology

#define __module_name__ 	"main"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sbi.h"
#include "console.h"
#include "printf.h"
#include "timer.h"
#include "trap.h"
#include "sched/proc.h"
#include "hal/plic.h"
#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kmalloc.h"
#include "hal/disk.h"
#include "fs/buf.h"
#include "utils/debug.h"
#ifndef QEMU
#include "hal/fpioa.h"
#include "hal/dmac.h"
#endif

static inline void inithartid(unsigned long hartid) {
	asm volatile("mv tp, %0" : : "r" (hartid & 0x1));
}

volatile static int started = 0;

void
main(unsigned long hartid, unsigned long dtb_pa)
{
	inithartid(hartid);
	
	if (hartid == 0) {
		started = 0;
		cpuinit();
		floatinithart();
		consoleinit();
		printfinit();   // init a lock for printf 
		print_logo();
		kpminit();       // physical page allocator
		kvminit();       // create kernel page table
		kvminithart();   // turn on paging
		kmallocinit();   // small physical memory allocator
		// timerinit();     // init a lock for timer
		trapinithart();  // install kernel trap vector, including interrupt handler
		procinit();
		plicinit();
		plicinithart();
		#ifndef QEMU
		fpioa_pin_init();
		dmac_init();
		#endif 
		disk_init();
		binit();         // buffer cache
		// fileinit();      // file table
		userinit();      // first user process
		printf("hart 0 init done\n");

		// we need IPI to wake up other hart(s)
		for (int i = 1; i < NCPU; i ++) {
			unsigned long mask = 1 << i;
			// struct sbiret res = sbi_send_ipi(mask, 0);
			sbi_send_ipi(mask, 0);
			__debug_assert("main", SBI_SUCCESS == res.error, "sbi_send_ipi failed");
		}
		__sync_synchronize();
		started = 1;
	}
	else
	{
		// hart 1
		while (started == 0)
			;
		__sync_synchronize();
		floatinithart();
		kvminithart();
		trapinithart();
		plicinithart();  // ask PLIC for device interrupts
		printf("hart 1 init done\n");
	}
	__debug_info("main", "hart %d enter main()...\n", hartid);

	// shrink the boot stack
	extern char boot_stack[];
	uint64 kstack = (uint64)boot_stack + hartid * 4 * PGSIZE;
	freepage_n(kstack, 3);

	scheduler();
}
