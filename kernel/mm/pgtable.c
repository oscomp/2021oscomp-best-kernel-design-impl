#include <os/elf.h>
#include <pgtable.h>
#include <log.h>

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
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC)){
        assert(0);
    }
    else
        return pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) +va_offset;
}

/* 设置页面的访问权限 */
/* success return 0, fail return -1 */
int32_t reset_va_page_attribute(uintptr_t va, uintptr_t pgdir_kva, uint64_t mask)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir_kva + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return -1;
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    else{
        clear_attribute(ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
        set_attribute(ptr, mask);
        return 0;
    }
        

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return -1;
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC))
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    else{
        clear_attribute(ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
        set_attribute(ptr, mask);
        return 0;
    }

    if (!get_attribute(*ptr,_PAGE_PRESENT)){
        return -1;
    }
    else if (!get_attribute(*ptr,_PAGE_READ)&!get_attribute(*ptr,_PAGE_WRITE)&!get_attribute(*ptr,_PAGE_EXEC)){
        return -1;
    }
    else{
        log(0, "%x\n", get_attribute(*ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC));
        clear_attribute(ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC);
        log(0, "%x\n", get_attribute(*ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC));
        set_attribute(ptr, mask);
        log(0, "%x\n", get_attribute(*ptr, _PAGE_READ|_PAGE_WRITE|_PAGE_EXEC));
        return 0;
    }
}
