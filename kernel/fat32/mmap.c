#include <os/fat32.h>
#include <os/stdio.h>
#include <os/sched.h>

/* 成功返回已映射区域的指针，失败返回-1*/
int64 fat32_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    uint32_t fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -1;

    if (!start){
        start = do_brk(0);
        do_brk(start + len);
    }

    uchar *buf = kalloc();
    fat32_read(fd, buf, len);

    current_running->fd[fd_index].mmap.start = start;
    current_running->fd[fd_index].mmap.len =len;
    current_running->fd[fd_index].mmap.prot = prot;
    current_running->fd[fd_index].mmap.flags = flags;
    current_running->fd[fd_index].mmap.off = off;

    memcpy(start, buf + off, len);

    return start;
}

/* 成功返回0，失败返回-1*/
int64 fat32_munmap(void *start, size_t len)
{
    for (int i = 0; i < NUM_FD; ++i)
    {
        if (current_running->fd[i].used == FD_USED && current_running->fd[i].mmap.start == start){
            fat32_seek(current_running->fd[i].fd_num, current_running->fd[i].mmap.off);
            fat32_write(current_running->fd[i].fd_num, start, len);
            return 0;
        }
    }
    return -1;
}
