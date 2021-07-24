#ifndef __FS_H
#define __FS_H

/**
 * A very simple implement of Virtual File System.
 * 
 * To support some unix-style operations like 'mount',
 * It's easier to design the mechanism on a vfs.
 * 
 * This implement is not complete, and the interfaces
 * are poorly considered, which are just designed for
 * FAT32.
 */

#include "types.h"
#include "fs/stat.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"
#include "mm/mmap.h"

struct superblock;
struct inode;
struct dentry;

struct statfs {
	int64	f_type;			/* Type of filesystem (see below) */
	int64	f_bsize;		/* Optimal transfer block size */
	uint64	f_blocks;		/* Total data blocks in filesystem */
	uint64	f_bfree;		/* Free blocks in filesystem */
	uint64	f_bavail;		/* Free blocks available to unprivileged user */
	uint64	f_files;		/* Total inodes in filesystem */
	uint64	f_ffree;		/* Free inodes in filesystem */
	int32	f_fsid[2];		/* Filesystem ID */
	int64	f_namelen;		/* Maximum length of filenames */
	int64	f_frsize;		/* Fragment size (since Linux 2.6) */
	int64	f_flags;		/* Mount flags of filesystem (since Linux 2.6.36) */
	int64	f_spare[4];		/* Padding bytes reserved for future use */
};


/**
 * This collection specifies how to access the disk.
 * All file ops end up calling to these.
 */
struct fs_op {
	struct inode *(*alloc_inode)(struct superblock *sb);
	void (*destroy_inode)(struct inode *ip);
	int (*write)(struct superblock *sb, int usr, char *src, uint64 blockno, uint64 off, uint64 len);
	int (*read)(struct superblock *sb, int usr, char *dst, uint64 blockno, uint64 off, uint64 len);
	int (*clear)(struct superblock *sb, uint64 blockno, uint64 blockcnt);
	int (*statfs)(struct superblock *sb, struct statfs *stat);
};

struct inode_op {
	struct inode *(*create)(struct inode *ip, char *name, int mode);
	struct inode *(*lookup)(struct inode *dir, char *filename, uint *poff);
	int (*truncate)(struct inode *ip);
	int (*unlink)(struct inode *ip);
	int (*update)(struct inode *ip);
	int (*getattr)(struct inode *ip, struct kstat *st);
	int (*setattr)(struct inode *ip, struct kstat *st);
	int (*rename)(struct inode *ip, struct inode *dp, char *newname);
};

struct dentry_op {
	int (*delete)(struct dentry *de);
	struct dentry *(*cache)(struct dentry *parent, char *childname);
};

struct file_op {
	int (*read)(struct inode *ip, int usr, uint64 dst, uint off, uint n);
	int (*write)(struct inode *ip, int usr, uint64 src, uint off, uint n);
	int (*readdir)(struct inode *ip, struct dirent *dent, uint off);
	int (*readv)(struct inode *ip, struct iovec *iovecs, int count, uint off);
	int (*writev)(struct inode *ip, struct iovec *iovecs, int count, uint off);
};


struct superblock {
	uint				blocksz;
	uint				devnum;
	struct inode		*dev;
	char				type[16];

	struct superblock	*next;
	int					ref;		// sum of refs of all its inodes
	
	struct sleeplock	sb_lock;
	struct fs_op		op;

	struct spinlock		cache_lock;
	struct dentry		*root;
};


// For inode.state
#define I_STATE_VALID		(1 << 0)
#define I_STATE_DIRTY		(1 << 2)
#define I_STATE_FREE		(1 << 3)
#define I_STATE_LOCKED		(1 << 4)

struct inode {
	uint64				inum;
	int					ref;
	int					state;
	uint16				mode;
	int16				dev;
	int					size;
	int					nlink;	// useless on FAT
	struct superblock	*sb;

	struct sleeplock	lock;	// mutex, I/O control
	struct inode_op		*op;
	struct file_op		*fop;
	struct spinlock		ilock;	// protect itself
	struct mapped 		*maphead;

	struct dentry		*entry;
};


#define MAXNAME 255

struct dentry {
	char				filename[MAXNAME + 1];
	// int					ref;
	struct inode		*inode;
	struct dentry		*parent;
	struct dentry		*next;
	struct dentry		*child;
	struct dentry_op	*op;
	struct superblock	*mount;
};


extern struct superblock rootfs;

void rootfs_init();
void rootfs_print();

struct inode *create(struct inode *dp, char *path, int mode);
int unlink(struct inode *ip);

struct inode *idup(struct inode *ip);
void iput(struct inode *ip);
void ilock(struct inode *ip);
void iunlock(struct inode *ip);

int de_delete(struct dentry *de);
struct dentry *de_check_cache(struct dentry *parent, char *name);
/**
 * If de is a moint point, return the mounted root.
 */
static inline struct dentry *de_mnt_in(struct dentry *de)
{
	while (de->mount != NULL)
		de = de->mount->root;
	return de;
}

struct inode *namei(char *path);
struct inode *nameiparent(char *path, char *name);
struct inode *nameifrom(struct inode *ip, char *path);
struct inode *nameiparentfrom(struct inode *ip, char *path, char *name);
int namepath(struct inode *ip, char *path, int max);

int isdirempty(struct inode *dp);
int do_mount(struct inode *dev, struct inode *mntpoint, char *type, int flag, void *data);
int do_umount(struct inode *mntpoint, int flag);
int do_rename(struct inode *iold, struct inode *dstdir, char *newname);

static inline void iunlockput(struct inode *ip)
{
	iunlock(ip);
	iput(ip);
}


#endif
