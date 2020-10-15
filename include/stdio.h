#ifndef INCLUDE_STDIO_H_
#define INCLUDE_STDIO_H_

#include<stdarg.h>

/* kernel printf */
int printk(const char *fmt, ...);
int vprintk(const char *fmt, va_list va);

/* user printk */
int printf(const char *fmt, ...);

#endif
