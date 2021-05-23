/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *        Process scheduling related content, such as: scheduler, process blocking,
 *                 process wakeup, process creation, process kill, etc.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_FS_H_
#define INCLUDE_FS_H_


/* TODO: just add these if you needed, other content is from p3 */
#include <type.h>
#include <os/mm.h>
#include <sbi.h>
#include <pgtable.h>
#include <os/string.h>



#define MAGIC_NUM 0x6666666
#define START_FS 8192
#define FS_SIZE (512lu*1024lu*1024lu)
#define FS_BLOCKS (FS_SIZE/BLOCK_SIZE)
#define NUM_LEVEL1_PTR 4
#define NUM_LEVEL2_PTR 2
#define MAX_NAMELEN 20
#define MAX_PATHLEN 128
#define NUM_FD 16
#define DATA_SIZE NORMAL_PAGE_SIZE
#define LEVEL1_SIZE ((NUM_LEVEL1_PTR)*(DATA_SIZE))
#define LEVEL2_SIZE ((NUM_LEVEL2_PTR)*(sb.level2_num)*DATA_SIZE)
#define MAX_FILE_SIZE (LEVEL1_SIZE + LEVEL2_SIZE)
#define SEC_PER_DATA (DATA_SIZE/BLOCK_SIZE)
#define LN_SOFT 0
#define LN_HARD 1

/* Shouldn't be larger than a sector */
// real number
typedef struct superblock{
    unsigned long magic;
    unsigned int start_sector, num_sector;
    unsigned int inode_map_start, inode_map_num;
    unsigned int block_map_start, block_map_num;
    unsigned int inode_start, inode_num;
    unsigned int data_start, data_num;
    unsigned int level2_num;
    size_t ientry, dentry;
}superblock_t;

/* filetype */
typedef enum filetype
{
    None,
    Directory,
    File,
}filetype_t;

/* ientry */
typedef struct ientry{
    unsigned long size;
    filetype_t filetype;
    unsigned int block[NUM_LEVEL1_PTR]; // sector num
    unsigned int block2[NUM_LEVEL2_PTR]; // sector num
}ientry_t;

/* dheader */
typedef unsigned int dheader_t;
/* dentry */
typedef enum linktype{
    NOLINK,
    HARD,
    SOFT,
}linktype_t;

typedef struct dentry{
    char filename[MAX_NAMELEN];
    unsigned int inode_num;
    int used;
    linktype_t link;
}dentry_t;

/* file descriptor */
#define O_RDONLY 1 /* read only open */
#define O_WRONLY 2 /* write only open */
#define O_RDWR 3 /* read/write open */
typedef struct fd{
    unsigned int inode_num;
    unsigned int priviledge;
    unsigned long pos;
    int used;
}fd_t;

extern superblock_t sb;
extern char temp[NORMAL_PAGE_SIZE];
extern char temp_2[NORMAL_PAGE_SIZE];
extern unsigned int temp_num;
extern fd_t fd[NUM_FD];
extern ientry_t cwd;
extern int cwd_inum;
extern char cwd_path[MAX_PATHLEN];

int do_fopen(char *filepath, unsigned int priviledge);
void do_fclose(int fd);
int do_fwrite(int fd, char *buf, size_t size);
int do_fread(int fd, char *buf, size_t size);
void do_mkfs();
void do_statfs();
int do_mkdir(char *filepath);
int do_rmdir(char *filepath);
int do_cd(char *dirpath);
int do_ls(char *dirpath);
int do_touch(char *filepath);
int do_cat(char *filepath);
int do_fseek(int fdn, size_t pos);
int do_ftell(int fdn);
int do_ln(char *doublepath);
void do_sp();

static inline void readsb(superblock_t *sb)
{
    sbi_sd_read(kva2pa(temp), 1, START_FS);
    memcpy(sb, temp, sizeof(superblock_t));
}

static inline void bread(unsigned int block_num)
{
    temp_num = block_num;
    sbi_sd_read(kva2pa(temp), SEC_PER_DATA, block_num);
}

static inline void bwrite(unsigned int block_num)
{
    sbi_sd_write(kva2pa(temp), SEC_PER_DATA, block_num);
}

static inline void icreate(unsigned int inode_num,unsigned int block_num, filetype_t type)
{
    int c = sb.inode_start + inode_num/(BLOCK_SIZE/sb.ientry);
    sbi_sd_read(kva2pa(temp), 1, c);
    ientry_t *crt = temp + sb.ientry*(inode_num % (BLOCK_SIZE/sb.ientry));
    if (crt->filetype != None)
        assert(0);
    crt->filetype = type;
    for (int i = 0; i < NUM_LEVEL1_PTR; ++i)
        if (crt->block[i] == 0)
            crt->block[i] = block_num;
}

