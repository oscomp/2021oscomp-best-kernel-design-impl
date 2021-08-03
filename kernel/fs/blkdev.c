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

#ifndef __DEBUG_fs
#undef DEBUG
#endif

#define __module_name__ "blkdev"

#include "types.h"
#include "param.h"
#include "fs/fs.h"
#include "fat32/fat32.h"
#include "fs/buf.h"
#include "mm/kmalloc.h"
#include "sched/proc.h"
#include "utils/string.h"
#include "utils/debug.h"


struct dentry_op generic_dop = {
	.delete = de_delete,
	.cache = de_check_cache,
};


static int img_write_block(struct superblock *sb, int usr, char *src, uint64 sectorno, uint64 off, uint64 len)
{
	__debug_info("imgfs_write", "sec:%d off:%d len:%d\n", sectorno, off, len);
	
	struct inode *img = sb->dev;
	
	ilock(img);
	int ret = img->fop->write(img, usr, (uint64)src, sb->blocksz * sectorno + off, len);
	iunlock(img);

	return ret;
}


static int img_read_block(struct superblock *sb, int usr, char *dst, uint64 sectorno, uint64 off, uint64 len)
{
	__debug_info("imgfs_read", "sec:%d off:%d len:%d\n", sectorno, off, len);
	
	struct inode *img = sb->dev;
	
	ilock(img);
	int ret = img->fop->read(img, usr, (uint64)dst, sb->blocksz * sectorno + off, len);
	iunlock(img);

	return ret;
}


static int img_clear_block(struct superblock *sb, uint64 sectorno, uint64 sectorcnt)
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


// // Caller must hold img->lock.
// struct superblock *image_fs_init(struct inode *img)
// {
// 	if (img->size < 512) // Can such a small file contain a file system image?
// 		return NULL;

// 	struct superblock *sb = NULL;
// 	char *sb_buf = NULL;
// 	struct fat32_sb *fat = NULL;
// 	struct inode *iroot = NULL;
	
// 	__debug_info("image_fs_init", "start\n");
// 	if ((sb = kmalloc(sizeof(struct superblock))) == NULL) {
// 		return NULL;
// 	}
// 	if ((sb_buf = kmalloc(512)) == NULL || 
// 		img->fop->read(img, 0, (uint64)sb_buf, 0, 512) != 512 ||
// 		(fat = fat32_init(sb_buf)) == NULL ||
// 		img->fop->read(img, 0, (uint64)sb_buf, fat->fs_info * 512, 512) != 512 ||
// 		fat32_info_init(fat, sb_buf) < 0)
// 	{
// 		goto fail;
// 	}
// 	kfree(sb_buf);
// 	sb_buf = NULL;

// 	initsleeplock(&sb->sb_lock, "imgfs_sb");
// 	initlock(&sb->cache_lock, "imgfs_dcache");
// 	sb->next = NULL;
// 	sb->devnum = img->inum;
// 	sb->real_sb = fat;
// 	sb->ref = 0;
// 	sb->blocksz = fat->bpb.byts_per_sec;
// 	sb->op.alloc_inode = fat_alloc_inode;
// 	sb->op.destroy_inode = fat_destroy_inode;
// 	sb->op.read = imgfs_read;
// 	sb->op.write = imgfs_write;
// 	sb->op.clear = imgfs_clear;
// 	sb->op.statfs = fat_stat_fs;

// 	// Initialize in-mem root.
// 	iroot = fat32_root_init(sb);
// 	sb->root = kmalloc(sizeof(struct dentry));
// 	if (iroot == NULL || sb->root == NULL)
// 		goto fail;

// 	iroot->entry = sb->root;
// 	memset(sb->root, 0, sizeof(struct dentry));
// 	sb->root->inode = iroot;
// 	sb->root->op = &rootfs_dentry_op;
// 	// sb->root->filename[0] = '\0';

// 	sb->dev = idup(img);

// 	__debug_info("image_fs_init", "done\n");
// 	return sb;

// fail:
// 	__debug_warn("image_fs_init", "fail\n");
// 	if (iroot)
// 		fat_destroy_inode(iroot);
// 	if (fat)
// 		kfree(fat);
// 	if (sb_buf)
// 		kfree(sb_buf);
// 	if (sb)
// 		kfree(sb);
// 	return NULL;
// }


