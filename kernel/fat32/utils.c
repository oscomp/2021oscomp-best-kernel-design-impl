#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

/* from old dentry to new dentry*/
/* always return the next dentry, unless something wrong about the file system */
/* need a buf, or p is meanningless */
dentry_t *get_next_dentry(dentry_t *p, uchar *dirbuff, ientry_t *now_clus, isec_t *now_sec)
{
    p++;
    if ((uintptr_t)p - (uintptr_t)dirbuff == BUFSIZE){
        // read another
        *now_sec += BUFSIZE / SECTOR_SIZE;
        if (*now_sec - first_sec_of_clus(*now_clus) == fat.bpb.sec_per_clus){
            *now_clus = get_next_cluster(*now_clus); //another cluster
            *now_sec = first_sec_of_clus(*now_clus);
        }
        sd_read(dirbuff, *now_sec);
        p = dirbuff;
    }
    return p;
}

/* get next cluster number */
/* always return success */
uint32_t get_next_cluster(uint32_t cluster)
{
    uchar *buf2 = kalloc();
    // printk_port("buf2 is %lx\n", buf2);
    sd_read(buf2, fat_sec_of_clus(cluster));
    uint32_t ret = (*(uint32_t*)((char*)buf2 + fat_offset_of_clus(cluster)));
    kfree(buf2);
    return ret;
}

/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return BOTTOM dentry point if success, NULL if fail */
/* ignore if search "." or ".." in root dir */
/* pos赋值为最后一条目录 */
dentry_t *search(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos)
{
    // printk_port("p addr: %lx, buf: %lx\n", *pp, buf);
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    // printk_port("5\n");
    dentry_t *p = (dentry_t *)buf;
    uchar *filename = kalloc();

    *ignore = 0;

    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        long_dentry_t *q = (long_dentry_t *)p;
        uint8 item_num; /* dentry length */

        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){
            item_num = q->sequence & 0x0f; // entry num

            /* get filename */
            uint8 isbreak = 0;
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
                }
                p = get_next_dentry(p, buf, &now_clus, &now_sec);
                q = (long_dentry_t *)p;
            }
        }
        // short dentry
        else{
            item_num = 0;
            /* filename */
            uint8 name_cnt = 0;
            for (uint8 i = 0; i < SHORT_DENTRY_FILENAME_LEN; ++i)
            {
                if (p->filename[i] == ' ') break;
                else filename[name_cnt++] = p->filename[i];
            }
            if (p->extname[0] != ' ') filename[name_cnt++] = '.';
            for (uint8 i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            {
                if (p->extname[i] == ' ') break;
                else filename[name_cnt++] = p->extname[i];
            }
            filename[name_cnt++] = 0;
        }

        // printk_port("filename is %s\n", filename);
        // printk_port("name is %s\n", name);
        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            if (pos){
                pos->offset = (void*)p - (void*)buf;
                pos->len = item_num + 1;
                pos->sec = now_sec;
            }
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            if (pos){
                pos->offset = (void*)p - (void*)buf;
                pos->len = item_num + 1;
                pos->sec = now_sec;
            }
            kfree(filename);
            return p;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    if (mode == FILE_DIR && (!strcmp(name, ".") || !strcmp(name, "..")) && dir_first_clus == root_first_clus){
        *ignore = 1;
    }

    kfree(filename);
    return NULL;
}

