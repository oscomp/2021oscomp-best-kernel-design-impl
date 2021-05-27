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
uchar buf[NORMAL_PAGE_SIZE];
uchar filebuf[NORMAL_PAGE_SIZE];
ientry_t cwd_first_clus;
ientry_t cwd_clus;
isec_t cwd_sec;
uchar cwd_path[MAX_PATHLEN];

uchar stdout_buf[NORMAL_PAGE_SIZE];
uchar stdin_buf[NORMAL_PAGE_SIZE];

#define ACCEPT_NUM  1
uchar accept_table[14][10] = {{"MKDIR"}, {"OPEN"},  {"CLONE"}, {"GETPID"}, {"UNAME"},  {"FORK"}, {"GETPPID"}, {"GETTIMEOFDAY"}, 
    {"WAIT"}, {"WAITPID"}, {"EXIT"},{"YIELD"}, {"TIMES"}, {"SLEEP"}};

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

uint8_t fat32_init()
{
    /* read FAT32 info */
    uint8_t *b = kalloc();
    // read superblock
    sd_read_sector(b, 0, 4);
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

    /* read root dir and store it for now*/
    cwd_first_clus = fat.bpb.root_clus;
    cwd_clus = fat.bpb.root_clus;
    strcpy(cwd_path, "/");
    cwd_sec = first_sec_of_clus(fat.bpb.root_clus);
    /* from root sector read buffsize */
    sd_read(buf, cwd_sec);
    kfree(b);

    return 0;
}

/* read file from sd card and put it into readyqueue*/
/*  1: continue to read
   -1: busy
    0: end
    */ 
int8 fat32_read_test(const char *filename)
{
    static uint32 cnt = 0;

    /* busy */
    for (int i = 1; i < NUM_MAX_TASK; ++i)
    {
        if (pcb[i].status != TASK_EXITED)
            return -1;
    }

    uchar *_elf_binary;
    uint32_t length;

#ifdef K210

    static dentry_t *p = (dentry_t *)buf;
    // 0x0f: long dentry
    while (p->attribute == 0x0f || 0xE5 == p->filename[0]){
        p = get_next_dentry(p, buf, &cwd_clus, &cwd_sec);
    }
    /* now we are at a real dentry */

    // 0x00: time to end
    uchar *n = (uchar *)p;
    uint8_t index;
    for (index = 0; index < 32; ++index)
    {
        // prints("<n: %d>\n", *n);
        if (*n++ != 0)
            break;
    }
    if (index == 32){
        // printk_port("<return>");
        return 0;
    }
    // no length or deleted
    if (p->length == 0) {
        // printk_port("<cause 1>\n");
        p = get_next_dentry(p, buf, &cwd_clus, &cwd_sec); 
        return 1;
    }
    // not rw-able
    if (p->attribute != FILE_ATTRIBUTE_GDIR){ 
        // printk_port("<cause 2>\n");
        p = get_next_dentry(p, buf, &cwd_clus, &cwd_sec); return 1;
    }
    /* debug return */
    uint8 i;
    for (i = 0; i < ACCEPT_NUM; ++i)
    {
        uint8 num = (strlen(accept_table[i]) > 6)? 6 : strlen(accept_table[i]);
        if (!memcmp(p->filename, accept_table[i], num)){
            break;
        }
    }
    if (i == ACCEPT_NUM){
        p = get_next_dentry(p, buf, &cwd_clus, &cwd_sec);
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
            uint64_t user_stack_page_kva = alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            uint64_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper);
            share_pgtable(pgdir,pa2kva(PGDIR_PA));
            // prepare stack
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, 0, user_stack - NORMAL_PAGE_SIZE, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            #ifdef K210
            // free resource
            allocfree();
            p = get_next_dentry(p, buf, &cwd_clus, &cwd_sec);
            #endif

            return -1;
        }
    assert(0);
}



/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
size_t fat32_write(uint32 fd, uchar *buff, uint64_t count)
{
    if (count == 0) return 0;
    if (fd == stdout){
        memcpy(stdout_buf, buff, count);
        stdout_buf[count] = 0;
        return printk_port(stdout_buf);
    }

}

uchar unicode2char(uint16_t unich)
{
    return (unich >= 65 && unich <= 90)? unich - 65 + 'A' :
        (unich >= 97 && unich <= 122)? unich - 97 + 'a' :
        (unich == 95)? '_' : 
        (unich == 46)? '.':
        0;  
}

