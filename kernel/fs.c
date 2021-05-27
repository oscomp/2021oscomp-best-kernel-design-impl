// Virtual File System (only supports FAT32 so far).

#ifndef __DEBUG_fs
#undef DEBUG
#endif

#define __module_name__	"fs"

#include "include/param.h"
#include "include/fs.h"
#include "include/fat32.h"
#include "include/buf.h"
#include "include/kmalloc.h"
#include "include/proc.h"
#include "include/printf.h"
#include "include/string.h"
#include "include/debug.h"


extern struct superblock *disk_fs_init(struct inode *dev);
extern int diskfs_write(struct superblock *sb, int usr, char *src, uint64 sectorno, uint64 off, uint64 len);
extern int diskfs_read(struct superblock *sb, int usr, char *dst, uint64 sectorno, uint64 off, uint64 len);
extern int diskfs_clear(struct superblock *sb, uint64 sectorno, uint64 sectorcnt);

static struct dentry *de_root_generate(struct dentry *parent,
						char *name, int inum, int mode, int devnum, int childnum);
static struct dentry *de_check_cache(struct dentry *parent, char *name);
int de_delete(struct dentry *de);

static struct inode *rootfs_create(struct inode *ip, char *name, int mode);
static struct inode *rootfs_lookup(struct inode *dir, char *filename, uint *poff);
static int rootfs_iop1(struct inode *ip);
static int rootfs_getattr(struct inode *ip, struct kstat *st);
static int root_file_rw(struct inode *ip, int usr, uint64 dst, uint off, uint n);
static int root_file_readdir(struct inode *ip, struct dirent *dent, uint off);

/**
 * Root file system functions.
 *
 */

extern struct inode_op fat32_inode_op;
extern struct file_op fat32_file_op;

static struct superblock rootfs = {
	.devnum = ROOTDEV,
	.dev = NULL,
	.op.alloc_inode = fat_alloc_inode,
	.op.destroy_inode = fat_destroy_inode,
	.op.write = diskfs_write,
	.op.read = diskfs_read,
	.op.clear = diskfs_clear,
};

struct dentry_op rootfs_dentry_op = {
	.delete = de_delete,
};

struct file_op rootfs_file_op = {
	.read = root_file_rw,
	.write = root_file_rw,
	.readdir = root_file_readdir,
};

struct inode_op rootfs_inode_op = {
	.create = rootfs_create,
	.lookup = rootfs_lookup,
	.truncate = rootfs_iop1,
	.unlink = rootfs_iop1,
	.update = rootfs_iop1,
	.getattr = rootfs_getattr,
};

// Must be called by initcode.
void rootfs_init()
{
	__debug_info("rootfs_init", "enter\n");
	rootfs.ref = 0;
	initsleeplock(&rootfs.sb_lock, "rootfs_sb");
	initlock(&rootfs.cache_lock, "rootfs_dcache");

	rootfs.real_sb = NULL;
	if ((rootfs.root = de_root_generate(NULL, "/", 0, I_MODE_DIR, 0, 0)) == NULL)
		panic("rootfs_init 1");

	struct dentry *de, *home, *con;
	if ((de = de_root_generate(rootfs.root, "dev", 1, I_MODE_DIR, 0, 0)) == NULL)
		panic("rootfs_init 2");
	if ((home = de_root_generate(rootfs.root, "home", 2, I_MODE_DIR, 0, 1)) == NULL)
		panic("rootfs_init 3");
	if ((con = de_root_generate(de, "console", 3, 0, 2, 0)) == NULL)
		panic("rootfs_init 4");
	if ((de = de_root_generate(de, "vda2", 4, 0, ROOTDEV, 1)) == NULL)
		panic("rootfs_init 5");

	extern struct file_op console_op;
	con->inode->fop = &console_op;

	if (do_mount(de->inode, home->inode, "fat32", 0, 0) < 0)
		panic("rootfs_init 6");

	__debug_info("rootfs_init", "done\n");
}


static struct dentry *de_root_generate(struct dentry *parent,
			char *name, int inum, int mode, int devnum, int childnum)
{
	struct dentry *de;
	struct inode *ip;
	if ((de = kmalloc(sizeof(struct dentry))) == NULL ||
		(ip = kmalloc(sizeof(struct inode))) == NULL)
	{
		__debug_warn("de_root_generate", "fail\n");
		if (de)
			kfree(de);
		return NULL;
	}
	memset(de, 0, sizeof(struct dentry));
	memset(ip, 0, sizeof(struct inode));

	// let's play some tricks
	*(int *)(de->filename + 128) = childnum;

	// insert those files into root's dentry tree
	de->inode = ip;
	ip->entry = de;
	ip->sb = &rootfs;
	
	if (parent) {
		de->next = parent->child;
		parent->child = de;
		de->parent = parent;
	}

	ip->inum = inum;
	ip->dev = devnum;
	ip->op = &rootfs_inode_op;
	ip->fop = &rootfs_file_op;
	ip->state = I_STATE_VALID;	// not necessary
	ip->mode = mode;
	
	initsleeplock(&ip->lock, "inode");
	safestrcpy(de->filename, name, MAXNAME);
	__debug_info("de_root_generate", "done\n");

	return de;
}

