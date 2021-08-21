#include <os/list.h>
#include <os/mm.h>
#include <os/sched.h>
#include <pgtable.h>
#include <os/string.h>
#include <assert.h>
#include <sbi.h>
#include <string.h>
#include <screen.h>
#include <log.h>
#include <os/fat32.h>

ptr_t memCurr = FREEMEM;
// ptr_t memTop = KERNEL_END;
unsigned int start_block_id = 1024;
static ptr_t memalloc = KERNEL_END;

LIST_HEAD(freePageList);
static LIST_HEAD(shmPageList);
static LIST_HEAD(freePagebackupList);
LIST_HEAD(swapPageList);
LIST_HEAD(availableSwapSpace);
static LIST_HEAD(waitPageList);

static uint64_t swap_page();
static uint64_t find_swap_page_kva(uint64_t pgdir);

#define NUM_REC ((KERNEL_END - FREEMEM) / NORMAL_PAGE_SIZE)
#define PAGE_KVA_TO_PAGE_NODE_INDEX(kva) \
    (((kva) - FREEMEM) / NORMAL_PAGE_SIZE)

page_node_t pageRecyc[NUM_REC];

void init_recyc()
{
    for (uint32 i = 0; i < NUM_REC; ++i){
        pageRecyc[i].list.ptr = FREEMEM + i * NORMAL_PAGE_SIZE;
        list_add_tail(&pageRecyc[i], &freePageList);
    }
}

/* put page_node into freePagebackupList */
/* share num = 1, file map = NULL */
/* return page kva */
static uint64_t alloc_page_node(page_node_t *page_node)
{
    page_node->share_num = 1;
    page_node->copy_on_write = 0;
    page_node->file_map_info.len = 0;
    page_node->is_shared = 0;
    page_node->is_swapped = 0;
    list_del(&page_node->list);
    list_add(&page_node->list, &freePagebackupList);
    return page_node->list.ptr;
}

/* free this page node */
/* share num--, if = 0, file map = NULL */
/* if real freed, return 0; else return 1 */
static uint64_t free_page_node(page_node_t *page_node)
{
    page_node->share_num--;
    if (page_node->share_num == 0){
        if (page_node->file_map_info.len != 0 && page_node->is_file_map_dirty){
            // printk_port("write back");
            file_map_write_back(&page_node->file_map_info);
            // page_node->file_map_info.len = 0;
            // assert(0);
        }
        list_del(&page_node->list);
        list_add(&page_node->list, &freePageList);
        return 0;
    }
    return 1;
}

/* check memory usage, don't call it */
void handle_memory_leak(void *pcb_void)
{
    pcb_t *pcb = (pcb_t *)pcb_void;
    assert(pcb->status == TASK_EXITED);
    uint64_t freemem = 0, allocatedmem = 0, mymem = 0;
    for (list_node_t *temp = freePageList.next; temp != &freePageList; temp = temp->next){
        freemem += NORMAL_PAGE_SIZE;
    }
    page_node_t *page_node, *q;
    list_for_each_entry_safe(page_node, q, &freePagebackupList, list){
        if (page_node->list.tid == pcb->tid && page_node->share_num == 1){
            mymem += NORMAL_PAGE_SIZE;
            // free_page_node(page_node);
        }
        allocatedmem += NORMAL_PAGE_SIZE;
    }
    printk_port("free memsize is %lx, allocatedmem is %lx\n", freemem, allocatedmem);
    printk_port("tid %d leek memsize is %lx\n", pcb->tid, mymem);
}

/* return page kva */
ptr_t allocPage()
{
    // log(0, "memCurr %lx, memalloc %lx", memCurr, memalloc);
    ptr_t ret = 0;
    while (!ret){
        if (!list_empty(&freePageList)){
            page_node_t *page_node = list_entry(freePageList.next, page_node_t, list);        
            ret = alloc_page_node(page_node);
        }
        else{
            printk_port("I'm pid %d tid %d and I'm blocked\n", current_running->pid, current_running->tid);
            do_block(&current_running->list, &waitPageList);
            do_scheduler();
        }
    }
    // log(DEBUG, "alloc: %lx by tid %d\n",ret, current_running->tid);
    return ret;
}

