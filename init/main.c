/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include <common.h>
#include <os/irq.h>
#include <os/mm.h>
#include <os/sched.h>
#include <screen.h>
#include <sbi.h>
#include <stdio.h>
#include <os/time.h>
#include <os/syscall.h>
#include <os/futex.h>
#include <test.h>
#include <pgtable.h>
#include <os/elf.h>
#include <sdcard.h>
#include <fpioa.h>
#include <memlayout.h>
#include <user_programs.h>
#include <buf.h>
#include <os/fat32.h>
#include <os/uname.h>

#include <csr.h>
#include <asm.h>

extern void ret_from_exception();

uint64_t shell_pgdir;

static void init_pcb()
{
    current_running = &pid0_pcb;
    /* init shell */
    pcb_t *pcb_underinit = &pcb[0];
    ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE;
    ptr_t user_stack = USER_STACK_ADDR;

    init_pcb_default(pcb_underinit, KERNEL_PROCESS);
    
    unsigned char *_elf_shell;
    int length;
    get_elf_file("shell",&_elf_shell,&length);
    uintptr_t pgdir = allocPage();
    clear_pgdir(pgdir);
    alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
    uintptr_t test_shell = (uintptr_t)load_elf(_elf_shell,length,pgdir,alloc_page_helper);
    shell_pgdir = pgdir;

    init_pcb_stack(pgdir, kernel_stack, user_stack, test_shell, 0, NULL, pcb_underinit);
    list_add_tail(&pcb_underinit->list,&ready_queue);

    /* init pcb */
    for (int i = 1; i < NUM_MAX_TASK; ++i)
    {
        pcb[i].list.ptr = &pcb[i];
        pcb[i].pid = 0;
        pcb[i].wait_list.next = &pcb[i].wait_list;pcb[i].wait_list.prev = &pcb[i].wait_list;
        pcb[i].status = TASK_EXITED;
        list_add_tail(&pcb[i].list,&available_queue);
    }

    /* init pid0 */
    pid0_pcb.list.next = &pid0_pcb.list; pid0_pcb.list.prev = &pid0_pcb.list;pid0_pcb.list.ptr = &pid0_pcb;
    pid0_pcb.wait_list.next = &pid0_pcb.wait_list; pid0_pcb.wait_list.prev = &pid0_pcb.wait_list;
    pid0_pcb2.list.next = &pid0_pcb2.list; pid0_pcb2.list.prev = &pid0_pcb2.list;pid0_pcb2.list.ptr = &pid0_pcb2;
    pid0_pcb2.wait_list.next = &pid0_pcb2.wait_list; pid0_pcb2.wait_list.prev = &pid0_pcb2.wait_list;
}

/* just for temp use */
extern void do_testdisk();
/* end */

static void init_syscall(void)
{
    // initialize system call table.

    /* for shell to use */
    syscall[SYSCALL_WRITE] = &screen_write;
    // syscall[SYSCALL_READ] = ???;
    syscall[SYSCALL_CURSOR] = &screen_move_cursor;

    syscall[SYS_exit] = &do_exit;

    syscall[SYSCALL_TESTDISK] = &do_testdisk;

    syscall[SYS_write] = &fat32_write;
    syscall[SYSCALL_TEST] = &fat32_read_test;
    syscall[SYS_getpid] = &do_getpid;
    syscall[SYS_uname] = &do_uname;

    syscall[SYS_sched_yield] = &do_scheduler;
    syscall[SYS_times] = &do_times;
    syscall[SYS_gettimeofday] = &do_gettimeofday;
    syscall[SYS_clone] = &do_clone;
    syscall[SYS_wait4] = &do_wait4;
    syscall[SYS_getppid] = &do_getppid;
    syscall[SYS_nanosleep] = &do_nanosleep;
    syscall[SYS_mkdirat] = &fat32_mkdir;
    syscall[SYS_openat] = &fat32_open;
    syscall[SYS_close] = &fat32_close;
    // syscall[SYS_mmap] = &do_mmap;
}

// The beginning of everything >_< ~~~~~~~~~~~~~~
int main()
{
    // // init Process Control Block (-_-!)
    init_pcb();
    printk("> [INIT] PCB initialization succeeded.\n\r");
    // // read CPU frequency
    time_base = TIMEBASE / TICKCOUNT;
    // init interrupt (^_^)
    init_exception();
    printk("> [INIT] Interrupt processing initialization succeeded.\n\r");
    // init system call table (0_0)
    init_syscall();
    printk("> [INIT] System call initialized successfully.\n\r");

#ifdef K210
    // init sdcard (@—.—@)
    fpioa_pin_init();
    printk("> [INIT] FPIOA initialized successfully.\n\r");
    ioremap(UARTHS, NORMAL_PAGE_SIZE);
#else
    ioremap(UART0, NORMAL_PAGE_SIZE);
#endif

#ifndef K210
    // virtio mmio disk interface
    ioremap(VIRTIO0, NORMAL_PAGE_SIZE);
#endif

    ioremap(CLINT, 0x10000);

    ioremap(PLIC, 0x400000);

#ifdef K210
    ioremap(GPIOHS, 0x1000);
    ioremap(GPIO, 0x1000);
    ioremap(SPI_SLAVE, 0x1000);
    ioremap(GPIOHS, 0x1000);
    ioremap(SPI0, 0x1000);
    ioremap(SPI1, 0x1000);
    ioremap(SPI2, 0x1000);
#endif

    printk("> [INIT] IOREMAP initialized successfully.\n\r");

    share_pgtable(shell_pgdir,pa2kva(PGDIR_PA));
    printk("> [INIT] SHARE PGTABLE initialized successfully.\n\r");

#ifdef K210
    sdcard_init();
    printk("> [INIT] SD card initialized successfully.\n\r");
    fat32_init();
    printk("> [INIT] FAT32 initialized successfully.\n\r");
#else
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    printk("> [INIT] PLIC initialized successfully.\n\r");
    disk_init();
    printk("> [INIT] Virtio Disk initialized successfully.\n\r");
#endif
    // init screen (QAQ)
    init_screen();
    printk("> [INIT] SCREEN initialization succeeded.\n\r");
    // Setup timer interrupt and enable all interrupt
    sbi_set_timer(get_ticks() + (time_base / PREEMPT_FREQUENCY));
    /* setup exception */
    clear_interrupt();
    setup_exception();
    enable_interrupt();

    while (1) {
        // (QAQQQQQQQQQQQ)
        // If you do non-preemptive scheduling, you need to use it
        // to surrender control do_scheduler();
        __asm__ __volatile__("wfi\n\r":::);
        ;
    };
    return 0;
}