static struct inode *rootfs_create(struct inode *ip, char *name, int mode) {
	return NULL;
}

static struct inode *rootfs_lookup(struct inode *dir, char *filename, uint *poff) {
	return NULL;
}

static int rootfs_iop1(struct inode *ip) {
	return -1;
}

static int rootfs_getattr(struct inode *ip, struct kstat *st) {
	memset(st, 0, sizeof(struct kstat));
    st->blksize = ip->sb->blocksz;
    st->size = ip->size;
    st->blocks = (st->size + st->blksize - 1) / st->blksize;
    st->dev = ip->sb->devnum;
	st->rdev = ip->dev;
    st->ino = ip->inum;
    st->mode = ip->mode;
	return 0;
}

static int root_file_rw(struct inode *ip, int usr, uint64 dst, uint off, uint n) {
	return 0;
}

static int root_file_readdir(struct inode *ip, struct dirent *dent, uint off) {
	struct dentry *entry = ip->entry;
	if (!(ip->mode & I_MODE_DIR))
		return -1;

	int childnum = off >> 5;
	acquire(&rootfs.cache_lock);
	struct dentry *child;
	for (child = entry->child; child != NULL; child = child->next)
	{
		if (*(int*)(child->filename + 128) == childnum)
			break;
	}
	release(&rootfs.cache_lock);

	if (child) {
		struct inode *ip = child->inode;
		safestrcpy(dent->name, child->filename, 128);
		int size = sizeof(struct dirent) - sizeof(dent->name) + strlen(dent->name) + 1;
		size += (sizeof(uint64) - (size % sizeof(uint64))) % sizeof(uint64); // Align to 8.
		dent->ino = ip->inum;
		dent->off = off;
		dent->reclen = size;
		dent->type = (ip->mode & I_MODE_DIR) ? T_DIR : (ip->dev ? T_DEVICE : T_FILE);
		return 32;
	}
	return 0;
}


/**
 * File system inode
 * 
 */

struct inode *create(struct inode *dp, char *path, int mode)
{
	struct inode *ip;
	char name[MAXNAME + 1];
	struct dentry *de;

	if (dp != NULL && !(dp->mode & I_MODE_DIR)) {
		__debug_warn("create", "create on file\n");
		return NULL;
	}
	
	if ((dp = nameiparentfrom(dp, path, name)) == NULL) {
		__debug_warn("create", "%s doesn't exist\n", path);
		return NULL;
	}

	ilock(dp);
	if (dp->state & I_STATE_FREE) {
		__debug_warn("create", "%s doesn't exist\n", path);
		iunlockput(dp);
		return NULL;
	}

	struct superblock *sb = dp->sb;
	acquire(&sb->cache_lock);
	de = de_check_cache(dp->entry, name);
	release(&sb->cache_lock);
	if (de != NULL) {
		iunlockput(dp);
		ip = idup(de->inode);
		if ((mode ^ ip->mode) & I_MODE_DIR) { // dir and file
			iput(ip);
			__debug_warn("create", "%s exists in cache but type is wrong\n", path);
			ip = NULL;
		} else {
			ilock(ip);
		}
		return ip;
	}

	if ((de = kmalloc(sizeof(struct dentry))) == NULL) {
		iunlockput(dp);
		return NULL;
	}

	if ((ip = dp->op->create(dp, name, mode)) == NULL) {
		__debug_warn("create", "%s create fail\n", path);
		iunlockput(dp);
		kfree(de);
		return NULL;
	}

	idup(ip);
	ip->state = I_STATE_VALID;
	ip->entry = de;

	safestrcpy(de->filename, name, MAXNAME + 1);
	de->child = NULL;
	de->mount = NULL;
	de->inode = ip;
	de->op = &rootfs_dentry_op;

	acquire(&sb->cache_lock);
	de->parent = dp->entry;
	de->next = dp->entry->child;
	dp->entry->child = de;
	release(&sb->cache_lock);

	if ((mode ^ ip->mode) & I_MODE_DIR) {
		iunlockput(dp);
		iput(ip);
		__debug_warn("create", "%s exists but type is wrong\n", path);
		return NULL;
	}

