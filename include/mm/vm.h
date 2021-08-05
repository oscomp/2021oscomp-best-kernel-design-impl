#ifndef __VM_H 
#define __VM_H 

#include "types.h"
#include "hal/riscv.h"
#include "mm/usrmm.h"

// Flags of unmappages()
#define VM_FREE (1 << 0)  // free the pages when unmapping
#define VM_USER (1 << 1)  // are the pages belong to user?
#define VM_HOLE (1 << 2)  // whether allow unmapped or invalid hole in the pages range

static inline void permit_usr_mem()
{
  #ifndef QEMU
  // w_sstatus(r_sstatus() & ~SSTATUS_PUM);
  clr_sstatus_bit(SSTATUS_PUM);
  #else
  // w_sstatus(r_sstatus() | SSTATUS_SUM);
  set_sstatus_bit(SSTATUS_SUM);
  #endif
}

static inline void protect_usr_mem()
{
  #ifndef QEMU
  // w_sstatus(r_sstatus() | SSTATUS_PUM);
  set_sstatus_bit(SSTATUS_PUM);
  #else
  // w_sstatus(r_sstatus() & ~SSTATUS_SUM);
  clr_sstatus_bit(SSTATUS_SUM);
  #endif
}

extern pagetable_t kernel_pagetable;

void            kvminit(void);
void            kvminithart(void);
void            kvmmap(uint64 va, uint64 pa, uint64 sz, int perm);
pagetable_t     kvmcreate(void);
void            kvmfree(pagetable_t kpt, int stack_free);

void            uvminit(pagetable_t, uchar *, uint);
pagetable_t     uvmcreate(void);
// int             uvmcopy(pagetable_t, pagetable_t, pagetable_t, uint64);
// int             uvmcopy_cow(pagetable_t old, pagetable_t new, uint64 start, uint64 end, enum segtype);
int             uvmcopy(pagetable_t old, pagetable_t new, uint64 start, uint64 end, int cow);
uint64          uvmalloc(pagetable_t, uint64 start, uint64 end, int perm);
uint64          uvmdealloc(pagetable_t, uint64, uint64);
int             uvmprotect(pagetable_t pagetable, uint64 va, uint64 len, int prot);
void            uvmclear(pagetable_t, uint64);
void            uvmfree(pagetable_t pt);

pte_t *         walk(pagetable_t pagetable, uint64 va, int alloc);
uint64          walkaddr(pagetable_t, uint64);
uint64          kwalkaddr(pagetable_t pagetable, uint64 va);
uint64          kvmpa(uint64);

void            pagereg(uint64 pa, uint8 init);
int             pageput(uint64 pa);
int             mappages(pagetable_t pt, uint64 va, uint64 size, uint64 pa, int perm);
void            unmappages(pagetable_t pt, uint64 va, uint64 npages, int flag);

int             copyout(pagetable_t, uint64, char *, uint64);
int             copyin(pagetable_t, char *, uint64, uint64);
int             copyinstr(pagetable_t, char *, uint64, uint64);
int             copyout2(uint64 dstva, char *src, uint64 len);
int             copyin2(char *dst, uint64 srcva, uint64 len);
int             copyinstr2(char *dst, uint64 srcva, uint64 max);
void            vmprint(pagetable_t pagetable);

int             handle_page_fault(int type, uint64 badaddr);
uint64          kern_pgfault_escape(uint64 badaddr);

extern int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
extern int either_copyin(void *dst, int user_src, uint64 src, uint64 len);

#endif 
