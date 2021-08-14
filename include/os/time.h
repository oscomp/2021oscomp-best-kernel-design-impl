
#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include <type.h>
#include <os/list.h>

#define NUM_TIMER 16

#define MICRO 6
#define NANO 9
#define TIMEBASE 403000000
#define TICKCOUNT 62

typedef void (*TimerCallback)(void *parameter);
typedef uint64_t time_t;
typedef uint64_t clock_t;

typedef struct timer
{
    list_node_t list;
    uint64_t timeout_tick;
    TimerCallback callback_func;
    void *parameter;
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

extern uint32_t time_base;
extern uint64_t time_elapsed;
extern uint64_t MHZ;

void init_timers();
uint64_t get_timer(void);
uint64_t get_ticks(void);
void kernel_time_count();
void user_time_count();

uint64_t do_times(struct tms *tms);
int8_t do_gettimeofday(struct timespec *ts);
int32_t do_clock_gettime(uint64_t clock_id, struct timespec *tp);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

extern uint64_t get_time_base();

extern list_head timers;
void timer_create(TimerCallback func, void* parameter, uint64_t tick);

// this should be called by handle_int
void timer_check(void);

void latency(uint64_t time);

#endif
