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
	int size;

	// extract user args
	// if (argint(0, &signum) < 0) {
	// 	return -EINVAL;
	// }
	// if (argaddr(1, &uptr_act) < 0) {
	// 	return -EINVAL;
	// }
	// if (argaddr(2, &uptr_oldact) < 0) {
	// 	return -EINVAL;
	// }
	argint(0, &signum);
	argaddr(1, &uptr_act);
	argaddr(2, &uptr_oldact);
	argint(3, &size);

	// copy struct sigaction from user space 
	struct sigaction act;
	struct sigaction oldact;
	// if (uptr_act && copyin2((char*)&act, uptr_act, sizeof(struct sigaction)) < 0) {
	// 	return -EFAULT;
	// }
	if (size > 16 * 8) {
		size = 16 * 8;
	}
	else if (size < 8) {
		size = 8;
	}

	if (uptr_act) {
		if (
			copyin2((char*)&(act.__sigaction_handler), uptr_act, sizeof(__sighandler_t)) < 0 || 
			copyin2((char*)&(act.sa_mask), uptr_act + sizeof(__sighandler_t), size) < 0 || 
			copyin2((char*)&(act.sa_flags), uptr_act + sizeof(__sighandler_t) + size, sizeof(int) < 0)
		) {
			return -EFAULT;
		}
	}

	if (set_sigaction(
		signum, 
		uptr_act ? &act : NULL, 
		uptr_oldact ? &oldact : NULL, 
		size / 8
	) < 0) {
		return -EINVAL;
	}

	// copyout old struct sigaction 
	// if (uptr_oldact && copyout2(uptr_oldact, (char*)&oldact, sizeof(struct sigaction)) < 0) {
	// 	return -EFAULT;
	// }
	if (uptr_oldact) {
		if (
			copyout2(uptr_oldact, (char*)&(act.__sigaction_handler), sizeof(__sighandler_t)) < 0 || 
			copyout2(uptr_oldact + sizeof(__sighandler_t), (char*)&(act.sa_mask), size) < 0 || 
			copyout2(uptr_oldact + sizeof(__sighandler_t) + size, (char*)&(act.sa_flags), sizeof(int) < 0)
		) {
			return -EFAULT;
		}
	}

	return 0;
}

uint64 sys_rt_sigprocmask(void) {
	int how;
	uint64 uptr_set, uptr_oldset;

	__sigset_t set, oldset;
	int size;

	// extract user arguments
	// if (argint(0, &how) < 0) 
	// 	return -EINVAL;
	// if (argaddr(1, &uptr_set) < 0) 
	// 	return -EINVAL;
	// if (argaddr(2, &uptr_oldset) < 0) 
	// 	return -EINVAL;
	argint(0, &how);
	argaddr(1, &uptr_set);
	argaddr(2, &uptr_oldset);
	argint(3, &size);

	if (size > 16 * 8) {
		size = 16 * 8;
	}
	else if (size < 8) {
		size = 8;
	}

	// copy in __sigset_t 
	if (uptr_set && copyin2((char*)&set, uptr_set, size) < 0) {
		return -EFAULT;
	}

	if (sigprocmask(
		how, 
		&set, 
		uptr_oldset ? &oldset : NULL, 
		size / 8
	)) {
		return -EINVAL;
	}

	if (uptr_oldset && copyout2(uptr_oldset, (char*)&oldset, size) < 0) {
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