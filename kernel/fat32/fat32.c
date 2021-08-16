#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

fat_t fat;
uchar root_buf[NORMAL_PAGE_SIZE];
uchar filebuf[NORMAL_PAGE_SIZE];
ientry_t cwd_first_clus = 0;
ientry_t cwd_clus = 0, root_clus = 0, root_first_clus = 0;
isec_t cwd_sec = 0, root_sec = 0;

uint8_t fat32_init()
{
    /* read FAT32 info */
    uint8_t *b = kalloc();
    // read superblock
    sd_read_sector(b, 0, 1);
    // store basic messages
    if (memcmp((char const*)(b + 82), "FAT32", 5))
        printk("not FAT32 volume");
    fat.bpb.byts_per_sec = *(uint8 *)(b + 11) + (*(uint8 *)(b + 12))*256;
    fat.bpb.sec_per_clus = *(b + 13);
    fat.bpb.rsvd_sec_cnt = *(uint16 *)(b + 14);
    fat.bpb.fat_cnt = *(b + 16);
    fat.bpb.hidd_sec = *(uint32 *)(b + 28);
    fat.bpb.tot_sec = *(uint32 *)(b + 32);
    fat.bpb.fat_sz = *(uint32 *)(b + 36);
    fat.bpb.root_clus = *(uint32 *)(b + 44);

    fat.first_data_sec = fat.bpb.rsvd_sec_cnt + fat.bpb.hidd_sec + fat.bpb.fat_cnt * fat.bpb.fat_sz;
    fat.data_sec_cnt = fat.bpb.tot_sec - fat.first_data_sec;
    fat.data_clus_cnt = fat.data_sec_cnt / fat.bpb.sec_per_clus;
    fat.byts_per_clus = fat.bpb.sec_per_clus * fat.bpb.byts_per_sec;

    printk("> [FAT32 init] byts_per_sec: %d\n", fat.bpb.byts_per_sec);
    printk("> [FAT32 init] root_clus: %d\n", fat.bpb.root_clus);
    printk("> [FAT32 init] sec_per_clus: %d\n", fat.bpb.sec_per_clus);
    printk("> [FAT32 init] fat_cnt: %d\n", fat.bpb.fat_cnt);
    printk("> [FAT32 init] fat_sz: %d\n", fat.bpb.fat_sz);
    printk("> [FAT32 init] fat_rsvd: %d\n", fat.bpb.rsvd_sec_cnt);
    printk("> [FAT32 init] fat_hid: %d\n", fat.bpb.hidd_sec);

    /* init metadata */
    init_inode();

    /* init pipe */
    init_pipe();

    kfree(b);

    return 0;
}

/* init metadata */
void init_inode()
{
    /* read root dir and store it for now*/
    cwd_first_clus = fat.bpb.root_clus;
    cwd_clus = fat.bpb.root_clus;
    cwd_sec = first_sec_of_clus(fat.bpb.root_clus);
    root_first_clus = cwd_first_clus;
    root_clus = cwd_clus;
    root_sec = cwd_sec;
    /* from root sector read buffsize */
    sd_read(root_buf, cwd_sec);    
}

/* read file from sd card and put it into readyqueue*/
/*  1: continue to read
   -1: busy
    0: end
    */ 