	iunlockput(dp);
	ilock(ip);

	return ip;
}


// Caller must hold ip->lock.
int unlink(struct inode *ip)
{
	struct superblock *sb = ip->sb;
	struct dentry *de = ip->entry;

	__debug_info("unlink", "unlink %s\n", de->filename);
	for (sb = &rootfs; sb != NULL; sb = sb->next) {
		if (sb->dev == ip) {
			__debug_warn("unlink", "%s is busy\n", de->filename);
			return -1;
		}
	}

	sb = ip->sb;
	if (de == sb->root) {
		__debug_warn("unlink", "try to unlink root\n");
		return -1;
	}
	if (ip->op->unlink(ip) < 0) {
		__debug_warn("unlink", "fail\n");
		return -1;
	}

	acquire(&sb->cache_lock);
	// if (ip->nlink == 0) {
		// No other files link to ip
		ip->state |= I_STATE_FREE;
		// Remove the dentry from cache, but do not free it.
		de_delete(de);
	// }
	release(&sb->cache_lock);

	return 0;
}


struct inode *idup(struct inode *ip)
{
	acquire(&ip->sb->cache_lock);
	ip->sb->ref++;
	ip->ref++;
	release(&ip->sb->cache_lock);
	return ip;
}


void iput(struct inode *ip)
{
	// Lock the cache so that no one can get ip.
	struct superblock *sb = ip->sb;
	acquire(&sb->cache_lock);
	sb->ref--;
	if (ip->ref == 1) {
		// ref == 1 means no other process can have ip locked,
		// so this acquiresleep() won't block (or deadlock).
		acquiresleep(&ip->lock);
		release(&sb->cache_lock);

		// This file is removed, so its dentry should have been
		// deleted from the dentry tree.
		// This inode is invisible, just free it.
		if (ip->state & I_STATE_FREE) {
			if (ip->nlink == 0)
				ip->op->truncate(ip);
			kfree(ip->entry);
			sb->op.destroy_inode(ip);
			return;
		}
		else if (ip->state & I_STATE_DIRTY) {
			ip->op->update(ip);
		}
		releasesleep(&ip->lock);
		acquire(&sb->cache_lock);
	}
	ip->ref--;
	release(&sb->cache_lock);
}


void ilock(struct inode *ip)
{
	if (ip == 0 || ip->ref < 1)
		panic("ilock");
	acquiresleep(&ip->lock);
}


void iunlock(struct inode *ip)
{
	if (ip == 0 || !holdingsleep(&ip->lock) || ip->ref < 1)
		panic("iunlock");
	releasesleep(&ip->lock);
}


/**
 * File system dentry
 * 
 */


/**
 * If de is a moint point, return the mounted root.
 */
static inline struct dentry *de_mnt_in(struct dentry *de)
{
	while (de->mount != NULL) {
		de = de->mount->root;
	}
	return de;
}

// Returns a dentry struct. If name is given, check ecache. It is difficult to cache entries
// by their whole path. But when parsing a path, we open all the directories through it, 
// which forms a linked list from the final file to the root. Thus, we use the "parent" pointer 
// to recognize whether an entry with the "name" as given is really the file we want in the right path.
// Should never get root by eget, it's easy to understand.
// Caller should hold superblock's cache_lock.
static struct dentry *de_check_cache(struct dentry *parent, char *name)
{
	struct dentry *pde = NULL;
	struct dentry *de = parent->child;
	for (; de != NULL; pde = de, de = de->next) {          // LRU algo
		if (strncmp(de->filename, name, MAXNAME) == 0) {
			if (de != parent->child) {
				pde->next = de->next;
				de->next = parent->child;
				parent->child = de;
			}
			return de_mnt_in(de);
		}
	}
	return NULL;
}


// static void eavail(struct dirent *ep)
// {
// 	if (ep->valid == 1 || ep->ref != 1) {
// 		panic("eavail");
// 	}
// 	ep->valid = 1;
// 	initsleeplock(&ep->lock, "entry");
// 	acquire(&ecachelock);
// 	ep->next = ep->parent->child;
// 	ep->parent->child = ep;
// 	ep->child = NULL;
// 	release(&ecachelock);
// }


// Caller must hold superblock's cache_lock.
int de_delete(struct dentry *de)
{
	if (de->child != NULL)
		panic("de_delete: has children");

	struct dentry **pde;
	for (pde = &de->parent->child; *pde != NULL; pde = &(*pde)->next) {
		if (*pde == de) {
			*pde = de->next;
			return 0;
		}
	}

	panic("de_delete: not in cache delete");
	return -1;
}

