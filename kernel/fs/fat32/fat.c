#ifndef __DEBUG_fat32 
#undef DEBUG 
#endif 

#define __module_name__ "fat32"

#include "sync/sleeplock.h"
#include "printf.h"
#include "fat32.h"
#include "mm/pm.h"
#include "errno.h"

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

// Init a fat region cache when creating fat32 superblock.
int fat_cache_init(struct superblock *sb)
{
	struct fat32_sb *fat = sb2fat(sb); 
	if (fat->fatcache.page)
		panic("fat_cache_init: superblock is not clean");

	if ((fat->fatcache.page = allocpage()) == NULL)
		return -ENOMEM;
	
	char *afsec = fat->fatcache.page;
	uint32 alloc_fat_sec = (fat->next_free >= 2) ? 
							fat_sec_of_clus(fat, fat->next_free, 1) :
							fat->bpb.rsvd_sec_cnt;
	
	for (int i = 0; i < FAT_CACHE_NSEC; i++) {
		fat->fatcache.fatsec[i] = 0;
		fat->fatcache.lrucnt[i] = 0;
		fat->fatcache.dirty[i] = 0;
	}
	fat->fatcache.allocidx = 0;
	fat->fatcache.fatsec[0] = alloc_fat_sec;
	fat->next_free_fat = fat->bpb.rsvd_sec_cnt;

	acquiresleep(&sb->sb_lock);
	if (sb->op.read(sb, 0, afsec, alloc_fat_sec, 0, SECSZ) != SECSZ) {
		releasesleep(&sb->sb_lock);
		freepage(fat->fatcache.page);
		fat->fatcache.page = NULL;
		return -EIO;
	}

	// Check whether the 'next free' field lie to us.
	if (*(uint32 *)(afsec + fat_offset_of_clus(fat, fat->next_free)) != 0
		&& fat_update_next_free(sb) == 0)
		panic("fat_cache_init: no next free");

	releasesleep(&sb->sb_lock);
	return 0;
}

// Call here before killing superblock.
void fat_cache_free(struct superblock *sb)
{
	struct fat32_sb *fat = sb2fat(sb);
	if (fat->fatcache.page)
		freepage(fat->fatcache.page);
}

// Caller should hold sb->sb_lock. Sync the fat region and something else.
void fat_cache_sync(struct superblock *sb)
{
	struct fat32_sb *fat = sb2fat(sb);

	// sync the fat region
	for (int i = 0; i < FAT_CACHE_NSEC; i++) {
		if (fat->fatcache.dirty[i]) {
			if (sb->op.write(sb, 0, &fat->fatcache.page[i * SECSZ],
							fat->fatcache.fatsec[i], 0, SECSZ) == SECSZ)
			{
				fat->fatcache.dirty[i] = 0;
			}
		}
	}

	// sync the fs info sec
	sb->op.write(sb, 0, (char *)&fat->free_count, fat->fs_info,
				FAT_FREE_CNT_OFF, sizeof(fat->free_count));
	sb->op.write(sb, 0, (char *)&fat->next_free, fat->fs_info,
				FAT_NEXT_FREE_OFF, sizeof(fat->next_free));
}

/**
 * Caller must hold sb->sb_lock.
 * Get the index in the cache page of the given fat sector.
 * If cached hit, return 1, the index returns in *pidx.
 * Otherwise return 0, the index is the least recently used one
 * (or an available one if there is).
 */
static int get_cache_fat(struct superblock *sb, uint32 fatsec, int *pidx)
{
	struct fat32_sb *fat = sb2fat(sb);
	int aidx = fat->fatcache.allocidx;
	int victim = -1;
	uint32 max = 0;

	for (int i = 0; i < FAT_CACHE_NSEC; i++) {
		uint32 sec = fat->fatcache.fatsec[i];
		if (sec == fatsec) {	// cache hit
			*pidx = i;
			pidx = NULL;
			fat->fatcache.lrucnt[i] = 0;
		}
		else if (sec == 0) {	// find an empty slot
			/**
			 * We fill the cache page in an index-increasing way,
			 * so if we meet an empty slot, the next slots must be
			 * empty, too. We can finish our search.
			 * Since no fat region sector will be invalid, once the
			 * cache page becomes full, it will always be full.
			 * There is no case where there is an empty hole inside it.
			 */
			victim = i;
			break;
		}
		else if (i != aidx) {	// We don't evict the alloc sec.
			uint32 cnt = ++fat->fatcache.lrucnt[i];
			if (cnt > max) {
				max = cnt;
				victim = i;
			}
		}
	}
	if (pidx) {					// cache not hit
		*pidx = victim;
		return 0;
	}
	return 1;
}

