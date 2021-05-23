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
#include "include/kmalloc.h"
#include "include/usrmm.h"
#include "include/debug.h"

#include "include/proc.h"

void swtch(struct context*, struct context*);

void forkret(void);

// pid management 
int pid;
struct spinlock pid_lock;
static int allocpid(void) {
	int ret;
	acquire(&pid_lock);
	ret = pid;
	pid = pid + 1;
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
	if (NULL != p->next)
		p->next->prev = p->prev;
	p->prev = NULL;
	p->next = NULL;
}

#define __insert_runnable(priority, p) do {\
	(p)->state = RUNNABLE; \
	proc_list_insert(&(proc_runnable[priority]), p); \
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

static void freeproc(struct proc *p) {
	// file system fields were freed in exit() 

	// free pagetable 
	if (p->pagetable) 
		proc_freepagetable(p->pagetable, p->segment);

	// free trapframe 
	freepage(p->trapframe);

	// parenting is handled in exit() 

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
	__debug_info("proc_freepagetable", "enter\n");
  unmappages(pagetable, TRAPFRAME, 1, 0);
  __debug_info("proc_freepagetable", "leave\n");
  delsegs(pagetable, head);
  __debug_info("proc_freepagetable", "leave 2\n");
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

	// init lock here
	initlock(&(p->lk), "proc");

	p->chan = NULL;
	p->killed = 0;
	p->pid = allocpid();

	p->child = NULL;

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

	// user kernel stack 
	p->kstack = VKSTACK;

	p->context.ra = (uint64)forkret;
	p->context.sp = p->kstack + PGSIZE;

	// init ofile 
	for (int i = 0; i < NOFILE; i ++) 
		p->ofile[i] = NULL;

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
	struct seg *seg;
	struct seg **s2 = &(np->segment);
	for (seg = p->segment; seg != NULL; seg = seg->next) {
		struct seg *s = kmalloc(sizeof(struct seg));
		if (s == NULL) {
			__debug_warn("fork_cow", "seg kmalloc fail\n");
			freeproc(np);
			return -1;
		}
		// Copy user memory from parent to child.
		if (uvmcopy_cow(p->pagetable, np->pagetable, 
				seg->addr, seg->addr + seg->sz, seg->type) < 0) 
		{
			__debug_warn("fork_cow", "uvmcopy_cow fails\n");
			__debug_warn("fork_cow", "p %s\n", p->name);
			__debug_warn("fork_cow", "%p, %p, %d\n", 
					seg->addr, seg->addr + seg->sz, seg->type);
			kfree(s);
			__debug_warn("fork_cow", "exit kfree\n");
			freeproc(np);
			return -1;
		}
		memmove(s, seg, sizeof(struct seg));
		s->next = NULL;
		*s2 = s;
		s2 = &s->next;
	}
	
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

	__debug_info("fork_cow", "leave from fork\n");

	return pid;
}

