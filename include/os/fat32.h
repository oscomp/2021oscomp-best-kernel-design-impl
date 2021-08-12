#ifndef _FAT32_INCLUDE_
#define _FAT32_INCLUDE_

#include <type.h>
#include <pgtable.h>
#include <../../drivers/sdcard/include/sdcard.h>
#include <os/sched.h>
#include <os/system.h>
#include <os/ring_buffer.h>
#include <os/pipe.h>

#ifndef max
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef min
#define min(x,y) (((x) > (y)) ? (y) : (x))
#endif

#define BUFSIZE (min(NORMAL_PAGE_SIZE, CLUSTER_SIZE))
#define READ_BUF_CNT (BUFSIZE/SECTOR_SIZE)
#define BUFF_ALIGN(sec) (sec - (sec - first_sec_of_clus(clus_of_sec(sec))) % READ_BUF_CNT)

typedef struct fat{
    uint32_t  first_data_sec;
    uint32_t  data_sec_cnt;
    uint32_t  data_clus_cnt;
    uint32_t  byts_per_clus;

    struct {
        uint16_t  byts_per_sec;
        uint8_t   sec_per_clus;
        uint16_t  rsvd_sec_cnt;
        uint8_t   fat_cnt;            /* count of FAT regions */
        uint32_t  hidd_sec;           /* count of hidden sectors */
        uint32_t  tot_sec;            /* total count of sectors including all regions */
        uint32_t  fat_sz;             /* count of sectors for a FAT region */
        uint32_t  root_clus;
    } bpb;

}fat_t;

/* first cluster numer of cwd */
typedef uint32_t ientry_t;
#define LAST_CLUS_OF_FILE 0x0fffffffu

#define SHORT_DENTRY_FILENAME_LEN 8
#define SHORT_DENTRY_EXTNAME_LEN 3

#pragma pack(1)
/* directory entry */
typedef struct dentry{

    uchar filename[SHORT_DENTRY_FILENAME_LEN];  //7:0
    uchar extname[SHORT_DENTRY_EXTNAME_LEN];   //10:8
    uchar attribute;    //11
    uchar reserved;     //12
    uint8 create_time_ms;   //13
    uint16 create_time;     //15:14
    uint16 create_date;     //17:16
    uint16 last_visited;    //19:18
    uint16 HI_clusnum;      //21:20
    uint16 last_modified_time;   //23:22
    uint16 last_modified_date;      //25:24
    uint16 LO_clusnum;      //27:26
    uint32 length;          //31:28

}dentry_t;
#pragma pack()

/* kstatus */
struct kstat {
    dev_t st_dev;
    ino_t st_ino;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    dev_t st_rdev;
    unsigned long __pad;
    off_t st_size;
    blksize_t st_blksize;
    int __pad2;
    blkcnt_t st_blocks;
    long st_atime_sec;
    long st_atime_nsec;
    long st_mtime_sec;
    long st_mtime_nsec;
    long st_ctime_sec;
    long st_ctime_nsec;
    unsigned __unused[2];
}kstat_t;

#define S_IFDIR 0x4000
#define S_IFREG 0x8000

struct stat {
    dev_t     st_dev;         /* ID of device containing file */
    ino_t     st_ino;         /* Inode number */
    mode_t    st_mode;        /* File type and mode */
    nlink_t   st_nlink;       /* Number of hard links */
    uid_t     st_uid;         /* User ID of owner */
    gid_t     st_gid;         /* Group ID of owner */
    dev_t     st_rdev;        /* Device ID (if special file) */
    off_t     st_size;        /* Total size, in bytes */
    blksize_t st_blksize;     /* Block size for filesystem I/O */
    blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

    /* Since Linux 2.6, the kernel supports nanosecond
      precision for the following timestamp fields.
      For the details before Linux 2.6, see NOTES. */

    struct timespec st_atim;  /* Time of last access */
    struct timespec st_mtim;  /* Time of last modification */
    struct timespec st_ctim;  /* Time of last status change */
};


/* dir entry for linux */
struct linux_dirent64 {
    uint64        d_ino;
    int64         d_off;
    unsigned short  d_reclen;
    unsigned char   d_type;
    char           d_name[];
};

#define FD_UNREDIRECTED 0
#define FD_REDIRECTED 1

