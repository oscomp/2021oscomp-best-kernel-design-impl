#include <os/list.h>
#include <os/mm.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/time.h>
#include <os/irq.h>
#include <stdio.h>
#include <assert.h>
#include <pgtable.h>
#include <csr.h>
#include <qemu.h>
#include <os/elf.h>
#include <os/fat32.h>


/* default setup pcb */ 
/* tid = pid */
void init_pcb_default(pcb_t *pcb_underinit,task_type_t type) 
{
    pcb_underinit->exec = 0;
    pcb_underinit->preempt_count = 0; 
    pcb_underinit->list.ptr = pcb_underinit; 
    pcb_underinit->pid = process_id;
    pcb_underinit->pid_on_exec = process_id;
    pcb_underinit->tid = process_id++;
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
        memset(&pcb_underinit->fd[i], 0, sizeof(fd_t));
        pcb_underinit->fd[i].fd_num = i;
        // pcb_underinit->fd[i].redirected = FD_UNREDIRECTED;
        // pcb_underinit->fd[i].piped = FD_UNPIPED;
        // pcb_underinit->fd[i].poll_status = 0;
    }
    // open stdin , stdout and stderr
    pcb_underinit->fd[0].dev = STDIN; pcb_underinit->fd[0].used = FD_USED; pcb_underinit->fd[0].flags = O_RDONLY; 
    pcb_underinit->fd[1].dev = STDOUT; pcb_underinit->fd[1].used = FD_USED; pcb_underinit->fd[1].flags = O_WRONLY;
    pcb_underinit->fd[2].dev = STDERR; pcb_underinit->fd[2].used = FD_USED; pcb_underinit->fd[2].flags = O_WRONLY;
    for (int i = 3; i < NUM_FD; ++i){
        pcb_underinit->fd[i].used = FD_UNUSED; // remember to set up other props when use it
        pcb_underinit->fd[i].dev = DEFAULT_DEV;
    }
    /* systime */
    pcb_underinit->stime = 0;
    pcb_underinit->utime = 0;
    /* my file descriptor */
    pcb_underinit->myelf_fd.pos = 0;
    /* context switch time */
    pcb_underinit->scheduler_switch_time = 0;
    pcb_underinit->yield_switch_time = 0;
}

void init_pcb_exec(pcb_t *pcb_underinit)
{
    pcb_underinit->spawn_num = 0;
    pcb_underinit->pid = pcb_underinit->pid_on_exec;
    pcb_underinit->type = USER_PROCESS;
    pcb_underinit->mode = AUTO_CLEANUP_ON_EXIT;
}
// static void copyout(uintptr_t pgdir, unsigned char *dst, unsigned char *src, size_t size)
// {
//     alloc_page_helper(user_stack,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
// }

static void get_random_bytes(unsigned char *random, size_t size)
{
    // unsigned char c = 0x11;
    // for (uint64_t i = 0; i < size; ++i)
    //     random[i] = 2*c + i;
    random[0] = 0xd6;
    random[1] = 0x75;
    random[2] = 0x73;
    random[3] = 0x2b;
    random[4] = 0x99;
    random[5] = 0xcc;
    random[6] = 0x10;
    random[7] = 0x5d;
    random[8] = 0x03;
    random[9] = 0xdd;
    random[10] = 0xc3;
    random[11] = 0xe6;
    random[12] = 0xef;
    random[13] = 0x1d;
    random[14] = 0x5a;
    random[15] = 0x75;
}

static uint64_t get_offset_from_random(unsigned char *random)
{
    return 8;
}

static uint64_t get_envc_from_envp(char *envp[])
{
    uint64_t envc = 0;
    if (envp)
        while (envp[envc]){
            envc++;
        }
    return envc;
}

static uint64_t get_aux_vec_size(aux_elem_t *aux_vec)
{
    uint64_t size = 0;
    if (!aux_vec){        
        size += 2 * sizeof(uintptr_t);
    }
    else{
        while (aux_vec->id != 0)
        {
            size += 2 * sizeof(uintptr_t);
            aux_vec++;
        }
        // last should be 0 and ignore val
        size += 2 * sizeof(uintptr_t);
    }
    return size;
}

static inline uint32_t is_random_pad(aux_elem_t *aux_vec)
{
    if (!aux_vec) return 0;
    while (aux_vec->id != 0)
        if (aux_vec->id == AT_RANDOM)
            return 1;
        else
            aux_vec++;
    return 0;
}

