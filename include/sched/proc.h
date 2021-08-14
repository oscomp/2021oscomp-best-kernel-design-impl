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
#include "time.h"

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

#define RUNNABLE 		1
#define RUNNING			2
#define SLEEPING 		3
#define ZOMBIE 			4

#define __occupy(state) \
	(((cpuid() + 1) << 8) | (state))
#define __liberate(state) \
	((state) & 0xff)


struct tms {
	uint64 utime;		// user time 
	uint64 stime;		// system time 
	uint64 cutime;		// user time of children 
	uint64 cstime;		// system time of children 
	uint64 ikstmp;		// the last moment when entering kernel
	uint64 okstmp;		// the last moment when leaving kernel
};

// Process Control Block 
struct proc {
	// basic information 
	// these fields MUST be protected by lk 
	int killed;				// if non-zero, have been killed
	int xstate;				// Exit status to be returned to parent's wait()
	int pid;				// Process ID 
	struct proc *hash_next;		// next proc on hash list 
	struct proc **hash_pprev;	// previous 'struct proc*' field 

	// next and prev are protected by proc_lock 
	struct proc *sched_next;		// point to next proc 
	struct proc **sched_pprev;
	int timer;						// timer 
	uint64 state;					// current state of proc 
	void *chan;						// the reason this proc is sleeping for 
	uint64 sleep_expire;			// wake up time for sleeping

	// times for process performance 
	struct tms proc_tms;
	int64 vswtch;			// voluntary context switches
	int64 ivswtch;			// involuntary context switches

	// parenting
	struct spinlock lk;				// lock to protect parenting of parent proc 
	struct proc *child;				// point to first child 
	struct proc *parent;			// point to its parent, NULL if it's `init`
	struct proc *sibling_next;		// point to first sibling 
	struct proc **sibling_pprev;		// point to previous sibling->sibling_next 

	// memory 
	uint64 kstack;					// virtual address of kernel stack 
	uint64 badaddr;					// bad address after page fault 
	pagetable_t pagetable;			// user pagetable 
	struct trapframe *trapframe;	// data page for trampoline.S 
	struct seg *segment;			// first seg list node 
	uint64 pbrk;					// program break

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

/* Jump into User Mode from Kernel */
void enter_user(uint64 old_kstack) __attribute__((noreturn));


/* Memory-Management Related */

/* Grow or shrink user memory by `n` bytes. 
	Return 0 on success, -1 on failure. */
int growproc(uint64 newbrk);


/* CPU */

/* Per-CPU state */
struct cpu {
	struct proc *proc;		// The process running on this cpu, or NULL 
	// struct context context;	// swtch() here to enter scheduler() 
	int noff;				// Depth of push_off() nesting 
	int intena;				// Were interrupts enabled before push_off()?
};

static inline int cpuid(void) {
	return r_tp();
}
struct cpu *mycpu(void);
struct proc *myproc(void);

/* Initialization */
void cpuinit(void);
void procinit(void);

/* Set up first user process. */
void userinit(void);

int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int either_copyin(void *dst, int user_src, uint64 src, uint64 len);

/* Display content of all registers */
void procdump(void);
int procnum(void);

#endif 
