// Process Management 

#ifndef __DEBUG_proc 
#undef DEBUG 
#endif 

#define __module_name__ 	"proc"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sync/spinlock.h"
#include "intr.h"
#include "mm/pm.h"
#include "printf.h"
#include "utils/string.h"
#include "fs/fs.h"
#include "fs/file.h"
#include "trap.h"
#include "mm/vm.h"
#include "mm/kmalloc.h"
#include "mm/usrmm.h"
#include "mesg/signal.h"
#include "errno.h"
#include "utils/debug.h"

#include "sched/proc.h"

void swtch(struct context*, struct context*);

void forkret(void);

// pid management 
int __pid;
struct spinlock pid_lock;
static int allocpid(void) {
	int ret;
	acquire(&pid_lock);
	ret = __pid;
	__pid = __pid + 1;
	release(&pid_lock);

	return ret;
}

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
static void __proc_list_insert_no_lock(struct proc **head, struct proc *p) {
	__debug_assert("proc_list_insert", NULL != p, "insert NULL into list\n");

	struct proc *tmp = *head;
	while (NULL != tmp) {
		head = &(tmp->next);
		tmp = tmp->next;
	}
	*head = p;
	p->prev = head;
	p->next = NULL;
}
static void __proc_list_remove_no_lock(struct proc *p) {
	__debug_assert("proc_list_remove", NULL != p, "remove NULL from list\n");

	*(p->prev) = p->next;
	if (NULL != p->next)
		p->next->prev = p->prev;
	p->prev = NULL;
	p->next = NULL;
}

#define __insert_runnable(priority, p) do {\
	(p)->state = RUNNABLE; \
	__proc_list_insert_no_lock(&(proc_runnable[priority]), p); \
} while (0)
#define __insert_sleep(p) do {\
	(p)->state = SLEEPING; \
	__proc_list_insert_no_lock(&proc_sleep, p); \
} while (0)
#define __insert_zombie(p) do {\
	(p)->state = ZOMBIE; \
	__proc_list_insert_no_lock(&proc_zombie, p); \
} while (0)
#define __remove(p) \
	__proc_list_remove_no_lock(p) 

static void freeproc(struct proc *p) {
	// file system fields were freed in exit() 

	// free pagetable 
	if (p->pagetable) 
		proc_freepagetable(p->pagetable, p->segment);

	// free trapframe 
	freepage(p->trapframe);

	// parenting is handled in exit() 

	// free sigact list 
	if (p->sig_act)
		sigact_free(p->sig_act);

	// free the proc itself 
	kfree(p);
}

extern char trampoline[];	// trampoline.S

// Create a user page table for a given process,
// with no user memory, but with trampoline pages.
pagetable_t
proc_pagetable(struct proc *p)
{
  pagetable_t pagetable;

  // An empty page table.
  pagetable = kvmcreate();
  if(pagetable == 0)
	return NULL;

  // map the trapframe just below TRAMPOLINE, for trampoline.S.
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
			(uint64)(p->trapframe), PTE_R | PTE_W) < 0){
	kvmfree(pagetable, 1);
	return NULL;
  }

  return pagetable;
}

// Free a process's page table, and free the
// physical memory it refers to.
void
proc_freepagetable(pagetable_t pagetable, struct seg *head)
{
	// __debug_info("proc_freepagetable", "enter\n");
  unmappages(pagetable, TRAPFRAME, 1, 0);
//   __debug_info("proc_freepagetable", "leave\n");
  delsegs(pagetable, head);
//   __debug_info("proc_freepagetable", "leave 2\n");
  uvmfree(pagetable);
  kvmfree(pagetable, 1);
}


// alloc a proc ready for running 
static struct proc *allocproc(void) {
	struct proc *p = kmalloc(sizeof(struct proc));

	if (NULL == p) {
		__debug_warn("allocproc", "fail to kmalloc() proc\n");
		return NULL;
	}

	// initlock 
	initlock(&p->lk, "p->lk");

	p->chan = NULL;
	p->killed = 0;
	p->pid = allocpid();

	p->child = NULL;

	p->sig_act = NULL;

	p->proc_tms.utime = 0;
	p->proc_tms.stime = 0;
	p->proc_tms.cutime = 0;
	p->proc_tms.cstime = 0;
	p->vswtch = 0;
	p->ivswtch = 0;

	// allocate a page trapframe 
	p->trapframe = (struct trapframe*)allocpage();
	if (NULL == p->trapframe) {
		__debug_warn("allocproc", "fail to alloc trapframe\n");
		kfree(p);
		return NULL;
	}

