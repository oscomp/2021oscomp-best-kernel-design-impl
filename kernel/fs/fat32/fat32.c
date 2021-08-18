#ifndef __DEBUG_fat32 
#undef DEBUG 
#endif 

#define __module_name__ "fat32"

#include "param.h"
#include "types.h"
#include "printf.h"
#include "errno.h"
#include "hal/riscv.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"
#include "sched/proc.h"
#include "fs/stat.h"
#include "fs/fs.h"
#include "fat32.h"
#include "mm/kmalloc.h"
#include "utils/string.h"
#include "utils/debug.h"

// FAT32 inode operation collection
struct inode_op fat32_inode_op = {
	.create = fat_alloc_entry,
	.lookup = fat_lookup_dir,
	.truncate = fat_truncate_file,
	.unlink = fat_remove_entry,
	.update = fat_update_entry,
	.getattr = fat_stat_file,
	.setattr = fat_set_file_attr,
	.rename = fat_rename_entry,
};

struct file_op fat32_file_op = {
	.read = fat_read_file,
	.write = fat_write_file,
	.readdir = fat_read_dir,
	.readv = fat_read_file_vec,
	.writev = fat_write_file_vec,
};

/**
 * Initialize FAT32 file system
 * @param		sb		superblock that get from fat32_get_sb()
 * @return				the root inode
 */
struct inode *fat32_init(struct superblock *sb)
{
	int const secsz = 512;
	struct fat32_sb *fat = sb2fat(sb);
	struct inode *iroot = NULL;
	char *buf = kmalloc(secsz);
	if (buf == NULL)
		return NULL;

	__debug_info("fat32_init", "enter\n");
	
	int ret;
	acquiresleep(&sb->sb_lock);
	ret = sb->op.read(sb, 0, buf, 0, 0, secsz);
	releasesleep(&sb->sb_lock);
	if (ret < 0)
		goto end;

	if (strncmp((char const*)(buf + 82), "FAT32", 5)) {
		__debug_error("fat32_init", "not FAT32 volume\n");
		goto end;
	}

	memmove(&fat->bpb.byts_per_sec, buf + 11, 2);            // avoid misaligned load on k210
	fat->bpb.sec_per_clus = *(buf + 13);
	fat->bpb.rsvd_sec_cnt = *(uint16 *)(buf + 14);
	fat->bpb.fat_cnt = *(buf + 16);
	fat->bpb.hidd_sec = *(uint32 *)(buf + 28);
	fat->bpb.tot_sec = *(uint32 *)(buf + 32);
	fat->bpb.fat_sz = *(uint32 *)(buf + 36);
	fat->bpb.root_clus = *(uint32 *)(buf + 44);
	fat->fs_info = *(uint16 *)(buf + 48);
	fat->first_data_sec = fat->bpb.rsvd_sec_cnt + fat->bpb.fat_cnt * fat->bpb.fat_sz;
	fat->data_sec_cnt = fat->bpb.tot_sec - fat->first_data_sec;
	fat->data_clus_cnt = fat->data_sec_cnt / fat->bpb.sec_per_clus;
	fat->byts_per_clus = fat->bpb.sec_per_clus * fat->bpb.byts_per_sec;

	__debug_info("FAT32 init", "byts_per_sec: %d\n", fat->bpb.byts_per_sec);
	__debug_info("FAT32 init", "root_clus: %d\n", fat->bpb.root_clus);
	__debug_info("FAT32 init", "sec_per_clus: %d\n", fat->bpb.sec_per_clus);
	__debug_info("FAT32 init", "fat_cnt: %d\n", fat->bpb.fat_cnt);
	__debug_info("FAT32 init", "fat_sz: %d\n", fat->bpb.fat_sz);
	__debug_info("FAT32 init", "first_data_sec: %d\n", fat->first_data_sec);

	__debug_info("FAT32 init", "done\n");

	// read fs info sector
	acquiresleep(&sb->sb_lock);
	ret = sb->op.read(sb, 0, buf, fat->fs_info, 0, secsz);
	releasesleep(&sb->sb_lock);
	if (ret < 0)
		goto end;

