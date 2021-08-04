#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <os/sched.h>

uchar stdin_buf[NORMAL_PAGE_SIZE];

/* success : read count */
int64 fat32_read(fd_num_t fd, uchar *buf, size_t count)
{
    debug();
    uint8_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;
    // 如果是管道，就读取管道输出
    if (current_running->fd[fd_index].piped == FD_PIPED)
        return pipe_read(buf, current_running->fd[fd_index].pip_num, count);

    size_t mycount = 0;
    size_t realcount = min(count, current_running->fd[fd_index].length);
    uchar *buff = kalloc();
    printk_port("buff is %lx\n", buff);
    ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
    isec_t now_sec = first_sec_of_clus(now_clus);

    while (mycount < realcount){
        size_t readsize = min(BUFSIZE, realcount - mycount);
        debug();
        sd_read(buff, now_sec);
        debug();
        memcpy(buf, buff, readsize);
        buf += readsize;
        mycount += readsize;
        current_running->fd[fd_index].pos += readsize;
        if (mycount % CLUSTER_SIZE == 0){
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
