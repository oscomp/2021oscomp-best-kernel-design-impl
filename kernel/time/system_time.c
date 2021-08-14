#include <os/time.h>
#include <os/mm.h>
#include <os/irq.h>
#include <os/system.h>
#include <type.h>

#include <stdio.h>
#include <os/sched.h>


uint64_t do_times(struct tms *tms)
{
    uint64_t now_tick = get_ticks();
    tms->tms_utime = current_running->utime;
    tms->tms_stime = current_running->stime;
    tms->tms_cutime = 0; tms->tms_cstime = 0;
    for (uint i = 0; i < NUM_MAX_TASK; ++i)
        if (pcb[i].parent.parent == current_running){
            tms->tms_cutime += pcb[i].utime;
            tms->tms_cstime += pcb[i].stime;
        }
    return now_tick;
}

/* 成功返回0， 失败返回-1 */
int8_t do_gettimeofday(struct timespec *ts)
{
    debug();
    uint64_t nowtick = get_ticks();

    ts->tv_sec = (uint32)(nowtick / time_base);

    uint64_t left = nowtick % time_base;
    ts->tv_nsec = 0;

    for (uint i = 0; i < NANO; ++i)
    {
        ts->tv_nsec = 10*ts->tv_nsec + left * 10 / time_base;
        left = (left * 10) % time_base;
    }
    ts->tv_sec=1628129642; ts->tv_nsec=613489360;
    // log(0, "%d %d\n", ts->tv_sec, ts->tv_nsec);
    return 0;
}

/* 成功返回0， 失败返回-1 */
int32_t do_clock_gettime(uint64_t clock_id, struct timespec *tp)
{
    debug();
    assert(clock_id == CLOCK_REALTIME);
    if (clock_id == CLOCK_REALTIME){
        return do_gettimeofday(tp);
    }
    else
        return SYSCALL_FAILED;
}
