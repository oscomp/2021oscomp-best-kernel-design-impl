// Process Management 

#ifndef __DEBUG_proc 
#undef DEBUG 
#endif 

#define __module_name__ 	"proc"

#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/spinlock.h"
#include "include/intr.h"
#include "include/pm.h"
#include "include/printf.h"
#include "include/string.h"
#include "include/fs.h"
#include "include/file.h"
#include "include/trap.h"
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
static void proc_list_remove(struct proc *p) {
	__debug_assert("proc_list_remove", NULL != p, "remove NULL from list\n");

	*(p->prev) = p->next;
	p->next->prev = p->prev;
	p->prev = NULL;
	p->next = NULL;
}

#define __insert_runnable(priority, p) do {\
	(p)->state = RUNNABLE; \
	proc_list_insert(&proc_runnable[priority], p); \
} while (0)
#define __insert_sleep(p) do {\
	(p)->state = SLEEPING; \
	proc_list_insert(&proc_sleep, p); \
} while (0)
#define __insert_zombie(p) do {\
	(p)->state = ZOMBIE; \
	proc_list_insert(&proc_zombie, p); \
} while (0)
#define __remove(p) \
	proc_list_remove(p) 

// alloc a proc ready for running 
static struct proc *allocproc(void) {
	struct proc *p = kmalloc(sizeof(struct proc));

	if (NULL == p) {
		__debug_warn("allocproc", "fail to kmalloc() proc\n");
		return NULL;
	}

	// init lock here
	initlock(&(p->lk), "proc");

	p->chan = NULL;
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

	// pagetable 
	p->sz = 0;
	if (NULL == (p->pagetable = proc_pagetable(p)) || 
			NULL == (p->kpagetable = proc_kpagetable()))
	{
		freeproc(p);
		__debug_warn("allocproc", "fail to init pagetable\n");
		return NULL;
	}

	// user kernel stack 
	p->kstack = VKSTACK;

	p->context.ra = (uint64)forkret;
	p->context.sp = p->kstack + PGSIZE;

	// init ofile 
	for (int i = 0; i < NOFILE; i ++) 
		np->ofile = NULL;

	return p;
}

static void freeproc(struct proc *p) {
	// file system fields were freed in exit() 

	// free pagetable 
	if (p->pagetable) 
		proc_freepagetable(p->pagetable, p->sz);
	if (p->kpagetable)
		kvmfree(p->kpagetable, 1);

	// free trapframe 
	freepage(p->trapframe);

	// parenting is handled in exit() 

	// free the proc itself 
	kfree(p);
}

int fork(void) {
	struct proc *p = myproc();
	struct proc *np;

	// allocate proc 
	np = allocproc();
	if (NULL == np) {
		__debug_warn("fork", "fail to allocate new proc\n");
		return -1;
	}

	// copy parent's memory 
	if (uvmcopy(p->pagetable, np->pagetable, np->kpagetable, p->sz) < 0) {
		freeproc(np);
		__debug_warn("fork", "fail to copy parent's memory\n");
		return -1;
	}
	np->sz = p->sz;
	
	// copy parent's trapframe 
	*(np->trapframe) = *(p->trapframe);
	np->trapframe->a0 = 0;

	// parenting 
	// as `p` is the running process, its lock is held in scheduler() 
	acquire(&p->lk);	// enter cs p 
	np->parent = p;
	np->sibling = p->child;
	p->child = np;
	release(&p->lk);	// leave cs p 

	// filesystem 
	for (int i = 0; i < NOFILE; i ++) {
		if (NULL != p->ofile[i]) {
			np->ofile[i] = filedup(p->ofile[i]);
		}
	}
	np->cwd = idup(p->cwd);

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
	iput(p->cwd);

	// write in xstate 
	p->xstate = xstate;

	__enter_proc_cs 

	__remove(p);		// remove p from its list 
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
			__debug_assert("exit", NULL == tmp, "p is not in child list\n");

			tmp->sibling = p->sibling;
		}
		release(&(p->parent->lk));	// leave cs p->parent 
	}

	// re-parent all it's child to `init`
	acquire(&p->lk);	// enter cs p 
	struct proc *tmp = p->child;
	while (NULL != tmp) {
		extern struct proc *__initproc;
		tmp->parent = __initproc;	// re-parent to init 
		tmp = tmp->sibling;
	}
	release(&p->lk);	// leave cs p 

	// p is freed from parent's child list, no long 
	// needs lock for protection 
	wakeup(p->parent);

	// jump into scheduler 
	sched();
	__debug_error("exit", "zombie exit\n");
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
				if (addr && copyout2(addr, (char*)&np->xstate, sizeof(np->xstate))) {
					__leave_proc_cs 
					__debug_error("wait", "fail to copy out xstate\n");
				}
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
			p->timer = TIMER_IRQ;
			__insert_runnable(PRIORITY_IRQ, p);
		}
		p = next;
	}

	__leave_proc_cs 
}

