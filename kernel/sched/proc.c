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
#include "sbi.h"
#include "mm/pm.h"
#include "printf.h"
#include "utils/string.h"
#include "fs/fs.h"
#include "fs/file.h"
#include "trap.h"
#include "mm/vm.h"
#include "mm/kmalloc.h"
#include "mm/usrmm.h"
#include "sched/signal.h"
#include "errno.h"
#include "sched/signal.h"
#include "sched/proc.h"

#include "utils/debug.h"

uint64 swtch(struct context *old, struct context *new);
void sched(void);

void forkret(uint64 kstack);

int __pid;
#define HASH_SIZE 	17
#define __hash(pid) 	((pid) % HASH_SIZE)
struct proc *pid_hash[HASH_SIZE];
struct spinlock hash_lock;

#define __enter_hash_cs \
	acquire(&hash_lock);
#define __leave_hash_cs \
	release(&hash_lock);

// Always insert new proc to the front of hash list 
// so procs on list is in descending order. 
static void hash_insert_no_lock(struct proc *p) {
	__debug_assert("hash_insert", NULL != p, 
			"p is NULL\n");
	
	int key = __hash(p->pid);
	p->hash_next = pid_hash[key];
	p->hash_pprev = &(pid_hash[key]);
	if (NULL != pid_hash[key]) {
		pid_hash[key]->hash_pprev = &(p->hash_next);
	}
	pid_hash[key] = p;
}

static void hash_remove_no_lock(struct proc *p) {
	__debug_assert("hash_remove", NULL != p, 
			"p is NULL\n");

	*(p->hash_pprev) = p->hash_next;
	if (NULL != p->hash_next) {
		p->hash_next->hash_pprev = p->hash_pprev;
	}
	p->hash_pprev = NULL;
	p->hash_next = NULL;
}

static struct proc *hash_search_no_lock(int pid) {
	__debug_assert("hash_search", pid >= 1, "pid %d too small\n", pid);


	int key = __hash(pid);
	struct proc *tmp = pid_hash[key];
	while (tmp && tmp->pid != pid) {
		if (tmp->pid < pid) {
			return NULL;
		}
		else {
			tmp = tmp->hash_next;
		}
	}

	return tmp;
}

struct cpu cpus[NCPU];
void cpuinit(void) {
	memset(cpus, 0, sizeof(cpus));
}
struct cpu *mycpu(void) {
	int id = cpuid();
	return &cpus[id];
}
struct proc *myproc(void) {
	push_off();
	struct cpu *c = mycpu();
	struct proc *p = c->proc;
	pop_off();
	return p;
}

extern char trampoline[];		// trampoline.S

// Create a user page table for a given process,
// with no user memory, but with trampoline pages.
static pagetable_t
proc_pagetable(struct proc *p)
{
	pagetable_t pagetable;

	// An empty page table.
	pagetable = kvmcreate();
	if(pagetable == 0)
		return NULL;

	return pagetable;
}

// Free a process's page table, and free the
// physical memory it refers to.
static void
proc_freepagetable(pagetable_t pagetable, struct seg *head)
{
	delsegs(pagetable, head);
	uvmfree(pagetable);
	kvmfree(pagetable, 1);
}

// Free most components of proc, excepting kernel stack, 
// kernel mapping of pagetable and proc itself. 
static void freeproc(struct proc *p) {
	// file system fields were freed in exit() 

	// free pagetable 
	proc_freepagetable(p->pagetable, p->segment);

	// free trapframe 
	kfree(p->trapframe);

	freepage(p->kstack);

	// parenting is handled in exit() and wait4()

	// free signal 
	sigaction_free(p->sig_act);

	// free the list of sig_frame 
	sigframefree(p->sig_frame);

	// remove this proc from pid_hash
	__enter_hash_cs
	hash_remove_no_lock(p);
	__leave_hash_cs

	kfree(p);
}

static struct proc *allocproc(void) {
	struct proc *p = kmalloc(sizeof(struct proc));
	if (NULL == p) {
		__debug_warn("allocproc", "fail to alloc\n");
		return NULL;
	}

	// times for process performance 
	p->proc_tms.utime = 0;
	p->proc_tms.stime = 0;
	p->proc_tms.cutime = 0;
	p->proc_tms.cstime = 0;
	p->vswtch = p->ivswtch = 0;

