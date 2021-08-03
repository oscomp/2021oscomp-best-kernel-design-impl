#include <os/elf.h>
#include <stdio.h>

static inline int is_elf_format(unsigned char *binary)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)binary;

    if (ehdr->e_ident[0] == EI_MAG0 &&
        ehdr->e_ident[1] == EI_MAG1 &&
        ehdr->e_ident[2] == EI_MAG2 &&
        ehdr->e_ident[3] == EI_MAG3) {
        return 0;
    }

    return 1;
}

/* prepare_page_for_kva should return a kernel virtual address */
/* return entry point va of this elf */
/* modify *edata as end of all data */
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
    int i = 0;

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
    while (ph_entry_count--) {
        phdr = (Elf64_Phdr *)ptr_ph_table;
        // if (phdr->p_type >= 0 && phdr->p_type <= 9)
        //     sbi_console_putchar('0' + phdr->p_type);
        // if (phdr->p_type == PT_GNU_STACK && phdr->p_memsz > 0)
        //     sbi_console_putchar('a');
        // else if (phdr->p_type == PT_GNU_RELRO)
        //     sbi_console_putchar('b');
        // else
        //     sbi_console_putchar('c');
        if (phdr->p_type == PT_LOAD || phdr->p_type == PT_GNU_RELRO) {
            if (!first_load_p_vaddr) first_load_p_vaddr = phdr->p_vaddr;
            for (i = 0; i < phdr->p_memsz; ) {
                uintptr_t offset_in_page = (phdr->p_vaddr + i) % NORMAL_PAGE_SIZE; // offset in this page
                uint64_t copy_bytes; // how many bytes are copied
                if (i < phdr->p_filesz) {
                    // sbi_console_putchar('7');
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    copy_bytes = MIN(phdr->p_filesz - i, NORMAL_PAGE_SIZE - offset_in_page);
                    memcpy(
                        bytes_of_page + offset_in_page,
                        elf_binary + phdr->p_offset + i,
                        copy_bytes);
                    if (offset_in_page + copy_bytes < NORMAL_PAGE_SIZE) {
                        // sbi_console_putchar('8');
                        for (int j =
                                 offset_in_page + copy_bytes;
                             j < NORMAL_PAGE_SIZE; ++j) {
                            bytes_of_page[j] = 0;
                        }
                        copy_bytes = NORMAL_PAGE_SIZE - offset_in_page;
                    }
                } else {
                    // sbi_console_putchar('9');
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    copy_bytes = NORMAL_PAGE_SIZE - offset_in_page;
                    for (int j = offset_in_page;
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
    elf->phoff = first_load_p_vaddr + ehdr->e_phoff;
    elf->phent = ehdr->e_phentsize;
    elf->phnum = ehdr->e_phnum;
    elf->entry = ehdr->e_entry;
    elf->edata = shdr->sh_addr + shdr->sh_size;

    return ehdr->e_entry;
}
