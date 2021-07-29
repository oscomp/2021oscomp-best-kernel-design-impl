#include "types.h"
#include "fs/stat.h"
#include "sysinfo.h"
#include "user.h"

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
