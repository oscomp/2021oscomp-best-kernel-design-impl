// Kernel part for handling signals 

#define __module_name__		"signal"

#include "proc.h"
#include "kmalloc.h"
#include "utils/list.h"
#include "signal.h"

#include "debug.h"

// static void handle_signal(struct ksignal *ksig, struct pt_regs *regs) {}

// static void do_signal(struct pt_regs *regs) {}

// void do_notify_resume(void) {
// 	// currently do nothing
// }

// register a new signal handler, or search for specific one 
int set_sigaction(int signum, struct sigaction const *act, struct sigaction *oldact) {
	struct proc *p = myproc();
	
	// search for wanted sigaction
	ksigaction_t *tmp = p->sig_act;
	while (NULL != tmp) {
		if (signum == tmp->signum) {
			break;
		}
		else {
			tmp = list_next(ksigaction_t, tmp);
		}
	}

	// never exists and we want to insert, create a new one
	if (NULL == tmp && NULL != act) {
		tmp = (ksigaction_t*)kmalloc(sizeof(ksigaction_t));
		__debug_assert("sigaction", NULL != tmp, "error kmalloc\n");
		tmp->signum = signum;
		list_push_front(&(p->sig_act), tmp);
	}
	else {
		// not found and we shall not create a new one 
		__debug_warn("sigaction", "signum %d not found", signum);
		return -1;
	}

	// copy in 
	if (NULL != act) tmp->act = *act;
	if (NULL != oldact) *oldact = tmp->act;

	return 0;
}

// set/get proc mask bits 
int sigprocmask(int how, __sigset_t *set, __sigset_t *oldset) {
	struct proc *p = myproc();

	if (NULL != oldset) {
		*oldset = p->sig_set;
	}

	set->__val[0] &= ~((1 << SIGTERM) | (1 << SIGKILL));

	for (int i = 0; i < SIG_LENGTH; i ++) {		
		switch (how) {
			case SIG_BLOCK: 
				p->sig_set.__val[i] |= set->__val[i];
				break;
			case SIG_UNBLOCK: 
				p->sig_set.__val[i] &= ~(set->__val[i]);
				break;
			case SIG_SETMASK: 
				p->sig_set.__val[i] = set->__val[i];
				break;
		}
	}

	return 0;
}

// currently do nothing 
int sigqueue(int pid, int sig, union sigval const value) {
	return 0;
}

void sigact_copy(ksigaction_t **pdst, ksigaction_t *src) {
	ksigaction_t *tmp;

	while (NULL != src) {
		tmp = (ksigaction_t*)kmalloc(sizeof(ksigaction_t));
		__debug_assert("sigact_copy", NULL != tmp, "error kmalloc\n");
		tmp->act = src->act;
		tmp->signum = src->signum;

		// push front is faster, as the order is less considered
		list_push_front(pdst, tmp);

		src = list_next(ksigaction_t, src);
	}
}

void sigact_free(ksigaction_t *head) {
	while (NULL != head) {
		ksigaction_t *next = list_next(ksigaction_t, head);
		kfree(head);
		head = next;
	}
}