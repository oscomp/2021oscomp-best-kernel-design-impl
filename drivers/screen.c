#include <screen.h>
#include <common.h>
#include <stdio.h>
#include <os/string.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/irq.h>
#include <sbi.h>
#include <pgtable.h>

#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   50

/* screen buffer */
char new_screen[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};
char old_screen[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};

/* cursor position */
void vt100_move_cursor(int x, int y)
{
    // \033[y;xH
    disable_preempt();
    printk("%c[%d;%dH", 27, y, x);
    current_running->cursor_x = x;
    current_running->cursor_y = y;
    enable_preempt();
}

/* clear screen */
static void vt100_clear()
{
    // \033[2J
    printk("%c[2J", 27);
}

/* hidden cursor */
static void vt100_hidden_cursor()
{
    // \033[?25l
    printk("%c[?25l", 27);
}

/* write a char */
static void screen_write_ch(char ch)
{
    if (ch == '\n')
    {
        current_running->cursor_x = 1;
        current_running->cursor_y++;
    }
    else if (ch == '\b')
    {
        current_running->cursor_x--;
        new_screen[(current_running->cursor_y - 1) * SCREEN_WIDTH + (current_running->cursor_x - 1)] = ' ';
    }
    else
    {
        new_screen[(current_running->cursor_y - 1) * SCREEN_WIDTH + (current_running->cursor_x - 1)] = ch;
        current_running->cursor_x++;
    }
}

void init_screen(void)
{
    #ifndef K210
    // vt100_hidden_cursor();
    // vt100_clear();
    #endif
}

void screen_clear(void)
{
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            new_screen[i * SCREEN_WIDTH + j] = ' ';
        }
    }
    current_running->cursor_x = 1;
    current_running->cursor_y = 1;
    #ifndef K210
    // screen_reflush();
    #endif
}

void screen_move_cursor(int x, int y)
{
    current_running->cursor_x = x;
    current_running->cursor_y = y;
}

void screen_write(char *buff)
{
    // static int32_t cnt = 0;
    // sbi_console_putchar('A'+ cnt % 10); cnt++;
    uint32_t i = 0;
    uint32_t l = strlen(buff);

    for (i = 0; i < l; i++)
    {
        screen_write_ch(buff[i]);
    }
}

/*
 * This function is used to print the serial port when the clock
 * interrupt is triggered. However, we need to pay attention to
 * the fact that in order to speed up printing, we only refresh
 * the characters that have been modified since this time.
 */
void screen_reflush(void)
{
    uint32_t i, j;
    uint32_t prev_cursor_x = current_running->cursor_x, prev_cursor_y = current_running->cursor_y;

    /* here to reflush screen buffer to serial port */
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            /* We only print the data of the modified location. */
            if (new_screen[i * SCREEN_WIDTH + j] != old_screen[i * SCREEN_WIDTH + j])
            {
                // sbi_console_putchar('6');
                vt100_move_cursor(j + 1, i + 1);
                port_write_ch(new_screen[i * SCREEN_WIDTH + j]);
                old_screen[i * SCREEN_WIDTH + j] = new_screen[i * SCREEN_WIDTH + j];
            }
            // else
            //     sbi_console_putchar('7');
        }
    }

    /* recover cursor position */
    vt100_move_cursor(prev_cursor_x, prev_cursor_y);
    current_running->cursor_x = prev_cursor_x; current_running->cursor_y = prev_cursor_y;
}
