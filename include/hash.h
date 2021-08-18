#ifndef INCLUDE_HASH_H
#define INCLUDE_HASH_H

#include <type.h>

static inline uint8 hash8(char *buf, int len)
{
    uint8 result = 0;
    for (int i = 0; i < len; i++)
        result ^= buf[i];

    return result;
}

static inline uint16 hash16(char *buf, int len)
{
    uint16 result = 0;
    for (int i = 0; i < len / 2 * 2; i += 2)
        result ^= *(uint16 *)(buf + i);

    if (len % 2)
        result ^= (uint8)(buf[len-1]);
    
    return result;
}

#endif