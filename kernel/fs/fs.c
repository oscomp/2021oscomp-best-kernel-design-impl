
#include <stdio.h>
#include <os/fs.h>
#include <os/string.h>
#include <assert.h>


superblock_t sb;
char temp[NORMAL_PAGE_SIZE];
char temp_2[NORMAL_PAGE_SIZE];
unsigned int temp_num;
fd_t fd[NUM_FD];
ientry_t cwd;
int cwd_inum;
char cwd_path[MAX_PATHLEN];

/* first path linked TO 2nd path */
int do_ln(char *doublepath)
{
    int op;
    if (doublepath[0] == '-' && doublepath[1] == 's'){
        op = LN_SOFT; //soft
        doublepath += 3;
    }
    else
        op = LN_HARD; //hard

    char *temp1 = doublepath;
    while (*temp1 != ' ') temp1++;
    *temp1 = '\0';

    char *firstpath = doublepath, *secondpath = temp1 + 1;
    char firstpath_copy[MAX_NAMELEN], scndpath_copy[MAX_NAMELEN];
    strcpy(firstpath_copy, firstpath); strcpy(scndpath_copy, secondpath);

    char namenow[MAX_NAMELEN];

    strcpy(namenow, get_name_of(secondpath));
    ientry_t now;
    int now_inum, linked_inum;
    if ((now_inum = get_dirinum_from_path(secondpath, &now)) == -1) return 0;
    // search in now
    int isfound = 0;
    bread(now.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                get_inode_from_num(crt->inode_num, &now);
                if (now.filetype == File){
                    // file exists
                    linked_inum = crt->inode_num;
                    isfound = 1;
                    break;             
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    if (!isfound) return 0;

    /* Linked File exists */
    /* Search linking file */
    strcpy(namenow, get_name_of(firstpath));
    if ((now_inum = get_dirinum_from_path(firstpath, &now)) == -1) return 0;
    // search in now
    int tempbid = now.block[0];
    bread(tempbid);
    head = temp;
    crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                ientry_t crti;
                get_inode_from_num(crt->inode_num, &crti);
                if (crti.filetype == File){
                    // file exists, that shouldn't happen
                    get_path_from_inum(now_inum, temp);
                    prints("File already exist: %s\n",strcat(temp,namenow));
                    return 0;                    
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    // CREATE FILE
    bread(tempbid);
    head = temp;
    crt = temp + sb.dentry;
    // search for unused dentry
    for (int i = 0; i < *head; ){
        if (crt->used == 0) break;
        else i++;
        crt = (uintptr_t)crt + sb.dentry;
    }
    // write directory dentry
    if (strlen(namenow) > MAX_NAMELEN) assert(0);
    strcpy(crt->filename, namenow);
    int freei;
    if (op == LN_HARD){
        crt->inode_num = linked_inum;
        freei = linked_inum;
        crt->link = HARD;
    }
    else if (op == LN_SOFT){
        crt->inode_num = find_free_ientry();
        freei = crt->inode_num;
        crt->link = SOFT;
    }
    if (crt->inode_num == -1) assert(0);
    crt->used = 1;    
    *head += 1;
    bwrite(tempbid);
    // write dir inode
    get_inode_from_num(now_inum, &now);
    now.size += sb.dentry;
    writei(now_inum, &now);

    // write new file inode
    if (op == LN_SOFT)
    {
        char last[MAX_PATHLEN];
        if (scndpath_copy[0] == '/')
            strcpy(last, scndpath_copy);
        else{
            get_path_from_inum(cwd_inum, last);
            if (cwd_inum != 0) strcat(last, "/");
            strcat(last, scndpath_copy);
        }
        
        ientry_t entry;
        entry.size = strlen(last);
        entry.filetype = File;
        entry.block[0] = find_free_block();
        writei(freei, &entry);

        bread(entry.block[0]);
        memcpy(temp, last, strlen(last)+1);
        bwrite(entry.block[0]);
    }
    return 1;
}
/* open a new file, and set pos = 0 */
/* open an existed file, and set pos = 0 */
int do_fopen(char *filepath, unsigned int priviledge)
{
    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(filepath));

    ientry_t now;
    int now_inum;
    if ((now_inum = get_dirinum_from_path(filepath, &now)) == -1) return -1;

    // search in now
    int tempbid = now.block[0];
    bread(tempbid);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                get_inode_from_num(crt->inode_num, &now);
                if (now.filetype == File){
                    // file exists
                    // search in already open file (including hard link)
                    for (int i = 0; i < NUM_FD; ++i)
                        if (fd[i].used && fd[i].inode_num == crt->inode_num){
                            fd[i].pos = 0;
                            return i;
                        }
                    // not opened yet
                    if (crt->link == SOFT)
                    {
                        bread(now.block[0]);
                        char softpath[MAX_NAMELEN];
                        strcpy(softpath, temp);
                        return do_fopen(softpath, priviledge);
                    }
                    int fdnum = find_free_fd();
                    if (fdnum == -1) assert(0);
                    set_fd(&fd[fdnum], crt->inode_num, priviledge, 0);
                    return fdnum;
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    // CREATE FILE
    bread(tempbid);
    head = temp;
    crt = temp + sb.dentry;
    // search for unused dentry
    for (int i = 0; i < *head; ){
        if (crt->used == 0) break;
        else i++;
        crt = (uintptr_t)crt + sb.dentry;
    }
    // write directory dentry
    if (strlen(namenow) > MAX_NAMELEN) assert(0);
    strcpy(crt->filename, namenow);
    int freei = find_free_ientry();
    crt->inode_num = freei;
    if (crt->inode_num == -1) assert(0);
    crt->used = 1;
    *head += 1;
    bwrite(tempbid);
    // write dir inode
    get_inode_from_num(now_inum, &now);
    now.size += sb.dentry;
    writei(now_inum, &now);

    // write new file inode
    ientry_t entry;
    memset(&entry, 0, sizeof(ientry_t));
    entry.size = 0;
    entry.filetype = File;
    entry.block[0] = find_free_block();
    writei(freei, &entry);

    // store in fd
    int fdnum = find_free_fd();
    if (fdnum == -1) assert(0);
    set_fd(&fd[fdnum], freei, priviledge, 0);
    return fdnum;
}

void do_fclose(int fdn)
{
    if (fd[fdn].used == 0) assert(0);
    fd[fdn].used = 0;
}

int do_fwrite(int fdn, char *buf, size_t size)
{
    if (fd[fdn].used == 0) assert(0);
    if (fd[fdn].priviledge == O_RDONLY) assert(0);
    ientry_t now;
    get_inode_from_num(fd[fdn].inode_num, &now);
    if (fd[fdn].pos+size > MAX_FILE_SIZE) assert(0);

    int temp_pos = fd[fdn].pos;
    fd[fdn].pos += size;

    int ret = size;

    while (size > 0){
        int sz = ((temp_pos + size - 1)/DATA_SIZE != temp_pos/DATA_SIZE)? 
            DATA_SIZE - (temp_pos%DATA_SIZE) : size;

        int a = get_bsec_from_pos(temp_pos, &now);
        if (!a){
            set_inode_from_pos(temp_pos, &now, fd[fdn].inode_num);
            a = get_bsec_from_pos(temp_pos, &now);
        }
        bread(a);
        memcpy(temp + (temp_pos % DATA_SIZE), buf, sz);
        bwrite(a);
        // printk("write:%d\n\r",a);
        buf += sz;
        size -= sz;
        temp_pos += sz;
    }

    if (fd[fdn].pos > now.size){
        now.size = fd[fdn].pos;
        writei(fd[fdn].inode_num, &now);
    }
    return ret;
}  

int do_fread(int fdn, char *buf, size_t size)
{
    if (fd[fdn].used == 0) assert(0);
    if (fd[fdn].priviledge == O_WRONLY) assert(0);
    ientry_t now;
    get_inode_from_num(fd[fdn].inode_num, &now);

    int ret;
    if (fd[fdn].pos+size > now.size)
        ret = now.size - fd[fdn].pos;
    else ret = size;

    int temp_pos = fd[fdn].pos;
    fd[fdn].pos += ret;
    size = ret;

    while (size > 0){
        int sz = ((temp_pos + size - 1)/DATA_SIZE != temp_pos/DATA_SIZE)? 
            DATA_SIZE - (temp_pos%DATA_SIZE) : size;

        bread(get_bsec_from_pos(temp_pos, &now));
        // printk("read:%d\n\r",get_bsec_from_pos(temp_pos, &now));
        memcpy(buf, temp + (temp_pos % DATA_SIZE), sz);
        buf += sz;
        size -= sz;
        temp_pos += sz;
    }
    return ret;
}

int do_touch(char *filepath)
{
    char nowpath[64];
    strcpy(nowpath, filepath);

    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(filepath));

    ientry_t now;
    int now_inum;
    if ((now_inum = get_dirinum_from_path(filepath, &now)) == -1) return 0;

    // search in now
    bread(now.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                ientry_t tempi;
                get_inode_from_num(crt->inode_num, &tempi);
                if (tempi.filetype == File){
                    prints("File already exists.\n");
                    return 0;
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    int tmp;
    if ((tmp = do_fopen(nowpath, O_RDWR)) == -1)
        return 0;
    else
    {
        do_fclose(tmp);
        return 1;
    }
}

int do_cat(char *filepath)
{
    char nowpath[64];
    strcpy(nowpath, filepath);

    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(filepath));

    ientry_t now;
    int now_inum;
    if ((now_inum = get_dirinum_from_path(filepath, &now)) == -1) return 0;

    // search in now
    bread(now.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                ientry_t tempi;
                get_inode_from_num(crt->inode_num, &tempi);
                if (tempi.filetype == File){
                    int fdn = do_fopen(nowpath, O_RDWR);

                    /* tempi could be soft link inode */
                    /* Need to reload it */
                    get_inode_from_num(fd[fdn].inode_num, &tempi);

                    if (tempi.size > DATA_SIZE){
                        prints("Too big for cat.\n");
                        return 0;
                    }
                    do_fread(fdn, temp, tempi.size);
                    for (int j = 0; j < tempi.size; ++j)
                        prints("%c",temp[j]);
                    return 1;
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    return 0;
}

int do_fseek(int fdn, size_t pos)
{
    if (fd[fdn].used == 0) assert(0);
    if (pos >= MAX_FILE_SIZE) return 0;
    fd[fdn].pos = pos;
    return 1;
}

int do_ftell(int fdn)
{
    if (fd[fdn].used == 0) assert(0);
    return fd[fdn].pos;
}


/* mkfs */

void init_sb()
{
    prints("[FS] Start initializing file system...\n");

    prints("[FS] Setting superblock...\n");
    sb.magic = MAGIC_NUM;
    sb.start_sector = START_FS; sb.num_sector = FS_BLOCKS;
    sb.inode_map_start = sb.start_sector + 1; sb.inode_map_num = 1;
    sb.block_map_start = sb.inode_map_start + sb.inode_map_num + 1;
    sb.block_map_num = 4;
    sb.inode_start = sb.block_map_start + sb.block_map_num + 1;
    sb.inode_num = 16;
    sb.data_start = sb.inode_start + sb.inode_num + 1;
    sb.data_num = FS_BLOCKS;
    sb.ientry = 64;
    sb.dentry = 32;
    sb.level2_num = NORMAL_PAGE_SIZE / sizeof(unsigned int);

    prints("    magic : 0x%lx\n",sb.magic);
    prints("    num sector : %d, start sector : %d\n",sb.data_num,sb.start_sector);
    prints("    inode map : %d (%d)\n",sb.inode_map_start - sb.start_sector,sb.inode_map_num);
    prints("    block map : %d (%d)\n",sb.block_map_start - sb.start_sector,sb.block_map_num);
    prints("    inode : %d (%d)\n",sb.inode_start - sb.start_sector,sb.inode_num);
    prints("    data : %d (%d)\n",sb.data_start - sb.start_sector,sb.data_num);
    prints("    inode entry : %dB, dir entry : %dB\n",sb.ientry,sb.dentry);
    prints("    level2 ptr num: %dB\n", sb.level2_num);

    memcpy(temp, &sb, sizeof(superblock_t));
    sbi_sd_write(kva2pa(temp), 1, START_FS);
}

void init_imap()
{
    prints("[FS] Setting inode map...\n");
    memset(temp, 0, BLOCK_SIZE);
    for (int i = 0; i < sb.inode_map_num; ++i)
        sbi_sd_write(kva2pa(temp), 1, sb.inode_map_start + i);    
}

void init_bmap()
{
    prints("[FS] Setting block map...\n");
    memset(temp, 0, BLOCK_SIZE);
    for (int i = 0; i < sb.block_map_num; ++i)
        sbi_sd_write(kva2pa(temp), 1, sb.block_map_start + i);    
}

void init_inode()
{
    prints("[FS] Setting inode...\n");
    memset(temp, 0, BLOCK_SIZE); // filetype = None
    for (int i = 0; i < sb.inode_num; ++i)
        sbi_sd_write(kva2pa(temp), 1, sb.inode_start + i);    
}

void init_fd()
{
    for (int i = 0; i < NUM_FD; ++i)
        fd[i].used = 0;
}

void init_rootdir()
{
    prints("[FS] Setting rootdir...\n");
    int freei,freeb;
    if ((freei = find_free_ientry()) == -1)
        assert(0);
    if ((freeb = find_free_block()) == -1)
        assert(0);

    memset(temp, 0, NORMAL_PAGE_SIZE);
    char *crt;
    // write dentry
    crt = temp + sb.dentry;
    dentry_t *new_dentry = crt;
    strcpy(new_dentry->filename,".");
    new_dentry->inode_num = freei;
    new_dentry->used = 1;

    new_dentry = crt + sb.dentry;
    strcpy(new_dentry->filename,"..");
    new_dentry->inode_num = freei;
    new_dentry->used = 1;
    // write dheader
    crt -= sb.dentry;
    dheader_t *header = crt;
    *header = 2;
    // save block
    bwrite(freeb);
    // write inode
    int c = sb.inode_start + freei/(BLOCK_SIZE/sb.ientry);
    if (c != sb.inode_start) assert(0);
    sbi_sd_read(kva2pa(temp), 1, c);
    ientry_t *pt = temp + sb.ientry*(freei % (BLOCK_SIZE/sb.ientry));
    if (pt->filetype != None)
        assert(0);
    pt->size = 3*sb.dentry;
    pt->filetype = Directory;
    pt->block[0] = freeb;
    memcpy(&cwd, pt, sizeof(ientry_t)); //cwd = root
    cwd_inum = freei;
    sbi_sd_write(kva2pa(temp), 1, c);
}

void do_mkfs(int op)
{
    readsb(&sb);
    if (sb.magic == MAGIC_NUM && op){
        prints("[FS] File system already exists.\n");
    }
    else{
        init_sb();
        init_imap();
        init_bmap();
        init_inode();
        init_fd();
        init_rootdir();
        prints("[FS] Initializing file system succeeded.\n");
    }
    
    bread(sb.inode_start);
    memcpy(&cwd, temp, sizeof(ientry_t));
    cwd_inum = 0;
}

void do_statfs()
{
    readsb(&sb);
    prints("    magic : 0x%lx\n",sb.magic);
    int cnt_usedsec = 0, cnt_usedinode = 0;
    int i;
    char *crt;

    for (i = 0; i < sb.block_map_num; ++i){
        sbi_sd_read(kva2pa(temp), 1, sb.block_map_start + i);
        crt = temp;
        for (int j = 0; j < BLOCK_SIZE; ++j){
            for (int k = 0; k < 8; ++k)
                if (((1<<k) & *crt) != 0)
                    cnt_usedsec++;
            crt++;
        }
    }
    for (i = 0; i < sb.inode_map_num; ++i){
        sbi_sd_read(kva2pa(temp), 1, sb.inode_map_start + i);
        crt = temp;
        for (int j = 0; j < BLOCK_SIZE; ++j){
            for (int k = 0; k < 8; ++k)
                if (((1<<k) & *crt) != 0)
                    cnt_usedinode++;
            crt++;
        }
    }
    prints("    used sector : %d/%d, start sector : %d\n",cnt_usedsec*8,sb.data_num,sb.start_sector);
    prints("    inode map : %d (%d), used : %d/%d\n",sb.inode_map_start - sb.start_sector,
        sb.inode_map_num, cnt_usedinode, 128);
    prints("    block map : %d (%d)\n",sb.block_map_start - sb.start_sector,sb.block_map_num);
    prints("    inode : %d (%d)\n",sb.inode_start - sb.start_sector,sb.inode_num);
    prints("    data : %d (%d)\n",sb.data_start - sb.start_sector,sb.data_num);
    prints("    inode entry : %dB, dir entry : %dB\n",sb.ientry,sb.dentry);
    prints("    inode entry : %dB, dir entry : %dB\n",sizeof(ientry_t), sizeof(dentry_t));
    prints("    level2 ptr num: %d\n", sb.level2_num);
}

int do_cd(char *dirpath)
{
    if (strcmp(dirpath, "/") == 0){
        bread(sb.inode_start);
        memcpy(&cwd, temp, sizeof(ientry_t));
        cwd_inum = 0;
        get_path_from_inum(cwd_inum, dirpath);
        return 1;
    }

    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(dirpath));
    int now_inum;
    ientry_t now;
    if ((now_inum = get_dirinum_from_path(dirpath, &now)) == -1) return 0;
    else{
        // Find child
        bread(now.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                if (strcmp(crt->filename, namenow) == 0){
                    ientry_t tempi;
                    get_inode_from_num(crt->inode_num, &tempi);
                    if (tempi.filetype == Directory){
                        cwd_inum = crt->inode_num;
                        get_inode_from_num(cwd_inum, &cwd);
                        get_path_from_inum(cwd_inum, dirpath);
                        return 1;
                    }
                }
            }
            crt = (uintptr_t)crt + sb.dentry;
        }
        return 0;
    }
}

int do_mkdir(char *dirpath)
{
    if (strcmp(dirpath, "/") == 0) return 0;
    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(dirpath));

    ientry_t now;
    int now_inum;

    if ((now_inum = get_dirinum_from_path(dirpath, &now)) == -1) return 0;
    else{
        // Add dentry in parent directory
        bread(now.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        // search in dir
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                ientry_t crti;
                get_inode_from_num(crt->inode_num, &crti);
                if (crti.filetype == Directory && strcmp(crt->filename, namenow) == 0){
                    get_path_from_inum(now_inum, temp);
                    prints("Directory already exists: %s\n",strcat(temp, namenow));
                    return 0;
                }
            }
            crt++;
        }
        // search for unused dentry
        for (int i = 0; i < *head; ){
            if (crt->used == 0) break;
            else i++;
            crt = (uintptr_t)crt + sb.dentry;
        }
        // write parent directory dentry
        if (strlen(namenow) > MAX_NAMELEN) assert(0);
        strcpy(crt->filename, namenow);
        int freei = find_free_ientry();
        crt->inode_num = freei;
        if (crt->inode_num == -1) assert(0);
        crt->used = 1;
        *head += 1;
        bwrite(now.block[0]);
        // write parent inode
        now.size += sb.dentry;
        writei(now_inum, &now);

        // Create directory
        // write block
        int freeb = find_free_block();
        dentry_t *entry;

        memset(temp, 0, NORMAL_PAGE_SIZE);
        *head = 2; //. and .. 
        // .
        entry = temp + sb.dentry;        
        strcpy(entry->filename, ".");
        entry->inode_num = freei;
        entry->used = 1;
        // ..
        entry = temp + 2*sb.dentry;
        strcpy(entry->filename, "..");
        entry->inode_num = now_inum;
        entry->used = 1;
        bwrite(freeb);
        // write inode
        ientry_t wi;
        memset(&wi, 0, sizeof(ientry_t));
        wi.size = 3*sb.dentry;
        wi.filetype = Directory;
        wi.block[0] = freeb;
        writei(freei, &wi);
    }
    return 1;
}


int do_rmdir(char *dirpath)
{
    if (strcmp(dirpath, "/") == 0) return 0;
    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(dirpath));
    ientry_t parent, now;
    int parent_inum, now_inum;

    if ((parent_inum = get_dirinum_from_path(dirpath, &parent)) == -1) return 0;
    else{
        // Find child
        bread(parent.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        int found = 0;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                ientry_t crti;
                get_inode_from_num(crt->inode_num, &crti);
                if (crti.filetype == Directory && strcmp(crt->filename, namenow) == 0){
                    now_inum = crt->inode_num;
                    get_inode_from_num(now_inum, &now);
                    // delete dentry
                    *head -= 1;
                    crt->used = 0;
                    bwrite(parent.block[0]);
                    found = 1; break;
                }
            }
            crt++;
        }
        if (!found) return 0;

        // delete block
        delete_block((now.block[0] - sb.data_start) / 8);
        // delete inode
        delete_inode(now_inum);

    }
    return 1;
}