	// pagetable 
	p->segment = NULL;
	if (NULL == (p->pagetable = proc_pagetable(p))) {
		__debug_warn("allocproc", "fail to set pagetable\n");
		freeproc(p);
		return NULL;
	}

	// init fds 
	memset(&p->fds, 0, sizeof(p->fds));

	// user kernel stack 
	p->kstack = VKSTACK;

	p->context.ra = (uint64)forkret;
	p->context.sp = p->kstack + PGSIZE;

	return p;
}

int fork_cow(void) {
	struct proc *p = myproc();
	struct proc *np;

	// allocate proc 
	np = allocproc();
	if (NULL == np) {
		__debug_warn("fork", "fail to allocate new proc\n");
		return -1;
	}

	// copy parent's memory layout 
	np->segment = copysegs(p->pagetable, p->segment, np->pagetable);
	if (np->segment == NULL) {
		__debug_warn("fork", "fail to copy segments\n");
		freeproc(np);
		return -1;
	}
	np->pbrk = p->pbrk;

	// filesystem 
	if (copyfdtable(&p->fds, &np->fds) < 0) {
		freeproc(np);
		return -1;
	}
	np->cwd = idup(p->cwd);
	np->elf = idup(p->elf);

	// copy parent's trapframe 
	*(np->trapframe) = *(p->trapframe);
	np->trapframe->a0 = 0;

	// parenting 
	__enter_proc_cs
	np->parent = p;
	np->sibling_prev = &(p->child);
	np->sibling_next = p->child;
	if (NULL != p->child) {
		p->child->sibling_prev = &(np->sibling_next);
	}
	p->child = np;
	__leave_proc_cs

    // copy signal related information
	sigact_copy(&np->sig_act, p->sig_act);
	np->sig_set = p->sig_set;		// copy parent's mask setting 
	memset((void*)&np->sig_pending, 0, sizeof(__sigset_t));	// do not copy pending list 

	// copy debug info 
	safestrcpy(np->name, p->name, sizeof(p->name));
	np->tmask = p->tmask;

	int pid = np->pid;

	__enter_proc_cs 
	// init timer 
	np->timer = TIMER_NORMAL;
	// set runnable 
	__insert_runnable(PRIORITY_NORMAL, np);
	__leave_proc_cs 

	__debug_info("fork_cow", "leave %d -> %d\n", p->pid, pid);

	return pid;
}

// similiar to fork, but different 
// a lot can be improved later 
int clone(uint64 flag, uint64 stack) {
	// currently don't deal with flag 
	if (NULL == stack) 
		return fork_cow();

	struct proc *p = myproc();
	struct proc *np;

	// allocate proc 
	np = allocproc();
	if (NULL == np) {
		__debug_warn("fork", "fail to allocate new proc\n");
		return -1;
	}

	// copy parent's memory layout 
	np->segment = copysegs(p->pagetable, p->segment, np->pagetable);
	if (np->segment == NULL) {
		__debug_warn("fork", "fail to copy segments\n");
		freeproc(np);
		return -1;
	}
	np->pbrk = p->pbrk;

	// these parts may be improved later 
	// filesystem 
	if (copyfdtable(&p->fds, &np->fds) < 0) {
		freeproc(np);
		return -1;
	}

	np->cwd = idup(p->cwd);
	np->elf = idup(p->elf);

	// copy parent's trapframe 
	*(np->trapframe) = *(p->trapframe);
	np->trapframe->a0 = 0;
	np->trapframe->sp = stack;

	// parenting 
	__enter_proc_cs
	np->parent = p;
	np->sibling_prev = &(p->child);
	np->sibling_next = p->child;
	if (NULL != p->child) {
		p->child->sibling_prev = &(np->sibling_next);
	}
	p->child = np;
	__leave_proc_cs

	// copy signal related information
	sigact_copy(&np->sig_act, p->sig_act);
	np->sig_set = p->sig_set;		// copy parent's mask setting 
	memset((void*)&np->sig_pending, 0, sizeof(__sigset_t));	// do not copy pending list 

    __debug_assert("clone", p->pid < np->pid, "child pid bigger\n");

	// copy debug info 
	safestrcpy(np->name, p->name, sizeof(p->name));
	np->tmask = p->tmask;

	int pid = np->pid;

	__enter_proc_cs 
	// init timer 
	np->timer = TIMER_NORMAL;
	// set runnable 
	__insert_runnable(PRIORITY_NORMAL, np);
	__leave_proc_cs 

	__debug_info("clone", "leave from fork\n");

	return pid;
}

