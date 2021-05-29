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
    #ifndef K210
    vt100_move_cursor(current_running->cursor_x,
                      current_running->cursor_y);
    #endif
    switch_to(previous_running,current_running);
}

/* default setup pcb */
void init_pcb_default(pcb_t *pcb_underinit,task_type_t type) 
{
    pcb_underinit->preempt_count = 0; 
    pcb_underinit->list.ptr = pcb_underinit; 
    pcb_underinit->pid = process_id++; 
    pcb_underinit->type = type; 
    init_list_head(&pcb_underinit->wait_list);
    pcb_underinit->status = TASK_READY; 
    pcb_underinit->priority = DEFAULT_PRIORITY; 
    pcb_underinit->temp_priority = pcb_underinit->priority; 
    pcb_underinit->mode = DEFAULT_MODE; 
    pcb_underinit->spawn_num = 0;
    pcb_underinit->cursor_x = 1; pcb_underinit->cursor_y = 1; 
    pcb_underinit->mask = 0xf; 
    pcb_underinit->parent.parent = NULL;

    /* file descriptors */
    // number, piped
    for (int i = 0; i < NUM_FD; ++i){
        pcb_underinit->fd[i].fd_num = i;
        pcb_underinit->fd[i].piped = FD_UNPIPED;
    }
    // open stdin , stdout and stderr
    pcb_underinit->fd[0].dev = STDIN; pcb_underinit->fd[0].used = FD_USED; pcb_underinit->fd[0].flags = O_RDONLY; 
    pcb_underinit->fd[1].dev = STDOUT; pcb_underinit->fd[1].used = FD_USED; pcb_underinit->fd[1].flags = O_WRONLY;
    pcb_underinit->fd[2].dev = STDERR; pcb_underinit->fd[2].used = FD_USED; pcb_underinit->fd[2].flags = O_WRONLY;
    for (int i = 3; i < NUM_FD; ++i)
        pcb_underinit->fd[i].used = FD_UNUSED; // remember to set up other props when use it

    // systime
    pcb_underinit->stime = 0;
    pcb_underinit->utime = 0;
}

/* prepare pcb stack for ready process */
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
    set_stack_base(pcb, kernel_stack, user_stack);
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

/* copy context */
/* kernel_stack and user_stack are stack top addr */
static void copy_to(pcb_t *pcb_underinit, uint64_t kernel_stack_top, uint64_t user_stack)
{
    uint64_t ker_stack_size, user_stack_size;
    ker_stack_size = current_running->kernel_stack_base + NORMAL_PAGE_SIZE - current_running->kernel_sp;
    user_stack_size = current_running->user_stack_base + NORMAL_PAGE_SIZE - current_running->user_sp;
    
    pcb_underinit->kernel_stack_base = kernel_stack_top - NORMAL_PAGE_SIZE;
    pcb_underinit->user_stack_base = user_stack; //wrong but no use
    pcb_underinit->kernel_sp = kernel_stack_top - ker_stack_size;
    pcb_underinit->user_sp = user_stack;
    copy_stack(pcb_underinit, ker_stack_size, user_stack_size);

    // copy fd
    for (int i = 0; i < NUM_FD; ++i)
        memcpy(&pcb_underinit->fd[i], &current_running->fd[i], sizeof(fd_t));
}

