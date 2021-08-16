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
    // if (signum > NUM_SIG)
    //     return 0;
    // sigaction_t *this_sigaction = &current_running->siginfo[signum - 1].sigaction;
    // if (oldact)
    //     memcpy(oldact, this_sigaction, sizeof(sigaction_t));
    // if (act)
    //     memcpy(this_sigaction, act, sizeof(sigaction_t));
    return 0;
}

void do_rt_sigreturn()
{
    return ;
}