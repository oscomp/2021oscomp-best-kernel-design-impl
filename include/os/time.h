
#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include <type.h>
#include <os/list.h>

#define NANO 9
#define TIMEBASE 403000000
#define TICKCOUNT 62

typedef void (*TimerCallback)(void *parameter);
typedef uint32_t time_t;
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

/* for gettimeofday */
struct timespec {
    time_t tv_sec; // seconds
    uint32_t tv_nsec; // and nanoseconds
};

extern uint32_t time_base;
extern uint64_t time_elapsed;
extern uint64_t MHZ;

uint64_t get_timer(void);
uint64_t get_ticks(void);
void kernel_time_count();
void user_time_count();

uint64_t do_times(struct tms *tms);
int8_t do_gettimeofday(struct timespec *ts);

extern uint64_t get_time_base();

extern list_head timers;
void timer_create(TimerCallback func, void* parameter, uint64_t tick);

// this should be called by handle_int
void timer_check(void);

void latency(uint64_t time);

#endif