void exit(int xstate) {
	struct proc *p = myproc();

	extern struct proc *__initproc;
	__debug_assert("exit", __initproc != p, "init exiting\n");

	__debug_info("exit", "p %s\n", p->name);

	// free filesystem fields 
	for (int fd = 0; fd < NOFILE; fd ++) {
		if (NULL != p->ofile[fd]) {
			struct file *f = p->ofile[fd];
			fileclose(f);
		}
	}
	iput(p->cwd);

	// write in xstate 
	acquire(&p->lk);
	p->xstate = xstate;
	release(&p->lk);

	// remove itself from parent's child list 
	// concurrency safety of sibling list is protected by p->parent->lk 
	extern struct proc *__initproc;
	if (__initproc != p->parent) {
		acquire(&(p->parent->lk));	// enter cs p->parent 
		if (p->parent->child == p) {
			p->parent->child = p->sibling;
		}
		else {
			struct proc *tmp = p->parent->child;
			while (tmp && tmp->sibling != p) {
				tmp = tmp->sibling;
			}
			__debug_assert("exit", NULL != tmp, "p is not in child list\n");

			tmp->sibling = p->sibling;
		}
		release(&(p->parent->lk));	// leave cs p->parent 
	}

	// re-parent all it's child to `__initproc`
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
	__enter_proc_cs 

	__remove(p);		// remove p from its list 
	__insert_zombie(p);	// re-insert p into `proc_zombie`

	sched();
	__debug_error("exit", "zombie exit\n");
	panic("panic!\n");
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
					panic("panic!\n");
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
				np = np->next;
			}
		}
		// search sleeping list 
		np = proc_sleep;
		while (NULL != np) {
			if (np->parent == p) {
				has_child = 1;
				goto end_search;
			}
			np = np->next;
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
			struct proc *next = p->next;
			if (RUNNING != p->state) {
				p->timer = p->timer - 1;
				if (0 == p->timer) {	// timeout 
					__remove(p);
					__insert_runnable(PRIORITY_TIMEOUT, p);
				}
			}
			p = next;
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
				//acquire(&tmp->lk);
				tmp->killed = 1;
				//release(&tmp->lk);
				__leave_proc_cs 
				__debug_info("kill", "leave runnable\n");
				return 0;
			}
			else {
				tmp = tmp->next;
			}
		}
	}
	// search sleep 
	tmp = proc_sleep;
	while (NULL != tmp) {
		if (pid == tmp->pid) {
			// found 
			//acquire(&tmp->lk);
			tmp->killed = 1;
			//release(&tmp->lk);
			// wakeup the proc 
			__remove(tmp);
			tmp->timer = TIMER_IRQ;
			__insert_runnable(PRIORITY_IRQ, tmp);

			__leave_proc_cs 
			__debug_info("kill", "leave sleep\n");
			return 0;
		}
		else {
			tmp = tmp->next;
		}
	}

	__leave_proc_cs 
	return -1;
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

void sleep(void *chan, struct spinlock *lk) {
	struct proc *p = myproc();

	__debug_assert("sleep", NULL != p, "p == NULL\n");

	if (NULL != lk) {
		release(lk);
	}
	__enter_proc_cs 

	p->chan = chan;
	__remove(p);	// remove p from runnable 
	__insert_sleep(p);

	sched();
	p->chan = NULL;

	__leave_proc_cs 
	if (NULL != lk) {
		acquire(lk);
	}
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
			__debug_info("scheduler()", "swtch from %s\n", tmp->name);
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
	__debug_info("sched", "p %s\n", p->name);
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
	pid = 1;
	initlock(&pid_lock, "pid_lock");

	// init cpus 
	memset(cpus, 0, sizeof(cpus));

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

int first = 1;
struct proc *__initproc = NULL;
// init the first proc `initcode`
// run at kernel init time, and only hart0 should run this 
void userinit(void) {
	struct proc *p;

	p = allocproc();
	first = 1;
	__initproc = p;

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

	safestrcpy(p->name, "initcode", sizeof(p->name));

	p->tmask = 0;

	__enter_proc_cs 
	__insert_runnable(PRIORITY_NORMAL, p);
	__leave_proc_cs 

	__debug_info("userinit", "init\n");
}

// Grow or shrink user memory by n bytes. 
// Return 0 on success, -1 on failure 
int growproc(int n) {
  struct proc *p = myproc();

  struct seg *heap = getseg(p->segment, HEAP);
  struct seg *stack = heap->next;

  uint64 oldva = heap->addr + heap->sz;
  uint64 newva = oldva + n;

  if(n > 0){
    if (newva > stack->addr - stack->sz ||
        uvmalloc(p->pagetable, oldva, newva, PTE_W|PTE_R) == 0) {
      return -1;
    }
  } else if(n < 0){
    if (newva < heap->addr || newva > oldva) {
      newva = heap->addr;
    }

    uvmdealloc(p->pagetable, newva, oldva, HEAP);
  }
  heap->sz += n;
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
		uint64 load = 0, heap = 0;
		for (struct seg *s = list->segment; s != NULL; s = s->next) {
			if (s->type == LOAD) {
				load += s->sz;
			} else if (s->type == HEAP) {
				heap = s->sz;
			}
		}
		printf("%d\t%s\t%d\t%s\t%d\t%d\n", 
			list->pid, 
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
	printf("\nPID\tSTATE\tKILLED\tNAME\tMEM_LOAD\tMEM_HEAP\n");
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