/* return inode index */
/* remember to bread first */
static inline int find_free_ientry()
{
    char *crt;
    char tempb[BLOCK_SIZE];
    for (int i = 0; i < sb.inode_map_num; ++i)
    {
        sbi_sd_read(kva2pa(tempb), 1, sb.inode_map_start+i);
        crt = tempb;
        for (int j = 0; j < BLOCK_SIZE; ++j){
            for (int k = 0; k < 8; ++k)
                if ( ( (1 << k) & *crt) == 0){
                    *crt |= (1 << k);
                    sbi_sd_write(kva2pa(tempb), 1, sb.inode_map_start+i);
                    return ((BLOCK_SIZE*8*i+8*j+k) > 128)? -1 : (BLOCK_SIZE*i+8*j+k);
                }
            crt++;
        }
    }
    return -1;
}

static inline unsigned int get_bsec_of(int bnum)
{
    return bnum*(NORMAL_PAGE_SIZE/BLOCK_SIZE) + sb.data_start;
}

/* return block's first sector number */
static inline int find_free_block()
{
    char *crt;
    char tempb[BLOCK_SIZE];
    for (int i = 0; i < sb.block_map_num; ++i)
    {
        sbi_sd_read(kva2pa(tempb), 1, sb.block_map_start+i);
        crt = tempb;
        for (int j = 0; j < BLOCK_SIZE; ++j){
            for (int k = 0; k < 8; ++k)
                if ( ( (1 << k) & *crt) == 0){
                    *crt |= (1 << k);
                    sbi_sd_write(kva2pa(tempb), 1, sb.block_map_start+i);
                    return get_bsec_of(BLOCK_SIZE*8*i+8*j+k);
                }
            crt++;        
        }        
    }
    assert(0);
}

/* return index */
static inline int find_free_fd()
{
    for (int i = 0; i < NUM_FD; ++i)
        if (fd[i].used == 0){
            fd[i].used = 1;
            return i;
        }
    assert(0);
}

static inline unsigned int get_isec_of(int inum)
{
    return inum/(BLOCK_SIZE/sb.ientry) + sb.inode_start;
}

static inline unsigned int get_iofst_of(int inum)
{
    return inum % (BLOCK_SIZE/sb.ientry);
}

static inline void get_inode_from_num(int inum, ientry_t *inode)
{
    char temps[BLOCK_SIZE];
    sbi_sd_read(kva2pa(temps), 1, get_isec_of(inum));
    memcpy(inode, temps + sb.ientry*get_iofst_of(inum), sizeof(ientry_t));
}

static inline void writei(int inum, ientry_t *inode)
{
    char temps[BLOCK_SIZE];
    sbi_sd_read(kva2pa(temp), 1, get_isec_of(inum));
    ientry_t *pt = temp + sb.ientry*get_iofst_of(inum);
    memcpy(pt, inode, sizeof(ientry_t));
    sbi_sd_write(kva2pa(temp), 1, get_isec_of(inum));
}

static inline void delete_inode(int inum)
{
    // clear inode
    char temps[BLOCK_SIZE];
    sbi_sd_read(kva2pa(temp), 1, get_isec_of(inum));
    ientry_t *pt = temp + sb.ientry*get_iofst_of(inum);
    memset(pt, 0, sb.ientry);
    sbi_sd_write(kva2pa(temp), 1, get_isec_of(inum));
    // clear imap
    int i = inum/(BLOCK_SIZE*8);
    sbi_sd_read(kva2pa(temp), 1, sb.inode_map_start + i);
    char *crt = temp + (inum % (BLOCK_SIZE*8)) / 8;
    *crt &= ~(1 << (inum % 8));
    sbi_sd_write(kva2pa(temp), 1, sb.inode_map_start + i);
}

static inline void delete_block(int bnum)
{
    //clear bmap
    int i = bnum/(BLOCK_SIZE*8);
    sbi_sd_read(kva2pa(temp), 1, sb.block_map_start + i);
    char *crt = temp + (bnum % (BLOCK_SIZE*8)) / 8;
    *crt &= ~(1 << (bnum % 8));
    sbi_sd_write(kva2pa(temp), 1, sb.block_map_start + i);
}

static inline void set_fd(fd_t *fdn, int inode_num, int priviledge, int pos)
{
    fdn->inode_num = inode_num;
    fdn->priviledge = priviledge;
    fdn->pos = pos;
    fdn->used = 1;
}

