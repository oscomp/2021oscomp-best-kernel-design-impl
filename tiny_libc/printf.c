
#include <stdarg.h>
#include <string.h>
#include <sys/syscall.h>

static unsigned int mini_itoa(
    long value, unsigned int radix, unsigned int uppercase,
    unsigned int unsig, char *buffer, unsigned int zero_pad)
{
    char *pbuffer = buffer;
    int negative  = 0;
    unsigned int i, len;

    /* No support for unusual radixes. */
    if (radix > 16) return 0;

    if (value < 0 && !unsig) {
        negative = 1;
        value    = -value;
    }

    /* This builds the string back to front ... */
    do {
        int digit = value % radix;
        *(pbuffer++) =
            (digit < 10 ? '0' + digit :
                          (uppercase ? 'A' : 'a') + digit - 10);
        value /= radix;
    } while (value > 0);

    for (i = (pbuffer - buffer); i < zero_pad; i++)
        *(pbuffer++) = '0';

    if (negative) *(pbuffer++) = '-';

    *(pbuffer) = '\0';

    /* ... now we reverse it (could do it recursively but will
     * conserve the stack space) */
    len = (pbuffer - buffer);
    for (i = 0; i < len / 2; i++) {
        char j              = buffer[i];
        buffer[i]           = buffer[len - i - 1];
        buffer[len - i - 1] = j;
    }

    return len;
}

struct mini_buff
{
    char *buffer, *pbuffer;
    unsigned int buffer_len;
};

static int _putc(int ch, struct mini_buff *b)
{
    if ((unsigned int)((b->pbuffer - b->buffer) + 1) >=
        b->buffer_len)
        return 0;
    *(b->pbuffer++) = ch;
    *(b->pbuffer)   = '\0';
    return 1;
}

static int _puts(char *s, unsigned int len, struct mini_buff *b)
{
    unsigned int i;

    if (b->buffer_len - (b->pbuffer - b->buffer) - 1 < len)
        len = b->buffer_len - (b->pbuffer - b->buffer) - 1;

    /* Copy to buffer */
    for (i = 0; i < len; i++) *(b->pbuffer++) = s[i];
    *(b->pbuffer) = '\0';

    return len;
}

static int mini_vsnprintf(
    char *buffer, unsigned int buffer_len, const char *fmt,
    va_list va)
{
    struct mini_buff b;
    char bf[24];
    char ch;

    b.buffer     = buffer;
    b.pbuffer    = buffer;
    b.buffer_len = buffer_len;

    while ((ch = *(fmt++))) {
        if ((unsigned int)((b.pbuffer - b.buffer) + 1) >=
            b.buffer_len)
            break;
        if (ch != '%')
            _putc(ch, &b);
        else {
            char zero_pad = 0;
            int longflag = 0;
            char *ptr;
            unsigned int len;

            ch = *(fmt++);

            /* Zero padding requested */
            if (ch == '0') {
                while ((ch = *(fmt++))) {
                    if (ch == '\0') goto end;
                    if (ch >= '0' && ch <= '9') {
                        zero_pad = zero_pad * 10 + ch - '0';
                    } else {
                        break;
                    }
                }
            }
            if (ch == 'l') {
                longflag = 1;
                ch = *(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;

                case 'l':
                    longflag = 1;
                    break;

                case 'u':
                case 'd':
                    len = mini_itoa(
                        longflag == 0 ? (unsigned long)va_arg(
                                            va, unsigned int) :
                                        va_arg(va, unsigned long),
                        10, 0, (ch == 'u'), bf, zero_pad);
                    _puts(bf, len, &b);
                    longflag = 0;
                    break;
                case 'x':
                case 'X':
                    len = mini_itoa(
                        longflag == 0 ? (unsigned long)va_arg(
                                            va, unsigned int) :
                                        va_arg(va, unsigned long),
                        16, (ch == 'X'), 1, bf, zero_pad);
                    _puts(bf, len, &b);
                    longflag = 0;
                    break;

                case 'c':
                    _putc((char)(va_arg(va, int)), &b);
                    break;

                case 's':
                    ptr = va_arg(va, char *);
                    _puts(ptr, strlen(ptr), &b);
                    break;

                default:
                    _putc(ch, &b);
                    break;
            }
        }
    }
end:
    return b.pbuffer - b.buffer;
}

int vprintf(const char *fmt, va_list _va)
{
    va_list va;
    va_copy(va, _va);

    int ret;
    char buff[256];

    ret = mini_vsnprintf(buff, 256, fmt, va);

    buff[ret] = '\0';

    sys_write(buff);

    return ret;
}

int printf(const char *fmt, ...)
{
    int ret = 0;
    va_list va;

    va_start(va, fmt);
    ret = vprintf(fmt, va);
    va_end(va);

    return ret;
}

int puts(const char *str)
{
    return printf("%s\n", str);
}

int putchar(int ch)
{
    return printf("%c", ch);
}
