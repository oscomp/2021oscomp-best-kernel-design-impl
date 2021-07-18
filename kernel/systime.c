// Time-Related Syscalls 

#ifndef __DEBUG_systime 
#undef DEBUG 
#endif 

#define __module_name__ 	"systime" 

#include "include/param.h"
#include "include/types.h"
#include "include/syscall.h"
#include "include/printf.h"
#include "include/proc.h"
#include "include/timer.h"
#include "include/vm.h"
#include "include/riscv.h"
#include "include/errno.h"
#include "include/time.h"

#include "include/debug.h"

uint64 sys_times(void) {
	uint64 tms;
	if (argaddr(0, &tms) < 0) {
		return -1;
	}

	struct proc *p = myproc();
	if (tms && copyout2(tms, (char*)&(p->proc_tms), sizeof(p->proc_tms)) < 0) {
		return -1;
	}
	return ticks;
}

typedef struct timeval TimeVal;

uint64 sys_gettimeofday(void) {
	TimeVal tval;
	uint64 tmp_ticks = r_time();
	tval.sec = tmp_ticks / CLK_FREQ;
	tval.usec = tmp_ticks / (CLK_FREQ / 100000);
	tval.usec = (tval.usec % 100000) * 10;

	uint64 ptval;
	if (argaddr(0, &ptval) < 0) {
		return -1;
	}

	if (ptval && copyout2(ptval, (char*)&tval, sizeof(tval)) < 0) {
		return -1;
	}
	return 0;
}

uint64 sys_adjtimex(void) {
	return 0;
}

uint64 sys_clock_settime(void) {
	int clockid;
	uint64 tp;

	if (argint(0, &clockid) < 0) {
		return -EINVAL;
	}
	if (argaddr(0, &tp) < 0) {
		return -EINVAL;
	}

	if (CLOCK_REALTIME != clockid) {
		return -EINVAL;
	}

	struct timespec cpu_clock;
	uint64 tmp_ticks = r_time();
	cpu_clock.sec = tmp_ticks / CLK_FREQ;
	cpu_clock.nsec = 0;

	if (tp && copyin2((char*)&cpu_clock, tp, sizeof(struct timespec)) < 0) {
		return -EFAULT;
	}

	return 0;
}

uint64 sys_clock_gettime(void) {
	int clockid;
	uint64 tp;

	if (argint(0, &clockid) < 0) {
		return -EINVAL;
	}
	if (argaddr(0, &tp) < 0) {
		return -EINVAL;
	}

	if (CLOCK_REALTIME != clockid) {
		return -EINVAL;
	}

	struct timespec cpu_clock;
	if (tp && copyout2(tp, (char*)&cpu_clock, sizeof(struct timespec)) < 0) {
		return EFAULT;
	}

	return 0;
}