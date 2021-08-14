/* 
 * Physical Page Allocator 
 */

#ifndef __PM_H
#define __PM_H

#include "types.h"
#include "utils/debug.h"

/* init the allocator */
void kpminit(void);

/* allocate n physical page(s) */
void *allocpage_n(uint64 n);

/* free n allocated physical page(s) */
void freepage_n(uint64 start, uint64 n);

#define allocpage() ({\
	void *page = allocpage_n(1);\
	__debug_info("allocpage", "at %s: %d, %p\n", __FILE__, __LINE__, page);\
	page;\
})

#define freepage(start) do {\
	__debug_info("freepage", "at %s: %d, %p\n", __FILE__, __LINE__, start);\
	freepage_n((uint64)start, 1);\
} while (0)

uint64 idlepages(void);

void pm_dump(void);

#endif
