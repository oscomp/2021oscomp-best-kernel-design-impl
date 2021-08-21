#include <os/fat32.h>
#include <os/ring_buffer.h>
#include <os/sched.h>
#include <os/stdio.h>

pipe_t pipes[NUM_PIPE] = {0};
/* init pipe */
void init_pipe()
{
    for (int i = 0; i < NUM_PIPE; ++i){
        pipes[i].r_valid = PIPE_INVALID;
        pipes[i].w_valid = PIPE_INVALID;
    }
}

/* close read end of this pipe */
/* actually, just decrease the "valid" value */
/* the other end could still be PIPE_VALID */
int16 close_pipe_read(pipe_num_t pip_num)
{
    if (pipes[pip_num].r_valid == PIPE_INVALID)
        return SYSCALL_FAILED;
    log(0, "pipe %d r_valid-- by %d and is %d", pip_num, current_running->tid, pipes[pip_num].r_valid - 1);
    pipes[pip_num].r_valid--;
    return SYSCALL_SUCCESSED;
}

/* close read end of this pipe */
/* actually, just decrease the "valid" value */
/* the other end could still be PIPE_VALID */
int16 close_pipe_write(pipe_num_t pip_num){
    if (pipes[pip_num].w_valid == PIPE_INVALID)
        return SYSCALL_FAILED;
    log(0, "pipe %d w_valid-- by %d and is %d", pip_num, current_running->tid, pipes[pip_num].w_valid - 1);
    pipes[pip_num].w_valid--;
    return SYSCALL_SUCCESSED;
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
            pipes[pip_num].pid = current_running->pid;
            init_ring_buffer(&pipes[pip_num].rbuf);
            init_list_head(&pipes[pip_num].read_wait_list);
            init_list_head(&pipes[pip_num].write_wait_list);
            pipes[pip_num].r_valid = PIPE_VALID;
            pipes[pip_num].w_valid = PIPE_VALID;
            break;
        }
    }

    if (pip_num == NUM_PIPE){
        printk_port("no pipe available");
        return SYSCALL_FAILED;
    }

    for (int i = 0; i < 2; ++i)
    {
        current_running->fd[fd_index[i]].used = FD_USED;
        current_running->fd[fd_index[i]].piped = FD_PIPED;
        current_running->fd[fd_index[i]].pip_num = pip_num;
        if (i == 0) current_running->fd[fd_index[i]].is_pipe_read = 1;
        else current_running->fd[fd_index[i]].is_pipe_write = 1;
    }
    log(0, "return fd1:%d, fd2:%d", *fd, *(fd+1));
    return SYSCALL_SUCCESSED;
}

/* read from pipe */
/* return read count */
int64 pipe_read(uchar *buf, pipe_num_t pip_num, size_t count)
{
    // uint64_t buf_kva = get_kva_of(buf, current_running->pgdir); /* in case thread changes */
    if (pipes[pip_num].r_valid < PIPE_VALID)
        return -1;
    // else if (pipes[pip_num].r_valid > PIPE_VALID)
    //     log(0, "dangerous pipe read, more than 1 outlet");
    size_t readsize;
    while ((readsize = read_ring_buffer(&pipes[pip_num].rbuf, buf, count)) <= 0){
        if (pipes[pip_num].w_valid >= PIPE_VALID && !is_sig_recved(current_running)){
            do_block(&current_running->list, &pipes[pip_num].read_wait_list);
            do_scheduler();
        }
        else{
            readsize = 0;
            break;
        }
    }
    /* someone may be waiting for writing pipe */
    if (readsize > 0 && !list_empty(&pipes[pip_num].write_wait_list)){
        do_unblock(pipes[pip_num].write_wait_list.next);
    }
    log(0, "pipe[%d] read %d", pip_num, readsize);
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
    int64_t writesize;
    while((writesize = write_ring_buffer(&pipes[pip_num].rbuf, buf, count)) <= 0){
        if (pipes[pip_num].r_valid >= PIPE_VALID && !is_sig_recved(current_running)){
            do_block(&current_running->list, &pipes[pip_num].write_wait_list);
            do_scheduler();
        }
        else{
            writesize = 0;
            break;
        }
    }
    /* someone may be waiting for reading pipe */
    if (writesize > 0 && !list_empty(&pipes[pip_num].read_wait_list)){
        do_unblock(pipes[pip_num].read_wait_list.next);
    }
    /* someone maybe polling */
    log(0, "pipe[%d] write %d", pip_num, writesize);
    return writesize; // write count
}
