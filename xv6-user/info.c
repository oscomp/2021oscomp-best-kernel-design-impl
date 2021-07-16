#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/sysinfo.h"
#include "xv6-user/user.h"

int main()
{
    struct sysinfo info;
    if (sysinfo(&info) < 0) {
        printf("sysinfo fail!\n");
    } else {
        printf("total memory: %d KB\n", info.totalram >> 10);
        printf("free memory: %d KB\n", info.freeram >> 10);
        printf("process amount: %d\n", info.procs);
        printf("ticks since boot: %d\n", info.uptime);
    }
    exit(0);
}
