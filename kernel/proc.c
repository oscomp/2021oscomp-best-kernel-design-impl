// Process Management 

#ifndef __DEBUG_proc 
#undef DEBUG 
#endif 

#include "include/types.h"
#include "include/scheduler.h"
#include "include/spinlock.h"
#include "include/kmalloc.h"
#include "include/pm.h"
#include "include/string.h"
#include "include/vm.h"
#include "include/debug.h"

#include "include/proc.h"

// pid management 
int pid;
struct spinlock pid_lock;
static int allopid(void) {
	int ret;
	acquire(&pid_lock);
	ret = pid;
	pid = pid + 1;
	release(&pid_lock);

	return ret;
}

struct cpu cpus[NCPU];

// lists for scheduling 

#define TIMER_IRQ 			5
#define TIMER_NORMAL 		10 

#define PRIORITY_TIMEOUT 	0
#define PRIORITY_IRQ 		1
#define PRIORITY_NORMAL 	2
#define PRIORITY_NUMBER 	3 
struct proc *proc_runnable[PRIORITY_NUMBER];
struct proc *proc_sleep;
struct proc *proc_zombie;
struct spinlock proc_lock;

#define __enter_proc_cs \
	acquire(&proc_lock);
#define __leave_proc_cs \
	release(&proc_lock);

// before modifying proc_list, lock should be acquired 
static void proc_list_insert(struct proc **head, struct proc *p) {
	__debug_assert("proc", NULL != p, "insert NULL into list\n");

	struct proc *tmp = *head;
	while (NULL != tmp) {
		head = &(tmp->next);
		tmp = tmp->next;
	}
	*head = p;
	p->prev = head;
	p->next = NULL;
}
static void proc_list_remove(struct proc *p) {
	__debug_assert("proc", NULL != p, "remove NULL from list\n");

	*(p->prev) = p->next;
	p->next->prev = p->prev;
	p->prev = NULL;
	p->next = NULL;
}

#define __insert_runnable(priority, p) \
	proc_list_insert(&proc_runnable[priority], p)
#define __insert_sleep(p) \
	proc_list_insert(&proc_sleep, p)
#define __insert_zombie(p) \
	proc_list_insert(&proc_zombie, p)
#define __remove(p) \
	proc_list_remove(p) 

// alloc a proc ready for running 
static struct proc *allocproc(void) {
	struct proc *p = kmalloc(sizeof(struct proc));

	if (NULL == p) {
		__debug_warn("allocproc", "fail to kmalloc() proc");
		return NULL;
	}

	// init lock here
	initlock(&(p->lk), "proc");

	p->chan = NULL;
	p->state = RUNNABLE;
	p->killed = 0;
	p->pid = allocpid();

	p->child = NULL;

	// allocate a page trapframe 
	p->trapframe = (struct trapframe*)allopage();
	if (NULL == p->trapframe) {
		__debug_warn("allocproc", "fail to alloc trapframe\n");
		kfree(p);
		return NULL;
	}

	// TODO: pagetable 

	// init ofile 
	for (int i = 0; i < NOFILE; i ++) 
		np->ofile = NULL;

	return p;
}

static void freeproc(struct proc *p) {
	// file system fields were freed in exit() 

	// TODO: pagetable 

	// free trapframe 
	freepage(p->trapframe);

	// parenting is handled in exit() 

	// free the proc itself 
	kfree(p);
}

int fork(void) {
	struct proc *p = myproc();
	struct proc *np;

	#ifdef DEBUG 
	// if `init` calls fork, in a stable kernel this should never happen 
	if (NULL == p) {
		__debug_error("fork", "myproc() is init\n");
		return -1;
	}
	#endif 

	// allocate proc 
	np = allocproc();
	if (NULL == np) {
		__debug_warn("fork", "fail to allocate new proc\n");
		return -1;
	}

	// TODO 
	// copy parent's memory, leave it blank here as pagetable scheme unknown 

	// parenting 
	// as `p` is the running process, its lock is held in scheduler() 
	acquire(&p->lk);	// enter cs p 
	np->parent = np;
	np->sibling = p->child;
	p->child = np;
	release(&p->lk);	// leave cs p 

	// filesystem 
	for (int i = 0; i < NOFILE; i ++) {
		if (NULL != p->ofile[i]) {
			np->ofile[i] = filedup(p->ofile[i]);
		}
	}
	np->cwd = edup(p->cwd);

	// copy parent's context 
	*(np->context) = *(p->context);

	// copy debug info 
	safestrcpy(np->name, p->name, sizeof(p->name));

	int pid = np->pid;

	__enter_proc_cs 
	// init timer 
	np->timer = TIMER_NORMAL;
	// set runnable 
	__insert_runnable(PRIORITY_NORMAL, np);
	__leave_proc_cs 

	return pid;
}