int8 fat32_read_test(const char *filename)
{
    /* busy */
    for (uint i = 1; i < NUM_MAX_TASK; ++i)
    {
        if (pcb[i].status != TASK_EXITED)
            return -1;
    }

    uchar *_elf_binary;
    uint32_t length;

#ifdef K210

    static dentry_t *p = (dentry_t *)root_buf;
    // 0x0f: long dentry
    while (p->attribute == 0x0f || 0xE5 == p->filename[0]){
        p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
    }
    /* now we are at a real dentry */

    // 0x00: time to end
    uint8_t index;
    if (is_zero_dentry(p)){
        printk_port("<return success>");
        return 0;
    }
    // no length or deleted
    // if (p->length == 0) {
    //     // printk_port("<cause 1>\n");
    //     p = get_next_dentry(p, root_buf, &root_clus, &root_sec); 
    //     return 1;
    // }
    // not rw-able
    // if (p->attribute != FILE_ATTRIBUTE_ARCH){ 
    //     // printk_port("<cause 2>\n");
    //     p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
    //     return 1;
    // }
    printk_port("%s\n", p->filename);
    printk_port("length:%d\n", p->length);
    p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
    return 1;

    if (!memcmp(p->filename, "TEST_",5) || !memcmp(p->filename,"TEXT",4) ||
        !memcmp(p->filename, "RUN", 3)){
        p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
        return 1;
    }
    /* Now we must read file */
    // readfile
    uint32_t cluster = get_cluster_from_dentry(p);
    uint32_t sec = first_sec_of_clus(cluster); // make sure you know the start addr
    uchar *file = allocproc(); // make sure space is enough
    uchar *temp = file; // for future use
    for (uint32_t i = 0; i < p->length; )
    {
        // 1. read 1 cluster
        sd_read(temp, sec);
        i += BUFSIZE;
        temp += BUFSIZE;
        // 2. compute sec number of next buf-size datablock
        if (i % CLUSTER_SIZE == 0){
            cluster = get_next_cluster(cluster);
            sec = first_sec_of_clus(cluster);
        }
        else
            sec += READ_BUF_CNT;
    }

    /* set elf_binary and length */
    _elf_binary = file;
    length = p->length;

    cwd_first_clus = root_first_clus;
    cwd_clus = first_sec_of_clus(cwd_first_clus);

#else

    get_elf_file(filename,&_elf_binary,&length);

#endif

    for (uint i = 1; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status == TASK_EXITED)
        {
            // init pcb
            pcb_t *pcb_underinit = &pcb[i];
            ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE;
            ptr_t user_stack = USER_STACK_ADDR;

            init_pcb_default(pcb_underinit, USER_PROCESS);

            // load file to memory
            uintptr_t pgdir = allocPage();
            clear_pgdir(pgdir);

            alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            alloc_page_helper(user_stack,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);

            uint64_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper,&pcb_underinit->elf);
            pcb_underinit->edata = pcb_underinit->elf.edata;            
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // prepare stack
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, NULL, NULL, NULL, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            #ifdef K210
            // free resource
            allocfree();
            p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
            #endif

            return -1;
        }
    assert(0);
}

/* write count bytes from buff to file in fd */
/* return fd_num: success
          -1: fail
        */
