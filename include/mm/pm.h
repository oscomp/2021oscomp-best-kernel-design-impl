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

/* allocate 1 physicl page */
uint64 _allocpage(void);

/* free 1 allocated physical page */
void _freepage(uint64 start);

// static inline void *allocpage(void) {
// 	return (void*)_allocpage();
// }
#define allocpage() ((void*)_allocpage())
// static inline void freepage(void *start) {
// 	_freepage((uint64)start);
// }
#define freepage(start) 	_freepage((uint64)(start))

uint64 idlepages(void);

void pm_dump(void);

#endif
