// Process Management 

#ifndef __PROC_H 
#define __PROC_H 

#include "types.h"
#include "param.h"
#include "trap.h"
#include "mm/usrmm.h"
#include "fs/file.h"
#include "fs/fs.h"
#include "hal/riscv.h"
#include "mesg/signal.h"
#include "sync/spinlock.h"

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

struct tms {
	uint64 utime;		// user time 
	uint64 stime;		// system time 
	uint64 cutime;		// user time of children 
	uint64 cstime;		// system time of children 
};

// Process Control Block 
struct proc {
	// a useless lock 
	struct spinlock lk;

	// basic information 
	// these fields MUST be protected by lk 
	int killed;				// if non-zero, have been killed
	int xstate;				// Exit status to be returned to parent's wait()
	int pid;				// Process ID 

	// next and prev are protected by proc_lock 
	struct proc *next;		// point to next proc 
	struct proc **prev;
	int timer;				// timer 
	enum procstate state;	// process state 
	void *chan;				// the reason this proc is sleeping for 

	// times for process performance 
	struct tms proc_tms;

	// parenting
	// these fields can only be operated by proc itself 
	struct proc *child;				// point to first child 
	struct proc *parent;			// point to its parent, NULL if it's `init`
	struct proc *sibling_next;		// point to first sibling 
	struct proc **sibling_prev;		// point to previous sibling->sibling_next 

	// memory 
	uint64 kstack;					// virtual address of kernel stack 
	//uint64 sz;						// size of process memory 
	pagetable_t pagetable;			// user pagetable 
	struct trapframe *trapframe;	// data page for trampoline.S 
	struct seg *segment;			// first seg list node 

	// file system 
	struct fdtable fds;				// Open files
	struct inode *cwd;				// Current directory
	struct inode *elf;				// Executable file
	// scheduling 
	struct context context;			// the "trapframe" for kernel running

	// signal 
	ksigaction_t *sig_act;
	__sigset_t sig_set;
	__sigset_t sig_pending;

	// debug 
	char name[16];	// process name 
	int tmask;		// trace mask 
};

/* Create a new process, copying the parent. 
	Sets up child mem space to return as if from fork() system call 
	`cow` means that fork implements 'Copy-On-Write' strategy */
int fork_cow(void);

/* Create a process-level thread, partly copying the parent. */
int clone(uint64 flag, uint64 stack);

/* Exit the current process. does not return. 
	An exited process remains in the zombie state until its 
	parent calls wait(). */
void exit(int xstatus);

/* Kill the process with the given pid. 
	The victim won't exit until it tries to return 
	to user space (see usertrap() in trap.c) */
int kill(int pid);


/* Scheduling */

// update each proc's timer 
void proc_tick(void);

/* Wait for a child process to exit and return its pid. 
	`options` indicates whether wait4() will block. 
	Return -1 if this process has no children */
int wait4(int pid, uint64 status, uint64 options);

/* Give up CPU and enter scheduler */
void yield(void);

/* Atomically release lock and sleep on chan. 
	Reacquires lock when awakened */
void sleep(void *chan, struct spinlock *lk);

/* Wake up all processes sleeping on chan. 
	Must be called without any p->lock. */
void wakeup(void *chan);

/* Return the next runnable process */
struct proc const *get_runnable(void);

/* Select next proc to run */
void scheduler(void) __attribute__((noreturn));

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
void proc_freepagetable(pagetable_t pagetable, struct seg *head);


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
void procdump(void);
int procnum(void);

#endif 