unicode_t char2unicode(char ch)
{
    return (ch >= 'A' && ch <= 'Z')? 65 + ch - 'A' :
            (ch >= 'a' && ch <= 'z')? 97 + ch - 'a':
            (ch == '_')? 95 :
            (ch == '.')? 46 :
            0;
}

// void get_file_name_from_long_dentry()
// {

// }

/* create new file */
/* temp1: filename */
/* now_clus : now dir cluster */
ientry_t _create_new(uchar *temp1, ientry_t now_clus, uchar *tempbuf, file_mode_t mode)
{
    printk_port("temp: %s\n", temp1);
    uint32_t length = strlen(temp1);
    uint32_t parent_first_clus = now_clus;
    dentry_t *p;

    /* never move temp1 */

    uint32_t demand, sec;
    if (length <= 8)
        demand = 1;
    else
        demand = length / LONG_DENTRY_NAME_LEN + 2; // 1 for /, 1 for short entry
    // find empty entry
    p = search_empty_entry(now_clus, tempbuf, demand, &sec);
    now_clus = clus_of_sec(sec);
    ientry_t new_clus = search_empty_fat(tempbuf);

    if (!p) assert(0);

    // 1. prepare short entry
    dentry_t new_dentry;
    memset(&new_dentry, 0, sizeof(new_dentry));

    // filename:
    if (demand == 1){
        // only short
        for (uint i = 0; i < length; ++i)
            new_dentry.filename[i] = *(temp1+i);
        for (uint i = length; i < 8; ++i)
            new_dentry.filename[i] = ' ';
    }
    else{
        for (uint i = 0; i < 6; ++i)
            new_dentry.filename[i] = *(temp1+i);
        new_dentry.filename[6] = '~'; new_dentry.filename[7] = '1'; // dont think about 2 or more
    }
    // printk_port("name : %s\n",new_dentry.filename);
    // extname:
    for (uint i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
        new_dentry.extname[i] = ' ';
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
                                            (now_len == length)? 0x00 :
                                            0xff;
            }
            for (uint i = 0; i < LONG_DENTRY_NAME2_LEN; ++i)
            {
                now_len = j*LONG_DENTRY_NAME_LEN + LONG_DENTRY_NAME1_LEN + i;
                long_entries[j].name2[i] = (now_len < length)? char2unicode(*(temp1+now_len)) :
                                            (now_len == length)? 0x00 :
                                            0xff;
            }
            for (uint i = 0; i < LONG_DENTRY_NAME3_LEN; ++i)
            {
                now_len = j*LONG_DENTRY_NAME_LEN + LONG_DENTRY_NAME1_LEN + LONG_DENTRY_NAME2_LEN + i;
                long_entries[j].name3[i] = (now_len < length)? char2unicode(*(temp1+now_len)) :
                                            (now_len == length)? 0x00 :
                                            0xff;
            }
            long_entries[j].sequence = (0x40 | (j + 1)) & 0x4f;   
            long_entries[j].attribute = FILE_ATTRIBUTE_LONG;
            long_entries[j].reserved = 0;
            // checksum
            uint8 sum = 0; uchar *fcb = &new_dentry;
            for (int i = 0; i < 11; ++i)
                sum = (sum & 0x1 ? 0x80 : 0) + (sum >> 1) + *fcb++;
            long_entries[j].checksum = sum;
            long_entries[j].reserved = 0lu;
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
                uint32_t dir_new_clus = search_empty_fat(tempbuf);
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
        p->create_time = 0x53D4; //10:30:20
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
        p->create_time = 0x53D4; //10:30:20
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



/* make a directory */
/* success 0, fail -1 */
uint8_t fat32_mkdir(uint32_t dirfd, const uchar *path_const, uint32_t mode)
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
            if (current_running->fd[dirfd].used)
                now_clus = current_running->fd[dirfd].first_clus_num;
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

        if (isend){
            // already exists
            if (search(temp1, now_clus, tempbuf, SEARCH_DIR)){
                kfree(tempbuf);
                return 0;
            }

            _create_new(*temp1, now_clus, tempbuf, FILE_DIR);

            kfree(tempbuf);
            return 0;
        }
        else{
            // search dir until fail or goto search file
            while (p = search(temp1, now_clus, tempbuf, SEARCH_DIR)){
                now_clus = get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
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
uint32_t search_empty_fat(uchar *buf)
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
            p = get_next_dentry(p, buf, &now_clus, *sec);
            ret_p = p; ret_sec = *sec;
        }
        else{
            cnt++;
            if (cnt == demand) {*sec = ret_sec; return ret_p;}
            else{
                p = get_next_dentry(p, buf, &now_clus, *sec);
            }
        }
    }
    return NULL;
}



