// Virtual File System (only supports FAT32 so far).

#ifndef __DEBUG_fs
#undef DEBUG
#endif

#define __module_name__	"fs"

#include "param.h"
#include "fs/fs.h"
#include "mm/kmalloc.h"
#include "sched/proc.h"
#include "printf.h"
#include "utils/string.h"
#include "utils/debug.h"
#include "errno.h"


/**
 * File system inode
 * 
 */

struct inode *create(struct inode *dp, char *path, int mode)
{
	struct inode *ip;
	char name[MAXNAME + 1];
	struct dentry *de;

	if (dp != NULL && !S_ISDIR(dp->mode)) {
		__debug_warn("create", "create on file\n");
		return NULL;
	}

	if ((dp = nameiparentfrom(dp, path, name)) == NULL) {
		__debug_warn("create", "%s doesn't exist\n", path);
		// root doesn't have parent
		if (strncmp(path, "/", MAXPATH) != 0 || !S_ISDIR(mode))
			return NULL;
		dp = namei("/");
		ilock(dp);
		return dp;
	}

	ilock(dp);
	if (dp->state & I_STATE_FREE) {
		__debug_warn("create", "%s doesn't exist\n", path);
		iunlockput(dp);
		return NULL;
	}

	struct superblock *sb = dp->sb;
	acquire(&sb->cache_lock);
	de = dp->entry->op->cache(dp->entry, name);
	release(&sb->cache_lock);
	if (de != NULL) {
		iunlockput(dp);
		ip = idup(de->inode);
		if ((mode ^ ip->mode) & S_IFMT) { // unmatch file type
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
	de->op = dp->entry->op;

	acquire(&sb->cache_lock);
	de->parent = dp->entry;
	de->next = dp->entry->child;
	dp->entry->child = de;
	release(&sb->cache_lock);

	if ((mode ^ ip->mode) & S_IFMT) {
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
	struct superblock *sb;
	struct dentry *de = ip->entry;

	__debug_info("unlink", "unlink %s\n", de->filename);
	for (sb = &rootfs; sb != NULL; sb = sb->next) {
		if (sb->dev == ip) {
			__debug_warn("unlink", "%s is busy\n", de->filename);
			return -EBUSY;
		}
	}

	sb = ip->sb;
	if (de == sb->root) {
		__debug_warn("unlink", "try to unlink root\n");
		return -EACCES;
	}

	int ret = ip->op->unlink(ip);
	if (ret < 0) {
		__debug_warn("unlink", "fail\n");
		return ret;
	}

	acquire(&sb->cache_lock);
	// if (ip->nlink == 0) {
		// No other files link to ip
		ip->state |= I_STATE_FREE;
		// Remove the dentry from cache, but do not free it.
		de->op->delete(de);
	// }
	release(&sb->cache_lock);

	return ret;
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


// Returns a dentry struct. If name is given, check ecache. It is difficult to cache entries
// by their whole path. But when parsing a path, we open all the directories through it, 
// which forms a linked list from the final file to the root. Thus, we use the "parent" pointer 
// to recognize whether an entry with the "name" as given is really the file we want in the right path.
// Should never get root by eget, it's easy to understand.
// Caller should hold superblock's cache_lock.
struct dentry *de_check_cache(struct dentry *parent, char *name)
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


// Caller must hold superblock's cache_lock.
int de_delete(struct dentry *de)
{
	// if (de->child != NULL)
	// 	panic("de_delete: has children");

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
	if (rootfs.root->mount)
		de_print(rootfs.root->mount, 0);
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
	if (!S_ISDIR(dir->mode))
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

	__debug_info("dirlookup", "checking cache\n");
	acquire(&sb->cache_lock);
	de = dir->entry->op->cache(dir->entry, filename);
	release(&sb->cache_lock);
	if (de != NULL) {
		__debug_info("dirlookup", "cache hit: %s\n", filename);
		return idup(de->inode);
	}

	__debug_info("dirlookup", "look up on disk: %s\n", filename);
	if ((de = kmalloc(sizeof(struct dentry))) == NULL) {
		return NULL;
	}

	struct inode *ip = dir->op->lookup(dir, filename, poff);
	if (ip == NULL) {
		__debug_warn("dirlookup", "file not found: %s\n", filename);
		kfree(de);
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
	de->op = dir->entry->op;

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
		if (strncmp(path, "/proc/self/exe", 15) == 0) {
			struct proc *p = myproc();
			if (p->elf) return idup(p->elf);
			return NULL;
		}
		ip = de_mnt_in(rootfs.root)->inode;
		ip = idup(ip);
	} else if (*path != '\0') {
		if (ip != NULL)
			ip = idup(ip);
		else {
			ip = idup(myproc()->cwd);
		}
	} else {
		__debug_warn("lookup_path", "path invalid\n");
		return NULL;
	}

	while ((path = skipelem(path, name, MAXNAME)) != 0) {
		__debug_info("lookup_path", "lookup: %s\n", name);
		ilock(ip);
		if (!S_ISDIR(ip->mode)) {
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

	int len;
	char *p = path + max - 1;
	*p = '\0';

	acquire(&sb->cache_lock);
	
	while (de == sb->root) {	// test mount
		release(&sb->cache_lock);
		if (sb == &rootfs) {	// Meet root of rootfs.
			safestrcpy(path, rootfs.root->filename, max);	
			return strlen(rootfs.root->filename) + 1;
		}
		de = de->parent;
		sb = de->inode->sb;
		acquire(&sb->cache_lock);
	}
	
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

	return len;
}


/**
 * Other file system operations.
 * 
 * 
 */


// Is the directory dp empty except for "." and ".." ?
int isdirempty(struct inode *dp)
{
	if (dp->entry->child)
		return 0;

	struct dirent dent;
	int off = 0, ret;
	while (1) {
		ret = dp->fop->readdir(dp, &dent, off);
		if (ret < 0)
			return -1;
		else if (ret == 0)
			return 1;
		else if ((dent.name[0] == '.' && dent.name[1] == '\0') ||     // skip the "." and ".."
				 (dent.name[0] == '.' && dent.name[1] == '.' && dent.name[2] == '\0'))
		{
			off += ret;
		}
		else
			return 0;
	}
}

int do_rename(struct inode *iold, struct inode *dstdir, char *newname)
{
	if (iold->sb != dstdir->sb)
		return -EXDEV;
	if (!dstdir->op->rename)
		return -EACCES;

	int ret = 0;
	struct inode *inew = NULL;
	// lock dstdir to check newname and avoid creating
	ilock(dstdir);
	if (trysleeplock(&iold->lock) < 0) { // just try, in case of deadlock
		iunlock(dstdir);
		return -EBUSY;
	}

	// In what universe can we move a directory into its child?
	acquire(&iold->sb->cache_lock);
	for (struct dentry *de = dstdir->entry; de != NULL; de = de->parent)
		if (de == iold->entry) {
			release(&iold->sb->cache_lock);
			ret = -EINVAL;
			goto end;
		}
	release(&iold->sb->cache_lock);

	if ((inew = dirlookup(dstdir, newname, NULL)) != NULL) {	// new exists
		if (trysleeplock(&inew->lock) < 0) {
			iput(inew);
			inew = NULL;
			ret = -EBUSY;
			goto end;
		}
		if (S_ISDIR(inew->mode)) {			// it's ok to overwrite an empty dir
			if (!S_ISDIR(iold->mode)) {
				ret = -EISDIR;
				goto end;
			} else if (isdirempty(inew) != 1) {
				ret = -EEXIST;
				goto end;
			}
			if (unlink(inew) < 0) {
				ret = -EBUSY;
				goto end;
			}
			iunlockput(inew);
			inew = NULL;
		} else {
			ret = -EEXIST;
			goto end;
		}
	}

	ret = dstdir->op->rename(iold, dstdir, newname);
	if (ret < 0) {
		goto end;
	}

	struct dentry *oldent, *dstent;
	oldent = iold->entry;
	dstent = dstdir->entry;

	acquire(&iold->sb->cache_lock);
	de_delete(oldent);
	oldent->parent = dstent;
	oldent->next = dstent->child;
	dstent->child = oldent;
	release(&iold->sb->cache_lock);

end:
	iunlock(iold);
	iunlock(dstdir);
	if (inew)
		iunlockput(inew);
	return ret;
}

void syncfs(void)
{
	struct superblock *sb;
	acquire(&rootfs.cache_lock);
	for (sb = rootfs.next; sb != NULL; sb = sb->next) {
		if (sb->op.sync) {
			release(&rootfs.cache_lock);
			sb->op.sync(sb);
			acquire(&rootfs.cache_lock);
		}
	}
	release(&rootfs.cache_lock);
}