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

void scheduler_switch_time_count()
{
    if (!current_running->exec)
        current_running->scheduler_switch_time++;
}

void yield_switch_time_count()
{
    current_running->yield_switch_time++;
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

uint64_t timespec_to_tick(struct timespec *ts)
{
    uint64_t nsec = ts->tv_nsec, nticks = 0;
    for (uint8_t i = 0; i < NANO; i++){
        nticks += time_base * (nsec % 10);
        nticks /= 10;
        nsec /= 10;
    }
    return nticks + ts->tv_sec * time_base;
}

uint64_t timeval_to_tick(struct timeval *tv)
{
    uint64_t usec = tv->tv_usec, uticks = 0;
    for (uint8_t i = 0; i < MICRO; i++){
        uticks += time_base * (usec % 10);
        uticks /= 10;
        usec /= 10;
    }
    return uticks + tv->tv_sec * time_base;
}

void tick_to_timespec(time_t time, struct timespec *ts)
{
    ts->tv_sec = time / time_base;
    uint64_t left = time % time_base;
    ts->tv_nsec = 0;
    for (uint i = 0; i < NANO; ++i)
    {
        ts->tv_nsec = 10*ts->tv_nsec + left * 10 / time_base;
        left = (left * 10) % time_base;
    }
}

void tick_to_timeval(time_t time, struct timeval *tv)
{
    tv->tv_sec = time / time_base; /* compute second */

    uint64_t left = time % time_base; /* compute micro seconds */
    tv->tv_usec = 0;
    for (uint i = 0; i < MICRO; ++i)
    {
        tv->tv_usec = 10*tv->tv_usec + left * 10 / time_base;
        left = (left * 10) % time_base;
    }
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