int32_t fat32_openat(fd_num_t fd, const uchar *path_const, uint32 flags, uint32 mode)
{
    debug();
    log(0, "fd:%d path:%s, len:%d, flags:%lx %d", fd, path_const, strlen(path_const), flags, flags & O_CREAT);
    assert(fd == AT_FDCWD);
    /* O_CREAT | O_DIRECTORY is not defined */
    if ((flags & (O_CREAT | O_DIRECTORY)) == (O_CREAT | O_DIRECTORY))
        return SYSCALL_FAILED;

    /* for const */
    uint32 path_len = strlen(path_const);
    uchar path[path_len+1];
    strcpy(path, path_const);
    /* handle \r */
    handle_windows_switch_line(path);

    /* handle /dev/zero */
    if (!strcmp(path_const, "/dev/zero")){
        log(0, "is /dev/zero");
        for (int i = 0; i < NUM_FD; ++i){
            if (!current_running->fd[i].used){
                current_running->fd[i].used = FD_USED;
                current_running->fd[i].dev = DEV_ZERO;
                return current_running->fd[i].fd_num;
            }
        }
        return -1;
    }
    else if (!strcmp(path_const, "/dev/null")){
        log(0, "is /dev/null");
        for (int i = 0; i < NUM_FD; ++i){
            if (!current_running->fd[i].used){
                current_running->fd[i].used = FD_USED;
                current_running->fd[i].dev = DEV_NULL;
                return current_running->fd[i].fd_num;
            }
        }
        return -1;
    }

    #ifndef K210
    for (int i = 0; i < NUM_FD; ++i)
        if (!current_running->fd[i].used){
            current_running->fd[i].used = FD_USED;
            return current_running->fd[i].fd_num;
        }
        return -1;
    #endif

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus; // now cluster num
    if (parse_filepath_init(path, fd, &now_clus, &temp1, &temp2) == SYSCALL_FAILED)
        return SYSCALL_FAILED;

    uint8 isend = 0;
    uchar *buf = kalloc(); // for search
    dentry_t *p = (dentry_t *)buf; // for loop


    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        uint8 ignore;

        if (isend){
            // success
            search_mode_t search_mode = (!strcmp(temp1, "."))? SEARCH_DIR :
                                        ((O_DIRECTORY & flags) != 0) ? SEARCH_DIR : 
                                        SEARCH_ALL;
            if (search_mode == SEARCH_DIR)
                log(0, "open dir");
            else
                log(0, "open any file");
            dir_pos_t dir_pos;
            // 1. found
            if ((p = search(temp1, now_clus, buf, search_mode, &ignore, &dir_pos)) != NULL || ignore == 1){
                if (ignore){
                    /* now clus is root clus, and this time is searching for "." or ".." */
                    assert(now_clus == root_clus);
                    p = buf;
                    set_dentry_cluster(p, now_clus);
                    p->attribute = FILE_ATTRIBUTE_CHDIR;
                    p->length = 0;
                    p->create_time_ms = 0;
                    p->create_time = 0x53D4; //10:30:40
                    p->create_date = 0x52BB; // 2021/5/27
                    p->last_visited = 0x52BB;
                    p->last_modified_time = 0x53D3;   //23:22
                    p->last_modified_date = 0x52BB;     //25:24
                }

                for (uint8 i = 0; i < NUM_FD; ++i)
                {
                    if (!set_fd(current_running, i, p, &dir_pos, flags)){
                        debug();                        
                        kfree(buf);
                        return current_running->fd[i].fd_num; // use i as index
                    }
                }
                // no available fd
            }
            // 2.create
            else if ((flags & O_CREAT) != 0){
                ientry_t new_clus = _create_new_file(temp1, now_clus, buf, &dir_pos, FILE_FILE);
                assert(dir_pos.sec == BUFF_ALIGN(dir_pos.sec));
                sd_read(buf, dir_pos.sec);
                p = (dentry_t *)((void*)buf + dir_pos.offset);

                for (uint8 i = 0; i < NUM_FD; ++i)
                {
                    if (!set_fd(current_running, i, p, &dir_pos, flags)){ //new file length = 0
                        kfree(buf);
                        return current_running->fd[i].fd_num; // use i as index
                    }
                }
                // no available fd
            }
            kfree(buf);
            return (int32_t)(-ENOENT);
        }
        else{
            // printk_port("dirname: %s\n", temp1);
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, buf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(buf);
            return (int32_t)(-ENOENT);
        }
    }
}

/* check if this file can be accessed */
/* if can, return 0; else return -1 */
int32_t fat32_faccessat(fd_num_t dirfd, const char *pathname, int mode, int flags)
{
    debug();
    #ifndef K210
    return -1;
    #endif

    int32_t fd;
    // int32_t ret;
    if ((fd = fat32_openat(dirfd, pathname, flags, mode)) >= 0)
    {
        fat32_close(fd);
        return SYSCALL_SUCCESSED;
    }
    else
        return fd;
}

