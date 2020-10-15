#include <test2.h>
#include <mthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/syscall.h>

static int is_init = FALSE;
static char blank[] = {"                                             "};

/* if you want to use spin lock, you need define SPIN_LOCK */
//  #define SPIN_LOCK
static mthread_spinlock_t spin_lock;

/* if you want to use mutex lock, you need define MUTEX_LOCK */
#define MUTEX_LOCK
static mthread_mutex_t mutex_lock;

void lock2_task1(void)
{
        int print_location = 3;
        while (1)
        {
                int i;
                if (!is_init)
                {

#ifdef SPIN_LOCK
                        mthread_spin_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                        mthread_mutex_init(&mutex_lock);
#endif
                        is_init = TRUE;
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Applying for a lock.\n");

#ifdef SPIN_LOCK
                mthread_spin_lock(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mthread_mutex_lock(&mutex_lock);
#endif

                for (i = 0; i < 20; i++)
                {
                        sys_move_cursor(1, print_location);
                        printf("> [TASK] Has acquired lock and running.(%d)\n", i);
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Has acquired lock and exited.\n");

#ifdef SPIN_LOCK
                mthread_spin_unlock(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mthread_mutex_unlock(&mutex_lock);
#endif
        }
}

void lock2_task2(void)
{
        int print_location = 4;
        while (1)
        {
                int i;
                if (!is_init)
                {

#ifdef SPIN_LOCK
                        mthread_spin_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                        mthread_mutex_init(&mutex_lock);
#endif
                        is_init = TRUE;
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Applying for a lock.\n");

#ifdef SPIN_LOCK
                mthread_spin_lock(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mthread_mutex_lock(&mutex_lock);
#endif

                for (i = 0; i < 20; i++)
                {
                        sys_move_cursor(1, print_location);
                        printf("> [TASK] Has acquired lock and running.(%d)\n", i);
                }

                sys_move_cursor(1, print_location);
                printf("%s", blank);

                sys_move_cursor(1, print_location);
                printf("> [TASK] Has acquired lock and exited.\n");

#ifdef SPIN_LOCK
                mthread_spin_unlock(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                mthread_mutex_unlock(&mutex_lock);
#endif
        }
}