void exit(int xstate) {
	struct proc *p = myproc();

	// free filesystem fields 
	for (int fd = 0; fd < NOFILE; fd ++) {
		if (NULL != p->ofile[fd]) {
			struct file *f = p->ofile[fd];
			fileclose(f);
		}
	}
	eput(p->cwd);

	// write in xstate 
	p->xstate = xstate;

	// TODO: memory 

	__enter_proc_cs 

	__remove(p);		// remove p from its list 
	p->state = ZOMBIE;
	__insert_zombie(p);	// re-insert p into `proc_zombie`

	__leave_proc_cs 

	// remove itself from parent's child list 
	// concurrency safety of sibling list is protected by p->parent->lk 
	if (NULL != p->parent) {
		acquire(&(p->parent->lk));	// enter cs p->parent 
		if (p->parent->child == p) {
			p->parent->child = p->sibling;
		}
		else {
			struct proc *tmp = p->parent->child;
			while (tmp && tmp->sibling != p) {
				tmp = tmp->sibling;
			}
			#ifdef DEBUG 
			if (NULL == tmp) {
				panic("exit(): p is not in child list\n");
			}
			#endif 

			tmp->sibling = p->sibling;
		}
		release(&(p->parent->lk));	// leave cs p->parent 
	}

	// re-parent all it's child to `init`
	acquire(&p->lk);	// enter cs p 
	struct proc *tmp = p->child;
	while (NULL != tmp) {
		tmp->parent = NULL;	// re-parent to init 
		tmp = tmp->sibling;
	}
	release(&p->lk);	// leave cs p 

	// p is freed from parent's child list, no long 
	// needs lock for protection 
	wakeup(p->parent);

	// jump into scheduler 
	sched();
	panic("exit(): zombie exit\n");
}

int wait(uint64 addr) {
	struct proc *np;
	struct proc *p = myproc();

	__enter_proc_cs 
	while(1) {
		np = proc_zombie;
		while (NULL != np) {
			if (np->parent == p) {
				// find one 
				int pid = np->pid;
				// TODO: copy xstate out 
				__remove(np);
				freeproc(np);
				__leave_proc_cs 
				return pid;
			}
			else {
				np = np->next;
			}
		}

		// if not found in zombie, maybe child proc is still 
		// runnable or sleeping? 
		int has_child = 0;
		for (int i = 0; i < PRIORITY_NUMBER; i ++) {
			np = proc_runnable[i];
			while (NULL != np) {
				if (np->parent == p) {
					has_child = 1;
					goto end_search;
				}
			}
		}
		// search sleeping list 
		np = proc_sleep;
		while (NULL != np) {
			if (np->parent == p) {
				has_child = 1;
				goto end_search;
			}
		}
		end_search: if (has_child) {
			// wait for child to exit 
			sleep(p, &proc_lock);
		}
		else {	// the child(cake) is a lie!
			__leave_proc_cs 
			return -1;
		}
	}
}

void proc_tick(void) {
	__enter_proc_cs 

	for (int i = PRIORITY_IRQ; i < PRIORITY_NUMBER; i ++) {
		struct proc *p = proc_runnable[i];
		while (NULL != p) {
			if (RUNNING != p->state) {
				struct proc *next = p->next;
				p->timer = p->timer - 1;
				if (0 == p->timer) {	// timeout 
					__remove(p);
					__insert_runnable(PRIORITY_TIMEOUT, p);
				}
				p = next;
			}
		}
	} 

	__leave_proc_cs 
}

int kill(int pid) {
	struct proc *tmp;

	__enter_proc_cs 

	// search runnable 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (pid == tmp->pid) {
				// found 
				tmp->killed = 1;
				__leave_proc_cs 
				return 0;
			}
		}
	}
	// search sleep 
	tmp = proc_sleep;
	while (NULL != tmp) {
		if (pid == tmp->pid) {
			// found 
			tmp->killed = 1;
			// wakeup the proc 
			tmp->state = RUNNABLE;
			__remove(tmp);
			__insert_runnable(tmp, PRIORITY_NORMAL);

			__leave_proc_cs 
			return 0;
		}
	}

	__leave_proc_cs 
	return -1;
}

