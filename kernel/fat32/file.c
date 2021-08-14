#include <os/fat32.h>

/* change name */
/* success return 0, fail return -1 */
int fat32_renameat2(fd_num_t olddirfd, const char *oldpath_const, fd_num_t newdirfd, const char *newpath_const, unsigned int flags)
{
    debug();
    log(0, "olddirfd: %lx, oldpath: %s", olddirfd, oldpath_const);
    log(0, "newdirfd: %lx, newpath: %s, flags: %d %lx", newdirfd, newpath_const, flags, flags);

    assert(olddirfd == AT_FDCWD);

    size_t oldpath_len = strlen(oldpath_const);
    uchar oldpath[oldpath_len + 1];
    strcpy(oldpath, oldpath_const);
    handle_windows_switch_line(oldpath);

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus; // now cluster num
    if (parse_filepath_init(oldpath, olddirfd, &now_clus, &temp1, &temp2) == SYSCALL_FAILED)
        return SYSCALL_FAILED;

    uchar *buf = kalloc(); // for search

    /* parse path */
    uint8 isend = 0;
    dentry_t *p = (dentry_t *)buf; // for loop

    /* 1. find old file and get file information from dentry */
    uint8 ignore;
    struct dir_pos top;
    dentry_t old_dentry;
    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        if (isend){
            // 1. found
            if ((p = search2(temp1, now_clus, buf, SEARCH_ALL, &ignore, &top)) != NULL){ // can't be ignored, because you cant delete rootdir
                log(0, "found old file");
                // (1) read in
                isec_t now_sec = top.sec; // not real now_sec, but fisrt sec of this buf
                now_clus = clus_of_sec(now_sec);
                assert(now_sec == first_sec_of_clus(now_clus) && top.len > 0 && top.offset < BUFSIZE);
                p = buf + top.offset;
                sd_read(buf, now_sec);

                // (2) write
                while (top.len--){
                    if (top.len == 0){
                        /* get dentry content */
                        memcpy(&old_dentry, p, sizeof(dentry_t));
                        log(0, "get old dentry");
                    }
                    p->filename[0] = 0xE5;
                    if (p + 1 == buf + BUFSIZE)
                        sd_write(buf, now_sec);
                    p = get_next_dentry(p, buf, &now_clus, &now_sec); // you can find next buff location
                }
                sd_write(buf, now_sec);
                break;
            }
            // 2. not found
            else{
                kfree(buf);
                return -ENOENT;
            }
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, buf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(buf);
            return -ENOENT;
        }
    }
    log(0, "old dentry filename: %s", old_dentry.filename);
    log(0, "old dentry attribute: %x", old_dentry.attribute);

    /* 2. create or overwrite new */

    assert(newdirfd == AT_FDCWD);

    size_t newpath_len = strlen(newpath_const);
    uchar newpath[newpath_len + 1];
    strcpy(newpath, newpath_const);
    handle_windows_switch_line(newpath);

    now_clus = -1;
    if (parse_filepath_init(newpath, newdirfd, &now_clus, &temp1, &temp2) == SYSCALL_FAILED)
        return SYSCALL_FAILED;
    log(0, "newpath: %lx, temp1: %lx, temp2: %lx, now_clus: %d", newpath, temp1, temp2, now_clus);

    isend = 0;
    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        if (isend){
            // success
            ientry_t dir_first_clus = now_clus;
            /* if found, need to delete it first */
            if ((p = search2(temp1, now_clus, buf, SEARCH_ALL, &ignore, &top)) != NULL){ // can't be ignored, because you cant delete rootdir
                log(0, "found new file at clus %d, delete it first", get_cluster_from_dentry(p));
                // (1) read dir page in
                isec_t now_sec = top.sec; // not real now_sec, but fisrt sec of this buf
                now_clus = clus_of_sec(now_sec);
                assert(now_sec == first_sec_of_clus(now_clus) && top.len > 0 && top.offset < BUFSIZE);
                p = buf + top.offset;
                sd_read(buf, now_sec);

                ientry_t garbage_clus;
                // (2) delete
                while (top.len--){
                    if (top.len == 0){
                        /* get garbage clus number */
                        garbage_clus = get_cluster_from_dentry(p);
                        log(0, "garbage_clus is %d", garbage_clus);
                    }
                    p->filename[0] = 0xE5;
                    if (p + 1 == buf + BUFSIZE)
                        sd_write(buf, now_sec);
                    p = get_next_dentry(p, buf, &now_clus, &now_sec); // you can find next buff location
                }
                sd_write(buf, now_sec);
                clear_all_garbage_clus_from_table(garbage_clus);
            }
            // 2. create new
            ientry_t new_file_clus = _create_new_dentry(temp1, dir_first_clus, buf, NULL, &old_dentry);
            log(0, "new_file_clus is %d", new_file_clus);
            kfree(buf);
            return SYSCALL_SUCCESSED;
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, buf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(buf);
            return -ENOENT;
        }
    }
}



/* update time stamps */
/* times cound be NULL, but still check if it can open this file */
/* success return 0, fail return -1 */
int32_t do_utimensat(fd_num_t dirfd, const char *pathname, const struct timespec times[2], int32_t flags)
{
    #ifndef K210
    return -1;
    #endif

    debug();
    int32_t fd;
    if ((fd = fat32_openat(dirfd, pathname, flags, 0)) >= 0){
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
        fd = fat32_openat(dirfd, pathname, O_RDWR|O_CREAT, 0);
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

    kfree(buff);
    return mycount;
}