	// parenting 
	initlock(&p->lk, "p->lk");
	p->child = NULL;

	// memory 
	p->kstack = (uint64)allocpage();
	if (NULL == p->kstack) {
		__debug_warn("allocproc", "fail to set kstack\n");
		kfree(p);
		return NULL;
	}
	p->trapframe = kmalloc(sizeof(struct trapframe));
	if (NULL == p->trapframe) {
		__debug_warn("allocproc", "fail to set trapframe\n");
		freepage((void*)(p->kstack));
		kfree(p);
		return NULL;
	}
	p->segment = NULL;
	if (NULL == (p->pagetable = proc_pagetable(p))) {
		__debug_warn("allocproc", "fail to set pagetable\n");
		kfree((void*)p->trapframe);
		freepage((void*)p->kstack);
		kfree(p);
		return NULL;
	}

	// scheduling 
	p->chan = NULL;
	p->sched_next = NULL;
	p->sched_pprev = NULL;
	p->context.ra = (uint64)forkret;
	p->context.sp = p->kstack + PGSIZE;

	// signal 
	p->sig_act = NULL;
	p->sig_frame = NULL;
	for (int i = 0; i < SIGSET_LEN; i ++) {
		p->sig_pending.__val[i] = 0;
	}
	p->killed = 0;

	// file system 
	memset(&p->fds, 0, sizeof(p->fds));
	p->cwd = p->elf = NULL;

	// debug information left for initialization 

	// pid management 
	__enter_hash_cs
	p->pid = __pid ++;
	hash_insert_no_lock(p);
	__leave_hash_cs

	return p;
}


// lists for scheduling 
#define TIMER_IRQ				5
#define TIMER_NORMAL			10
#define PRIORITY_TIMEOUT		0
#define PRIORITY_IRQ			1
#define PRIORITY_NORMAL			2
#define PRIORITY_NUMBER 		3
struct proc *proc_runnable[PRIORITY_NUMBER];
struct proc *proc_sleep;
struct spinlock proc_lock;

// NOTICE! To avoid any potential deadlock with proc_lock, 
// proc_lock should be acquired last with any situation requiring 
// multiple spinlocks. 
#define __enter_proc_cs \
	acquire(&proc_lock);
#define __leave_proc_cs \
	release(&proc_lock);

static void __proc_list_insert_no_lock(struct proc **head, struct proc *p) {
	__debug_assert("proc_list_insert", NULL != p, "insert NULL into list\n");

	struct proc *tmp = *head;
	while (NULL != tmp) {
		head = &(tmp->sched_next);
		tmp = tmp->sched_next;
	}
	*head = p;
	p->sched_pprev = head;
	p->sched_next = NULL;
}
static void __proc_list_remove_no_lock(struct proc *p) {
	__debug_assert("proc_list_remove", NULL != p, "remove NULL from list\n");

	*(p->sched_pprev) = p->sched_next;
	if (NULL != p->sched_next)
		p->sched_next->sched_pprev = p->sched_pprev;
	p->sched_pprev = NULL;
	p->sched_next = NULL;
}

#define __insert_runnable(priority, p) do {\
	(p)->state = RUNNABLE; \
	__proc_list_insert_no_lock(&(proc_runnable[priority]), p); \
} while (0)
#define __insert_sleep(p) do {\
	(p)->state = SLEEPING; \
	__proc_list_insert_no_lock(&proc_sleep, p); \
} while (0)
#define __remove(p) \
	__proc_list_remove_no_lock(p) 


