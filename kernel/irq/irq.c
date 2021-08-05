#include <os/irq.h>
#include <os/time.h>
#include <os/sched.h>
#include <os/string.h>
#include <stdio.h>
#include <assert.h>
#include <sbi.h>
#include <screen.h>
#include <pgtable.h>
#include <qemu.h>
#include <memlayout.h>
#include <log.h>

#define SXLEN 64

handler_t irq_table[IRQC_COUNT];
handler_t exc_table[EXCC_COUNT];
uintptr_t riscv_dtb;

void reset_irq_timer()
{
    // TODO clock interrupt handler.
    // TODO: call following functions when task4
    // #ifndef K210
    // screen_reflush();
    // #endif
    timer_check();

    // note: use sbi_set_timer
    // remember to reschedule

    sbi_set_timer(get_ticks() + (time_base / PREEMPT_FREQUENCY));
    do_scheduler();
}

void interrupt_helper(regs_context_t *regs, uint64_t stval, uint64_t cause, uint64_t tp)
{
    /* user time count */
    user_time_count();
    // call corresponding handler by the value of `cause`
    int is_interrupt = (cause >> (SXLEN - 1));
    is_interrupt = !(is_interrupt == 0);
    int exception_code = (cause << 1) >> 1;
    if (is_interrupt)
        (*irq_table[exception_code])(regs,stval,exception_code);
    else
        (*exc_table[exception_code])(regs,stval,exception_code);
    /* kernel time count */
    kernel_time_count(); // syscall or interrupt
}

void handle_int(regs_context_t *regs, uint64_t interrupt, uint64_t cause)
{
    reset_irq_timer();
}

void init_exception()
{
    /* TODO: initialize irq_table and exc_table */
    /* note: handle_int, handle_syscall, handle_other, etc.*/
    irq_table[IRQC_U_SOFT] = &handle_software; 
    irq_table[IRQC_S_SOFT] = &handle_software;
    irq_table[IRQC_M_SOFT] = &handle_software;  
    irq_table[IRQC_U_TIMER] = &handle_other;
    irq_table[IRQC_S_TIMER] = &handle_int;
    irq_table[IRQC_M_TIMER] = &handle_other; 
    irq_table[IRQC_U_EXT] = &handle_other;   
    irq_table[IRQC_S_EXT] = &handle_sext;   
    irq_table[IRQC_M_EXT] = &handle_other;

    exc_table[EXCC_SYSCALL] = &handle_syscall;
    exc_table[EXCC_INST_MISALIGNED] = &handle_other; 
    exc_table[EXCC_INST_ACCESS] = &handle_other;
    exc_table[EXCC_BREAKPOINT] = &handle_other;
    #ifdef K210  
    exc_table[EXCC_LOAD_ACCESS] = &handle_pgfault;
    exc_table[EXCC_STORE_ACCESS] = &handle_pgfault; 
    #else
    exc_table[EXCC_LOAD_ACCESS] = &handle_other;
    exc_table[EXCC_STORE_ACCESS] = &handle_other; 
    #endif
    exc_table[EXCC_INST_PAGE_FAULT] = &handle_pgfault; 
    exc_table[EXCC_LOAD_PAGE_FAULT] = &handle_pgfault;   
    exc_table[EXCC_STORE_PAGE_FAULT] = &handle_pgfault;
}

extern int plic_claim(void);
extern void disk_intr(void);
extern void plic_complete(int irq);

void handle_sext(regs_context_t *regs, uint64_t interrupt, uint64_t cause)
{
    // this is a supervisor external interrupt, via PLIC.

    // irq indicates which device interrupted.
    #ifndef K210
    // int irq = plic_claim();
    // if(irq == UART0_IRQ){
    //   // uartintr();
    // } else if(irq == VIRTIO0_IRQ){
    //   disk_intr();
    // } else if(irq){
    //   prints("unexpected interrupt irq=%d\n", irq);
    // }
    // // the PLIC allows each device to raise at most one
    // // interrupt at a time; tell the PLIC the device is
    // // now allowed to interrupt again.
    // if(irq)
    //   plic_complete(irq);
    #endif

    return ;
}


void handle_pgfault(regs_context_t *regs, uint64_t stval, uint64_t cause)
{
    // for (uint32 i = 0; i < 30000000; ++i)
    // {
    //     printk_port("stval: %lx\n", regs->sbadaddr);
    //     printk_port("scause: %lx\n", regs->scause);
    //     printk_port("sepc: %lx\n", regs->sepc);
    //     printk_port("ra: %lx", regs->regs[1]);
    // }
    log(0, "pgfault stval:%lx\n", stval);
    handle_other(regs,stval,cause);
    if (stval >= 0xffffffff00000000lu || stval < 0x1000)
        handle_other(regs,stval,cause);
    uint64_t satp = read_satp();
    uint64_t va = stval;
    uint64_t pgdir = (satp&0xffffffffffflu) << NORMAL_PAGE_SHIFT;

    uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    PTE *ptr = pa2kva(pgdir) + vpn2*sizeof(PTE);
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
    if (!get_attribute(*ptr,_PAGE_PRESENT))
    {
        // log(0, "1");
        if (!get_attribute(*ptr,_PAGE_SWAP)){
            // log(0, "2");
            uintptr_t pgdir2 = allocPage();
            uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
            set_pfn(ptr,pfn2);
        }
        else{
            assert(0);
        }
        set_attribute(ptr,_PAGE_READ|_PAGE_WRITE|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_PRESENT|_PAGE_USER);
    }
    else if (cause == 15 && !get_attribute(*ptr,_PAGE_WRITE))
    {
        printk_port("Segmentation fault\n");        assert(0);
    }
    else if (cause == 13 && !get_attribute(*ptr,_PAGE_READ)) // read/write on inst
    {
        printk_port("Segmentation fault\n");        assert(0);
    }
    else if (cause == 12 && !get_attribute(*ptr,_PAGE_EXEC)) // inst on read/write
    {
        printk_port("Segmentation fault\n");        assert(0);
    }
    else
    {
        if (cause == 12 || cause == 13)
            set_attribute(ptr,_PAGE_ACCESSED);
        else if (cause == 15)
            set_attribute(ptr,_PAGE_ACCESSED|_PAGE_DIRTY);
    }
}

void handle_software()
{
    log(DEBUG, "handle_software\n");
    while(1);
}

void handle_other(regs_context_t *regs, uint64_t stval, uint64_t cause)
{
    char* reg_name[] = {
        "zero "," ra  "," sp  "," gp  "," tp  ",
        " t0  "," t1  "," t2  ","s0/fp"," s1  ",
        " a0  "," a1  "," a2  "," a3  "," a4  ",
        " a5  "," a6  "," a7  "," s2  "," s3  ",
        " s4  "," s5  "," s6  "," s7  "," s8  ",
        " s9  "," s10 "," s11 "," t3  "," t4  ",
        " t5  "," t6  "
    };
    for (int i = 0; i < 32; i += 3) {
        for (int j = 0; j < 3 && i + j < 32; ++j) {
            printk("%s : %016lx ",reg_name[i+j], regs->regs[i+j]);
        }
        printk("\n\r");
    }
    printk("sstatus: 0x%lx sbadaddr: 0x%lx scause: %lu\n\r",
           regs->sstatus, regs->sbadaddr, regs->scause);
    printk("sepc: 0x%lx\n\r", regs->sepc);
    assert(0);
}