int disk_write_block(struct superblock *sb, int usr, char *src, uint64 sectorno, uint64 off, uint64 len)
{
	if (off + len > BSIZE)
		panic("diskfs_write");

	// __debug_info("diskfs_write", "sec:%d off:%d len:%d\n", sectorno, off, len);
	struct buf *b = bread(sb->devnum, sectorno);
	int ret = either_copyin(b->data + off, usr, (uint64)src, len);
	
	// if (ret < 0) { // fail to write
	// 	b->valid = 0;  // invalidate the buf
	// } else {
	// 	ret = len;
	// 	bwrite(b, BWRITE_BACK);
	// }

	if (ret == 0)
		ret = len;

	bwrite(b, BWRITE_BACK);
	brelse(b);

	return ret;
}


int disk_read_block(struct superblock *sb, int usr, char *dst, uint64 sectorno, uint64 off, uint64 len)
{
	if (off + len > BSIZE)
		panic("diskfs_read");

	// __debug_info("diskfs_read", "sec:%d off:%d len:%d\n", sectorno, off, len);
	struct buf *b = bread(sb->devnum, sectorno);
	int ret = either_copyout(usr, (uint64)dst, b->data + off, len);
	brelse(b);

	return ret < 0 ? -1 : len;
}


int disk_clear_block(struct superblock *sb, uint64 sectorno, uint64 sectorcnt)
{
	struct buf *b;
	while (sectorcnt--) {
		b = bget(sb->devnum, sectorno++);	// we don't care about the original data
		memset(b->data, 0, BSIZE);
		bwrite(b, BWRITE_BACK);
		brelse(b);
	}
	return 0;
}


struct superblock *fs_install(struct inode *dev)
{
	int isdev;

	__debug_info("fs_install", "enter check\n");
	if (dev->mode & S_IFBLK) {
		struct inode *ip = namei("/dev/vda2");
		if (ip != dev) {
			if (ip != NULL)
				iput(ip);
			return NULL;
		}
		iput(ip);
		isdev = 1;
	} else if (dev->mode & S_IFREG)
		isdev = 0;
	else
		return NULL;

	__debug_info("fs_install", "begin\n");

	struct inode *iroot = NULL;
	struct superblock *sb = fat32_get_sb();

	if (sb == NULL)
		return NULL;

	sb->op.alloc_inode = fat_alloc_inode;
	sb->op.destroy_inode = fat_destroy_inode;
	sb->op.statfs = fat_stat_fs;
	if (isdev) {
		sb->op.read = disk_read_block;
		sb->op.write = disk_write_block;
		sb->op.clear = disk_clear_block;
	} else {
		sb->op.read = img_read_block;
		sb->op.write = img_write_block;
		sb->op.clear = img_clear_block;
	}

	initsleeplock(&sb->sb_lock, "fs_sb");
	initlock(&sb->cache_lock, "fs_dcache");

	sb->dev = idup(dev);
	sb->devnum = dev->dev;
	sb->next = NULL;
	sb->ref = 0;

	__debug_info("fs_install", "read superblock\n");
	iroot = fat32_init(sb);
	if (iroot == NULL)
		goto fail;

	if (isdev && sb->blocksz != BSIZE)
		goto fail;

	__debug_info("fs_install", "stub 1\n");
	sb->root = kmalloc(sizeof(struct dentry));
	if (sb->root == NULL)
		goto fail;

	__debug_info("fs_install", "stub 2\n");
	iroot->entry = sb->root;

	// Initialize in-mem dentry struct for root.
	memset(sb->root, 0, sizeof(struct dentry));
	sb->root->inode = iroot;
	sb->root->op = &generic_dop;
	__debug_info("fs_install", "done\n");

	return sb;

fail:
	__debug_warn("fs_install", "fail\n");
	iput(dev);
	if (iroot)
		fat_destroy_inode(iroot);
	if (sb)
		fat32_kill_sb(sb);
	return NULL;
}

static void fs_clean_dentry(struct superblock *sb, struct dentry *de)
{
	sb->op.destroy_inode(de->inode);
	for (struct dentry *child = de->child; child != NULL;) {
		struct dentry *temp = child->next;
		fs_clean_dentry(sb, child);
		child = temp;
	}
	kfree(de);
}

void fs_uninstall(struct superblock *sb)
{
	iput(sb->dev);
	fs_clean_dentry(sb, sb->root);
	fat32_kill_sb(sb);
}
