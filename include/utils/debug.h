// This header contains some useful macros for debug 

// To avoid namespace conflicts, use `__` before macro name 

#ifndef __DEBUG_H 
#define __DEBUG_H 

// `module_name` must be a C-type string 
#define __INFO(str) 	"[\e[32;1m"str"\e[0m]"
#define __WARN(str) 	"[\e[33;1m"str"\e[0m]"
#define __ERROR(str) 	"[\e[31;1m"str"\e[0m]"

#ifndef __module_name__ 
	#define __module_name__ 	"xv6-k210"
#endif 

#include "printf.h"

// display message only if it's DEBUG mode 
#ifdef DEBUG 
#define __debug_msg(...) \
	printf(__VA_ARGS__)
#else 
#define __debug_msg(...) \
	do {} while(0)
#endif 

#define __debug_info(func, ...) \
	__debug_msg(__INFO(__module_name__)": "func": "__VA_ARGS__) 
#define __debug_warn(func, ...) \
	__debug_msg(__WARN(__module_name__)": "func": "__VA_ARGS__) 
#define __debug_error(func, ...) do {\
	__debug_msg(__ERROR(__module_name__)": "func": "__VA_ARGS__);\
	printf("%s: line %d\n", __FILE__, __LINE__);\
} while (0)

#ifdef DEBUG 
	#define __debug_assert(func, cond, ...) do {\
		if (!(cond)) {\
			__debug_error(func, __VA_ARGS__);\
			panic("panic!\n");\
		}\
	} while (0)
#else 
	#define __debug_assert(func, cond, ...) \
		do {} while(0)
#endif 

#endif 
