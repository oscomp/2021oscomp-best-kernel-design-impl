#ifndef INCLUDE_STDIO_H_
#define INCLUDE_STDIO_H_

#include <stdarg.h>
#include <type.h>

/* kernel printf */
int printk(const char *fmt, ...);
int vprintk(const char *fmt, va_list va);
uint32_t printk_port(const char *fmt, ...);
/* user printk */
int printf(const char *fmt, ...);

#endif