/* free this page: baseAddr */
void freePage(ptr_t baseAddr)
{   
    // for (list_node_t *temp = freePageList.next; temp != &freePageList; temp = temp->next){
    //     if (baseAddr == temp->ptr)
    //         assert(0);
    // }
    page_node_t *page_node = &pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(baseAddr)];
    uint8_t ret = free_page_node(page_node);
    /* unblock the process whose pid is the smallest in the list */
    /* threads have the same pid, but it will be blocked to list end (see do_block) */
    /* so here we choose the first one, if several processes are blocked */
    if (!ret){
        pcb_t *blocked_proc = NULL, *smallest_pid_blocked_proc = NULL;
        pid_t smallest_pid = 0; /* every process has pid larger than 0 */
        list_for_each_entry(blocked_proc, &waitPageList, list){
            if (smallest_pid == 0 || smallest_pid > blocked_proc->pid){
                smallest_pid = blocked_proc->pid;
                smallest_pid_blocked_proc = blocked_proc;
            }
        }
        if (smallest_pid_blocked_proc){
            printk_port("pid %d tid %d is to be unblocked", smallest_pid_blocked_proc->pid, smallest_pid_blocked_proc->tid);
            do_unblock(smallest_pid_blocked_proc);
        }
    }
}

void *kmalloc(size_t size)
{
    debug();
    log(0, "dangerous malloc");
    memalloc -= size;
    memalloc -= 64 - (size%64);
    return memalloc;
}

/* set this page to be COW */
uintptr_t set_page_kva_copy_on_write(uint64_t page_kva, int is_writable)
{
    page_node_t *page_node = &pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)];
    page_node->share_num++;
    if (!page_node->is_shared && is_writable)
        page_node->copy_on_write = 1;
}

/* set file_map_info point to file map information(a page kva) */
/* set dirty = 0 */
uintptr_t set_page_kva_file_map_info(uintptr_t page_kva, void *file_map_info)
{
    memcpy(&pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info, file_map_info, sizeof(fat32_file_map_info_t));
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_accessed = 0;
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_dirty = 0;
}

void map_normal_page(uint64_t va, uint64_t kva, uint64_t pgdir, uint64_t mask)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
        ptr = pgdir2 + vpn1*sizeof(PTE);
    }
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
        ptr = pgdir2 + vpn0*sizeof(PTE);
    }
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    uint64_t pfn2 = (kva2pa(kva)&VA_MASK) >> NORMAL_PAGE_SHIFT;
    assert(*ptr == 0);
    set_pfn(ptr,pfn2);
    set_attribute(ptr, _PAGE_PRESENT|_PAGE_USER|mask|_PAGE_ACCESSED|_PAGE_DIRTY);
}

/* add attribute to this virtual address */
/* return mask if success, 0 if fail */
uint64_t add_page_uva_attribute(uint64_t va, uint64_t pgdir, uint64_t mask)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT))
        return 0;
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT))
        return 0;
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    add_attribute(ptr, mask);
    local_flush_tlb_all();
    return mask;
}

/* map this uva to kva for all threads in current_running */
/* cannot be used for clone */
uintptr_t share_page_helper(uintptr_t uva, uintptr_t kva, uintptr_t mask)
{
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(kva)].is_shared = 1;
    // for (uint32_t j = 0; j < NUM_MAX_TASK; j++){
    //     if (!is_exited(&pcb[j]) && current_running != &pcb[j] && pcb[j].pid == current_running->pid){
    //         map_normal_page(uva, kva, pcb[j].pgdir, mask);
    //         pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(kva)].share_num++;
    //     }
    // }
}

