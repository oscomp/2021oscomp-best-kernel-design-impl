#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

/* round with bit */
#define ROUND(a,n) ((a) & (~((1lu << (n)) - 1lu)) )
#define ROUNDUP(a,n) (( (a) + (1lu << (n)) - 1lu  ) & (~( (1lu << (n)) - 1lu )) )
/* round with number */
#define ROUND2(a,n) ((a) & (~((n) - 1lu)) )
#define ROUNDUP2(a,n) (((a) + ((n) - 1lu)) & (~((n) - 1lu)) )

#endif