int clone(uint64 flag, uint64 stack) {
	struct proc *p = myproc();
	struct proc *np;

	__debug_info("clone", "%d enter\n", p->pid);

	np = allocproc();
	if (NULL == np) {
		__debug_warn("fork", "fail to alloc\n");
		return -1;
	}

	// copy parent's memory layout 
	np->segment = copysegs(p->pagetable, p->segment, np->pagetable);
	if (NULL == np->segment) {
		__debug_warn("clone", "fail to copy segments\n");
		freeproc(np);
		return -1;
	}
	np->pbrk = p->pbrk;

	// copy parent's signal fields 
	if (0 != sigaction_copy(&np->sig_act, p->sig_act)) {
		__debug_warn("clone", "fail to copy sigaction\n");
		freeproc(np);
		return -1;
	}
	for (int i = 0; i < SIGSET_LEN; i ++) {
		np->sig_set.__val[i] = p->sig_set.__val[i];
	}

	// these parts may be improved later 
	// filesystem 
	if (copyfdtable(&p->fds, &np->fds) < 0) {
		__debug_warn("clone", "fail to copy fdt\n");
		freeproc(np);
		return -1;
	}
	np->cwd = idup(p->cwd);
	np->elf = idup(p->elf);

	// copy parent's trapframe 
	if (r_sstatus_fs() == SSTATUS_FS_DIRTY) {
		floatstore(p->trapframe);
		w_sstatus_fs(SSTATUS_FS_CLEAN);
	}
	*(np->trapframe) = *(p->trapframe);
	np->trapframe->a0 = 0;

	if (NULL != stack) {
		np->trapframe->sp = stack;
	}

	// parenting 
	acquire(&p->lk);
	np->parent = p;
	np->sibling_pprev = &(p->child);
	np->sibling_next = p->child;
	if (NULL != p->child) {
		p->child->sibling_pprev = &(np->sibling_next);
	}
	p->child = np;
	release(&p->lk);

	// copy debug info 
	safestrcpy(np->name, p->name, sizeof(p->name));
	np->tmask = p->tmask;

	int pid = np->pid;

	__debug_info("clone", "hart %d: %d -> %d\n", cpuid(), p->pid, np->pid);
	__debug_info("clone", "hart %d new proc name %s\n", cpuid(), np->name);

	__enter_proc_cs 
	np->timer = TIMER_NORMAL;
	__insert_runnable(PRIORITY_NORMAL, np);
	__leave_proc_cs 

	return pid;
}