// pgdir and kernel_stack_base are kva
void free_all_pages(uint64_t pgdir, uint64_t kernel_stack_base)
{
    // make sure no page fault
    // set_satp(SATP_MODE_SV39,0,PGDIR_PA >> NORMAL_PAGE_SHIFT);
    // local_flush_tlb_all();
    // free page
    for (uint32_t i = 0; i < NUM_PTE_ENTRY; i++)
    {
        // int clear1 = 1;
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT)&&get_attribute(*ptr1,_PAGE_USER)){
            // level1 valid, goto level2
            for (uint32_t j = 0; j < NUM_PTE_ENTRY; j++){
                PTE* ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT)&&get_attribute(*ptr2,_PAGE_USER)){
                    // level2 valid, goto level3
                    for (uint32_t k = 0; k < NUM_PTE_ENTRY; k++){
                        PTE* ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        if (get_attribute(*ptr3,_PAGE_PRESENT)&&get_attribute(*ptr3,_PAGE_USER)){
                            //level3 valid, goto free
                            freePage(pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT));
                        }
                    }
                    freePage(pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT));
                }
            }
            freePage(pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT));
        }
    }
    freePage(pgdir);
    freePage(kernel_stack_base);
}

/**
 this is used for mapping kernel virtual address into user page table 
 if page is valid, then no share
 guarantee ioremap
 */
void share_pgtable(uintptr_t dest_pgdir, uintptr_t src_pgdir)
{
    PTE* dest = dest_pgdir, *src = src_pgdir;
    for (int i = 0; i < NUM_PTE_ENTRY; ++i)
    {
        PTE *tmp1 = src + i, *tmp2 = dest + i;
        if (get_attribute(*tmp1,_PAGE_PRESENT) && !get_attribute(*tmp2,_PAGE_PRESENT))
            *tmp2 = *tmp1;
    }
}

/* allocate physical page for `va`, mapping it into `pgdir`,
   return the kernel virtual address for the page.
   */
/* User are set */
uintptr_t alloc_page_helper(uintptr_t va, uintptr_t pgdir, uint64_t mask)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2*sizeof(PTE);
    // 2  
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
        ptr = pgdir2 + vpn1*sizeof(PTE);
    }
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    // 1
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
        ptr = pgdir2 + vpn0*sizeof(PTE);
    }
    else
        ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    // 0
    uint64_t ret;
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER|mask|_PAGE_ACCESSED|_PAGE_DIRTY);
        ret = pgdir2;
    }
    else
        ret = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT);
    local_flush_tlb_all();
    return ret;
}

/* free page using page user addr */
PTE free_page_helper(uintptr_t va, uintptr_t pgdir)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr1 = pgdir + vpn2*sizeof(PTE);
    PTE *ptr2, *ptr3;
    PTE ret = 0;
    // 2  
    if (!get_attribute(*ptr1,_PAGE_PRESENT))
        return ret;
    else
        ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    // 1
    if (!get_attribute(*ptr2,_PAGE_PRESENT))
        return ret;
    else
        ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    // 0
    if (!get_attribute(*ptr3,_PAGE_PRESENT))
        return ret;
    else{
        ret = *ptr3;
        // a. free real page
        *ptr3 = 0;
        freePage(PTE2kva(ret));
    }
    // b. free 3 level page
    ptr3 = (PTE *)PAGE_ALIGN((uintptr_t)ptr3);
    uint32_t i;
    for (i = 0; i < NUM_PTE_ENTRY; i++)
        if (*(ptr3 + i))
            break;
    if (i < NUM_PTE_ENTRY){
        /* somebody else still use this page table */
        local_flush_tlb_all();
        return ret;
    }
    /* this page table is all zero */
    *ptr2 = 0;
    freePage(ptr3);
    // c. free 2 level page
    ptr2 = (PTE *)PAGE_ALIGN((uintptr_t)ptr2);
    for (i = 0; i < NUM_PTE_ENTRY; i++)
        if (*(ptr2 + i))
            break;
    if (i < NUM_PTE_ENTRY){
        /* somebody else still use this page table */
        local_flush_tlb_all();
        return ret;
    }
    /* this page table is all zero */
    *ptr1 = 0;
    freePage(ptr2);

    /* pgdir should never be freed */

    local_flush_tlb_all();
    return ret;
}

