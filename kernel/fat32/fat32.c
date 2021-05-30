#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) > (y)) ? (y) : (x))

#define BUFSIZE (min(NORMAL_PAGE_SIZE, CLUSTER_SIZE))
#define READ_BUF_CNT (BUFSIZE/SECTOR_SIZE)

fat_t fat;
uchar root_buf[NORMAL_PAGE_SIZE];
uchar filebuf[NORMAL_PAGE_SIZE];
ientry_t cwd_first_clus;
ientry_t cwd_clus, root_clus, root_first_clus;
isec_t cwd_sec, root_sec;
pipe_t pipes[NUM_PIPE];

uchar stdout_buf[NORMAL_PAGE_SIZE];
uchar stdin_buf[NORMAL_PAGE_SIZE];

// #define ACCEPT_NUM  26
// uchar accept_table[26][20] = {{"brk"}, {"chdir"}, {"clone"}, {"close"}, {"dup"}, {"dup2"}, {"execve"},
//     {"exit"}, {"fork"}, {"fstat"}, {"getcwd"}, {"getdents"}, {"getpid"}, {"getppid"},
//     {"gettimeofday"}, {"mkdir_"}, {"open"}, 
//     {"openat"}, {"read"}, {"sleep"}, {"times"}, 
//     {"uname"},{"wait"}, {"waitpid"}, {"write"} ,{"yield"} 
            
//     };
// // uchar accept_table[26][20] = {{"brk"}, {"chdir"}, {"clone"}, {"close"}, {"dup"}, {"dup2"}, {"execve"},
// //     {"exit"}, {"fork"}, {"fstat"}, {"getcwd"}, {"getdents"}, {"getpid"}, {"getppid"},
// //     {"gettimeofday"}, {"mkdir_"}, {"open"}, 
// //     {"openat"}, {"read"}, {"sleep"}, {"times"}, 
// //     {"uname"},{"wait"}, {"waitpid"}, {"write"} ,{"yield"} 
            
// //     };


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

/* init pipe */
void init_pipe()
{
    for (int i = 0; i < NUM_PIPE; ++i){
        pipes[i].r_valid = PIPE_INVALID;
        pipes[i].w_valid = PIPE_INVALID;
    }
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
    // dentry_t *pp = buf;
    // while (!is_zero_dentry(pp)){
    //     printk_port("ffname: %s\n", pp->filename);
    //     pp = get_next_dentry(pp, buf, &root_clus, &root_sec);
    // }
    // while(1);

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
    if (p->length == 0) {
        // printk_port("<cause 1>\n");
        p = get_next_dentry(p, root_buf, &root_clus, &root_sec); 
        return 1;
    }
    // not rw-able
    if (p->attribute != FILE_ATTRIBUTE_GDIR){ 
        // printk_port("<cause 2>\n");
        p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
        return 1;
    }
    /* debug return */
    // uint8 i;
    // for (i = 0; i < ACCEPT_NUM; ++i)
    // {
    //     uint8 num = (strlen(accept_table[i]) > 6)? 6 : strlen(accept_table[i]);
    //     if (!memcmp(p->filename, accept_table[i], num)){
    //         break;
    //     }
    // }
    // if (i == ACCEPT_NUM){
    //     p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
    //     return 1;
    // }

    if (!memcmp(p->filename, "TEST_",5) || !memcmp(p->filename,"TEXT",4) ||
        !memcmp(p->filename, "RUN", 3)){
    // if (memcmp(p->filename, "UMOUNT", 6)){
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
    // static uint cnt = 0;
    // uchar *file = allocproc();

    // fd_num_t fd = fat32_open(AT_FDCWD ,&accept_table[cnt][0], O_RDWR, 0);
    
    // cnt++;

    // length = fat32_read(fd, file, 1000000);

    // printk_port("filename: %s\n", &accept_table[cnt-1][0]);
    // printk_port("length: %d\n", length);

    // fat32_close(fd);

    // _elf_binary = file;

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
            uint64_t edata;
            uint64_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper,&edata);
            pcb_underinit->edata = edata;            
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // prepare stack
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, NULL, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            #ifdef K210
            // free resource
            allocfree();
            p = get_next_dentry(p, root_buf, &root_clus, &root_sec);
            #endif

            // if (cnt == ACCEPT_NUM)
            //     return 0;
            // else{
            //     printk_port("<finish>\n");
            //     return -1;
            // }
            return -1;
        }
    assert(0);
}


