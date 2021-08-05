/* RISC-V kernel boot stage */
#include <pgtable.h>
#include <sbi.h>
#include <os/elf.h>

typedef void (*kernel_entry_t)(unsigned long, uintptr_t);

extern unsigned char _elf_main[];
extern unsigned _length_main;

/********* setup memory mapping ***********/
unsigned long pagetop = PGDIR_PA;
uintptr_t alloc_page()
{
    pagetop += NORMAL_PAGE_SIZE;
}

void itoa(uint64_t num,char *str)
{
    while (num)
    {
        *str++ = num%10 + '0';
        num /= 10;
    }
    *str++ = '\n';*str = '\0';
}

// using 2MB large page
void map_page(uint64_t va, uint64_t pa, PTE *pgdir)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT, pfn = pa >> VA_VPN1_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2;

    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        alloc_page();
        clear_pgdir(pagetop);
        uint64_t pfn0 = pagetop >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn0);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
        ptr = pagetop + vpn1*sizeof(PTE);
    }
    else
    {
        ptr = (get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    // char num[200];
    // itoa(vpn1,num);sbi_console_putstr(num);    
    }
    // char num[200];
    // itoa(pfn,num);sbi_console_putstr("accutal:");sbi_console_putstr(num);    
    set_pfn(ptr,pfn << PPN_BITS);
    set_attribute(ptr,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
}

void enable_vm()
{
    // TODO: write satp to enable paging
    // remember to flush TLB
    set_satp(SATP_MODE_SV39,0,PGDIR_PA >> NORMAL_PAGE_SHIFT);
    local_flush_tlb_all();
}

/* Sv-39 mode
 * 0x0000_0000_0000_0000-0x0000_003f_ffff_ffff is for user mode
 * 0xffff_ffc0_0000_0000-0xffff_ffff_ffff_ffff is for kernel mode
 */
void setup_vm()
{
    clear_pgdir(PGDIR_PA);
    for (uint64_t i = START_ENTRYPOINT; i < KERNEL_END; i += LARGE_PAGE_SIZE)
    {
        map_page(i,(i&0xffffffff),PGDIR_PA);
    }
    for (uint64_t i = BOOT_KERNEL; i < BOOT_KERNEL_END; i += LARGE_PAGE_SIZE)
    {
        map_page(i,i,PGDIR_PA);
    }
    // enable virtual memory
    enable_vm();
}

uintptr_t directmap(uintptr_t kva, uintptr_t pgdir)
{
    // ignore pgdir
    return kva;
}

kernel_entry_t start_kernel = NULL;

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
        if (phdr->p_type == PT_LOAD) {
            if (!first_load_p_vaddr) first_load_p_vaddr = phdr->p_vaddr;
            for (i = 0; i < phdr->p_memsz; ) {
                uintptr_t offset_in_page = (phdr->p_vaddr + i) % NORMAL_PAGE_SIZE; // offset in this page
                uint64_t copy_bytes; // how many bytes are copied
                if (i < phdr->p_filesz) {
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    copy_bytes = MIN(phdr->p_filesz - i, NORMAL_PAGE_SIZE - offset_in_page);
                    memcpy(
                        bytes_of_page + offset_in_page,
                        elf_binary + phdr->p_offset + i,
                        copy_bytes);
                    if (offset_in_page + copy_bytes < NORMAL_PAGE_SIZE) {
                        for (int j =
                                 offset_in_page + copy_bytes;
                             j < NORMAL_PAGE_SIZE; ++j) {
                            bytes_of_page[j] = 0;
                        }
                        copy_bytes = NORMAL_PAGE_SIZE - offset_in_page;
                    }
                } else {
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


/*********** start here **************/
int boot_kernel(unsigned long mhartid, uintptr_t riscv_dtb)
{    
    if (mhartid == 0) {
        setup_vm();
        // load kernel
        ELF_info_t elf;
        start_kernel =
                (kernel_entry_t)load_elf(_elf_main, _length_main,
                                         PGDIR_PA, directmap, &elf);
    } else {
        // TODO: what should we do for other cores?
        while (1) sbi_console_putchar('6');
    }
    // uintptr_t s0 = get_s0();
    // local_flush_tlb_all();   
    // char *c = (char*)start_kernel+0x1000;
    // char num[200];
    // itoa(start_kernel,num);
    // for (int i = 0; i < 20; ++i)
    //  {
    //     sbi_console_putchar('0'+(*c) % 10);
    //  }     
    start_kernel(mhartid, riscv_dtb);
    return 0;
}