// Caller must hold sb->sb_lock.
static void fat_cache_load(struct superblock *sb, uint32 newsec, int idx)
{
	struct fat32_sb *fat = sb2fat(sb);
	char *secdata = &fat->fatcache.page[idx * SECSZ];

	if (fat->fatcache.dirty[idx]) {
		sb->op.write(sb, 0, secdata, fat->fatcache.fatsec[idx], 0, SECSZ);
		fat->fatcache.dirty[idx] = 0;
	}

	sb->op.read(sb, 0, secdata, newsec, 0, SECSZ);
	fat->fatcache.fatsec[idx] = newsec;
	fat->fatcache.lrucnt[idx] = 0;
}


#define FAT_ENTRY_NUM	(SECSZ / sizeof(uint32))

// Caller should hold sb->sb_lock.
// Update the next_free field (and fat cache if necessary).
uint32 fat_update_next_free(struct superblock *sb)
{
	struct fat32_sb *fat = sb2fat(sb);
	int aidx = fat->fatcache.allocidx;
	uint32 *asecdata = (uint32 *)&fat->fatcache.page[aidx * SECSZ];

	// It's OK if fat->next_free is an illegal value (when initializing).
	uint32 fatoff = fat_offset_of_clus(fat, fat->next_free) / sizeof(uint32);
	uint32 cnt = FAT_ENTRY_NUM;

	if (asecdata[fatoff] == 0) {
		asecdata[fatoff] = FAT32_EOC + 7;		// mark allocated
		fat->fatcache.dirty[aidx] = 1;
		fat->free_count--;
		cnt--;
		fatoff = (fatoff + 1) % FAT_ENTRY_NUM;
	}

	// Look up the new next_free
	fat->next_free = 0;
	int idx = aidx;
	for (;;) {
		for (; cnt > 0; cnt--, fatoff = (fatoff + 1) % FAT_ENTRY_NUM) {
			if (asecdata[fatoff] == 0) {		// found
				fat->fatcache.allocidx = aidx;
				fat->next_free = fatoff + FAT_ENTRY_NUM * 
									(fat->fatcache.fatsec[aidx] - fat->bpb.rsvd_sec_cnt);
				return fat->next_free;			// in most case we go out from here
			}
		}
		// Unluckily, the current alloc-sec has no cluster.
		do {
			// Switch to next cached fat sector.
			aidx = (aidx + 1) % FAT_CACHE_NSEC;
			// There is no available cluster in cached fat sec.
			if (aidx == idx)
				goto notfound;
		} while (fat->fatcache.fatsec[aidx] == 0);

		asecdata = (uint32 *)&fat->fatcache.page[aidx * SECSZ];
		fatoff = 0;
		cnt = FAT_ENTRY_NUM;
	}

notfound:
	// Unfortunately, all the cached fat sectors have no cluster.
	// We have to do more work -- look up to the disk.
	if (fat->free_count == 0)
		return 0;

	/**
	 * We are going to dig in the disk. But before we start, we
	 * need to do some preliminaries. That is, we must jump over
	 * the sectors that are cached, because we've checked all of
	 * them. More importantly, the identical sectors read from
	 * the disk are not the latest (the cached ones are).
	 */
	uint32 cachesec[FAT_CACHE_NSEC + 1];
	uint32 sec, max = 0;
	cnt = FAT_CACHE_NSEC;
	for (aidx = idx = 0; idx < FAT_CACHE_NSEC; idx++) {
		cachesec[idx] = fat->fatcache.fatsec[idx];
		if (cachesec[idx] == 0) {		// find empty cache slot
			aidx = idx;
			cnt = idx + 1;
			do cachesec[idx++] = 0;
			while (idx < FAT_CACHE_NSEC);
			break;
		}
		sec = fat->fatcache.lrucnt[idx];
		if (sec > max) {				// to determine the eviction victim
			max = sec;
			aidx = idx;
		}
	}

	for (idx = 0; idx < cnt - 1; idx++)	// small array, swap sort is enough
		for (sec = idx + 1; sec < cnt; sec++)
			if (cachesec[idx] > cachesec[sec]) {
				max = cachesec[idx];
				cachesec[idx] = cachesec[sec];
				cachesec[sec] = max;
			}

	asecdata = (uint32 *)&fat->fatcache.page[aidx * SECSZ];
	max = fat->bpb.rsvd_sec_cnt + fat->bpb.fat_sz;	// last sector of fat region
	sec = fat->next_free_fat;
	cachesec[FAT_CACHE_NSEC] = 0;		// place a sentry in tail

try:
	for (idx = 0; cachesec[idx] < sec && idx < cnt; idx++);
	for (; sec < max; sec++) {
		if (sec == cachesec[idx]) {		// meet one in the cache
			idx++;						// since we sorted, move to next value
			continue;
		}
		fat_cache_load(sb, sec, aidx);
		for (fatoff = 0; fatoff < FAT_ENTRY_NUM; fatoff++) {
			if (asecdata[fatoff] == 0) {			// found
				fat->next_free_fat = sec + 1;
				fat->fatcache.allocidx = aidx;
				fat->next_free = fatoff + FAT_ENTRY_NUM * 
									(sec - fat->bpb.rsvd_sec_cnt);
				return fat->next_free;
			}
		}
	}
	if (max != fat->bpb.rsvd_sec_cnt + fat->bpb.fat_sz)		// the second try fail
		panic("fat_update_next_free: no next but free_count is not zero");

	// try again from the first fat region sector.
	max = fat->next_free_fat;
	sec = fat->bpb.rsvd_sec_cnt;
	goto try;
}

