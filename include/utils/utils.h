#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#include <sbi.h>

/* round with bit */
#define ROUNDDOWN(a,n) ((a) & (~((1lu << (n)) - 1lu)) )
#define ROUNDUP(a,n) (( (a) + (1lu << (n)) - 1lu  ) & (~( (1lu << (n)) - 1lu )) )
/* round with number, only works for n = power of two */
#define ROUNDDOWN_NUM(a, n) (((uint64_t)(a)) & ~((n)-1))
#define ROUNDUP_NUM(a,n) (((a) + ((n) - 1lu)) & (~((n) - 1lu)) )

#ifndef minmax
    #define min(x,y) (((x) < (y))? (x) : (y))
    #define max(x,y) (((x) > (y))? (x) : (y))
#endif

#define debugger_print() \
    sbi_console_putchar('6')

#endif