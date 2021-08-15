#include <os/system.h>

int do_prlimit(pid_t pid,   int resource,  const struct rlimit *new_limit, 
    struct rlimit *old_limit)
{
    debug();
    return 0;
}