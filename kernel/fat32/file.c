#include <os/fat32.h>

/* update time stamps */
/* times cound be NULL, but still check if it can open this file */
/* success return 0, fail return -1 */
int32_t do_utimensat(fd_num_t dirfd, const char *pathname, const struct timespec times[2], int32_t flags)
{
    #ifndef K210
    return -1;
    #endif

    debug();
    fd_num_t fd;
    if ((fd = fat32_open(dirfd, pathname, flags, 0)) != -1){
        int32_t fd_index;
        assert((fd_index = get_fd_index(fd, current_running)) != -1);
        fd_t *fdp = &current_running->fd[fd_index];
        fdp->atime_sec = 1383584112;
        fdp->atime_nsec = 0;
        fdp->mtime_sec = 1383584112;
        fdp->mtime_nsec = 0;
        fdp->ctime_sec = 1383584112;
        fdp->ctime_nsec = 0;
        fat32_close(fd);
        log(0, "success utimenat");
        return SYSCALL_SUCCESSED;
    }
    else{
        log(0, "file not exist");
        fd = fat32_open(dirfd, pathname, O_RDWR|O_CREAT, 0);
        assert(fat32_close(fd) == 0);
        return SYSCALL_SUCCESSED;
    }
}

/* send file content from in_fd to out_fd */
/* start from *offset */
/* offset should be set to *offset+count after sending */
/* success return realcount, fail return -1 */
size_t do_sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
{
    debug();
    log(0, "out:%d, in:%d, count:%d", out_fd, in_fd, count);
    int32_t fd_index_in, fd_index_out;
    if ((fd_index_in = get_fd_index(in_fd, current_running)) == -1
        || (fd_index_out = get_fd_index(out_fd, current_running)) == -1)
        return SYSCALL_FAILED;

    uchar *buff = kalloc();

    if (offset){
        log(0, "offset: %lx", *offset);
        fat32_lseek(in_fd, *offset, SEEK_SET);
    }
    size_t mycount = 0;
    while (mycount < count){
        ssize_t readsize = fat32_read(in_fd, buff, NORMAL_PAGE_SIZE);
        if (readsize <= 0) break;
        fat32_write(out_fd, buff, readsize);
        mycount += readsize; 
    }
    if (offset)
        *offset += mycount;
    return mycount;
}