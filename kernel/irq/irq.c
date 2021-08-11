#include <os/irq.h>
#include <os/time.h>
#include <os/sched.h>
#include <os/string.h>
#include <os/fat32.h>
#include <stdio.h>
#include <assert.h>
#include <sbi.h>
#include <screen.h>
#include <pgtable.h>
#include <qemu.h>
#include <memlayout.h>
#include <log.h>
#include <os/io.h>

#define SXLEN 64

handler_t irq_table[IRQC_COUNT];
handler_t exc_table[EXCC_COUNT];
uintptr_t riscv_dtb;

void reset_irq_timer()
{
    /* clock interrupt handler */
    timer_check();
    screen_reflush();
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
    exc_table[EXCC_BREAKPOINT] = &handle_other;
    #ifdef K210  
    exc_table[EXCC_INST_ACCESS] = &handle_pgfault;
    exc_table[EXCC_LOAD_ACCESS] = &handle_pgfault;
    exc_table[EXCC_STORE_ACCESS] = &handle_pgfault; 
    #else
    exc_table[EXCC_INST_ACCESS] = &handle_other;
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
    // printk_port("pgfault stval:%lx, cause:%d\n", stval, cause);
    // printk_port("current pid is %d, tid is %d\n", current_running->pid, current_running->tid);
    // printk_port("pgdir: %lx", current_running->pgdir);
    /* if not valid address, go handle other */

    if (stval >= 0xffffffff00000000lu || stval < current_running->elf.text_begin){
        log(0, "invalid addr");
        handle_other(regs,stval,cause);
    }
    // uint64_t satp = read_satp();
    // uint64_t va = stval;
    // uint64_t pgdir = (satp&0xffffffffffflu) << NORMAL_PAGE_SHIFT;

    // uint64_t vpn2 = (va&VA_MASK) >> VA_VPN2_SHIFT;
    // uint64_t vpn1 = ((va&VA_MASK) >> VA_VPN1_SHIFT) & (NUM_PTE_ENTRY - 1);
    // uint64_t vpn0 = ((va&VA_MASK) >> VA_VPN0_SHIFT) & (NUM_PTE_ENTRY - 1);
    // PTE *ptr = pa2kva(pgdir) + vpn2*sizeof(PTE);
    // // 2
    // if (!get_attribute(*ptr,_PAGE_PRESENT))
    // {
    //     uintptr_t pgdir2 = allocPage();
    //     clear_pgdir(pgdir2);
    //     uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
    //     set_pfn(ptr,pfn2);
    //     set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
    //     ptr = pgdir2 + vpn1*sizeof(PTE);
    // }
    // else
    //     ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn1*sizeof(PTE);
    // // 1
    // if (!get_attribute(*ptr,_PAGE_PRESENT))
    // {
    //     uintptr_t pgdir2 = allocPage();
    //     clear_pgdir(pgdir2);
    //     uint64_t pfn2 = (kva2pa(pgdir2)&VA_MASK) >> NORMAL_PAGE_SHIFT;        
    //     set_pfn(ptr,pfn2);
    //     set_attribute(ptr,_PAGE_PRESENT|_PAGE_USER);
    //     ptr = pgdir2 + vpn0*sizeof(PTE);
    // }
    // else
    //     ptr = pa2kva(get_pfn(*ptr) << NORMAL_PAGE_SHIFT) + vpn0*sizeof(PTE);

    // // 0
    // if (!get_attribute(*ptr,_PAGE_PRESENT))
    // {
    //     // log(0, "1");
    //     if (!get_attribute(*ptr,_PAGE_SWAP)){
    //         // log(0, "2");
            for (uint8_t i = 0; i < NUM_PHDR_IN_PCB; i++){
                Elf64_Phdr *phdr = (Elf64_Phdr *)(&current_running->phdr[i]);
                if (stval >= phdr->p_vaddr && stval < phdr->p_vaddr + phdr->p_memsz)
                {
                    /* it is in ELF file */
                    fat32_lseekmy(PAGE_ALIGN(stval - phdr->p_vaddr + phdr->p_offset), SEEK_SET);
                    alloc_page_helper(stval, current_running->pgdir, _PAGE_ALL_MOD); /* FOR NOW all mod */
                    /* now we can access this address */
                    unsigned char *stval_page_uva = PAGE_ALIGN(stval);
                    fat32_readmy(stval_page_uva, NORMAL_PAGE_SIZE);
                    uint64_t data_top = phdr->p_vaddr + phdr->p_filesz;
                    if (phdr->p_filesz < phdr->p_memsz && stval >= PAGE_ALIGN(data_top)){
                        /* should pad 0 for bss */                        
                        if (PAGE_ALIGN(stval) == PAGE_ALIGN(data_top)){ 
                            log(0, "%lx", PAGE_OFFSET(data_top));                           
                            for (uint64_t pos = PAGE_OFFSET(data_top); pos < NORMAL_PAGE_SIZE; pos++)
                                stval_page_uva[pos] = 0; /* just pad part of this page */
                        }
                        else
                            for (uint64_t pos = 0; pos < NORMAL_PAGE_SIZE; pos++)
                                stval_page_uva[pos] = 0; /* pad all page */
                    }
                    // if (i == 1 && stval == 0x121478){
                    //     uint64_t test = get_kva_of(PAGE_ALIGN(stval), current_running->pgdir);
                    //     uint64_t *test2 = (uint64_t *)test;
                    //     for (uint64_t k = 0; k < NORMAL_PAGE_SIZE / 8; k++){
                    //         log(0, "%lx:%lx", test2, *test2);
                    //         test2++;
                    //     }
                    //     while(1);
                    // }
                    // log(0, "handle pgfault success");
                    return ;
                }
            }
            handle_other(regs,stval,cause);
            // if (cause == EXCC_INST_ACCESS || cause == EXCC_INST_PAGE_FAULT)
            //     set_attribute(ptr,_PAGE_ALL_MOD|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_PRESENT|_PAGE_USER);
            // else if (cause == EXCC_LOAD_ACCESS || cause == EXCC_LOAD_PAGE_FAULT)
            //     set_attribute(ptr,_PAGE_ALL_MOD|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_PRESENT|_PAGE_USER);
            // else if (cause == EXCC_STORE_ACCESS || cause == EXCC_STORE_PAGE_FAULT)
            //     set_attribute(ptr,_PAGE_ALL_MOD|_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_PRESENT|_PAGE_USER);
    //     }
    //     else assert(0);
    // }
    // else if (cause == 15 && !get_attribute(*ptr,_PAGE_WRITE))
    // {
    //     printk_port("Segmentation fault\n");        assert(0);
    // }
    // else if (cause == 13 && !get_attribute(*ptr,_PAGE_READ)) // read/write on inst
    // {
    //     printk_port("Segmentation fault\n");        assert(0);
    // }
    // else if (cause == 12 && !get_attribute(*ptr,_PAGE_EXEC)) // inst on read/write
    // {
    //     printk_port("Segmentation fault\n");        assert(0);
    // }
    // else
    // {
    //     assert(0);
    //     if (cause == 12 || cause == 13)
    //         set_attribute(ptr,_PAGE_ACCESSED);
    //     else if (cause == 15)
    //         set_attribute(ptr,_PAGE_ACCESSED|_PAGE_DIRTY);
    // }
}

void handle_software()
{
    debug();
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
