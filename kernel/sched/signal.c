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
	__debug_assert("__insert_sig", NULL != ksig, "ksig == NULL\n");

	// Create a new one 
	ksig->next = p->sig_act;
	p->sig_act = ksig;
}

static ksigaction_t *__search_sig(struct proc *p, int signum) {
	__debug_assert("__search_sig", NULL != p, "p == NULL\n");

	ksigaction_t const* tmp = p->sig_act;

	while (NULL != tmp) {
		if (tmp->signum == signum) {
			break;
		}
		tmp = tmp->next;
	}

	return (ksigaction_t*)tmp;
}

int set_sigaction(
	int signum, 
	struct sigaction const *act, 
	struct sigaction *oldact
) {
	struct proc *p = myproc();

	ksigaction_t *tmp = __search_sig(p, signum);

	if (NULL != oldact && NULL != tmp) {
		*oldact = tmp->sigact;
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

	for (int i = 0; i < SIGSET_LEN; i ++) {
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

void sigframefree(struct sig_frame *head) {
	while (NULL != head) {
		struct sig_frame *next = head->next;
		__debug_assert("sigframefree", next != head, "loop!\n");
		__debug_info("sigframefree", "free trapframe %p\n", head->tf);
		kfree(head->tf);
		__debug_info("sigframefree", "free %p\n", head);
		kfree(head);
		head = next;
	}
}

void sigaction_free(ksigaction_t *head) {
	while (NULL != head) {
		ksigaction_t *next = head->next;
		kfree(head);
		head = next;
	}
}

int sigaction_copy(ksigaction_t **pdst, ksigaction_t const *src) {
	ksigaction_t *tmp = NULL;

	*pdst = NULL;
	if (NULL == src) {
		return 0;
	}

	while (NULL != src) {
		tmp = kmalloc(sizeof(ksigaction_t));
		if (NULL == tmp) {
			__debug_warn("sigaction_copy", "fail to alloc\n");
			sigaction_free(*pdst);
			*pdst = NULL;
			return -1;
		}

		*tmp = *src;
		tmp->next = *pdst;
		*pdst = tmp;

		src = src->next;
	}

	return 0;
}

extern char sig_trampoline[];
extern char sig_handler[];
extern char default_sigaction[];

void sigdetect(void) {
	struct proc *p = myproc();
	struct sig_frame *frame;

	__debug_assert("sigdetect", NULL != p, "p == NULL\n");

	int signum = 0;
	for (int i = 0; i < SIGSET_LEN; i ++) {
		int const len = sizeof(unsigned long) * 8;
		int bit = 0;
		for (; bit < len; bit ++) {
			if (p->sig_pending.__val[i] & (1ul << bit)) {
				signum += bit;
				p->sig_pending.__val[i] &= ~(1ul << bit);
				goto find;
			}
		}
		signum += len;
	}
	return ;	// no signal to handle

find: 
	frame = kmalloc(sizeof(struct sig_frame));
	__assert("sigdetect", NULL != frame, "alloc frame failed\n");

	struct trapframe *tf = kmalloc(sizeof(struct trapframe));
	__assert("sigdetect", NULL != tf, "alloc tf failed\n");

	// search for signal handler 
	ksigaction_t *sigact = __search_sig(p, signum);

	// copy mask 
	for (int i = 0; i < SIGSET_LEN; i ++) {
		frame->mask.__val[i] = p->sig_set.__val[i];
		if (NULL == sigact) {
			p->sig_set.__val[i] = 0;
		}
		else {
			p->sig_set.__val[i] &= sigact->sigact.sa_mask.__val[i];
		}
	}

	// store proc's trapframe 
	frame->tf = p->trapframe;
	tf->epc = (uint64)(SIG_TRAMPOLINE + ((uint64)sig_handler - (uint64)sig_trampoline));
	tf->sp = p->trapframe->sp;
	tf->a0 = signum;
	if (NULL != sigact) {
		tf->a1 = (uint64)(sigact->sigact.__sigaction_handler.sa_handler);
	}
	else {
		// use the default handler 
		tf->a1 = (uint64)(SIG_TRAMPOLINE + ((uint64)default_sigaction - (uint64)sig_trampoline));
	}
	p->trapframe = tf;

	// insert sig_frame into proc's sig_frame list 
	frame->next = p->sig_frame;
	p->sig_frame = frame;
}

void sigreturn(void) {
	struct proc *p = myproc();

	__debug_assert("sigreturn", NULL != p, "no proc\n");
	if (NULL == p->sig_frame) {	// it's not in a sighandler!
		exit(-1);
	}

	__debug_info("sigreturn", "pid %d sigreturn\n", p->pid);

	struct sig_frame *frame = p->sig_frame;
	for (int i = 0; i < SIGSET_LEN; i ++) {
		p->sig_set.__val[i] = frame->mask.__val[i];
	}
	kfree(p->trapframe);
	p->trapframe = frame->tf;

	// remove this frame from list 
	p->sig_frame = frame->next;
	kfree(frame);
}