// search the hash map to wake up procs 
static void __wakeup_no_lock(void *chan) {
	struct proc *p = proc_sleep;
	while (NULL != p) {
		struct proc *next = p->sched_next;
		if ((uint64)chan == (uint64)p->chan) {
			__debug_info("__wakeup_no_lock", "wakeup %d\n", p->pid);
			__remove(p);
			p->timer = TIMER_IRQ;
			p->chan = NULL;
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

	__debug_info("exit", "hart %d pid %d\n", cpuid(), p->pid);
	delsegs(p->pagetable, p->segment);
	p->segment = NULL;
	uvmfree(p->pagetable);

	// close all open files 
	dropfdtable(&p->fds);
	iput(p->cwd);
	iput(p->elf);

	p->xstate = xstate;

	// re-parent all its children to `__initproc`
	acquire(&p->lk);
	if (NULL != p->child) {
		struct proc *first, *last;
		first = last = p->child;

		while (NULL != last->sibling_next) {
			last->parent = __initproc;
			last = last->sibling_next;
		}
		last->parent = __initproc;

		acquire(&__initproc->lk);
		first->sibling_pprev = &(__initproc->child);
		last->sibling_next = __initproc->child;
		if (NULL != __initproc->child) {
			__initproc->child->sibling_pprev = &(last->sibling_next);
		}
		__initproc->child = first;
		release(&__initproc->lk);
	}
	release(&p->lk);

	__debug_info("exit", "%d exit %d\n", p->pid, xstate);
	p->parent->sig_pending.__val[0] |= 1ul << SIGCHLD;
	if (0 == p->parent->killed || SIGCHLD < p->parent->killed) {
		p->parent->killed = SIGCHLD;
	}
	// by holding parent's lock, parent cannot re-parent child
	acquire(&p->parent->lk);

	// acquire proc_lock after parent's lock, to avoid deadlock 
	// with parent calling sleep(p, &p->lk) in wait4(). 
	__enter_proc_cs

	p->state = ZOMBIE;
	__remove(p); 
	__wakeup_no_lock(__initproc);
	__wakeup_no_lock(p->parent);

	// As we don't release p->parent->lk, p->parent is not able 
	// to start searching for zombie child. 
	// As there's no "kernel stack" and kernel pagetable after booting, 
	// parent can free child's proc block only after child has switched out 
	// so release parent's lock in scheduler. 

	sched();

	panic("panic! living dead!\n");
}

#define WAIT_OPTIONS_WNOHANG 		1
#define WAIT_OPTIONS_WUNTRACED 		2
#define WAIT_OPTIONS_WCONTINUED 	8
int wait4(int pid, uint64 status, uint64 options) {
	struct proc *np;
	struct proc *p = myproc();

	__debug_info("wait4", "hart %d pid %d\n", cpuid(), p->pid);

	acquire(&p->lk);	// lock must be held to avoid missing wakeup from child's exit()
	while (1) {
		np = p->child;
		while (NULL != np) {
			if (ZOMBIE == np->state && (-1 == pid || pid == np->pid)) {
				// the child is ready to free
				__debug_info("wait4", "%d buries %d\n", p->pid, np->pid);

				int child_pid = np->pid;
				// remove the child from child list 
				*(np->sibling_pprev) = np->sibling_next;
				if (NULL != np->sibling_next) {
					np->sibling_next->sibling_pprev = np->sibling_pprev;
				}

				// it's okay to remove lk now, as we find one proc to free 
				release(&p->lk);

				// add child's tms time 
				p->proc_tms.cstime += np->proc_tms.stime + np->proc_tms.cstime;
				p->proc_tms.cutime += np->proc_tms.utime + np->proc_tms.cutime;

				// copyout child's xstate 
				np->xstate <<= 8;
				if (status && copyout2(status, (char*)&(np->xstate), sizeof(np->xstate))) {
					__debug_error("wait4", "fail to copy out xstate\n");
					panic("panic!\n");
				}

				freeproc(np);
				return child_pid;
			}
			else {
				np = np->sibling_next;
			}
		}

		// if not found, maybe child proc is still runnable or sleeping?
		if (NULL != p->child) {
			if (options & WAIT_OPTIONS_WNOHANG) {
				release(&p->lk);
				return 0;
			}
			else {
				sleep(p, &p->lk);
			}
		}
		else {
			release(&p->lk);
			return -1;
		}
	}
}


// Well, isn'kill a function for signal? We can't place 
// it in sched/signal.c because we need to search for the target 
// proc with pid, that requires hash_lock. 
int kill(int pid, int sig) {
	struct proc *tmp;

	__enter_hash_cs 
	// search for the wanted proc 
	tmp = hash_search_no_lock(pid);
	if (NULL == tmp) {	// if not found 
		__leave_hash_cs 
		return -ESRCH;
	}

	// we must hold hash_lock here, in case tmp is freed by its parent
	// I mean calling freeproc 

	__debug_info("kill", "%d send sig %d to %d\n", myproc()->pid, sig, pid);

	int const len = sizeof(unsigned long) * 8;
	int bit = sig % len;
	int i = sig / len;

	__assert("kill", i < SIGSET_LEN, "signal too large %d\n", sig);
	tmp->sig_pending.__val[i] |= 1ul << bit;
	if (0 == tmp->killed || sig < tmp->killed) {
		tmp->killed = sig;
	}

	// printf("pid %d pending_num = %d\n", pid, tmp->killed);

	__enter_proc_cs 
	if (SLEEPING == tmp->state) {
		__remove(tmp);
		tmp->timer = TIMER_IRQ;
		tmp->chan = NULL;
		__insert_runnable(PRIORITY_IRQ, tmp);
	}
	__leave_proc_cs 

	__leave_hash_cs 
	return 0;
}

void sleep(void *chan, struct spinlock *lk) {
	struct proc *p = myproc();

	__debug_assert("sleep", NULL != p, "p == NULL\n");
	__debug_assert("sleep", NULL != lk, "lk == NULL\n");

	// Either proc_lock or lk must be held, 
	// so that proc would sleep atomically
	if (&proc_lock != lk) {
		__enter_proc_cs 
		release(lk);
	}

	p->vswtch += 1;
	p->chan = chan;
	__remove(p);		// remove p from runnable 
	__insert_sleep(p);

	sched();
	p->chan = NULL;

	// release proc_lock first to avoid deadlock
	// in case another call to sleep() with the same lk 
	__leave_proc_cs 
	acquire(lk);
}

static struct proc *__get_runnable_no_lock(void) {
	struct proc const *tmp;

	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (RUNNABLE == tmp->state) {
				__debug_info("__get_runnable_no_lock", 
						"hart %d get runnable %d\n", 
						cpuid(), tmp->pid);
				return (struct proc*)tmp;
			}
			tmp = tmp->sched_next;
		}
	}

	return NULL;
}