// get the next runnable proc 
// lock must be ACQUIRED before 
static struct proc *__get_runnable_no_lock(void) {
	for (int i = 0; i < PRIORITY_TIMEOUT; i < PRIORITY_NUMBER; i ++) {
		struct proc *tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (RUNNABLE == tmp->state) {
				return tmp;
			}
			tmp = tmp->next;
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

void scheduler(void) __attribute__((noreturn)) {
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
			w_satp(MAKE_SATP(tmp->kpagetable));
			sfence_vma();

			swtch(&c->context, &p->context);

			// switch back to kernel pagetable 
			w_satp(MAKE_SATP(kernel_pagetable));
			sfence_vma();
		}
		c->proc = NULL;
		__leave_proc_cs
	} 

	__debug_error("scheduler", "scheduler returns!\n");
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
		first = 0;
		rootfs_init();
		myproc()->cwd = namei("/");
	}

	usertrapret();
}

// a user program that calls exec("/init")
// od -t xC initcode
uchar initcode[] = {
  0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
  0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
  0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
  0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
  0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
  0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

int first;
struct proc *__initproc;
// init the first proc `initcode`
// run at kernel init time, and only hart0 should run this 
void userinit(void) {
	struct proc *p;

	p = allocproc();
	__initproc = p;

	// allocate one user page and copy init's instruction 
	// and data into it 
	uvminit(p->pagetable, p->kpagetable, initcode, sizeof(initcode));
	p->sz = PGSIZE;

	// prepare for the very first "return" from kernel to user 
	p->trapframe->epc = 0x0;
	p->trapframe->sp = PGSIZE;

	safestrcpy(p->name, "initcode", sizeof(p->name));

	p->tmask = 0;

	__enter_proc_cs 
	__insert_runnable(p);
	__leave_proc_cs 

	__debug_info("userinit", "init\n");
}

// Grow or shrink user memory by n bytes. 
// Return 0 on success, -1 on failure 
int growproc(int n) {
	int sz;
	struct proc *p = myproc();

	sz = p->sz;
	if (n > 0) {
		if (0 == (sz = uvmalloc(p->pagetable, p->kpagetable, sz, sz+n))) 
			return -1;
	}
	else if (n < 0) {
		sz = uvmdealloc(p->pagetable, p->kpagetable, sz, sz + n);
	}
	p->sz = sz;

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

static void __print_list(struct proc *list) {
	while (list) {
		printf("%d\t%s\t%s\t%d\n", 
			list->pid,
			__state_to_str(list->state),
			list->name,
			list->sz
		);
		list = list->next;
	}
}

// print current processes to console 
void procdump(void) {
	print("\nPID\tSTATE\tNAME\tMEM\n");
	__enter_proc_cs 

	// display runnable 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		__print_list(proc_runnable[i]);
	}

	// display sleeping 
	__print_list(proc_sleep);

	// display zombie 
	__print_list(proc_zombie);

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
