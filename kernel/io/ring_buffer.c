#include <os/sched.h>
#include <os/ring_buffer.h>
#include <os/time.h>

/* wait until ring buffer is not empty or timeout */
/* wait success return 0, timeout return 1, error return -1 */
int wait_ring_buffer_read(struct ring_buffer *rbuf, time_t final_ticks)
{
    while (ring_buffer_empty(rbuf)){
        time_t now_ticks = get_ticks();
        if (final_ticks < now_ticks)
            return 1;
        do_scheduler();
    }
    return 0;
}

/* wait until ring buffer is not full or timeout */
/* wait success return 0, timeout return 1, error return -1 */
int wait_ring_buffer_write(struct ring_buffer *rbuf, time_t final_ticks)
{
    while (ring_buffer_full(rbuf)){
        time_t now_ticks = get_ticks();
        if (final_ticks < now_ticks)
            return 1;
        do_scheduler();
    }
    return 0;
}