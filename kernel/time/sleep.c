#include <os/time.h>
#include <os/sched.h>

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