static inline uint32_t is_execfn(aux_elem_t *aux_vec)
{
    if (!aux_vec) return 0;
    while (aux_vec->id != 0)
        if (aux_vec->id == AT_EXECFN)
            return 1;
        else
            aux_vec++;
    return 0;
}

#define STACK_ALIGN 0x10
/* copy things to user stack */
/* argv, envp, aux_vec could be NULL, filename must not be NULL */
/* return user_sp */
static uintptr_t copy_above_user_stack(uintptr_t sp_kva, unsigned char* argv[], unsigned char *envp[],
    aux_elem_t *aux_vec, unsigned char *filename)
{
    unsigned char random[16];
    get_random_bytes(random, 16);
    uint64_t random_offset = get_offset_from_random(random);

    uint32_t argc = get_argc_from_argv(argv);
    uint64_t envc = get_envc_from_envp(envp);
    // printk_port("argc is %d\n", argc);
    // printk_port("envc is %d\n", envc);
    uint64_t aux_vec_size = get_aux_vec_size(aux_vec);

    // 0. compute size
    uint64_t size = 0;
    size += sizeof(uint64_t);
    size += (argc + 1) * sizeof(uintptr_t);
    size += (envc + 1) * sizeof(uintptr_t);
    size += get_aux_vec_size(aux_vec);
    size += random_offset;
    size += 16 * is_random_pad(aux_vec);
    if (argv)        
        for (uint i = 0; i < argc; ++i)
            size += strlen(argv[i]) + 1;
    if (envp)        
        for (uint i = 0; i < envc; ++i)
            size += strlen(envp[i]) + 1;
    size += (strlen(filename) + 1) * is_execfn(aux_vec);
    if (size % STACK_ALIGN){
        size += STACK_ALIGN - size % STACK_ALIGN;
    }

    assert(size < NORMAL_PAGE_SIZE);
    // printk_port("size is %lx\n", size);
    sp_kva -= size;
    uintptr_t start_sp_kva = sp_kva;
    uintptr_t sp_uva = USER_STACK_ADDR - size;

    // 1. copy argc
    memcpy(sp_kva, &argc, sizeof(uint32_t));
    sp_kva += sizeof(uint64_t);
    // 2. alloc argv
    uint64_t *argv_start = (uint64_t *)sp_kva;
    sp_kva += (argc + 1) * sizeof(uintptr_t);
    // 3. alloc envp
    uint64_t *envp_start = (uint64_t *)sp_kva;
    sp_kva += (envc + 1) * sizeof(uintptr_t);
    // 4. copy aux_vec
    aux_elem_t *mem_aux_vec = (aux_elem_t *)sp_kva;
    aux_elem_t *start_mem_aux_vec = mem_aux_vec;
    if (!aux_vec){        
        mem_aux_vec->id = 0; mem_aux_vec->val = 0;
        sp_kva += 2 * sizeof(uintptr_t);
    }
    else{
        while (aux_vec->id != 0)
        {
            mem_aux_vec->id = aux_vec->id;
            mem_aux_vec->val = aux_vec->val;
            mem_aux_vec++; aux_vec++;
            sp_kva += 2 * sizeof(uintptr_t);
        }
        // last should be 0 and ignore val
        mem_aux_vec->id = 0;
        mem_aux_vec->val = 0;
        sp_kva += 2 * sizeof(uintptr_t);
    }
    aux_elem_t *last_mem_aux_vec = mem_aux_vec;
    // 5. random pad
    for (uint i = 0; i < random_offset; ++i)
        *(char*)(sp_kva + i) = 0; // magic
    sp_kva += random_offset;
    memcpy(sp_kva, random, 16);
    while (mem_aux_vec->id != AT_RANDOM && mem_aux_vec >= start_mem_aux_vec)
        mem_aux_vec--;
    if (mem_aux_vec->id == AT_RANDOM && mem_aux_vec >= start_mem_aux_vec){
        mem_aux_vec->val = (uint64_t)sp_kva - (uint64_t)start_sp_kva + sp_uva;
        sp_kva += 16;
    }
    mem_aux_vec = last_mem_aux_vec;
    // 6. copy argv strings
    if (argv){        
        for (uint i = 0; i < argc; ++i){
            // log(0, "argv[%d] is :%s", i, argv[i]);
            memcpy(sp_kva, argv[i], strlen(argv[i]) + 1);
            *argv_start = sp_kva - start_sp_kva + sp_uva;
            sp_kva += strlen(argv[i]) + 1;
            argv_start++;
        }
    }
    *argv_start = 0;
    // 7. copy envp strings
    if (envp){        
        for (uint i = 0; i < envc; ++i){
            memcpy(sp_kva, envp[i], strlen(envp[i]) + 1);
            *envp_start = sp_kva - start_sp_kva + sp_uva;
            sp_kva += strlen(envp[i]) + 1;
            envp_start++;
        }
    }
    *envp_start = 0;
    // 8. copy filename
    assert(filename);
    while (mem_aux_vec->id != AT_EXECFN && mem_aux_vec >= start_mem_aux_vec)
        mem_aux_vec--;
    if (mem_aux_vec->id == AT_EXECFN && mem_aux_vec >= start_mem_aux_vec){
        memcpy(sp_kva, filename, strlen(filename) + 1);
        // printk_port("filename: %s\n", sp_kva);
        mem_aux_vec->val = (uint64_t)sp_kva - (uint64_t)start_sp_kva + sp_uva;
        sp_kva += strlen(filename) + 1;
    }
    // 9. padding
    if (sp_kva % STACK_ALIGN){
        unsigned char *temp = (unsigned char *)sp_kva;
        for (uint i = 0; i < STACK_ALIGN - sp_kva % STACK_ALIGN; ++i)
            temp[i] = 0;
        sp_kva += STACK_ALIGN - sp_kva % STACK_ALIGN;
    }
    // for (uint64_t i = start_sp_kva; i < sp_kva; i += 8)
    //     printk_port("%lx:%lx\n", i, *((uint64_t *)i));
    // for (uint64_t i = start_sp_kva; i < sp_kva; i += 1)
    //     printk_port("%lx:%lx\n", i, *((unsigned char*)i));
    return sp_uva; //user sp
}

