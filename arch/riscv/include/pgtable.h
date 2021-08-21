#ifndef PGTABLE_H
#define PGTABLE_H

#include <type.h>
#include <sbi.h>
#include <assert.h>
#include <qemu.h>

#define SATP_MODE_SV39 8
#define SATP_MODE_SV48 9

#define SATP_ASID_SHIFT 44lu
#define SATP_MODE_SHIFT 60lu

#define NORMAL_PAGE_SHIFT 12lu
#define NORMAL_PAGE_SIZE (1lu << NORMAL_PAGE_SHIFT)
#define LARGE_PAGE_SHIFT 21lu
#define LARGE_PAGE_SIZE (1lu << LARGE_PAGE_SHIFT)
#define SUPER_PAGE_SHIFT 30lu
#define SUPER_PAGE_SIZE (1lu << SUPER_PAGE_SHIFT)

#define PAGE_OFFSET(ptr) \
    ( (ptr) & (NORMAL_PAGE_SIZE - 1lu) )
#define PAGE_ALIGN(ptr) \
    ( (ptr) & ~(NORMAL_PAGE_SIZE - 1lu) )
#define PAGE_ALIGNUP(ptr) \
    ( ((ptr) + NORMAL_PAGE_SIZE - 1lu) & ~(NORMAL_PAGE_SIZE - 1lu) )

#define START_ENTRYPOINT 0xffffffff80000000lu

#ifdef K210
    #define KERNEL_ENTRYPOINT 0xffffffff80134000lu
    #define KERNEL_END 0xffffffff80600000lu
#else
    #define KERNEL_ENTRYPOINT 0xffffffff80600000lu
    #define KERNEL_END 0xffffffff80d00000lu
#endif

/* level1 pgdir */
#ifdef K210
    #define PGDIR_PA ((KERNEL_ENTRYPOINT & 0xfffffffflu) - 0x3000) 
#else
    #define PGDIR_PA ((KERNEL_ENTRYPOINT & 0xfffffffflu) - 0x10000)
#endif


#define BOOT_KERNEL (START_ENTRYPOINT & 0xfffffffflu)
#define BOOT_KERNEL_END (KERNEL_END & 0xfffffffflu)

/* mprotect */
#define PROT_NONE 0x0
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_GROWSUP 0x2000000
#define PROT_GROWSDOWN 0x1000000



/*
 * Flush entire local TLB.  'sfence.vma' implicitly fences with the instruction
 * cache as well, so a 'fence.i' is not necessary.
 */
static inline void local_flush_tlb_all(void)
{
    __asm__ __volatile__ ("fence\nfence.i\nsfence.vma" : : : "memory");
}

/* Flush one page from local TLB */
static inline void local_flush_tlb_page(unsigned long addr)
{
    __asm__ __volatile__ ("sfence.vma %0" : : "r" (addr) : "memory");
}

static inline void local_flush_cache_all(void)
{
    __asm__ __volatile__ ("fence\nfence.i" ::: "memory");
}

static inline void local_flush_icache_all(void)
{
    __asm__ __volatile__ ("fence.i" ::: "memory");
}

static inline void local_flush_dcache_all(void)
{
    __asm__ __volatile__ ("fence" ::: "memory");
}


static inline void flush_icache_all(void)
{
    local_flush_icache_all();
    sbi_remote_fence_i(NULL);
}

static inline void flush_tlb_all(void)
{
    local_flush_tlb_all();
    sbi_remote_sfence_vma(NULL, 0, -1);
}
static inline void flush_tlb_page_all(unsigned long addr)
{
    local_flush_tlb_page(addr);
    sbi_remote_sfence_vma(NULL, 0, -1);
}

static inline void set_satp(
    unsigned mode, unsigned asid, unsigned long ppn)
{
    unsigned long __v =
        (unsigned long)(((unsigned long)mode << SATP_MODE_SHIFT) | ((unsigned long)asid << SATP_ASID_SHIFT) | ppn);
    __asm__ __volatile__("csrw satp, %0" : : "rK"(__v) : "memory");
}

