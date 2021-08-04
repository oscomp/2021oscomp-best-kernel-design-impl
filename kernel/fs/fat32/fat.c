#ifndef __DEBUG_fat32 
#undef DEBUG 
#endif 

#define __module_name__ "fat32"

#include "sync/sleeplock.h"
#include "printf.h"
#include "fat32.h"

/**
 * For the given number of a data cluster, return the number of the sector in a FAT table.
 * @param   fat         fat32 super block
 * @param   cluster     number of a data cluster
 * @param   fat_num     number of FAT table from 1, shouldn't be larger than bpb::fat_cnt
 *                      we only use fat region 1 in xv6-k210, lazy to check legality
 */
static inline uint32 fat_sec_of_clus(struct fat32_sb *fat, uint32 cluster, uint8 fat_num)
{
	return fat->bpb.rsvd_sec_cnt + (cluster << 2) / fat->bpb.byts_per_sec + fat->bpb.fat_sz * (fat_num - 1);
}

/**
 * For the given number of a data cluster, return the offest in the corresponding sector in a FAT table.
 * @param   fat         fat32 super block
 * @param   cluster     number of a data cluster
 */
static inline uint32 fat_offset_of_clus(struct fat32_sb *fat, uint32 cluster)
{
	return (cluster << 2) % fat->bpb.byts_per_sec;
}

/**
 * Read the FAT table content corresponded to the given cluster number.
 * @param   sb          super block, we need the functions in it
 * @param   cluster     the number of cluster which you want to read its content in FAT table
 */
uint32 read_fat(struct superblock *sb, uint32 cluster)
{
	if (cluster >= FAT32_EOC) {
		return cluster;
	}
	struct fat32_sb *fat = sb2fat(sb);
	if (cluster > fat->data_clus_cnt + 1) {     // because cluster number starts at 2, not 0
		printf("read_fat: cluster=%d exceed the sum=%d!\n", cluster, fat->data_clus_cnt);
		__alert_fs_err("read_fat");
		return 0;
	}
	uint32 fat_sec = fat_sec_of_clus(fat, cluster, 1);
	uint32 sec_off = fat_offset_of_clus(fat, cluster);
	// here should be a cache layer for FAT table, but not implemented yet.
	uint32 next_clus;
	acquiresleep(&sb->sb_lock);
	sb->op.read(sb, 0, (char *)&next_clus, fat_sec, sec_off, sizeof(next_clus));
	releasesleep(&sb->sb_lock);

	// __debug_info("read_fat", "cur:%d next:%d\n", cluster, next_clus);
	return next_clus;
}

/**
 * Write the FAT region content corresponded to the given cluster number.
 * @param   sb          super block
 * @param   cluster     the number of cluster to write its content in FAT table
 * @param   content     the content which should be the next cluster number of FAT end of chain flag
 */
int write_fat(struct superblock *sb, uint32 cluster, uint32 content)
{
	struct fat32_sb *fat = sb2fat(sb);
	if (cluster > fat->data_clus_cnt + 1) {
		printf("write_fat: cluster=%d exceed the sum=%d!\n", cluster, fat->data_clus_cnt);
		__alert_fs_err("write_fat");
		return -1;
	}
	uint32 fat_sec = fat_sec_of_clus(fat, cluster, 1);
	uint32 sec_off = fat_offset_of_clus(fat, cluster);
	acquiresleep(&sb->sb_lock);
	sb->op.write(sb, 0, (char *)&content, fat_sec, sec_off, sizeof(content));
	releasesleep(&sb->sb_lock);
	return 0;
}
