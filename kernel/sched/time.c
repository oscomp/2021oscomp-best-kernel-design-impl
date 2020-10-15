#include <os/time.h>
#include <os/mm.h>
#include <os/irq.h>
#include <type.h>

LIST_HEAD(timers);

uint64_t time_elapsed = 0;
uint32_t time_base = 0;

void timer_create(TimerCallback func, void* parameter, uint64_t tick)
{
    disable_preempt();

    // TODO:

    enable_preempt();
}

void timer_check()
{
    disable_preempt();

    /* TODO: check all timers
     *  if timeouts, call callback_func and free the timer.
     */

    enable_preempt();
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
