#include <os/system.h>
#include <os/mm.h>
#include <os/sched.h>
#include <os/time.h>

/* FOR NOW return */
void do_syslog()
{
    debug();
    return ;
}

/* returns certain statistics on memory and swap usage, 
    as well as the load average */
/* success return 0, else return -1 */
int32_t do_sysinfo(struct sysinfo *info)
{
    debug();
    info->uptime = get_timer();

    info->loads[0] = 0; info->loads[1] = 0; info->loads[2] = 0;

    info->totalram= FREEMEM_TOP - FREEMEM;

    info->freeram = 0;
    for (list_node_t *i = freePageList.next; i != &freePageList; i = i->next)
        info->freeram += NORMAL_PAGE_SIZE;
    info->freeram += FREEMEM_TOP - memCurr;

    info->sharedram = 0;

    info->bufferram = 0x10000;

    info->totalswap = 0;

    info->freeswap = 0;

    info->procs = 0;
    for (uint8_t i = 0; i < NUM_MAX_TASK; i++)
        if (pcb[i].status != TASK_ZOMBIE && pcb[i].status != TASK_EXITED)
            info->procs++;

    info->totalhigh = 0;

    info->freehigh = 0;

    info->mem_unit = 1;

    return 0;
}

static void set_timeval_from_ticks(time_t time, struct timeval *tv)
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

int32_t do_getrusage(int32_t who, struct rusage *usage)
{
    debug();
    log(0, "self? %d, children? :%d", who == RUSAGE_SELF, who == RUSAGE_CHILDREN);
    if (who == RUSAGE_SELF){
        log(0, "utime %ld, stime %ld", current_running->utime, current_running->stime);
        set_timeval_from_ticks(current_running->utime, &usage->ru_utime);
        set_timeval_from_ticks(current_running->stime, &usage->ru_stime);
    }
    else if (who == RUSAGE_CHILDREN){
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