/* return pcb's free space */
static uintptr_t find_my_free_space(pcb_t *pcb, size_t len)
{
    uintptr_t start = pcb->user_stack_base - 2*NORMAL_PAGE_SIZE; /* pile start */
    uint64_t temp = 0;
    uintptr_t page_kva = NULL;
    while (temp < len){
        if (probe_kva_of(start, pcb->pgdir))
            temp = 0;
        else
            temp += NORMAL_PAGE_SIZE;
        start -= NORMAL_PAGE_SIZE;
    }
    return start + NORMAL_PAGE_SIZE;
}
/* find free space for mmap */
/* if shared, must find a space that all threads are available on */
uintptr_t find_free_space_and_set(size_t len)
{
    uintptr_t ret = find_my_free_space(current_running, len);
    if (ret < get_user_addr_top(current_running))
        set_user_addr_top(current_running, ret);
    
    // log2(0, "final free space is at %lx", ret);
    return ret;
}

uintptr_t directmap(uintptr_t kva, uintptr_t pgdir)
{
    return kva;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void)
{
    return (void *)allocPage();
}

void kfree(void *p)
{
    freePage(p);
}

void *allocproc()
{
    return MEM_FOR_PROC;
}

void allocfree()
{
    return ;
}

/* Only swap its own unshared page
    no other processes' page */
uint64_t swap_page()
{
    uint64_t page_basekva = find_swap_page_kva(current_running->pgdir);
    assert(page_basekva);
    unsigned int block_id;
    uint64_t mem_address = kva2pa(page_basekva);
    if (!list_empty(&availableSwapSpace)){
        swappage_node_t *temp = availableSwapSpace.next->ptr;
        block_id = temp->block_id;
        temp->page_basekva = page_basekva;
        list_del(&temp->list);list_add_tail(&temp->list,&swapPageList);
    }
    else{
        block_id = start_block_id; start_block_id += BLOCKS_PER_PAGE;
        swappage_node_t *temp = (swappage_node_t *)kmalloc(sizeof(swappage_node_t));
        temp->list.ptr = temp; temp->block_id = block_id; 
        temp->page_basekva = page_basekva;
        list_add_tail(&temp->list,&swapPageList);
    }
    sbi_sd_write(mem_address, BLOCKS_PER_PAGE, block_id);
    return page_basekva;
}

