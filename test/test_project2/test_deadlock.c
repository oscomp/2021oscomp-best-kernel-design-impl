#include <stdio.h>
#include <test2.h>
#include <sys/syscall.h>

#define INTERVAL 10000

void deadlock_task(void)
{
    int i, keep_alive = 0;
    int print_location = 7;
    int magic = 1;
    char name[] = "Hero";

    sys_move_cursor(1, print_location);
    printf("> start dead lock\n");
    while (1) {
        if (keep_alive % INTERVAL == 0) {
	    magic = (magic * 21403 + 2531011) & 0xffff;
            sys_move_cursor(1, print_location);
            printf("> I am %s .This is a deadlock %d          \n", name, magic);
	}
        ++keep_alive;
    }
    printf("> [TASK] This task is to test sleep(). (%d)\n", i);
}
