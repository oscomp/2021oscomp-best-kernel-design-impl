#include <os/ioremap.h>
#include <os/mm.h>
#include <pgtable.h>
#include <type.h>

// maybe you can map it to IO_ADDR_START ?
static uintptr_t io_base = IO_ADDR_START;
static uintptr_t pgtable_base = IO_PAGE_START;

uintptr_t alloc_page()
{
    pgtable_base += NORMAL_PAGE_SIZE;
    return pgtable_base;
}


void map_page(uint64_t va, uint64_t pa, PTE *pgdir)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT, pfn = pa >> VA_VPN1_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2;
    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        uintptr_t pagebase = alloc_page();
        clear_pgdir(pagebase);
        uint64_t pfn0 = kva2pa(pagebase) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn0);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
        ptr = pagebase + vpn1*sizeof(PTE);
    }
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    set_pfn(ptr,pfn << PPN_BITS);
    set_attribute(ptr,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
}


void *ioremap(unsigned long phys_addr, unsigned long size)
{
    // uintptr_t prev_io_base = io_base;
    // uint64_t pagenum = (size + LARGE_PAGE_SIZE-1) / LARGE_PAGE_SIZE;
    // for (uint64_t i = 0; i < pagenum; ++i){
    //     map_page(prev_io_base+i*LARGE_PAGE_SIZE,phys_addr+i*LARGE_PAGE_SIZE,pa2kva(PGDIR_PA));
    // }
    // local_flush_tlb_all();
    // io_base += LARGE_PAGE_SIZE*pagenum;
    // return prev_io_base + (phys_addr & (LARGE_PAGE_SIZE-1));
    uint64_t pagenum = (size + LARGE_PAGE_SIZE-1) / LARGE_PAGE_SIZE;
    for (uint64_t i = 0; i < pagenum; ++i){
        map_page(phys_addr+i*LARGE_PAGE_SIZE,phys_addr+i*LARGE_PAGE_SIZE,pa2kva(PGDIR_PA));
    }
    local_flush_tlb_all();
    return ;
}

void iounmap(void *io_addr)
{
    // TODO: a very naive iounmap() is OK
    // maybe no one would call this function?
    // *pte = 0;
}