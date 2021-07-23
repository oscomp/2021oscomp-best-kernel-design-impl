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


/* FAT32 superblock */
struct fat32_sb {
	uint32		first_data_sec;
	uint32		data_sec_cnt;
	uint32		data_clus_cnt;
	uint32		byts_per_clus;
	uint32		free_count;		// of cluster
	uint32		next_free;		// clus
	uint16		fs_info;
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
};


#include "utils/rbtree.h"

/* Inode of FAT32 in-memory format */
struct fat32_entry {
	uint8	attribute;
	uint8	create_time_tenth;
	uint16	create_time;
	uint16	create_date;
	uint16	last_access_date;
	uint32	first_clus;
	uint16	last_write_time;
	uint16	last_write_date;
	uint32	file_size;

	uint	ent_cnt;
	uint32	cur_clus;
	uint	clus_cnt;
	struct rb_root	rb_clus;
};

// Convert to FAT32 superblock.
static inline struct fat32_sb *sb2fat(struct superblock *sb)
{
	return (struct fat32_sb *)sb->real_sb;
}

// Convert to the so-call inode of FAT32.
static inline struct fat32_entry *i2fat(struct inode *ip)
{
	return (struct fat32_entry *)ip->real_i;
}

/**
 * @param   fat         fat32 super block
 * @param   cluster     cluster number starts from 2, which means no 0 and 1
 */
static inline uint32 first_sec_of_clus(struct fat32_sb *fat, uint32 cluster)
{
	return ((cluster - 2) * fat->bpb.sec_per_clus) + fat->first_data_sec;
}


// cluster.c
uint32				alloc_clus(struct superblock *sb);
int 				free_clus(struct superblock *sb, uint32 cluster);
int					reloc_clus(struct inode *ip, uint off, int alloc);
void				free_clus_cache(struct fat32_entry *entry);
// fat.c
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
void 				__alert_fs_err(const char *func);
uint				fat_rw_clus(struct superblock *sb, uint32 cluster, int write, int user,
									uint64 data, uint off, uint n);
struct inode*		fat_alloc_inode(struct superblock *sb);
void				fat_destroy_inode(struct inode *ip);
struct fat32_entry*	fat_lookup_dir_ent(struct inode *dir, char *filename, uint *poff);


struct fat32_sb*	fat32_init(char *boot_sector);
int					fat32_info_init(struct fat32_sb* fat, char *fsinfo_sector);
struct inode*		fat32_root_init(struct superblock *sb);
struct inode*		fat_lookup_dir(struct inode *dir, char *filename, uint *poff);
// struct inode*		fat_alloc_inode(struct superblock *sb);
int					fat_stat_fs(struct superblock *sb, struct statfs *stat);
// void				fat_destroy_inode(struct inode *ip);
// struct inode*		fat_alloc_entry(struct inode *dir, char *name, int mode);
// int					fat_update_entry(struct inode *ip);
// int					fat_remove_entry(struct inode *ip);
int					fat_truncate_file(struct inode *ip);
int					fat_stat_file(struct inode *ip, struct kstat *st);
int					fat_read_dir(struct inode *dir, struct dirent *dent, uint off);
int					fat_read_file(struct inode *ip, int user_dst, uint64 dst, uint off, uint n);
int					fat_write_file(struct inode *ip, int user_src, uint64 src, uint off, uint n);
// int					fat_rename(struct inode *ip, struct inode *dp, char *name);
int					fat_set_file_attr(struct inode *ip, struct kstat *st);
int					fat_read_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off);
int					fat_write_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off);

#endif