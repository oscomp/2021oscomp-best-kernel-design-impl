
#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include <type.h>
#include <os/list.h>
#include <time/itimer.h>

#define NUM_TIMER 16

#define MICRO 6
#define NANO 9
#define TIMEBASE 403000000
#define TICKCOUNT 62

typedef void (*TimerCallback)(void *);

typedef struct timer
{
    timer_id_t timer_id;
    list_node_t list;
    uint64_t timeout_tick;
    TimerCallback callback_func;
    uint64_t parameter;
} timer_t;

/* for gettimes */
struct tms              
{                     
    clock_t tms_utime;  
    clock_t tms_stime;  
    clock_t tms_cutime; 
    clock_t tms_cstime; 
};

#pragma pack(1)

/* nanosecond precison timespec */
struct timespec {
    time_t tv_sec; // seconds
    time_t tv_nsec; // and nanoseconds
};
#pragma pack
/* microsecond precison timeval */
struct timeval {
    time_t  tv_sec;   
    time_t  tv_usec;
};

#define CLOCK_REALTIME 0x0

#define SECONDS_PER_MIN 60
#define MIN_PER_HOUR 60
#define HOUR_PER_DAY 24
#define DAY_PER_MONTH 30
#define MONTH_PER_YEAR 12
/* for time trasmission */
struct regular_time{
    uint32_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint64_t seconds;
    uint64_t nano_seconds;
};

/* itimer */
#define ITIMER_REAL 0 /* Timers run in real time */
#define ITIMER_VIRTUAL 1 /* Timers run only when the process is executing */
#define ITIMER_PROF 2 /* Timers run when the process is executing and 
                        when the system is executing on behanlf of the process */
struct itimerval {
    struct timeval it_interval; /* Interval for periodic timer */
    struct timeval it_value;    /* Time until next expiration */
};

struct rusage {
    struct timeval ru_utime; /* user CPU time used */
    struct timeval ru_stime; /* system CPU time used */
    long   ru_maxrss;        /* maximum resident set size */
    long   ru_ixrss;         /* integral shared memory size */
    long   ru_idrss;         /* integral unshared data size */
    long   ru_isrss;         /* integral unshared stack size */
    long   ru_minflt;        /* page reclaims (soft page faults) */
    long   ru_majflt;        /* page faults (hard page faults) */
    long   ru_nswap;         /* swaps */
    long   ru_inblock;       /* block input operations */
    long   ru_oublock;       /* block output operations */
    long   ru_msgsnd;        /* IPC messages sent */
    long   ru_msgrcv;        /* IPC messages received */
    long   ru_nsignals;      /* signals received */
    long   ru_nvcsw;         /* voluntary context switches */
    long   ru_nivcsw;        /* involuntary context switches */
};

extern uint32_t time_base;
extern uint64_t time_elapsed;
extern uint64_t MHZ;

void init_timers();
uint64_t get_timer(void);
uint64_t get_ticks(void);
void kernel_time_count();
void user_time_count();
uint64_t timespec_to_tick(struct timespec *ts);
uint64_t timeval_to_tick(struct timeval *ts);
void tick_to_timeval(time_t time, struct timeval *tv);

uint64_t do_times(struct tms *tms);
int8_t do_gettimeofday(struct timespec *ts);
int32_t do_clock_gettime(uint64_t clock_id, struct timespec *tp);
int do_setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
int32_t do_getrusage(int32_t who, struct rusage *usage);

extern uint64_t get_time_base();

extern list_head timers;
extern list_head available_timers;
void timer_create(TimerCallback func, void* parameter, uint64_t tick);
void do_set_test_timer();
// this should be called by handle_int
void timer_check(void);
timer_t *alloc_timer();
void latency(uint64_t time);

/* add this timer to timers */
static inline void enable_timer(timer_t *timer)
{
    list_add_tail(&timer->list, &timers);
}
/* delete this timer from timer list */
static inline void disable_timer(timer_t *timer)
{
    list_del(&timer->list);
}
/* put this timer to available list */
static inline void free_timer(timer_t *timer)
{
    list_add_tail(&timer->list, &available_timers);
}

#endif
