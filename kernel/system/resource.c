#include <os/system.h>

int do_prlimit(pid_t pid,   int resource,  const struct rlimit *new_limit, 
    struct rlimit *old_limit)
{
    debug();
    assert(pid == 0 && resource == RLIMIT_NOFILE);
    static struct rlimit my_rlimit = {.rlim_cur = 1024, .rlim_max = 1024*1024};
    if (new_limit)
        memcpy(&my_rlimit, new_limit, sizeof(struct rlimit));
    if (old_limit)
        memcpy(old_limit, &my_rlimit, sizeof(struct rlimit));
    return 0;
}