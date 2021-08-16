#include <os/time.h>
#include <os/sched.h>
#include <os/system.h>
#include <os/mm.h>

/* send SIGALARM and reset the timer */
static void handle_itimer(struct itimer_param* param)
{
    assert(param);
    /* No send signal FOR NOW */
    pcb_t *pcb = param->mypcb;
    time_t nowtick = get_ticks();
    if (param->interval == 0){
        kfree(param);
        pcb->itimer.parameter = 0;
        disable_timer(&pcb->itimer);
    }
    else
        pcb->itimer.timeout_tick = nowtick + param->interval;
}

/* set iteration timer for myproc */
/* new_value contains time value */
/* put old time value into old_value */
int do_setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value)
{
    debug();
    assert(which == ITIMER_REAL);
    /* use paremter as tick interval */
    if (!new_value || !old_value)
        return -EFAULT;
    itimer_param_t *param = (itimer_param_t*)(current_running->itimer.parameter);
    /* old_value */
    time_t old_inteval_tick = 0;
    int64_t old_value_tick = 0;
    time_t nowtick = get_ticks();
    if (param){
        old_inteval_tick = param->interval;
        old_value_tick = current_running->itimer.timeout_tick - nowtick;
        if (old_value_tick < 0)
            old_value_tick = 0;
    }
    tick_to_timeval(old_inteval_tick, &(old_value->it_interval));
    tick_to_timeval(old_value_tick, &(old_value->it_value));
    /* new_value */
    time_t interval = timeval_to_tick(&new_value->it_interval);
    time_t value = timeval_to_tick(&new_value->it_value);
    if (value != 0){
        current_running->itimer.callback_func = &handle_itimer;
        current_running->itimer.timeout_tick = nowtick + value; /* until next timeout */
        if (!param){
            param = kalloc();
            param->mypcb = current_running;
            current_running->itimer.parameter = param;
        }
        param->interval = interval;
        enable_timer(&current_running->itimer);
    }
    else{
        disable_timer(&current_running->itimer);
        if (param){
            kfree(param);
            current_running->itimer.parameter = 0;
        }
    }
    return 0;
}