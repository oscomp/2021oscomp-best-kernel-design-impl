
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
#include <os/io.h>
#include <os/system.h>
#include <os/poll.h>

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
    alloc_page_helper(user_stack,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);

    uintptr_t test_shell = (uintptr_t)load_elf(_elf_shell,length,pgdir,alloc_page_helper, &pcb_underinit->elf);
    pcb_underinit->edata = pcb_underinit->elf.edata;
    shell_pgdir = pgdir;

    init_pcb_stack(pgdir, kernel_stack, user_stack, test_shell, NULL, NULL, NULL, pcb_underinit);
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

static void init_syscall(void)
{
    // initialize system call table.

    /* for shell to use */
    syscall[SYSCALL_WRITE] = &screen_assert;
    // syscall[SYSCALL_READ] = ???;
    syscall[SYSCALL_CURSOR] = &screen_assert;

    syscall[SYS_exit] = &do_exit;
    syscall[SYS_exit_group] = &do_exit_group;

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
    syscall[SYS_mkdirat] = &fat32_mkdirat;
    syscall[SYS_openat] = &fat32_openat;
    syscall[SYS_close] = &fat32_close;
    syscall[SYS_chdir] = &fat32_chdir;
    syscall[SYS_getcwd] = &fat32_getcwd;
    syscall[SYS_read] = &fat32_read;
    syscall[SYS_execve] = &do_exec;
    syscall[SYS_brk] = &do_brk;
    syscall[SYS_dup] = &fat32_dup;
    syscall[SYS_dup3] = &fat32_dup3;
    syscall[SYS_fstat] = &fat32_fstat;
    syscall[SYS_getdents64] = &fat32_getdents64;
    syscall[SYS_pipe2] = &fat32_pipe2;

    syscall[SYS_linkat] = &fat32_link;
    syscall[SYS_unlinkat] = &fat32_unlinkat;
    syscall[SYS_mount] = &fat32_mount;
    syscall[SYS_umount2] = &fat32_umount;
    syscall[SYS_mmap] = &do_mmap;
    syscall[SYS_munmap] = &do_munmap;

    syscall[SYS_getuid] = &do_getuid;
    syscall[SYS_geteuid] = &do_geteuid;
    syscall[SYS_getgid] = &do_getgid;
    syscall[SYS_getegid] = &do_getegid;

    syscall[SYS_writev] = &fat32_writev;
    syscall[SYS_readv] = &fat32_readv;
    syscall[SYS_readlinkat] = &fat32_readlinkat;
    syscall[SYS_mprotect] = &do_mprotect;
    syscall[SYS_set_tid_address] = &do_set_tid_address;
    syscall[SYS_clock_gettime] = &do_clock_gettime;

    syscall[SYS_ioctl] = &do_ioctl;
    syscall[SYS_fcntl] =&fat32_fcntl;

    syscall[SYS_rt_sigaction] = &do_rt_sigaction;
    syscall[SYS_rt_sigprocmask] = &do_rt_sigprocmask;
    syscall[SYS_rt_sigreturn] = &do_rt_sigreturn;
    syscall[SYS_fstatat] = &fat32_fstatat;
    syscall[SYS_syslog] = &do_syslog;
    syscall[SYS_faccessat] = &fat32_faccessat;
    syscall[SYS_sysinfo] = &do_sysinfo;
    syscall[SYS_lseek] = &fat32_lseek;
    syscall[SYS_kill] = &do_kill;

    syscall[SYS_utimensat] = &do_utimensat;
    syscall[SYS_gettid] = &do_gettid;
    syscall[SYS_ppoll] = &do_ppoll;
    syscall[SYS_sendfile] = &do_sendfile;
    syscall[SYS_renameat2] = &fat32_renameat2;
}

// The beginning of everything >_< ~~~~~~~~~~~~~~
int main()
{
    // // init Process Control Block (-_-!)
    init_pcb();
    printk_port("> [INIT] PCB initialization succeeded.\n\r");
    // // read CPU frequency
    time_base = TIMEBASE / TICKCOUNT;
    // init interrupt (^_^)
    init_exception();
    printk_port("> [INIT] Interrupt processing initialization succeeded.\n\r");
    // init system call table (0_0)
    init_syscall();
    printk_port("> [INIT] System call initialized successfully.\n\r");
    init_recyc();
    printk_port("> [INIT] Memory initialization succeeded.\n\r");
    init_timers();
    printk_port("> [INIT] Timers initialization succeeded.\n\r");

#ifdef K210
    // init sdcard (@—.—@)
    fpioa_pin_init();
    printk_port("> [INIT] FPIOA initialized successfully.\n\r");
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
    // ioremap(GPIOHS, 0x1000);
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
    sbi_set_timer(get_ticks() + (time_base / PREEMPT_FREQUENCY) * 100);
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
