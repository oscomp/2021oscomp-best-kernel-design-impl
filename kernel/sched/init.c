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

// static void copyout(uintptr_t pgdir, unsigned char *dst, unsigned char *src, size_t size)
// {
//     alloc_page_helper(user_stack,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
// }

static void get_random_bytes(unsigned char *random, size_t size)
{
    unsigned char c = 0x11;
    for (uint64_t i = 0; i < size; ++i)
        random[i] = 2*c + i;
}

static uint64_t get_offset_from_random(unsigned char *random)
{
    return 3;
}

static uint64_t get_argc_from_argv(char *argv[])
{
    uint64_t argc = 0;
    if (argv)
        while (argv[argc]) argc++;
    return argc;
}

static uint64_t get_envc_from_envp(char *envp[])
{
    uint64_t envc = 0;
    if (envp)
        while (envp[envc]) envc++;
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

// /* copy things to user stack */
// /* argv, envp, aux_vec could be NULL */
// static uintptr_t copy_above_user_stack(uintptr_t sp_kva, unsigned char* argv[], unsigned char *envp[],
//     aux_elem_t *aux_vec, unsigned char *filename)
// {
//     uintptr_t sp_uva = USER_STACK_ADDR;
//     uintptr_t start_sp_kva = sp_kva;
//     // 1. copy argc
//     uint64_t argc = get_argc_from_argv(argv);
//     // printk_port("argc is %d\n", argc);
//     memcpy(sp_kva, &argc, sizeof(uint64_t));
//     sp_kva += sizeof(uint64_t);
//     // 2. alloc argv
//     uint64_t *argv_start = (uint64_t *)sp_kva;
//     sp_kva += (argc + 1) * sizeof(uintptr_t);
//     // 3. alloc envp
//     uint64_t *envp_start = (uint64_t *)sp_kva;
//     uint envc = 0;
//     if (envp)
//         while (envp[envc]) envc++;
//     // printk_port("envc is %d\n", envc);
//     sp_kva += (envc + 1) * sizeof(uintptr_t);
//     // 4. copy aux_vec
//     aux_elem_t *mem_aux_vec = (aux_elem_t *)sp_kva;
//     aux_elem_t *start_mem_aux_vec = mem_aux_vec;
//     if (!aux_vec){        
//         mem_aux_vec->id = 0; mem_aux_vec->val = 0;
//         sp_kva += 2 * sizeof(uintptr_t);
//     }
//     else{
//         while (aux_vec->id != 0)
//         {
//             mem_aux_vec->id = aux_vec->id;
//             mem_aux_vec->val = aux_vec->val;
//             mem_aux_vec++; aux_vec++;
//             sp_kva += 2 * sizeof(uintptr_t);
//         }
//         // last should be 0 and ignore val
//         mem_aux_vec->id = 0;
//         mem_aux_vec->val = 0;
//         sp_kva += 2 * sizeof(uintptr_t);
//     }
//     aux_elem_t *last_mem_aux_vec = mem_aux_vec;
//     // 5. random pad
//     for (uint i = 0; i < 3; ++i)
//         *(char*)(sp_kva + i) = 0; // magic
//     sp_kva += 3;
//     unsigned char random[16];
//     get_random_bytes(random, 16);
//     memcpy(sp_kva, random, 16);
//     while (mem_aux_vec->id != AT_RANDOM && mem_aux_vec >= start_mem_aux_vec)
//         mem_aux_vec--;
//     if (mem_aux_vec->id == AT_RANDOM && mem_aux_vec >= start_mem_aux_vec){
//         mem_aux_vec->val = (uint64_t)sp_kva - (uint64_t)start_sp_kva + sp_uva;
//         sp_kva += 16;
//     }
//     mem_aux_vec = last_mem_aux_vec;
//     // 6. copy argv strings
//     if (argv){        
//         for (uint i = 0; i < argc; ++i){
//             memcpy(sp_kva, argv[i], strlen(argv[i]) + 1);
//             *argv_start = sp_kva - start_sp_kva + sp_uva;
//             sp_kva += strlen(argv[i]) + 1;
//             argv_start++;
//         }
//     }
//     *argv_start = 0;
//     // 7. copy envp strings
//     if (envp){        
//         for (uint i = 0; i < envc; ++i){
//             memcpy(sp_kva, envp[i], strlen(envp[i]) + 1);
//             *envp_start = sp_kva - start_sp_kva + sp_uva;
//             sp_kva += strlen(envp[i]) + 1;
//             envp_start++;
//         }
//     }
//     *envp_start = 0;
//     // 8. copy filename
//     assert(filename);
//     while (mem_aux_vec->id != AT_EXECFN && mem_aux_vec >= start_mem_aux_vec)
//         mem_aux_vec--;
//     if (mem_aux_vec->id == AT_EXECFN && mem_aux_vec >= start_mem_aux_vec){
//         memcpy(sp_kva, filename, strlen(filename) + 1);
//         // printk_port("filename: %s\n", sp_kva);
//         mem_aux_vec->val = (uint64_t)sp_kva - (uint64_t)start_sp_kva + sp_uva;
//         sp_kva += strlen(filename) + 1;
//     }
//     // 9. padding
//     if (sp_kva % 16){
//         unsigned char *temp = (unsigned char *)sp_kva;
//         for (uint i = 0; i < 16 - sp_kva % 16; ++i)
//             temp[i] = 0;
//         sp_kva += 16 - sp_kva % 16;
//     }
//     // printk_port("final sp is %lx\n", sp_kva);
//     // for (uint64_t i = start_sp_kva; i < sp_kva; i += 8)
//     //     printk_port("%lx:%lx\n", i, *((uint64_t *)i));
//     return sp_kva;
// }

/* copy things to user stack */
/* argv, envp, aux_vec could be NULL, filename must not be NULL */
/* return user_sp */
static uintptr_t copy_above_user_stack(uintptr_t sp_kva, unsigned char* argv[], unsigned char *envp[],
    aux_elem_t *aux_vec, unsigned char *filename)
{
    unsigned char random[16];
    get_random_bytes(random, 16);
    uint64_t random_offset = get_offset_from_random(random);

    uint64_t argc = get_argc_from_argv(argv);
    uint64_t envc = get_envc_from_envp(envp);
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
    if (size % 16){
        size += 16 - size % 16;
    }

    assert(size < NORMAL_PAGE_SIZE);
    // printk_port("size is %lx\n", size);
    sp_kva -= size;
    uintptr_t start_sp_kva = sp_kva;
    uintptr_t sp_uva = USER_STACK_ADDR - size;

    // 1. copy argc
    // printk_port("argc is %d\n", argc);
    memcpy(sp_kva, &argc, sizeof(uint64_t));
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
    if (sp_kva % 16){
        unsigned char *temp = (unsigned char *)sp_kva;
        for (uint i = 0; i < 16 - sp_kva % 16; ++i)
            temp[i] = 0;
        sp_kva += 16 - sp_kva % 16;
    }
    // printk_port("final sp is %lx, usp is %lx\n", sp_kva, sp_uva);
    // for (uint64_t i = start_sp_kva; i < sp_kva; i += 8)
    //     printk_port("%lx:%lx\n", i, *((uint64_t *)i));
    return sp_uva; //user sp
}

/* prepare pcb stack for ready process */
/* kernel_stack is kernel_stack_top, user_stack is user_stack_top */
/* prepare USER content, SWITCH content and ARGV & ENVP & AUX_VEC */
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
    pcb->user_sp = (reg_t)user_stack;
    set_stack_base(pcb, kernel_stack, user_stack);
    pcb->pgdir = pgdir;

    reg_t *regs = pt_regs->regs;    
    regs[3] = gp;  //gp
    regs[4] = pcb; //tp
    regs[10] = get_argc_from_argv(argv); //a0=argc
    // regs[11]= (ptr_t)argv; //a1=argv
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

    /* get aux_vec first */
    /* this var is increasing */
    uint64_t user_stack_kva = get_kva_of(user_stack, pgdir);
    /* now is prepare stack: argv, envp and aux */
    unsigned char *filename = NULL;
    char backupname[10]; strcpy(backupname,"name");
    if (argv)
        filename = argv[0];
    else
        filename = backupname;
    pcb->user_sp = copy_above_user_stack(user_stack_kva, argv, envp, aux_vec, filename);   
    assert(USER_STACK_ADDR - pcb->user_sp <= NORMAL_PAGE_SIZE);
}
