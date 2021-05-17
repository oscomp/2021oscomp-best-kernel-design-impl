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

// `p` is guaranteed with every component 
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
	enqueue_normal(np);
	return pid;
}

// `zombie` are procs that have exited or been killed, waiting for 
// parent to get `xstate`
struct proc *zombie;
struct spinlock zombie_lock;
// turn p into ZOMBIE!!! Trying to eat your brain!
static void turn_zombie(struct proc *p) {
	acquire(&zombie_lock);	// enter cs 

	if (NULL == zombie) {
		zombie = p;
		p->prev = &zombie;
	}
	else {
		struct proc *tmp = zombie;
		while (NULL != tmp->next) 
			tmp = tmp->next;
		tmp->next = p;
		p->prev = &(tmp->next);
	}
	p->next = NULL;
	p->state = ZOMBIE;

	release(&zombie_lock);	// leave cs 
}
// find parent's zombie child 
static struct proc *corrupt_zombie(struct proc *parent) {
	struct proc *tmp = NULL;

	acquire(&zombie_lock);	// enter cs 

	tmp = zombie;
	while (NULL != tmp) {
		if (parent == tmp->parent) {
			// find it! remove the corrupt from zombies 
			*(tmp->prev) = tmp->next;
			tmp->next->prev = tmp->prev;
			break;
		}
		else {
			tmp = tmp->next;
		}
	}

	release(&zombie_lock);	// leave cs 

	return tmp;
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

	// TODO: memory 

	// remove itself from proc queue 
	acquire(&proc_lock);	// enter cs proc 
	*(p->prev) = p->next;
	p->next->prev = p->prev;
	release(&proc_lock);	// leave cs proc 