/* get directory entry */
/* return readsize if success, -1 fail */
/* make sure buf is enough */
int64 fat32_getdent(fd_num_t fd, char *outbuf, uint32_t len)
{
    uint8 fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -1;

    ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
    isec_t now_sec = first_sec_of_clus(now_clus);
    uchar *buf = kalloc();
    sd_read(buf, now_sec); /* read full buff*/

    dentry_t *p = buf;
    uint32_t readsize = 0;

    // uint32_t basic_size = sizeof(uint64) + sizeof(int64) + sizeof(unsigned short) + sizeof(unsigned char);
    uint32_t basic_size = sizeof(struct linux_dirent64);

    /* for output */
    uchar *filename = kalloc();// :d_name
    /* end for output */

    /* similar to search */
    while (!is_zero_dentry(p) && readsize < len){
        
        /* for output */
        uint8 item_num_copy; // :=d_reclen, d_offset
        ientry_t now_clus_copy; //:=d_ino
        /* end for output */

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        /* now we are at a real dirent */
        now_clus_copy = now_clus; // record first clus, not last

        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){

            uint8 item_num = q->sequence & 0x0f; // entry num
            item_num_copy = item_num + 1;

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
            item_num_copy + 1;
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

        if (readsize + basic_size + strlen(filename) + 1 > len)
            break;

        /* output */
        struct linux_dirent64 *now = (struct linux_dirent64 *)(outbuf + readsize);

        uint32_t oldsize = readsize;
        readsize += basic_size + strlen(filename) + 1;
        readsize += 64 - (readsize % 64);

        now->d_ino = now_clus_copy;
        now->d_off = readsize - oldsize;
        // now->d_reclen = item_num_copy*sizeof(dentry_t);
        now->d_reclen = readsize - oldsize;
        now->d_type = p->attribute & 0x10;
        memcpy(now->d_name, filename, strlen(filename) + 1); 

        p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    kfree(filename);
    kfree(buf);
    return readsize;
}

/* create pipe */
/* success return 0, fail return -1; */
int16 fat32_pipe2(fd_num_t fd[], int32 mode)
{
    int16 fd_index[2] = {-1,-1};
    for (int i = 0; i < NUM_FD; ++i)
        if (current_running->fd[i].used == FD_UNUSED){
            if (fd_index[0] == -1){
                fd[0] = current_running->fd[i].fd_num;
                fd_index[0] = i;
            }
            else if (fd_index[1] == -1){
                fd[1] = current_running->fd[i].fd_num;
                fd_index[1] = i;
                break;
            }
        }

    pipe_num_t pip_num;
    for (pip_num = 0; pip_num < NUM_PIPE; ++pip_num)
    {
        if (pipes[pip_num].r_valid == PIPE_INVALID && pipes[pip_num].w_valid == PIPE_INVALID){
            pipes[pip_num].fd[0] = fd[0];
            pipes[pip_num].fd[1] = fd[1];
            pipes[pip_num].top = 0;
            pipes[pip_num].bottom = 0;
            pipes[pip_num].pid = current_running->pid;
            init_list_head(&pipes[pip_num].wait_list);
            pipes[pip_num].r_valid = PIPE_ERROR; // cannot be used when error
            pipes[pip_num].w_valid = PIPE_ERROR;
            break;
        }
    }

    if (pip_num == NUM_PIPE) return -1;

    for (int i = 0; i < 2; ++i)
    {
        current_running->fd[fd_index[i]].used = FD_USED;
        current_running->fd[fd_index[i]].piped = FD_PIPED;
        current_running->fd[fd_index[i]].pip_num = pip_num;
    }
}

/* write count bytes from buff to file in fd */
/* return fd_num: success
          -1: fail
        */
int16 fat32_open(fd_num_t fd, const uchar *path_const, uint32 flags, uint32 mode)
{
    int fd_index = get_fd_index(fd, current_running);
    if (fd != AT_FDCWD && fd_index < 0) return -1;

    /* for const */
    uchar path[strlen(path_const)+1];
    strcpy(path, path_const);

    uint8 isend = 0;

    uchar *temp1, *temp2; // for path parse
    uint32_t now_clus; // now cluster num
    uchar *buf = kalloc(); // for search

    dentry_t *p = (dentry_t *)buf; // for loop

    if (path[0] == '/'){
        now_clus = fat.bpb.root_clus;
        temp2 = &path[1], temp1 = &path[1];
    }
    else{
        if (fd == AT_FDCWD)
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

        if (isend){
            // success
            search_mode_t search_mode = (!strcmp(temp1, "."))? SEARCH_DIR :
                                        ((O_DIRECTORY & flags) == 0) ? SEARCH_FILE : 
                                        SEARCH_DIR;
            // 1. found
            if ((p = search(temp1, now_clus, buf, search_mode, &ignore, NULL)) != NULL || ignore == 1){
                if (ignore){
                    // use buf to non-null
                    p = buf;
                    set_dentry_cluster(p, now_clus);
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
                    if (!set_fd_from_dentry(current_running, i, p, flags)){                        
                        kfree(buf);
                        return current_running->fd[i].fd_num; // use i as index
                    }
                }
                // no available dx
                kfree(buf);
                return -1;
            }
            // 2.create
            else{
                if (search_mode == SEARCH_DIR || (flags & O_CREATE) == 0){
                    printk_port("no\n");
                    kfree(buf);
                    return -1;
                }// you cant mkdir here! BUT you can create a file
                else{
                    printk_port("yes\n");
                    ientry_t new_clus = _create_new(temp1, now_clus, buf, FILE_FILE);
                    p = buf;
                    set_dentry_cluster(p, new_clus);
                    p->length = 0;
                    p->create_time_ms = 0;
                    p->create_time = 0x53D4; //10:30:40
                    p->create_date = 0x52BB; // 2021/5/27
                    p->last_visited = 0x52BB;
                    p->last_modified_time = 0x53D3;   //23:22
                    p->last_modified_date = 0x52BB;     //25:24

                    for (uint8 i = 0; i < NUM_FD; ++i)
                    {
                        if (!set_fd_from_dentry(current_running, i, p, flags)){ //new file length = 0
                            kfree(buf);
                            return current_running->fd[i].fd_num; // use i as index
                        }
                    }
                    // no available dx
                    kfree(buf);
                    return -1;
                }
            }
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
            return -1;
        }
    }
}

/* show file status */
/* success return 0，fail return -1 */
int16 fat32_fstat(fd_num_t fd, struct kstat *kst)
{
    uint8 fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1)
        return -1;
    /* exists, now read fd info */
    kst->st_dev = current_running->fd[fd_index].dev;
    kst->st_ino = current_running->fd[fd_index].first_clus_num;
    kst->st_mode = current_running->fd[fd_index].flags;
    kst->st_nlink = current_running->fd[fd_index].nlink;
    kst->st_uid = current_running->fd[fd_index].uid;
    kst->st_gid = current_running->fd[fd_index].gid;
    kst->st_rdev = current_running->fd[fd_index].rdev;
    kst->st_size = current_running->fd[fd_index].length;
    kst->st_blksize = fat.byts_per_clus;
    kst->st_blocks = current_running->fd[fd_index].length / fat.byts_per_clus;
    kst->st_atime_sec = current_running->fd[fd_index].atime_sec;
    kst->st_mtime_sec = current_running->fd[fd_index].mtime_sec;
    kst->st_ctime_sec = current_running->fd[fd_index].ctime_sec;
    kst->st_atime_nsec = 0;
    kst->st_mtime_nsec = 0;
    kst->st_ctime_nsec = 0;
    return 0;
}

