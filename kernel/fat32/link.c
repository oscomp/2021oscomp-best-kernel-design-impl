#include <os/fat32.h>


int16 fat32_link()
{
    debug();
    return -1;
}

/* remove file */
/* success return 0, fail return -1 */
int fat32_unlinkat(fd_num_t dirfd, const char* path_t, uint32_t flags)
{
    debug();
    log(0, "dirfd: %x, path_t: %s, flags: %lx", dirfd, path_t, flags);
    assert(dirfd == AT_FDCWD);

    uchar path[strlen(path_t) + 1];
    strcpy(path, path_t);
    handle_windows_switch_line(path);

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus; // now cluster num

    if (parse_filepath_init(path, dirfd, &now_clus, &temp1, &temp2) == SYSCALL_FAILED)
        return SYSCALL_FAILED;

    uchar *buf = kalloc(); // for search

    /* parse path */
    uint8 isend = 0;
    dentry_t *p = (dentry_t *)buf; // for loop

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        uint8 ignore;
        struct dir_pos top;

        if (isend){
            // success
            search_mode_t search_mode = ((AT_REMOVEDIR & flags) != 0) ? SEARCH_DIR : 
                                        SEARCH_FILE;
            log(0, "search_mode : %d", search_mode);
            // 1. found
            if ((p = search2(temp1, now_clus, buf, search_mode, &ignore, &top)) != NULL){ // can't be ignored, because you cant delete rootdir
                log(0, "found this file");
                // (1) read in
                isec_t now_sec = top.sec; // not real now_sec, but fisrt sec of this buf
                now_clus = clus_of_sec(now_sec);
                assert(now_sec == first_sec_of_clus(now_clus) && top.len > 0 && top.offset < BUFSIZE);
                p = buf + top.offset;
                sd_read(buf, now_sec);

                ientry_t garbage_clus;
                // (2) write
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
                // (3) clear fat table
                clear_all_garbage_clus_from_table(garbage_clus);
                kfree(buf);
                return SYSCALL_SUCCESSED;
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
}
