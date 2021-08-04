#ifndef MM_H
#define MM_H

#include <type.h>
#include <os/list.h>
#include <pgtable.h>

#define MEM_SIZE 32
#define PAGE_SIZE 4096 // 4K
#define BLOCK_SIZE 512
#define BLOCKS_PER_PAGE (PAGE_SIZE/BLOCK_SIZE)
#define INIT_KERNEL_STACK 0xffffffff80400000lu
#define FREEMEM (INIT_KERNEL_STACK+4*PAGE_SIZE)
#define USER_STACK_ADDR 0x3fffab0000lu
#define USER_PILE_ADDR 0x20000000lu
#define FREEMEM_TOP KERNEL_END
#define MEM_FOR_PROC 0xffffffff80150000lu

/* Rounding; only works for n = power of two */
#define ROUND(a, n)     (((((uint64_t)(a))+(n)-1)) & ~((n)-1))
#define ROUNDDOWN(a, n) (((uint64_t)(a)) & ~((n)-1))

extern ptr_t memCurr;

typedef struct swappage
{
    unsigned int block_id;
    uint64_t page_basekva;
    list_node_t list;
}swappage_node_t;

typedef struct shm_page
{
    unsigned int totalnum;
    uint64_t page_basekva;
    int key;
    list_node_t list;
}shm_page_node_t;
extern list_head swapPageList;
extern list_head availableSwapSpace;

extern ptr_t allocPage();
extern void freePage(ptr_t baseAddr);
extern void* kmalloc(size_t size);
extern void share_pgtable(uintptr_t dest_pgdir, uintptr_t src_pgdir);
extern uintptr_t alloc_page_helper(uintptr_t va, uintptr_t pgdir, uint64_t mask);
uintptr_t shm_page_get(int key);
void shm_page_dt(uintptr_t addr);
uintptr_t directmap(uintptr_t kva, uintptr_t pgdir);
void free_all_pages(uint64_t pgdir, uint64_t kernel_stack_base);

uint64_t do_mmap(uint64_t start,uint32_t len, uint8 prot, uint8 flags,uint32_t fd,uint32_t off);
int64_t do_brk(uintptr_t ptr);

void *kalloc(void);
void kfree(void *p);

void *allocproc();
void allocfree();

void init_recyc();

#endif /* MM_H */