uint64_t get_argc_from_argv(char *argv[])
{
    uint64_t argc = 0;
    if (argv)
        while (argv[argc]) argc++;
    return argc;
}

/* prepare pcb stack for ready process */
/* kernel_stack is kernel_stack_top, user_stack is user_stack_top */
/* prepare USER content, SWITCH content and ARGV & ENVP & AUX_VEC */
/* argv, envp, aux_vec cound be NULL, called at special point */
/* user_stack_base is set */
void init_pcb_stack(
    ptr_t pgdir, ptr_t kernel_stack, ptr_t user_stack, ptr_t entry_point, unsigned char* argv[], unsigned char *envp[],
    aux_elem_t *aux_vec, pcb_t *pcb)
{
    regs_context_t *pt_regs =
        (regs_context_t *)(kernel_stack - sizeof(regs_context_t));
    
    reg_t gp, ra;
    
    gp = __global_pointer$;
    ra = pcb->elf.entry;

    pcb->kernel_sp = (reg_t)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));
    pcb->user_sp = (void*)user_stack;
    pcb->user_stack_base = (void*)user_stack - USER_STACK_INIT_SIZE;
    pcb->user_addr_top = user_stack - USER_STACK_INIT_SIZE;
    pcb->pgdir = pgdir;

    reg_t *regs = pt_regs->regs;    
    regs[3] = gp;  //gp
    regs[4] = 0; //tp
    regs[10] = 0; // execve return 0
    // regs[11]= (ptr_t)argv; //a1=argv
    pt_regs->sstatus = SR_SUM | SR_FS; //enable supervisor-mode userpage access and float point instruction

#ifdef K210
    pt_regs->sstatus = SR_FS; //enable supervisor-mode userpage access for K210
#endif

    pt_regs->sepc = ra;
    unsigned mode = SATP_MODE_SV39, asid = pcb->tid, ppn = kva2pa(pgdir) >> NORMAL_PAGE_SHIFT;
    pt_regs->satp = (unsigned long)(((unsigned long)mode << SATP_MODE_SHIFT) | ((unsigned long)asid << SATP_ASID_SHIFT) | ppn);
    
    switchto_context_t *switch_to_reg = 
        (switchto_context_t *)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));
    switch_to_reg->regs[0] = &ret_from_exception;
    switch_to_reg->satp = pt_regs->satp;

    /* 注意，user_stack并未分配页面 */
    uint64_t user_stack_kva = get_kva_of(user_stack - NORMAL_PAGE_SIZE, pgdir) + NORMAL_PAGE_SIZE;

    unsigned char *filename = NULL;
    char backupname[10]; strcpy(backupname,"name");
    if (argv)
        filename = argv[0];
    else
        filename = backupname;
    /* 返回的sp作为用户栈指针 */
    pcb->user_sp = copy_above_user_stack(user_stack_kva, argv, envp, aux_vec, filename); 
    // printk_port("sp is at %lx, %lx\n", pcb->user_sp, get_kva_of(pcb->user_sp, pcb->pgdir)); 
    assert(USER_STACK_ADDR - pcb->user_sp < NORMAL_PAGE_SIZE);
}