// turn current proc into `RUNNABLE`
// and switch to scheduler 
void yield(void) {
	struct proc *p = myproc();

	__enter_proc_cs 

	p->state = RUNNABLE;
	p->timer = TIMER_NORMAL;
	__remove(p);
	__insert_runnable(PRIORITY_NORMAL, p);

	// swtch to scheduler 
	sched();

	__leave_proc_cs 
}

void sleep(void *chan, struct spinlock *lk) {
	struct proc *p = myproc();

	release(lk);
	__enter_proc_cs 

	p->chan = chan;
	p->state = SLEEPING;
	__remove(p);	// remove p from runnable 
	__insert_sleep(p);

	sched();
	p->chan = NULL;

	__leave_proc_cs 
	acquire(lk);
}

void wakeup(void *chan) {
	__enter_proc_cs 

	struct proc *p = proc_sleep;
	while (NULL != p) {
		struct proc *next = p->next;
		if (chan == p->chan) {
			__remove(p);
			p->state = RUNNABLE;
			p->timer = TIMER_IRQ;
			__insert_runnable(PRIORITY_IRQ, p);
		}
		p = next;
	}

	__leave_proc_cs 
}

// idle proc is the implied proc running when there's 
// no proc available in runnable list. 
// idle_proc should run STRICTLY with proc_lock ACQUIRED 
int shell_pid;		// the pid of the first shell process 
static void idle_proc(void) {
	static char *argv[] = {"sh", 0};
	struct proc *tmp = proc_zombie;
	
	while (NULL != tmp) {
		
	}
}

#define TIMER_IDLE 		15
void scheduler(void) __attribute__((noreturn)) {
	struct proc *tmp;
	struct cpu *c = mycpu();
	// `idle_timer` is used to make sure that `idle` codes will 
	// be executed from time to time, to avoid starvation 
	int idle_timer = TIMER_IDLE;

	// it should never returns!
	while (1) {
		scheduler_start: 
		__enter_proc_cs 
		if (0 == idle_timer) {
			// if no proc found, run implied `idle` proc, 
			// it will clean up zombie procs whose parents are dead 
			idle_proc();
			__leave_proc_cs 
			intr_on();
			idle_timer = TIMER_IDLE;
		}
		else {
			for (int i = PRIORITY_TIMEOUT; i < PRIORITY_NUMBER; i ++) {
				tmp = proc_runnable[i];
				while (NULL != tmp) {
					if (RUNNABLE == tmp->state) {
						tmp->state = RUNNING;
						c->proc = tmp;
						// TODO: switch pagetable 
						swtch(&c->context, &p->context);
						// TODO: switch pagetable 
						__leave_proc_cs 
						intr_on();
						idle_timer -= 1;
						goto scheduler_start;
					} 
					else {
						tmp = tmp->next;
					} 
				}
			}
			__leave_proc_cs 
			intr_on();
			idle_timer = 0;
		}
	} 

	__debug_error("scheduler", "scheduler returns!\n");
	panic("critical problem\n");
}

// sched() is the only way back to scheduler() 
void sched(void) {
	int intena;
	struct proc *p = myproc();

	if (!holding(&proc_lock)) 
		panic("sched proc_lock\n");
	if (1 != mycpu()->noff) 
		panic("sched locks\n");
	if (RUNNING == p->state) 
		panic("sched running\n");
	if (intr_get()) 
		panic("sched interruptible\n");

	intena = mycpu()->intena;
	swtch(&p->context, &mycpu()->context);
	mycpu()->intena = intena;
}

void procinit(void) {
	// init pid 
	pid = 2;	// 1 is reserved for in-kernel process `init`
	initlock(&pid_lock, "pid_lock");

	// init cpus 
	memset(cpus, 0, sizeof(cpus));

	// init queue 
	for (int i = 0; i < NUM; i ++) 
		proc_runnable[i] = NULL;
	proc_sleep = NULL;
	proc_zombie = NULL;
	initlock(&proc_lock, "proc_lock");
}

void userinit(void) {
	
}
