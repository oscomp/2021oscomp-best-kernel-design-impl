#ifndef __DEBUG_syssignal
#undef DEBUG
#endif 

#define __module_name__ 		"syssignal"

#include "types.h"
#include "sched/signal.h"
#include "syscall.h"
#include "printf.h"
#include "param.h"
#include "mm/vm.h"
#include "errno.h"

#include "utils/debug.h"

uint64 sys_rt_sigaction(void) {
	int signum;
	uint64 uptr_act;		// struct sigaction const *act
	uint64 uptr_oldact;		// struct sigaction *oldact

	// extract user args
	if (argint(0, &signum) < 0) {
		return -EINVAL;
	}
	if (argaddr(1, &uptr_act) < 0) {
		return -EINVAL;
	}
	if (argaddr(2, &uptr_oldact) < 0) {
		return -EINVAL;
	}

	// copy struct sigaction from user space 
	struct sigaction act;
	struct sigaction oldact;
	if (uptr_act && copyin2((char*)&act, uptr_act, sizeof(struct sigaction)) < 0) {
		return -EFAULT;
	}

	if (set_sigaction(
		signum, 
		uptr_act ? &act : NULL, 
		uptr_oldact ? &oldact : NULL
	) < 0) {
		return -EINVAL;
	}

	// copyout old struct sigaction 
	if (uptr_oldact && copyout2(uptr_oldact, (char*)&oldact, sizeof(struct sigaction)) < 0) {
		return -EFAULT;
	}

	return 0;
}

uint64 sys_rt_sigprocmask(void) {
	int how;
	uint64 uptr_set, uptr_oldset;

	__sigset_t set, oldset;

	// extract user arguments
	if (argint(0, &how) < 0) 
		return -EINVAL;
	if (argaddr(1, &uptr_set) < 0) 
		return -EINVAL;
	if (argaddr(2, &uptr_oldset) < 0) 
		return -EINVAL;

	// copy in __sigset_t 
	if (uptr_set && copyin2((char*)&set, uptr_set, sizeof(__sigset_t)) < 0) {
		return -EFAULT;
	}

	if (sigprocmask(
		how, 
		&set, 
		uptr_oldset ? &oldset : NULL
	)) {
		return -EINVAL;
	}

	if (uptr_oldset && copyout2(uptr_oldset, (char*)&oldset, sizeof(__sigset_t)) < 0) {
		return -EFAULT;
	}

	return 0;
}

uint64 sys_kill(void) {
	int pid, sig;

	// if(argint(0, &pid) < 0)
	// 	return -1;
	argint(0, &pid);
	argint(1, &sig);
	return kill(pid, sig);
}