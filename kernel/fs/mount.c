#ifndef __DEBUG_fs
#undef DEBUG
#endif

#define __module_name__ "mount"

#include "fs/fs.h"
#include "errno.h"
#include "mm/pm.h"
#include "utils/string.h"
#include "sprintf.h"
#include "utils/debug.h"

int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);

static int mountinfo_read(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
	uint tot = 0;
	char *buf = allocpage();
	if (buf == NULL)
		return -ENOMEM;

	char *pdev = buf + PGSIZE - MAXPATH;
	char *pmnt = pdev - MAXPATH;

	struct superblock *sb = rootfs.next;
	for (; sb; sb = sb->next) {
		if (sb->dev) {
			namepath(sb->dev, pdev, MAXPATH);
		} else {
			safestrcpy(pdev, sb->type, sizeof(sb->type));
		}
		namepath(sb->root->inode, pmnt, MAXPATH);
		int len = sprintf(buf + tot, PGSIZE - 2 * MAXPATH - tot,
						"%s %s %s\n", pdev, pmnt, sb->type); 
		if (len < 0)
			break;
		tot += len;
		// p[len - 1] = ' ';
		// p += len;
		// tot += len;
		// if (tot >= bufsz) {
		// 	tot = bufsz;
		// 	break;
		// }
		// len = namepath(sb->root->inode, p, bufsz - tot);
		// p[len - 1] = ' ';
		// p += len;
		// tot += len;
		// if (tot >= bufsz) {
		// 	tot = bufsz;
		// 	break;
		// }
		// len = strlen(sb->type);
		// if (tot + len + 1 >= bufsz) {
		// 	break;
		// }
		// safestrcpy(p, sb->type, sizeof(sb->type));
		// p[len++] = '\n';
		// p += len;
		// tot += len;
	}

	uint ret = 0;
	if (off < tot) {
		ret = tot - off < n ? tot - off : n;
		if (either_copyout(usr, dst, buf + off, ret) < 0)
			ret = -EFAULT;
	}

	freepage(buf);
	return ret;
}


extern int dummy_file_rw(struct inode *ip, int usr, uint64 dst, uint off, uint n);
extern int root_file_readdir(struct inode *ip, struct dirent *dent, uint off);
extern int dummy_file_rw_vec(struct inode *ip, struct iovec *iovecs, int count, uint off);

struct file_op mountinfo_fop = {
	.read = mountinfo_read,
	.write = dummy_file_rw,
	.readdir = root_file_readdir,
	.readv = dummy_file_rw_vec,
	.writev = dummy_file_rw_vec,
};

extern struct superblock *fs_install(struct inode *dev);
extern void fs_uninstall(struct superblock *sb);

// Ignore flag and data in our implement.
// Caller must hold all inodes' locks.
int do_mount(struct inode *dev, struct inode *mntpoint, char *type, int flag, void *data)
{
	if (strncmp("vfat", type, 5) != 0 &&
		strncmp("fat32", type, 6) != 0)
	{
		__debug_warn("do_mount", "Unsupported fs type: %s\n", type);
		return -1;
	}
	// if (mntpoint->entry == rootfs.root) {
	// 	__debug_warn("do_mount", "can not mount at \"/\"\n");
	// 	return -1;
	// }

	__debug_info("do_mount", "dev:%s mntpnt:%s\n", dev->entry->filename, mntpoint->entry->filename);

	if (S_ISDIR(dev->mode) || !S_ISDIR(mntpoint->mode)) {
		__debug_warn("do_mount", "Error file type: dev:%x mntpoint:%x\n",
			dev->mode, mntpoint->mode);
		return -ENOTBLK;
	}

	struct dentry *dmnt = mntpoint->entry;
	// We planed only to support one mount at a time.
	// But in fact, if mntpoint is mounted, namei will get its mounting dev,
	// so this could be a root of a fs, at which can be mounted.
	if (dmnt->mount != NULL) {
		__debug_error("do_mount", "%s is already be mounted\n", dmnt->filename);
		panic("do_mount");
	}

	struct superblock *sb;
	sb = fs_install(dev);
	if (sb == NULL)
		return -1;

	acquire(&rootfs.cache_lock); // borrow this lock

	struct superblock *psb = &rootfs;
	while (psb->next != NULL)
		psb = psb->next;
	psb->next = sb;
	sb->root->parent = dmnt;
	safestrcpy(sb->root->filename, dmnt->filename, sizeof(dmnt->filename));
	safestrcpy(sb->type, type, sizeof(sb->type));
	dmnt->mount = sb;

	release(&rootfs.cache_lock);

	idup(mntpoint);

	return 0;
}

// Caller must hold mntpoint->lock.
// flag is of no use in out implement.
int do_umount(struct inode *mntpoint, int flag)
{
	if (!S_ISDIR(mntpoint->mode)) {
		__debug_warn("do_umount", "try to umount file: %s\n", mntpoint->entry->filename);
		return -ENOTDIR;
	}

	struct superblock *sb = mntpoint->sb;
	if (mntpoint->entry != sb->root || sb == &rootfs || sb == &devfs || sb == &procfs) {
		__debug_warn("do_umount", "%s is not a mount point\n", mntpoint->entry->filename);
		return -1;
	}

	struct dentry *de = mntpoint->entry;
	struct superblock *sb_prnt = NULL;
	do { // Look up parent of the mount point.
		de = de->parent;
		if (de == NULL) { // Meet root of rootfs.
			break;
		}
		sb_prnt = de->inode->sb;
	} while (de == sb_prnt->root);

	// Lock parent to block cache check.
	acquire(&sb_prnt->cache_lock);
	// Check whether only we hold this ref.
	if (sb->ref > 1) {
		__debug_warn("do_umount", "mount point is busy\n");
		release(&sb_prnt->cache_lock);
		return -EBUSY;
	}
	// Unmount from dentry tree.
	de = mntpoint->entry;
	de->parent->mount = NULL;
	release(&sb_prnt->cache_lock);

	// Put the inode it mounts at.
	iput(de->parent->inode);

	// Remove from superblock list.
	acquire(&rootfs.cache_lock);
	sb_prnt = &rootfs;
	while (sb_prnt->next != sb)
		sb_prnt = sb_prnt->next;
	sb_prnt->next = sb->next;
	release(&rootfs.cache_lock);

	fs_uninstall(sb);

	return 0;
}

int mountsysfs(struct superblock *sb, struct inode *mntpoint, char *type)
{
	if (!S_ISDIR(mntpoint->mode))
		return -1;

	acquire(&rootfs.cache_lock); // borrow this lock

	struct superblock *psb = &rootfs;
	while (psb->next != NULL)
		psb = psb->next;
	psb->next = sb;

	struct dentry *dmnt = mntpoint->entry;
	sb->root->parent = dmnt;
	safestrcpy(sb->root->filename, dmnt->filename, sizeof(dmnt->filename));
	safestrcpy(sb->type, type, sizeof(sb->type));
	dmnt->mount = sb;

	release(&rootfs.cache_lock);

	idup(mntpoint);
	return 0;
}