// hang up current proc 
int yield(void) {
	struct proc *p = myproc();

	__debug_assert("yield", p != NULL, "p == NULL\n");

	__enter_proc_cs 
	if (NULL == __get_runnable_no_lock()) {
		// if there's no proc to run, yielding may not a good idea 
		__leave_proc_cs 
		return 0;
	}

	__remove(p);
	p->timer = TIMER_NORMAL;
	__insert_runnable(PRIORITY_NORMAL, p);

	// run into scheduler to choose next proc for running 
	sched();
	__leave_proc_cs 

	return 1;
}


// a fork child's very first sheduling by scheduler() will swtch to forkret 
void forkret(uint64 kstack) {
	extern int fs_first;

	__leave_proc_cs 

	w_sstatus_fs(SSTATUS_FS_CLEAN);

	if (fs_first) {
		fs_first = 0;
		rootfs_init();
		myproc()->cwd = namei("/");
	}

	myproc()->ikstmp = readtime();
	usertrapret();
}

void scheduler(void) {
	struct proc *tmp;
	struct cpu *c = mycpu();

	// it should never returns!
	while (1) {
		int found = 0;
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

			// if tmp is zombie, we need to release the lock of it's parent 
			if (ZOMBIE == tmp->state) {
				release(&(tmp->parent->lk));
			}
			found = 1;
		}
		c->proc = NULL;
		__leave_proc_cs
		if (!found) {
			intr_on();
			asm volatile("wfi");
		}
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
	if (1 != mycpu()->noff && !(ZOMBIE == p->state && 2 == mycpu()->noff)) 
		panic("sched locks\n");
	if (RUNNING == p->state) 
		panic("sched running\n");
	if (intr_get()) 
		panic("sched interruptible\n");

	// save floating-point registers
	if (r_sstatus_fs() == SSTATUS_FS_DIRTY) {
		// printf(__INFO("sched")"floatstore, pid=%d, name=%s\n", p->pid, p->name);
		floatstore(p->trapframe);
		w_sstatus_fs(SSTATUS_FS_CLEAN);
	}
	
	p->proc_tms.stime += readtime() - p->ikstmp;

	intena = mycpu()->intena;
	swtch(&p->context, &mycpu()->context);
	mycpu()->intena = intena;

	p->ikstmp = readtime();

	// On k210, sstatus.fs seems to be hardwired to dirty(11b).
	// if (r_sstatus_fs() == SSTATUS_FS_DIRTY)
	// 	panic("sched out with dirty floating-point registers");

	floatload(p->trapframe);
	w_sstatus_fs(SSTATUS_FS_CLEAN);
}


