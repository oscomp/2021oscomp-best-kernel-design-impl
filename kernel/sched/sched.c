#include <os/list.h>
#include <os/mm.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/time.h>
#include <os/irq.h>
#include <screen.h>
#include <stdio.h>
#include <assert.h>
#include <sbi.h>
#include <pgtable.h>
#include <csr.h>
#include <qemu.h>
#include <os/elf.h>
#include <os/fat32.h>

#define TOO_LARGE_PRIORITY 10000000
#define true 1
#define false 0

pcb_t pcb[NUM_MAX_TASK];
const ptr_t pid0_stack = INIT_KERNEL_STACK + 2*PAGE_SIZE;
const ptr_t pid0_stack2 = INIT_KERNEL_STACK + 4*PAGE_SIZE;
pcb_t pid0_pcb = {
    .pid = 0,
    .kernel_sp = (ptr_t)(pid0_stack - PAGE_SIZE),
    .user_sp = (ptr_t)(pid0_stack),
    .status = TASK_RUNNING,
    .preempt_count = 0,
    .priority = -1,
    .cursor_x = 1,
    .cursor_y = 1
};

// pcb_t pid0_pcbs[NR_CPUS];
pcb_t pid0_pcb2 = {
    .pid = 0, //modified
    .kernel_sp = (ptr_t)(pid0_stack2 - PAGE_SIZE),
    .user_sp = (ptr_t)(pid0_stack2),
    .status = TASK_RUNNING,
    .preempt_count = 0,
    .priority = -1,
    .cursor_x = 1,
    .cursor_y = 1
};

/* all kinds of queues */
LIST_HEAD(ready_queue);
LIST_HEAD(general_block_queue);
LIST_HEAD(available_queue);
LIST_HEAD(fileop_queue);

/* current running task PCB */
pcb_t * volatile current_running;

/* global process id */
pid_t process_id = 1;

/* do scheduler time counter */
int FORMER_TICKS_COUNTER;
int LATTER_TICKS_COUNTER;

void do_scheduler(void)
{
    // static int32_t cnt = 0;
    // prints("%d\n", cnt++);
    // Modify the current_running pointer.
    pcb_t *previous_running = current_running;

    // put previous running into queue
    if (previous_running->status == TASK_RUNNING)
    {        
        previous_running->status = TASK_READY;
        previous_running->temp_priority = previous_running->priority;
        list_add_tail(&previous_running->list,&ready_queue);
    }
    /* kernel time count */
    kernel_time_count(); // kernel trans?
    // choose next running
    /* priority schedule*/
    current_running = NULL;
    /*priority schedule*/
    list_node_t *pt_readyqueue = ready_queue.next;
    pcb_t *pt_pcb;

    int32_t max_priority = -1;

    while (pt_readyqueue != &ready_queue)
    {
        pt_pcb = pt_readyqueue->ptr;
        uint64_t mask = pt_pcb->mask, cpuID = 0;
        if ( ((mask >> cpuID) & 0x1) != 0 && max_priority < pt_pcb->temp_priority)
        {
            max_priority = pt_pcb->temp_priority;
            current_running = pt_pcb;
        }
        pt_readyqueue = pt_readyqueue->next;
    }
    if (current_running != NULL){
        current_running->status = TASK_RUNNING;
        list_del(&current_running->list);
    }

    // regs_context_t *pt_regs =
    //     (regs_context_t *)(0xffffffff80505000 - sizeof(regs_context_t));
    // prints("sepc: %lx \n", pt_regs->sepc);
    // prints("sstatus: %lx \n", pt_regs->sstatus);
    // prints("ra: %lx \n", pt_regs->regs[1]);
    // prints("sp: %lx \n", current_running->user_sp);
    // prints("gp: %lx \n", pt_regs->regs[3]);
    // prints("tp: %lx \n", pt_regs->regs[4]);
    // prints("scause:%lx\n", pt_regs->scause);
    // prints("sbadaddr: %lx\n", pt_regs->sbadaddr);
    // screen_move_cursor(1,1);

    // we have delete current running, now modify temp_priority
    pt_readyqueue = ready_queue.next;
    while (pt_readyqueue != &ready_queue)
    {
        pt_pcb = pt_readyqueue->ptr;
        pt_pcb->temp_priority++;
        pt_readyqueue = pt_readyqueue->next;
    }

    if (!current_running)
        current_running = &pid0_pcb;
    /* end schedule */
    // #ifndef K210
    // vt100_move_cursor(current_running->cursor_x,
    //                   current_running->cursor_y);
    // #endif
    switch_to(previous_running,current_running);
}


