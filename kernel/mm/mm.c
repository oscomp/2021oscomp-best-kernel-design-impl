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

ptr_t memCurr = FREEMEM;
// ptr_t memTop = KERNEL_END;
unsigned int start_block_id = 1024;
static ptr_t memalloc = KERNEL_END;

LIST_HEAD(freePageList);
static LIST_HEAD(shmPageList);
static LIST_HEAD(freePagebackupList);
LIST_HEAD(swapPageList);
LIST_HEAD(availableSwapSpace);

static uint64_t swap_page();
static uint64_t find_swap_page_kva(uint64_t pgdir);

#define NUM_REC ((KERNEL_END - FREEMEM) / NORMAL_PAGE_SIZE)
list_node_t pageRecyc[NUM_REC];

void init_recyc()
{
    for (uint32 i = 0; i < NUM_REC; ++i)
        list_add_tail(&pageRecyc[i], &freePagebackupList);
}

ptr_t allocPage()
{
    ptr_t ret;
    // memCurr += NORMAL_PAGE_SIZE;
    // ret = memCurr - NORMAL_PAGE_SIZE;
    if (!list_empty(&freePageList)){
        list_node_t *tmp = freePageList.next;
        // prints("666 %lx\n",freePageList.next);
        list_del(tmp);
        list_add_tail(tmp, &freePagebackupList);
        // printk_port("777\n");
        ret = tmp->ptr;
    }
    else if (memCurr < memalloc){
        memCurr += NORMAL_PAGE_SIZE;
        ret = memCurr - NORMAL_PAGE_SIZE;
    }
    else
        assert(0);
    // log(DEBUG, "alloc: %lx\n",ret);
    return ret;
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

void freePage(ptr_t baseAddr)
{   
    // printk_port("free :%lx\n", baseAddr);
    uint8_t clear = 1;
    for (list_node_t* i = shmPageList.next; i != &shmPageList; i=i->next)
    {
        assert(0);
        shm_page_node_t *temp = i->ptr;
        if (temp->page_basekva == baseAddr){
            if (temp->totalnum > 1){
                temp->totalnum--;clear = 0;
            }
            else
                list_del(&temp->list);
            break;
        }
    }
    if (clear){
        // printk_port("sizeof : %d\n", sizeof(list_node_t));
        list_node_t *tmp = freePagebackupList.next;
        assert(tmp != &freePagebackupList);
        // printk_port("free: %lx\n", baseAddr);
        tmp->ptr = baseAddr;
        list_del(tmp);
        list_add_tail(tmp,&freePageList);
    }
    // vt100_move_cursor(1,10);
    // printk("Free:%x",baseAddr);
}

void *kmalloc(size_t size)
{
    debug();
    log(0, "dangerous malloc");
    memalloc -= size;
    memalloc -= 64 - (size%64);
    return memalloc;
}

uint64_t find_freepage_uva(uint64_t pgdir)
{
    uint64_t vpn2,vpn1,vpn0;
    for (int i = 0; i < NUM_USER_PTE_ENTRY; ++i)
    {
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (!get_attribute(*ptr1,_PAGE_PRESENT)&&!get_attribute(*ptr1,_PAGE_SWAP)){
            vpn2 = i; vpn1 = 0; vpn0 = 0;
            return (vpn2 << VA_VPN2_SHIFT)|(vpn1<<VA_VPN1_SHIFT)|(vpn0<<VA_VPN0_SHIFT);
        }
        else{
            for (int j = 0; j < NUM_PTE_ENTRY; ++j){
                PTE *ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (!get_attribute(*ptr2,_PAGE_PRESENT)&&!get_attribute(*ptr2,_PAGE_SWAP)){
                    vpn2 = i; vpn1 = j; vpn0 = 0;
                    return (vpn2 << VA_VPN2_SHIFT)|(vpn1<<VA_VPN1_SHIFT)|(vpn0<<VA_VPN0_SHIFT);
                }
                else{                   
                    for (int k = (i+j==0)? 1 : 0; k < NUM_PTE_ENTRY; ++k)
                    {
                        PTE *ptr3 = pa2kva(get_pfn(*ptr2) << NORMAL_PAGE_SHIFT) + k*sizeof(PTE);
                        if (!get_attribute(*ptr3,_PAGE_PRESENT)&&!get_attribute(*ptr3,_PAGE_SWAP)){
                            vpn2 = i; vpn1 = j; vpn0 = k;
                            return (vpn2 << VA_VPN2_SHIFT)|(vpn1<<VA_VPN1_SHIFT)|(vpn0<<VA_VPN0_SHIFT);
                        }
                    }
                }
            }
        }
    }
}

uintptr_t shm_page_get(int key)
{
    uint64_t freepage_base_uva = find_freepage_uva(current_running->pgdir);
    for (list_node_t* i = shmPageList.next; i != &shmPageList; i=i->next)
    {
        shm_page_node_t *temp = i->ptr;
        if (temp->key == key){        
            mapping_page(freepage_base_uva,temp->page_basekva,current_running->pgdir);
            temp->totalnum++;
            return freepage_base_uva;
        } //found
    }
    shm_page_node_t *new = (shm_page_node_t*)kmalloc(sizeof(shm_page_node_t));
    new->totalnum = 1;
    new->page_basekva = alloc_page_helper(freepage_base_uva,current_running->pgdir,_PAGE_READ|_PAGE_WRITE);
    mapping_page(freepage_base_uva,new->page_basekva,current_running->pgdir);
    new->key = key;
    new->list.ptr = new;
    list_add_tail(&new->list,&shmPageList);
    return freepage_base_uva;
}

void shm_page_dt(uintptr_t addr)
{
    freePage(get_kva_of(addr,current_running->pgdir));
    uint64_t va = addr;
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = current_running->pgdir + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_SWAP)) assert(0)
    else ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_SWAP)) assert(0)
    else ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_SWAP)) assert(0)
    else *ptr = 0;
}