#define LONG_DENTRY_NAME1_LEN 5
#define LONG_DENTRY_NAME2_LEN 6
#define LONG_DENTRY_NAME3_LEN 2
#define LONG_DENTRY_NAME_LEN (LONG_DENTRY_NAME1_LEN+LONG_DENTRY_NAME2_LEN+LONG_DENTRY_NAME3_LEN)

#pragma pack(1)
/* long dentry */
typedef struct long_dentry{

    uchar sequence;   //0
    uint16_t name1[LONG_DENTRY_NAME1_LEN];    //10:1
    uchar attribute;  //11
    uchar reserved;   //12
    uchar checksum;    //13
    uint16_t name2[LONG_DENTRY_NAME2_LEN]; //25:14
    uint16_t reserved2; //27:26
    uint16_t name3[LONG_DENTRY_NAME3_LEN]; //31:28 

}long_dentry_t;
#pragma pack()

struct iovec{
    void *iov_base;
    size_t iov_len;
};

typedef enum{
    SEARCH_FILE,
    SEARCH_DIR,
}search_mode_t;

typedef enum{
    FILE_FILE,
    FILE_DIR,
}file_type_t;

enum{
    NO_EXT_NAME,
    EXT_NAME,
};

enum{
    FD_UNUSED,
    FD_USED,
};

#define SECTOR_SIZE (fat.bpb.byts_per_sec)
#define CLUSTER_SIZE (fat.byts_per_clus)
#define READ_PAGE_CNT (NORMAL_PAGE_SIZE/fat.bpb.byts_per_sec)
#define READ_CLUS_CNT (CLUSTER_SIZE/fat.bpb.byts_per_sec)

#define FILE_ATTRIBUTE_RW 0x00
#define FILE_ATTRIBUTE_RO 0x01
#define FILE_ATTRIBUTE_HID 0x02
#define FILE_ATTRIBUTE_SYS 0x04
#define FILE_ATTRIBUTE_VOL 0x08
#define FILE_ATTRIBUTE_CHDIR 0x10
#define FILE_ATTRIBUTE_GDIR 0x20
#define FILE_ATTRIBUTE_LONG 0x0f

#define AT_FDCWD 0xffffff9c

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define DEFAULT_DEV 3

/* file map */
#define MAP_FILE 0
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0X02
#define MAP_FAILED ((void *) -1)

/* fcntl */
#define F_DUPFD 0
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3

#define F_DUPFD_CLOEXEC 1030

#define FD_CLOEXEC 1

/* lseek */
#define SEEK_SET 0x0
#define SEEK_CUR 0x1
#define SEEK_END 0x2

/* getdents64 */
#define DT_DIR 4
#define DT_REG 8

extern fat_t fat;
extern ientry_t cwd_first_clus;
extern ientry_t cwd_clus, root_clus, root_first_clus;
extern isec_t cwd_sec, root_sec;
extern pipe_t pipes[NUM_PIPE];

int8 fat32_read_test(const char *filename);

int16 fat32_open(fd_num_t fd, const uchar *path, uint32 flags, uint32 mode);
int16 fat32_close(fd_num_t fd);

int64 fat32_read(fd_num_t fd, uchar *buf, size_t count);
int64 fat32_readv(fd_num_t fd, struct iovec *iov, int iovcnt);
int64 fat32_write(fd_num_t fd, uchar *buff, uint64_t count);
int64 fat32_writev(fd_num_t fd, struct iovec *iov, int iovcnt);

int16 fat32_mkdir(fd_num_t dirfd, const uchar *path, uint32_t mode);
int16 fat32_chdir(const char* path_t);

uchar *fat32_getcwd(uchar *buf, size_t size);

fd_num_t fat32_dup(fd_num_t fd);
fd_num_t fat32_dup3(fd_num_t old, fd_num_t new, uint8 no_use);

int16 fat32_fstat(fd_num_t fd, struct kstat *kst);

int64 fat32_getdents64(fd_num_t fd, char *buf, uint32_t len);

int16 fat32_pipe2(fd_num_t fd[], int32 mode);

int64_t fat32_lseek(fd_num_t fd, size_t off, uint32_t whence);

int16 fat32_link();
int16 fat32_unlink(fd_num_t dirfd, const char* path, uint32_t flags);

int16 fat32_mount();
int16 fat32_umount();

int64 fat32_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off);
int64 fat32_munmap(void *start, size_t len);