/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return dentry point if success, NULL if fail */
dentry_t *search(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode)
{
    // printk_port("p addr: %lx, buf: %lx\n", *pp, buf);
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    // printk_port("5\n");
    dentry_t *p = (dentry_t *)buf;
    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        uchar *filename;
        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){
            uint8 item_num = q->sequence & 0x0f; // entry num
            filename = kmalloc((LONG_DENTRY_NAME1_LEN + LONG_DENTRY_NAME2_LEN + LONG_DENTRY_NAME3_LEN)*item_num); // filename

            /* get filename */
            uint8 isbreak = 0;
            uint16_t unich; // unicode

            while (item_num--){
                uint8 name_cnt = 0;
                for (uint8 i = 0; i < LONG_DENTRY_NAME1_LEN; ++i){
                    // name1
                    unich = q->name1[i];
                    if (unich == 0x0000 || unich == 0xffff){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;           
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME2_LEN; ++i){
                    // name1
                    unich = q->name2[i];
                    if (unich == 0x0000 || unich == 0xffff){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;            
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME3_LEN; ++i){
                    // name1
                    unich = q->name3[i];
                    if (unich == 0x0000 || unich == 0xffff){
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
            filename = kmalloc(SHORT_DENTRY_FILENAME_LEN + SHORT_DENTRY_EXTNAME_LEN + 2); //'.' and addition 0 in the end
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

        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            return p;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    return NULL;
}

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
uint16 fat32_open(uint32 fd, const uchar *path_const, uint32 flags, uint32 mode)
{
    uchar path[strlen(path_const)+1];strcpy(path, path_const);

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
            if (fd < NUM_FD && current_running->fd[fd].used)
                now_clus = current_running->fd[fd].first_clus_num;
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

        if (isend){
            // success
            search_mode_t search_mode = ((O_DIRECTORY & flags) == 0) ? SEARCH_FILE : SEARCH_DIR;
            // 1. found
            if ((p = search(temp1, now_clus, buf, search_mode)) != NULL){

                for (uint8 i = 0; i < NUM_FD; ++i)
                {
                    if (!current_running->fd[i].used){
                        current_running->fd[i].dev = 0;
                        current_running->fd[i].first_clus_num = get_cluster_from_dentry(p);
                        current_running->fd[i].flags = flags;
                        current_running->fd[i].pos = 0;
                        current_running->fd[i].used = 1;
                        kfree(buf);
                        return i;
                    }
                }
                // no available dx
                kfree(buf);
                return -1;
            }
            // 2.create
            else{
                if (search_mode == SEARCH_DIR){
                    kfree(buf);
                    return -1;
                }// you cant mkdir here! BUT you can create a file
                else{
                    ientry_t new_clus = _create_new(temp1, now_clus, buf, FILE_FILE);
                    for (uint8 i = 0; i < NUM_FD; ++i)
                    {
                        if (!current_running->fd[i].used){
                            current_running->fd[i].dev = 0;
                            current_running->fd[i].first_clus_num = new_clus;
                            current_running->fd[i].flags = flags;
                            current_running->fd[i].pos = 0;
                            current_running->fd[i].used = 1;
                            kfree(buf);
                            return i;
                        }
                    }
                    // no available dx
                    kfree(buf);
                    return -1;
                }
            }
        }
        else{
            // search dir until fail or goto search file
            while (p = search(temp1, now_clus, buf, SEARCH_DIR)){
                now_clus = get_cluster_from_dentry(p);
                ++temp2;
                temp1 = temp2;
            }
            kfree(buf);
            return -1;
        }
    }
}

/* close fd */
/* success return 0, fail return 1*/
uint8 fat32_close(uint32 fd)
{
    if (current_running->fd[fd].used)
    {
        current_running->fd[fd].used = FD_UNUSED;
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



/* go to child directory */