void mapping_page(uint64_t va,uint64_t kva,uint64_t pgdir)
{
    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pgdir + vpn2*sizeof(PTE);
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_SWAP))
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
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_SWAP))
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
    set_pfn(ptr,pfn2);
    set_attribute(ptr,_PAGE_SHARE|_PAGE_READ|_PAGE_WRITE|_PAGE_PRESENT|_PAGE_USER);
}

// pgdir and kernel_stack_base are kva
void free_all_pages(uint64_t pgdir, uint64_t kernel_stack_base)
{
    // make sure no page fault
    set_satp(SATP_MODE_SV39,0,PGDIR_PA >> NORMAL_PAGE_SHIFT);
    // free page
    for (uint32_t i = 0; i < NUM_PTE_ENTRY; ++i)
    {
        // int clear1 = 1;
        PTE *ptr1 = pgdir + i*sizeof(PTE);
        if (get_attribute(*ptr1,_PAGE_PRESENT)&&get_attribute(*ptr1,_PAGE_USER)){
            // level1 valid, goto level2
            for (uint32_t j = 0; j < NUM_PTE_ENTRY; ++j){
                PTE* ptr2 = pa2kva(get_pfn(*ptr1) << NORMAL_PAGE_SHIFT) + j*sizeof(PTE);
                if (get_attribute(*ptr2,_PAGE_PRESENT)&&get_attribute(*ptr2,_PAGE_USER)){
                    // level2 valid, goto level3
                    for (uint32_t k = 0; k < NUM_PTE_ENTRY; ++k){
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
    // uint64_t kernel_stack_page = (kernel_stack%NORMAL_PAGE_SIZE)? 
    //         kernel_stack-(kernel_stack % NORMAL_PAGE_SIZE) : kernel_stack - NORMAL_PAGE_SIZE;
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
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_PRESENT))
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
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_PRESENT))
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
    if (!get_attribute(*ptr,_PAGE_PRESENT)&&!get_attribute(*ptr,_PAGE_PRESENT))
    {
        uintptr_t pgdir2 = allocPage();
        clear_pgdir(pgdir2);
        uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
        set_pfn(ptr,pfn2);
        set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER|mask);
        ret = pgdir2;
    }
    else
        ret = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT);
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