	if (*(uint32*)buf != 0x41615252 || 
		*(uint32*)(buf + 484) != 0x61417272 ||
		*(uint32*)(buf + 508) != 0xaa550000)
	{
		__debug_warn("fat32_init", "wrong magic\n");
		goto end;
	}

	fat->free_count = *(uint32*)(buf + FAT_FREE_CNT_OFF);
	fat->next_free = *(uint32*)(buf + FAT_NEXT_FREE_OFF);
	__debug_info("fat32_init", "free_count: %d\n", fat->free_count);
	__debug_info("fat32_init", "next_free: %d\n", fat->next_free);

	if (fat_cache_init(sb) < 0)
		goto end;

	struct fat32_entry *root = kmalloc(sizeof(struct fat32_entry));
	if (root == NULL)
		goto end;

	memset(root, 0, sizeof(struct fat32_entry));
	root->attribute = (ATTR_DIRECTORY | ATTR_SYSTEM);
	root->first_clus = fat->bpb.root_clus;

	iroot = &root->vfs_inode;
	iroot->mode = S_IFDIR | 0x1ff;
	iroot->state |= I_STATE_VALID;
	iroot->nlink = 1;
	iroot->op = &fat32_inode_op;
	iroot->fop = &fat32_file_op;
	iroot->sb = sb;

	sb->blocksz = fat->bpb.byts_per_sec;

	__debug_info("fat32_root_init", "root cluster: %d\n", root->first_clus);

end:
	kfree(buf);
	return iroot;
}

int fat_stat_fs(struct superblock *sb, struct statfs *stat)
{
	struct fat32_sb *fat = sb2fat(sb);
	
	stat->f_type = 0x4d44;	// fat32 magic
	stat->f_bsize = fat->byts_per_clus;
	stat->f_blocks = fat->bpb.tot_sec / fat->bpb.sec_per_clus;
	stat->f_bfree = fat->free_count;
	stat->f_bavail = fat->free_count;
	stat->f_files = 0;
	stat->f_ffree = 0;
	stat->f_fsid[0] = sb->devnum;
	stat->f_fsid[1] = 0;
	stat->f_namelen = FAT32_MAX_FILENAME;
	stat->f_frsize = 0;
	stat->f_flags = 0;

	return 0;
}

struct superblock *fat32_get_sb(void)
{
	struct fat32_sb *fat = kmalloc(sizeof(struct fat32_sb));
	if (!fat)
		return NULL;
	memset(fat, 0, sizeof(struct fat32_sb));
	return &fat->vfs_sb;
}

void fat32_kill_sb(struct superblock *sb)
{
	struct fat32_sb *fat = sb2fat(sb);
	fat_cache_free(sb);
	kfree(fat);
}

void fat32_sync_sb(struct superblock *sb)
{
	acquiresleep(&sb->sb_lock);
	fat_cache_sync(sb);
	releasesleep(&sb->sb_lock);
}

void __alert_fs_err(const char *func)
{
	printf(__ERROR("%s: alert! something wrong happened!")"\n", func);
	printf(__ERROR("You might need to format your SD!")"\n");
}

/**
 * Read/Write a cluster, caller must hold relative locks
 */
uint fat_rw_clus(struct superblock *sb, uint32 cluster, int write, int user, uint64 data, uint off, uint n)
{
	struct fat32_sb *fat = sb2fat(sb);
	// if (off + n > fat->byts_per_clus)
	// 	panic("fat_rw_clus: offset out of range");

	uint tot, m;
	uint16 const bps = fat->bpb.byts_per_sec;
	uint sec = first_sec_of_clus(fat, cluster) + off / bps;

	// __debug_info("fat_rw_clus", "clus:%d off:%d len:%d\n", cluster, off, n);
	// for (tot = 0; tot < n; tot += m, off += m, data += m, sec++) {
	// 	m = bps - off % bps;
	// 	if (n - tot < m) {
	// 		m = n - tot;
	// 	}
	// 	// __debug_info("fat_rw_clus", "sec:%d m:%d\n", sec, m);
	// 	if (write) {
	// 		if (sb->op.write(sb, user, (char*)data, sec, off % bps, m) < 0) {
	// 			break;
	// 		}
	// 	} else if (sb->op.read(sb, user, (char*)data, sec, off % bps, m) < 0) {
	// 		break;
	// 	}
	// }
	if (write) {
		for (tot = 0; tot < n; tot += m, off += m, data += m, sec++) {
			m = bps - off % bps;
			if (n - tot < m)
				m = n - tot;
			// __debug_info("fat_rw_clus", "sec:%d m:%d\n", sec, m);
			if (sb->op.write(sb, user, (char*)data, sec, off % bps, m) < 0)
				break;
		}
	} else {
		tot = sb->op.read(sb, user, (char*)data, sec, off % bps, n);
	}
	// __debug_info("fat_rw_clus", "done: tot:%d\n", tot);
	return tot;
}

