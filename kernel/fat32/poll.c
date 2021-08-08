#include <os/poll.h>

static short get_revents(fd_t *fd)
{
    return POLLIN | POLLHUP;
}

/* poll if event(s) happens() */
/* 成功返回fds有revents的个数，失败返回-1 */
int32_t do_ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *tmo_p, const sigset_t *sigmask)
{
    debug();
    nfds_t ret = 0;
    for (int i = 0; i < nfds; ++i)
    {
        log(0, "fd is %d, events is %lx", fds->fd, fds->events);
        if (fds->fd < 0){
            /* ignore this fds */
            fds->revents = 0lu;
            ret++;
        }
        else{
            uint8_t fd_index = get_fd_index(fds->fd, current_running);
            if (fd_index == -1)
                return SYSCALL_FAILED;
            if (current_running->fd[fd_index].poll_status & fds->events == fds->events){
                /* poll success */
                fds->revents = get_revents(&current_running->fd[fd_index]);
                ret++;
            }
        }
        fds++;
    }
    return ret;
}