static void __wakeup_no_lock(void *chan) {
	struct proc *p = proc_sleep;
	while (NULL != p) {
		struct proc *next = p->next;
		if ((uint64)chan == (uint64)p->chan) {
			__remove(p);
			p->timer = TIMER_IRQ;
			__insert_runnable(PRIORITY_IRQ, p);
		}
		p = next;
	}
}

void wakeup(void *chan) {
	__enter_proc_cs 
	__wakeup_no_lock(chan);
	__leave_proc_cs 
}

void exit(int xstate) {
	struct proc *p = myproc();

	extern struct proc *__initproc;
	__debug_assert("exit", __initproc != p, "init exiting\n");

	__debug_info("exit", "pid %d %s\n", p->pid, p->name);

	// close all open files 
	dropfdtable(&p->fds);
	iput(p->cwd);
	iput(p->elf);

	// DO NOT REMOVE ITSELF FROM PARENT'S CHILD LIST!
	// IN CASE THAT PARENT WAS KILLED BEFORE ABLE TO WAIT 

	// write in xstate 
	p->xstate = xstate;

	__enter_proc_cs 
	// re-parent all it's child to `__initproc`
	struct proc *tmp = p->child;
	while (NULL != tmp) {
		// printf("%d\t%d\t%d\n", p->parent->pid, p->pid, tmp->pid);
		__debug_assert("exit", tmp != tmp->sibling_next, "dead loop\n");
		tmp->parent = __initproc;	// re-parent to init 
		struct proc *tmp2 = tmp->sibling_next;
		tmp->sibling_prev = &(__initproc->child);
		tmp->sibling_next = __initproc->child;
		if (NULL != __initproc->child)
			__initproc->child->sibling_prev = &(tmp->sibling_next);
		__initproc->child = tmp;
		tmp = tmp2;
	}
	// printf("%d %d done\n", p->parent->pid, p->pid);
	__debug_info("exit", "leave\n");

	// jump into scheduler 

	__remove(p);		// remove p from its list 
	__insert_zombie(p);	// re-insert p into `proc_zombie`

	// wakeup parent after the proc is inserted into zombie list 
	__wakeup_no_lock(p->parent);
	// wakeup __initproc means no bad 
	__wakeup_no_lock(__initproc);

	sched();
	__debug_error("exit", "zombie exit\n");
	panic("panic!\n");
}

#define WAIT_OPTIONS_WNOHANG 		1
#define WAIT_OPTIONS_WUNTRACED 		2
#define WAIT_OPTIONS_WCONTINUED 	8
static inline int __is_child_no_lock(int pid, struct proc *p, struct proc *np) {
	if (-1 == pid) {
		return p == np->parent;
	}
	else {
		return p == np->parent && pid == np->pid;
	}
}
int wait4(int pid, uint64 status, uint64 options) {
	struct proc *np;
	struct proc *p = myproc();

	__enter_proc_cs 

	while(1) {
		np = proc_zombie;
		while (NULL != np) {
			if (__is_child_no_lock(pid, p, np)) {
				__debug_info("wait", "%d picks up %d\n", p->pid, np->pid);
				// find one 
				int child_pid = np->pid;
				// remove child from parent's child list 
				*(np->sibling_prev) = np->sibling_next;
				if (NULL != np->sibling_next) 
					np->sibling_next->sibling_prev = np->sibling_prev;
				// add child's tms time 
				p->proc_tms.cstime += np->proc_tms.stime + np->proc_tms.cstime;
				p->proc_tms.cutime += np->proc_tms.utime + np->proc_tms.cutime;
				// copyout child's xstate 
				np->xstate <<= 8;
				if (status && copyout2(status, (char*)&np->xstate, sizeof(np->xstate))) {
					__leave_proc_cs 
					__debug_error("wait", "fail to copy out xstate\n");
					panic("panic!\n");
				}
				__remove(np);
				__leave_proc_cs 
				freeproc(np);
				return child_pid;
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
				if (__is_child_no_lock(pid, p, np)) {
					has_child = 1;
					goto end_search;
				}
				np = np->next;
			}
		}
		// search sleeping list 
		np = proc_sleep;
		while (NULL != np) {
			if (__is_child_no_lock(pid, p, np)) {
				has_child = 1;
				goto end_search;
			}
			np = np->next;
		}
		end_search: if (has_child) {
			if (0 != (options & WAIT_OPTIONS_WNOHANG)) {
				// no-hang, return immediately 
				__leave_proc_cs 
				return 0;
			}
			else {
				sleep(p, &proc_lock);
			}
		}
		else {	// the child(cake) is a lie!
			__leave_proc_cs 
			return -1;
		}
	}
}


