#include <os/fat32.h>

ientry_t mount_clus;
char mountpoint[20];

/* mount special to dir */
/* 成功返回0，失败返回-1 */
int16 fat32_mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
    uchar *tempbuf = kalloc(); // for search

    uchar path[strlen(dir)+1]; strcpy(path, dir);

    uint8 isend = 0;

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus, new_clus; // now cluster num
    uint32_t parent_first_clus;

    dentry_t *p = (dentry_t *)tempbuf; // for loop

    if (path[0] == '/'){
        now_clus = fat.bpb.root_clus;
        temp2 = &path[1], temp1 = &path[1];
    }
    else{
        now_clus = cwd_first_clus;
        temp2 = path; temp1 = path;
    }

    strcpy(mountpoint, dir);

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';
        // printk_port("isend: %d\n", isend);
        uint8 ignore;

        if (isend){
            struct dir_pos pos;
            // found
            if (p = search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, &pos)){
                if (ignore){
                    // cannot mount on rootdir
                    kfree(tempbuf);
                    return -1;
                }
                else{
                    sd_read(tempbuf, pos.sec);
                    mount_clus = get_cluster_from_dentry(p); // store origin clus for umount
                    set_dentry_cluster(p, root_first_clus); // mount sd card
                    sd_write(tempbuf, pos.sec);
                    kfree(tempbuf);
                    return 0;
                }
            }

            kfree(tempbuf);
            return -1;
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(tempbuf);
            return -1;
        }
    }
}

int16 fat32_umount(const char *special, int flags)
{
    uchar *tempbuf = kalloc(); // for search

    uchar path[strlen(mountpoint)+1]; strcpy(path, mountpoint);

    uint8 isend = 0;

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus, new_clus; // now cluster num
    uint32_t parent_first_clus;

    dentry_t *p = (dentry_t *)tempbuf; // for loop

    if (path[0] == '/'){
        now_clus = fat.bpb.root_clus;
        temp2 = &path[1], temp1 = &path[1];
    }
    else{
        now_clus = cwd_first_clus;
        temp2 = path; temp1 = path;
    }

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';
        // printk_port("isend: %d\n", isend);
        uint8 ignore;

        if (isend){
            struct dir_pos pos;
            // found
            if (p = search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, &pos)){
                if (ignore){
                    // cannot umount rootdir
                    kfree(tempbuf);
                    return -1;
                }
                else{                    
                    sd_read(tempbuf, pos.sec);
                    set_dentry_cluster(p, mount_clus); // mount sd card
                    sd_write(tempbuf, pos.sec);
                    kfree(tempbuf);
                    return 0;
                }
            }

            kfree(tempbuf);
            return -1;
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(tempbuf);
            return -1;
        }
    }
}
