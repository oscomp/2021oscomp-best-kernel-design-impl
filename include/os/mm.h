#ifndef MM_H
#define MM_H

#include <type.h>
#include <os/list.h>
#include <pgtable.h>
#include <qemu.h>
#include <log.h>

#define MEM_SIZE 32
#define PAGE_SIZE 4096 // 4K
#define BLOCK_SIZE 512
#define BLOCKS_PER_PAGE (PAGE_SIZE/BLOCK_SIZE)

#ifdef K210
    #define INIT_KERNEL_STACK 0xffffffff80400000lu
#else
    #define INIT_KERNEL_STACK 0xffffffff80b00000lu
#endif

#define FREEMEM (INIT_KERNEL_STACK+4*PAGE_SIZE)
#define USER_STACK_ADDR 0x3ffff2a000lu
#define USER_PILE_ADDR 0x20000000lu
#define FREEMEM_TOP KERNEL_END
#define MEM_FOR_PROC 0xffffffff80150000lu

#define KERNEL_STACK_SIZE NORMAL_PAGE_SIZE
#define PAGES_PER_USER_STACK 10
#define USER_STACK_INIT_SIZE (PAGES_PER_USER_STACK * NORMAL_PAGE_SIZE)

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

extern ptr_t memCurr;
extern list_head freePageList;
extern list_head swapPageList;
extern list_head availableSwapSpace;

extern ptr_t allocPage();
extern void freePage(ptr_t baseAddr);
extern void* kmalloc(size_t size);
extern void share_pgtable(uintptr_t dest_pgdir, uintptr_t src_pgdir);
extern uintptr_t alloc_page_helper(uintptr_t va, uintptr_t pgdir, uint64_t mask);
uintptr_t free_page_helper(uintptr_t va, uintptr_t pgdir);
uintptr_t shm_page_get(int key);
void shm_page_dt(uintptr_t addr);
uintptr_t directmap(uintptr_t kva, uintptr_t pgdir);
void free_all_pages(uint64_t pgdir, uint64_t kernel_stack_base);

#define MAP_SHARED 0x1
#define MAP_PRIVATE 0x2
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MMAP_ALLOC_PAGE_FD -1
int64 do_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off);
int64 do_munmap(void *start, size_t len);
int do_msync(void *addr, size_t length, int flags);
int64_t do_brk(uintptr_t ptr);
int do_mprotect(void *addr, size_t len, int prot);

void *kalloc(void);
void kfree(void *p);

void *allocproc();
void allocfree();

void init_recyc();
void handle_memory_leak(void *pcb);

#endif /* MM_H */
