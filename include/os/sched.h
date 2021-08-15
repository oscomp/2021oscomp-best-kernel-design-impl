
#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_


#include <type.h>
#include <os/list.h>
#include <os/mm.h>
#include <os/smp.h>
#include <os/time.h>
#include <os/elf.h>

#define NUM_MAX_TASK 16

/* used to save register infomation */
typedef struct regs_context
{
    /* Saved main processor registers.*/
    reg_t regs[32];

    /* Saved special registers. */
    reg_t sstatus;
    reg_t sepc;
    reg_t sbadaddr;
    reg_t scause;
    reg_t satp;
} regs_context_t;

/* used to save register infomation in switch_to */
typedef struct switchto_context
{
    /* Callee saved registers.*/
    reg_t regs[14];
    reg_t satp
} switchto_context_t;

typedef enum {
    TASK_BLOCKED,
    TASK_RUNNING,
    TASK_READY,
    TASK_ZOMBIE,
    TASK_EXITED,
} task_status_t;

typedef enum {
    ENTER_ZOMBIE_ON_EXIT,
    AUTO_CLEANUP_ON_EXIT,
} spawn_mode_t;

#define DEFAULT_MODE AUTO_CLEANUP_ON_EXIT

typedef enum {
    KERNEL_PROCESS,
    KERNEL_THREAD,
    USER_PROCESS,
    USER_THREAD,
} task_type_t;

typedef uint32_t isec_t;

typedef struct dir_pos{
    uint64_t offset;
    size_t len;
    isec_t sec;
}dir_pos_t;

#define NUM_FD 16
#define NUM_PHDR_IN_PCB 2

typedef uint64_t poll_status_t;

#pragma pack(8)
typedef struct fd{
    /* dev number */
    uint8 dev;
    /* first clus number */
    uint32 first_clus_num;
    /* file mode */
    uint32 mode;
    /* open flags */
    uint8 flags;
    /* position */
    uint64 pos;
    /* length */
    uint32 length;
    /* dir_pos */
    dir_pos_t dir_pos;

    /* fd number */
    /* default: its index in fd-array*/
    fd_num_t fd_num;

    /* used */
    uint8 used;
    /* redirect */
    uint8 redirected;
    uint8 redirected_fd_index;

    /* piped */
    uint8 piped;
    /* pipes[pip_num] is the pipe */
    pipe_num_t pip_num;
    uint8 is_pipe_read;
    uint8 is_pipe_write;

    /* poll status */
    poll_status_t poll_status;

    /* mmap */
    struct {
        void *start;
        size_t len;
        int flags;
        off_t off;
    }mmap;

    /* links */
    uint8 nlink;

    /* status */
    uid_t uid;
    gid_t gid;

    dev_t rdev;

    blksize_t blksize;

    long atime_sec;
    long atime_nsec;
    long mtime_sec;
    long mtime_nsec;
    long ctime_sec;
    long ctime_nsec;
}fd_t;
#pragma pack()

#pragma pack(8)
/* Process Control Block */
struct pcb
{
    /* register context */
    // this must be this order!! The order is defined in regs.h
    reg_t kernel_sp;
    reg_t user_sp;

    /* is this an exec pcb? */
    uint8_t exec;
    // count the number of disable_preempt
    // enable_preempt enables CSR_SIE only when preempt_count == 0
    reg_t preempt_count;

    // stack base
    ptr_t user_stack_base;
    // ptr_t kernel_stack_base;
    ptr_t user_addr_top; /* for mem alloc */

    /* previous, next pointer */
    list_node_t list;
    list_head wait_list;

    /* process id */
    pid_t pid;
    pid_t pid_on_exec;
    pid_t tid;

    /* kernel/user thread/process */
    task_type_t type;

    /* BLOCK | READY | RUNNING */
    task_status_t status;

    /* spawn mode */
    spawn_mode_t mode;

    /* spawn num */
    spawn_num_t spawn_num;

    /* priority */
    int32_t priority;
    int32_t temp_priority;

    /* mask CPU */
    uint64_t mask;

    /* kva pgdir */
    uint64_t pgdir;

    /* cursor position */
    int32_t cursor_x;
    int32_t cursor_y;

    /* time */
    uint64_t time_start;

    /* point to parent */
    struct parent{
        struct pcb *parent;
        uint32_t flag;
    }parent;

    /* exit status */
    int32_t exit_status;

    /* systime */
    uint64_t stime;
    uint64_t utime;

    /* file descriptor */
    fd_t fd[NUM_FD];

