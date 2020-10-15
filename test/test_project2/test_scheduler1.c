#include <stdio.h>
#include <test2.h>
#include <os/sched.h>
#include <screen.h>

static char blank[] = {"                   "};
static char plane1[] = {"    ___         _  "};
static char plane2[] = {"| __\\_\\______/_| "};
static char plane3[] = {"<[___\\_\\_______| "};
static char plane4[] = {"|  o'o             "};

void printk_task1(void)
{
    int i;
    int print_location = 1;

    for (i = 0;; i++)
    {
        vt100_move_cursor(1, print_location);
        printk("> [TASK] This task is to test scheduler. (%d)\n", i);
        do_scheduler();
    }
}

void printk_task2(void)
{
    int i;
    int print_location = 2;

    for (i = 0;; i++)
    {
        vt100_move_cursor(1, print_location);
        printk("> [TASK] This task is to test scheduler. (%d)", i);
        do_scheduler();
    }
}

void drawing_task1(void)
{
    int i, j = 22;

    while (1)
    {
        for (i = 60; i > 0; i--)
        {
            /* move */
            vt100_move_cursor(i, j + 0);
            printk("%s", plane1);

            vt100_move_cursor(i, j + 1);
            printk("%s", plane2);

            vt100_move_cursor(i, j + 2);
            printk("%s", plane3);

            vt100_move_cursor(i, j + 3);
            printk("%s", plane4);
        }
        do_scheduler();

        vt100_move_cursor(1, j + 0);
        printk("%s", blank);

        vt100_move_cursor(1, j + 1);
        printk("%s", blank);

        vt100_move_cursor(1, j + 2);
        printk("%s", blank);

        vt100_move_cursor(1, j + 3);
        printk("%s", blank);
    }
}
