//
// formatted string output -- sprintf
//

#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "hal/riscv.h"
#include "sprintf.h"
#include "utils/debug.h"

static char digits[] = "0123456789abcdef";

// static char* 
// sprintstring(char* d, int bufLength, const char* s) {
//   int count = 0;
//   while (*s)
//   {
//     // consputc(*s++);
//     *d++ = *s++;
//     if (++count >= bufLength) {
//       return NULL;
//     }
//   }
//   return d;
// }

static char* 
sprintint(char* d, int bufLength, int xx, int base, int sign)
{
	char buf[16];
	int i;
	uint x;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do {
		buf[i++] = digits[x % base];
	} while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	int count = 0;
	while (--i >= 0){
		*d++ = buf[i];
		// consputc(buf[i]);
		if (++count >= bufLength) {
			return NULL;
		}
	}
	return d;
}


static char* 
sprintptr(char* d, int bufLength, uint64 x)
{
	int i;
	int count = 0;
	// consputc('0');
	*d++ =  '0';
	if (++count >= bufLength) {
		return NULL;
	}
	// consputc('x');
	*d++ = 'x';
	if (++count >= bufLength) {
		return NULL;
	}
	for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4){
		// consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
		*d++ = digits[x >> (sizeof(uint64) * 8 -4)];
		if (++count >= bufLength) {
			return NULL;
		}
	}
	return d;
}

/*
 * Description: print to the string. only understands %d, %x, %p, %s.
 * Return: number of characters written, -1 at overflow.
 */
int
sprintf(char *s, unsigned long bufLength, char *fmt, ...)
{
	va_list ap;
	int i, c;
	char *source;
	char *start = s;
	char *current = s;
	char *end = &s[bufLength];

	if (fmt == 0)
		panic("null fmt");

	va_start(ap, fmt);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
		if(c != '%'){
			// consputc(c);
			*current++ = c;
			if (current == end) {
				return -1;
			}
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;
		switch(c){
		case 'd':
			current = sprintint(current, bufLength-(current-start), va_arg(ap, int), 10, 1);
			if (!current) {
				return -1;
			}
			break;
		case 'x':
			current = sprintint(current, bufLength-(current-start), va_arg(ap, int), 16, 1);
			if (!current) {
				return -1;
			}
			break;
		case 'p':
			current = sprintptr(current, bufLength-(current-start), va_arg(ap, uint64));
			if (!current) {
				return -1;
			}
			break;
		case 's':
			if((source = va_arg(ap, char*)) == 0)
				source = "(null)";
			for(; *source; source++) {
				// consputc(*source);
				*current++ = *source;
				if (current == end) {
					return -1; 
				}
			}
			break;
		case '%':
			// consputc('%');
			*current++ = '%';
			if (current == end) {
				return -1;
			}
			break;
		default:
			// Print unknown % sequence to draw attention.
			// consputc('%');
			*current++ = '%';
			if (current == end) {
				return -1;
			}
			// consputc(c);
			*current++ = c;
			if (current == end) {
				return -1;
			}
			break;
		}
	}
	*current++ = '\0';
	return current - start;
}