// There is no real inode on disk, just return an in-mem structure.
struct inode *fat_alloc_inode(struct superblock *sb)
{
	struct fat32_entry *ep = kmalloc(sizeof(struct fat32_entry));
	if (ep == NULL) {
		return NULL;
	}
	memset(ep, 0, sizeof(struct fat32_entry));

	struct inode *ip = &ep->vfs_inode;
	ip->sb = sb;
	ip->nlink = 1;
	initsleeplock(&ip->lock, "inode");
	initlock(&ip->ilock, "inode2");

	return ip;
}

void fat_destroy_inode(struct inode *ip)
{
	// if (ip == NULL)
	// 	panic("fat_destroy_inode");

	struct fat32_entry *ep = i2fat(ip);
	free_clus_cache(ep);
	kfree(ep);
}

/**
 * Caller must hold ip->lock.
 * Read from the file that ip corresponds to.
 * @param   ip          the inode
 * @param   user_dst    whether the dst is user space
 * @param   dst         data dst
 * @param   ip          the inode
 * @param   off         the offset from the beginning of the relative file
 * @param   n           number of bytes to read
 * @return              the total bytes that have been read
 */
int fat_read_file(struct inode *ip, int user_dst, uint64 dst, uint off, uint n)
{
	struct fat32_entry *entry = i2fat(ip);

	if (off > entry->file_size || off + n < off || (entry->attribute & ATTR_DIRECTORY)) {
		return 0;
	}
	if (off + n > entry->file_size) {
		n = entry->file_size - off;
	}

	// __debug_info("fat_read_file", "file:%s off:%d len:%d\n", ip->entry->filename, off, n);

	uint tot, m;
	uint32 const bpc = sb2fat(ip->sb)->byts_per_clus;
	uint32 clus;
	for (tot = 0; tot < n; tot += m, off += m, dst += m) {
		if ((clus = reloc_clus(ip, off, 0)) == 0) {
			break;
		}
		m = bpc - off % bpc;
		if (n - tot < m) {
			m = n - tot;
		}
		if (fat_rw_clus(ip->sb, clus, 0, user_dst, dst, off % bpc, m) != m) {
			break;
		}
	}
	// __debug_info("fat_read_file", "done  read:%d\n", tot);
	return tot;
}

int fat_read_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off)
{
	uint tot = 0;
	for (int i = 0; i < count; i++) {
		uint64 addr = (uint64)iovecs[i].iov_base;
		uint n = iovecs[i].iov_len;
		if (!rangeinseg(addr, addr + n))
			return -EFAULT;

		uint ret = fat_read_file(ip, 1, addr, off + tot, n);
		tot += ret;
		if (ret != n) {
			break;
		}
	}
	return tot;
}

/**
 * Caller must hold ip->lock.
 * Write to the file that ip corresponds to.
 * @param   ip          the inode
 * @param   user_src    whether the src is user space
 * @param   src         data src
 * @param   ip          the inode
 * @param   off         the offset from the beginning of the relative file
 * @param   n           number of bytes to write
 * @return              the total bytes that have been written
 *                      or -1 if completely fail
 */
