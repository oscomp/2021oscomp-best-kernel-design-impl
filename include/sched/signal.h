#ifndef __SIGNAL_H
#define __SIGNAL_H

#include "types.h"
#include "trap.h"

#define SIGRTMIN 	34
#define SIGRTMAX	64

// Some other signals 
#define SIGTERM 	15
#define SIGKILL		9
#define SIGABRT		6
#define SIGHUP		1
#define SIGINT		2
#define SIGQUIT		3
#define SIGILL		4
#define SIGTRAP		5

// Signal Flags
#define SA_NOCLDSTOP	0x00000001
#define SA_NOCLDWAIT	0x00000002
#define SA_NODEFER		0x08000000
#define SA_RESETHAND	0x80000000
#define SA_RESTART		0x10000000
#define SA_SIGINFO		0x00000004

// Flags for sigprocmask 
#define SIG_BLOCK 		0
#define SIG_UNBLOCK 	1
#define SIG_SETMASK		2

#define SIG_LENGTH 		16

typedef void (*__sighandler_t)(int);

typedef struct {
	unsigned long __val[16];
} __sigset_t;

struct sigaction {
	union {		// let's make it simple, only sa_handler is supported 
		__sighandler_t sa_handler;
		// void (*sa_sigaction)(int, siginfo_t *, void *);
	} __sigaction_handler;
	__sigset_t sa_mask;	// signals to be blocked during handling 
	int sa_flags;
	// void (*sa_restorer)(void);	// this field is not used on risc-v
};

typedef struct __ksigaction_t {
	struct __ksigaction_t *next;
	struct __ksigaction_t **pprev;
	struct sigaction sigact;
	int signum;
} ksigaction_t;

int set_sigaction(
	int signum, 
	struct sigaction const *act, 
	struct sigaction *oldact
);

int sigprocmask(
	int how, 
	__sigset_t *set, 
	__sigset_t *oldset
);

void sigqueue(int pid, int signum);

struct sig_frame {
	__sigset_t mask;
	struct trapframe *tf;
	struct sig_frame *next;
};

// Free the list of sig_frame. 
void sigframefree(struct sig_frame *head);

// Detect if there's a signal that we can handle, if is, 
// store the old trapframe and pending to sig_frame list. 
void sigdetect(void);

// Return from a signal handling, restore previous trapframe and pending 
// from sig_frame list. 
void sigreturn(void);

#endif 