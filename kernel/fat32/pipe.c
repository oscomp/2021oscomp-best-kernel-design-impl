#include <os/fat32.h>
#include <os/ring_buffer.h>

pipe_t pipes[NUM_PIPE] = {0};
/* init pipe */
void init_pipe()
{
    for (int i = 0; i < NUM_PIPE; ++i){
        pipes[i].r_valid = PIPE_INVALID;
        pipes[i].w_valid = PIPE_INVALID;
    }
}

/* create pipe */
/* success return 0, fail return -1; */
/* fd[2] is fd num, should be transmitted to fd_index */
/* fd.pip_num is set to be the index of pipe */
int16 fat32_pipe2(fd_num_t *fd, int32 mode)
{
    debug();
    int16 fd_index[2] = {-1,-1};
    for (int i = 0; i < NUM_FD; ++i)
        if (current_running->fd[i].used == FD_UNUSED){
            if (fd_index[0] == -1){
                fd[0] = current_running->fd[i].fd_num;
                fd_index[0] = i;
            }
            else if (fd_index[1] == -1){
                fd[1] = current_running->fd[i].fd_num;
                fd_index[1] = i;
                break;
            }
        }

    pipe_num_t pip_num;
    for (pip_num = 0; pip_num < NUM_PIPE; ++pip_num)
    {
        if ((pipes[pip_num].r_valid | pipes[pip_num].w_valid) == PIPE_INVALID){
            pipes[pip_num].fd[0] = fd[0];
            pipes[pip_num].fd[1] = fd[1];
            pipes[pip_num].pid = current_running->pid;
            init_ring_buffer(&pipes[pip_num].rbuf);
            init_list_head(&pipes[pip_num].wait_list);
            pipes[pip_num].r_valid = PIPE_VALID;
            pipes[pip_num].w_valid = PIPE_VALID;
            break;
        }
    }

    if (pip_num == NUM_PIPE){
        log(0, "no pipe available");
        return SYSCALL_FAILED;
    }

    for (int i = 0; i < 2; ++i)
    {
        current_running->fd[fd_index[i]].used = FD_USED;
        current_running->fd[fd_index[i]].piped = FD_PIPED;
        current_running->fd[fd_index[i]].pip_num = pip_num;
    }
    log(0, "return fd1:%d, fd2:%d", *fd, *(fd+1));
    return SYSCALL_SUCCESSED;
}

/* read from pipe */
/* return read count */
int64 pipe_read(uchar *buf, pipe_num_t pip_num, size_t count)
{
    uint64_t buf_kva = get_kva_of(buf, current_running->pgdir); /* in case thread changes */
    if (pipes[pip_num].r_valid < PIPE_VALID)
        return -1;
    else if (pipes[pip_num].r_valid > PIPE_VALID)
        log(0, "dangerous pipe read, more than 1 outlet");
    size_t readsize;
    while ((readsize = read_ring_buffer(&pipes[pip_num].rbuf, buf_kva, count)) <= 0){
        do_block(&current_running->list, &pipes[pip_num].wait_list);
        do_scheduler();
    }
    log(0, "pipe read %d", readsize);
    return readsize;
}

/* write pipe */
/* return write count */
ssize_t pipe_write(uchar *buf, pipe_num_t pip_num, size_t count)
{
    if (pipes[pip_num].w_valid < PIPE_VALID)
        return -1;
    else if (pipes[pip_num].w_valid > PIPE_VALID)
        log(0, "dangerous pipe write, more than 1 inlet");
    int64_t writesize = write_ring_buffer(&pipes[pip_num].rbuf, buf, count);
    /* someone maybe waiting for read pipe */
    if (writesize > 0 && !list_empty(&pipes[pip_num].wait_list))
        do_unblock(pipes[pip_num].wait_list.next);
    /* someone maybe polling */
    log(0, "pipe write %d", writesize);
    return writesize; // write count
}
