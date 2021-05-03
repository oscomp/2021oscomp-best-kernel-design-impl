// Process Scheduling 

#ifndef __SCHEDULER_H
#define __SCHEDULER_H 

#include "proc.h"
#include "spinlock.h"

// Per-CPU state.
struct cpu {
	struct proc *proc;			// The process running on this cpu, or null.
	struct context context;		// swtch() here to enter scheduler().
	int noff;					// Depth of push_off() nesting.
	int intena;					// Were interrupts enabled before push_off()?
};

int cpuid(void);
struct cpu *mycpu(void);
struct proc *myproc(void);

void scheduler(void) __attribute__((noreturn));
/* return to scheduler() */
void sched(void);

/* give up CPU and enter scheduler() */
void yield(void);

/* sleep to wait for event `chan`, and release the lock held by 
	the proc */
void sleep(void *chan, struct spinlock *lk);
/* wakeup all proc sleeping on event `chan` */
void wakeup(void *chan);

/* queue `p` to the end of `irq` list */
void enqueue_irq(struct proc *p);

/* update `irq` list, make procs with timer == 0 to be handled first */
void update_irq(void);

/* queue `p` to the end of `normal` list */
void enqueue_normal(struct proc *p);

/* update `normal` list, make procs with timer == 0 to be handled first */
void update_normal(void);

/* initialize */
void schedinit(void);

#endif 
