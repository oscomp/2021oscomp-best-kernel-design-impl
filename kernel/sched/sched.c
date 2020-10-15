#include <os/list.h>
#include <os/mm.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/time.h>
#include <os/irq.h>
#include <screen.h>
#include <stdio.h>
#include <assert.h>

pcb_t pcb[NUM_MAX_TASK];
const ptr_t pid0_stack = INIT_KERNEL_STACK + 2*PAGE_SIZE;
pcb_t pid0_pcb = {
    .pid = 0,
    .kernel_sp = (ptr_t)(pid0_stack - PAGE_SIZE),
    .user_sp = (ptr_t)pid0_stack,
    .preempt_count = 0,
    .cursor_x = 1,
    .cursor_y = 1
};

/* all kinds of queues */
LIST_HEAD(ready_queue);


/* current running task PCB */
pcb_t * volatile current_running;

/* global process id */
pid_t process_id = 1;

void do_scheduler(void)
{

    // TODO schedule
    // Modify the current_running pointer.
    pcb_t *previous_running = current_running;

    // put previous running into queue
    if (previous_running->pid && previous_running->status == TASK_RUNNING)
    {        
        previous_running->status = TASK_READY;
        list_add_tail(&previous_running->list,&ready_queue);
    }

    // choose next running
    if (ready_queue.next == &ready_queue)
        current_running = &pid0_pcb;
    else
        current_running = ready_queue.next->pt_pcb;

    current_running->status = TASK_RUNNING;
    list_del(&current_running->list);
    // restore the current_runnint's cursor_x and cursor_y
    // printk("x:%d,y:%d\n\r",current_running->cursor_x,current_running->cursor_y);
    vt100_move_cursor(current_running->cursor_x,
                      current_running->cursor_y);
    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;
    // TODO: switch_to current_running
    
    // vt100_move_cursor(1,35);
    // printk("switch from id[ %d ] to id[ %d ].\n\r",previous_running->pid,current_running->pid);
    // printk("switch_to entrypoint: %x*****\n\r",&switch_to);
    switch_to(previous_running,current_running);
    
}

void do_sleep(uint32_t sleep_time)
{
    // TODO: sleep(seconds)
    // note: you can assume: 1 second = `timebase` ticks
    // 1. block the current_running
    // 2. create a timer which calls `do_unblock` when timeout
    // 3. reschedule because the current_running is blocked.
}

void do_block(list_node_t *pcb_node, list_head *queue)
{
    // TODO: block the pcb task into the block queue
    pcb_t *pcb = pcb_node->pt_pcb;
    
    pcb->status = TASK_BLOCKED;
    list_add_tail(pcb_node,queue);   

    do_scheduler();
    do_mutex_lock_acquire(&mutex_lock);
}

void do_unblock(list_node_t *pcb_node)
{
    // TODO: unblock the `pcb` from the block queue    
    list_del(pcb_node);
    pcb_t *pcb = pcb_node->pt_pcb;
    pcb->status = TASK_READY;
    list_add(pcb_node,&ready_queue);
}