void proc_tick(void) {
	__enter_proc_cs 

	// runnable 
	for (int i = PRIORITY_IRQ; i < PRIORITY_NUMBER; i ++) {
		struct proc *p = proc_runnable[i];
		while (NULL != p) {
			struct proc *next = p->next;
			if (RUNNING != p->state) {
				p->timer = p->timer - 1;
				if (0 == p->timer) {	// timeout 
					__remove(p);
					__insert_runnable(PRIORITY_TIMEOUT, p);
				}
			}
			// else {
			// 	p->proc_tms.utime += 1;
			// }
			// p->proc_tms.stime += 1;
			p = next;
		}
	} 

	// sleep 
	// struct proc *tmp = proc_sleep;
	// while (NULL != tmp) {
	// 	tmp->proc_tms.stime += 1;
	// 	tmp = tmp->next;
	// }

	__leave_proc_cs 
}

static struct proc *get_proc_no_lock(int pid) {
	struct proc *tmp;

	// search runnable
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (pid == tmp->pid) 
				return tmp;
			else 
				tmp = tmp->next;
		}
	}

	// search sleep 
	tmp = proc_sleep;
	while (NULL != tmp) {
		if (pid == tmp->pid) 
			return tmp;
		else 
			tmp = tmp->next;
	}

	return NULL;
}

int kill(int pid) {
	struct proc *tmp;

	__enter_proc_cs 

	tmp = get_proc_no_lock(pid);
	if (NULL == tmp) {
		__leave_proc_cs 
		return -ESRCH;
	}

	tmp->killed = 1;
	if (SLEEPING == tmp->state) {
		__remove(tmp);
		tmp->timer = TIMER_IRQ;
		__insert_runnable(PRIORITY_IRQ, tmp);
	}

	__leave_proc_cs 

	return 0;
}

// turn current proc into `RUNNABLE`
// and switch to scheduler 
void yield(void) {
	struct proc *p = myproc();

	__debug_assert("yield", p != NULL, "p == NULL\n");
	__enter_proc_cs 

	__remove(p);
	p->timer = TIMER_NORMAL;
	__insert_runnable(PRIORITY_NORMAL, p);

	// swtch to scheduler 
	sched();

	__leave_proc_cs 
}

// sleep must be atomic, so lk must be held when going into sleep()
void sleep(void *chan, struct spinlock *lk) {
	struct proc *p = myproc();

	__debug_assert("sleep", NULL != p, "p == NULL\n");
	__debug_assert("sleep", NULL != lk, "lk == NULL\n");

	// __debug_info("sleep", "enter\n");
	// either proc_lock or lk must be held at any time, 
	// so that proc would sleep atomically 
	if (&proc_lock != lk) {
		__enter_proc_cs 
		release(lk);
	}

	p->vswtch += 1;
	p->chan = chan;
	__remove(p);	// remove p from runnable 
	__insert_sleep(p);

	sched();
	p->chan = NULL;

	// release proc_lock first to avoid deadlock 
	// with another call to sleep() with the same lk
	__leave_proc_cs 
	acquire(lk);
}

// get the next runnable proc 
// lock must be ACQUIRED before 
static struct proc *__get_runnable_no_lock(void) {
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		struct proc *tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (RUNNABLE == tmp->state) {
				return tmp;
			} else {
				tmp = tmp->next;
			}
		}
	}
	return NULL;
}

struct proc const *get_runnable(void) {
	struct proc *ret;
	__enter_proc_cs 
	ret = __get_runnable_no_lock();
	__leave_proc_cs 
	return ret;
}

