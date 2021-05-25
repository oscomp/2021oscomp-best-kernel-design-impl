#ifndef __DEBUG_sysuname 
#undef DEBUG 
#endif 

#define __module_name__ 	"sysuname"

#include "include/uname.h"
#include "include/syscall.h"

uint64 sys_uname(void) {
	uint64 addr;

	if (argaddr(0, &addr) < 0) {
		return -1;
	}

	return uname_copyout(addr);
}
