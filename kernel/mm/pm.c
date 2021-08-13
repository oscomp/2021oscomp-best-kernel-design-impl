// Physical memory or pages management

// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#ifndef __DEBUG_pm 
#undef DEBUG 
#endif 

#define __module_name__ 		"pm"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sync/spinlock.h"
#include "mm/pm.h"
#include "utils/string.h"
#include "printf.h"
#include "utils/debug.h"

extern char kernel_end[];	// first address after kernel 

struct run {
	struct run *next;
	uint64 npage;
};

static struct {
	struct spinlock lock;
	struct run *freelist;
	uint64 npage;
} kmem;

#define __enter_kmem_cs \
	acquire(&kmem.lock);
#define __leave_kmem_cs \
	release(&kmem.lock);

static void freerange(uint64 pa_start, uint64 pa_end) {
	uint64 start = PGROUNDUP(pa_start);
	uint64 npage = (pa_end - start) / PGSIZE;

	freepage_n(start, npage);
}

void kpminit(void) {
	initlock(&kmem.lock, "kmem");
	kmem.freelist = NULL;
	kmem.npage = 0;
	freerange((uint64)kernel_end, (uint64)PHYSTOP);
	__debug_info("kpminit", "kernel_end: %p, phystop: %p, npage %d\n", 
			kernel_end, (void*)PHYSTOP, kmem.npage);
}

void *allocpage_n(uint64 n) {
	struct run *pa;
	struct run **pprev;

	__debug_assert("allocpage_n", n > 0, "n <= 0\n");

	__enter_kmem_cs 
	pa = kmem.freelist;
	pprev = &(kmem.freelist);

	while (NULL != pa) {
		if (pa->npage >= n) {
			struct run *ret = (struct run*)(
				(uint64)pa + PGSIZE * (pa->npage - n)
			);
			if (pa == ret) {
				*pprev = pa->next;
			}
			else {
				pa->npage -= n;
				pa = ret;
			}

			kmem.npage -= n;

			break;
		}

		pprev = &(pa->next);
		pa = pa->next;
	}
	__leave_kmem_cs 

	#ifdef DEBUG
	// fill the memory with junk, so bugs may lead to a faster panic. 
	if (NULL != pa) {
		memset(pa, 5, n * PGSIZE);
	}
	#endif 

	__debug_info("allocpage_n", "%p, %d\n", pa, n);
	return (void*)pa;
}

void freepage_n(uint64 start, uint64 n) {
	extern char boot_stack[];
	extern char boot_stack_top[];

	__debug_info("freepage_n", "start = %p, n = %d\n", start, n);

	if (((uint64)start % PGSIZE) != 0 || n <= 0 || 
		(	// memory range that could be freed 
			!(start >= (uint64)kernel_end && (start + n * PGSIZE) <= PHYSTOP) && 
			!(start >= (uint64)boot_stack && (start + n * PGSIZE) <= (uint64)boot_stack_top)
		)
	) {
		panic("freepage");
	}

	__enter_kmem_cs 
	if (NULL == kmem.freelist) {		// if the allocator is empty
		kmem.freelist = (struct run*)start;
		kmem.freelist->npage = n;
		kmem.freelist->next = NULL;
		kmem.npage = n;

		__leave_kmem_cs 

		return ;
	}

	// find the first block of memory after the memory to free 
	struct run *prev = NULL;
	struct run *next = kmem.freelist;
	while (NULL != next) {
		if ((uint64)next > start) {
			break;
		}
		else {
			prev = next;
			next = next->next;
		}
	}

	struct run *pa = (struct run*)start;
	pa->next = next;
	pa->npage = n;

	// merge with previous 
	if (NULL != prev) {
		if (((uint64)prev + prev->npage * PGSIZE) == (uint64)pa) {
			prev->npage += n;
			pa = prev;
		}
		else {
			prev->next = pa;
		}
	}
	else {
		kmem.freelist = pa;
		pa->npage = n;
	}

	// merge with next 
	if (NULL != next) {
		if (((uint64)pa + pa->npage * PGSIZE) == (uint64)next) {
			// mergable 
			pa->npage += next->npage;
			pa->next = next->next;
		}
	}

	kmem.npage += n;

	__debug_info("freepage_n", "avail_addr: %p, avail: %d\n", kmem.freelist, kmem.npage);

	__leave_kmem_cs 
}

uint64 idlepages(void) {
	return kmem.npage;
}

void pm_dump(void) {
	__enter_kmem_cs 

	struct run *pa = kmem.freelist;
	printf("kmem avail: %d\n", kmem.npage);
	while (NULL != pa) {
		printf("\t%p %d\n", pa, pa->npage);
		pa = pa->next;
	}

	__leave_kmem_cs 
}