void scheduler(void) {
	struct proc *tmp;
	struct cpu *c = mycpu();

	// it should never returns!
	while (1) {
		intr_on();		// make sure interrupts are available 
		__enter_proc_cs 
		tmp = __get_runnable_no_lock();
		if (NULL != tmp) {	// if found one 
			tmp->state = RUNNING;
			c->proc = tmp;

			// switch to user kernel pagetable 
			// will switch to user's pagetable at usertrapret 
			w_satp(MAKE_SATP(tmp->pagetable));
			sfence_vma();
			// swtch context 
			swtch(&c->context, &tmp->context);
			// switch back to kernel pagetable 
			w_satp(MAKE_SATP(kernel_pagetable));
			sfence_vma();
			/*__debug_info("scheduler()", "swtch from %s\n", tmp->name);*/
		}
		c->proc = NULL;
		__leave_proc_cs
	} 

	__debug_error("scheduler", "scheduler returns!\n");
	panic("panic!\n");
}

// sched() is the only way back to scheduler() 
void sched(void) {
	int intena;
	struct proc *p = myproc();

	__debug_assert("sched", NULL != p, "NULL == p\n");
	// __debug_info("sched", "p %s\n", p->name);
	if (!holding(&proc_lock)) 
		panic("sched proc_lock\n");
	if (1 != mycpu()->noff) 
		panic("sched locks\n");
	if (RUNNING == p->state) 
		panic("sched running\n");
	if (intr_get()) 
		panic("sched interruptible\n");

	p->proc_tms.stime += readtime() - p->proc_tms.ikstmp;

	intena = mycpu()->intena;
	swtch(&p->context, &mycpu()->context);
	mycpu()->intena = intena;

	p->proc_tms.ikstmp = readtime();
}

void cpuinit(void) {
	memset(cpus, 0, sizeof(cpus));
}

void procinit(void) {
	// init pid 
	__pid = 1;
	initlock(&pid_lock, "pid_lock");

	// init list 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) 
		proc_runnable[i] = NULL;
	proc_sleep = NULL;
	proc_zombie = NULL;
	initlock(&proc_lock, "proc_lock");
	__debug_info("procinit", "init\n");
}

// a fork child's very first scheduling by scheduler() 
// will swtch to forkret 
void forkret(void) {
	extern int first;

	__leave_proc_cs 
	if (first) {
		// File system initialization must be run in the context of a 
		// regular process (e.g., because it calls sleep), and thus 
		// cannot be run from main() 
		__debug_info("forkret", "hart%d enter here\n", cpuid());
		first = 0;
		rootfs_init();
		myproc()->cwd = namei("/home");
	}

	myproc()->proc_tms.ikstmp = readtime();
	usertrapret();
}

