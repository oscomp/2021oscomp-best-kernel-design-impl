#ifndef __SIGNAL_H
#define __SIGNAL_H

#include "types.h"
#include "utils/list.h"

#define SIGRTMIN 	34
#define SIGRTMAX	64

// Some other signals 
#define SIGTERM 	15
#define SIGKILL		9

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
	void (*sa_restorer)(void);
};

typedef struct {
	list_node_t __list;
	struct sigaction act;
	int signum;
} ksigaction_t;

union sigval {
	int sival_int;
	void *sival_ptr;
};

void do_notify_resume(void);

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

int sigqueue(int pid, int sig, union sigval const value);

void sigact_copy(ksigaction_t **pdst, ksigaction_t *src);
void sigact_free(ksigaction_t *head);

#endif 