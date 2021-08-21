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
                     "JOURNAL_STREAM=8:9290",
                     "HUSHLOGIN=FALSE",
                     "PATH=/",
                     "MAIL=/var/mail/root",
                     "_=/usr/bin/bash",
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

    // NEW_AUX_ENT(AT_SYSINFO_EHDR, 0x3fc2dee000);     // 0x21
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

static int is_sh(char *file_name)
{
    size_t len = strlen(file_name);
    log(0, "len is %d", len);
    if (len > 3 && file_name[len-3] == '.' && file_name[len-2] == 's' && file_name[len-1] == 'h')
        return 1;
    else
        return 0;
}

/* a whole new pcb */
/* parent.parent remains unchanged */
int8 do_exec(const char* file_name, char* argv[], char *const envp[])
{
    debug();
    static int ctx_cnt = 0;
    if (!strcmp(argv[1], "lat_ctx")){
        if (ctx_cnt == 0){
            ctx_cnt++;
            do_exit(0);
        }
    }
    else if (!strcmp(argv[1], "echo")){
        if (!strcmp(argv[3], "lat_ctx"))
            do_exit(0);
        // argv[11] = NULL;
    }

    // printk_port("argv[1] is %s\n", argv[1]);
    // if (argv[2])
    //     printk_port("argv[2] is %s\n", argv[2]);
    // init pcb
    pcb_t *pcb_underinit = current_running;
    ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE; /* just 1 page */
    ptr_t user_stack = USER_STACK_ADDR;
    log(0, "exec pid is %d", pcb_underinit->pid_on_exec);
    init_pcb_exec(pcb_underinit);

    // get file
    unsigned char *_elf_binary = NULL;
    int length;
    int32 fd;
    
    if ((fd = fat32_openat(AT_FDCWD ,file_name, O_RDWR, 0)) < 0){
        assert(0);
        return SYSCALL_FAILED;
    }
    uchar *new_argv = NULL;
    if (is_sh(file_name)){
        unsigned char *file_start = kalloc();
        fat32_read(fd, file_start, NORMAL_PAGE_SIZE);
        if (memcmp(file_start, "#!", 2)){
            log(0, "file not start with #!");
            return SYSCALL_FAILED;
        }
        else{
            uchar *temp = &file_start[2], *temp1;
            uint32_t add_argc = 0;
            // 0. get added argc
            while (*temp != '\n'){
                if (*temp == ' ')
                    add_argc++;
                temp++;
            }
            add_argc++;
            log(0, "add_argc:%d", add_argc);

            // 1. alloc argv space
            new_argv = kalloc();
            uint32_t argc = get_argc_from_argv(argv);
            uint64_t *argv_start = new_argv;
            uintptr_t strings = new_argv + (argc + add_argc + 1) * sizeof(uintptr_t);
            // 2. copy #!
            temp = &file_start[2];
            temp1 = &file_start[2];
            while (*temp1 != '\n'){
                if (*temp1 == ' '){
                    *temp1 = 0;
                    memcpy(strings, temp, strlen(temp) + 1);
                    *argv_start = strings;
                    strings += strlen(temp) + 1;
                    argv_start++;
                    temp = temp1 + 1;
                }
                temp1++;
            }
            *temp1 = 0;
            memcpy(strings, temp, strlen(temp) + 1);
            *argv_start = strings;
            strings += strlen(temp) + 1;
            argv_start++;
            // 3. copy original argv
            for (uint i = 0; i < argc; ++i){
                // log(0, "argv[%d] is :%s", i, argv[i]);
                memcpy(strings, argv[i], strlen(argv[i]) + 1);
                *argv_start = strings;
                strings += strlen(argv[i]) + 1;
                argv_start++;
            }
            // 4. end with NULL
            *argv_start = 0;
            // 5. test
            unsigned char **test_argv = new_argv;
            uint8_t cnt = 0;
            while (test_argv[cnt]){
                log(0, "test argv[%d] is %s", cnt, test_argv[cnt]);
                cnt++;
            }
        }
        kfree(file_start);
        fat32_close(fd);
    }
    if (new_argv){
        log(0, "old argv is %lx, new argv is %lx", argv, new_argv);
        argv = new_argv;
        log(0, "new argv[0] is %s", argv[0]);
        if ((fd = fat32_openat(AT_FDCWD, argv[0], O_RDWR, 0)) < 0){
            assert(0);
            return SYSCALL_FAILED;
        }
    }

    uintptr_t pgdir = allocPage(), prev_pgdir = pcb_underinit->pgdir, prev_ker_stack_base = PAGE_ALIGN(pcb_underinit->kernel_sp);
    clear_pgdir(pgdir);
    share_pgtable(pgdir,pa2kva(PGDIR_PA)); /* 只有内核高地址段才存在PGDIR_PA */
    for (uintptr_t j = 0; j < USER_STACK_INIT_SIZE / NORMAL_PAGE_SIZE; j++)
        alloc_page_helper(user_stack - (j + 1)*NORMAL_PAGE_SIZE, pgdir, _PAGE_READ|_PAGE_WRITE);
    // set_user_addr_top(pcb_underinit, user_stack - USER_STACK_INIT_SIZE - NORMAL_PAGE_SIZE);


    uintptr_t test_elf = (uintptr_t)lazy_load_elf(fd, pgdir, alloc_page_helper, pcb_underinit);
    set_pcb_edata(pcb_underinit);

    /* init_aux_vec(&aux_vec) */
    aux_elem_t aux_vec[MAX_AUX_ARG+1];
    set_aux_vec(&aux_vec, &pcb_underinit->elf);
    /* init kernel and user stack */
    init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argv, fixed_envp, &aux_vec, pcb_underinit);
    // no need to add to ready_queue
    // remember to close this temp fd
    // already done in lazy load

    /* free all previous pages */
    /* if multiple cores are open, need to lock 'prev_ker_stack_base' until switch to another process */
    free_all_pages(prev_pgdir, prev_ker_stack_base);
    if (new_argv)
        kfree(new_argv);
    /* tell scheduler that I am a ecex-on process */
    pcb_underinit->exec = 1;
    do_scheduler();
    /* this return never happens */
    return current_running->pid; // no need but do
}
