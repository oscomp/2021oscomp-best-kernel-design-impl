/* RISC-V kernel boot stage */
#include <context.h>
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

/*********** start here **************/
int boot_kernel(unsigned long mhartid, uintptr_t riscv_dtb)
{    
    if (mhartid == 0) {
        setup_vm();
        // load kernel
        start_kernel =
                (kernel_entry_t)load_elf(_elf_main, _length_main,
                                         PGDIR_PA, directmap);
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
