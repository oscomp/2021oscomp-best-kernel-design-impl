#ifndef __PRINTF_H
#define __PRINTF_H

#include "hal/riscv.h"

void printfinit(void);

void printf(char *fmt, ...);

void __panic(char *s) __attribute__((noreturn));
#define panic(s) do {\
	printf("[\e[31;1mpanic\e[0m]: hart %d at %s: %d\n", r_tp(), __FILE__, __LINE__);\
	__panic(s);\
} while (0)

void backtrace();

void print_logo();

#endif 