/* copy context */
/* kernel_stack and user_stack are stack top addr */
static void copy_parent_stacks_and_set_sp(pcb_t *pcb_underinit, uint64_t kernel_stack_top, uint64_t user_stack_top)
{
    uint64_t ker_stack_size, user_stack_size;
    /* kernel_sp is just at user context, but maybe not equal to register sp */
    ker_stack_size = KERNEL_STACK_SIZE - PAGE_OFFSET(current_running->kernel_sp);
    user_stack_size = current_running->user_stack_base + USER_STACK_INIT_SIZE - current_running->user_sp;
    
    pcb_underinit->kernel_sp = kernel_stack_top - ker_stack_size; /* for user context */
    pcb_underinit->user_sp = user_stack_top; /* no need to be aligned with user_sp */
    memcpy(pcb_underinit->kernel_sp, current_running->kernel_sp, ker_stack_size);
    memcpy(pcb_underinit->user_sp - 8, current_running->user_sp - 8, 8); /* copy tp, very essential */

    // copy fd
    for (int i = 0; i < NUM_FD; ++i)
        memcpy(&pcb_underinit->fd[i], &current_running->fd[i], sizeof(fd_t));
}

/* clone a child thread for current thread */
/* FOR NOW, use tls as entry point */
/* stack : ADDR OF CHILD STACK POINT */
/* success: child pid; fail: -1 */
pid_t do_clone(uint32_t flag, uint64_t stack, pid_t ptid, void *tls, pid_t ctid)
{
    debug();
    for (uint i = 1; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status == TASK_EXITED)
        {
            pcb_t *pcb_underinit = &pcb[i];
            init_pcb_default(pcb_underinit, USER_THREAD);
            /* set current_running pcb */
            current_running->spawn_num++;

            pcb_underinit->parent.parent = current_running;
            pcb_underinit->parent.flag = flag;

            /* init child pcb */
            ptr_t kernel_stack_top = allocPage() + NORMAL_PAGE_SIZE;  
            // if stack = NULL, automatically set up      
            ptr_t user_stack_top = (stack)? stack : USER_STACK_ADDR + 3 * current_running->spawn_num * NORMAL_PAGE_SIZE;
            for (uint64_t i = 0; i < USER_STACK_INIT_SIZE / NORMAL_PAGE_SIZE; i++)
                alloc_page_helper(user_stack_top - (i+1)*NORMAL_PAGE_SIZE, current_running->pgdir, _PAGE_READ|_PAGE_WRITE);

            // pgdir
            pcb_underinit->pgdir = current_running->pgdir;
           
            // prepare stack
            copy_parent_stacks_and_set_sp(pcb_underinit, kernel_stack_top, user_stack_top);

            // return 0 if child
            regs_context_t *pt_regs =
                (regs_context_t *)(kernel_stack_top - sizeof(regs_context_t));
            // pt_regs->regs[4] = pcb_underinit;
            pt_regs->regs[10] = 0;

            // prepare switch context
            pcb_underinit->kernel_sp -= sizeof(switchto_context_t);
            switchto_context_t *switch_to_reg = 
                (switchto_context_t *)(pcb_underinit->kernel_sp);
            // kernel stack should be different
            switch_to_reg->regs[0] = &ret_from_exception;

            // add to ready queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);
            return pcb_underinit->pid;
        }
    return -1;
}

pid_t do_wait4(pid_t pid, uint16_t *status, int32_t options)
{
    debug();
    uint64_t status_ker_va = NULL;
    if (status) status_ker_va = get_kva_of(status,current_running->pgdir);
    pid_t ret = -1;
    for (uint i = 1; i < NUM_MAX_TASK; ++i)
    {
        if (pcb[i].parent.parent == current_running && (pid == -1 || pid == pcb[i].pid)){
            // confirm pid
            if (pcb[i].status == TASK_READY){
                current_running->status = TASK_BLOCKED;
                list_add_tail(&current_running->list, &pcb[i].wait_list);
                ret = pcb[i].pid;
                do_scheduler();
                if (status_ker_va) WEXITSTATUS(status_ker_va,pcb[i].exit_status);
                i = 1; // start from beginning when wake up
            }
            else
                ret = pcb[i].pid;
        }
    }
    return ret;
}


