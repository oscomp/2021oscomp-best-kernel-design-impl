#ifndef __FAT32_H
#define __FAT32_H

#include "fs/stat.h"
#include "fs/fs.h"

/**
 * FAT32 file attribute
 */
#define ATTR_READ_ONLY		0x01
#define ATTR_HIDDEN			0x02
#define ATTR_SYSTEM			0x04
#define ATTR_VOLUME_ID		0x08
#define ATTR_DIRECTORY		0x10
#define ATTR_ARCHIVE		0x20
#define ATTR_LONG_NAME		0x0F

/**
 *  FAT32 flags
 */
#define FAT32_EOC			0x0ffffff8	// last fat chain flag
#define LAST_LONG_ENTRY		0x40		// last long entry flag
#define EMPTY_ENTRY			0xe5
#define END_OF_DIR			0x00		// end of dir

/**
 * FAT32 parameter
 */
#define CHAR_LONG_NAME		13			// filename capacity each entry
#define CHAR_SHORT_NAME		11			// filename capacity each entry
#define FAT32_MAX_FILENAME  255
#define FAT32_MAX_PATH      260


/**
 * Some frequently used parameter
 */
#define FAT_FREE_CNT_OFF	488			// offset in the fs info sector
#define FAT_NEXT_FREE_OFF	492			// the same meaning


/**
 * FAT region cache
 */
#ifndef BSIZE
#define SECSZ				512
#else
#define SECSZ				BSIZE
#endif

#define FAT_CACHE_NSEC		(PGSIZE / SECSZ)

/* FAT32 superblock */
struct fat32_sb {
	uint32		first_data_sec;
	uint32		data_sec_cnt;
	uint32		data_clus_cnt;
	uint32		byts_per_clus;
	uint32		free_count;		// of cluster
	uint32		next_free;		// clus
	uint16		fs_info;		// fs info sector
	uint32		next_free_fat;	// the next fat sec that has room
	struct {
		uint16	byts_per_sec;
		uint8	sec_per_clus;
		uint16	rsvd_sec_cnt;
		uint8	fat_cnt;		// count of FAT regions
		uint32	hidd_sec;		// count of hidden sectors
		uint32	tot_sec;		// total count of sectors including all regions
		uint32	fat_sz;			// count of sectors for a FAT region
		uint32	root_clus;
	} bpb;
	struct {
		char	*page;
		int		allocidx;
		uint32	fatsec[FAT_CACHE_NSEC];	// assume that sector size is 512 byte
		uint32	lrucnt[FAT_CACHE_NSEC];
		int8	dirty[FAT_CACHE_NSEC];	// whether the alloc sec dirty
	} fatcache;
	struct superblock vfs_sb;
};


#include "utils/rbtree.h"

struct clus_table;

/* Inode of FAT32 in-memory format */
struct fat32_entry {
	uint8	attribute;
	uint8	create_time_tenth;
	uint16	create_time;
	uint16	create_date;
	uint16	last_access_date;
	uint16	last_write_time;
	uint16	last_write_date;
	uint32	first_clus;
	uint32	file_size;

	uint32	ent_cnt;
	struct clus_table	*cur_clus;
	struct rb_root		rb_clus;
	struct inode 		vfs_inode;
};

// Convert to FAT32 superblock.
static inline struct fat32_sb *sb2fat(struct superblock *sb)
{
	return container_of(sb, struct fat32_sb, vfs_sb);
}

// Convert to the so-call inode of FAT32.
static inline struct fat32_entry *i2fat(struct inode *ip)
{
	return container_of(ip, struct fat32_entry, vfs_inode);
}

/**
 * @param   fat         fat32 super block
 * @param   cluster     cluster number starts from 2, which means no 0 and 1
 */
static inline uint32 first_sec_of_clus(struct fat32_sb *fat, uint32 cluster)
{
	return ((cluster - 2) * fat->bpb.sec_per_clus) + fat->first_data_sec;
}


extern struct inode_op fat32_inode_op;
extern struct file_op fat32_file_op;

// cluster.c
uint32				alloc_clus(struct superblock *sb);
int 				free_clus(struct superblock *sb, uint32 cluster);
uint32				reloc_clus(struct inode *ip, uint off, int alloc);
void				free_clus_cache(struct fat32_entry *entry);
// fat.c
int					fat_cache_init(struct superblock *sb);
void				fat_cache_free(struct superblock *sb);
void				fat_cache_sync(struct superblock *sb);
uint32				fat_update_next_free(struct superblock *sb);
uint32				read_fat(struct superblock *sb, uint32 cluster);
int					write_fat(struct superblock *sb, uint32 cluster, uint32 content);
// dirent.c
int					fat_make_entry(struct inode *dir, struct fat32_entry *ep,
									char *filename, uint off, int islong);
struct inode*		fat_alloc_entry(struct inode *dir, char *name, int mode);
int					fat_remove_entry(struct inode *ip);
int					fat_update_entry(struct inode *ip);
int					fat_read_entry(struct inode *dir, struct fat32_entry *ep,
									char *namebuf, uint off, int *count);
int					fat_rename_entry(struct inode *ip, struct inode *dp, char *name);
// fat32.c
struct superblock*	fat32_get_sb(void);
void				fat32_kill_sb(struct superblock *sb);
void				fat32_sync_sb(struct superblock *sb);
struct inode*		fat32_init(struct superblock *sb);
int					fat_stat_fs(struct superblock *sb, struct statfs *stat);
uint				fat_rw_clus(struct superblock *sb, uint32 cluster, int write, int user,
									uint64 data, uint off, uint n);
struct inode*		fat_alloc_inode(struct superblock *sb);
void				fat_destroy_inode(struct inode *ip);
int					fat_truncate_file(struct inode *ip);
int					fat_stat_file(struct inode *ip, struct kstat *st);
int					fat_set_file_attr(struct inode *ip, struct kstat *st);
int					fat_read_file(struct inode *ip, int user_dst, uint64 dst, uint off, uint n);
int					fat_write_file(struct inode *ip, int user_src, uint64 src, uint off, uint n);
int					fat_read_dir(struct inode *dir, struct dirent *dent, uint off);
int					fat_read_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off);
int					fat_write_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off);
struct fat32_entry*	fat_lookup_dir_ent(struct inode *dir, char *filename, uint *poff);
struct inode*		fat_lookup_dir(struct inode *dir, char *filename, uint *poff);

void 				__alert_fs_err(const char *func);

#endif