/* copy context */
/* kernel_stack and user_stack are stack top addr */
static void copy_parent_all_and_set_sp(pcb_t *pcb_underinit, uint64_t kernel_stack_top, uint64_t user_stack_top)
{
    uint64_t ker_stack_size, user_stack_size;
    /* kernel_sp is just at user context, but maybe not equal to register sp */
    ker_stack_size = sizeof(regs_context_t) + sizeof(switchto_context_t);
    log(0, "ker_stack_size: %lx", ker_stack_size);
    user_stack_size = current_running->user_stack_base + USER_STACK_INIT_SIZE - current_running->user_sp;
    log(0, "user_stack_base: %lx, user_stack_size:%lx", current_running->user_stack_base, user_stack_size);
    
    pcb_underinit->kernel_sp = kernel_stack_top - ker_stack_size; /* for user context */
    pcb_underinit->user_sp = user_stack_top - user_stack_size; /* no need to be aligned with user_sp */
    pcb_underinit->user_stack_base = user_stack_top - USER_STACK_INIT_SIZE;
    memcpy(pcb_underinit->kernel_sp + sizeof(switchto_context_t), PAGE_ALIGNUP(current_running->kernel_sp) - sizeof(regs_context_t), sizeof(regs_context_t));
    // memcpy(pcb_underinit->user_sp, current_running->user_sp, user_stack_size); /* copy tp, very essential */

    // copy fd
    for (int i = 0; i < NUM_FD; ++i){
        memcpy(&pcb_underinit->fd[i], &current_running->fd[i], sizeof(fd_t));
        if (current_running->fd[i].piped == FD_PIPED){
            pipe_num_t pip_num = current_running->fd[i].pip_num;
            assert(current_running->fd[i].is_pipe_read || current_running->fd[i].is_pipe_write);
            if (current_running->fd[i].is_pipe_read){
                pipes[pip_num].r_valid++;
            }
            else if (current_running->fd[i].is_pipe_write){
                pipes[pip_num].w_valid++;
            }
            else{
                assert(0);
            }
        }
    }
    memcpy(&pcb_underinit->elf, &current_running->elf, sizeof(struct ELF_info));
    memcpy(&pcb_underinit->myelf_fd, &current_running->myelf_fd, sizeof(fd_t));
    for (int i = 0; i < NUM_PHDR_IN_PCB; i++)
        memcpy(&pcb_underinit->phdr[i], &current_running->phdr[i], sizeof(Elf64_Phdr));
}

static void init_clone_pcb_prop(pcb_t *pcb_underinit, uint32_t flag)
{
    pcb_underinit->type = USER_THREAD;
    pcb_underinit->mode = ENTER_ZOMBIE_ON_EXIT;
    pcb_underinit->parent.parent = current_running;
    pcb_underinit->parent.flag = flag;
    pcb_underinit->user_addr_top = current_running->user_addr_top;
    pcb_underinit->edata = current_running->edata;
}

/* init a cloned pcb */
void init_clone_pcb(uint64_t pgdir, pcb_t *pcb_underinit, uint64_t kernel_stack_top, uint64_t user_stack_top, uint32_t flag)
{
    init_clone_pcb_prop(pcb_underinit, flag);
    // prepare stack
    copy_parent_all_and_set_sp(pcb_underinit, kernel_stack_top, user_stack_top);
    pcb_underinit->pgdir = pgdir;
    regs_context_t *pt_regs =
        (regs_context_t *)(kernel_stack_top - sizeof(regs_context_t));
    /* return 0 if child */
    pt_regs->regs[10] = 0;
    unsigned mode = SATP_MODE_SV39, asid = pcb_underinit->tid, ppn = kva2pa(pgdir) >> NORMAL_PAGE_SHIFT;
    pt_regs->satp = (unsigned long)(((unsigned long)mode << SATP_MODE_SHIFT) | ((unsigned long)asid << SATP_ASID_SHIFT) | ppn);

    // prepare switch context under user context
    switchto_context_t *switch_to_reg = 
        (switchto_context_t *)(pcb_underinit->kernel_sp);
    // kernel stack should be different
    switch_to_reg->regs[0] = &ret_from_exception;
    switch_to_reg->satp = pt_regs->satp;
}