// static inline void set_satp(
//     unsigned mode, unsigned asid, unsigned long ppn)
// {
//     unsigned long __v =
//         (unsigned long)(((unsigned long)asid << 38) | ppn);
//     __asm__ __volatile__("fence\nfence.i\nsfence.vm\nfence\nfence.i\ncsrw satp, %0\nfence\nfence.i\nsfence.vm\nfence\nfence.i" : : "rK"(__v) : "memory");
// }

/*
 * PTE format:
 * | XLEN-1  10 | 9             8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
 *       PFN      reserved for SW   D   A   G   U   X   W   R   V
 */

#define _PAGE_ACCESSED_OFFSET 6

#define _PAGE_PRESENT (1 << 0)
#define _PAGE_READ (1lu << 1)     /* Readable */
#define _PAGE_WRITE (1lu << 2)    /* Writable */
#define _PAGE_EXEC (1lu << 3)     /* Executable */
#define _PAGE_USER (1 << 4)     /* User */
#define _PAGE_GLOBAL (1 << 5)   /* Global */
#define _PAGE_ACCESSED (1 << 6) /* Set by hardware on any access \
                                 */
#define _PAGE_DIRTY (1 << 7)    /* Set by hardware on any write */
#define _PAGE_SWAP (1 << 8)     /* Swapped to SD-card */
#define _PAGE_SHARE (1 << 9)    /* Shared page */

#define _PAGE_ALL_MOD (_PAGE_READ | _PAGE_WRITE | _PAGE_EXEC)

#define _PAGE_PFN_SHIFT 10lu

#define VA_MASK ((1lu << 39) - 1)

#define PPN_BITS 9lu
#define NUM_PTE_ENTRY (1 << PPN_BITS)
#define NUM_USER_PTE_ENTRY 0x100

#define VA_VPN0_SHIFT 12lu
#define VA_VPN1_SHIFT 21lu
#define VA_VPN2_SHIFT 30lu

typedef uint64_t PTE;

static inline uintptr_t kva2pa(uintptr_t kva)
{
    return kva - 0xffffffff00000000;
}

static inline uintptr_t pa2kva(uintptr_t pa)
{
    return pa + 0xffffffff00000000;
}

static inline uint64_t get_pa(PTE entry)
{
    return 0;
}

static inline uint64_t kva2pfn(uintptr_t kva)
{
    return (kva2pa(kva) & VA_MASK) >> NORMAL_PAGE_SHIFT;
}

/* Get/Set page frame number of the `entry` */
static inline uint64_t get_pfn(PTE entry)
{
    return (entry >> _PAGE_PFN_SHIFT) & VA_MASK;
}

static inline void set_pfn(PTE *entry, uint64_t pfn)
{
    *entry = (*entry & ((1<<_PAGE_PFN_SHIFT)-1)) | (pfn << _PAGE_PFN_SHIFT);
}

static inline uint64_t PTE2pa(PTE entry)
{
    return get_pfn(entry) << NORMAL_PAGE_SHIFT;
}

static inline uint64_t PTE2kva(PTE entry)
{
    return pa2kva(PTE2pa(entry));
}

/* Get/Set attribute(s) of the `entry` */
static inline long get_attribute(PTE entry, uint64_t mask)
{
    return entry & mask;
}
static inline void set_attribute(PTE *entry, uint64_t mask)
{
    *entry = *entry | (mask & ((1lu << _PAGE_PFN_SHIFT) - 1));
}
static inline void add_attribute(PTE *entry, uint64_t mask)
{
    *entry |= (mask & ((1lu << _PAGE_PFN_SHIFT) - 1));
}
static inline long get_all_attribute(PTE entry)
{
    return entry & ((1lu << _PAGE_PFN_SHIFT) - 1);
}

static inline void clear_attribute(PTE *entry, uint64_t bits)
{
    *entry = *entry & ~bits;
}

static inline void clear_pgdir(uintptr_t pgdir_addr)
{
    PTE *ptr = pgdir_addr;
    for (int i = 0; i < NUM_PTE_ENTRY; ++i)
    {
        *ptr &= 0;
        ptr++;
    }
}

uintptr_t get_kva_of(uintptr_t va, uintptr_t pgdir_va);
uintptr_t probe_kva_of(uintptr_t va, uintptr_t pgdir_kva);
void copy_page_table(uint64_t dest, uint64_t source, uint64_t user_addr_top);

#endif  // PGTABLE_H
