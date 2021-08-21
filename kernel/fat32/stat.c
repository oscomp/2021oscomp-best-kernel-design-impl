#include <os/fat32.h>

/* show file status using fdnum */
/* success return 0，fail return -1 */
int16 fat32_fstat(fd_num_t fd, struct kstat *kst)
{
    debug();
    #ifndef K210
    return -1;
    #endif

    // debug2();
    int fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -1;
    /* exists, now read fd info */
    kst->st_dev = current_running->fd[fd_index].dev;
    kst->st_ino = current_running->fd[fd_index].first_clus_num;
    kst->st_mode = current_running->fd[fd_index].flags;
    kst->st_nlink = current_running->fd[fd_index].nlink;
    // kst->st_uid = current_running->fd[fd_index].uid;
    // kst->st_gid = current_running->fd[fd_index].gid;
    kst->st_uid = 0;
    kst->st_gid = 0;
    kst->st_rdev = current_running->fd[fd_index].dev;
    kst->st_size = current_running->fd[fd_index].length;
    kst->st_blksize = fat.byts_per_clus;
    kst->st_blocks = current_running->fd[fd_index].length / fat.byts_per_clus;
    kst->st_atime_sec = 1383584112;
    kst->st_mtime_sec = 1383584112;
    kst->st_ctime_sec = 1383584112;
    kst->st_atime_nsec = 0;
    kst->st_mtime_nsec = 0;
    kst->st_ctime_nsec = 0;
    return 0;
}

/* show file status using pathname */
/* FOR NOW pathname is a file, not a directory */
/* success return 0, fail return -1 */
int fat32_fstatat(fd_num_t dirfd, const char *pathname, struct stat *statbuf, int32 flags)
{
    debug();
    log(0, "dirfd: %lx", dirfd);
    log(0, "pathname: %s", pathname);
    int32_t fd;
    fd_t *fdp = NULL;
    if ((fd = fat32_openat(dirfd, pathname, O_RDONLY, flags)) < 0 ){
        log(0, "is not a file");
        if ((fd = fat32_openat(dirfd, pathname, O_DIRECTORY, flags)) < 0 ){
            log(0, "is not a directory either, open failed");
            return fd;
        }
        else{
            log(0, "is a directory %d", fd);
            uint8_t fd_index = get_fd_index(fd, current_running);
            fdp = &current_running->fd[fd_index];
        }
    }
    else{
        log(0, "is a file %d", fd);
        uint8_t fd_index = get_fd_index(fd, current_running);
        fdp = &current_running->fd[fd_index];
    }
    assert(fdp);

    statbuf->st_dev = fdp->dev;
    statbuf->st_ino = fdp->first_clus_num;
    statbuf->st_mode = 0777|fdp->mode;
    statbuf->st_nlink = fdp->nlink;
    statbuf->st_uid = 0;
    statbuf->st_gid = 0;
    statbuf->st_blksize = SECTOR_SIZE;
    statbuf->st_blocks = (fdp->length % SECTOR_SIZE)? fdp->length / SECTOR_SIZE + 1 : fdp->length / SECTOR_SIZE;
    statbuf->st_size = fdp->length;
    log(0, "file length is %ld", statbuf->st_size);
    statbuf->st_atim.tv_sec=1628233081;
    statbuf->st_atim.tv_nsec=386050164;
    statbuf->st_mtim.tv_sec=1628233079;
    statbuf->st_mtim.tv_nsec=862011410;
    statbuf->st_ctim.tv_sec=1628233079;
    statbuf->st_ctim.tv_nsec=862011410;
    fat32_close(fd);
    return 0;
}
