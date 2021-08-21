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

void interrupt_helper(regs_context_t *regs, uint64_t stval, uint64_t cause)
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
    exc_table[EXCC_INST_ACCESS] = &handle_other;
    exc_table[EXCC_LOAD_ACCESS] = &handle_other;
    exc_table[EXCC_STORE_ACCESS] = &handle_other; 
    /* ifdef K210, the next three line override the three lines before */
    /* ifndef K210, set qemu page fault exception */
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
    // log2(0, "pgfault stval:%lx, cause:%d\n", stval, cause);
    /* if not valid address, go handle other */
    if (stval >= 0xffffffff00000000lu || stval < current_running->elf.text_begin){
        if (stval == do_rt_sigreturn)
            do_rt_sigreturn();
        else{
            log(0, "invalid addr");
            handle_other(regs,stval,cause);
        }
    }
    /* allocate memory, or handle memory exception */
    uint64_t page_kva;
    if ((page_kva = probe_kva_of(stval, current_running->pgdir)) != NULL){
        // log2(0, "writable? %d, readable? %d", is_page_kva_file_map_writable(page_kva), is_page_kva_file_map_readable(page_kva));
        if (cause == EXCC_STORE_PAGE_FAULT){
            /* copy on write */
            if (is_page_kva_copy_on_write(page_kva)){
                // log2(0, "copy on write, tid is %d", current_running->tid);
                // log2(0, "page_kva is %lx", page_kva);
                // log2(0, "share_num is %d", get_page_kva_share_num(page_kva));
                if (get_page_kva_share_num(page_kva) == 1){
                    /* only one, just set it to be writable */
                    clear_copy_on_write(page_kva);
                    // log2(0, "clear page_uva is %lx, share_num is %d", stval, get_page_kva_share_num(page_kva));
                    // log2(0, "clear Copy-on-Write is %d", is_copy_on_write(page_kva));
                    assert(add_page_uva_attribute(stval, current_running->pgdir, _PAGE_WRITE) != 0);
                }
                else{
                    /* need to do Copy-on-Write */
                    PTE ret = 0;
                    assert(ret = free_page_helper(stval, current_running->pgdir));
                    uintptr_t new_page_kva = alloc_page_helper(stval, current_running->pgdir, get_all_attribute(ret) | _PAGE_WRITE);
                    memcpy(new_page_kva, PTE2kva(ret), NORMAL_PAGE_SIZE);
                }                
                return ;
            }
            else if (is_page_kva_file_map_writable(page_kva)){
                /* the first member should read in, others just add attribute */
                if (!is_page_kva_file_map_accessed(page_kva)){
                    // log2(0, "set page_uva %lx accessed and dirty", stval);
                    file_map_read_in(get_page_kva_file_map_info(page_kva));
                    set_page_kva_file_map_accessed(page_kva);
                    set_page_kva_file_map_dirty(page_kva);
                }
                else if (!is_page_kva_file_map_dirty(page_kva)){
                    /* accessed, but not dirty */
                    // log2(0, "set page_uva %lx dirty", stval);
                    set_page_kva_file_map_dirty(page_kva);
                }
                assert(add_page_uva_attribute(stval, current_running->pgdir, _PAGE_READ|_PAGE_WRITE) != 0);
                return ;
            }
        }
        else if (cause == EXCC_LOAD_PAGE_FAULT){
            /* the first member should read in, others just add attribute */
            if (is_page_kva_file_map_readable(page_kva)){
                if (!is_page_kva_file_map_accessed(page_kva)){
                    // log2(0, "set page_uva %lx accessed", stval);
                    file_map_read_in(get_page_kva_file_map_info(page_kva));
                    set_page_kva_file_map_accessed(page_kva);
                }
                assert(add_page_uva_attribute(stval, current_running->pgdir, _PAGE_READ) != 0);
                return ;
            }
        }
        else{
            printk_port("stval is %lx, other protected\n", stval);
            handle_other(regs, stval, cause);
        }
    }
    else{
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
                        for (uint64_t pos = PAGE_OFFSET(data_top); pos < NORMAL_PAGE_SIZE; pos++)
                            stval_page_uva[pos] = 0; /* just pad part of this page */
                    }
                    else
                        for (uint64_t pos = 0; pos < NORMAL_PAGE_SIZE; pos++)
                            stval_page_uva[pos] = 0; /* pad all page */
                }
                return ;
            }
        }
    }
    // handle_other(regs,stval,cause);
    if (stval < current_running->user_stack_base && stval >= current_running->user_stack_base + NORMAL_PAGE_SIZE)
        printk_port("user stack requirement too large\n");
    printk_port("stval is %lx, SIGSEGV\n", stval);
    local_flush_tlb_all();
    do_kill(current_running->tid, SIGSEGV);
    return ;
}

void handle_software()
{
    debug();
    log(DEBUG, "handle_software\n");
    while(1);
}

void handle_other(regs_context_t *regs, uint64_t stval, uint64_t cause)
{
    printk_port("pid is %d, tid is %d\n", current_running->pid, current_running->tid);
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