/* duplicate file descriptor */
/* success return fd_num, fail return -1 */
fd_num_t fat32_dup(fd_num_t fd)
{
    uint8_t fd_index;
    if ((fd_index = get_fd_index(fd, current_running)) == -1) // old doesn't exists, cannot dup
        return -1;
    for (int i = 0; i < NUM_FD; ++i)
        // find an available one, i is index
        if (!current_running->fd[i].used){
            fd_num_t temp = current_running->fd[i].fd_num; // save fd_num
            memcpy(&current_running->fd[i], &current_running->fd[fd_index], sizeof(fd_t));
            current_running->fd[i].fd_num = temp;
            // must be used, because old is used, no need to set
            return temp;
        }
    /* no available */
    return -1;
}

/* duplicate file descriptor with assigned fd */
/* success return fd_num, fail return -1 */
fd_num_t fat32_dup3(fd_num_t old, fd_num_t new, uint8 no_use)
{
    uint8_t fd_index;
    if ((fd_index = get_fd_index(old, current_running)) == -1) // old not exist
        return -1;
    if (get_fd_index(new, current_running) != -1)// new exists
        return -1;

    /* Now new doesn't exists, which means we can use it */
    for (int i = 0; i < NUM_FD; ++i)
        // find an available one, i is index
        if (!current_running->fd[i].used){
            memcpy(&current_running->fd[i], &current_running->fd[fd_index], sizeof(fd_t));
            current_running->fd[i].fd_num = new;
            // must be used, because old is used, no need to set
            return new;
        }
    /* no available */
    return -1;
}

