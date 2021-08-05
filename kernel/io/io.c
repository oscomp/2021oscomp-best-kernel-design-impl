#include <os/fat32.h>

/* success return 0, fail return 0 */
int64_t do_ioctl(fd_num_t fd, uint64_t request, const char *argp[])
{
    uint32_t fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -1;

}