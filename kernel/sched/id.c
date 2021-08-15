#include <os/sched.h>

pid_t do_getpid()
{
    debug();
    return current_running->pid;
}

pid_t do_getppid()
{
    debug();
    if (current_running->parent.parent == NULL){
        log(0, "no parent");
        return 1;
    }
    else{
        log(0, "parent id is %d", current_running->parent.parent->pid);
        return current_running->parent.parent->pid;
    }
}

/* return 0 */
pid_t do_getuid()
{
    debug();
    return 0;
}

/* return 0 */
pid_t do_geteuid()
{
    debug();
    return 0;
}

/* return 0 */
pid_t do_getgid()
{
    debug();
    return 0;
}

/* return 0 */
pid_t do_getegid()
{
    debug();
    return 0;
}

pid_t do_set_tid_address(int *tidptr)
{
    debug();
    return current_running->tid;
}

pid_t do_gettid()
{
    debug();
    return current_running->tid;
}