// a user program that calls exec("/init")
// od -t xC initcode
// uchar initcode[] = {
//   0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
//   0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
//   0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
//   0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
//   0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
//   0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00
// };
uchar initcode[] = {
	0x17, 0x05, 0x00, 0x00, 0x03, 0x35, 0x05, 0x05, 0x97, 0x05, 0x00, 0x00, 0x83, 0xB5, 0x05, 0x05,
	0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00, 0x93, 0x08, 0xD0, 0x05, 0x73, 0x00, 0x00, 0x00,
	0xEF, 0xF0, 0x9F, 0xFF, 0x2E, 0x2F, 0x69, 0x6E, 0x69, 0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


int first = 1;
struct proc *__initproc = NULL;
// init the first proc `initcode`
// run at kernel init time, and only hart0 should run this 
void userinit(void) {
	struct proc *p;

	p = allocproc();
	first = 1;
	__initproc = p;

	p->parent = NULL;
	p->sibling_prev = NULL;
	p->sibling_next = NULL;

	// allocate one user page and copy init's instruction 
	// and data into it 
	uvminit(p->pagetable, initcode, sizeof(initcode));
	
	struct seg *s = kmalloc(sizeof(struct seg));
	s->addr = 0;
	s->sz = PGSIZE;
	s->next = NULL;
	s->type = LOAD;
	p->segment = s;

	// prepare for the very first "return" from kernel to user 
	p->trapframe->epc = 0x0;
	p->trapframe->sp = PGSIZE;

	// init signal-related fields
	p->sig_act = NULL;
	memset((void*)&(p->sig_set), 0, sizeof(__sigset_t));
	memset((void*)&(p->sig_pending), 0, sizeof(__sigset_t));

	safestrcpy(p->name, "initcode", sizeof(p->name));

	p->tmask = 0;
	p->elf = NULL;

	__enter_proc_cs 
	__insert_runnable(PRIORITY_NORMAL, p);
	__leave_proc_cs 

	__debug_info("userinit", "init\n");
}

// Grow or shrink user memory by n bytes. 
// Return 0 on success, -1 on failure 
int growproc(uint64 newbrk)
{
	struct proc *p = myproc();
	struct seg *heap = getseg(p->segment, HEAP);

	while (heap && p->pbrk != heap->addr + heap->sz)
		heap = getseg(heap->next, HEAP);

	if (!heap)	
	 	heap = locateseg(p->segment, p->pbrk - 1);
	if (!heap || heap->type != HEAP || newbrk < heap->addr) { // mmap or munmap ruined this
		return -1;
	}

	uint64 boundary = heap->next == NULL ?
						MAXUVA : PGROUNDDOWN(heap->next->addr) - PGSIZE;
	if (newbrk > boundary) {
		return -1;
	}

	if (newbrk < p->pbrk) { // only handle shrinking case, leave growing case for pg-fault
		uvmdealloc(p->pagetable, newbrk, p->pbrk);
		sfence_vma();
	}

	int64 diff = newbrk - p->pbrk;
	heap->sz += diff;
	p->pbrk = newbrk;
	return 0;
}

// Copy to either a user address, or kernel address,
// depending on usr_dst.
// Returns 0 on success, -1 on error.
int
either_copyout(int user_dst, uint64 dst, void *src, uint64 len)
{
  // struct proc *p = myproc();
  if(user_dst){
	// return copyout(p->pagetable, dst, src, len);
	return copyout2(dst, src, len);
  } else {
	memmove((char*)dst, src, len);
	return 0;
  }
}

// Copy from either a user address, or kernel address,
// depending on usr_src.
// Returns 0 on success, -1 on error.
int
either_copyin(void *dst, int user_src, uint64 src, uint64 len)
{
  // struct proc *p = myproc();
  if(user_src){
	// return copyin(p->pagetable, dst, src, len);
	return copyin2(dst, src, len);
  } else {
	memmove(dst, (char*)src, len);
	return 0;
  }
}

static inline char const *__state_to_str(enum procstate state) {
	switch (state) {
	case RUNNABLE: 		return "runnable"; 
	case RUNNING: 		return "running "; 
	case SLEEPING: 		return "sleeping";
	case ZOMBIE: 		return "zombie  ";
	default: 			return "\e[31;1m????????\e[0m";
	}
}

static void __print_list_no_lock(struct proc const *list) {
	while (list) {
		uint64 load = 0, heap = 0;
		for (struct seg const *s = list->segment; s != NULL; s = s->next) {
			if (s->type == LOAD) {
				load += s->sz;
			} else if (s->type == HEAP) {
				heap = s->sz;
			}
		}
		printf("%d\t%d\t%s\t%d\t%s\t%d\t%d\n", 
			list->pid, 
			__initproc == list ? 0 : list->parent->pid, 
			__state_to_str(list->state),
			list->killed, 
			list->name, 
			load, heap
		);
		list = list->next;
	}
}

// print current processes to console 
void procdump(void) {
	printf("epc = %p\n", r_sepc());
    acquire(&pid_lock);
    printf("pid = %d\n", __pid);
    release(&pid_lock);
	printf("\nPID\tPPID\tSTATE\tKILLED\tNAME\tMEM_LOAD\tMEM_HEAP\n");
	__enter_proc_cs 

	// display runnable 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		__print_list_no_lock(proc_runnable[i]);
	}

	// display sleeping 
	__print_list_no_lock(proc_sleep);

	// display zombie 
	__print_list_no_lock(proc_zombie);

	__leave_proc_cs 
}

int procnum(void) {
	int num = 0;
	struct proc const *p;

	__enter_proc_cs 

	// search runnable 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		p = proc_runnable[i];
		while (NULL != p) {
			num ++;
			p = p->next;
		}
	}
	// search sleep 
	p = proc_sleep;
	while (NULL != p) {
		num ++;
		p = p->next;
	}
	// search zombie 
	p = proc_zombie;
	while (NULL != p) {
		num ++;
		p = p->next;
	}

	__leave_proc_cs 

	return num;
}

void reg_info(void) {
	printf("register info: {\n");
	printf("sstatus: %p\n", r_sstatus());
	printf("sip: %p\n", r_sip());
	printf("sie: %p\n", r_sie());
	printf("sepc: %p\n", r_sepc());
	printf("stvec: %p\n", r_stvec());
	printf("satp: %p\n", r_satp());
	printf("scause: %p\n", r_scause());
	printf("stval: %p\n", r_stval());
	printf("sp: %p\n", r_sp());
	printf("tp: %p\n", r_tp());
	printf("ra: %p\n", r_ra());
	printf("}\n");
}
