/**
 * Driver for file system.
 * 
 * A image file with file system can be mounted,
 * This module provide the interface to operate
 * the file as if it is a real device.
 * 
 * SD card can be mounted, too.
 * 
 */

#ifndef __DEBUG_driver_fs
#undef DEBUG
#endif

#define __module_name__     "driver_fs"

#include "types.h"
#include "param.h"
#include "fs/fs.h"
#include "fs/fat32.h"
#include "fs/buf.h"
#include "mm/kmalloc.h"
#include "sched/proc.h"
#include "utils/string.h"
#include "utils/debug.h"


extern struct inode_op fat32_inode_op;
extern struct file_op fat32_file_op;

extern struct dentry_op rootfs_dentry_op;


static int imgfs_write(struct superblock *sb, int usr, char *src, uint64 sectorno, uint64 off, uint64 len)
{
	__debug_info("imgfs_write", "sec:%d off:%d len:%d\n", sectorno, off, len);
	
	struct inode *img = sb->dev;
	
	ilock(img);
	int ret = img->fop->write(img, usr, (uint64)src, sb->blocksz * sectorno + off, len);
	iunlock(img);

	return ret;
}


static int imgfs_read(struct superblock *sb, int usr, char *dst, uint64 sectorno, uint64 off, uint64 len)
{
	__debug_info("imgfs_read", "sec:%d off:%d len:%d\n", sectorno, off, len);
	
	struct inode *img = sb->dev;
	
	ilock(img);
	int ret = img->fop->read(img, usr, (uint64)dst, sb->blocksz * sectorno + off, len);
	iunlock(img);

	return ret;
}


static int imgfs_clear(struct superblock *sb, uint64 sectorno, uint64 sectorcnt)
{
	struct inode *img = sb->dev;
	int ret = 0;
	uint64 const blksz = sb->blocksz;

	char *buf = kmalloc(blksz);
	if (buf == NULL) 
		return -1;
	memset(buf, 0, blksz);

	uint64 pos = sectorno * blksz;
	ilock(img);
	for (; sectorcnt--; pos += blksz) {
		if (img->fop->write(img, 0, (uint64)buf, pos, blksz) != blksz) {
			ret = -1;
			break;
		}
	}
	iunlock(img);
	
	kfree(buf);
	return ret;
}


// Caller must hold img->lock.
struct superblock *image_fs_init(struct inode *img)
{
	if (img->size < 512) // Can such a small file contain a file system image?
		return NULL;

	struct superblock *sb = NULL;
	char *sb_buf = NULL;
	struct fat32_sb *fat = NULL;
	struct inode *iroot = NULL;
	
	__debug_info("image_fs_init", "start\n");
	if ((sb = kmalloc(sizeof(struct superblock))) == NULL) {
		return NULL;
	}
	if ((sb_buf = kmalloc(512)) == NULL || 
		img->fop->read(img, 0, (uint64)sb_buf, 0, 512) != 512 ||
		(fat = fat32_init(sb_buf)) == NULL ||
		img->fop->read(img, 0, (uint64)sb_buf, fat->fs_info * 512, 512) != 512 ||
		fat32_info_init(fat, sb_buf) < 0)
	{
		goto fail;
	}
	kfree(sb_buf);
	sb_buf = NULL;

	initsleeplock(&sb->sb_lock, "imgfs_sb");
	initlock(&sb->cache_lock, "imgfs_dcache");
	sb->next = NULL;
	sb->devnum = img->inum;
	sb->real_sb = fat;
	sb->ref = 0;
	sb->blocksz = fat->bpb.byts_per_sec;
	sb->op.alloc_inode = fat_alloc_inode;
	sb->op.destroy_inode = fat_destroy_inode;
	sb->op.read = imgfs_read;
	sb->op.write = imgfs_write;
	sb->op.clear = imgfs_clear;
	sb->op.statfs = fat_stat_fs;

	// Initialize in-mem root.
	iroot = fat32_root_init(sb);
	sb->root = kmalloc(sizeof(struct dentry));
	if (iroot == NULL || sb->root == NULL)
		goto fail;

	iroot->entry = sb->root;
	memset(sb->root, 0, sizeof(struct dentry));
	sb->root->inode = iroot;
	sb->root->op = &rootfs_dentry_op;
	// sb->root->filename[0] = '\0';

	sb->dev = idup(img);

	__debug_info("image_fs_init", "done\n");
	return sb;

fail:
	__debug_warn("image_fs_init", "fail\n");
	if (iroot)
		fat_destroy_inode(iroot);
	if (fat)
		kfree(fat);
	if (sb_buf)
		kfree(sb_buf);
	if (sb)
		kfree(sb);
	return NULL;
}