/* seek pos */
/* return pos if success, fail return -1 */
size_t fat32_seek(fd_num_t fd, size_t off)
{
    uint8_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;
    current_running->fd[fd_index].pos = off;
    return off;
}

/* read from pipe */
/* return read count */
int64 pipe_read(uchar *buf, pipe_num_t pip_num, size_t count)
{
    while (pipes[pip_num].r_valid != PIPE_INVALID && pipes[pip_num].top == pipes[pip_num].bottom){
        do_block(&current_running->list, &pipes[pip_num].wait_list);
        do_scheduler();
    }

    uint32_t readsize = min(pipes[pip_num].top - pipes[pip_num].bottom, count); // neq count
    memcpy(buf, pipes[pip_num].buff + pipes[pip_num].bottom, readsize); // maybe exceed?
    pipes[pip_num].bottom += readsize;
    return readsize;
}

/* success : read count */
int64 fat32_read(fd_num_t fd, uchar *buf, size_t count)
{
    uint8_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;

    if (current_running->fd[fd_index].piped == FD_PIPED)
        return pipe_read(buf, current_running->fd[fd_index].pip_num, count);
    // printk_port("fd: %d, length: %d\n", fd_index, current_running->fd[fd_index].length);
    size_t mycount = 0;
    size_t realcount = min(count, current_running->fd[fd_index].length);
    // printk_port("realcount: %d\n", realcount);
    uchar *buff = kalloc();
    ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
    isec_t now_sec = first_sec_of_clus(now_clus);

    while (mycount < realcount){
        size_t readsize = (mycount + BUFSIZE <= realcount) ? BUFSIZE : realcount - mycount;
        sd_read(buff, now_sec);
        memcpy(buf, buff, readsize);
        buf += readsize;
        mycount += readsize;
        if (mycount % CLUSTER_SIZE == 0){
            now_clus = get_next_cluster(now_clus);
            now_sec = first_sec_of_clus(now_clus);
        }
        else
            now_sec += READ_BUF_CNT ; // readsize / BUFSIZE == READ_BUF_CNT until last read
    }

    kfree(buff);

    return realcount;
}

/* write pipe */
/* return write count */
int64 pipe_write(uchar *buf, pipe_num_t pip_num, size_t count)
{
    if (pipes[pip_num].w_valid != PIPE_VALID)
        return -1;
    if (pipes[pip_num].top + count > PIPE_BUF_SIZE)
        return -1;
    memcpy(pipes[pip_num].buff + pipes[pip_num].top, buf, count);
    pipes[pip_num].top += count;
    if (!list_empty(&pipes[pip_num].wait_list))
        do_unblock(pipes[pip_num].wait_list.next);
    return count; // write count
}

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
int64 fat32_write(fd_num_t fd, uchar *buff, uint64_t count)
{
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0) return -1;

    // if (count == 0) return 0;
    if (current_running->fd[fd_index].dev == STDOUT){        
        // for (uint i = 0; i < count; ++i){
        //     sbi_console_putchar(*(buff + i));
        // }
        memcpy(stdout_buf, buff, count);
        stdout_buf[count] = 0;
        printk_port(stdout_buf);
        return count;
    }
    else{
        if (current_running->fd[fd_index].piped == FD_PIPED)
            return pipe_write(buff, current_running->fd[fd_index].pip_num, count);

        size_t mycount = 0;
        size_t realcount = count; // write as many as possible
        uchar *tempbuff = kalloc();
        ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
        ientry_t old_clus = now_clus;
        isec_t now_sec = first_sec_of_clus(now_clus);

        while (mycount < realcount){
            size_t writesize = (mycount + BUFSIZE <= realcount) ? BUFSIZE : realcount - mycount;
            sd_read(tempbuff,now_sec);
            memcpy(tempbuff + current_running->fd[fd_index].pos, buff, writesize);
            sd_write(tempbuff,now_sec);
            buff += writesize;
            mycount += writesize;
            if (mycount % CLUSTER_SIZE == 0){
                old_clus = now_clus;
                now_clus = get_next_cluster(now_clus);
                if (now_clus == 0x0fffffffu){
                    // new clus should be assigned
                    now_clus = search_empty_clus(tempbuff);
                    write_fat_table(old_clus, now_clus, tempbuff);
                }
                now_sec = first_sec_of_clus(now_clus);
            }
            else
                now_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
        }
        current_running->fd[fd_index].pos += realcount;
        current_running->fd[fd_index].length = max(current_running->fd[fd_index].pos, current_running->fd[fd_index].length);

        kfree(tempbuff);

        return realcount;
    }
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