/* NRU */
static uint64_t find_swap_page_kva(uint64_t pgdir)
{
    for (int i = 0; i < NUM_USER_PTE_ENTRY; ++i)
    {
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT) && get_attribute(*ptr1,_PAGE_USER)){
            for (int j = 0; j < NUM_PTE_ENTRY ; ++j)
            {
                PTE *ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT) && get_attribute(*ptr2,_PAGE_USER)){
                    for (int k = 0; k < NUM_PTE_ENTRY; ++k)
                    {
                        PTE *ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        // if (get_attribute(*ptr3,_PAGE_SHARE) || get_attribute(*ptr3,_PAGE_EXEC))
                        //     continue;
                        if (get_attribute(*ptr3,_PAGE_PRESENT) && get_attribute(*ptr3,_PAGE_USER)
                                && !get_attribute(*ptr3,_PAGE_ACCESSED) && !get_attribute(*ptr3,_PAGE_DIRTY)){
                            clear_attribute(ptr3,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
                            set_attribute(ptr3,_PAGE_SWAP);
                            return pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT);
                        }
                            
                    }
                }
            }
        } 
    }
    for (int i = 0; i < NUM_USER_PTE_ENTRY; ++i)
    {
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT)&&get_attribute(*ptr1,_PAGE_USER)){
            for (int j = 0; j < NUM_PTE_ENTRY ; ++j)
            {
                PTE *ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT)&&get_attribute(*ptr2,_PAGE_USER)){
                    for (int k = 0; k < NUM_PTE_ENTRY; ++k)
                    {
                        PTE *ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        // if (get_attribute(*ptr3,_PAGE_SHARE)||get_attribute(*ptr3,_PAGE_EXEC))
                        //     continue;
                        if (get_attribute(*ptr3,_PAGE_PRESENT)&&get_attribute(*ptr3,_PAGE_USER)
                                &&!get_attribute(*ptr3,_PAGE_ACCESSED)&&get_attribute(*ptr3,_PAGE_DIRTY)){
                            clear_attribute(ptr3,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
                            set_attribute(ptr3,_PAGE_SWAP);
                            return pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT);
                        }
                    }
                }
            }
        } 
    }
    for (int i = 0; i < NUM_USER_PTE_ENTRY; ++i)
    {
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT)&&get_attribute(*ptr1,_PAGE_USER)){
            for (int j = 0; j < NUM_PTE_ENTRY ; ++j)
            {
                PTE *ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT)&&get_attribute(*ptr2,_PAGE_USER)){
                    for (int k = 0; k < NUM_PTE_ENTRY; ++k)
                    {
                        PTE *ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        // if (get_attribute(*ptr3,_PAGE_SHARE)||get_attribute(*ptr3,_PAGE_EXEC))
                        //     continue;
                        if (get_attribute(*ptr3,_PAGE_PRESENT)&&get_attribute(*ptr3,_PAGE_USER)
                                &&get_attribute(*ptr3,_PAGE_ACCESSED)&&!get_attribute(*ptr3,_PAGE_DIRTY)){
                            clear_attribute(ptr3,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
                            set_attribute(ptr3,_PAGE_SWAP);
                            return pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT);
                        }
                    }
                }
            }
        } 
    }
    for (int i = 0; i < NUM_USER_PTE_ENTRY; ++i)
    {
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT)&&get_attribute(*ptr1,_PAGE_USER)){
            for (int j = 0; j < NUM_PTE_ENTRY ; ++j)
            {
                PTE *ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT)&&get_attribute(*ptr2,_PAGE_USER)){
                    for (int k = 0; k < NUM_PTE_ENTRY; ++k)
                    {
                        PTE *ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        // if (get_attribute(*ptr3,_PAGE_SHARE)||get_attribute(*ptr3,_PAGE_EXEC))
                        //     continue;
                        if (get_attribute(*ptr3,_PAGE_PRESENT)&&get_attribute(*ptr3,_PAGE_USER)
                                &&get_attribute(*ptr3,_PAGE_ACCESSED)&&get_attribute(*ptr3,_PAGE_DIRTY)){
                            clear_attribute(ptr3,_PAGE_PRESENT|_PAGE_ACCESSED|_PAGE_DIRTY);
                            set_attribute(ptr3,_PAGE_SWAP);
                            return pa2kva(get_pfn(*ptr3) << NORMAL_PAGE_SHIFT);
                        }
                    }
                }
            }
        } 
    }
    return NULL;
}

/* file map write back using page kva */
/* success return 0, fail return 1 */
uint8_t file_map_write_back_page_kva(uintptr_t page_kva)
{
    return file_map_write_back(&pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info);
}

uint8_t is_page_kva_copy_on_write(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].copy_on_write;
}

void clear_copy_on_write(uintptr_t page_kva)
{
    // log2(0, "clear copy on write: %lx %d", page_kva, current_running->tid);
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].copy_on_write = 0;
}
uint8_t is_page_kva_shared(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_shared;
}

uint8_t is_page_kva_swapped(uintptr_t page_kva)
{
    return 0;
}

uint8_t is_page_kva_free(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].share_num == 0;
}

uint8_t is_page_kva_file_mapped(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info.len != 0;
}

uint8_t is_page_kva_file_map_writable(uintptr_t page_kva)
{
    return (pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info.len != 0 )
            && ((pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info.flags & O_WRONLY) != 0);
}

uint8_t is_page_kva_file_map_readable(uintptr_t page_kva)
{
    return (pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info.len != 0 )
            && ((pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info.flags & O_RDONLY) != 0);
}

uint8_t is_page_kva_file_map_accessed(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_accessed;
}

uint8_t is_page_kva_file_map_dirty(uintptr_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_dirty;
}

uint8_t set_page_kva_file_map_accessed(uintptr_t page_kva)
{
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_accessed = 1;
}

uint8_t set_page_kva_file_map_dirty(uintptr_t page_kva)
{
    pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].is_file_map_dirty = 1;
}

fat32_file_map_info_t *get_page_kva_file_map_info(uintptr_t page_kva)
{
    return &(pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].file_map_info);
}

uint32_t get_page_kva_share_num(uint64_t page_kva)
{
    return pageRecyc[PAGE_KVA_TO_PAGE_NODE_INDEX(page_kva)].share_num;
}