/* copy inode into output */
/* return inode num */
static inline int get_dirinum_from_path(char *path, ientry_t *output)
{
    int isend = 0, isbreak = 0, isfound;
    int now_inum;
    ientry_t now;
    char *temp1, *temp2;
    char temps[BLOCK_SIZE];
    if (path[0] == '/'){
        sbi_sd_read(kva2pa(temps), 1, sb.inode_start);
        memcpy(&now, temps, sizeof(ientry_t));
        now_inum = 0;
        temp2 = &path[1], temp1 = &path[1];
    }
    else{
        memcpy(&now, &cwd, sizeof(ientry_t));
        now_inum = cwd_inum;
        temp2 = path, temp1 = path;
    }

    while (1)
    {
        while (*temp2 != '/' && *temp2 != '\0') temp2++;
        if (*temp2 == '\0') isend = 1;
        *temp2 = '\0';

        if (isend || *(temp2 + 1) == '\0'){
            memcpy(output, &now, sizeof(ientry_t));
            return now_inum;
        }
        else{
            isfound = 0;
            sbi_sd_read(kva2pa(temps), 1, now.block[0]);
            dheader_t *head = temps;
            dentry_t *crt = temps + sb.dentry;
            for (int i = 0; i < *head; ){
                if (crt->used){
                    i++;
                    ientry_t crti;
                    get_inode_from_num(crt->inode_num, &crti);
                    if (crti.filetype == Directory && strcmp(crt->filename, temp1) == 0){
                        now_inum = crt->inode_num;
                        memcpy(&now, &crti, sizeof(ientry_t));
                        if (!isend) ++temp2;
                        temp1 = temp2;
                        isfound = 1;
                        break;
                    }
                }
                crt = (uintptr_t)crt + sb.dentry;
            }
            if (!isfound) return -1;
        }
    }
}

static inline char *get_name_of(char *path)
{
    int i = strlen(path) - 1;
    for ( ; i >= 0; i--){
        if (*(path+i) != '/')
            continue;
        else
            return path + i + 1;
    }
    return path;
}

static inline char *get_bsec_from_pos(int pos, ientry_t *inode)
{
    if (pos < LEVEL1_SIZE)
        return inode->block[pos/DATA_SIZE];
    else{
        pos -= LEVEL1_SIZE;
        if (inode->block2[pos/(sb.level2_num*DATA_SIZE)] == 0)
            return 0;
        sbi_sd_read(temp_2 ,SEC_PER_DATA ,inode->block2[pos/(sb.level2_num*DATA_SIZE)]);
        unsigned int *bid = 
            temp_2 + sizeof(unsigned int)*((pos % (sb.level2_num*DATA_SIZE)) / DATA_SIZE);
        return *bid;
    }   
}

static inline void set_inode_from_pos(int pos, ientry_t *inode, int inum)
{
    if (pos < LEVEL1_SIZE){
        if (inode->block[pos/DATA_SIZE] != 0)
            assert(0);
        inode->block[pos/DATA_SIZE] = find_free_block();
    }
    else{
        pos -= LEVEL1_SIZE;
        int level2_num = pos/(sb.level2_num*DATA_SIZE); //ptr index
        if (inode->block2[level2_num] == 0)
        {
            inode->block2[level2_num] = find_free_block();
            memset(temp, 0, DATA_SIZE);
        }
        else
            bread(inode->block2[level2_num]);

        int level1_num = (pos % (sb.level2_num*DATA_SIZE)) / DATA_SIZE;
        unsigned int *bid = temp + sizeof(unsigned int)*(level1_num);
        if (*bid == 0){            
            *bid = find_free_block();
            bwrite(inode->block2[level2_num]);
        }
        else assert(0);
    }
    writei(inum, inode);
}

/* make sure inum is a directory */
static inline void get_path_from_inum(int inum, char *output)
{
    if (inum == 0){
        strcpy(output, "/");
        return;
    }

    int temp_inum = inum;
    char temp1[MAX_NAMELEN];
    char temp2[MAX_PATHLEN] = {0};

    while(temp_inum){

        ientry_t now;
        get_inode_from_num(temp_inum, &now);

        bread(now.block[0]);
        dentry_t *crt = temp + 2*sb.dentry; //".." is located
        int temp_inum2 = temp_inum;
        temp_inum = crt->inode_num;

        ientry_t now2;
        get_inode_from_num(crt->inode_num, &now2);
        bread(now2.block[0]);

        dheader_t *head = temp;
        crt = temp + sb.dentry;
        for (int i = 0; i < *head;){
            if (crt->used){
                i++;
                if (crt->inode_num == temp_inum2){
                    strcpy(temp1, crt->filename);
                    break;
                }
            }
            crt++;
        }

        char great[MAX_PATHLEN];
        strcpy(great, "/");
        strcat(great, temp1);
        strcat(great, temp2);
        strcpy(temp2, great);                
    }
    strcpy(output, temp2); /* make sure space is enough */
}
#endif