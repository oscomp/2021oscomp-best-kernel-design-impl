
#include <stdio.h>
#include <os/fs.h>
#include <os/string.h>
#include <assert.h>


superblock_t sb;
char temp[NORMAL_PAGE_SIZE];
unsigned int temp_num;
fd_t fd[NUM_FD];
ientry_t cwd;
int cwd_inum;

int do_fopen(char *filepath, unsigned int priviledge)
{
    // search in already open file
    for (int i = 0; i < NUM_FD; ++i)
        if (fd[i].used){
            ientry_t now;
            get_inode_from_num(fd[i].inode_num, &now);
            if (strcmp(now.filename, filepath) == 0)
                return i;
        }
    // search in cwd
    bread(cwd.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            if (strcmp(crt->filename, filepath) == 0){
                ientry_t now;
                get_inode_from_num(crt->inode_inum, &now);
                if (now.filetype == File){
                    //found
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
    bread(cwd.block[0]);
    head = temp;
    crt = temp + sb.dentry;
    // search for unused dentry
    for (int i = 0; i < *head; ){
        if (crt->used == 0) break;
        else i++;
        crt = (uintptr_t)crt + sb.dentry;
    }
    // write directory dentry
    if (strlen(filepath) > MAX_NAMELEN) assert(0);
    strcpy(crt->filename, filepath);
    int freei = find_free_ientry();
    crt->inode_num = freei;
    if (crt->inode_num == -1) assert(0);
    crt->used = 1;
    *head += 1;
    bwrite(cwd.block[0]);
    // write dir inode
    cwd.size += sb.dentry;
    writei(cwd_inum, &cwd);

    // write new file inode
    ientry_t entry;
    entry.size = 0;
    entry.filetype = File;
    entry.block[0] = find_free_block();
    writei(freei);

    // store in fd
    int fdnum = find_free_fd();
    if (fdnum == -1) assert(0);
    set_fd(&fd[fdnum], freei, priviledge, 0);
    return fdnum;
}

void do_fclose(int fdn)
{

}

void do_fwrite(int fdn, char *buf, size_t size)
{

}

void do_fread(int fdn, char *buf, size_t size)
{

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

    prints("    magic : 0x%lx\n",sb.magic);
    prints("    num sector : %d, start sector : %d\n",sb.data_num,sb.start_sector);
    prints("    inode map : %d (%d)\n",sb.inode_map_start - sb.start_sector,sb.inode_map_num);
    prints("    block map : %d (%d)\n",sb.block_map_start - sb.start_sector,sb.block_map_num);
    prints("    inode : %d (%d)\n",sb.inode_start - sb.start_sector,sb.inode_num);
    prints("    data : %d (%d)\n",sb.data_start - sb.start_sector,sb.data_num);
    prints("    inode entry : %dB, dir entry : %dB\n",sb.ientry,sb.dentry);

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
    bwrite(get_bsec_of(freeb));
    // write inode
    int c = sb.inode_start + freei/(BLOCK_SIZE/sb.ientry);
    if (c != sb.inode_start) assert(0);
    sbi_sd_read(kva2pa(temp), 1, c);
    ientry_t *pt = temp + sb.ientry*(freei % (BLOCK_SIZE/sb.ientry));
    if (pt->filetype != None)
        assert(0);
    pt->size = 3*sb.dentry;
    pt->filetype = Directory;
    pt->block[0] = get_bsec_of(freeb);
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
            if (*crt++) cnt_usedsec++;
        }
    }
    for (i = 0; i < sb.inode_map_num; ++i){
        sbi_sd_read(kva2pa(temp), 1, sb.inode_map_start + i);
        crt = temp;
        for (int j = 0; j < BLOCK_SIZE; ++j){
            if (*crt++) cnt_usedinode++;
        }
    }
    prints("    used sector : %d/%d, start sector : %d\n",cnt_usedsec,sb.data_num,sb.start_sector);
    prints("    inode map : %d (%d), used : %d/%d\n",sb.inode_map_start - sb.start_sector,
        sb.inode_map_num, cnt_usedinode, 128);
    prints("    block map : %d (%d)\n",sb.block_map_start - sb.start_sector,sb.block_map_num);
    prints("    inode : %d (%d)\n",sb.inode_start - sb.start_sector,sb.inode_num);
    prints("    data : %d (%d)\n",sb.data_start - sb.start_sector,sb.data_num);
    prints("    inode entry : %dB, dir entry : %dB\n",sb.ientry,sb.dentry);
}

int do_cd(const char *dirpath)
{
    int isend = 0;
    int now_inum;
    ientry_t now;
    char *temp1, *temp2;
    if (dirpath[0] == '/'){
        bread(sb.inode_start);
        memcpy(&now, temp, sizeof(ientry_t));
        now_inum = 0;
        temp2 = &dirpath[1], temp1 = &dirpath[1];
    }
    else{
        memcpy(&now, &cwd, sizeof(ientry_t));
        now_inum = cwd_inum;
        temp2 = dirpath, temp1 = dirpath;
    }
s1:
    while (*temp2 != '/' && *temp2 != '\0') temp2++;
    if (*temp2 == '\0') isend = 1;
    *temp2 = '\0';

    if (strlen(temp1) == 0){
        memcpy(&cwd, &now, sizeof(ientry_t));
        cwd_inum = now_inum;
    }
    else{
        bread(now.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                if (strcmp(crt->filename, temp1) == 0){
                    now_inum = crt->inode_num;
                    get_inode_from_num(now_inum, &now);
                    if (!isend) ++temp2;
                    temp1 = temp2;
                    goto s1;
                }
            }
            crt = (uintptr_t)crt + sb.dentry;
        }
        return 0;
    }
    if (isend) return 1;
    else return 0;
}

