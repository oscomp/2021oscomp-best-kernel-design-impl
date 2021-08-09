// Timer Interrupt handler

#ifndef __DEBUG_timer
#undef DEBUG
#endif 

#include "types.h"
#include "param.h"
#include "hal/riscv.h"
#include "sbi.h"
#include "sync/spinlock.h"
#include "timer.h"
#include "printf.h"
#include "sched/proc.h"

#include "utils/debug.h"

struct spinlock tickslock;
uint64 ticks;

void timerinit() {
	initlock(&tickslock, "time");
	ticks = 0;

	#ifdef DEBUG
	printf("timerinit\n");
	#endif
}

void
set_next_timeout() {
	// There is a very strange bug,
	// if comment the `printf` line below
	// the timer will not work.

	// this bug seems to disappear automatically
	// printf("");
	// sbi_set_timer(r_time() + INTERVAL);
	struct sbiret result = sbi_xv6_get_timer();
	__debug_assert("set_next_timeout", SBI_SUCCESS == result.error, "SBI call failed");
	result = sbi_set_timer(result.value + INTERVAL);
	__debug_assert("set_next_timeout", SBI_SUCCESS == result.error, "SBI call failed");
}

void timer_tick() {
	if (cpuid() == 0) {
		ticks++;
		wakeup(&ticks);
	}
	set_next_timeout();
}