void de_print(struct superblock *sb, int level);

static void do_de_print(struct dentry *de, int level)
{
	struct dentry *child;
	for (child = de->child; child != NULL; child = child->next) {
		for (int i = 0; i < level; i++) {
			printf("\t");
		}
		printf(__INFO("%d")" %s\n", child->inode->ref, child->filename);
		do_de_print(child, level + 1);
		if (child->mount) {
			release(&child->inode->sb->cache_lock);
			de_print(child->mount, level);
			acquire(&child->inode->sb->cache_lock);
		}
	}
}

void de_print(struct superblock *sb, int level)
{
	acquire(&sb->cache_lock);

	struct dentry *root = sb->root;
	for (int i = 0; i < level; i++) {
		printf("\t");
	}
	if (sb->dev) {
		printf(__INFO("%d/%d")" %s mounted at %s\n",
			sb->ref, root->inode->ref, sb->dev->entry->filename, root->parent->filename);
	} else {
		printf(__INFO("%d/%d")" %s\n", sb->ref, root->inode->ref, root->filename);
	}
	do_de_print(root, level + 1);

	release(&sb->cache_lock);
}


void rootfs_print()
{
	printf("\n"__INFO("file tree")":\n");
	de_print(&rootfs, 0);
}


/**
 * File system path
 * 
 */


/**
 * Seacher for the entry in a directory and return a structure. Besides, record the offset of
 * some continuous empty slots that can fit the length of filename.
 * Caller must hold entry->lock.
 * @param   dp          entry of a directory file
 * @param   filename    target filename
 * @param   poff        offset of proper empty entry slots from the beginning of the dir
 */
struct inode *dirlookup(struct inode *dir, char *filename, uint *poff)
{
	if (!(dir->mode & I_MODE_DIR))
		panic("dirlookup");

	__debug_info("dirlookup", "start searching %s\n", filename);
	struct superblock *sb = dir->sb;
	struct dentry *de, *parent;
	if (strncmp(filename, ".", MAXNAME) == 0) {
		de = de_mnt_in(dir->entry);
		return idup(de->inode);
	}
	else if (strncmp(filename, "..", MAXNAME) == 0) {
		de = dir->entry;
		while (de == sb->root) { // It indicates that de is a root and may be a mount point.
			de = de->parent;
			if (de == NULL) { // Meet root of rootfs.
				de = sb->root;
				break;
			}
			sb = de->inode->sb;
		}
		de = de_mnt_in(de->parent == NULL ? de : de->parent); // Now we found the real parent.
		return idup(de->inode);
	}

	__debug_info("dirlookup", "not dots\n");
	acquire(&sb->cache_lock);
	__debug_info("dirlookup", "checking cache\n");
	de = de_check_cache(dir->entry, filename);
	__debug_info("dirlookup", "check done\n");
	release(&sb->cache_lock);
	if (de != NULL) {
		__debug_info("dirlookup", "cache hit: %s\n", filename);
		return idup(de->inode);
	}

	__debug_info("dirlookup", "look up on disk: %s\n", filename);
	struct inode *ip = dir->op->lookup(dir, filename, poff);
	if (ip == NULL || (de = kmalloc(sizeof(struct dentry))) == NULL) {
		if (ip) {
			sb->op.destroy_inode(ip);
		}
		__debug_warn("dirlookup", "file not found: %s\n", filename);
		return NULL;
	}
	__debug_info("dirlookup", "found: %s\n", filename);

	idup(ip);
	ip->entry = de;
	ip->state = I_STATE_VALID;

	safestrcpy(de->filename, filename, MAXNAME + 1);
	de->child = NULL;
	de->mount = NULL;
	de->inode = ip;
	de->op = &rootfs_dentry_op;

	acquire(&sb->cache_lock);
	parent = dir->entry;
	de->parent = parent;
	de->next = parent->child;
	parent->child = de;
	release(&sb->cache_lock);

	return ip;
}


static char *skipelem(char *path, char *name, int max)
{
	while (*path == '/') {
		path++;
	}
	if (*path == 0) { return NULL; }
	char *s = path;
	while (*path != '/' && *path != 0) {
		path++;
	}
	int len = path - s;
	if (len > max) {
		len = max;
	}
	name[len] = 0;
	memmove(name, s, len);
	while (*path == '/') {
		path++;
	}
	return path;
}


// FAT32 version of namex in xv6's original file system.
static struct inode *lookup_path(struct inode *ip, char *path, int parent, char *name)
{
	struct inode *next;
	if (*path == '/') {
		ip = idup(rootfs.root->inode);
	} else if (*path != '\0') {
		if (ip != NULL)
			ip = idup(ip);
		else
			ip = idup(myproc()->cwd);
	} else {
		__debug_warn("lookup_path", "path invalid\n");
		return NULL;
	}