int do_ls(char *dirpath)
{
    char namenow[MAX_NAMELEN];
    strcpy(namenow, get_name_of(dirpath));
    ientry_t dest;
    int dest_inum;

    if (strcmp(dirpath, "/") == 0){
        bread(sb.inode_start);
        memcpy(&dest, temp, sizeof(ientry_t));
        dest_inum = 0;
    }
    else if ((dest_inum = get_dirinum_from_path(dirpath, &dest)) == -1)
        return 0;
    // bread(cwd.block[0]);
    // dheader_t *head = temp;
    // dentry_t *crt = temp + sb.dentry;
    // for (int i = 0; i < *head; ){
    //     if (crt->used){
    //         i++;
    //         ientry_t now;
    //         get_inode_from_num(crt->inode_num,&now);
    //         char type[10];
    //         if (now.filetype == Directory) strcpy(type, "Dir");
    //         else if (now.filetype == File) strcpy(type, "File");
    //         prints("%s    %d    %s\n", crt->filename, now.size, type);
    //     }
    //     crt = (uintptr_t)crt + sb.dentry;
    // }
    //Find dir
    bread(dest.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, namenow) == 0){
                ientry_t now;
                get_inode_from_num(crt->inode_num,&now);
                if (now.filetype == Directory) //Found dest
                {
                    prints("%s      %s      %s\n","name", "size", "type");
                    bread(now.block[0]);
                    dheader_t *head2 = temp;
                    dentry_t *crt2 = temp + sb.dentry;
                    for (int j = 0; j < *head2;){
                        if (crt2->used){
                            j++;
                            ientry_t now2;
                            get_inode_from_num(crt2->inode_num,&now2);
                            char type[10];
                            if (now2.filetype == Directory) strcpy(type, "Dir");
                            else if (now2.filetype == File) strcpy(type, "File");
                            prints("%s    %d    %s\n", crt2->filename, now2.size, type);
                        }
                        crt2++;
                    }
                    return 1;
                }
            }
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
    return 0;
}

