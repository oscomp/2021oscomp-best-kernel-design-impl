#include <os/io.h>
#include <sys/select.h>
#include <os/fat32.h>
#include <os/sched.h>
#include <screen.h>

/* success return 0, error return -1, timeout return 1 /=*/
static int handle_select(fd_num_t fd, int mode, uint64_t timeout_ticks)
{
    int32_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0)
        return -1;

    fd_t *fdp = &current_running->fd[fd_index];
    uint64_t final_ticks = get_ticks() + timeout_ticks;
    log(0, "fd is %d, mode is %d", fd, mode);
    log(0, "nowticks:%ld, final_ticks:%ld", get_ticks(), final_ticks);

    if (mode == SELECT_READ){
        if (fdp->dev == STDIN){            
            return wait_ring_buffer_read(&stdin_buf, final_ticks);;
        }
        else if (fdp->piped){            
            return wait_ring_buffer_read(&(pipes[fdp->pip_num].rbuf), final_ticks);;
        }
        else
            assert(0);
    }
    else if (mode == SELECT_WRITE){
        if (fdp->dev == STDOUT){
            return wait_ring_buffer_write(&stdout_buf, final_ticks);
        }
        else if (fdp->dev == STDERR){
            return wait_ring_buffer_write(&stderr_buf, final_ticks);
        }
        else if (fdp->piped){
            return wait_ring_buffer_write(&(pipes[fdp->pip_num].rbuf), final_ticks);
        }
        else
            return 0; /* Normal file is always OK to be written */
    }
    return -1;
}

static void clear_all_fds(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
    uint32_t fd;
    for (fd = 0; fd < nfds; fd++){
        if (readfds)
            FD_CLR(fd, readfds);
        if (writefds)
            FD_CLR(fd, writefds);
        if (exceptfds)
            FD_CLR(fd, exceptfds);
    }
}

/* select which file descriptor is ready for operation */
int do_pselect6(int nfds, fd_set *readfds, fd_set *writefds,
                   fd_set *exceptfds, const struct timespec *timeout,
                   const sigset_t *sigmask)
{
    debug();
    if (nfds < 0)
        return SYSCALL_FAILED;
    else if (nfds == 0)
        return 0;
    // assert(nfds <= 32);
    log(0, "nfds is %d", nfds);
    log(0, "size of fd_set is %d", sizeof(fd_set));

    uint64_t timeout_ticks = timespec_to_tick(timeout);
    log(0, "timeout_ticks is %ld", timeout_ticks);

    uint32_t fd = 0, ret_num = 0;
    for (fd = 0; fd < nfds; fd++){
        /* one time return 1 */
        if (readfds && FD_ISSET(fd, readfds)){
            int32_t ret;
            if ((ret = handle_select(fd, SELECT_READ, timeout_ticks)) < 0){
                return SYSCALL_FAILED;
            }
            else if (ret == 0){
                ret_num++;
                clear_all_fds(nfds, readfds, writefds, exceptfds);
                FD_SET(fd, readfds);
                break;
            }
            else{
                FD_CLR(fd, readfds);
            }
        }
        if (writefds && FD_ISSET(fd, writefds)){
            int32_t ret;
            if ((ret = handle_select(fd, SELECT_WRITE, timeout_ticks)) < 0){
                return SYSCALL_FAILED;
            }
            else if (ret == 0){
                ret_num++;
                clear_all_fds(nfds, readfds, writefds, exceptfds);
                FD_SET(fd, writefds);
                break;
            }
            else{
                FD_CLR(fd, readfds);
            }
        }
        if (exceptfds && FD_ISSET(fd, exceptfds))
            FD_CLR(fd, exceptfds);
    }
    return ret_num;
}