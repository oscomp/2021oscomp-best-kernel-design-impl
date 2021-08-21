#ifndef INCLUDE_OS_FILEMAP_H
#define INCLUDE_OS_FILEMAP_H

#include <type.h>

typedef struct fat32_file_map_info{
    uintptr_t memory_kva; /* memory kernel virtual address */
    uint32_t sec; /* buf aligned */
    size_t offset; /* offset in buf */
    size_t len; /* not larger than buf */
    uint8_t flags;
}fat32_file_map_info_t;

#endif