#include <os/elf.h>
#include <pgtable.h>
#include <log.h>
#include <os/mm.h>
#include <os/sched.h>
#include <utils/utils.h>

uintptr_t get_kva_of(uintptr_t va, uintptr_t pgdir_kva)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t va_offset = (va&VA_MASK) & (NORMAL_PAGE_SIZE - 1);
    PTE *ptr = pgdir_kva + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        assert(0);
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + (vpn1 << VA_VPN1_SHIFT) + (vpn0 << VA_VPN0_SHIFT)+va_offset;

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        assert(0);
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + (vpn0 << VA_VPN0_SHIFT)+va_offset;

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        assert(0);
    }
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) +va_offset;
}

/* if not mapped, return NULL */
/* regardless of attribute */
uintptr_t probe_kva_of(uintptr_t va, uintptr_t pgdir_kva)
{
    if (!va) assert(0);
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t va_offset = (va&VA_MASK) & (NORMAL_PAGE_SIZE - 1);
    PTE *ptr = pgdir_kva + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return NULL;
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + (vpn1 << VA_VPN1_SHIFT) + (vpn0 << VA_VPN0_SHIFT)+va_offset;

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return NULL;
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + (vpn0 << VA_VPN0_SHIFT)+va_offset;

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return NULL;
    }
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) +va_offset;
}


/* 重新设置页面的访问权限mask */
/* 只重置read/write/exec */
/* success return 0, fail return -1 */
int32_t reset_va_page_attribute(uintptr_t va, uintptr_t pgdir_kva, uint64_t mask)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir_kva + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT))
        return -1;
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
        
    if (!get_attribute(*ptr,_PAGE_PRESENT))
        return -1;
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);

    if (!get_attribute(*ptr,_PAGE_PRESENT))
        return -1;
    else{
        uintptr_t page_kva = PTE2kva(*ptr);
        if (is_page_kva_shared(page_kva))
            return -1;
        else if (is_page_kva_copy_on_write(page_kva) && (mask & _PAGE_WRITE != 0))
            printk_port("dangerous mprotect, copy on write page is set to be read only\n");
        clear_attribute(ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
        set_attribute(ptr, mask);
        return 0;
    }
}

/* copy source pgdir to destination pgdir */
/* they map to the same page, but they are two diffrent page table */
void copy_page_table(uintptr_t dest, uintptr_t source, uint64_t user_stack_base)
{
    clear_pgdir(dest);
    share_pgtable(dest, pa2kva(PGDIR_PA));

    uint64_t new_page1, new_page2, pfn;
    // free page
    for (uint32_t i = 0; i < NUM_PTE_ENTRY; i++)
    {
        PTE *ptr1 = source + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT) && get_attribute(*ptr1,_PAGE_USER)){
            // level1 valid, copy level1
            new_page1 = allocPage();
            clear_pgdir(new_page1);
            PTE *ptr1_new = dest + i*sizeof(PTE);
            pfn = kva2pfn(new_page1);
            set_pfn(ptr1_new, pfn);
            set_attribute(ptr1_new,_PAGE_PRESENT|_PAGE_USER);
            // num2 = (user_stack_base - i * SUPER_PAGE_SIZE) / LARGE_PAGE_SIZE + 1; 
            for (uint32_t j = 0; j < NUM_PTE_ENTRY ; j++){
                PTE* ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT) && get_attribute(*ptr2,_PAGE_USER)){
                    // level2 valid, copy level2
                    new_page2 = allocPage();
                    clear_pgdir(new_page2);
                    PTE *ptr2_new = new_page1 + j*sizeof(PTE);
                    pfn = kva2pfn(new_page2);
                    set_pfn(ptr2_new, pfn);
                    set_attribute(ptr2_new, _PAGE_PRESENT|_PAGE_USER);
                    // num3 = (user_stack_base - i * SUPER_PAGE_SIZE - j * LARGE_PAGE_SIZE) / NORMAL_PAGE_SIZE + 1;
                    for (uint32_t k = 0; k < NUM_PTE_ENTRY; k++){
                        if (i*SUPER_PAGE_SIZE + j*LARGE_PAGE_SIZE + k*NORMAL_PAGE_SIZE >= user_stack_base)
                            return ;
                        PTE* ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        if (get_attribute(*ptr3,_PAGE_PRESENT) && get_attribute(*ptr3,_PAGE_USER) ){
                            //level3 valid, map level3
                            PTE *ptr3_new = new_page2 + k*sizeof(PTE);
                            uintptr_t mapped_page_kva = pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT);
                            set_page_kva_copy_on_write(mapped_page_kva, get_attribute(*ptr3, _PAGE_WRITE));
                            /* no writable */
                            if (!is_page_kva_shared(mapped_page_kva))
                                clear_attribute(ptr3, _PAGE_WRITE);
                            /* map to the same page as source */
                            *ptr3_new = *ptr3;
                        }
                    }
                }
            }
        }
    }
}