int fat_write_file(struct inode *ip, int user_src, uint64 src, uint off, uint n)
{
	struct fat32_entry *entry = i2fat(ip);

	if (off > entry->file_size || off + n < off || (uint64)off + n > 0x80000
		|| (entry->attribute & ATTR_READ_ONLY)) {
		return -1;
	}
	if (entry->first_clus == 0) {   // so file_size if 0 too, which requests off == 0
		if ((entry->first_clus = alloc_clus(ip->sb)) == 0) {
			return -1;
		}
		// entry->clus_cnt = 0;
		ip->state |= I_STATE_DIRTY;
	}

	uint tot, m;
	struct superblock *sb = ip->sb;
	struct fat32_sb *fat = sb2fat(sb);
	uint32 const bpc = fat->byts_per_clus;
	uint32 const bps = fat->bpb.byts_per_sec;
	uint32 clus;
	for (tot = 0; tot < n; tot += m, off += m, src += m) {
		if ((clus = reloc_clus(ip, off, 1)) == 0) {
			break;
		}
		m = bpc - off % bpc;
		if (n - tot < m) {
			m = n - tot;
		}
		if (off >= ip->size && m < bpc) {
			int nsec = m / bps;
			uint32 sec = first_sec_of_clus(sb, clus) + nsec;
			sb->op.clear(sb, sec, fat->bpb.sec_per_clus - nsec);
		}
		if (fat_rw_clus(sb, clus, 1, user_src, src, off % bpc, m) != m) {
			break;
		}
	}
	if (n > 0 && off > entry->file_size) {
		ip->size = entry->file_size = off;
		ip->state |= I_STATE_DIRTY;
	}
	// __debug_info("fat_write_file", "file:%s off:%d len:%d written:%d\n", ip->entry->filename, off, n, tot);
	return tot;
}

int fat_write_file_vec(struct inode *ip, struct iovec *iovecs, int count, uint off)
{
	uint tot = 0;
	for (int i = 0; i < count; i++) {
		uint64 addr = (uint64)iovecs[i].iov_base;
		uint n = iovecs[i].iov_len;
		if (!rangeinseg(addr, addr + n))
			return -EFAULT;

		int ret = fat_write_file(ip, 1, addr, off + tot, n);
		if (ret < 0) {
			return ret;
		}
		tot += ret;
		if (ret != n) {
			break;
		}
	}
	return tot;
}

/**
 * Truncate the file content.
 * Caller must hold ip->lock.
 */
int fat_truncate_file(struct inode *ip)
{
	struct fat32_entry *entry = i2fat(ip);
	uint32 const bpc = sb2fat(ip->sb)->byts_per_clus;
	uint32 off = 0;
	uint32 clus = reloc_clus(ip, off, 0);

	while (clus != 0) {
		free_clus(ip->sb, clus);
		off += bpc;
		clus = reloc_clus(ip, off, 0);
	}
	free_clus_cache(entry);
	ip->size = entry->file_size = 0;
	entry->first_clus = 0;

	ip->state |= I_STATE_DIRTY;
	return 0;
}

int fat_stat_file(struct inode *ip, struct kstat *st)
{
	struct fat32_entry *ep = i2fat(ip);
	struct fat32_sb *fat = sb2fat(ip->sb);

	// printf("1. c_t_t\t%d\n", ep->create_time_tenth);
	// printf("2. c_t  \t%d\n", ep->create_time);
	// printf("3. c_d  \t%d\n", ep->create_date);
	// printf("4. l_a_d\t%d\n", ep->last_access_date);
	// printf("5. l_w_t\t%d\n", ep->last_write_time);
	// printf("6. l_w_d\t%d\n", ep->last_write_date);

	memset(st, 0, sizeof(struct kstat));
	st->blksize = fat->byts_per_clus;
	st->size = ep->file_size;
	st->blocks = (st->size + st->blksize - 1) / st->blksize;
	st->dev = ip->sb->devnum;
	st->ino = ep->first_clus;
	st->mode = ip->mode;
	st->nlink = ip->nlink;

	return 0;
}

int fat_set_file_attr(struct inode *ip, struct kstat *st)
{
	// struct fat32_entry *ep = i2fat(ip);
	return 0;
}

/**
 * Read a directory from off, and stat the next file. Skip empty entries.
 * @return  bytes that the entries take up, or 0 if no entries 
 */
