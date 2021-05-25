// Time-Related Syscalls 

#ifndef __DEBUG_systime 
#undef DEBUG 
#endif 

#define __module_name__ 	"systime" 

#include "include/types.h"
#include "include/printf.h"

uint64 sys_times(void) {
	printf("not implemented yet!\n");
	return -1;
}

uint64 sys_gettimeofday(void) {
	printf("not implemented yet!\n");
	return -1;
}
