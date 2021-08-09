#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <os/fat32.h>
#include <os/ring_buffer.h>

int64_t do_ioctl(fd_num_t fd, uint64_t request, const char *argp[]);

#endif