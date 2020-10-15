#ifndef STDATOMIC_H
#define STDATOMIC_H

/* from linux/arch/riscv/include/asm/cmpxchg.h and atomic.h */

#include <stdint.h>

typedef volatile uint32_t atomic_uint32_t;
typedef volatile uint64_t atomic_uint64_t;

typedef volatile unsigned int atomic_uint;
typedef volatile int atomic_int;
typedef volatile unsigned long atomic_ulong;
typedef volatile long atomic_long;

static inline int fetch_add(volatile void* obj, int arg)
{
    uint32_t ret;
    __asm__ __volatile__ (
        "amoadd.w.aqrl %0, %2, %1\n"
        : "=r"(ret), "+A" (*obj)
        : "r"(arg)
        : "memory");
    return ret;
}

static inline int fetch_sub(volatile void* obj, int arg)
{
    uint32_t ret;
    arg = 0 - arg;
    __asm__ __volatile__ (
        "amoadd.w.aqrl %0, %2, %1\n"
        : "=r"(ret), "+A" (*obj)
        : "r"(arg)
        : "memory");
    return ret;
}

static inline int atomic_exchange(volatile void* obj, int desired)
{
    int ret;
    __asm__ __volatile__ (
        "amoswap.w.aqrl %0, %2, %1\n"
        : "=r"(ret), "+A" (*obj)
        : "r"(desired)
        : "memory");
    return ret;
}

static inline long atomic_exchange_d(volatile void* obj, long desired)
{
    uint64_t ret;
    __asm__ __volatile__ (
                          "amoswap.d.aqrl %0, %2, %1\n"
                          : "=r"(ret), "+A" (*obj)
                          : "r"(desired)
                          : "memory");
    return ret;
}

/* if *mem_addr == old_val, then *mem_addr = new_val, else return *mem_addr */
static inline int atomic_compare_exchange(volatile void* obj, int expected, int desired)
{
    uint32_t ret;
    register unsigned int __rc;
    __asm__ __volatile__ (
          "0:	lr.w %0, %2\n"
          "	bne  %0, %z3, 1f\n"
          "	sc.w.rl %1, %z4, %2\n"
          "	bnez %1, 0b\n"
          "	fence rw, rw\n"
          "1:\n"
          : "=&r" (ret), "=&r" (__rc), "+A" (*obj)
          : "rJ" (expected), "rJ" (desired)
          : "memory");
    return ret;
}

/* if *mem_addr == old_val, then *mem_addr = new_val, else return *mem_addr */
static inline long atomic_compare_exchange_d(volatile void* obj, long expected, long desired)
{
    uint64_t ret;
    register unsigned int __rc;

    __asm__ __volatile__ (
          "0:	lr.d %0, %2\n"
          "	bne  %0, %z3, 1f\n"
          "	sc.d.rl %1, %z4, %2\n"
          "	bnez %1, 0b\n"
          "	fence rw, rw\n"
          "1:\n"
          : "=&r" (ret), "=&r" (__rc), "+A" (*obj)
          : "rJ" (expected), "rJ" (desired)
          : "memory");

    return ret;
}

#endif /* ATOMIC_H */