int fat_read_dir(struct inode *ip, struct dirent *dent, uint off)
{
	if (!(i2fat(ip)->attribute & ATTR_DIRECTORY))
		return -1;

	int count = 0;
	uint off2 = off - off % 32;

	struct fat32_entry entry;
	int ret;
	while ((ret = fat_read_entry(ip, &entry, dent->name, off2, &count)) == 0) {
		// Skip empty entries.
		off2 += count << 5;
	}

	// Meet end of dir
	if (ret < 0) {
		return 0;
	}
	off2 += (count << 5);
	ret = off2 - off;

	// Size of this dent, varies from length of filename.
	int size = sizeof(struct dirent) - sizeof(dent->name) + strlen(dent->name) + 1;
	size += (sizeof(uint64) - (size % sizeof(uint64))) % sizeof(uint64); // Align to 8.

	dent->ino = entry.first_clus;
	dent->off = off2;
	dent->reclen = size;
	dent->type = (entry.attribute & ATTR_DIRECTORY) ? T_DIR : T_FILE;

	return ret;
}

/**
 * Seacher for the entry in a directory and return a structure. Besides, record the offset of
 * some continuous empty slots that can fit the length of filename.
 * 
 * Doesn't handle "." and "..", vfs will do that.
 * Doesn't check cache, vfs also takes care of that.
 * 
 * Caller must hold dir->lock.
 * @param   dir         the directory inode
 * @param   filename    target filename
 * @param   poff        offset of proper empty entry slots from the beginning of the dir
 */
struct fat32_entry *fat_lookup_dir_ent(struct inode *dir, char *filename, uint *poff)
{
	// if (!(i2fat(dir)->attribute & ATTR_DIRECTORY))
	// 	panic("dirlookup not DIR");
	if (dir->state & I_STATE_FREE)
		return NULL;

	// __debug_info("fat_lookup_dir_ent", "in\n");
	struct inode *ip = fat_alloc_inode(dir->sb);
	if (ip == NULL)
		return NULL;
	struct fat32_entry *ep = i2fat(ip);

	int len = strlen(filename);
	int entcnt = (len + CHAR_LONG_NAME - 1) / CHAR_LONG_NAME + 1;   // count of l-n-entries, rounds up. plus s-n-e
	int count = 0;
	int type;
	uint off = 0;
	char namebuf[FAT32_MAX_FILENAME + 1];
	while ((type = fat_read_entry(dir, ep, namebuf, off, &count) != -1)) {
		if (type == 0) {
			if (poff && count >= entcnt) {
				*poff = off;
				poff = NULL;
			}
		} else if (strncmp(filename, namebuf, FAT32_MAX_FILENAME) == 0) {
			if (poff) {
				*poff = off;
			}
			ep->ent_cnt = count;
			return ep;
		}
		off += count << 5;
	}
	if (poff) {
		*poff = off;
	}
	kfree(ep);
	return NULL;
}

/**
 * Call to 'fat_lookup_dir_ent', and stuff the fat32_entry into an inode.
 * 
 * Caller must hold dir->lock.
 */
struct inode *fat_lookup_dir(struct inode *dir, char *filename, uint *poff)
{
	// struct superblock *sb = dir->sb;
	struct inode *ip = NULL;

	uint off = 0;    // Anyhow, we need this on FAT32
	struct fat32_entry *ep = fat_lookup_dir_ent(dir, filename, &off);
	if (ep == NULL)
		goto end;

	ip = &ep->vfs_inode;
	ip->op = dir->op;
	ip->fop = dir->fop;
	ip->size = ep->file_size;
	ip->mode = (ep->attribute & ATTR_DIRECTORY) ? S_IFDIR : S_IFREG;
	ip->mode |= 0x1ff;

	uint64 clus = reloc_clus(dir, off, 0);
	ip->inum = (clus << 32) | (off % sb2fat(dir->sb)->byts_per_clus);
	__debug_info("fat_lookup_dir", "name:%s ipos: clus=%d coff=%d inum=%p sizeof(uint64)=%d\n",
				filename, clus, off, ip->inum, sizeof(uint64));

end:
	if (poff) {
		*poff = off;
	}
	return ip;
}
