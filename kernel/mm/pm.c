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

extern char kernel_end[];		// first address after kernel 
extern char boot_stack_top[];	// first address after kernel bootstack 

struct run {
	struct run *next;
	uint64 npage;
};

struct pm_allocator {
	struct spinlock lock;
	struct run *freelist;
	uint64 npage;
};

struct pm_allocator multiple;
struct pm_allocator single;

#define SINGLE_PAGE_NUM 		400
uint64 START_SINGLE = PHYSTOP - SINGLE_PAGE_NUM * PGSIZE;

#define __enter_mul_cs \
	acquire(&multiple.lock);
#define __leave_mul_cs \
	release(&multiple.lock);
#define __enter_sin_cs \
	acquire(&single.lock);
#define __leave_sin_cs \
	release(&single.lock);


// Allocate n pages 
static void *__mul_alloc_no_lock(uint64 n) {
	struct run *pa;
	struct run **pprev;

	pa = multiple.freelist;
	pprev = &(multiple.freelist);

	while (NULL != pa) {
		if (pa->npage >= n) {
			struct run *ret = (struct run*)(
				(uint64)pa + PGSIZE * (pa->npage - n)
			);
			if (pa == ret) {	// this block is used up, remove it
				*pprev = pa->next;
			}
			else {
				pa->npage -= n;
				pa = ret;
			}

			multiple.npage -= n;
			break;
		}

		pprev = &(pa->next);
		pa = pa->next;
	}

	return (void*)pa;
}

// free n pages
static void __mul_free_no_lock(uint64 start, uint64 n) {
	if (NULL == multiple.freelist) {
		multiple.freelist = (struct run*)start;
		multiple.freelist->npage = n;
		multiple.freelist->next = NULL;
		multiple.npage = n;

		return ;
	}

	struct run *prev = NULL;
	struct run *next = multiple.freelist;
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
		multiple.freelist = pa;
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

	multiple.npage += n;
}

static void __mul_freerange(uint64 pa_start, uint64 pa_end) {
	uint64 start = PGROUNDUP(pa_start);
	uint64 npage = (pa_end - start) / PGSIZE;

	__enter_mul_cs 
	__mul_free_no_lock(pa_start, npage);
	__leave_mul_cs 
}


static void *__sin_alloc_no_lock(void) {
	struct run *ret = single.freelist;

	if (NULL != ret) {
		single.freelist = ret->next;
		single.npage -= 1;
	}

	return ret;
}

static void __sin_free_no_lock(uint64 pa) {
	struct run *page = (struct run*)pa;

	page->next = single.freelist;
	single.freelist = page;
	single.npage += 1;
}

static void __sin_freerange(uint64 pa_start, uint64 pa_end) {
	uint64 start = PGROUNDUP(pa_start);

	__enter_sin_cs 
	while (start < pa_end) {
		__sin_free_no_lock(start);
		start += PGSIZE;
	}
	__leave_sin_cs 
}

void kpminit(void) {
	// init multiple 
	multiple.freelist = NULL;
	multiple.npage = 0;
	initlock(&multiple.lock, "multi-lock");

	// init single 
	single.freelist = NULL;
	single.npage = 0;
	initlock(&single.lock, "single-lock");

	__assert("kpminit", START_SINGLE - (uint64)boot_stack_top >= PGSIZE, 
			"START_SINGLE = %p, boot_stack_top = %p\n", 
			START_SINGLE, boot_stack_top);
	__mul_freerange((uint64)boot_stack_top, START_SINGLE);
	__sin_freerange(START_SINGLE, PHYSTOP);

	__debug_info("kpminit", "kernel_end: %p, start_single: %p, phystop: %p, npage %d\n", 
			kernel_end, (void*)START_SINGLE, (void*)PHYSTOP, multiple.npage + single.npage);
}

void *allocpage_n(uint64 n) {
	__debug_assert("allocpage_n", n > 1, "n = %d\n", n);

	void *ret;
	__enter_mul_cs 
	ret = __mul_alloc_no_lock(n);
	__leave_mul_cs 

	#ifdef DEBUG
	if (NULL != ret) {
		memset(ret, 0x5, n * PGSIZE);
	}
	#endif 
	__debug_info("allocpage_n", "%p %d page(s)\n", ret, n);

	return ret;
}

void freepage_n(uint64 start, uint64 n) {
	__debug_assert("freepage_n", 
		(start >= (uint64)kernel_end && start < START_SINGLE) && 
		(0 == start % PGSIZE) && n > 1, 
		"start = %p, n = %d\n", start, n
	);

	__enter_mul_cs 
	__mul_free_no_lock(start, n);
	__leave_mul_cs 

	__debug_info("freepage_n", "%p %d page(s)\n", start, n);
}

uint64 _allocpage(void) {
	struct run *ret;

	__enter_sin_cs 
	ret = __sin_alloc_no_lock();
	__leave_sin_cs 

	if (NULL == ret) {
		// we can't allocate one from single, let's borrow one from multiple
		__enter_mul_cs 
		ret = __mul_alloc_no_lock(1);
		__leave_mul_cs 
	}

	#ifdef DEBUG
	if (NULL != ret) {
		memset(ret, 5, PGSIZE);
	}
	#endif 
	__debug_info("allocpage", "%p\n", ret);

	return (uint64)ret;
}

void _freepage(uint64 pa) {
	uint64 start = (uint64)pa;

	__debug_assert("freepage", 
		(start >= (uint64)kernel_end && start < PHYSTOP) && 
		0 == start % PGSIZE, 
		"start = %p\n", start
	);

	if (start < START_SINGLE) {
		__enter_mul_cs 
		__mul_free_no_lock(start, 1);
		__leave_mul_cs 
	}
	else {
		__enter_sin_cs 
		__sin_free_no_lock(start);
		__leave_sin_cs 
	}

	__debug_info("freepage", "%p\n", start);
}

uint64 idlepages(void) {
	return multiple.npage + single.npage;
}

void pm_dump(void) {
	__enter_mul_cs 
	struct run *pa = multiple.freelist;
	printf("multiple avail: %d\n", multiple.npage);
	while (NULL != pa) {
		printf("\t%p %d\n", pa, pa->npage);
		pa = pa->next;
	}
	__leave_mul_cs 

	__enter_sin_cs 
	printf("single avail: %d\n", single.npage);
	__leave_sin_cs 
}