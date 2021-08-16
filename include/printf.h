#ifndef __PRINTF_H
#define __PRINTF_H

#include "utils/debug.h"
#include "sched/proc.h"

void printfinit(void);

void printf(char *fmt, ...);

void __panic(char *s) __attribute__((noreturn));
#define panic(s) do {\
	printf(__ERROR(__module_name__)": hart %d at %s: %d\n", \
			cpuid(), __FILE__, __LINE__\
	);\
	__panic(s);\
} while (0)

void backtrace();

void print_logo();

#endif 
