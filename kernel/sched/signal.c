// Kernel part for handling signals 

#ifndef __DEBUG_signal
#undef DEBUG
#endif 

#define __module_name__		"signal"

#include "sched/proc.h"
#include "mm/kmalloc.h"
#include "sync/spinlock.h"
#include "sched/signal.h"

#include "utils/debug.h"

// Please be noticed that before we insert a new ksig into 
// the list, we must make sure that there's no sigaction for 
// the same signum in the sigaction list. 
static void __insert_sig(struct proc *p, ksigaction_t *ksig) {
	__debug_assert("__insert_sig", NULL != p, "p == NULL\n");

	if (NULL == ksig) return ;

	ksigaction_t *tmp = p->sig_act;

	// Create a new one 
	tmp = p->sig_act;
	ksig->next = p->sig_act;
	ksig->pprev = &(p->sig_act);
	if (NULL != tmp) {
		tmp->pprev = &(ksig->next);
	}
	p->sig_act = ksig;
}

static void __free_sig(struct proc *p) {
	__debug_assert("__free_sig", NULL != p, "p == NULL\n");

	ksigaction_t *tmp = p->sig_act;

	while (NULL != tmp) {
		ksigaction_t *next = tmp->next;
		kfree(tmp);
		tmp = next;
	}

	p->sig_act = NULL;
}

static ksigaction_t const*__search_sig(struct proc *p, int signum) {
	__debug_assert("__search_sig", NULL != p, "p == NULL\n");

	ksigaction_t const* tmp = p->sig_act;

	while (NULL != tmp) {
		if (tmp->signum == signum) {
			break;
		}
		tmp = tmp->next;
	}

	return tmp;
}

// Check out to see if there's a signal to handle
void do_notify_resume(void) {
	TODO
}

int set_sigaction(
	int signum, 
	struct sigaction const *act, 
	struct sigaction *oldact
) {
	struct proc *p = myproc();
	ksigaction_t *tmp = __search_sig(p, signum);

	if (NULL != oldact && NULL != tmp) {
		*old = tmp->sigact;
	}

	if (NULL != act) {
		if (NULL == tmp) {
			// insert a new action into the proc 
			ksigaction_t *new = kmalloc(sizeof(ksigaction_t));
			__debug_assert("set_sigaction", NULL != new, "alloc error\n");

			new->sigact = *act;
			new->signum = signum;

			__insert_sig(p, new);
		}
		else {
			tmp->sigact = *act;
		}
	}

	return 0;
}

int sigprocmask(
	int how, 
	__sigset_t *set, 
	__sigset_t *oldset
) {
	struct proc *p = myproc();

	__debug_assert("sigprocmask", NULL != p, "p == NULL\n");

	for (int i = 0; i < SIG_LENGTH; i ++) {
		if (NULL != oldset) {
			oldset->__val[i] = p->sig_set.__val[i];
		}

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
			default: 
				panic("invalid how\n");
		}
	}

	return 0;
}

void sigqueue(void) {
	panic("Not implemented!\n");
}


void sigframefree(struct sig_frame *head) {
	while (NULL != head) {
		struct sig_frame *next = head->next;
		kfree(head);
		head = next;
	}
}

__sighandler_t term_handler;
void sig_handler(int signum, __sig_handler_t handler);

void sigdetect(void) {
	struct proc *p = myproc();

	__debug_assert("sigdetect", NULL != p, "p == NULL\n");

	int signum = 0;
	for (int i = 0; i < 16; i ++) {
		int const len = sizeof(unsigned long) * 8;
		int bit = 0;
		for (; bit < len; bit ++) {
			if (p->sig_pending.__val[i] & (1 << bit)) {
				signum += bit;
				goto sig_find;
			}
		}
		signum += len;
	}
	return ;	// no signal to handle

sig_find: 
	struct sig_frame *frame = kmalloc(sizeof(struct sig_frame));
	__assert()
}

void sigreturn(void) {
	TODO
}