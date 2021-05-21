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
#include "include/usrmm.h"
#include "include/kmalloc.h"
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
		proc_freepagetable(p->pagetable, p->sz);
	if (p->kpagetable)
		kvmfree(p->kpagetable, 1);

	// free trapframe 
	freepage(p->trapframe);

	// parenting is handled in exit() 

	// free the proc itself 
	kfree(p);
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

	p->killed = 0;
	p->pid = allocpid();

	p->timer = 0;
	p->chan = NULL;

	// init parenting 
	p->child = NULL;

	// init memory 
	p->kstack = VKSTACK;
	p->sz = 0;
	// An empty user pagetable 
	// and an identical kernel pagetable for this proc 
	if ((p->pagetable = proc_pagetable(p)) == NULL) {
		freeproc(p);
		return NULL;
	}

	// allocate a page trapframe 
	p->trapframe = (struct trapframe*)allocpage();
	if (NULL == p->trapframe) {
		__debug_warn("allocproc", "fail to alloc trapframe\n");
		kfree(p);
		return NULL;
	}
	p->segment = NULL;

	// init ofile 
	for (int i = 0; i < NOFILE; i ++) 
		p->ofile[i] = NULL;
	p->cwd = NULL;

	return p;
}

// free a proc structure and the data hanging from it,
// including user pages.
// p->lock must be held.
static void freeproc(struct proc *p) {
	if(p->trapframe)
		freepage((void*)p->trapframe);
	p->trapframe = 0;
	if(p->pagetable)
		proc_freepagetable(p->pagetable, p->segment);
	p->pagetable = 0;
	p->segment = 0;
	p->pid = 0;
	p->parent = 0;
	p->name[0] = 0;
	p->chan = 0;
	p->killed = 0;
	p->xstate = 0;
	p->state = UNUSED;
}

int fork(void) {
	struct proc *p = myproc();
	struct proc *np;

<<<<<<< HEAD
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
=======
  // An empty page table.
  pagetable = kvmcreate();
  if(pagetable == 0)
    return NULL;

  // map the trampoline code (for system call return)
  // at the highest user virtual address.
  // only the supervisor uses it, on the way
  // to/from user space, so not PTE_U.
  // if(mappages(pagetable, TRAMPOLINE, PGSIZE,
  //             (uint64)trampoline, PTE_R | PTE_X, 0) < 0){
  //   uvmfree(pagetable, 0);
  //   return NULL;
  // }

  // map the trapframe just below TRAMPOLINE, for trampoline.S.
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
            (uint64)(p->trapframe), PTE_R | PTE_W) < 0){
    kvmfree(pagetable, 1);
    return NULL;
  }
>>>>>>> usrmm

	int pid = np->pid;

<<<<<<< HEAD
	__enter_proc_cs 
	// init timer 
	np->timer = TIMER_NORMAL;
	// set runnable 
	__insert_runnable(PRIORITY_NORMAL, np);
	__leave_proc_cs 

	return pid;
}
=======
// Free a process's page table, and free the
// physical memory it refers to.
void
proc_freepagetable(pagetable_t pagetable, struct seg *head)
{
  // unmappages(pagetable, TRAMPOLINE, 1, 0, 0);
  unmappages(pagetable, TRAPFRAME, 1, 0);
  delsegs(pagetable, head);
  uvmfree(pagetable);
  kvmfree(pagetable, 1);
}

// pagetable_t proc_kpagetable(struct proc *p)
// {
//   pagetable_t pagetable;

//   if((pagetable = kvmcreate()) == NULL)
//     return NULL;

//   if(mappages(pagetable, TRAPFRAME, PGSIZE,
//             (uint64)(p->trapframe), PTE_R | PTE_W, 0) < 0){
//     kvmfree(pagetable, 1);
//     return NULL;
//   }

//   return pagetable;
// }

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
>>>>>>> usrmm

