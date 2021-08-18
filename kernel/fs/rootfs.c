#ifndef __DEBUG_fs
#undef DEBUG
#endif

#define __module_name__ "rootfs"

#include "types.h"
#include "param.h"
#include "errno.h"
#include "fs/fs.h"
#include "mm/kmalloc.h"
#include "mm/vm.h"
#include "printf.h"
#include "utils/string.h"
#include "utils/debug.h"


struct superblock rootfs;
struct superblock procfs;
struct superblock devfs;


/**
 * Root file system functions.
 * Deny most of them, because this is a virtual in-memory file system.
 */

struct inode *dummy_create(struct inode *ip, char *name, int mode) { return NULL; }
struct inode *dummy_lookup(struct inode *dir, char *filename, uint *poff) { return NULL; }
int dummy_iop1(struct inode *ip) { return -1; }
int dummy_setattr(struct inode *ip, struct kstat *st) { return -1; }
int dummy_rename(struct inode *ip, struct inode *dp, char *newname) { return -1; }
int dummy_file_rw(struct inode *ip, int usr, uint64 dst, uint off, uint n) { return 0; }
int dummy_file_readdir(struct inode *dp, struct dirent *dent, uint off) { return -1; }
int dummy_file_rw_vec(struct inode *ip, struct iovec *iovecs, int count, uint off) { return 0; }

static int rootfs_getattr(struct inode *ip, struct kstat *st)
{
	memset(st, 0, sizeof(struct kstat));
	st->blksize = ip->sb->blocksz;
	st->size = ip->size;
	st->blocks = (st->size + st->blksize - 1) / st->blksize;
	st->dev = ip->sb->devnum;
	st->rdev = ip->dev;
	st->ino = ip->inum;
	st->mode = ip->mode;
	st->nlink = ip->nlink;
	return 0;
}


int root_file_readdir(struct inode *dp, struct dirent *dent, uint off)
{
	struct dentry *entry = dp->entry;
	if (!S_ISDIR(dp->mode))
		return -1;

	int childnum = off >> 5;
	acquire(&rootfs.cache_lock);
	struct dentry *child;
	for (child = entry->child; child != NULL && childnum > 0; child = child->next)
	{
		childnum--;
	}
	release(&rootfs.cache_lock);

	if (!child)
		return 0;

	struct inode *ip = child->inode;
	safestrcpy(dent->name, child->filename, 128);
	int size = sizeof(struct dirent) - sizeof(dent->name) + strlen(dent->name) + 1;
	size += (sizeof(uint64) - (size % sizeof(uint64))) % sizeof(uint64); // Align to 8.
	dent->ino = ip->inum;
	dent->off = off;
	dent->reclen = size;
	dent->type = S_ISDIR(ip->mode) ? T_DIR : (ip->dev ? T_DEVICE : T_FILE);
	return 32;
}


int zero_read(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
#define ZBUFSZ		128
#define ZBIGBUFSZ	512

	// uint bufsz = ZBUFSZ;
	// char sbuf[ZBUFSZ];
	// char *buf = sbuf;

	// if (n > ZBUFSZ * 8) {
	// 	buf = kmalloc(ZBIGBUFSZ);
	// 	if (buf == NULL)
	// 		buf = sbuf;
	// 	else
	// 		bufsz = ZBIGBUFSZ;
	// }

	// memset(buf, 0, bufsz);

	// uint tot, m = bufsz;
	// for (tot = 0; tot < n; tot += m, dst += m) {
	// 	if (n - tot < m)
	// 		m = n - tot;
	// 	if (either_copyout_nocheck(usr, dst, buf, m) < 0)
	// 		break;
	// }

	// if (bufsz == ZBIGBUFSZ)
	// 	kfree(buf);

	char c = 0;
	if (copyout_noinc_nocheck(dst, &c, n) < 0)
		return -EFAULT;

	return n;
}


int zero_write(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
	return n;
}

int null_read(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
	return 0;
}


static struct dentry *de_rootfs_cache(struct dentry *parent, char *name)
{
	struct dentry *de;
	for (de = parent->child; de != NULL; de = de->next) {
		if (strncmp(de->filename, name, MAXNAME) == 0) {
			return de_mnt_in(de);
		}
	}
	return NULL;
}


struct dentry_op rootfs_dentry_op = {
	.delete = de_delete,
	.cache = de_rootfs_cache,
};

struct file_op rootfs_file_op = {
	.read = dummy_file_rw,
	.write = dummy_file_rw,
	.readdir = root_file_readdir,
	.readv = dummy_file_rw_vec,
	.writev = dummy_file_rw_vec,
};

struct inode_op rootfs_inode_op = {
	.create = dummy_create,
	.lookup = dummy_lookup,
	.truncate = dummy_iop1,
	.unlink = dummy_iop1,
	.update = dummy_iop1,
	.getattr = rootfs_getattr,
	.setattr = dummy_setattr,
	.rename = dummy_rename,
};

