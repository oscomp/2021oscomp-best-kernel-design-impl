#include <os/mm.h>
#include <os/sched.h>
#include <pgtable.h>
#include <os/string.h>
#include <assert.h>
#include <string.h>
#include <log.h>
#include <os/fat32.h>

/* addr 必须对齐4K */
/* 成功返回0, 失败返回-1 */
int do_mprotect(void *addr, size_t len, int prot)
{
    debug();
    if ((uintptr_t)addr % NORMAL_PAGE_SIZE) return -1;
    uint64_t mask = 0lu;

    if (prot & PROT_READ)
        mask |= _PAGE_READ;
    if (prot & PROT_WRITE)
        mask |= _PAGE_WRITE;
    if (prot & PROT_EXEC)
        mask |= _PAGE_EXEC;

    if (prot != PROT_NONE)
        mask |= _PAGE_ACCESSED | _PAGE_DIRTY;

    for (uintptr_t i = 0; i < len; i += NORMAL_PAGE_SIZE)
    {
        uintptr_t uva = (uintptr_t)addr + i;
        if (probe_kva_of(uva, current_running->pgdir) == NULL){
            /* user virtual address may be very difficult to handle */
            /* uva > boundary, but PAGE_ALIGN(uva) <= boundary */
            uintptr_t uva_bottom = PAGE_ALIGN(uva);
            uintptr_t uva_top = uva_bottom + NORMAL_PAGE_SIZE;
            uint8 j;
            for (j = 0; j < NUM_PHDR_IN_PCB; j++){
                Elf64_Phdr *phdr = (Elf64_Phdr *)(&current_running->phdr[j]);
                if ((uva_bottom >= phdr->p_vaddr && uva_bottom < phdr->p_vaddr + phdr->p_memsz) || 
                    (uva_bottom < phdr->p_vaddr && uva_top > phdr->p_vaddr))
                {
                    /* it is in ELF file */
                    fat32_lseekmy(PAGE_ALIGN(uva_bottom - phdr->p_vaddr + phdr->p_offset), SEEK_SET);
                    alloc_page_helper(uva_bottom, current_running->pgdir, _PAGE_ALL_MOD); /* FOR NOW all mod */
                    /* now we can access this address */
                    fat32_readmy(uva_bottom, NORMAL_PAGE_SIZE);
                    uint64_t data_top = phdr->p_vaddr + phdr->p_filesz;
                    unsigned char *uva_char = (unsigned char *)uva_bottom;
                    if (phdr->p_filesz < phdr->p_memsz && uva_bottom >= PAGE_ALIGN(data_top)){
                        /* should pad 0 for bss */                        
                        if (uva_bottom == PAGE_ALIGN(data_top)){ 
                            for (uint64_t pos = PAGE_OFFSET(data_top); pos < NORMAL_PAGE_SIZE; pos++)
                                uva_char[pos] = 0; /* just pad part of this page */
                        }
                        else
                            for (uint64_t pos = 0; pos < NORMAL_PAGE_SIZE; pos++)
                                uva_char[pos] = 0; /* pad all page */
                    }
                    break;
                }
            }
            if (j == NUM_PHDR_IN_PCB){
                /* not in file */
                alloc_page_helper(uva_bottom, current_running->pgdir, _PAGE_ALL_MOD);
            }
        }
        if (reset_va_page_attribute(uva, current_running->pgdir, mask))
            return -1;
    }
    return 0;
}

/* modify data top addr */
/* success return 0, fail return -1*/
int64_t do_brk(uintptr_t ptr)
{
    debug();
    uint64_t user_addr_top = get_user_addr_top(current_running);
    uint64_t edata = get_edata(current_running);
    if (ptr > user_addr_top){
        printk_port("brk arg0 too large\n");
        return -1; // larger than stack
    }
    else if (!ptr){
        // alloc_page_helper(current_running->edata, current_running->pgdir,
        //     _PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
        return edata; // probe edata
    }
    else if (ptr < edata) return -1; // cannot be smaller than current addr
    else{
        for (uintptr_t i = edata; i < ptr; i += NORMAL_PAGE_SIZE)
            alloc_page_helper(i, current_running->pgdir, _PAGE_READ|_PAGE_WRITE);
        log2(0, "brk size is %lx", ptr - edata);
        set_edata(current_running, ptr);
        return ptr;
    }
}
