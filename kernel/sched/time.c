#include <os/time.h>
#include <os/mm.h>
#include <os/irq.h>
#include <type.h>

/*for debug*/
#include <stdio.h>
#include <os/sched.h>
/*         */


LIST_HEAD(timers);

uint64_t time_elapsed = 0;
uint32_t time_base = 0;

/* create a timer to sleep */
/* call func(parameter) when timeout */
/* tick is sleep interval ticks */
void timer_create(TimerCallback func, void* parameter, uint64_t tick)
{
    disable_preempt();

    timer_t *newtimer = (timer_t *)kmalloc(sizeof(timer_t));

    newtimer->timeout_tick = get_ticks() + tick;
    newtimer->callback_func = func;
    newtimer->parameter = parameter;
    newtimer->list.ptr = newtimer;
    list_add_tail(&newtimer->list,&timers);

    enable_preempt();
}

void timer_check()
{
    disable_preempt();
    timer_t *handling_timer, *next_timer;
    uint64_t nowtick = get_ticks();
    // check all timers
    list_for_each_entry_safe(handling_timer, next_timer, &timers, list)
    {
        if (handling_timer->timeout_tick < nowtick)
        {            
            list_del(&handling_timer->list);
            (*handling_timer->callback_func)(handling_timer->parameter);
        }
    }
    enable_preempt();
}

int8_t do_gettimeofday(struct timespec *ts)
{
    uint64_t nowtick = get_ticks();

    ts->tv_sec = (uint32)(nowtick / time_base);

    uint64_t left = nowtick % time_base;
    ts->tv_nsec = 0;

    for (uint i = 0; i < NANO; ++i)
    {
        ts->tv_nsec = 10*ts->tv_nsec + left * 10 / time_base;
        left = (left * 10) % time_base;
    }
    return 0;
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