int diskfs_write(struct superblock *sb, int usr, char *src, uint64 sectorno, uint64 off, uint64 len)
{
	if (off + len > BSIZE)
		panic("diskfs_write");

	// __debug_info("diskfs_write", "sec:%d off:%d len:%d\n", sectorno, off, len);
	struct buf *b = bread(sb->devnum, sectorno);
	int ret = either_copyin(b->data + off, usr, (uint64)src, len);
	
	if (ret < 0) { // fail to write
		b->valid = 0;  // invalidate the buf
	} else {
		ret = len;
		bwrite(b);
	}
	brelse(b);

	return ret;
}


int diskfs_read(struct superblock *sb, int usr, char *dst, uint64 sectorno, uint64 off, uint64 len)
{
	if (off + len > BSIZE)
		panic("diskfs_read");

	// __debug_info("diskfs_read", "sec:%d off:%d len:%d\n", sectorno, off, len);
	struct buf *b = bread(sb->devnum, sectorno);
	int ret = either_copyout(usr, (uint64)dst, b->data + off, len);
	brelse(b);

	return ret < 0 ? -1 : len;
}


int diskfs_clear(struct superblock *sb, uint64 sectorno, uint64 sectorcnt)
{
	struct buf *b;
	while (sectorcnt--) {
		b = bread(sb->devnum, sectorno++);
		memset(b->data, 0, BSIZE);
		bwrite(b);
		brelse(b);
	}
	return 0;
}


struct superblock *disk_fs_init(struct inode *dev)
{
	__debug_info("sdfd_init", "enter\n");

	struct superblock *sb = kmalloc(sizeof(struct superblock));
	if (sb == NULL) {
		return NULL;
	}

	struct fat32_sb *fat;
	struct inode *iroot = NULL;

	// Read superblock from sector 0.
	__debug_info("diskfs_init", "read superblock\n");
	struct buf *b = bread(dev->dev, 0);
	fat = fat32_init((char*)b->data);
	__debug_info("disk_fs_init", "stub 0\n");
	brelse(b);
	if (fat == NULL || BSIZE != fat->bpb.byts_per_sec) {
		__debug_error("fat32_init", "byts_per_sec: %d != BSIZE: %d\n", fat->bpb.byts_per_sec, BSIZE);
		goto fail;
	}
	b = bread(dev->dev, fat->fs_info);
	if (fat32_info_init(fat, (char *)b->data) < 0) {
		__debug_error("fat32_init", "fail to read fs info! fs_info_sec=%d\n", fat->fs_info);
		brelse(b);
		goto fail;
	}
	brelse(b);

	__debug_info("disk_fs_init", "stub 1\n");
	sb->real_sb = fat;
	sb->blocksz = fat->bpb.byts_per_sec;
	sb->op.alloc_inode = fat_alloc_inode;
	sb->op.destroy_inode = fat_destroy_inode;
	sb->op.read = diskfs_read;
	sb->op.write = diskfs_write;
	sb->op.clear = diskfs_clear;
	sb->op.statfs = fat_stat_fs;
	initsleeplock(&sb->sb_lock, "diskfs_sb");
	initlock(&sb->cache_lock, "diskfs_dcache");
	sb->next = NULL;
	sb->devnum = dev->dev;
	sb->ref = 0;
	sb->blocksz = fat->bpb.byts_per_sec;

	__debug_info("disk_fs_init", "stub 2\n");
	// Initialize in-mem root.
	if ((iroot = fat32_root_init(sb)) == NULL || 
		(sb->root = kmalloc(sizeof(struct dentry))) == NULL)
	{
		goto fail;
	}

	__debug_info("disk_fs_init", "stub 3\n");

	iroot->entry = sb->root;

	// Initialize in-mem dentry struct for root.
	memset(sb->root, 0, sizeof(struct dentry));
	sb->root->inode = iroot;
	sb->root->op = &rootfs_dentry_op;
	__debug_info("rootfs_init", "done\n");

	sb->dev = idup(dev);

	return sb;

fail:
	__debug_warn("sd_fs_init", "fail\n");
	if (iroot)
		fat_destroy_inode(iroot);
	if (fat)
		kfree(fat);
	if (sb)
		kfree(sb);
	return NULL;
}


static void fs_clean(struct superblock *sb, struct dentry *de)
{
	sb->op.destroy_inode(de->inode);
	for (struct dentry *child = de->child; child != NULL;) {
		struct dentry *temp = child->next;
		fs_clean(sb, child);
		child = temp;
	}
	kfree(de);
}

void fs_uninstall(struct superblock *sb)
{
	iput(sb->dev);
	fs_clean(sb, sb->root);
	kfree(sb->real_sb);
	kfree(sb);
}
