#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <os/sched.h>
#include <log.h>

uchar stdin_buf[NORMAL_PAGE_SIZE];

/* success : read count */
int64 fat32_read(fd_num_t fd, uchar *buf, size_t count)
{
    uint8_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;
    // 如果是管道，就读取管道输出
    if (current_running->fd[fd_index].piped == FD_PIPED)
        return pipe_read(buf, current_running->fd[fd_index].pip_num, count);

    size_t mycount = 0;
    size_t realcount = min(count, current_running->fd[fd_index].length - current_running->fd[fd_index].pos);
    if (realcount == 0)
        return 0;
    assert(realcount > 0);
    uchar *buff = kalloc();

    ientry_t now_clus = get_clus_from_len(current_running->fd[fd_index].first_clus_num, current_running->fd[fd_index].pos);
    isec_t now_sec = get_sec_from_clus_and_offset(now_clus, current_running->fd[fd_index].pos % CLUSTER_SIZE);
    now_sec -= now_sec % READ_BUF_CNT; /* bufsize aligned */

    while (mycount < realcount){
        size_t pos_offset_in_buf = current_running->fd[fd_index].pos % BUFSIZE;
        size_t readsize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
        sd_read(buff, now_sec);
        memcpy(buf, buff + pos_offset_in_buf, readsize);
        buf += readsize;
        mycount += readsize;
        current_running->fd[fd_index].pos += readsize;
        if (current_running->fd[fd_index].pos % CLUSTER_SIZE == 0){
            now_clus = get_next_cluster(now_clus);
            now_sec = first_sec_of_clus(now_clus);
        }
        else{
            now_sec += READ_BUF_CNT ; // readsize / BUFSIZE == READ_BUF_CNT until last read
        }
    }

    kfree(buff);

    return realcount;
}

size_t fat32_readlinkat(fd_num_t dirfd, const char *pathname, char *buf, size_t bufsiz)
{
    char busybox_path[25] = "/root/static-bin/busybox";
    if (!strcmp(pathname, "/proc/self/exe")){
        strcpy(buf, busybox_path);
        return strlen(buf);
    }
    else
        assert(0);
}
