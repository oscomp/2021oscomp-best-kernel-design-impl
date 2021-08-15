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