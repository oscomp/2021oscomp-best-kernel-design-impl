#include <screen.h>
#include <common.h>
#include <stdio.h>
#include <os/string.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/irq.h>
#include <sbi.h>
#include <pgtable.h>
#include <os/ring_buffer.h>
#include <assert.h>
#include <log.h>

struct ring_buffer stdin_buf;
struct ring_buffer stdout_buf;
struct ring_buffer stderr_buf;
unsigned char screen_buf[RING_BUFFER_SIZE];

/* init all ring buffers my system needs */
void init_screen()
{
    init_ring_buffer(&stdin_buf);
    init_ring_buffer(&stdout_buf);
    init_ring_buffer(&stderr_buf);
}

/* reflush all the buffer */
void screen_reflush()
{
    /* 1. stdout */
    size_t count = read_ring_buffer(&stdout_buf, &screen_buf, RING_BUFFER_SIZE);
    for (uint64_t i = 0; i < count; i++)
        sbi_console_putchar(screen_buf[i]);
    /* 2. stderr */
    count = read_ring_buffer(&stderr_buf, &screen_buf, RING_BUFFER_SIZE);
    for (uint64_t i = 0; i < count; i++)
        sbi_console_putchar(screen_buf[i]);
}

/* just go to assert */
void screen_assert()
{
    assert(0);
}