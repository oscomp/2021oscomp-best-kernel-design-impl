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
#include "include/time.h"

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