void exit(int xstate) {
	struct proc *p = myproc();

<<<<<<< HEAD
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
=======
  p = allocproc();
  initproc = p;
  
  // allocate one user page and copy init's instructions
  // and data into it.
  uvminit(p->pagetable, initcode, sizeof(initcode));
  struct seg *s = kmalloc(sizeof(struct seg));
  s->addr = 0;
  s->sz = PGSIZE;
  s->next = NULL;
  s->type = LOAD;
  p->segment = s;
>>>>>>> usrmm

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

<<<<<<< HEAD
int kill(int pid) {
	struct proc *tmp;

	__enter_proc_cs 

	// search runnable 
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (pid == tmp->pid) {
				// found 
				acquire(&tmp->lk);
				tmp->killed = 1;
				release(&tmp->lk);
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
			acquire(&tmp->lk);
			tmp->killed = 1;
			release(&tmp->lk);
			// wakeup the proc 
			tmp->state = RUNNABLE;
			__remove(tmp);
			__insert_runnable(PRIORITY_NORMAL, tmp);

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
=======
// Grow or shrink user memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
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

// Create a new process, copying the parent.
// Sets up child kernel stack to return as if from fork() system call.
// int
// fork(void)
// {
//   int i, pid;
//   struct proc *np;
//   struct proc *p = myproc();

//   // Allocate process.
//   if((np = allocproc()) == NULL){
//     return -1;
//   }

//   // Copy user memory from parent to child.
//   if(uvmcopy(p->pagetable, np->pagetable, np->kpagetable, p->sz) < 0){
//     freeproc(np);
//     release(&np->lock);
//     return -1;
//   }
//   np->sz = p->sz;

//   np->parent = p;

//   // copy tracing mask from parent.
//   np->tmask = p->tmask;

//   // copy saved user registers.
//   *(np->trapframe) = *(p->trapframe);

//   // Cause fork to return 0 in the child.
//   np->trapframe->a0 = 0;

//   // increment reference counts on open file descriptors.
//   for(i = 0; i < NOFILE; i++)
//     if(p->ofile[i])
//       np->ofile[i] = filedup(p->ofile[i]);
//   np->cwd = edup(p->cwd);

//   safestrcpy(np->name, p->name, sizeof(p->name));

//   pid = np->pid;

//   np->state = RUNNABLE;

//   release(&np->lock);

//   return pid;
// }

int fork_cow(void)
{
  int i, pid;
  struct proc *np;
  struct proc *p = myproc();
>>>>>>> usrmm

	__enter_proc_cs 

<<<<<<< HEAD
	p->timer = TIMER_NORMAL;
	__remove(p);
	__insert_runnable(PRIORITY_NORMAL, p);
=======
  struct seg *seg;
  struct seg **s2 = &np->segment;
  for (seg = p->segment; seg != NULL; seg = seg->next) {
    struct seg *s = kmalloc(sizeof(struct seg));
    if (s == NULL) {
      goto fail;
    }
    // Copy user memory from parent to child.
    // __debug_info("fork_cow", "type=%d start=%p end=%p\n", seg->type, start, end);
    if (uvmcopy_cow(p->pagetable, np->pagetable, seg->addr, seg->addr + seg->sz, seg->type) < 0) {
      kfree(s);
      goto fail;
    }
    memmove(s, seg, sizeof(struct seg));
    s->next = NULL;
    *s2 = s;
    s2 = &s->next;
  }
>>>>>>> usrmm

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

<<<<<<< HEAD
// get the next runnable proc 
// lock must be ACQUIRED before 
static struct proc *__get_runnable_no_lock(void) {
	for (int i = 0; i < PRIORITY_NUMBER; i ++) {
		struct proc *tmp = proc_runnable[i];
		while (NULL != tmp) {
			if (RUNNABLE == tmp->state) {
				return tmp;
			}
			tmp = tmp->next;
		}
	}
	return NULL;
=======
  return pid;

fail:
  freeproc(np);
  release(&np->lock);
  return -1;
>>>>>>> usrmm
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
			w_satp(MAKE_SATP(tmp->kpagetable));
			sfence_vma();

			swtch(&c->context, &tmp->context);

			// switch back to kernel pagetable 
			w_satp(MAKE_SATP(kernel_pagetable));
			sfence_vma();
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
	uvminit(p->pagetable, p->kpagetable, initcode, sizeof(initcode));
	p->sz = PGSIZE;

<<<<<<< HEAD
	// prepare for the very first "return" from kernel to user 
	p->trapframe->epc = 0x0;
	p->trapframe->sp = PGSIZE;
=======
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  extern pagetable_t kernel_pagetable;

  c->proc = 0;
  for(;;){
    // Avoid deadlock by ensuring that devices can interrupt.
    intr_on();
    
    int found = 0;
    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE) {
        // Switch to chosen process.  It is the process's job
        // to release its lock and then reacquire it
        // before jumping back to us.
        // printf("[scheduler]found runnable proc with pid: %d\n", p->pid);
        p->state = RUNNING;
        c->proc = p;
        w_satp(MAKE_SATP(p->pagetable));
        sfence_vma();
        swtch(&c->context, &p->context);
        w_satp(MAKE_SATP(kernel_pagetable));
        sfence_vma();
        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;

        found = 1;
      }
      release(&p->lock);
    }
    if(found == 0) {
      intr_on();
      asm volatile("wfi");
    }
  }
}
>>>>>>> usrmm

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

extern char trampoline[];	// trampoline.S

// Create a user page table for a given process,
// with no user memory, but with trampoline pages.
pagetable_t
proc_pagetable(struct proc *p)
{
  pagetable_t pagetable;

  // An empty page table.
  pagetable = uvmcreate();
  if(pagetable == 0)
    return NULL;

  // map the trampoline code (for system call return)
  // at the highest user virtual address.
  // only the supervisor uses it, on the way
  // to/from user space, so not PTE_U.
  if(mappages(pagetable, TRAMPOLINE, PGSIZE,
              (uint64)trampoline, PTE_R | PTE_X) < 0){
    uvmfree(pagetable, 0);
    return NULL;
  }

  // map the trapframe just below TRAMPOLINE, for trampoline.S.
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
              (uint64)(p->trapframe), PTE_R | PTE_W) < 0){
    vmunmap(pagetable, TRAMPOLINE, 1, 0);
    uvmfree(pagetable, 0);
    return NULL;
  }

  return pagetable;
}