/* FOR NOW return 0 */
int32_t fat32_fcntl(fd_num_t fd, int32_t cmd, int32_t arg)
{
    debug();
    log(0, "fd:%d, cmd:%d, arg:%d", fd, cmd, arg);
    int32_t fd_index;

    if ((fd_index = get_fd_index(fd, current_running)) == -1) // old doesn't exists, cannot dup
        return SYSCALL_FAILED;

    if (cmd == F_GETFL)
        return current_running->fd[fd_index].flags;

    else if (cmd == F_SETFD)
        current_running->fd[fd_index].flags = arg;

    else if (cmd == F_DUPFD_CLOEXEC){
        int32_t fd_min = SYSCALL_FAILED;
        for (uint8_t i = 0; i < NUM_FD; i++){
            if (!current_running->fd[i].used)
                if (fd_min == SYSCALL_FAILED)
                    fd_min = current_running->fd[i].fd_num;
                else
                    fd_min = min(fd_min, current_running->fd[i].fd_num);
        }
        /* found min free fd_num, and if no free fd, fd_min is -1 */
        log(0, "fd_min: %d", fd_min);
        if (fd_min == SYSCALL_FAILED)
            return SYSCALL_FAILED;
        else{
            fd_min = max(fd_min, arg);
            log(0, "final fd_min is %d", fd_min);
            assert(fat32_dup3(fd, fd_min, IGNORE) != SYSCALL_FAILED);
            return fd_min;
        }

    }
    return SYSCALL_SUCCESSED;
}

/* duplicate file descriptor */
/* success return fd_num, fail return -1 */
fd_num_t fat32_dup(fd_num_t fd)
{
    debug();
    log(0, "dup fd : %d", fd);
    int32_t fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1) // old doesn't exists, cannot dup
        return -1;
    for (int i = 0; i < NUM_FD; ++i)
        // find an available one, i is index
        if (!current_running->fd[i].used){
            redirect_fd(&current_running->fd[i], fd_index);
            current_running->fd[i].used = FD_USED;
            // fd[i] must be unused
            return current_running->fd[i].fd_num;
        }
    /* no available */
    return -1;
}

/* duplicate file descriptor with assigned fd */
/* success return fd_num, fail return -1 */
fd_num_t fat32_dup3(fd_num_t old, fd_num_t new, uint8 no_use)
{
    debug();
    log(0, "old: %d, new:%d", old, new);

    if (old == new)
        return SYSCALL_FAILED;

    int32_t old_fd_index, new_fd_index;
    if ((old_fd_index = get_fd_index(old, current_running)) == -1){
        log(0, "old not exist");
        return -1;
    }

    /* Now new could be open and not redirected, which means we need to close it first */
    if ((new_fd_index = get_fd_index(new, current_running)) != -1){
        log(0, "new exist");
        // new_fd_index = get_my_fd_index(new, current_running);
        // if (current_running->fd[new_fd_index].redirected == FD_UNREDIRECTED){
        fat32_close(new);
        // }
        // current_running->fd[new_fd_index].used = FD_UNUSED;
    }

    /* 1. there is fd, whose fd_num is new, and available */
    /* if you do fat32_close just before, there must be one file descriptor who matches this loop */
    uint8_t i;
    for (i = 0; i < NUM_FD; ++i){
        // find an available one, i is index
        if (!current_running->fd[i].used && current_running->fd[i].fd_num == new){
            log(0, "just found");
            redirect_fd(&current_running->fd[i], old_fd_index);
            current_running->fd[i].used = FD_USED;
            return new;
        }
    }
    /* 2. there is available fd, and its num should be set as new */
    for (i = 0; i < NUM_FD; ++i){
        // find an available one, i is index
        if (!current_running->fd[i].used){
            log(0, "finally found");
            redirect_fd(&current_running->fd[i], old_fd_index);
            current_running->fd[i].fd_num = new; /* fd_num should be new */
            current_running->fd[i].used = FD_USED;
            return new;
        }
    }
    /* no available */
    return -1;
}

