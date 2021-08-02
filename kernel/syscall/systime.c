// Time-Related Syscalls 

#ifndef __DEBUG_systime 
#undef DEBUG 
#endif 

#define __module_name__ 	"systime" 

#include "param.h"
#include "types.h"
#include "syscall.h"
#include "printf.h"
#include "sched/proc.h"
#include "timer.h"
#include "mm/vm.h"
#include "hal/riscv.h"
#include "errno.h"
#include "time.h"

#include "utils/debug.h"

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
	if (argaddr(1, &tp) < 0) {
		return -EINVAL;
	}

	if (CLOCK_REALTIME != clockid) {
		return -EINVAL;
	}

	struct timespec cpu_clock;
	// uint64 tmp_ticks = r_time();
	// cpu_clock.sec = tmp_ticks / CLK_FREQ;
	// cpu_clock.nsec = 0;
	uint64 tmp_ticks = readtime();
	convert_to_timespec(tmp_ticks, &cpu_clock);

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
	if (argaddr(1, &tp) < 0) {
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

uint64 sys_setitimer(void)
{
	int which;
	uint64 newptr;
	uint64 oldptr;
	struct itimeval newval;
	// struct proc *p;

	argint(0, &which);
	argaddr(1, &newptr);
	argaddr(2, &oldptr);

	if (which != CLOCK_REALTIME)
		return -EINVAL;

	if (copyin2((char*)&newval, newptr, sizeof(struct itimeval)) < 0)
		return -EFAULT;	

	// p = myproc();
	// if (oldptr && copyout2(oldptr, (char*)&p->alarmtimer, sizeof(struct itimeval)) < 0)
	// 	return -EFAULT;

	__debug_info("sys_setitimer", "new={%ds|%dus, %ds|%dus}\n",
				newval.interval.sec, newval.interval.usec, newval.value.sec, newval.value.usec);

	// __debug_info("sys_setitimer", "new={%ds|%dus, %ds|%dus}, old={%ds|%dus, %ds|%dus}\n",
	// 			newval.interval.sec, newval.interval.usec, newval.value.sec, newval.value.usec,
	// 			p->alarmtimer.interval.sec, p->alarmtimer.interval.usec,
	// 			p->alarmtimer.value.sec, p->alarmtimer.value.usec);

	// p->alarmtimer = newval;

	return 0;
}