    /* elf info */
    struct ELF_info elf;

    /* edata */
    uint64_t edata;

    /* file discriptor for ELF only, always open */
    fd_t myelf_fd;
    /* ELF program headers for load */
    Elf64_Phdr phdr[NUM_PHDR_IN_PCB];

    /* context switch time */
    uint64_t scheduler_switch_time;
    uint64_t yield_switch_time;
};
#pragma pack()
typedef struct pcb pcb_t;

#define DEFAULT_PRIORITY 1
#define WEXITSTATUS(status,exit_status) (*((uint16_t *)status) = ((exit_status) << 8) & 0xff00)

/* task information, used to init PCB */
typedef struct task_info
{
    ptr_t entry_point;
    task_type_t type;
} task_info_t;

/* aux vector */
#define MAX_AUX_ARG 0x30

#define AT_PHDR 3
#define AT_PHENT  4
#define AT_PHNUM  5
#define AT_PAGESZ  6
#define AT_BASE  7
#define AT_FLAGS  8
#define AT_ENTRY  9
#define AT_UID   11
#define AT_EUID  12
#define AT_GID   13
#define AT_EGID 14
#define AT_HWCAP  16
#define AT_CLKTCK  17
#define AT_SECURE 23
#define AT_RANDOM 0x19
#define AT_EXECFN  0x1f
#define AT_SYSINFO_EHDR 0x21
typedef struct aux_elem
{
    uint64_t id;
    uint64_t val;
}aux_elem_t;

/* signal */
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGTERM 15
#define SIGCHLD 17
#define SIGKILL 9
#define SIGSTOP 19      
#define SIGTSTP 20
#define SIGCONT 18
#define SIGUSR1 10
#define SIGUSR2 12

/* sa_flags */
#define SA_NOCLDSTOP 0x1
#define SA_NOCLDWAIT 0x2
#define SA_SIGINFO 0x4
#define SA_RESTORER 0x4000000
#define SA_NODEFER 0x40000000
#define SA_ONSTACK 0x8000000
#define SA_RESETHAND 0x80000000
#define SA_RESTART 0x10000000

/* for sigprocmask */
#define SIG_BLCOK 0x0
#define SIG_UNBLOCK 0x1
#define SIG_SETMASK 0x2

/* size of sigset_t */
#define _NSIG_WORDS 16

typedef struct{
    unsigned long sig[_NSIG_WORDS];
}sigset_t;

typedef struct siginfo{
    int      si_signo;     /* Signal number */
    int      si_errno;     /* An errno value */
    int      si_code;      /* Signal code */
    int      si_trapno;    /* Trap number that caused
                             hardware-generated signal
                             (unused on most architectures) */
    pid_t    si_pid;       /* Sending process ID */
    uid_t    si_uid;       /* Real user ID of sending process */
    int      si_status;    /* Exit value or signal */
    clock_t  si_utime;     /* User time consumed */
    clock_t  si_stime;     /* System time consumed */
    sigval_t si_value;     /* Signal value */
    int      si_int;       /* POSIX.1b signal */
    void    *si_ptr;       /* POSIX.1b signal */
    int      si_overrun;   /* Timer overrun count;
                             POSIX.1b timers */
    int      si_timerid;   /* Timer ID; POSIX.1b timers */
    void    *si_addr;      /* Memory location which caused fault */
    long     si_band;      /* Band event (was int in glibc 2.3.2 and earlier) */
    int      si_fd;        /* File descriptor */
    short    si_addr_lsb;  /* Least significant bit of address
                             (since Linux 2.6.32) */
    void    *si_lower;     /* Lower bound when address violation
                             occurred (since Linux 3.19) */
    void    *si_upper;     /* Upper bound when address violation */
    int      si_pkey;      /* Protection key on PTE that caused
                             fault (since Linux 4.6) */
    void    *si_call_addr; /* Address of system call instruction
                             (since Linux 3.5) */
    int      si_syscall;   /* Number of attempted system call
                             (since Linux 3.5) */
    unsigned int si_arch;  /* Architecture of attempted system call
                             (since Linux 3.5) */
}siginfo_t;


typedef struct sigaction{
    void     (*sa_handler)(int);
    // void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
    void     (*sa_restorer)(void);
}sigaction_t;

/* ready queue to run */
extern list_head ready_queue;
extern list_head general_block_queue;
extern list_head available_queue;
extern list_head fileop_queue;

/* current running task PCB */
extern pcb_t * volatile current_running;
extern pid_t process_id;

