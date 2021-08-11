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
#include <log.h>

/* execvc a process */
/* success: no return value */
/* fail: return -1 */
static const char *fixed_envp[] = {"SHELL=/bin/bash",
                     "PWD=/",
                     "LOGNAME=root",
                     "MOTD_SHOWN=pam",
                     "HOME=/root",
                     "LANG=C.UTF-8",
                     "INVOCATION_ID=9f58417fca9d46d4a23cde1329404868",
                     "TERM=vt220",
                     "USER=root",
                     "SHLVL=1",
                     "JOURNAL_STREAM=8:9282",
                     "HUSHLOGIN=FALSE",
                     "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
                     "MAIL=/var/mail/root",
                     "_=./busybox",
                     "OLDPWD=/root",
                     0};


/* set auxiliary vector BUT AT_EXECFN */
/* AT_EXECFN need to be decided last */              
static void set_aux_vec(aux_elem_t *aux_vec, ELF_info_t *elf)
{
    uint index = 0;
#define NEW_AUX_ENT(id0, val0) \
    aux_vec[index].id = id0; \
    aux_vec[index++].val = val0; \
    // 0x21
    NEW_AUX_ENT(0x28, 0);
    NEW_AUX_ENT(0x29, 0);
    NEW_AUX_ENT(0x2a, 0);
    NEW_AUX_ENT(0x2b, 0);
    NEW_AUX_ENT(0x2c, 0);
    NEW_AUX_ENT(0x2d, 0);

    NEW_AUX_ENT(AT_SYSINFO_EHDR, 0x3fc2dee000);     // 0x21
    NEW_AUX_ENT(AT_HWCAP, 0x112d);                  // 0x10
    NEW_AUX_ENT(AT_PAGESZ, NORMAL_PAGE_SIZE);       // 0x06
    NEW_AUX_ENT(AT_CLKTCK, 0x64);                   // 0x11
    NEW_AUX_ENT(AT_PHDR, elf->text_begin + elf->phoff);               // 0x03
    NEW_AUX_ENT(AT_PHENT, elf->phent);              // 0x04
    NEW_AUX_ENT(AT_PHNUM, elf->phnum);              // 0x05
    NEW_AUX_ENT(AT_BASE, 0);                        // 0x07
    NEW_AUX_ENT(AT_FLAGS, 0);                       // 0x08
    NEW_AUX_ENT(AT_ENTRY, elf->entry);              // 0x09
    NEW_AUX_ENT(AT_UID, 0);                         // 0x0b
    NEW_AUX_ENT(AT_EUID, 0);                        // 0x0c
    NEW_AUX_ENT(AT_GID, 0);                         // 0x0d
    NEW_AUX_ENT(AT_EGID, 0);                        // 0x0e
    NEW_AUX_ENT(AT_SECURE, 0);                      // 0x17
    NEW_AUX_ENT(AT_RANDOM, 0);                      // 0x19
    NEW_AUX_ENT(AT_EXECFN, 0);                      // 0x1f
    NEW_AUX_ENT(0, 0);
#undef NEW_AUX_ENT
}

int8 do_exec(const char* file_name, char* argv[], char *const envp[])
{
    debug();
    // init pcb
    pcb_t *pcb_underinit = current_running;
    ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE; /* just 1 page */
    /* now we can set spawn num to 0, as no address collision will occur */
    pcb_underinit->spawn_num = 0;
    ptr_t user_stack = USER_STACK_ADDR;
    log(0, "exec pid is %d", pcb_underinit->pid_on_exec);
    init_pcb_exec(pcb_underinit);

    // get file
    unsigned char *_elf_binary = NULL;
    int length;
    int32 fd;

    // #ifdef K210

    if ((fd = fat32_open(AT_FDCWD ,file_name, O_RDWR, 0)) == -1){
        return -1;
    }
    // length = current_running->fd[fd].length;
    // _elf_binary = (char *)allocproc();
    // if ((uintptr_t)(_elf_binary + length) > (uintptr_t)pa2kva(PGDIR_PA)){
    //     printk_port("%lx\n%lx\n", kva2pa(_elf_binary + length), PGDIR_PA);
    //     assert(0);
    // }
    // // log(DEBUG,"length: %d\n", length);
    // fat32_read(fd, _elf_binary, length);

    // #else

    // get_elf_file("busybox", &_elf_binary, &length);

    // #endif

    uintptr_t pgdir = allocPage();
    clear_pgdir(pgdir);
    share_pgtable(pgdir,pa2kva(PGDIR_PA)); /* 只有内核高地址段才存在PGDIR_PA */
    for (uintptr_t j = 0; j < USER_STACK_INIT_SIZE / NORMAL_PAGE_SIZE; j++)
        alloc_page_helper(user_stack - (j + 1)*NORMAL_PAGE_SIZE, pgdir, _PAGE_READ|_PAGE_WRITE);
    set_user_addr_top(pcb_underinit, user_stack - USER_STACK_INIT_SIZE);

    // #ifndef K210
    // uintptr_t test_elf = (uintptr_t)load_elf(_elf_binary, length, pgdir, alloc_page_helper, &pcb_underinit->elf);
    // #else
    uintptr_t test_elf = (uintptr_t)lazy_load_elf(fd, pgdir, alloc_page_helper, pcb_underinit);
    // #endif
    set_pcb_edata(pcb_underinit);

    // prepare stack(argv,envp,aux)
    aux_elem_t aux_vec[MAX_AUX_ARG+1];
    /* init_aux_vec(&aux_vec) */
    set_aux_vec(&aux_vec, &pcb_underinit->elf);
    // init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argv, fixed_envp, &aux_vec, pcb_underinit);
    init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argv, fixed_envp, &aux_vec, pcb_underinit);
    // no need to add to ready_queue
    // list_del(&pcb_underinit->list);
    // list_add_tail(&pcb_underinit->list,&ready_queue);

    // remember to close this temp fd
    // already done in lazy load

    current_running->exec = 1;
    do_scheduler();
    /* this return never happens */
    return current_running->pid; // no need but do
}
