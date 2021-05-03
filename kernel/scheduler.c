// Process Scheduling 

#include "include/riscv.h"
#include "include/types.h"
#include "include/intr.h"
#include "include/spinlock.h"
#include "include/string.h"

#include "include/scheduler.h"

// Per-CPU state 
struct cpu cpus[NCPU];

int cpuid(void) {
	int id = r_tp();
	return id;
}

struct cpu *mycpu(void) {
	int id = cpuid();
	struct cpu *c = &cpus[id];

	return c;
}

struct proc *myproc(void) {
	push_off();
	struct cpu *c = mycpu();
	struct proc *p = c->proc;
	pop_off();

	return p;
}

// process queues 
struct proc *irq;		// IRQ have the highest priority 
struct proc *normal;	// normal processes 
struct spinlock irq_lock;
struct spinlock normal_lock;

static void _enqueue(struct proc **queue, struct proc *p) {
	struct proc *tmp = *queue;

	while (NULL != tmp) {
		queue = &(tmp->next);
		tmp = tmp->next;
	}

	*queue = p;
	p->next = NULL;
}

#define IRQ_TIMER_DEFAULT 		3
#define NORMAL_TIMER_DEFAULT 	10 

void enqueue_irq(struct proc *p) {
	// enter critical section 
	acquire(&irq_lock);

	p->timer = IRQ_TIMER_DEFAULT;
	_enqueue(&irq, p);

	release(&irq_lock);
	//leave critical section 
}

void enqueue_normal(struct proc *p) {
	// enter critical section 
	acquire(&normal_lock);

	p->timer = NORMAL_TIMER_DEFAULT;
	_enqueue(&normal, p);

	release(&normal_lock);
	// leave critical section 
}

void update_irq(void) {
	struct proc *cur;
	struct proc **pre;

	// enter critical section 
	acquire(&irq_lock);

	// update timer 
	for (cur = irq; NULL != cur; cur = cur->next) {
		if (0 != cur->timer) 
			cur->timer --;
	}

	// re-order irq queue 
	cur = irq;
	pre = &irq;
	while (NULL != cur) {
		if (0 == cur->timer) {
			*pre = cur->next;
			cur->next = irq;
			irq = cur;
			cur = *pre;
		}
		else {
			pre = &(cur->next);
			cur = cur->next;
		}
	}

	release(&irq_lock);
	// leave critical section 
}

void update_normal(void) {
	struct proc *cur;
	struct proc **pre;

	// enter critical section 
	acquire(&normal_lock);

	// update timer 
	for (cur = normal; NULL != cur; cur = cur->next) {
		if (0 != cur->timer) 
			cur->timer --;
	}

	// re-order normal queue 
	cur = normal;
	pre = &normal;
	while (NULL != cur) {
		if (0 == cur->timer) {
			*pre = cur->next;
			enqueue_irq(cur);
			cur = *pre;
		}
		else {
			pre = &(cur->next);
			cur = cur->next;
		}
	}

	release(&normal_lock);
	// leave critical section 
}

extern void swtch(struct context *a, struct context *b);

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void scheduler(void) __attribute__((noreturn)) {
	extern pagetable_t kernel_pagetable;
	struct cpu *c = mycpu();
	struct proc *p;

	while (1) {
		// avoid deadlock 
		intr_on();

		// first acquire `normal_lock`, to avoid deadlock with `update_normal`
		acquire(&normal_lock);
		acquire(&irq_lock);
		// get proc to run, and remove it from the queue 
		if (NULL != irq) {
			p = irq;
			irq = irq->next;
		}
		else if (NULL != normal) {
			p = normal;
			normal = normal->next;
		}
		else {
			p = NULL;
		}
		release(&normal_lock);
		release(&irq_lock);

		// if no proc found 
		if (NULL == p) {
			// avoid endless looping in `scheduler()`
			intr_on();
			asm volatile("wfi");
		}
		// switch to proc 
		c->proc = p;
		w_satp(MAKE_SATP(p->kpagetable));
		sfence_vma();
		swtch(&c->context, &p->context);
		w_satp(MAKE_SATP(kernel_pagetable));
		sfence_vma();
		// Process is done running for now, 
		c->proc = NULL;
	}
}

void sched(void) {
	int intena;
	struct proc *p = myproc();

	if (1 != mycpu()->noff) 
		panic("sched locks");
	if (intr_get()) 
		panic("sched interruptible");

	intena = mycpu()->intena;
	swtch(&p->context, &mycpu()->context);
	mycpu()->intena = intena;
}

void yield(void) {
	struct proc *p = myproc();

	// re-enqueue p into normal queue 
	enqueue_normal(p);
	sched();
}

void schedinit(void) {
	// init cpus 
	memset(cpus, 0, sizeof(cpus));

	// init queue 
	irq = normal = NULL;
	initlock(&irq_lock, "irq_lock");
	initlock(&normal_lock, "normal_lock");
}

void procdump(void) {
	
}

uint64 procnum(void) {
	
}