// Free a process's page table, and free the
// physical memory it refers to.
void
proc_freepagetable(pagetable_t pagetable, uint64 sz)
{
  vmunmap(pagetable, TRAMPOLINE, 1, 0);
  vmunmap(pagetable, TRAPFRAME, 1, 0);
  uvmfree(pagetable, sz);
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

<<<<<<< HEAD
static inline char const *__state_to_str(enum procstate state) {
	switch (state) {
	case RUNNABLE: 		return "runnable"; 
	case RUNNING: 		return "running "; 
	case SLEEPING: 		return "sleeping";
	case ZOMBIE: 		return "zombie  ";
	default: 			return "\e[31;1m????????\e[0m";
	}
=======
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  struct proc *p;
  char *state;

  printf("\nPID\tLOAD\tHEAP\tSTATE\tNAME\n");
  for(p = proc; p < &proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";

    uint64 load = 0, heap = 0;
    for (struct seg *s = p->segment; s != NULL; s = s->next) {
      if (s->type == LOAD) {
        load += s->sz;
      } else if (s->type == HEAP) {
        heap = s->sz;
      }
    }

    printf("%d\t%d\t%d\t%s\t%s", p->pid, load, heap, state, p->name);
    printf("\n");
  }
>>>>>>> usrmm
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
	printf("\nPID\tSTATE\tNAME\tMEM\n");
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
