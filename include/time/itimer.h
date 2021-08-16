#ifndef INCLUDE_ITIMER_H
#define INCLUDE_ITIMER_H

#include <type.h>

typedef struct itimer_param{
    time_t interval;
    void * mypcb;
}itimer_param_t;

#endif