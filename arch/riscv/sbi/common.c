#include <common.h>
#include <os/string.h>
#include <sbi.h>

void port_write_ch(char ch)
{
    sbi_console_putchar((int)ch);
}

void port_write(char *str)
{
    sbi_console_putstr(str);
}
