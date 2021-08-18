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
	struct sigaction *oldact, 
	int len
) {
	struct proc *p = myproc();

	ksigaction_t *tmp = __search_sig(p, signum);
	// printf("pid %d search %d find %p\n", p->pid, signum, tmp);

	if (NULL != oldact && NULL != tmp) {
		oldact->__sigaction_handler = tmp->sigact.__sigaction_handler;
		// oldact->sa_flags = tmp->sigact.sa_flags;
		// for (int i = 0; i < len; i ++) {
		// 	oldact->sa_mask.__val[i] = tmp->sigact.sa_mask.__val[i];
		// }
	}

	if (NULL != act) {
		if (NULL == tmp) {
			// insert a new action into the proc 
			ksigaction_t *new = kmalloc(sizeof(ksigaction_t));
			__debug_assert("set_sigaction", NULL != new, "alloc error\n");
			__insert_sig(p, new);
			tmp = new;
		}

		// tmp->sigact.sa_flags = act->sa_flags;
		tmp->sigact.__sigaction_handler = act->__sigaction_handler;
		// for (int i = 0; i < len; i ++) {
		// 	tmp->sigact.sa_mask.__val[i] = act->sa_mask.__val[i];
		// }
		tmp->signum = signum;
	}

	// tmp = p->sig_act;
	// printf("pid = %d\n", p->pid);
	// while (NULL != tmp) {
	// 	printf("signum = %d, handler = %p\n", tmp->signum, tmp->sigact.__sigaction_handler.sa_handler);
	// 	tmp = tmp->next;
	// }
	// printf("++++++++++++++++++++++\n");

	return 0;
}

int sigprocmask(
	int how, 
	__sigset_t *set, 
	__sigset_t *oldset, 
	int len
) {
	struct proc *p = myproc();

	__debug_assert("sigprocmask", NULL != p, "p == NULL\n");

	for (int i = 0; i < len; i ++) {
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

	// SIGTERM cannot be masked 
	p->sig_set.__val[0] &= 1ul << SIGTERM;

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

void sighandle(void) {
	struct proc *p = myproc();

	__debug_assert("sigdetect", NULL != p, "p == NULL\n");

	__debug_info("sighandle", "killed = %d\n", p->killed);
	int signum = 0;
	if (p->killed) {
		signum = p->killed;

		const int len = sizeof(unsigned long) * 8;
		int i = (unsigned long)(p->killed) / len;
		int bit = (unsigned long)(p->killed) % len;
		p->sig_pending.__val[i] &= ~(1ul << bit++);
		p->killed = 0;

		for (; i < SIGSET_LEN; i ++) {
			while (bit < len) {
				if (p->sig_pending.__val[i] & (1ul << bit)) {
					p->killed = i * len + bit;
					goto start_handle;
				}
				bit ++;
			}
		}

		__debug_info("sighandler", "pid %d update pending_num %d -> %d\n", 
				p->pid, signum, p->killed);
	}
	else {
		// no signal to handle
		return ;
	}

	struct sig_frame *frame;
	struct trapframe *tf;
	ksigaction_t *sigact;
start_handle: 
	// search for signal handler 
	sigact = __search_sig(p, signum);

	// fast skip 
	// if (NULL == sigact && SIGCHLD == signum) {
	// 	return ;
	// }
	if (SIGCHLD == signum && 
		(NULL == sigact || NULL == sigact->sigact.__sigaction_handler.sa_handler)) {
			return;
	}

	frame = kmalloc(sizeof(struct sig_frame));
	__assert("sigdetect", NULL != frame, "alloc frame failed\n");

	tf = kmalloc(sizeof(struct trapframe));
	__assert("sigdetect", NULL != tf, "alloc tf failed\n");

	// copy mask 
	// for (int i = 0; i < SIGSET_LEN; i ++) {
	// 	frame->mask.__val[i] = p->sig_set.__val[i];
	// 	if (NULL == sigact) {
	// 		p->sig_set.__val[i] = 0;
	// 	}
	// 	else {
	// 		p->sig_set.__val[i] &= sigact->sigact.sa_mask.__val[i];
	// 	}
	// }

	// store proc's trapframe 
	frame->tf = p->trapframe;
	tf->epc = (uint64)(SIG_TRAMPOLINE + ((uint64)sig_handler - (uint64)sig_trampoline));
	tf->sp = p->trapframe->sp;
	tf->a0 = signum;
	if (NULL != sigact && sigact->sigact.__sigaction_handler.sa_handler) {
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
	// for (int i = 0; i < SIGSET_LEN; i ++) {
	// 	p->sig_set.__val[i] = frame->mask.__val[i];
	// }
	kfree(p->trapframe);
	p->trapframe = frame->tf;

	// remove this frame from list 
	p->sig_frame = frame->next;
	kfree(frame);
}