	p->xstate = xstate;
	turn_zombie(p);

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

struct cpu cpus[NCPU];

// queue for scheduling 
struct proc *timeout;
struct proc *irq;
struct proc *normal;
struct spinlock *proc_lock;
// lock should be acquired before 
static void _enqueue(struct proc **head, struct proc *p) {
	struct proc *tmp = *head;
	while (NULL != tmp) {
		head = &(tmp->next);
		tmp = tmp->next;
	}
	*head = p;
	p->next = NULL;
	p->prev = head;
}
// search for the first proc that fits func(p, value) != 0
// proc_lock must be acquired before 
static struct proc *search_proc(
	int (*func)(struct proc*, void*), 
	void *value
) {
	struct proc *tmp;

	tmp = timeout;
	while (NULL != tmp) {
		if (func(tmp, value)) return tmp;
		else {
			tmp = tmp->next;
		}
	}

	tmp = irq;
	while (NULL != tmp) {
		if (func(tmp, value)) return tmp;
		else {
			tmp = tmp->next;
		}
	}

	tmp = normal;
	while (NULL != tmp) {
		if (func(tmp, value)) return tmp;
		else {
			tmp = tmp->next;
		}
	}

	return NULL;
}
// do the same thing for every proc that meets the condition 
// also proc_lock should be held 
static void foreach_proc(
	int (*func)(struct proc*, void*), 
	void *value, 
	void (*handler)(struct proc*, void*), 
	void *hvalue
) {
	struct proc *tmp;

	tmp = timeout;
	while (NULL != tmp) {
		if (func(tmp, value)) {
			handler(tmp, hvalue);
		}
		else {
			tmp = tmp->next;
		}
	}

	tmp = irq;
	while (NULL != tmp) {
		if (func(tmp, value)) {
			handler(tmp, hvalue);
		}
		else {
			tmp = tmp->next;
		}
	}

	tmp = normal;
	while (NULL != tmp) {
		if (func(tmp, value)) {
			handler(tmp, hvalue);
		}
		else {
			tmp = tmp->next;
		}
	}
}

#define IRQ_TIMER 	5
void enqueue_irq(struct proc *p) {
	acquire(&p->lk);		// enter cs p 
	// update state and timer 
	p->state = RUNNABLE;
	p->timer = IRQ_TIMER;
	// enqueue to irq 
	acquire(&proc_lock);	// enter cs proc 
	_enqueue(&irq, p);
	release(&proc_lock);	// leave cs proc 
	release(&p->lk);		// leave cs p 
}

#define NORMAL_TIMER 	10 
void enqueue_normal(struct proc *p) {
	acquire(&p->lk);		// enter cs p 
	// update state and timer 
	p->state = RUNNABLE;
	p->timer = NORMAL_TIMER;
	// enqueue to normal 
	acquire(&proc_lock);	// enter cs proc 
	_enqueue(&normal, p);
	release(&proc_lock);	// leave cs proc 
	release(&p->lk);		// leave cs proc 
}

void proc_tick(void) {
	struct proc *tmp;

	acquire(&proc_lock);	// enter cs proc 

	tmp = normal;
	while (NULL != tmp) {
		acquire(&tmp->lk);
		if (RUNNABLE != tmp->state) {
			release(&tmp->lk);
			continue;
		}
		if (0 == tmp->timer) {
			struct proc *next = tmp->next;
			// remove tmp from normal 
			*(tmp->prev) = tmp->next;
			tmp->next->prev = tmp->prev;
			// re-enqueue tmp to timeout 
			_enqueue(&timeout, tmp);

			tmp = next;
		}
		else {
			tmp->timer = tmp->timer - 1;
			tmp = tmp->next;
		}
		release(&tmp->lk);
	}

	tmp = irq;
	while (NULL != tmp) {
		acquire(&tmp->lk);
		if (RUNNABLE != tmp->state) {
			release(&tmp->lk);
			continue;
		}
		if (0 == tmp->timer) {
			struct proc *next = tmp->next;
			// remove tmp from irq 
			*(tmp->prev) = tmp->next;
			tmp->next->prev = tmp->prev;
			// re-enqueue tmp to timeout 
			_enqueue(&timeout, tmp);

			tmp = next;
		}
		else {
			tmp->timer = tmp->timer - 1;
			tmp = tmp->next;
		}
		release(&tmp->lk);
	}

	release(&proc_lock);	// leave cs proc 
}

int wait(uint64 addr) {
	struct proc *p = myproc();

	#ifdef DEBUG 
	if (NULL == p) {
		__debug_error("wait", "init calls wait()\n");
		return -1;
	}
	#endif 

	// hold p->lk for the whole time to avoid lost 
	// wakeups from a child's exit() 
	acquire(&p->lk);

	struct proc *tmp;
	while (1) {
		acquire(&zombie_lock);	// enter cs zombie 
		tmp = corrupt_zombie(p);
		release(&zombie_lock);	// leave cs zombie 
		if (NULL != tmp) {
			int ret = tmp->pid;
			freeproc(tmp);
			return ret;
		}
		// if not in zombie, then check if p has any child 
		if (NULL == p->child) {	// if no child 
			release(&p->lk);
			return -1;
		}
		// wait for child to exit 
		sleep(p, &p->lk);
	}
}

int kill(int pid) {
	struct proc *tmp;

	#ifdef DEBUG 
	if (1 == pid) {
		__debug_warn("kill", "try to kill init\n");
	}
	#endif 

	// search proc queue 
	acquire(&proc_lock);	// enter cs proc 
	// define nested function to search list 
	// nested function is GNU C extension, may not be supported 
	// by other compilers 
	int func(struct proc *p, void *value) {
		int pid = (int)value;
		int ret;
		acquire(&p->lk);
		ret = (p->pid == pid);
		release(&p->lk);
		return ret;
	}
	int handle(struct proc *p, void *value) {
		acquire(&p->lk);
		p->killed = 1;
		release(&p->lk);
	}
	search_proc(func, (void*)pid, handle, NULL);
	release(&proc_lock);	// leave cs proc 

	return 0;
}

void yield(void) {}

void sleep(void *chan, struct spinlock *lk) {}

void wakeup(void *chan) {}

void scheduler(void) __attribute__((noreturn)) {}

void sched(void);

void procinit(void) {
	// init pid 
	pid = 2;	// 1 is reserved for in-kernel process `init`
	initlock(&pid_lock, "pid_lock");

	// init zombie 
	zombie = NULL;
	initlock(&zombie_lock, "zombie_lock");

	// init cpus 
	memset(cpus, 0, sizeof(cpus));

	// init queue 
	timeout = irq = normal = NULL;
	initlock(&proc_lock, "proc_lock");
}
