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

/* execvc a process */
/* success: no return value */
/* fail: return -1 */
static const char *fixed_envp[] = {"SHELL=ash",
                     "PWD=/",
                     "HOME=/",
                     "USER=root",
                     "MOTD_SHOWN=pam",
                     "LANG=C.UTF-8",
                     "INVOCATION_ID=e9500a871cf044d9886a157f53826684",
                     "TERM=vt220",
                     "SHLVL=2",
                     "JOURNAL_STREAM=8:9265",
                     "PATH=/",
                     "OLDPWD=/root",
                     "_=busybox",
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

    NEW_AUX_ENT(AT_HWCAP, 0x112d);                  // 0x10
    NEW_AUX_ENT(AT_PAGESZ, NORMAL_PAGE_SIZE);       // 0x06
    NEW_AUX_ENT(AT_CLKTCK, 0x64);                   // 0x17
    NEW_AUX_ENT(AT_PHDR, elf->phoff);               // 0x03
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
            _elf_binary = (char *)allocproc();
            if ((uintptr_t)(_elf_binary + length) > (uintptr_t)pa2kva(PGDIR_PA)){
                printk_port("%lx\n%lx\n", kva2pa(_elf_binary + length), PGDIR_PA);
                assert(0);
            }
            fat32_read(fd, _elf_binary, length);

            uintptr_t pgdir = allocPage();
            clear_pgdir(pgdir);
            alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            alloc_page_helper(user_stack,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);

            uintptr_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper, &pcb_underinit->elf);
            pcb_underinit->edata = USER_PILE_ADDR;
            // printk_port("test_elf is %lx\n", test_elf);
            // printk_port("edata is %lx, and real edata is %lx\n", pcb_underinit->edata, pcb_underinit->elf.edata);
            // printk_port("phoff is %lx\n", pcb_underinit->elf.phoff);
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // prepare stack(argv,envp,aux)
            aux_elem_t aux_vec[MAX_AUX_ARG+1];
            /* init_aux_vec(&aux_vec) */
            set_aux_vec(&aux_vec, &pcb_underinit->elf);
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, argv, fixed_envp, &aux_vec, pcb_underinit);

            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            return pcb[i].pid; // no need but do
        }
    return -1;
}