/**
 * Read the FAT table content corresponded to the given cluster number.
 * @param   sb          super block, we need the functions in it
 * @param   cluster     the number of cluster which you want to read its content in FAT table
 */
uint32 read_fat(struct superblock *sb, uint32 cluster)
{
	if (cluster >= FAT32_EOC)
		return cluster;

	struct fat32_sb *fat = sb2fat(sb);
	if (cluster > fat->data_clus_cnt + 1) {     // because cluster number starts at 2, not 0
		__alert_fs_err("read_fat");
		return 0;
	}

	uint32 fat_sec = fat_sec_of_clus(fat, cluster, 1);
	uint32 sec_off = fat_offset_of_clus(fat, cluster);
	uint32 next;
	int idx;

	acquiresleep(&sb->sb_lock);

	if (!get_cache_fat(sb, fat_sec, &idx))
		fat_cache_load(sb, fat_sec, idx);

	next = *(uint32 *)(fat->fatcache.page + idx * SECSZ + sec_off);

	releasesleep(&sb->sb_lock);
	return next;

/*
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
*/
}

/**
 * Write the FAT region content corresponded to the given cluster number.
 * @param   sb          super block
 * @param   cluster     the number of cluster to write its content in FAT table
 * @param   content     the content which should be the next cluster number of FAT end of chain flag
 */
int write_fat(struct superblock *sb, uint32 cluster, uint32 content)
{
	if (cluster >= FAT32_EOC)
		return -1;

	struct fat32_sb *fat = sb2fat(sb);
	if (cluster > fat->data_clus_cnt + 1) {     // because cluster number starts at 2, not 0
		__alert_fs_err("write_fat");
		return -1;
	}

	uint32 fat_sec = fat_sec_of_clus(fat, cluster, 1);
	uint32 sec_off = fat_offset_of_clus(fat, cluster);
	int idx;

	acquiresleep(&sb->sb_lock);

	if (!get_cache_fat(sb, fat_sec, &idx))
		fat_cache_load(sb, fat_sec, idx);

	*(uint32 *)(fat->fatcache.page + idx * SECSZ + sec_off) = content;
	fat->fatcache.dirty[idx] = 1;
	if (content == 0)
		fat->free_count++;

	releasesleep(&sb->sb_lock);
	return 0;
/*
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
*/
}
