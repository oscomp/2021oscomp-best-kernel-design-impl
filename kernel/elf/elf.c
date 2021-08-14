#include <os/elf.h>
#include <stdio.h>
#include <log.h>
#include <os/fat32.h>
#include <os/mm.h>
#include <os/sched.h>

/* prepare_page_for_kva should return a kernel virtual address */
/* return entry point va of this elf */
/* modify elf */
uintptr_t load_elf(
    unsigned char elf_binary[], unsigned length, uintptr_t pgdir,
    uintptr_t (*prepare_page_for_va)(uintptr_t va, uintptr_t pgdir, uint64_t mask),
    ELF_info_t *elf)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_binary;
    Elf64_Phdr *phdr = NULL;
    Elf64_Shdr *shdr = NULL;
    /* As a loader, we just care about segment,
     * so we just parse program headers.
     */
    unsigned char *ptr_ph_table = NULL;
    unsigned char *ptr_sh_table = NULL;
    Elf64_Half ph_entry_count;
    Elf64_Half ph_entry_size;
    Elf64_Half sh_entry_count;
    Elf64_Half sh_entry_size;
    uint64_t i = 0;

    // check whether `binary` is a ELF file.
    if (length < 4 || !is_elf_format(elf_binary)) {
        return 0;  // return NULL when error!
    }

    ptr_ph_table   = elf_binary + ehdr->e_phoff;
    ptr_sh_table   = elf_binary + ehdr->e_shoff;
    ph_entry_count = ehdr->e_phnum;
    ph_entry_size  = ehdr->e_phentsize;
    sh_entry_count = ehdr->e_shnum;
    sh_entry_size  = ehdr->e_shentsize;

    uint64_t first_load_p_vaddr = 0;
    uint8_t is_first = 1;
    while (ph_entry_count--) {
        phdr = (Elf64_Phdr *)ptr_ph_table;
        if (phdr->p_type == PT_LOAD) {
            // printk_port("p_vaddr:%lx, p_offset:%lx\n", phdr->p_vaddr, phdr->p_offset);
            // printk_port("filesz: %lx, memsz: %lx\n", phdr->p_filesz, phdr->p_memsz);
            if (is_first) { first_load_p_vaddr = phdr->p_vaddr; is_first = 0; }
            for (i = 0; i < phdr->p_memsz; ) {
                uintptr_t offset_in_page = (phdr->p_vaddr + i) % NORMAL_PAGE_SIZE; // offset in this page
                uint64_t copy_bytes; // how many bytes are copied
                if (i < phdr->p_filesz) {
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    copy_bytes = MIN(phdr->p_filesz - i, NORMAL_PAGE_SIZE - offset_in_page);

                    memcpy(
                        bytes_of_page,
                        elf_binary + phdr->p_offset + i - offset_in_page,
                        NORMAL_PAGE_SIZE);
                    if (offset_in_page + copy_bytes < NORMAL_PAGE_SIZE) {

                        for (uint64_t j =
                                 offset_in_page + copy_bytes;
                             j < NORMAL_PAGE_SIZE; ++j) {
                            bytes_of_page[j] = 0;
                        }
                    }
                } else {
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    copy_bytes = NORMAL_PAGE_SIZE - offset_in_page;

                    for (uint64_t j = offset_in_page;
                         j < NORMAL_PAGE_SIZE;
                         ++j) {
                        bytes_of_page[j] = 0;
                    }
                }
                i += copy_bytes;
            }
        }

        ptr_ph_table += ph_entry_size;
    }

    ptr_sh_table += sh_entry_size;
    shdr = (Elf64_Shdr *)ptr_sh_table; // ignore NULL sec
    while (shdr->sh_flags & SHF_ALLOC){        
        ptr_sh_table += sh_entry_size;
        shdr = (Elf64_Shdr *)ptr_sh_table;
    }
    /* for brk */
    shdr = ptr_sh_table - sh_entry_size; // last data

    /* set elf info */
    elf->text_begin = first_load_p_vaddr;
    elf->phoff = ehdr->e_phoff;
    elf->phent = ehdr->e_phentsize;
    elf->phnum = ehdr->e_phnum;
    elf->entry = ehdr->e_entry;
    elf->edata = shdr->sh_addr + shdr->sh_size;

    return ehdr->e_entry;
}

/* not prepare all page */
/* return entry point va of this elf */
/* modify elf */
uintptr_t lazy_load_elf(
    fd_num_t fd, uintptr_t pgdir,
    uintptr_t (*prepare_page_for_va)(uintptr_t va, uintptr_t pgdir, uint64_t mask), 
    void *pcb_as_void)
{
    debug();
    pcb_t *pcb = (pcb_t *)pcb_as_void;
    unsigned char *elf_binary = kalloc();

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_binary;
    Elf64_Phdr *phdr = NULL;
    Elf64_Shdr *shdr = NULL;
    /* As a loader, we just care about segment,
     * so we just parse program headers.
     */
    unsigned char *ptr_ph_table = NULL;
    unsigned char *ptr_sh_table = NULL;

    /* read ELF header */
    int16_t fd_index = get_fd_index(fd, current_running);
    assert(fd_index >= 0);
    memcpy(&pcb->myelf_fd, &current_running->fd[fd_index], sizeof(fd_t));
    fat32_close(fd);

    fat32_lseekmy(0, SEEK_SET);
    fat32_readmy(elf_binary, NORMAL_PAGE_SIZE);

    // check whether `binary` is a ELF file.
    if (current_running->myelf_fd.length < 4 || !is_elf_format(elf_binary)) {
        log(0, "is not an ELF");
        assert(0);  // return NULL when error!
    }
    
    /* 1. */
    pcb->elf.phoff = ehdr->e_phoff;
    pcb->elf.phent = ehdr->e_phentsize;
    pcb->elf.phnum = ehdr->e_phnum;
    pcb->elf.entry = ehdr->e_entry;

    /* 2. get program headers */
    uint64_t ph_entry_count = pcb->elf.phnum;
    assert(pcb->elf.phent * pcb->elf.phnum <= NORMAL_PAGE_SIZE); /* FOR NOW assume is 1 page */
    fat32_lseekmy(pcb->elf.phoff, SEEK_SET);
    fat32_readmy(elf_binary, NORMAL_PAGE_SIZE);

    phdr = (Elf64_Phdr *)elf_binary;
    uint8_t is_first = 1;

    uint8_t phdr_info_cnt = 0;
    assert(ph_entry_count > NUM_PHDR_IN_PCB);
    while (ph_entry_count--) {
        if (phdr->p_type == PT_LOAD) {
            memcpy(&pcb->phdr[phdr_info_cnt++], phdr, sizeof(Elf64_Phdr));
            pcb->elf.edata = phdr->p_vaddr + phdr->p_memsz;
            if (is_first) { pcb->elf.text_begin = phdr->p_vaddr; is_first = 0; }
        }
        phdr++;
    }
    kfree(elf_binary);

    return pcb->elf.entry;
}

