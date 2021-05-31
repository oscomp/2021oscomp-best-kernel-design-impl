
#include <common.h>
#include <screen.h>
#include <stdarg.h>
#include <os/sched.h>
#include <os/smp.h>
#include <os/irq.h>

static unsigned int mini_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

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
        int digit = 0;
        if (unsig) {
            digit = (unsigned long)value % (unsigned)radix;
        } else {
            digit = value % radix;
        }
        *(pbuffer++) =
            (digit < 10 ? '0' + digit :
             (uppercase ? 'A' : 'a') + digit - 10);
        if (unsig) {
            value = (unsigned long) value / (unsigned) radix;
        } else {
            value /= radix;
        }
    } while (value != 0);

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
                    _puts(ptr, mini_strlen(ptr), &b);
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

static int _vprintk(const char* fmt, va_list _va,
                   void (*output)(char*))
{
    va_list va;
    va_copy(va, _va);

    int ret;
    char buff[256];

    ret = mini_vsnprintf(buff, 256, fmt, va);

    buff[ret] = '\0';

    disable_preempt();
    output(buff);
    for (int i = 0; i < ret; ++i) {
        if (buff[i] == '\n') {
            current_running->cursor_y++;
        } else if (buff[i] == '\r') {
            current_running->cursor_x = 1;
        } else {
            current_running->cursor_x++;
        }
    }

    enable_preempt();

    return ret;
}

static int _vprints(const char* fmt, va_list _va,
                   void (*output)(char*))
{
    va_list va;
    va_copy(va, _va);

    int ret;
    char buff[256];

    ret = mini_vsnprintf(buff, 256, fmt, va);

    buff[ret] = '\0';

    disable_preempt();
    output(buff);

    enable_preempt();

    return ret;
}


int vprintk(const char *fmt, va_list _va)
{
    return _vprintk(fmt, _va, port_write);
}

int printk(const char *fmt, ...)
{
    int ret = 0;
    va_list va;

    va_start(va, fmt);
    ret = vprintk(fmt, va);
    va_end(va);

    return ret;
}

int vprints(const char *fmt, va_list _va)
{
    return _vprints(fmt, _va, screen_write);
}

int prints(const char *fmt, ...)
{
    int ret = 0;
    va_list va;

    va_start(va, fmt);
    ret = vprints(fmt, va);
    va_end(va);

    return ret;
}

/* for port output, direct output */

static int _vprintk_port(const char* fmt, va_list _va,
                   void (*output)(char*))
{
    va_list va;
    va_copy(va, _va);

    int ret;
    char buff[256];

    ret = mini_vsnprintf(buff, 256, fmt, va);

    buff[ret] = '\0';

    disable_preempt();
    port_write(buff);

    enable_preempt();

    return ret;
}


int vprintk_port(const char *fmt, va_list _va)
{
    return _vprintk_port(fmt, _va, port_write);
}

uint32_t printk_port(const char *fmt, ...)
{
    uint32_t ret = 0;
    va_list va;

    va_start(va, fmt);
    ret = vprintk_port(fmt, va);
    va_end(va);

    return ret;
}