extern pcb_t pcb[NUM_MAX_TASK];
extern pcb_t pid0_pcb;
// extern pcb_t pid0_pcbs[NR_CPUS];
extern pcb_t pid0_pcb2;
extern const ptr_t pid0_stack;
extern const ptr_t pid0_stack2;

extern void __global_pointer$();

void init_pcb_default(pcb_t *pcb_underinit,task_type_t type);
void init_pcb_stack(ptr_t pgdir, ptr_t kernel_stack, ptr_t user_stack, ptr_t entry_point,unsigned char *argv[], 
    unsigned char *envp[], aux_elem_t *aux_vec, pcb_t *pcb);
uint64_t get_argc_from_argv(char *argv[]);
void init_clone_pcb(uint64_t pgdir, pcb_t *pcb_underinit, uint64_t kernel_stack_top, uint64_t user_stack_top, uint32_t flag);
extern void ret_from_exception();
extern void switch_to(pcb_t *prev, pcb_t *next);
void do_scheduler(void);
void do_yield();
pid_t do_spawn(task_info_t *task, void* arg, spawn_mode_t mode);
void do_exit(int32_t exit_status);
void do_exit_group(int32_t exit_status);
void do_sleep(uint32_t);

void do_block(list_node_t *, list_head *queue);
void do_unblock(void *);

int32_t do_kill(pid_t pid, int32_t sig);
int do_waitpid(pid_t pid);
void do_process_show();
pid_t do_getpid();
pid_t do_getppid();
pid_t do_getuid();
pid_t do_geteuid();
pid_t do_getgid();
pid_t do_getegid();
pid_t do_gettid();

pid_t do_set_tid_address(int *tidptr);

int do_taskset(uint32_t pid,uint32_t mask);

pid_t do_clone(uint32_t flag, uint64_t stack, pid_t ptid, void *tls, pid_t ctid, void *nouse);
int64_t do_wait4(pid_t pid, uint16_t *status, int32_t options);

uint8_t do_nanosleep(struct timespec *sleep_time);
int8 do_exec(const char* file_name, char* argv[], char *const envp[]);

int32_t do_rt_sigprocmask(int32_t how, const sigset_t *restrict set, sigset_t *restrict oldset, size_t sigsetsize);
int32_t do_rt_sigaction(int32_t signum, struct sigaction *act, struct sigaction *oldact, size_t sigsetsize);
void do_rt_sigreturn();

void scheduler_switch_time_count();
void yield_switch_time_count();

/* scheduler counter */
extern int FORMER_TICKS_COUNTER;
extern int LATTER_TICKS_COUNTER;

extern void do_show_exec();

static inline uint64_t get_user_addr_top(pcb_t *pcb)
{
    // while (pcb->parent.parent && pcb->parent.parent->pid == pcb->pid)
    //     pcb = pcb->parent.parent;
    return pcb->user_addr_top;
}

static inline void set_user_addr_top(pcb_t *pcb, uint64_t user_addr_top)
{
    // while (pcb->parent.parent && pcb->parent.parent->pid == pcb->pid)
    //     pcb = pcb->parent.parent;
    assert(user_addr_top >= pcb->edata);
    assert(user_addr_top % NORMAL_PAGE_SIZE == 0);
    pcb->user_addr_top = user_addr_top;
}

static inline uint64_t get_edata(pcb_t *pcb)
{
    // while (pcb->parent.parent && pcb->parent.parent->pid == pcb->pid)
    //     pcb = pcb->parent.parent;
    return pcb->edata;
}

static inline void set_edata(pcb_t *pcb, uint64_t edata)
{
    // while (pcb->parent.parent && pcb->parent.parent->pid == pcb->pid)
    //     pcb = pcb->parent.parent;
    pcb->edata = edata;
}

static inline void set_pcb_edata(pcb_t *pcb_underinit)
{
    pcb_underinit->edata = PAGE_ALIGNUP(pcb_underinit->elf.edata);
}

/* copy parent thread's both stack to child thread */
/* copy necesaary size */
static inline void copy_stack(pcb_t *pcb_underinit, uint64_t ker_stack_size, uint64_t user_stack_size)
{
    /* maybe no need to copy this */
    // memcpy(pcb_underinit->user_sp, current_running->user_sp, user_stack_size);
}

static inline uint8_t count_spawn_num(pid_t pid)
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status != TASK_EXITED && pcb[i].status != TASK_ZOMBIE
            && pcb[i].pid == pid)
            ret++;
    return ret;
}

#endif