uint8 do_nanosleep(struct timespec *ts)
{
    debug();
    // 1. block the current_running
    // 2. create a timer which calls `do_unblock` when timeout
    // 3. reschedule because the current_running is blocked.
    do_block(&current_running->list,&general_block_queue);
    uint64_t sleep_ticks = 0, nsec = ts->tv_nsec;

    // printk_port("time: %d, %d\n", ts->tv_sec, ts->tv_nsec);
    for (uint i = 0; i < NANO; ++i){
        sleep_ticks = (sleep_ticks / 10)+ time_base * (nsec % 10);
        nsec /= 10;
    }

    sleep_ticks += ts->tv_sec * time_base;
    timer_create(&do_unblock, &current_running->list, sleep_ticks);
    do_scheduler();
    return 0;
}

void do_block(list_node_t *list, list_head *queue)
{
    debug();
    pcb_t *pcb = list_entry(list, pcb_t, list);

    pcb->status = TASK_BLOCKED;
    list_add_tail(list,queue);   
}

/* pcb_node is of type list_node_t */
void do_unblock(void *pcb_node)
{
    debug();
    // unblock the `pcb` from the block queue   
    list_del(pcb_node);
    pcb_t *pcb = (pcb_t *)((list_node_t *)pcb_node)->ptr;
    pcb->status = TASK_READY;
    list_add_tail(pcb_node,&ready_queue);
}

void do_exit(int32_t exit_status)
{
    debug();
    // check if some other thread is waiting
    // if there is, unblock them
    list_node_t *temp = current_running->wait_list.next;

    while (temp != &current_running->wait_list)
    {
        list_node_t *tempnext = temp->next;
        list_del(temp);

        pcb_t *pt_pcb = list_entry(temp, pcb_t, list);
        if (pt_pcb->status != TASK_EXITED){
            do_unblock(temp);
        }
        
        temp = tempnext;
    }

    current_running->exit_status = exit_status;
    // decide terminal state by mode
    if (current_running->type == USER_THREAD || current_running->mode == ENTER_ZOMBIE_ON_EXIT)
        current_running->status = TASK_ZOMBIE;
    else if (current_running->mode == AUTO_CLEANUP_ON_EXIT)
    {
        for (int i = 0; i < NUM_MAX_TASK; ++i)
            if (pcb[i].status == TASK_ZOMBIE && pcb[i].parent.parent == current_running){
                pcb[i].status = TASK_EXITED;
                pcb[i].parent.parent = NULL;
                list_add_tail(&pcb[i].list,&available_queue);
                free_all_pages(pcb[i].pgdir, PAGE_ALIGN(pcb[i].kernel_sp));
            }
        current_running->status = TASK_EXITED;
        list_add_tail(&current_running->list,&available_queue);
        free_all_pages(current_running->pgdir, PAGE_ALIGN(current_running->kernel_sp));
    }
    do_scheduler();
}

/* exit all threads */
/* same as exit FOR NOW */
void do_exit_group(int32_t exit_status)
{
    debug();
    do_exit(exit_status);
}

pid_t do_getpid()
{
    debug();
    return current_running->pid;
}

pid_t do_getppid()
{
    debug();
    if (current_running->parent.parent == NULL)
        return 1;
    return current_running->parent.parent->pid;
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
    return current_running->pid;
}


/***************/
//DEBUG FUNCTION
void show_ready_queue()
{
    vt100_move_cursor(1,8);
    list_node_t *test_readyqueue = ready_queue.next;
    while (test_readyqueue != &ready_queue)
    {
        pcb_t *a = test_readyqueue->ptr;
        printk("id[ %d ] is in ready_queue;\n",a->pid);
        test_readyqueue = test_readyqueue->next;
    }
    printk("\n\nid[ %d ] is running;\n",current_running->pid);
}