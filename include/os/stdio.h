#ifndef INCLUDE_STDIO_H_
#define INCLUDE_STDIO_H_

#include<stdarg.h>

/* kernel printf */
int printk(const char *fmt, ...);
int vprintk(const char *fmt, va_list va);
int prints(const char *fmt, ...);
int vprints(const char *fmt, va_list va);

#endif
