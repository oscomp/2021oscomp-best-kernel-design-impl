/* 
 * Physical Page Allocator 
 */

#ifndef __PM_H
#define __PM_H

#include "types.h"

// /* init the allocator */
// void            kpminit(void);

// /* allocate a physical page */
// void*           allocpage(void);

// /* free an allocated phyiscal page */
// void            freepage(void *);

// uint64          idlepages(void);

/* init the allocator */
void kpminit(void);

/* allocate n physical page(s) */
void *allocpage_n(uint64 n);

/* free n allocated physical page(s) */
void freepage_n(uint64 start, uint64 n);

/* allocate a physical page */
static inline void *allocpage(void) {
	return allocpage_n(1);
}

/* free an allocated physical page */
static inline void freepage(void *start) {
	freepage_n((uint64)start, 1);
}

uint64 idlepages(void);

void pm_dump(void);

#endif
