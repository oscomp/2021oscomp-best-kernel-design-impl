#include <os/mm.h>
#include <os/sched.h>
#include <pgtable.h>
#include <os/string.h>
#include <assert.h>
#include <string.h>
#include <log.h>

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
        if (reset_va_page_attribute(addr + i, current_running->pgdir, mask))
            return -1;
    }
}

/* modify data top addr */
/* success return 0, fail return -1*/
int64_t do_brk(uintptr_t ptr)
{
    debug();
    assert(ptr % NORMAL_PAGE_SIZE == 0);
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
        set_edata(current_running, ptr);
        return ptr;
    }
}
