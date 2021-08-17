#ifndef INCLUDE_SCHED_SIGNAL_H
#define INCLUDE_SCHED_SIGNAL_H

#include <type.h>

/* for sigprocmask */
#define SIG_BLOCK 0x0
#define SIG_UNBLOCK 0x1
#define SIG_SETMASK 0x2

/* size of sigset_t */
#define _NSIG_WORDS 1

/* signal */
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGALRM 14
#define SIGTERM 15
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19      
#define SIGTSTP 20

#define SIG_DFL ((void(*)(int))0)
#define SIG_IGN ((void(*)(int))1)

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

#pragma pack(1)
typedef struct sigaction{
    void     (*sa_handler)(int);
    // void     (*sa_sigaction)(int, siginfo_t *, void *);
    long        sa_flags;
    sigset_t   sa_mask;
    // void     (*sa_restorer)(void);
}sigaction_t;
#pragma pack()

typedef struct signal_context{
    reg_t regs[14];
    reg_t kernel_sp;
    reg_t user_sp;
    uint64_t mask;
}signal_context_t;

#define ISSET_SIG(signum,pcb) \
    ((pcb)->sig_recv & ( 1lu << ((signum) - 1) ) )

#define CLEAR_SIG(signum,pcb) \
    ((pcb)->sig_recv &= ~( 1lu << ((signum) - 1) ) )

#define SET_SIG(signum,pcb) \
    ((pcb)->sig_recv |= ( 1lu << ((signum) - 1) ) )

typedef struct pcb pcb_t;
void change_signal_mask(pcb_t *pcb, uint64_t newmask);
void send_signal(int signum, pcb_t *pcb);

#endif