/* create new file */
/* temp1: filename */
/* now_clus : now dir cluster */
ientry_t _create_new(uchar *temp1, ientry_t now_clus, uchar *tempbuf, file_type_t mode)
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
        demand = length / LONG_DENTRY_NAME_LEN + 2; // 1 for /, 1 for short entry
    // find empty entry
    p = search_empty_entry(now_clus, tempbuf, demand, &sec);
    now_clus = clus_of_sec(sec);
    ientry_t new_clus = search_empty_clus(tempbuf);

    if (!p) assert(0);

    // 1. prepare short entry
    dentry_t new_dentry;
    memset(&new_dentry, 0, sizeof(new_dentry));

    // filename:
    if (demand == 1){
        // only short
        for (uint i = 0; i < filename_length; ++i)
            new_dentry.filename[i] = *(temp1+i);
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
    new_dentry.reserved = 0;
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
        {
            sd_write(tempbuf, sec);
            if (clus_of_sec(sec + 1) != clus_of_sec(sec) && get_next_cluster(clus_of_sec(sec)) == 0x0fffffff){
                uint32_t dir_new_clus = search_empty_clus(tempbuf);
                write_fat_table(now_clus, dir_new_clus, tempbuf);
                now_clus = dir_new_clus;
            }
        }
        p = get_next_dentry(p, tempbuf, &now_clus, &sec);
    }
    memcpy(p, &new_dentry, sizeof(new_dentry));

    sd_write(tempbuf, sec);

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
int16 fat32_mkdir(fd_num_t dirfd, const uchar *path_const, uint32_t mode)
{
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
            int16 dirfd_index = get_fd_index(dirfd, current_running);
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
            if (search(temp1, now_clus, tempbuf, SEARCH_DIR, &ignore, NULL)){
                kfree(tempbuf);
                return 0;
            }
            _create_new(temp1, now_clus, tempbuf, FILE_DIR);

            kfree(tempbuf);
            return 0;
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

/* write clus */
/* old pt new */
void write_fat_table(uint32_t old_clus, uint32_t new_clus, uchar *buff)
{
    uint32_t *clusat;
    if (old_clus){
        /* TABLE 1*/
        sd_read(buff, fat_sec_of_clus(old_clus));
        clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(old_clus));
        *clusat = 0xffffffff & new_clus;
        sd_write(buff, fat_sec_of_clus(old_clus));
        /* TABLE 0*/
        sd_read(buff, fat_sec_of_clus(old_clus) - fat.bpb.fat_sz);
        clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(old_clus));
        *clusat = 0xffffffff & new_clus;
        sd_write(buff, fat_sec_of_clus(old_clus) - fat.bpb.fat_sz);
    }
    /* TABLE 1*/
    sd_read(buff, fat_sec_of_clus(new_clus));
    clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(new_clus));
    *clusat = 0x0fffffff;
    sd_write(buff, fat_sec_of_clus(new_clus));
    /* TABLE 0*/
    sd_read(buff, fat_sec_of_clus(new_clus) - fat.bpb.fat_sz);
    clusat = (uint32_t *)((char*)buff + fat_offset_of_clus(new_clus));
    *clusat = 0x0fffffff;
    sd_write(buff, fat_sec_of_clus(new_clus) - fat.bpb.fat_sz);
}

/* return empty clus num */
uint32_t search_empty_clus(uchar *buf)
{
    uint32_t now_sec = fat.first_data_sec - fat.bpb.fat_sz;
    uint32_t now_clus = clus_of_sec(now_sec);
    sd_read(buf, now_sec);
    uchar *temp = buf;
    uint j = 0;
    while (1){
        uint i;
        for (i = 0; i < 4; ++i)
        {
            if (*(temp + i + j))
                break;
        }
        if (i == 4) break;
        else j += 4;
    }
    write_fat_table(0, j/4, buf);
    return j/4;
}

