#include <os/fat32.h>


int16 fat32_link()
{
    return -1;
}

static void clear_all_garbage_clus_from_table(ientry_t cluster)
{
    assert(cluster != 0 && cluster != LAST_CLUS_OF_FILE);
    uchar *buf = kalloc();

    ientry_t *clusat;
    ientry_t next_clus, now_clus = cluster;

    do {
        log2(0, "clear clus number:%d", now_clus);
        next_clus = get_next_cluster(now_clus);

        isec_t sec = fat_sec_of_clus(now_clus) - fat.bpb.fat_sz;
        sd_read(buf, sec);
        clusat = (uint32_t *)((char*)buf + fat_offset_of_clus(now_clus));
        *clusat = 0;
        sd_write(buf, sec);

        now_clus = next_clus;
    } while (now_clus != LAST_CLUS_OF_FILE);

    kfree(buf);
}

/* remove file */
/* success return 0, fail return -1 */
int16 fat32_unlink(fd_num_t dirfd, const char* path_t, uint32_t flags)
{
    debug();
    log(0, "dirfd: %lx, path_t: %s, flags: %lx", dirfd, path_t, flags);
    int fd_index = get_fd_index(dirfd, current_running);
    if (dirfd != AT_FDCWD && fd_index < 0) return -1;

    uchar path[strlen(path_t) + 1];
    strcpy(path, path_t);
    if (path[strlen(path_t) - 1] == '\r')
    {
        assert(0);
        log(0, "handle r");
        path[strlen(path_t) - 1] = 0;
    }

    uchar *buf = kalloc(); // for search

    /* parse path */
    uint8 isend = 0;

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus; // now cluster num

    dentry_t *p = (dentry_t *)buf; // for loop

    if (path[0] == '/'){
        now_clus = fat.bpb.root_clus;
        temp2 = &path[1], temp1 = &path[1];
    }
    else{
        if (dirfd == AT_FDCWD)
            now_clus = cwd_first_clus;
        else
        {
            if (current_running->fd[fd_index].used)
                now_clus = current_running->fd[fd_index].first_clus_num;
            else
                return -1;
        }
        temp2 = path, temp1 = path;
    }

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        uint8 ignore;
        struct dir_pos top;

        if (isend){
            // success
            search_mode_t search_mode = (!strcmp(temp1, "."))? SEARCH_DIR :
                                        ((O_DIRECTORY & flags) == 0) ? SEARCH_FILE : 
                                        SEARCH_DIR;
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
                        log2(0, "garbage_clus is %d", garbage_clus);
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
                return 0;
            }
            // 2. not found
            else{
                kfree(buf);
                return -1;
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
            return -1;
        }
    }
}
