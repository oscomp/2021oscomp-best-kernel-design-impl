#include <os/poll.h>
#include <os/ring_buffer.h>
#include <screen.h>
#include <os/io.h>
#include <assert.h>

static short get_revents(fd_t *fd, short events)
{
    if (events == POLLIN)
        return POLLIN;
    else
        assert(0);
}

/* poll if event(s) happens() */
/* 成功返回fds有revents的个数，失败返回-1 */
int32_t do_ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *tmo_p, const sigset_t *sigmask)
{
    debug();
    static int cnt = 0;
    fds = get_kva_of(fds, current_running->pgdir);
    if (tmo_p)
        tmo_p = get_kva_of(fds, current_running->pgdir);
    if (sigmask)
        sigmask = get_kva_of(fds, current_running->pgdir);

    nfds_t ret = 0;
    for (int i = 0; i < nfds; ++i)
    {
        if (fds->fd < 0){
            /* ignore this fds */
            fds->revents = 0lu;
            ret++;
        }
        else{
            int32_t fd_index = get_fd_index(fds->fd, current_running);
            if (fd_index == -1)
                return SYSCALL_FAILED;
            if (current_running->fd[fd_index].dev == STDIN){
                if (fds->events & POLLIN){
                    while (ring_buffer_empty(&stdin_buf)){
                        do_scheduler();
                    }
                    /* poll success */
                    fds->revents = get_revents(&current_running->fd[fd_index], POLLIN);
                    ret++;
                }
            }
            else if (current_running->fd[fd_index].piped == FD_PIPED){
                if (fds->events & POLLIN){
                    struct ring_buffer *rbuf = &(pipes[current_running->fd[fd_index].pip_num].rbuf);
                    uint8_t timer = 0;
                    for (; timer < 100; timer++)
                        if(ring_buffer_empty(rbuf)){
                            // log(0, "poll failed %d times", timer);
                            do_scheduler();
                        }
                        else
                            break;
                    if (timer == 100){
                        fds->revents = POLLHUP;
                    }
                    else{
                        ret++;
                        /* poll success */
                        fds->revents = get_revents(&current_running->fd[fd_index], POLLIN);
                    }
                }
            }
        }

        fds++;
    }
    return ret;
}
