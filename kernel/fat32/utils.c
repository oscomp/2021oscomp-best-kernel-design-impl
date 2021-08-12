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
/* sec is buf aligned */
dentry_t *get_next_dentry(dentry_t *p, uchar *dirbuff, ientry_t *now_clus, isec_t *now_sec)
{
    p++;
    if ((uintptr_t)p - (uintptr_t)dirbuff == BUFSIZE){
        // read another
        *now_sec += READ_BUF_CNT;
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

        if ((p->attribute & FILE_ATTRIBUTE_CHDIR) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            if (pos){
                pos->offset = (void*)p - (void*)buf;
                pos->len = item_num + 1;
                log(0, "offset %lx, len %d", pos->offset, pos->len);
                log(0, "nowsec: %d aligned:%d", now_sec, first_sec_of_clus(clus_of_sec(now_sec)));
                pos->sec = BUFF_ALIGN(now_sec);
                log(0, "result sec: %d", pos->sec);
            }
            kfree(filename);
            return p;
        }
        else if ((p->attribute & FILE_ATTRIBUTE_CHDIR) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            if (pos){
                pos->offset = (void*)p - (void*)buf;
                pos->len = item_num + 1;
                log(0, "offset %lx, len %d", pos->offset, pos->len);
                log(0, "nowsec: %d aligned:%d", now_sec, first_sec_of_clus(clus_of_sec(now_sec)));
                pos->sec = BUFF_ALIGN(now_sec);
                log(0, "result sec: %d", pos->sec);
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
            log(0, "offset is %lx", pos->offset);
            log(0, "sec is %d, first sec of this clus is %d", top_sec, first_sec_of_clus(clus_of_sec(top_sec)));
            pos->sec = BUFF_ALIGN(top_sec);
            log(0, "buf-aligned sec is %d", pos->sec);
            pos->len = top_len;
            log(0, "len is %d", top_len);
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            pos->offset = (void*)top_pt - (void*)buf;
            log(0, "offset is %lx", pos->offset);
            log(0, "sec is %d, first sec of this clus is %d", top_sec, first_sec_of_clus(clus_of_sec(top_sec)));
            pos->sec = BUFF_ALIGN(top_sec);
            log(0, "buf-aligned sec is %d", pos->sec);
            pos->len = top_len;
            log(0, "len is %d", top_len);
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

    // pcb_under->fd[i].dev = DEFAULT_DEV;

    pcb_under->fd[i].first_clus_num = get_cluster_from_dentry(p);

    pcb_under->fd[i].mode = (p->attribute & FILE_ATTRIBUTE_CHDIR)? S_IFDIR:
                            (p->attribute & FILE_ATTRIBUTE_GDIR)? S_IFREG:
                            0;
    
    pcb_under->fd[i].flags = flags;

    pcb_under->fd[i].length = get_length_from_dentry(p);

    /* if append, pos = length; else, pos = 0 */
    pcb_under->fd[i].pos = (flags & O_APPEND) ? pcb_under->fd[i].length : 0;

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

/* return real file descriptor index(if it is a copied file descriptor, return its origin) */
/* Not found return -1 */
int16 get_fd_index(fd_num_t fd, void *arg)
{
    pcb_t *pcb = (pcb_t *)arg;
    for (int i = 0; i < NUM_FD; ++i)
    {
        if (pcb->fd[i].used == FD_USED && pcb->fd[i].fd_num == fd){
            while (pcb->fd[i].redirected == FD_REDIRECTED)
                i = pcb->fd[i].redirected_fd_index;
            return i;
        }
    }
    return -1;
}

/* return this fd index(even though if is a copied file descriptor, return itself) */
/* not found return -1 */
int16 get_my_fd_index(fd_num_t fd, void *arg)
{
    pcb_t *pcb = (pcb_t *)arg;
    for (int i = 0; i < NUM_FD; ++i)
        if (pcb->fd[i].used == FD_USED && pcb->fd[i].fd_num == fd)
            return i;
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
    int32_t strlen1 = strlen(name1), strlen2 = strlen(name2);
    if (strlen1 != strlen2)
        return 1;

    uchar name1_t[strlen1+1], name2_t[strlen2+1];
    strcpy(name1_t, name1); strcpy(name2_t, name2);
    for (int i = 0; i < strlen1; ++i)
        if (name1_t[i] >= 'A' && name1_t[i] <= 'Z')
            name1_t[i] = 'a' + name1_t[i] - 'A';
    for (int i = 0; i < strlen2; ++i)
        if (name2_t[i] >= 'A' && name2_t[i] <= 'Z')
            name2_t[i] = 'a' + name2_t[i] - 'A';
    return strcmp(name1_t, name2_t);
}

/* redirect a file descriptor */
/* new must be unused */
void redirect_fd(fd_t *new, fd_num_t old_fd_index)
{
    assert(new->used == FD_UNUSED);
    new->used = FD_USED;
    new->redirected = FD_REDIRECTED;
    new->redirected_fd_index = old_fd_index;
}

void clear_all_valid(fd_t *fdp)
{
    fdp->piped = FD_UNPIPED;
    fdp->is_pipe_read = PIPE_INVALID;
    fdp->is_pipe_write = PIPE_INVALID;
    fdp->redirected = FD_UNREDIRECTED;
    fdp->used = FD_UNUSED;
}

/* create new file */
/* temp1: filename */
/* now_clus : now dir cluster */
/* dir_pos could be NULL, and if it's not NULL, it is set to be the BOTTOM dentry */
/* mode: FILE_FILE is creating file, FILE_DIR is making new directory */
ientry_t _create_new(uchar *temp1, ientry_t now_clus, uchar *tempbuf, dir_pos_t *dir_pos, file_type_t mode)
{
    int noextname = 1;
    uchar *temp3 = temp1;
    while (*temp3 != '.' && *temp3 != '\0') temp3++;
    if (*temp3 == '.' && mode == FILE_FILE){
        noextname = 0; *temp3 = '\0';temp3++;
    }

    /* filename length, ext name length and total length */

    uint32_t filename_length = strlen(temp1);
    uint32_t extname_length = strlen(temp3);

    if (!noextname)
        *(temp3 - 1) = '.';
    uint32_t length = strlen(temp1);
    
    uint32_t parent_first_clus = now_clus;
    dentry_t *p;

    /* never move temp1 */

    uint32_t demand, sec;
    if (filename_length <= 8)
        demand = 1;
    else
        demand = length / LONG_DENTRY_NAME_LEN + 2; // 1 for div operation round, 1 for short entry

    // find empty entry
    p = search_empty_entry(now_clus, tempbuf, demand, &sec);
    now_clus = clus_of_sec(sec);
    assert(sec == first_sec_of_clus(now_clus));
    ientry_t new_clus = search_empty_clus(tempbuf);

    assert(p != NULL);

    // 1. prepare short entry
    dentry_t new_dentry;
    memset(&new_dentry, 0, sizeof(new_dentry));

    // filename:
    if (demand == 1){
        // only short
        for (uint i = 0; i < filename_length; ++i)
            new_dentry.filename[i] = (*(temp1+i) <= 'z' && *(temp1+i) >= 'a')? *(temp1+i) - 'a' + 'A' : *(temp1+i);
        for (uint i = filename_length; i < 8; ++i)
            new_dentry.filename[i] = ' ';
    }
    else{
        for (uint i = 0; i < 6; ++i)
            new_dentry.filename[i] = (*(temp1+i) <= 'z' && *(temp1+i) >= 'a')? *(temp1+i) - 'a' + 'A' : *(temp1+i);
        new_dentry.filename[6] = '~'; new_dentry.filename[7] = '1'; // dont think about 2 or more
    }

    // extname:
    if (noextname)
        for (uint i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            new_dentry.extname[i] = ' ';
    else{
        for (uint i = 0; i < extname_length; ++i)
            new_dentry.extname[i] = (*(temp3+i) <= 'z' && *(temp3+i) >= 'a')? *(temp3+i) - 'a' + 'A' : *(temp3+i);
        for (uint i = extname_length; i < 3; ++i)
            new_dentry.extname[i] = ' ';
    }

    // attribute
    if (mode == FILE_DIR)
        new_dentry.attribute = FILE_ATTRIBUTE_CHDIR;
    else
        new_dentry.attribute = FILE_ATTRIBUTE_GDIR;
    // reserved
    if (mode == FILE_DIR || demand > 1)
        new_dentry.reserved = 0;
    else
        new_dentry.reserved = 0x18;
    // create time
    new_dentry.create_time_ms = 0;
    new_dentry.create_time = 0x53D4; //10:30:20
    new_dentry.create_date = 0x52BB; // 2021/5/27
    new_dentry.last_visited = 0x52BB;
    new_dentry.HI_clusnum = (new_clus >> 16) & ((1lu << 16) - 1);      //21:20
    new_dentry.last_modified_time = 0x53D3;   //23:22
    new_dentry.last_modified_date = 0x52BB;     //25:24
    new_dentry.LO_clusnum = new_clus & ((1lu << 16) - 1);      //27:26
    new_dentry.length = 0x0000;     //31:28

    // 2. prepare long entry
    long_dentry_t long_entries[demand - 1];
    if (demand > 1){
        uint now_len;
        for (uint j = 0; j < demand - 1; ++j)
        {
            // name
            for (uint i = 0; i < LONG_DENTRY_NAME1_LEN; ++i)
            {
                now_len = j*LONG_DENTRY_NAME_LEN + i;
                long_entries[j].name1[i] = (now_len < length)? char2unicode(*(temp1+now_len)) :
                                            (now_len == length)? 0x0000 :
                                            0xffffu;
            }
            for (uint i = 0; i < LONG_DENTRY_NAME2_LEN; ++i)
            {
                now_len = j*LONG_DENTRY_NAME_LEN + LONG_DENTRY_NAME1_LEN + i;
                long_entries[j].name2[i] = (now_len < length)? char2unicode(*(temp1+now_len)) :
                                            (now_len == length)? 0x0000 :
                                            0xffffu;
            }
            for (uint i = 0; i < LONG_DENTRY_NAME3_LEN; ++i)
            {
                now_len = j*LONG_DENTRY_NAME_LEN + LONG_DENTRY_NAME1_LEN + LONG_DENTRY_NAME2_LEN + i;
                long_entries[j].name3[i] = (now_len < length)? char2unicode(*(temp1+now_len)) :
                                            (now_len == length)? 0x0000 :
                                            0xffffu;
            }
            long_entries[j].sequence = (j == demand - 2)? (0x40 | (j + 1)) & 0x4f : (j + 1) & 0x0f;   
            long_entries[j].attribute = FILE_ATTRIBUTE_LONG;
            long_entries[j].reserved = 0;
            // checksum
            uint8 sum = 0; uchar *fcb = &new_dentry;
            for (int i = 0; i < 11; ++i)
                sum = (sum & 0x1 ? 0x80 : 0) + (sum >> 1) + *fcb++;
            long_entries[j].checksum = sum;
            long_entries[j].reserved2 = 0lu;
        }
    }

    // 3. write entry
    sd_read(tempbuf, sec); // if no sec then p is meaningless
    for (uint i = 0; i < demand - 1; ++i)
    {
        memcpy(p, &long_entries[demand - 2 - i], sizeof(dentry_t));

        if (p + 1 == tempbuf + BUFSIZE) // boundary
            sd_write(tempbuf, sec);
        p = get_next_dentry(p, tempbuf, &now_clus, &sec);
    }
    memcpy(p, &new_dentry, sizeof(new_dentry));
    sd_write(tempbuf, sec);
    /* dir pos */
    if (dir_pos){
        dir_pos->offset = (void*)p - (void*)tempbuf;
        dir_pos->len = demand;
        dir_pos->sec = sec;
        log(0, "create, offset:%lx, len:%d, sec:%d", dir_pos->offset, dir_pos->len, dir_pos->sec);
    }
    else
        log(0, "dir_pos is NULL");

    //4. write dir itself
    if (mode == FILE_DIR){
        sec = first_sec_of_clus(new_clus);
        memset(tempbuf, 0, BUFSIZE);
        p = tempbuf;

        //"."
        p->filename[0] = '.'; for (uint i = 1; i < SHORT_DENTRY_FILENAME_LEN; i++) p->filename[i] = ' ';
        for (uint i = 0; i < SHORT_DENTRY_EXTNAME_LEN; i++) p->extname[i] = ' ';
        // attribute
        p->attribute = FILE_ATTRIBUTE_CHDIR;
        // reserved
        p->reserved = 0;
        // create time
        p->create_time_ms = 0;
        p->create_time = 0x53D4; //10:30:40
        p->create_date = 0x52BB; // 2021/5/27
        p->last_visited = 0x52BB;
        p->HI_clusnum = (new_clus >> 16) & ((1lu << 16) - 1);      //21:20
        p->last_modified_time = 0x53D3;   //23:22
        p->last_modified_date = 0x52BB;     //25:24
        p->LO_clusnum = new_clus & ((1lu << 16) - 1);      //27:26
        p->length = 0x0000;     //31:28

        p++;
        //".."
        p->filename[0] = '.'; p->filename[1] = '.'; for (uint i = 2; i < SHORT_DENTRY_FILENAME_LEN; i++) p->filename[i] = ' ';
        for (uint i = 0; i < SHORT_DENTRY_EXTNAME_LEN; i++) p->extname[i] = ' ';
        // attribute
        p->attribute = FILE_ATTRIBUTE_CHDIR;
        // reserved
        p->reserved = 0;
        // create time
        p->create_time_ms = 0;
        p->create_time = 0x53D4; //10:30:40
        p->create_date = 0x52BB; // 2021/5/27
        p->last_visited = 0x52BB;
        p->HI_clusnum = (parent_first_clus >> 16) & ((1lu << 16) - 1);      //21:20
        p->last_modified_time = 0x53D3;   //23:22
        p->last_modified_date = 0x52BB;     //25:24
        p->LO_clusnum = parent_first_clus & ((1lu << 16) - 1);      //27:26
        p->length = 0x0000;     //31:28

        sd_write(tempbuf, sec);
    }

    // 5. write cluster
    write_fat_table(0, new_clus, tempbuf);

    return new_clus;
}

/* return empty clus num */
uint32_t search_empty_clus(uchar *buf)
{
    uint32_t now_sec = fat.first_data_sec - fat.bpb.fat_sz;
    sd_read(buf, now_sec);
    uint j = 0;
    while (1){
        uint i;
        for (i = 0; i < 4; ++i)
        {
            if (*(buf + i + (j % BUFSIZE)))
                break;
        }
        if (i == 4) break;
        else j += 4;
        if (j % BUFSIZE == 0){
            now_sec += READ_BUF_CNT;
            sd_read(buf, now_sec);
        }
    }
    write_fat_table(0, j/4, buf);
    return j/4;
}

/* search for demand number of CONTINUOUS empty entry for mkdir */
/* make sure demand != 0 */
/* suggest we can find a result, or endless loop */
/* return top and modify sec as top*/
/* make sure a dirent is no longer than 1 clus */
dentry_t *search_empty_entry(uint32_t dir_first_clus, uchar *buf, uint32_t demand, uint32_t *sec)
{
    uint32_t now_clus = dir_first_clus;
    uint32_t old_clus;
    *sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, *sec);
    dentry_t *p = (dentry_t *)buf;
    uint32_t cnt = 0;
    dentry_t *ret_p = p;
    uint32_t ret_sec = *sec;

    while(1){
        old_clus = now_clus; 
        if (!is_zero_dentry(p) && 0xE5 != p->filename[0]){
            // printk_port("restart cnt\n");
            cnt = 0; 
            p = get_next_dentry(p, buf, &now_clus, sec);          
            ret_p = p; ret_sec = *sec;
        }
        else{
            // printk_port("cnt is %d\n", cnt + 1);
            cnt++;
            p = get_next_dentry(p, buf, &now_clus, sec);
            if (cnt == demand) {*sec = ret_sec; return ret_p;}
        }               
        // printk_port("nowclus:%x\n", now_clus);
        if (now_clus == LAST_CLUS_OF_FILE){
            now_clus = search_empty_clus(buf);

            // printk_port("old clus:%d\n", old_clus);
            // printk_port("new clus:%d\n", now_clus);

            // printk_port("new sec:%d, ret_sec:%d\n", first_sec_of_clus(now_clus), ret_sec);

            write_fat_table(old_clus, now_clus, buf); // make sure old can find new

            // printk_port("here, p:%lx, ret_p:%lx\n", p, ret_p);
            // printk_port("get_next_cluster: %d\n", get_next_cluster(old_clus));

            // need to return beginning sec, so change sec only if not start cnt
            // already start cnt: ret_sec is real
            // not already start: ret_sec is first sec of (0x0fffffff), so we have next 2 lines
            if (!cnt) *sec = first_sec_of_clus(now_clus); 
            else *sec = ret_sec;
            // ret_p = p = buf; (must be)
            return ret_p;
        }    
    }
}

/* search if cluster exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return filename if success, NULL if fail */
uchar *get_filename_from_clus(ientry_t cluster, uint32_t dir_first_clus, uchar *buf)
{
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    dentry_t *p = (dentry_t *)buf;
    uchar *filename = kalloc();

    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }
        
        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){

            uint8 item_num = q->sequence & 0x0f; // entry num

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
                else filename[name_cnt++] = p->filename[i];
            }
            filename[name_cnt++] = 0;
        }

        if (get_cluster_from_dentry(p) == cluster){
            strcpy(buf, filename);
            kfree(filename);
            return buf;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    kfree(filename);

    return NULL;
}
