#ifndef __TIME_H
#define __TIME_H

#include "types.h"

struct timespec {
	uint64 sec;			/* seconds */
	uint64 nsec;		/* nanoseconds */
};

struct timeval {
	uint64 sec;			/* seconds */
	uint64 usec;		/* microseconds or milliseconds? */
};

#endif