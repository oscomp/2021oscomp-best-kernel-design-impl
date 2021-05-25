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
    if (previous_running->pid && previous_running->status == TASK_RUNNING)
    {        
        previous_running->status = TASK_READY;
        previous_running->temp_priority = previous_running->priority;
        list_add_tail(&previous_running->list,&ready_queue);
    }
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
    #ifndef K210
    vt100_move_cursor(current_running->cursor_x,
                      current_running->cursor_y);
    #endif
    switch_to(previous_running,current_running);
}

void init_pcb_stack(
    ptr_t pgdir, ptr_t kernel_stack, ptr_t user_stack, ptr_t entry_point, int argc ,void *arg,
    pcb_t *pcb)
{
    regs_context_t *pt_regs =
        (regs_context_t *)(kernel_stack - sizeof(regs_context_t));
    
    reg_t gp, ra;
    
    gp = __global_pointer$;
    ra = entry_point;

    pcb->kernel_sp = (reg_t)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));
    pcb->user_sp = (reg_t)user_stack;
    pcb->kernel_stack_base = kernel_stack - NORMAL_PAGE_SIZE;
    pcb->user_stack_base = user_stack - NORMAL_PAGE_SIZE;
    pcb->pgdir = pgdir;

    reg_t *regs = pt_regs->regs;    
    regs[3] = gp;  //gp
    regs[4] = pcb; //tp
    regs[10] = argc; //a0=argc
    regs[11]= (ptr_t)arg; //a1=argv
    pt_regs->sstatus = SR_SUM; //enable supervisor-mode userpage access

#ifdef K210
    pt_regs->sstatus = 0; //enable supervisor-mode userpage access for K210
#endif

    pt_regs->sepc = ra;
    unsigned mode = SATP_MODE_SV39, asid = pcb->pid, ppn = kva2pa(pgdir) >> NORMAL_PAGE_SHIFT;
    pt_regs->satp = (unsigned long)(((unsigned long)mode << SATP_MODE_SHIFT) | ((unsigned long)asid << SATP_ASID_SHIFT) | ppn);
    
    switchto_context_t *switch_to_reg = 
        (switchto_context_t *)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));
    switch_to_reg->regs[0] = &ret_from_exception;
}


void do_sleep(uint32_t sleep_time)
{
    // TODO: sleep(seconds)
    // note: you can assume: 1 second = `timebase` ticks
    // 1. block the current_running
    // 2. create a timer which calls `do_unblock` when timeout
    // 3. reschedule because the current_running is blocked.
    
    do_block(&current_running->list,&general_block_queue);
    timer_create(&do_unblock, &current_running->list, sleep_time * time_base);
    do_scheduler();
}

void do_block(list_node_t *pcb_node, list_head *queue)
{
    // TODO: block the pcb task into the block queue
    pcb_t *pcb = pcb_node->ptr;

    pcb->status = TASK_BLOCKED;
    list_add_tail(pcb_node,queue);   
}

void do_unblock(void *pcb_node)
{
    // TODO: unblock the `pcb` from the block queue   
    list_del(pcb_node);
    pcb_t *pcb = (pcb_t *)((list_node_t *)pcb_node)->ptr;
    pcb->status = TASK_READY;
    list_add_tail(pcb_node,&ready_queue);
}

void do_exit()
{
    // check if some other thread is waiting
    // if there is, unblock them
    list_node_t *temp = current_running->wait_list.next;

    while (temp != &current_running->wait_list)
    {
        list_node_t *tempnext = temp->next;
        pcb_t *waitting = temp->ptr;
        pid_t waitting_pid = waitting->pid;
        list_del(temp);

        pcb_t *pt_pcb = temp->ptr;
        pt_pcb->status = TASK_READY;
        list_add_tail(temp,&ready_queue);

        temp = tempnext;
    }

    // decide terminal state by mode
    if (current_running->mode == ENTER_ZOMBIE_ON_EXIT)
        current_running->status = TASK_ZOMBIE;
    else if (current_running->mode == AUTO_CLEANUP_ON_EXIT)
    {
        current_running->status = TASK_EXITED;
        list_add_tail(&current_running->list,&available_queue);
        free_all_pages(current_running->pgdir, current_running->kernel_stack_base);
    }
    do_scheduler();
}

pid_t do_exec(const char* file_name, int argc, char* argv[], spawn_mode_t mode)
{
    for (int i = 1; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status == TASK_EXITED)
        {
            // init pcb
            pcb_t *pcb_underinit = &pcb[i];
            ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE;
            ptr_t user_stack = USER_STACK_ADDR;

            pcb_underinit->preempt_count = 0;
            pcb_underinit->list.ptr = pcb_underinit;
            pcb_underinit->pid = process_id++;
            pcb_underinit->type = USER_PROCESS;
            pcb_underinit->status = TASK_READY;
            pcb_underinit->priority = 1;
            pcb_underinit->temp_priority = pcb_underinit->priority;
            pcb_underinit->mode = mode;
            pcb_underinit->spawn_num = 0;
            pcb_underinit->cursor_x = 1; pcb_underinit->cursor_y = 1;
            pcb_underinit->mask = current_running->mask;

            // get file
            unsigned char *_elf_binary = NULL;
            int length;

            get_elf_file(file_name,&_elf_binary,&length);
            uintptr_t pgdir = allocPage();
            clear_pgdir(pgdir);
            uint64_t user_stack_page_kva = alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            uintptr_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper);
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // copy argc & argv
            if (argc){
                unsigned char **pointers = user_stack_page_kva;
                unsigned char *strings = user_stack_page_kva + argc*sizeof(unsigned char *);
                uint64_t write_to_stack = user_stack - NORMAL_PAGE_SIZE + argc*sizeof(unsigned char*);
                for (int i = 0; i < argc; ++i)
                {
                    // memcpy(pointers,&argv[i],sizeof(char *));pointers += sizeof(char*);
                    memcpy(strings,argv[i],strlen(argv[i])+1);
                    pointers[i] = write_to_stack;
                    strings += (strlen(argv[i])+1);write_to_stack += (strlen(argv[i])+1);
                }
            }

            // prepare stack
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argc, user_stack - NORMAL_PAGE_SIZE, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);
            // prints("mem:%lx",memalloc);
            return pcb[i].pid;
        }
    return -1;
}

pid_t do_getpid()
{
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