	while ((path = skipelem(path, name, MAXNAME)) != 0) {
		__debug_info("lookup_path", "lookup: %s\n", name);
		ilock(ip);
		if (!(ip->mode & I_MODE_DIR)) {
			iunlockput(ip);
			return NULL;
		}
		if (parent && *path == '\0') {
			iunlock(ip);
			return ip;
		}
		if ((next = dirlookup(ip, name, 0)) == NULL) {
			iunlockput(ip);
			__debug_warn("lookup_path", "dirlookup returns a NULL\n");
			return NULL;
		}
		__debug_info("lookup_path", "found: %s\n", next->entry->filename);
		iunlockput(ip);
		ip = next;
	}
	if (parent) {
		iput(ip);
		return NULL;
	}
	__debug_info("lookup_path", "finally: %s\n", ip->entry->filename);
	return ip;
}

struct inode *namei(char *path)
{
	char name[MAXNAME + 1];
	return lookup_path(NULL, path, 0, name);
}

struct inode *nameiparent(char *path, char *name)
{
	return lookup_path(NULL, path, 1, name);
}

struct inode *nameifrom(struct inode *ip, char *path)
{
	char name[MAXNAME + 1];
	return lookup_path(ip, path, 0, name);
}

struct inode *nameiparentfrom(struct inode *ip, char *path, char *name)
{
	return lookup_path(ip, path, 1, name);
}

// path is kernel space, and max must be bigger than 2.
int namepath(struct inode *ip, char *path, int max)
{
	// if (max < 2)
	// 	panic("namepath: what do you want from me by a max less than 2");

	struct superblock *sb = ip->sb;
	struct dentry *de = ip->entry;

	if (de == rootfs.root) {
		path[0] = '/';
		path[1] = '\0';
		return 0;
	}

	int len;
	char *p = path + max - 1;
	*p = '\0';

	acquire(&sb->cache_lock);
	for (;;) {
		while (de == sb->root) { // It indicates that de is a root and may be a mount point.
			if ((de = de->parent) == NULL) { // Meet root of rootfs.
				break;
			}
			release(&sb->cache_lock);
			sb = de->inode->sb;
			acquire(&sb->cache_lock);
		}
		if (de == NULL)
			break;
		len = strlen(de->filename);
		if ((p -= len) <= path) {
			release(&sb->cache_lock);
			return -1;
		}
		memmove(p, de->filename, len);
		*--p = '/';
		de = de->parent;
	}
	release(&sb->cache_lock);

	len = max - (p - path);
	memmove(path, p, len);		// memmove will handle overlap

	return 0;
}


/**
 * Other file system operations.
 * 
 * 
 */

extern struct superblock *image_fs_init(struct inode *img);
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
	if (mntpoint->entry == rootfs.root) {
		__debug_warn("do_mount", "can not mount at \"/\"\n");
		return -1;
	}

	__debug_info("do_mount", "dev:%s mntpnt:%s\n", dev->entry->filename, mntpoint->entry->filename);

	if ((dev->mode & I_MODE_DIR) || !(mntpoint->mode & I_MODE_DIR)) {
		__debug_warn("do_mount", "Error file type: dev:%x mntpoint:%x\n",
			dev->mode, mntpoint->mode);
		return -1;
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
	if (dev->dev == ROOTDEV) {
		// On FAT32 volume, there is no file with device type.
		sb = disk_fs_init(dev);
	}
	else {
		sb = image_fs_init(dev);
	}
	if (sb == NULL)
		return -1;

	acquire(&rootfs.cache_lock); // borrow this lock

	struct superblock *psb = &rootfs;
	while (psb->next != NULL)
		psb = psb->next;
	psb->next = sb;
	sb->root->parent = dmnt;
	safestrcpy(sb->root->filename, dmnt->filename, sizeof(dmnt->filename));
	dmnt->mount = sb;

	release(&rootfs.cache_lock);

	idup(mntpoint);

	return 0;
}

// Caller must hold mntpoint->lock.
// flag is of no use in out implement.
int do_umount(struct inode *mntpoint, int flag)
{
	if (!(mntpoint->mode & I_MODE_DIR)) {
		__debug_warn("do_umount", "try to umount file: %s\n", mntpoint->entry->filename);
		return -1;
	}

	struct superblock *sb = mntpoint->sb;
	if (mntpoint->entry != sb->root || sb == &rootfs) {
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
		return -1;
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