/* search for demand number of CONTINUOUS empty entry for mkdir */
/* make sure demand != 0 */
/* suggest we can find a result, or endless loop */
/* return top and modify sec as top*/
dentry_t *search_empty_entry(uint32_t dir_first_clus, uchar *buf, uint32_t demand, uint32_t *sec)
{
    uint32_t now_clus = dir_first_clus;
    *sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, *sec);
    dentry_t *p = (dentry_t *)buf;
    uint32_t cnt = 0;
    dentry_t *ret_p = p;
    uint32_t ret_sec = *sec;

    for (uint i = 0; i < 255; i++){
        if (!is_zero_dentry(p) && 0xE5 != p->filename[0]){
            cnt = 0;
            p = get_next_dentry(p, buf, &now_clus, sec);
            ret_p = p; ret_sec = *sec;
        }
        else{

            cnt++;
            if (cnt == demand) {*sec = ret_sec; return ret_p;}
            else{
                p = get_next_dentry(p, buf, &now_clus, sec);
            }
        }
    }
    return NULL;
}

/* search if cluster exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return filename if success, NULL if fail */
uchar *search_clus(ientry_t cluster, uint32_t dir_first_clus, uchar *buf)
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
            kfree(filename);
            return filename;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    kfree(filename);

    return NULL;
}

/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return BOTTOM dentry point if success, NULL if fail */
/* ignore if search "." or ".." in root dir */
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
                else filename[name_cnt++] = p->extname[i];
            }
            filename[name_cnt++] = 0;
        }

        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            if (pos){
                pos->sec = now_sec;
            }
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            if (pos){
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
        printk_port("name :%s, filename :%s\n", name, filename);

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

/* get current working dir name */
/* success: return pt to filename*/
/* fail: NULL */
uchar *fat32_getcwd(uchar *buf, size_t size)
{
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
        filename = search_clus(old_clus, now_clus, buff); // filename = myname

        // must found
        assert(filename != NULL);

        strcpy(output, "/");
        // printk_port("output :%s\n", output);
        strcat(output, filename);
        // printk_port("output :%s\n", output);
        strcat(output, temp_filename);
        // printk_port("output :%s\n", output);
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

    strcat(output, temp_filename);

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

int16 fat32_link()
{
    return -1;
}

/* remove file */
/* success return 0, fail return -1 */
int16 fat32_unlink(fd_num_t dirfd, const char* path_t, uint32_t flags)
{
    int fd_index = get_fd_index(dirfd, current_running);
    if (dirfd != AT_FDCWD && fd_index < 0) return -1;

    uchar path[strlen(path_t) + 1];
    strcpy(path, path_t);

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
            printk_port("filename :%s\n", temp1);
            // success
            search_mode_t search_mode = (!strcmp(temp1, "."))? SEARCH_DIR :
                                        ((O_DIRECTORY & flags) == 0) ? SEARCH_FILE : 
                                        SEARCH_DIR;
            // 1. found
            if ((p = search2(temp1, now_clus, buf, search_mode, &ignore, &top)) != NULL){ // can't be ignored, because you cant delete rootdir
                // (1) read in
                isec_t now_sec = top.sec; // not real now_sec, but fisrt sec of this buf
                now_clus = clus_of_sec(now_sec);
                p = buf + top.offset;
                sd_read(buf, now_sec);
                // (2) write
                while (top.len--){
                    p->filename[0] = 0xE5;
                    if (p + 1 == buf + BUFSIZE)
                        sd_write(buf, now_sec);
                    p = get_next_dentry(p, buf, &now_clus, &now_sec); // you can find next buff location
                }
                sd_write(buf, now_sec);
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


/* close fd */
/* success return 0, fail return 1*/
int16 fat32_close(fd_num_t fd)
{
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index == -1) return 1;
    if (current_running->fd[fd_index].used)
    {
        current_running->fd[fd_index].used = FD_UNUSED;
        if (current_running->fd[fd_index].piped == FD_PIPED)
            /* not that fd[0] != fd[1] , and if this fd is piped, its fd_num must equal to fd[0] or fd[1]*/
            if (pipes[current_running->fd[fd_index].pip_num].fd[0] == fd){
                pipes[current_running->fd[fd_index].pip_num].w_valid--; // ERROR to VALID, VALID to INVALID
            }
            else{
                pipes[current_running->fd[fd_index].pip_num].r_valid--;// ERROR to VALID, VALID to INVALID
            }
        return 0;
    }
    else
        return 1;
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

/* set fd */
/* i = fd index, NOT fd num */
/* success 0, fail -1 */
uint8 set_fd_from_dentry(void *pcb_underinit, uint i, dentry_t *p, uint32_t flags)
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