/* seek pos */
/* return pos if success, fail return -1 */
int64_t fat32_lseek(fd_num_t fd, size_t off, uint32_t whence)
{
    debug();
    int32_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;
    switch(whence){
        case SEEK_SET:
        current_running->fd[fd_index].pos = off;
        break;
        case SEEK_CUR:
        current_running->fd[fd_index].pos += off;
        break;
        case SEEK_END:
        current_running->fd[fd_index].pos = current_running->fd[fd_index].length + off;
        break;
        default: assert(0);
        break;
    }
    return current_running->fd[fd_index].pos;
}


uchar unicode2char(uint16_t unich)
{
    return (unich >= 65 && unich <= 90)? unich - 65 + 'A' :
        (unich >= 97 && unich <= 122)? unich - 97 + 'a' :
        (unich == 95)? '_' : 
        (unich == 46)? '.':
        (unich == 0x20)? ' ':
        0;  
}

unicode_t char2unicode(char ch)
{
    return (ch >= 'A' && ch <= 'Z')? 65 + ch - 'A' :
            (ch >= 'a' && ch <= 'z')? 97 + ch - 'a':
            (ch == '_')? 95 :
            (ch == '.')? 46 :
            (ch == ' ')? 0x20:
            0;
}

// void get_file_name_from_long_dentry()
// {

// }

/* go to child dir */
/* success 0, fail -1 */
int16 fat32_chdir(const char* path_t)
{
    uchar *buff = kalloc();

    uchar path[strlen(path_t) + 1];
    strcpy(path, path_t);

    if (!strcmp("/", path))
    {
        cwd_first_clus = root_first_clus;
        return 0;
    }

    char *temp1, *temp2;

    uint isend = 0;

    dentry_t *p;
    ientry_t now_clus;

    if (path[0] == '/'){
        now_clus = fat.bpb.root_clus;
        temp1 = &path[1], temp2 = &path[1];
    }
    else{
        now_clus = cwd_first_clus;
        temp1 = path; temp2 = path;
    }

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0' || *(temp2+1) == '\0' ) isend = 1;
        *temp2 = '\0';

        uint8 ignore;

        if (isend){
            // 1. not found
            if ((p = search(temp1, now_clus, buff, SEARCH_DIR, &ignore, NULL)) == NULL && ignore == 0){
                // no available dx
                kfree(buff);
                return -1;
            }
            // 2. found
            else{
                // printk_port("goto %d\n", get_cluster_from_dentry(p));
                // printk_port("HI: %d, LO: %d", p->HI_clusnum, p->LO_clusnum);
                cwd_first_clus = get_cluster_from_dentry(p);
                cwd_clus = cwd_first_clus;
                cwd_sec = first_sec_of_clus(cwd_clus);
                kfree(buff);
                return 0;
            }
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, buff, SEARCH_DIR, &ignore, NULL)) != NULL || ignore == 1){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(buff);
            return -1;
        }
    }
}


/* make a directory */
/* success 0, fail -1 */
int16 fat32_mkdirat(fd_num_t dirfd, const uchar *path_const, uint32_t mode)
{
    debug();
    log(0, "path_const is %s, mode is %lx", path_const, mode);

    uchar *tempbuf = kalloc(); // for search

    uchar path[strlen(path_const)+1]; strcpy(path, path_const);

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
        if (dirfd == AT_FDCWD)
            now_clus = cwd_first_clus;
        else{
            int32_t dirfd_index = get_fd_index(dirfd, current_running);
            if (current_running->fd[dirfd_index].used)
                now_clus = current_running->fd[dirfd_index].first_clus_num;
            else{
                kfree(tempbuf);
                return -1;
            }
        }
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
            // already exists
            if (search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, NULL) || ignore){
                kfree(tempbuf);
                return (-EEXIST);
            }
            _create_new_file(temp1, now_clus, tempbuf, NULL, FILE_DIR);

            kfree(tempbuf);
            return SYSCALL_SUCCESSED;
        }
        else{
            // search dir until fail or goto search file
            if ((p = search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, NULL)) != NULL || ignore){
                now_clus = (ignore) ? now_clus : get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
                continue ;
            }
            kfree(tempbuf);
            return (-ENOENT);
        }
    }

}

