#ifndef _FAT32_INCLUDE_
#define _FAT32_INCLUDE_

#include <type.h>
#include <pgtable.h>
#include <../../drivers/sdcard/include/sdcard.h>
#include <os/sched.h>

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
typedef uint32_t isec_t;

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

typedef enum{
    SEARCH_FILE,
    SEARCH_DIR,
}search_mode_t;

typedef enum{
    FILE_FILE,
    FILE_DIR,
}file_mode_t;

enum{
    FD_UNUSED,
    FD_USED,
};

typedef uint16_t unicode_t;

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

#define O_RDONLY 0x00
#define O_WRONLY 0x01
#define O_RDWR 0x02
#define O_DIRECTORY 0x200000

#define AT_FDCWD 0xffffff9c

#define stdin 0
#define stdout 1

#define MAX_PATHLEN 60

extern fat_t fat;

uint16 fat32_open(uint32 fd, const uchar *path, uint32 flags, uint32 mode);
int8 fat32_read_test(const char *filename);
size_t fat32_read(uint32 fd, uchar *buf, size_t count);
size_t fat32_write(uint32 fd, uchar *buff, uint64_t count);
dentry_t *search(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode);
uint8_t fat32_mkdir(uint32_t dirfd, const uchar *path, uint32_t mode);
uint8 fat32_close(uint32 fd);
uint8 fat32_chdir(const char* path_t);
uchar *fat32_getcwd(uchar *buf, size_t size);

uchar *search_clus(ientry_t cluster, uint32_t dir_first_clus, uchar *buf);
dentry_t *search_empty_entry(uint32_t dir_first_clus, uchar *buf, uint32_t demand, uint32_t *sec);
uint32_t search_empty_fat(uchar *buf);
ientry_t _create_new(uchar *temp1, ientry_t now_clus, uchar *tempbuf, file_mode_t mode);
uint8 set_fd_from_dentry(void *pcb, uint i,dentry_t *p, uint32_t flags);

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

static inline uint32 get_cluster_from_dentry(dentry_t *p)
{
    return ((uint32_t)p->HI_clusnum << 16) + p->LO_clusnum;
}

static inline uint32 get_length_from_dentry(dentry_t *p)
{
    return ((uint32_t)(p->length));
}

static inline uint32 fat_offset_of_clus(uint32 cluster)
{
    return ((cluster) * 4) % fat.bpb.byts_per_sec;
}

static inline uint32_t get_next_cluster(uint32_t cluster)
{
    uchar buf2[SECTOR_SIZE];
    // printk("cluster is %d, fatsec is %d\n",cluster, fat_sec_of_clus(cluster));
    sd_read_sector(buf2, fat_sec_of_clus(cluster), 1);
    // printk("offset: %x\n", fat_offset_of_clus(cluster));
    uint32_t ret = (*(uint32_t*)((char*)buf2 + fat_offset_of_clus(cluster)));
    return ret;
}

static inline uint32_t clus_of_sec(uint32_t sector)
{
    return (sector - fat.first_data_sec) / fat.bpb.sec_per_clus + 2;
}

uint8 is_zero_dentry(dentry_t *p);
uint8_t filenamecmp(const char *name1, const char *name2);

#endif