struct file_op zero_op = {
	.read = zero_read,
	.write = zero_write,
	.readdir = dummy_file_readdir,
	.readv = dummy_file_rw_vec,
	.writev = dummy_file_rw_vec,
};

struct file_op null_op = {
	.read = null_read,
	.write = zero_write,
	.readdir = dummy_file_readdir,
	.readv = dummy_file_rw_vec,
	.writev = dummy_file_rw_vec,
};

static struct dentry *de_root_generate(struct superblock *sb,
							struct dentry *parent, char *name,
							int inum, int mode, int devnum)
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

	// insert those files into root's dentry tree
	de->inode = ip;
	de->op = &rootfs_dentry_op;
	ip->entry = de;
	ip->sb = sb;
	
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
	ip->nlink = 1;
	
	initsleeplock(&ip->lock, "inode");
	initlock(&ip->ilock, "inode2");
	safestrcpy(de->filename, name, MAXNAME);
	__debug_info("de_root_generate", "done\n");

	return de;
}


// Must be called by initcode.
void rootfs_init()
{
	__debug_info("rootfs_init", "enter\n");

	int inum = 0;

	// init rootfs
	memset(&rootfs, 0, sizeof(struct superblock));
	initsleeplock(&rootfs.sb_lock, "rootfs_sb");
	initlock(&rootfs.cache_lock, "rootfs_dcache");

	if ((rootfs.root = de_root_generate(&rootfs, NULL, "/", inum++, S_IFDIR, 0)) == NULL)
		panic("rootfs_init: /");

	// init devfs
	struct dentry *con, *vda, *zero, *null;
	memset(&devfs, 0, sizeof(struct superblock));
	initsleeplock(&devfs.sb_lock, "devfs_sb");
	initlock(&devfs.cache_lock, "devfs_dcache");
	if ((devfs.root = de_root_generate(&devfs, NULL, "/", inum++, S_IFDIR, 0)) == NULL)
		panic("rootfs_init: devfs /");
	if ((con = de_root_generate(&devfs, devfs.root, "console", inum++, S_IFCHR, 2)) == NULL)
		panic("rootfs_init: devfs console");
	if ((vda = de_root_generate(&devfs, devfs.root, "vda2", inum++, S_IFBLK, ROOTDEV)) == NULL)
		panic("rootfs_init: devfs vda2");
	if ((zero = de_root_generate(&devfs, devfs.root, "zero", inum++, S_IFCHR, 3)) == NULL)
		panic("rootfs_init: devfs zero");
	if ((null = de_root_generate(&devfs, devfs.root, "null", inum++, S_IFCHR, 4)) == NULL)
		panic("rootfs_init: devfs null");

	extern struct file_op console_op;
	con->inode->fop = &console_op;

	zero->inode->fop = &zero_op;
	null->inode->fop = &null_op;

	// init procfs
	struct dentry *mount;
	memset(&procfs, 0, sizeof(struct superblock));
	initsleeplock(&procfs.sb_lock, "procfs_sb");
	initlock(&procfs.cache_lock, "procfs_dcache");
	if ((procfs.root = de_root_generate(&procfs, NULL, "/", inum++, S_IFDIR, 0)) == NULL)
		panic("rootfs_init: procfs /");
	if ((mount = de_root_generate(&procfs, procfs.root, "mounts", inum++, S_IFREG, 0)) == NULL)
		panic("rootfs_init: procfs mounts");
	if (de_root_generate(&procfs, procfs.root, "meminfo", inum++, S_IFREG, 0) == NULL)
		panic("rootfs_init: procfs meminfo");

	extern struct file_op mountinfo_fop;
	mount->inode->fop = &mountinfo_fop;

	// mount disk
	if (do_mount(vda->inode, rootfs.root->inode, "fat32", 0, 0) < 0)
		panic("rootfs_init: mount disk");

	// make directories and mount
	struct inode *dir;

	if ((dir = create(NULL, "/dev", S_IFDIR)) == NULL)
		panic("rootfs_init: on disk /dev");
	iunlockput(dir);
	dir = namei("/dev");
	ilock(dir);
	if (mountsysfs(&devfs, dir, "devfs") < 0)
		panic("rootfs_init: mount devfs");
	iunlockput(dir);

	if ((dir = create(NULL, "/proc", S_IFDIR)) == NULL)
		panic("rootfs_init: on disk /proc");
	iunlockput(dir);
	dir = namei("/proc");
	ilock(dir);
	if (mountsysfs(&procfs, dir, "procfs") < 0)
		panic("rootfs_init: mount procfs");
	iunlockput(dir);

	if ((dir = create(NULL, "/bin", S_IFDIR)) == NULL)
		panic("rootfs_init: on disk /bin");
	iunlockput(dir);

	syncfs();

	__debug_info("rootfs_init", "done\n");
}
