#ifndef _DEBUG_H
#define _DEBUG_H
#include <stdio.h>

#define debug() \
    { \
        printk_port("TRACE:%s in %s:%d\n",__FUNCTION__,__FILE__,__LINE__); \
    }
    
#endif