size_t fat32_readlinkat(fd_num_t dirfd, const char *pathname, char *buf, size_t bufsiz);
int32 fat32_fstatat(fd_num_t dirfd, const char *pathname, struct stat *statbuf, int32 flags);
int32_t fat32_faccessat(fd_num_t dirfd, const char *pathname, int mode, int flags);
int32_t fat32_fcntl(fd_num_t fd, int32_t cmd, int32_t arg);

int64 fat32_readmy(uchar *buf, size_t count);
int64_t fat32_lseekmy(size_t off, uint32_t whence);

int32_t do_utimensat(fd_num_t dirfd, const char *pathname, const struct timespec times[2], int32_t flags);
size_t do_sendfile(int out_fd, int in_fd, off_t *offset, size_t count);

void init_inode();
void init_pipe();


dentry_t *search(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos);
dentry_t *search2(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos);
uchar *get_filename_from_clus(ientry_t cluster, uint32_t dir_first_clus, uchar *buf);
dentry_t *search_empty_entry(uint32_t dir_first_clus, uchar *buf, uint32_t demand, uint32_t *sec);
uint32_t search_empty_clus(uchar *buf);

ientry_t _create_new(uchar *temp1, ientry_t now_clus, uchar *tempbuf, dir_pos_t *dir_pos, file_type_t mode);

dentry_t *get_next_dentry(dentry_t *p, uchar *dirbuff, ientry_t *now_clus, isec_t *now_sec);
uint8 set_fd(void *pcb_underinit, uint i, dentry_t *p, dir_pos_t *dir_pos, uint32_t flags);
int16 get_fd_index(fd_num_t fd, void *pcb);
int16 get_my_fd_index(fd_num_t fd, void *pcb);
void write_fat_table(uint32_t old_clus, uint32_t new_clus, uchar *buff);

uchar unicode2char(uint16_t unich);
unicode_t char2unicode(char ch);

void set_dentry_from_fd(dentry_t *p, fd_t *fdp);
uint32_t get_next_cluster(uint32_t cluster);
uint8 is_zero_dentry(dentry_t *p);
uint8_t filenamecmp(const char *name1, const char *name2);
void redirect_fd(fd_t *new, fd_num_t old_fd_index);
void clear_all_valid(fd_t *fdp);

/* get the first sector num of this cluster */
static inline uint32 first_sec_of_clus(uint32 cluster)
{
    return ((cluster - 2) * fat.bpb.sec_per_clus) + fat.first_data_sec;
}

/* get the sector num of this cluster in TABLE 1*/
static inline uint32 fat_sec_of_clus(uint32 cluster)
{
    return fat.bpb.rsvd_sec_cnt + fat.bpb.hidd_sec + \
        (cluster * 4) / fat.bpb.byts_per_sec + fat.bpb.fat_sz;
}

/* get offset of this cluster in fat table */
static inline uint32 fat_offset_of_clus(uint32 cluster)
{
    return ((cluster) * 4) % fat.bpb.byts_per_sec;
}

/* get sector number, this sector is in cluster, and offset */
static inline uint32 get_sec_from_clus_and_offset(uint32_t cluster, uint64_t offset)
{
    return first_sec_of_clus(cluster) + offset / fat.bpb.byts_per_sec;
}

static inline uint32 get_cluster_from_dentry(dentry_t *p)
{
    return ((uint32_t)p->HI_clusnum << 16) + p->LO_clusnum;
}

static inline void set_dentry_cluster(dentry_t *p, uint32 cluster)
{
    p->HI_clusnum = (cluster >> 16) & ((1lu << 16) - 1);
    p->LO_clusnum = cluster & ((1lu << 16) - 1);
}

static inline uint32 get_length_from_dentry(dentry_t *p)
{
    return p->length;
}

static inline uint32_t clus_of_sec(uint32_t sector)
{
    return (sector - fat.first_data_sec) / fat.bpb.sec_per_clus + 2;
}

/* cluster: first cluster number of this file */
/* 返回长度为length的块号 */
static inline uint32_t get_clus_from_len(uint32_t cluster, uint32_t length)
{
    /* find .pos cluster number */
    for (uint64_t i = CLUSTER_SIZE; i <= length; i += CLUSTER_SIZE)
        cluster = get_next_cluster(cluster);
    return cluster;
}

#endif