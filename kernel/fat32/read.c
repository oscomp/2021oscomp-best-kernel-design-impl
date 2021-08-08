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
    debug();
    log(0, "fd:%d count:%d", fd, count);
    uint8_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used){
        log(0, "fd not found");
        return SYSCALL_FAILED;
    }
    // 如果是管道，就读取管道输出
    if (current_running->fd[fd_index].piped == FD_PIPED){
        log(0, "it's a pipe fd");
        return pipe_read(buf, current_running->fd[fd_index].pip_num, count);
    }

    #ifndef K210
    uchar *mybuff;
    size_t length = -1;
    static int cnt = 0;
    if (cnt++ == 0)
        get_elf_file("code", &mybuff, &length);
    else if (cnt++ == 1)
        get_elf_file("cmd", &mybuff, &length);
    log(0, "retcount: %d", length);
    memcpy(buf, mybuff, length);
    return length;
    #endif

    size_t mycount = 0;
    assert(count < (1lu << 63)); /* cannot be too large */
    fd_t *fdp = &current_running->fd[fd_index];
    int64_t realcount = min((int64_t)count, (int64_t)(fdp->length) - (int64_t)(fdp->pos));
    if (realcount <= 0)
        return 0;
    uchar *buff = kalloc();

    ientry_t now_clus = get_clus_from_len(current_running->fd[fd_index].first_clus_num, current_running->fd[fd_index].pos);
    isec_t now_sec = get_sec_from_clus_and_offset(now_clus, current_running->fd[fd_index].pos % CLUSTER_SIZE);
    now_sec -= now_sec % READ_BUF_CNT; /* bufsize aligned */
    // log(0, "now_clus:%d, now_sec:%d", now_clus, now_sec);

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

    log(0, "retcount: %d", realcount);
    return realcount;
}

size_t fat32_readlinkat(fd_num_t dirfd, const char *pathname, char *buf, size_t bufsiz)
{
    debug();
    log(0, "dirfd: %d; pathname: %s", dirfd, pathname);
    char busybox_path[25] = "/root/static-bin/busybox";
    if (!strcmp(pathname, "/proc/self/exe")){
        strcpy(buf, busybox_path);
        return strlen(buf);
    }
    else
        assert(0);
}

/* return count */
int64 fat32_readv(fd_num_t fd, struct iovec *iov, int iovcnt)
{
    debug();
    log(0, "fd:%d, iovcnt:%d", fd, iovcnt);
    uint8 fd_index = get_fd_index(fd, current_running);

    if (fd_index < 0)
        return SYSCALL_FAILED;
    if (current_running->fd[fd_index].dev == STDOUT || current_running->fd[fd_index].dev == STDERR)
        return SYSCALL_FAILED;

    size_t count = 0, this_count;

    for (uint32_t i = 0; i < iovcnt; i++){
        if ((this_count = fat32_write(fd, iov->iov_base, iov->iov_len)) == SYSCALL_FAILED)
            return SYSCALL_FAILED;
        count += this_count;
        log(0, "count is %d", count);
        iov++;
    }
    return count; 
}