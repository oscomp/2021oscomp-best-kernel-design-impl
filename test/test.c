#include "test.h"

/* [TASK1] [TASK3] task group to test do_scheduler() */
// do_scheduler() annotations are required for non-robbed scheduling
struct task_info task2_1 = {(ptr_t)&printk_task1, KERNEL_THREAD};
struct task_info task2_2 = {(ptr_t)&printk_task2, KERNEL_THREAD};
struct task_info task2_3 = {(ptr_t)&drawing_task1, KERNEL_THREAD};
struct task_info *sched1_tasks[16] = {&task2_1, &task2_2, &task2_3};
int num_sched1_tasks = 3;

/* [TASK2] task group to test lock */
// test_lock1.c : Kernel space lock test
// test_lock2.c : User space lock test
struct task_info task2_4 = {(ptr_t)&lock_task1, KERNEL_THREAD};
struct task_info task2_5 = {(ptr_t)&lock_task2, KERNEL_THREAD};
struct task_info *lock_tasks[16] = {&task2_4, &task2_5};
int num_lock_tasks = 2;

/* [TASK4] task group to test interrupt */
// When the task is running, please implement the following system call :
// (1) sys_sleep()
// (2) sys_move_cursor()
// (3) sys_write()
struct task_info task2_6 = {(ptr_t)&sleep_task, USER_PROCESS};
struct task_info task2_7 = {(ptr_t)&timer_task, USER_PROCESS};
struct task_info *timer_tasks[16] = {&task2_6, &task2_7};
int num_timer_tasks = 2;

struct task_info task2_8 = {(ptr_t)&printf_task1, USER_PROCESS};
struct task_info task2_9 = {(ptr_t)&printf_task2, USER_PROCESS};
struct task_info task2_10 = {(ptr_t)&drawing_task2, USER_PROCESS};
struct task_info task2_11 = {(ptr_t)&deadlock_task, USER_PROCESS};
struct task_info *sched2_tasks[16] = {&task2_8, &task2_9, &task2_10, &task2_11};
int num_sched2_tasks = 4;

struct task_info task2_12 = {(ptr_t)&lock2_task1, USER_THREAD};
struct task_info task2_13 = {(ptr_t)&lock2_task2, USER_THREAD};
struct task_info *lock2_tasks[16] = {&task2_12, &task2_13};
int num_lock2_tasks = 2;

