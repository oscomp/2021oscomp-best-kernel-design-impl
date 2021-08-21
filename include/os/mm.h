#ifndef MM_H
#define MM_H

#include <type.h>
#include <os/list.h>
#include <pgtable.h>
#include <qemu.h>
#include <log.h>
#include <utils/utils.h>
#include <os/filemap.h>

#define MEM_SIZE 32
#define PAGE_SIZE 4096 // 4K
#define BLOCK_SIZE 512
#define BLOCKS_PER_PAGE (PAGE_SIZE/BLOCK_SIZE)

#ifdef K210
    #define INIT_KERNEL_STACK 0xffffffff80280000lu
#else
    #define INIT_KERNEL_STACK 0xffffffff80980000lu
#endif

#define FREEMEM (INIT_KERNEL_STACK+4*PAGE_SIZE)
#define USER_STACK_ADDR 0x3ffff2a000lu
#define USER_PILE_ADDR 0x20000000lu
#define FREEMEM_TOP KERNEL_END
#define MEM_FOR_PROC 0xffffffff80020000lu

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

typedef struct page_node{
    list_node_t list; /* ptr == page kernel virtual address, tid is the thread ID who allocates it */
    uint32_t share_num; /* share page number */
    uint8_t copy_on_write;   /* copy-on-write property */
    fat32_file_map_info_t file_map_info; /* if non Zero, point to file map information */
    uint8_t is_file_map_accessed;
    uint8_t is_file_map_dirty;
    uint8_t is_shared;  /* is this a real shared page? share num > 1 is not enouch */
    uint8_t is_swapped; /* is this page swapped to disk? */
}page_node_t;

extern ptr_t memCurr;
extern list_head freePageList;
extern list_head swapPageList;
extern list_head availableSwapSpace;

extern ptr_t allocPage();
extern void freePage(ptr_t baseAddr);
extern void* kmalloc(size_t size);
extern void share_pgtable(uintptr_t dest_pgdir, uintptr_t src_pgdir);
/* helpers */
uintptr_t alloc_page_helper(uintptr_t va, uintptr_t pgdir, uint64_t mask);
PTE free_page_helper(uintptr_t va, uintptr_t pgdir);
uintptr_t share_page_helper(uintptr_t uva, uintptr_t kva, uintptr_t mask);

void shm_page_dt(uintptr_t addr);
uintptr_t directmap(uintptr_t kva, uintptr_t pgdir);
void free_all_pages(uint64_t pgdir, uint64_t kernel_stack_base);
uintptr_t set_page_kva_copy_on_write(uint64_t page_kva, int is_writable);
uint8_t set_page_kva_file_map_accessed(uintptr_t page_kva);
uint8_t set_page_kva_file_map_dirty(uintptr_t page_kva);
fat32_file_map_info_t *get_page_kva_file_map_info(uintptr_t page_kva);

uint8_t is_page_kva_shared(uintptr_t page_kva);
uint8_t is_page_kva_copy_on_write(uintptr_t page_kva);
uint8_t is_page_kva_free(uintptr_t page_kva);
uint8_t is_page_kva_file_mapped(uintptr_t page_kva);
uint8_t is_page_kva_file_map_writable(uintptr_t page_kva);
uint8_t is_page_kva_file_map_readable(uintptr_t page_kva);
uint8_t is_page_kva_file_map_accessed(uintptr_t page_kva);
uint8_t is_page_kva_file_map_dirty(uintptr_t page_kva);

#define MAP_SHARED 0x1
#define MAP_PRIVATE 0x2
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MMAP_ALLOC_PAGE_FD -1
void init_recyc();
int64 do_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off);
int64 do_munmap(void *start, size_t len);
int do_msync(void *addr, size_t length, int flags);
int64_t do_brk(uintptr_t ptr);
int do_mprotect(void *addr, size_t len, int prot);

void *kalloc(void);
void kfree(void *p);

void *allocproc();

void allocfree();

void handle_memory_leak(void *pcb);
void map_normal_page(uint64_t va, uint64_t kva, uint64_t pgdir, uint64_t mask);
uint64_t prot_to_mask(int prot);
uintptr_t find_free_space_and_set(size_t len);
uintptr_t set_page_kva_file_map_info(uintptr_t page_kva, void *file_map_info);
uint32_t get_page_kva_share_num(uint64_t page_kva);
void clear_copy_on_write(uintptr_t page_kva);
uint64_t add_page_uva_attribute(uint64_t va, uint64_t pgdir, uint64_t mask);
uint8_t file_map_write_back_page_kva(uintptr_t page_kva);

#endif /* MM_H */