void do_sp()
{
    int length;
    char *elf_binary;
    get_elf_file("flightrace",&elf_binary, &length);
    char *t1 = (char*)kmalloc(length);
    char *t2 = (char*)kmalloc(length);

    printk("1:t1:%lx,t2:%lx,elf:%lx\n\r",t1,t2,elf_binary);

    int fd = do_fopen("flightrace_copy",O_RDWR);
    do_fread(fd, t1, length);
    do_fclose(fd);
    // fd = do_fopen("flightrace",O_RDWR);
    // do_fread(fd, t2, length);
    fd = do_fopen("flightrace",O_RDWR);
    do_fread(fd, t2, length);
    do_fclose(fd);
    // get_elf_file("flightrace",&t2, &length);

    printk("2:t1:%lx,t2:%lx,elf:%lx\n\r",t1,t2,elf_binary);
    // fd = do_fopen("1.txt", O_RDWR);
    // do_fwrite(fd,elf_binary,length);
    // do_fclose(fd);
    // fd = do_fopen("1.txt",O_RDWR);
    // do_fread(fd,t1,length);
    // do_fclose(fd);

    char*now1 = t1, *now2 = t2, *now3 = elf_binary;
    printk("now:t1:%lx,t2:%lx,elf:%lx\n\r",t1,t2,elf_binary);

    for (int i = 0; i < length; ++i)
    {
        if (*t2++ != *elf_binary++){
            printk("Diff:real and user %d\n\r",i);
            break;
        }
    }

    t1=now1;t2=now2;elf_binary=now3;
    printk("now:t1:%lx,t2:%lx,elf:%lx\n\r",t1,t2,elf_binary);

    for (int i = 0; i < length; ++i)
    {
        if (*t1++ != *t2++){
            printk("Diff:copy and real %d\n\r",i);
            break;
        }
    }

    t1=now1;t2=now2;elf_binary=now3;
    printk("now:t1:%lx,t2:%lx,elf:%lx\n\r",t1,t2,elf_binary);
    for (int i = 0; i < length; ++i)
    {
        if (*t1++ != *elf_binary++){
            printk("Diff:copy and user %d\n\r",i);
            break;
        }
    }
}