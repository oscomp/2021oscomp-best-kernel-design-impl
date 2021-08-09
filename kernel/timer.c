// Timer Interrupt handler

#include "types.h"
#include "param.h"
#include "hal/riscv.h"
#include "sbi.h"
#include "sync/spinlock.h"
#include "timer.h"
#include "printf.h"
#include "sched/proc.h"

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
	sbi_set_timer(r_time() + INTERVAL);
}

void timer_tick() {
	if (cpuid() == 0) {
		ticks++;
		wakeup(&ticks);
		
		// static uint64 last = 0;
		// uint64 now = readtime();
		// if (now - last >= CLK_FREQ) {
		// 	last = now;
		// 	printf("%d s\n", now / CLK_FREQ);
		// }
	}
	set_next_timeout();
}
