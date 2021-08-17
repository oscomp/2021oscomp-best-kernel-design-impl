#ifndef INCLUDE_PIPE_H
#define INCLUDE_PIPE_H

#include <type.h>
#include <os/list.h>
#include <os/ring_buffer.h>

/* pipe */
#define NUM_PIPE 16
#define PIPE_INVALID 0
#define PIPE_VALID 1
#define PIPE_ERROR 2

#define FD_UNPIPED 0
#define FD_PIPED 1

typedef struct pipe{
    struct ring_buffer rbuf;
    pid_t pid; // parent id
    list_head read_wait_list;
    list_head write_wait_list;
    uint8 r_valid;
    uint8 w_valid;
    uint8 mode;
}pipe_t;

void init_pipe();
int16 close_pipe_read(pipe_num_t pip_num);
int16 close_pipe_write(pipe_num_t pip_num);
int16 fat32_pipe2(fd_num_t *fd, int32 mode);
int64 pipe_read(uchar *buf, pipe_num_t pip_num, size_t count);
int64 pipe_write(uchar *buf, pipe_num_t pip_num, size_t count);

#endif