/* write clus */
/* old pt new */
/* old_clus为0时，只把new_clus标记为已使用 */
void write_fat_table(uint32_t old_clus, uint32_t new_clus, uchar *buff)
{
    uint32_t *clusat;
    if (old_clus){
        /* TABLE 1*/
        sd_read(buff, fat1_sec_of_clus(old_clus));
        clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(old_clus));
        *clusat = 0xffffffffu & new_clus;
        sd_write(buff, fat1_sec_of_clus(old_clus));
        /* TABLE 0*/
        sd_read(buff, fat2_sec_of_clus(old_clus));
        clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(old_clus));
        *clusat = 0xffffffffu & new_clus;
        sd_write(buff, fat2_sec_of_clus(old_clus));
    }
    /* TABLE 1*/
    sd_read(buff, fat1_sec_of_clus(new_clus));
    clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(new_clus));
    *clusat = LAST_CLUS_OF_FILE;
    sd_write(buff, fat1_sec_of_clus(new_clus));
    /* TABLE 0*/
    sd_read(buff, fat2_sec_of_clus(new_clus));
    clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(new_clus));
    *clusat = LAST_CLUS_OF_FILE;
    sd_write(buff, fat2_sec_of_clus(new_clus));
}

/* get current working dir name */
/* success: return pt to filename*/
/* fail: NULL */
uchar *fat32_getcwd(uchar *buf, size_t size)
{
    debug();
    #ifndef K210
    strcpy(buf, "/");
    return buf;
    #endif

    // printk_port("cwd66: %d\n", cwd_first_clus);
    if (size < 2) // at leaat for '/' and '\0'
        return NULL;

    uchar *buff = kalloc(); // for sdread
    uchar *output = kalloc(); // for final output
    uchar *temp_filename = kalloc(); // for temp store
    uchar *filename;
    size_t temp_size;
    ientry_t old_clus, now_clus = cwd_first_clus;
    dentry_t *p ;

    *temp_filename = 0; // for strlen and strcat
    temp_size = 0;

    uint8 is_root_dir = 1;
    uint8 ignore;

    while ((p = search("..", now_clus, buff, SEARCH_DIR, &ignore, NULL)) != NULL){

        is_root_dir = 0;

        old_clus = now_clus; // old_clus = now_clus
        now_clus = get_cluster_from_dentry(p); // now_clus = parent_dir_first_clus
        filename = get_filename_from_clus(old_clus, now_clus, buff); // filename = myname

        // must found
        assert(filename != NULL);

        strcpy(output, "/");
        // printk_port("output1 :%s\n", output);
        strcat(output, filename);
        // printk_port("output2 :%s\n", output);
        strcat(output, temp_filename);
        // printk_port("output3 :%s\n", output);
        strcpy(temp_filename, output);
        // printk_port("tempname :%s\n", temp_filename);

        temp_size = strlen(temp_filename);
        if (buf && temp_size >= size){
            // exceeding, = is also valid, as no space for '\0'
            kfree(temp_filename);
            kfree(output);
            kfree(buff);
            return NULL;
        }
    }

    // arrived root dir
    if (is_root_dir)
        strcpy(output, "/");
    else
        strcpy(output, temp_filename);

    kfree(temp_filename);
    kfree(buff);

    if (!buf){
        buf = do_brk(0);
        buf = do_brk(buf + strlen(output) + 1);
    }

    // if (!buf)
    //     buf = (uchar*)kmalloc(strlen(output) + 1);

    strcpy(buf, output);
    kfree(output);
    return buf;
}