int do_mkdir(char *dirpath)
{
    int isend = 0;
    ientry_t now;
    int now_inum;
    char *temp1, *temp2;

    if (dirpath[0] == '/'){
        bread(sb.inode_start);
        memcpy(&now, temp, sizeof(ientry_t));
        now_inum = 0;
        temp2 = &dirpath[1], temp1 = &dirpath[1];
    }
    else{
        memcpy(&now, &cwd, sizeof(ientry_t));
        now_inum = cwd_inum;
        temp2 = dirpath, temp1 = dirpath;
    }
s2:
    while (*temp2 != '/' && *temp2 != '\0') temp2++;
    if (*temp2 == '\0') isend = 1;
    *temp2 = '\0';

    if (strlen(temp1) == 0)
        return 1;
    else if (!isend){
        bread(now.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                if (strcmp(crt->filename, temp1) == 0){
                    now_inum = crt->inode_num;
                    get_inode_from_num(now_inum, &now);
                    ++temp2;temp1 = temp2;
                    goto s2;
                }
            }
            crt = (uintptr_t)crt + sb.dentry;
        }
        return 0;
    }
    else{
        dheader_t *head = temp;
        // Add dentry in parent directory
        bread(now.block[0]);
        dentry_t *crt = temp + sb.dentry;
        // search for unused dentry
        for (int i = 0; i < *head; ){
            if (crt->used == 0) break;
            else i++;
            crt = (uintptr_t)crt + sb.dentry;
        }
        // write parent directory dentry
        if (strlen(temp1) > MAX_NAMELEN) assert(0);
        strcpy(crt->filename, temp1);
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
        bwrite(get_bsec_of(freeb));
        // write inode
        ientry_t wi;
        wi.size = 3*sb.dentry;
        wi.filetype = Directory;
        wi.block[0] = get_bsec_of(freeb);
        writei(freei, &wi);
    }
    return 1;
}


int do_rmdir(char *dirpath)
{
    int isend = 0;
    ientry_t parent, now;
    int parent_inum, now_inum;
    char *temp1, *temp2;

    if (dirpath[0] == '/'){
        bread(sb.inode_start);
        memcpy(&parent, temp, sizeof(ientry_t));
        parent_inum = 0;
        temp2 = &dirpath[1], temp1 = &dirpath[1];
    }
    else{
        memcpy(&parent, &cwd, sizeof(ientry_t));
        parent_inum = cwd_inum;
        temp2 = dirpath, temp1 = dirpath;
    }
s3:
    while (*temp2 != '/' && *temp2 != '\0') temp2++;
    if (*temp2 == '\0') isend = 1;
    *temp2 = '\0';

    if (strlen(temp1) == 0)
        return 1;
    else if (!isend){
        bread(parent.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                if (strcmp(crt->filename, temp1) == 0){
                    parent_inum = crt->inode_num;
                    get_inode_from_num(parent_inum, &parent);
                    ++temp2;temp1 = temp2;
                    goto s3;
                }
            }
            crt = (uintptr_t)crt + sb.dentry;
        }
        return 0;
    }
    else{
        // Find child
        bread(parent.block[0]);
        dheader_t *head = temp;
        dentry_t *crt = temp + sb.dentry;
        int found = 0;
        for (int i = 0; i < *head; ){
            if (crt->used){
                i++;
                if (strcmp(crt->filename, temp1) == 0){
                    now_inum = crt->inode_num;
                    get_inode_from_num(now_inum, &now);
                    // delete dentry
                    *head -= 1;
                    crt->used = 0;
                    bwrite(parent.block[0]);
                    found = 1; break;
                }
            }
            crt = (uintptr_t)crt + sb.dentry;
        }
        if (!found) return 0;

        // delete block
        delete_block((now.block[0] - sb.data_start) / 8);
        // delete inode
        delete_inode(now_inum);

    }
    return 1;
}

void do_ls()
{
    prints("%s      %s      %s\n","name", "size", "type");
    bread(cwd.block[0]);
    dheader_t *head = temp;
    dentry_t *crt = temp + sb.dentry;
    for (int i = 0; i < *head; ){
        if (crt->used){
            i++;
            ientry_t now;
            get_inode_from_num(crt->inode_num,&now);
            char type[10];
            if (now.filetype == Directory) strcpy(type, "Dir");
            else if (now.filetype == File) strcpy(type, "File");
            prints("%s    %d    %s\n", crt->filename, now.size, type);
        }
        crt = (uintptr_t)crt + sb.dentry;
    }
}
