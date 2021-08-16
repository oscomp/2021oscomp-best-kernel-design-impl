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
    for (uint32_t i = 0; i < NUM_TIMER; ++i){
        all_timers[i].timer_id = i;
        list_add_tail(&all_timers[i].list, &available_timers);
    }
}

/* if no available timer, return NULL */
timer_t *alloc_timer()
{
    if (available_timers.next == &available_timers)
        return NULL;
    else{
        timer_t *ret = available_timers.next;
        list_del(&ret->list);
        return ret;
    }
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
    enable_timer(newtimer);

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
            disable_timer(handling_timer);
            free_timer(handling_timer);
            (*handling_timer->callback_func)(handling_timer->parameter);
        }
    }
    enable_preempt();
}

static void iteration_test_timer()
{
    printk_port("waiting\n");
    timer_t *mytimer = alloc_timer();
    assert(mytimer);
    mytimer->timeout_tick = get_ticks() + 50000000;
    mytimer->callback_func = &iteration_test_timer;
    enable_timer(mytimer);
}

void do_set_test_timer()
{
    timer_t *newtimer = alloc_timer();
    assert(newtimer != NULL);

    newtimer->timeout_tick = get_ticks() + 50000000;
    newtimer->callback_func = &iteration_test_timer;
    newtimer->parameter = 0;
    enable_timer(newtimer);    
}