/* close fd */
/* success return 0, fail return 1*/
int64 fat32_close(fd_num_t fd)
{
    debug();
    log(0, "thread %d is closing %d", current_running->tid, fd);

    int32_t fd_index, my_fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -SYSCALL_FAILED;
    my_fd_index = get_my_fd_index(fd, current_running);

    fd_t *fdp = &current_running->fd[my_fd_index];
    if (fdp->redirected == FD_REDIRECTED){
        log(0, "closing a redirected fd");
        for (uint8_t i = 0; i < NUM_FD; i++){
            if (current_running->fd[i].used && current_running->fd[i].redirected &&
                current_running->fd[i].redirected_fd_index == my_fd_index)
                /* multiple level link */
                current_running->fd[i].redirected_fd_index = current_running->fd[my_fd_index].redirected_fd_index;
        }
        clear_all_valid(fdp);
        /* go to close the real one */
        return SYSCALL_SUCCESSED;
    }
    else
    {
        /* for all the duplicated file descriptors who were directly-redirected to me, modify them */
        uint8_t i = 0;
        for (i = 0; i < NUM_FD; i++){
            if (current_running->fd[i].used == FD_USED && current_running->fd[i].redirected == FD_REDIRECTED &&
                current_running->fd[i].redirected_fd_index == my_fd_index){
                fd_num_t this_fd_num = current_running->fd[i].fd_num;
                memcpy(&current_running->fd[i], &current_running->fd[my_fd_index], sizeof(fd_t));
                current_running->fd[i].fd_num = this_fd_num; /* cannot copy fd_num */
                for (uint8_t j = i + 1; j < NUM_FD; j++){
                    /* i is the first file descriptor who point to my_fd_index, so start with i + 1 */
                    if (current_running->fd[j].used == FD_USED && current_running->fd[j].redirected == FD_REDIRECTED &&
                        current_running->fd[j].redirected_fd_index == my_fd_index)
                        current_running->fd[j].redirected_fd_index = i;
                }
                break;
            }
        }

        if (i == NUM_FD && fdp->piped == FD_PIPED){
            /* if nobody redirect to it ,close pipe */
            if (fdp->is_pipe_read && close_pipe_read(fdp->pip_num) == SYSCALL_FAILED)
                return SYSCALL_FAILED;
            if (fdp->is_pipe_write && close_pipe_write(fdp->pip_num) == SYSCALL_FAILED)
                return SYSCALL_FAILED;
        }
        /* else, nothing need to be done */   
             
        /* dentry information has changed when write */
        /* disable myself */
        clear_all_valid(fdp);
        return SYSCALL_SUCCESSED;
    }
}

void free_all_fds(pcb_t *pcb)
{
    for (uint8_t i = 0; i < NUM_FD; i++){
        if (pcb->fd[i].used == FD_UNUSED)
            continue;
        fd_t *fdp = &pcb->fd[i];

        if (fdp->redirected == FD_UNREDIRECTED && fdp->piped == FD_PIPED){
            /* if nobody redirect to it ,close pipe */
            if (fdp->is_pipe_read && close_pipe_read(fdp->pip_num) == SYSCALL_FAILED)
                assert(0);
            if (fdp->is_pipe_write && close_pipe_write(fdp->pip_num) == SYSCALL_FAILED)
                assert(0);
        }
    }   
}

/* read as many sectors as we can */
void sd_read(char *buf, uint32_t sec)
{
    sd_read_sector(buf, sec, READ_BUF_CNT);
}

/* write as many sectors as we can */
void sd_write(char *buf, uint32_t sec)
{
    for (int i = 0; i < READ_BUF_CNT; ++i)
    {
        sd_write_sector(buf, sec, 1);
        buf += SECTOR_SIZE;
        sec++;
    }    
}

/* cache write back */
int fat32_fsync(fd_num_t fd)
{
    debug();
    return SYSCALL_SUCCESSED;
}