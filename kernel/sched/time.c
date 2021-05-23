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
    // DEBUG: time_check
    // vt100_move_cursor(1,16);
    list_node_t *pointer = timers.next;
    uint64_t nowtick = get_ticks();
    // check all timers
    while (pointer != &timers)
    {
        timer_t *handling_timer = pointer->ptr;
        timer_t *temp = pointer->next;
            // vt100_move_cursor(1,10);
        if (handling_timer->timeout_tick <= nowtick)
        {            
            list_del(pointer);
            // DEBUG: time_check
            // list_node_t * a = (list_node_t *)(handling_timer->parameter);
            // pcb_t *b = a->ptr;
            // printk("unblock id[ %d ];\n",b->pid);
            (*handling_timer->callback_func)(handling_timer->parameter);
        }
        // DEBUG: time_check
        // else
        // {
        //     list_node_t * a = (list_node_t *)(handling_timer->parameter);
        //     pcb_t *b = a->ptr;
        //     printk("id[ %d ] blocked. now = %d, sleeptime = %d;\n",b->pid,nowtick,handling_timer->timeout_tick);
        // }
        pointer = temp;
    }
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