/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return p if success, NULL if fail */
/* ignore if search "." or ".." in root dir */
/* pos赋值为第一条目录 */
dentry_t *search2(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos)
{
    // printk_port("p addr: %lx, buf: %lx\n", *pp, buf);
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    // printk_port("5\n");
    dentry_t *p = (dentry_t *)buf;
    uchar *filename = kalloc();

    *ignore = 0;

    dentry_t *top_pt; /* ret */
    isec_t top_sec;
    size_t top_len;

    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        top_pt = p;
        top_sec = now_sec;

        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){
            uint8 item_num = q->sequence & 0x0f; // entry num
            top_len = item_num + 1;
            /* get filename */
            uint8 isbreak = 0;
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
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME2_LEN; ++i){
                    // name1
                    unich = q->name2[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;            
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME3_LEN; ++i){
                    // name1
                    unich = q->name3[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);  
                    name_cnt++;             
                }
                p = get_next_dentry(p, buf, &now_clus, &now_sec);
                q = (long_dentry_t *)p;
            }
        }
        // short dentry
        else{
            top_len = 1;
            /* filename */
            uint8 name_cnt = 0;
            for (uint8 i = 0; i < SHORT_DENTRY_FILENAME_LEN; ++i)
            {
                if (p->filename[i] == ' ') break;
                else filename[name_cnt++] = p->filename[i];
            }
            if (p->extname[0] != ' ') filename[name_cnt++] = '.';
            for (uint8 i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            {
                if (p->extname[i] == ' ') break;
                else filename[name_cnt++] = p->extname[i];
            }
            filename[name_cnt++] = 0;
        }

        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            pos->offset = (void*)top_pt - (void*)buf;
            pos->sec = top_sec;
            pos->len = top_len;
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            pos->offset = (void*)top_pt - (void*)buf;
            pos->sec = top_sec;
            pos->len = top_len;
            kfree(filename);
            return p;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    if (mode == FILE_DIR && (!strcmp(name, ".") || !strcmp(name, "..")) && dir_first_clus == root_first_clus){
        *ignore = 1;
    }

    kfree(filename);
    return NULL;
}

/* set fd */
/* i = fd index, NOT fd num */
/* success 0, fail -1 */
uint8 set_fd(void *pcb_underinit, uint i, dentry_t *p, dir_pos_t *dir_pos, uint32_t flags)
{
    // i is index
    pcb_t *pcb_under = (pcb_t *)pcb_underinit;
    if (pcb_under->fd[i].used) return -1;

    pcb_under->fd[i].dev = DEFAULT_DEV;

    pcb_under->fd[i].first_clus_num = get_cluster_from_dentry(p);
    
    pcb_under->fd[i].flags = flags;

    pcb_under->fd[i].pos = 0;

    pcb_under->fd[i].length = get_length_from_dentry(p);

    pcb_under->fd[i].used = 1;

    memcpy(&pcb_under->fd[i].dir_pos, dir_pos, sizeof(dir_pos_t));

    pcb_under->fd[i].piped = FD_UNPIPED;

    pcb_under->fd[i].nlink = 1;

    pcb_under->fd[i].uid = 0;
    pcb_under->fd[i].gid = 0;

    pcb_under->fd[i].rdev = DEFAULT_DEV;

    pcb_under->fd[i].blksize = CLUSTER_SIZE;

    pcb_under->fd[i].atime_sec = 1383584112;
    pcb_under->fd[i].atime_nsec = 0;
    pcb_under->fd[i].mtime_sec = 1383584112;
    pcb_under->fd[i].mtime_nsec = 0;
    pcb_under->fd[i].ctime_sec = 1383584112;
    pcb_under->fd[i].ctime_nsec = 0;
    // printk_port("i: %d, used: %d\n", i, pcb_under->fd[1].used);
    return 0;
}

/* set dentry info from fdp */
void set_dentry_from_fd(dentry_t *p, fd_t *fdp)
{
    p->length = fdp->length;
}

/* return file descriptor index whose fd_num = fd*/
/* Not found return -1 */
int16 get_fd_index(fd_num_t fd, void *arg)
{
    pcb_t *pcb = (pcb_t *)arg;
    for (int i = 0; i < NUM_FD; ++i)
    {
        if (pcb->fd[i].used && pcb->fd[i].fd_num == fd){
            return i;
        }
    }
    return -1;
}

uint8 is_zero_dentry(dentry_t *p)
{
    uchar *n = (uchar *)p;
    uint8_t index;
    for (index = 0; index < 32; ++index)
    {
        if (*n++ != 0)
            break;
    }
    if (index == 32)
        return 1;
    else
        return 0;
}

/* filename compare */
/* same return 0, other return 0*/
/* capital-ignore */
/* safe cmp */
uint8_t filenamecmp(const char *name1, const char *name2)
{
    uchar name1_t[strlen(name1)+1], name2_t[strlen(name2)+1];
    strcpy(name1_t, name1); strcpy(name2_t, name2);
    for (int i = 0; i < strlen(name1); ++i)
        if (name1_t[i] >= 'A' && name1_t[i] <= 'Z')
            name1_t[i] = 'a' + name1_t[i] - 'A';
    for (int i = 0; i < strlen(name1); ++i)
        if (name2_t[i] >= 'A' && name2_t[i] <= 'Z')
            name2_t[i] = 'a' + name2_t[i] - 'A';
    return strcmp(name1_t, name2_t);
}

