// Process Management 

#ifndef __PROC_H 
#define __PROC_H 

#include "types.h"
#include "riscv.h"
#include "spinlock.h"
#include "file.h"
#include "trap.h"

// Saved registers for kernel context switches.
struct context {
	uint64 ra;
	uint64 sp;

	// callee-saved
	uint64 s0;
	uint64 s1;
	uint64 s2;
	uint64 s3;
	uint64 s4;
	uint64 s5;
	uint64 s6;
	uint64 s7;
	uint64 s8;
	uint64 s9;
	uint64 s10;
	uint64 s11;
};

enum procstate {
	RUNNABLE, RUNNING, 
	SLEEPING, ZOMBIE, 
};

// Process Control Block 
struct proc {
	struct spinlock lk;

	// basic information 
	// these fields MUST be protected by lk 
	void *chan;
	enum procstate state;	// process state 
	int killed;				// if non-zero, have been killed
	int xstate;				// Exit status to be returned to parent's wait()
	int pid;				// Process ID 
	int timer;

	// next and prev are protected by queue's lock 
	struct proc *next;		// point to next proc 
	struct proc **prev;

	// parenting
	// these fields MUST be protected by lk 
	struct proc *child;			// point to first child 

	// parent and sibling are protected by parent's lock 
	struct proc *parent;		// point to its parent, NULL if it's `init`
	struct proc *sibling;		// point to first sibling 

	// trap handling 
	struct trapframe *tp;	// trapframe 

	// memory 
	// TODO: pagetable 

	// file system 
	struct file *ofile[NOFILE];	// open files 
	struct dirent *cwd;			// current director 

	// scheduling 
	struct context context;

	// debug 
	char name[16];	// process name 
	int tmask;		// trace mask 
};

/* Create a new process, copying the parent. 
	Sets up child mem space to return as if from fork() system call */
int fork(void);

/* Exit the current process. does not return. 
	An exited process remains in the zombie state until its 
	parent calls wait(). */
void exit(int xstatus);

/* Kill the process with the given pid. 
	The victim won't exit until it tries to return 
	to user space (see usertrap() in trap.c) */
int kill(int pid);


/* Scheduling */

void enqueue_irq(struct proc *p);
void enqueue_normal(struct proc *p);
void proc_tick(void);

/* Wait for a child process to exit and return its pid. 
	Return -1 if this process has no children. */
int wait(uint64 addr);

/* Give up CPU and enter scheduler */
void yield(void);

/* Atomically release lock and sleep on chan. 
	Reacquires lock when awakened */
void sleep(void *chan, struct spinlock *lk);

/* Wake up all processes sleeping on chan. 
	Must be called without any p->lock. */
void wakeup(void *chan);

/* Select next proc to run */
void scheduler(void) __attribute__((noreturn))

/* Return to schduler() */
void sched(void);


/* Memory-Management Related */

/* Grow or shrink user memory by `n` bytes. 
	Return 0 on success, -1 on failure. */
int growproc(int n);

/* Create a user pagetable for a given process, 
	with no user memory, but with trampoline pages. */
pagetable_t proc_pagetable(struct proc *p);

/* Free a process's pagetable, and free the physical 
	memory it refers to. */
void proc_freepagetable(pagetable_t pagetable, uint64 sz);


/* CPU */

/* Per-CPU state */
struct cpu {
	struct proc *proc;		// The process running on this cpu, or NULL 
	struct context context;	// swtch() here to enter scheduler() 
	int noff;				// Depth of push_off() nesting 
	int intena;				// Were interrupts enabled before push_off()?
};

int cpuid(void);
struct cpu *mycpu(void);
struct proc *myproc(void);

/* Initialization */
void procinit(void);

/* Set up first user process. */
void userinit(void);

int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int either_copyin(void *dst, int user_src, uint64 src, uint64 len);

/* Display content of all registers */
void reg_info(void);

#endif 
