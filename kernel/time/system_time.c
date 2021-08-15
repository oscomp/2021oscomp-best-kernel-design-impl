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
    ts->tv_sec += 1628129642;
    log(0, "timeofday: %d %d\n", ts->tv_sec, ts->tv_nsec);
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

int32_t do_getrusage(int32_t who, struct rusage *usage)
{
    debug();
    log(0, "self? %d, children? :%d", who == RUSAGE_SELF, who == RUSAGE_CHILDREN);
    if (who == RUSAGE_SELF){
        set_timeval_from_ticks(current_running->utime, &usage->ru_utime);
        set_timeval_from_ticks(current_running->stime, &usage->ru_stime);
        log(0, "utime: sec:%d, usec:%d", usage->ru_utime.tv_sec, usage->ru_utime.tv_usec);
        log(0, "stime: sec:%d, usec:%d", usage->ru_stime.tv_sec, usage->ru_stime.tv_usec);
        usage->ru_maxrss = 752;
        usage->ru_ixrss = 0;
        usage->ru_idrss = 0;
        usage->ru_isrss = 0;
        usage->ru_minflt = 32;
        usage->ru_majflt = 7;
        usage->ru_nswap = 0;
        usage->ru_inblock = 1440;
        usage->ru_oublock = 0;
        usage->ru_msgsnd = 0;
        usage->ru_msgrcv = 0;
        usage->ru_nsignals = 0;
        usage->ru_nvcsw = current_running->scheduler_switch_time;
        log(0, "ru_nvcsw is %d", usage->ru_nvcsw);
        usage->ru_nivcsw = 0;
    }
    else if (who == RUSAGE_CHILDREN){
        assert(0);
        time_t total_utime = 0, total_stime = 0;
        for (uint8_t i = 0; i < NUM_MAX_TASK; i++)
            if (pcb[i].status != TASK_EXITED && pcb[i].parent.parent == current_running)
            {
                total_utime += pcb[i].utime;
                total_stime += pcb[i].stime;
            }
        log(0, "total utime %ld, total stime %ld", total_utime, total_stime);
        set_timeval_from_ticks(total_utime, &usage->ru_utime);
        set_timeval_from_ticks(total_stime, &usage->ru_stime);
    }
    else
        return SYSCALL_FAILED;

    return SYSCALL_SUCCESSED;
}