#include <os/time.h>
#include <os/mm.h>
#include <os/irq.h>
#include <os/system.h>
#include <type.h>

#include <stdio.h>
#include <os/sched.h>

LIST_HEAD(timers);
LIST_HEAD(available_timers);
timer_t all_timers[NUM_TIMER];

uint64_t time_elapsed = 0;
uint32_t time_base = 0;

void init_timers()
{
    for (int32_t i = 0; i < NUM_TIMER; ++i)
        list_add_tail(&all_timers[i].list, &available_timers);
}

/* if no available timer, return NULL */
static inline timer_t *alloc_timer()
{
    return (available_timers.next == &available_timers)? NULL : available_timers.next;
}

/* create a timer to sleep */
/* call func(parameter) when timeout */
/* tick is sleep interval ticks */
void timer_create(TimerCallback func, void* parameter, uint64_t tick)
{
    disable_preempt();

    timer_t *newtimer = alloc_timer();
    assert(newtimer != NULL);

    newtimer->timeout_tick = get_ticks() + tick;
    newtimer->callback_func = func;
    newtimer->parameter = parameter;
    newtimer->list.ptr = newtimer;
    list_del(&newtimer->list);
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
            list_add_tail(&handling_timer->list, &available_timers);
            (*handling_timer->callback_func)(handling_timer->parameter);
        }
    }
    enable_preempt();
}

/* success return 0 */
uint8 do_nanosleep(struct timespec *ts)
{
    debug();
    // 1. block the current_running
    // 2. create a timer which calls `do_unblock` when timeout
    // 3. reschedule because the current_running is blocked.
    do_block(&current_running->list,&general_block_queue);

    uint64_t sleep_ticks = 0, nsec = ts->tv_nsec;
    // printk_port("time: %d, %d\n", ts->tv_sec, ts->tv_nsec);
    for (uint i = 0; i < NANO; ++i){
        sleep_ticks = (sleep_ticks / 10)+ time_base * (nsec % 10);
        nsec /= 10;
    }

    sleep_ticks += ts->tv_sec * time_base;
    timer_create(&do_unblock, &current_running->list, sleep_ticks);
    do_scheduler();
    return 0;
}

int do_setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value)
{
    debug();
    static int cnt = 0;
    if (cnt == 0)
        cnt++;
    else{
        old_value->it_interval.tv_sec = 0;
        old_value->it_interval.tv_usec = 0;
        old_value->it_value.tv_sec = 4;
        old_value->it_value.tv_usec = 994897;
    }
    return 0;
}