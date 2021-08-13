#include <os/fat32.h>
#include <utils/utils.h>
#include <pgtable.h>

/* get directory entry */
/* return readsize if success, -1 fail */
/* make sure buf is enough */
int64 fat32_getdents64(fd_num_t fd, char *outbuf, uint32_t len)
{
    debug();
    log(0, "fd:%d, outbuf:%lx, len:%x", fd, outbuf, len);

    int32_t fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == SYSCALL_FAILED)
        return SYSCALL_FAILED;

    ientry_t now_clus = get_clus_from_len(current_running->fd[fd_index].first_clus_num, current_running->fd[fd_index].pos);
    isec_t now_sec = BUFF_ALIGN( get_sec_from_clus_and_offset(now_clus, current_running->fd[fd_index].pos % CLUSTER_SIZE) );
    log(0, "pos is %d", current_running->fd[fd_index].pos);
    
    size_t readsize = 0, newsize = 0;
    uint32_t basic_size = sizeof(struct linux_dirent64);

    /* for output */
    uchar *buf = kalloc();
    uchar *filename = kalloc();// :d_name

    sd_read(buf, now_sec);
    dentry_t *p = (dentry_t *)((void*)buf + current_running->fd[fd_index].pos % BUFSIZE);
    /* similar to search */
    while (!is_zero_dentry(p) && readsize < len){
        
        ientry_t file_first_clus;

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
            current_running->fd[fd_index].pos += sizeof(dentry_t);
        }
        if (is_zero_dentry(p)){
            log(0, "break");
            break;
        }

        log(0, "test p delete: %lx", p->filename[0]);
        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){

            uint8 item_num = q->sequence & 0x0f; // entry num

            /* get filename */
            uint16_t unich; // unicode

            while (item_num--){
                uint8 name_cnt = 0;
                for (uint8 i = 0; i < LONG_DENTRY_NAME1_LEN; ++i){
                    // name1
                    unich = q->name1[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;
                    assert(name_cnt < NORMAL_PAGE_SIZE);           
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME2_LEN; ++i){
                    // name2
                    unich = q->name2[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;       
                    assert(name_cnt < NORMAL_PAGE_SIZE);      
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME3_LEN; ++i){
                    // name3
                    unich = q->name3[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);  
                    name_cnt++;  
                    assert(name_cnt < NORMAL_PAGE_SIZE);            
                }
                p = get_next_dentry(p, buf, &now_clus, &now_sec);
                q = (long_dentry_t *)p;
                current_running->fd[fd_index].pos += sizeof(dentry_t);
            }
        }
        // short dentry
        else{
            /* filename is always capital */
            uint8 name_cnt = 0;
            for (uint8 i = 0; i < SHORT_DENTRY_FILENAME_LEN; ++i)
            {
                if (p->filename[i] == ' ') break;
                else filename[name_cnt++] = (p->filename[i] >= 'A' && p->filename[i] <= 'Z')? p->filename[i] - 'A' + 'a' : p->filename[i];
            }
            if (p->extname[0] != ' ') filename[name_cnt++] = '.';
            for (uint8 i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            {
                if (p->extname[i] == ' ') break;
                else filename[name_cnt++] = (p->filename[i] >= 'A' && p->filename[i] <= 'Z')? p->filename[i] - 'A' + 'a' : p->filename[i];
            }
            filename[name_cnt++] = 0;
        }

        file_first_clus = get_cluster_from_dentry(p);
        log(0, "file_first_clus is %d", file_first_clus);

        log(0, "readsize: %d", readsize);
        newsize = ROUNDUP(readsize + basic_size + strlen(filename) + 1, 3);
        log(0, "newsize: %d", newsize);
        if (newsize > len)
            break;

        /* output */
        struct linux_dirent64 *now = (struct linux_dirent64 *)(outbuf + readsize);

        now->d_ino = file_first_clus;
        now->d_off = file_first_clus * CLUSTER_SIZE;
        now->d_reclen = newsize - readsize;
        now->d_type = (p->attribute & FILE_ATTRIBUTE_CHDIR)? DT_DIR : DT_REG;
        memcpy(now->d_name, filename, strlen(filename) + 1);
        log(0, "ino is %d, off is %lx, reclen is %d, type is %d", now->d_ino, now->d_off, now->d_reclen, now->d_type);
        log(0, "filename is %s, len %d", now->d_name, strlen(now->d_name));
        assert(now->d_name[0]);

        p = get_next_dentry(p, buf, &now_clus, &now_sec);
        current_running->fd[fd_index].pos += sizeof(dentry_t);
        readsize = newsize;
    }

    kfree(filename);
    kfree(buf);
    log(0, "ret size is %d", readsize);
    return readsize;
}
