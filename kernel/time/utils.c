#include <os/time.h>
#include <os/mm.h>
#include <os/irq.h>
#include <os/system.h>
#include <type.h>

#include <stdio.h>
#include <os/sched.h>

/* start and end counter */
/* for cpu compute SYS cpu time and USER cpu time */
/* start: enter kernel, end: exit kernel */
static uint64_t last_time;
void kernel_time_count()
{
    uint64_t now_tick = get_ticks();
    current_running->stime += now_tick - last_time;
    last_time = now_tick;
}
void user_time_count()
{
    uint64_t now_tick = get_ticks();
    current_running->utime += now_tick - last_time;
    last_time = now_tick;
}


uint64_t get_ticks()
{
    __asm__ __volatile__(
        "rdtime %0"
        : "=r"(time_elapsed));
    return time_elapsed;
}

uint64_t get_timer()
{
    return get_ticks() / time_base;
}

uint64_t get_time_base()
{
    return time_base;
}

void latency(uint64_t time)
{
    uint64_t begin_time = get_timer();

    while (get_timer() - begin_time < time);
    return;
}

void get_regular_time_from_spec(struct regular_time *mytp, struct timespec *tp)
{
    if (!mytp) return ;
    mytp->nano_seconds = tp->tv_nsec;

    time_t sec = tp->tv_sec;

    mytp->seconds = sec % SECONDS_PER_MIN;
    sec -= mytp->seconds;

    mytp->min = (sec / SECONDS_PER_MIN) % MIN_PER_HOUR;
    sec -= mytp->min * SECONDS_PER_MIN;

    mytp->hour = (sec / MIN_PER_HOUR) % HOUR_PER_DAY;
    sec -= mytp->hour * MIN_PER_HOUR * SECONDS_PER_MIN;

    return ;
}
