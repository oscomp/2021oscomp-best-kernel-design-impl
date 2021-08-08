#include <os/sched.h>

/* 通过how来设置procmask */
/* success return 0, fail return -1 */
int32_t do_rt_sigprocmask(int32_t how, const sigset_t *restrict set, sigset_t *restrict oldset, size_t sigsetsize)
{
    debug();
    return 0;
}

/* success return 0, fail return -1 */
int32_t do_rt_sigaction(int32_t signum, struct sigaction *act, struct sigaction *oldact, size_t sigsetsize)
{
    debug();
    return 0;
}

void do_rt_sigreturn()
{
    return ;
}