void proc_tick(void) {
	__enter_proc_cs 

	// runnable 
	struct proc *p;
	for (int i = PRIORITY_IRQ; i < PRIORITY_NUMBER; i ++) {
		p = proc_runnable[i];
		while (NULL != p) {
			struct proc *next = p->sched_next;
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

	// sleep 
	uint64 now = readtime();
	p = proc_sleep;
	while (NULL != p) {
		struct proc *next = p->sched_next;
		if (p->sleep_expire && now >= p->sleep_expire) {
			p->sleep_expire = 0;
			__remove(p);
			__insert_runnable(PRIORITY_TIMEOUT, p);
		}
		p = next;
	}

	__leave_proc_cs
}


// Grow or shrink user memory by n bytes. 
// Return 0 on success, -1 on failure. 
int growproc(uint64 newbrk) {
	struct proc *p = myproc();
	struct seg *heap = getseg(p->segment, HEAP);

	while (heap && p->pbrk != heap->addr + heap->sz) {
		heap = getseg(heap->next, HEAP);
	}

	if (!heap) {
		heap = locateseg(p->segment, p->pbrk - 1);
	}
	if (!heap || HEAP != heap->type || newbrk < heap->addr) {	// mmap or munmap ruined this 
		return -1;
	}

	uint64 boundary = NULL == heap->next ? 
			MAXUVA : PGROUNDDOWN(heap->next->addr) - PGSIZE;
	if (newbrk > boundary) {	// the newbrk outbreach the boundary
		return -1;
	}

	// to shrink
	if (newbrk < p->pbrk) {
		uvmdealloc(p->pagetable, newbrk, p->pbrk);
		sfence_vma();
	}

	// update heap size and pbrk
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


/* Functions for Debug */
static inline char const *__state_to_str(enum procstate state) {
	switch (state) {
	case RUNNABLE: 		return "runnable";
	case RUNNING: 		return "running";
	case SLEEPING: 		return "sleeping";
	case ZOMBIE: 		return "zombie";
	default: 			return "\e[31;1m???\e[0m";
	}
}

static void __print_proc_no_lock(struct proc *list) {
	extern struct proc *__initproc;

	while (list) {
		// calculate proc's load and heap memory size 
		uint64 load = 0, heap = 0;
		for (struct seg const *s = list->segment; NULL != s; s = s->next) {
			if (LOAD == s->type) {
				load += s->sz;
			}
			else if (HEAP == s->type) {
				heap = s->sz;
			}
		}

		printf("%d\t%d\t%s\t%d\t%s\t%d\t%d\t%p\n", 
			list->pid, 
			__initproc == list ? 0 : list->parent->pid, 
			__state_to_str(list->state),
			list->killed, 
			list->name, 
			load, heap, 
			list->chan
		);
		list = list->hash_next;
	}
}

// print all processes to console, for debugging 
void procdump(void) {
	printf("\nepc = %p\n", r_sepc());

	__enter_hash_cs
	printf("next pid = %d\n", __pid);
	printf("\nPID\tPPID\tSTATE\tKILLED\tNAME\tMEM_LOAD\tMEM_HEAP\n");
	for (int i = 0; i < HASH_SIZE; i ++) {
		__print_proc_no_lock(pid_hash[i]);
	}

	__leave_hash_cs
}

// return total number of proc 
int procnum(void) {
	int num = 0;
	struct proc const *tmp;

	__enter_hash_cs
	for (int i = 0; i < HASH_SIZE; i ++) {
		tmp = pid_hash[i];
		while (NULL != tmp) {
			num ++;
			tmp = tmp->hash_next;
		}
	}
	__leave_hash_cs

	return num;
}


uchar initcode[] = {
	0x17, 0x05, 0x00, 0x00, 0x03, 0x35, 0x05, 0x05, 0x97, 0x05, 0x00, 0x00, 0x83, 0xB5, 0x05, 0x05,
	0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00, 0x93, 0x08, 0xD0, 0x05, 0x73, 0x00, 0x00, 0x00,
	0xEF, 0xF0, 0x9F, 0xFF, 0x2E, 0x2F, 0x69, 0x6E, 0x69, 0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// initiailization 
int fs_first;
struct proc *__initproc = NULL;

// init first proc `__initproc`
void userinit(void) {
	fs_first = 1;

	struct proc *p = allocproc();
	__initproc = p;

	p->parent = NULL;
	p->sibling_pprev = NULL;
	p->sibling_next = NULL;

	// allocate one user page and copy initcode's instruction 
	// and data into it 
	uvminit(p->pagetable, initcode, sizeof(initcode));

	struct seg *s = kmalloc(sizeof(struct seg));
	s->addr = 0;
	s->sz = PGSIZE;
	s->next = NULL;
	s->type = LOAD;
	p->segment = s;

	// prepare for the very first "return from kernel"
	p->trapframe->epc = 0x0;
	p->trapframe->sp = VUSTACK;

	safestrcpy(p->name, "initcode", sizeof(p->name));

	p->tmask = 0;
	p->elf = NULL;

	for (int i = 0; i < SIGSET_LEN; i ++) {
		p->sig_set.__val[i] = ~0;
	}

	__enter_proc_cs 
	__insert_runnable(PRIORITY_NORMAL, p);
	__leave_proc_cs 

	__debug_info("userinit", "init\n");
}

void procinit(void) {
	// init pid_hash 
	__pid = 1;
	for (int i = 0; i < HASH_SIZE; i ++) {
		pid_hash[i] = NULL;
	}
	initlock(&hash_lock, "hash_lock");

	// init list 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		proc_runnable[i] = NULL;
	}
	proc_sleep = NULL;
	initlock(&proc_lock, "proc_lock");

	__debug_info("procinit", "init\n");
}