/* clone a child thread for current thread */
/* for now, use tls as entry point */
/* stack : ADDR OF CHILD STACK POINT */
/* success: child pid; fail: -1 */
pid_t do_clone(uint32_t flag, uint64_t stack, pid_t ptid, void *tls, pid_t ctid)
{
    for (int i = 1; i < NUM_MAX_TASK; ++i)
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
            ptr_t user_stack = (stack)? stack : USER_STACK_ADDR + current_running->spawn_num * NORMAL_PAGE_SIZE;

            // pgdir
            pcb_underinit->pgdir = current_running->pgdir;
           
            // prepare stack
            copy_to(pcb_underinit, kernel_stack_top, user_stack);

            // return 0 if child
            regs_context_t *pt_regs =
                (regs_context_t *)(kernel_stack_top - sizeof(regs_context_t));
            pt_regs->regs[4] = pcb_underinit;
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
    uint64_t status_ker = NULL;
    if (status) status_ker = get_kva_of(status,current_running->pgdir);
    pid_t ret = -1;
    for (int i = 1; i < NUM_MAX_TASK; ++i)
    {
        if (pcb[i].parent.parent == current_running && (pid == -1 || pid == pcb[i].pid)){
            // confirm pid
            if (pcb[i].status == TASK_READY){
                current_running->status = TASK_BLOCKED;
                list_add_tail(&current_running->list, &pcb[i].wait_list);
                ret = pcb[i].pid;
                do_scheduler();
                if (status_ker) WEXITSTATUS(status_ker,pcb[i].exit_status);
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
    pcb_t *pcb = list_entry(list, pcb_t, list);

    pcb->status = TASK_BLOCKED;
    list_add_tail(list,queue);   
}

/* pcb_node is of type list_node_t */
void do_unblock(void *pcb_node)
{
    // unblock the `pcb` from the block queue   
    list_del(pcb_node);
    pcb_t *pcb = (pcb_t *)((list_node_t *)pcb_node)->ptr;
    pcb->status = TASK_READY;
    list_add_tail(pcb_node,&ready_queue);
}

void do_exit(int32_t exit_status)
{
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
                list_add_tail(&current_running->list,&available_queue);
                freePage(pcb[i].kernel_stack_base);
            }
        current_running->status = TASK_EXITED;
        list_add_tail(&current_running->list,&available_queue);
        free_all_pages(current_running->pgdir, current_running->kernel_stack_base);
    }
    do_scheduler();
}

/* execvc a process */
/* success: no return value */
/* fail: return -1 */
int8 do_exec(const char* file_name, char* argv[], char *const envp)
{
    for (int i = 1; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status == TASK_EXITED)
        {
            // init pcb
            pcb_t *pcb_underinit = &pcb[i];
            ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE;
            ptr_t user_stack = USER_STACK_ADDR;

            init_pcb_default(pcb_underinit, USER_PROCESS);

            // get file
            unsigned char *_elf_binary = NULL;
            uint64 length;
            int32 fd;

            if ((fd = fat32_open(AT_FDCWD ,file_name, O_RDWR, 0)) == -1){
                return -1;
            }
            length = current_running->fd[fd].length;
            _elf_binary = (char *)kmalloc(length);
            fat32_read(fd, _elf_binary, length);

            uintptr_t pgdir = allocPage();
            clear_pgdir(pgdir);
            uint64_t user_stack_page_kva = alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            uint64_t edata;
            uintptr_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper, &edata);
            pcb_underinit->edata = edata;
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // copy argc & argv
            uint argc = 0;
            while (argv[argc] != NULL) argc++;
            if (argc){
                unsigned char **pointers = user_stack_page_kva; // point to argv[i] in i-th iteration
                unsigned char *strings = user_stack_page_kva + argc*sizeof(unsigned char *) + 1; // + 1 for NULL
                uint64_t write_to_stack = user_stack - NORMAL_PAGE_SIZE + argc*sizeof(unsigned char*) + 1; // virtual addr for user
                for (int i = 0; i < argc; ++i)
                {
                    memcpy(strings,argv[i],strlen(argv[i])+1);
                    pointers[i] = write_to_stack;
                    strings += (strlen(argv[i])+1); write_to_stack += (strlen(argv[i])+1);
                }
                pointers[argc] = 0;
            }

            // prepare stack(give argc and argv)(argv = user_stack - NORMAL_PAGE_SIZE)
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argc, user_stack - NORMAL_PAGE_SIZE, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            return pcb[i].pid; // no need but do
        }
    return -1;
}

pid_t do_getpid()
{
    return current_running->pid;
}

pid_t do_getppid()
{
    if (current_running->parent.parent == NULL)
        return 1;
    return current_running->parent.parent->pid;
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