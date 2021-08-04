#ifndef __DEBUG_fat32 
#undef DEBUG 
#endif 

#define __module_name__ "fat32"

#include "fat32.h"
#include "errno.h"
#include "printf.h"
#include "mm/kmalloc.h"
#include "utils/string.h"
#include "utils/debug.h"


/* On-disk directory entry structure */
/* Fields that start with "_" are something we don't use */
typedef struct short_name_entry {
	char        name[CHAR_SHORT_NAME];
	uint8       attr;
	uint8       _nt_res;
	uint8       crt_time_tenth;
	uint16      crt_time;
	uint16      crt_date;
	uint16      lst_acce_date;
	uint16      fst_clus_hi;
	uint16      lst_wrt_time;
	uint16      lst_wrt_date;
	uint16      fst_clus_lo;
	uint32      file_size;
} __attribute__((packed, aligned(4))) short_name_entry_t;

typedef struct long_name_entry {
	uint8       order;
	wchar       name1[5];
	uint8       attr;
	uint8       _type;
	uint8       checksum;
	wchar       name2[6];
	uint16      _fst_clus_lo;
	wchar       name3[2];
} __attribute__((packed, aligned(4))) long_name_entry_t;

union fat_disk_entry {
	short_name_entry_t  sne;
	long_name_entry_t   lne;
};


static void generate_shortname(char *shortname, char *name)
{
	static char illegal[] = { '+', ',', ';', '=', '[', ']', 0 };   // these are legal in l-n-e but not s-n-e
	int i = 0;
	char c, *p = name;
	for (int j = strlen(name) - 1; j >= 0; j--) {
		if (name[j] == '.') {
			p = name + j;
			break;
		}
	}
	while (i < CHAR_SHORT_NAME && (c = *name++)) {
		if (i == 8 && p) {
			if (p + 1 < name) { break; }            // no '.'
			else {
				name = p + 1, p = 0;
				continue;
			}
		}
		if (c == ' ') { continue; }
		if (c == '.') {
			if (name > p) {                    // last '.'
				memset(shortname + i, ' ', 8 - i);
				i = 8, p = 0;
			}
			continue;
		}
		if (c >= 'a' && c <= 'z') {
			c += 'A' - 'a';
		} else {
			if (strchr(illegal, c) != NULL) {
				c = '_';
			}
		}
		shortname[i++] = c;
	}
	while (i < CHAR_SHORT_NAME) {
		shortname[i++] = ' ';
	}
}

static uint8 cal_checksum(uchar* shortname)
{
	uint8 sum = 0;
	for (int i = CHAR_SHORT_NAME; i != 0; i--) {
		sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *shortname++;
	}
	return sum;
}

/**
 * Generate an on disk format entry and write to the disk.
 * Caller must hold dir->lock unless in fat_alloc_entry.
 * @param   dir         the directory inode
 * @param   ep          entry to write on disk
 * @param   off         offset int the dp, should be calculated via dirlookup before calling this
 */
int fat_make_entry(struct inode *dir, struct fat32_entry *ep, char *filename, uint off, int islong)
{
	struct fat32_entry *dp = i2fat(dir);
	if (!(dp->attribute & ATTR_DIRECTORY))
		panic("fat_make_entry: not dir");
	if (off % sizeof(union fat_disk_entry))
		panic("fat_make_entry: not aligned");

	uint32 const bpc = sb2fat(dir->sb)->byts_per_clus;
	uint32 clus;
	union fat_disk_entry de;
	memset(&de, 0, sizeof(de));
	if (!islong) {
		strncpy(de.sne.name, filename, sizeof(de.sne.name));
	}
    else {
		char shortname[CHAR_SHORT_NAME + 1];
		memset(shortname, 0, sizeof(shortname));
		generate_shortname(shortname, filename);
		de.lne.checksum = cal_checksum((uchar *)shortname);
		de.lne.attr = ATTR_LONG_NAME;

		for (int i = ep->ent_cnt - 1; i > 0; i--) {
			if ((de.lne.order = i) == ep->ent_cnt - 1) {
				de.lne.order |= LAST_LONG_ENTRY;
			}
			char *p = filename + (i - 1) * CHAR_LONG_NAME;
			uint8 *w = (uint8 *)de.lne.name1;
			int end = 0;
			for (int j = 1; j <= CHAR_LONG_NAME; j++) {
				if (end) {
					*w++ = 0xff;            // on k210, unaligned reading is illegal
					*w++ = 0xff;
				} else { 
					if ((*w++ = *p++) == 0) {
						end = 1;
					}
					*w++ = 0;
				}
				switch (j) {
					case 5:     w = (uint8 *)de.lne.name2; break;
					case 11:    w = (uint8 *)de.lne.name3; break;
				}
			}
			clus = reloc_clus(dir, off, 1);
			// __debug_info("fat_make_entry", "name:%s clus:%d off:%d\n", ep->filename, dp->cur_clus, off2);
			if (!clus || fat_rw_clus(dir->sb, clus, 1, 0, (uint64)&de, off % bpc, sizeof(de)) != sizeof(de))
				return -1;
			off += sizeof(de);
		}
		memset(&de, 0, sizeof(de));
		strncpy(de.sne.name, shortname, sizeof(de.sne.name));
	}
	de.sne.attr = ep->attribute;
	de.sne.fst_clus_hi = (uint16)(ep->first_clus >> 16);      // first clus high 16 bits
	de.sne.fst_clus_lo = (uint16)(ep->first_clus & 0xffff);     // low 16 bits
	de.sne.file_size = ep->file_size;                         // filesize is updated in eupdate()
	clus = reloc_clus(dir, off, 1);
	// __debug_info("fat_make_entry", "name:%s clus:%d off:%d\n", ep->filename, dp->cur_clus, off2);
	if (!clus || fat_rw_clus(dir->sb, clus, 1, 0, (uint64)&de, off % bpc, sizeof(de)) != sizeof(de))
		return -1;
	return 0;
}

// trim ' ' in the head and tail, '.' in head, and test legality
static char *formatname(char *name)
{
	static char illegal[] = { '\"', '*', '/', ':', '<', '>', '?', '\\', '|', 0 };
	char *p;
	while (*name == ' ' || *name == '.') { name++; }
	if (*name == '\0')
		return NULL;
	for (p = name; *p; p++) {
		char c = *p;
		if (c < 0x20 || strchr(illegal, c)) {
			return 0;
		}
	}
	while (p-- > name) {
		if (*p != ' ') {
			p[1] = '\0';
			break;
		}
	}
	if (p - name + 1 > FAT32_MAX_FILENAME)
		return NULL;
	return name;
}

/**
 * Allocate an entry on disk. If the file already exists, just return it.
 * Caller must hold dir->lock.
 * @param   dir         the directory inode
 * @param   name        the file to allocate on disk
 * @param   mode        dir or file (read/write control is not consider yet)
 * @return              the inode structure of the new file
 */
struct inode *fat_alloc_entry(struct inode *dir, char *name, int mode)
{
	__debug_info("fat_alloc_entry", "allocating %s\n", name);
	if (dir->state & I_STATE_FREE || !(name = formatname(name))) {        // detect illegal character
		return NULL;
	}
	struct fat32_entry *dp = i2fat(dir);
	if (!(dp->attribute & ATTR_DIRECTORY))
		panic("ealloc not dir");

	struct superblock *sb = dir->sb;
	struct inode *ip;
	uint off = 0;
	/* If the file exists on disk. */
	if ((ip = fat_lookup_dir(dir, name, &off)) != NULL) {      // entry exists
		return ip;
	}
	if ((ip = fat_alloc_inode(sb)) == NULL) {
		__debug_warn("fat_alloc_entry", "2\n");
		return NULL;
	}
	ip->op = dir->op;
	ip->fop = dir->fop;
	ip->mode = mode | 0x1ff;

	struct fat32_entry *ep = i2fat(ip);
	ep->attribute = S_ISDIR(mode) ? ATTR_DIRECTORY : 0;
	ep->ent_cnt = (strlen(name) + CHAR_LONG_NAME - 1) / CHAR_LONG_NAME + 1;
	ip->inum = ((uint64)reloc_clus(dir, off, 0) << 32) | (off % sb2fat(sb)->byts_per_clus);

	if (S_ISDIR(mode)) {    // generate "." and ".." for ep
		if ((ep->first_clus = alloc_clus(sb)) == 0 ||
			fat_make_entry(ip, ep, ".          ", 0, 0) < 0 ||
			fat_make_entry(ip, dp, "..         ", 32, 0) < 0)
		{
			if (ep->first_clus > 0)
				free_clus(sb, ep->first_clus);
			goto fail;
		}
	}
	else {  // what is this for?
		ep->attribute |= ATTR_ARCHIVE;
	}

	if (fat_make_entry(dir, ep, name, off, 1) < 0)
		goto fail;

	return ip;

fail:
	__debug_warn("fat_alloc_entry", "fail\n");
	__alert_fs_err("fat_alloc_entry");
	fat_destroy_inode(ip);
	return NULL;
}

/**
 * Remove the entry in its parent directory.
 * The content of the file is removed by 'truncate'.
 */
int fat_remove_entry(struct inode *ip)
{
	if (ip->state & I_STATE_FREE)
		return -ENOENT;
	if (ip->nlink == 0)
		return -ENOENT;
	ip->nlink--;

	struct fat32_sb *fat = sb2fat(ip->sb);
	struct fat32_entry *entry = i2fat(ip);
	uint32 clus = ip->inum >> 32;
	uint32 off = ip->inum & 0xffffffff;

	__debug_info("fat_remove_entry", "clus:%d off:%d entcnt:%d\n", clus, off, entry->ent_cnt);
	uint8 flag = EMPTY_ENTRY;
	for (int i = 0; i < entry->ent_cnt; i++) {
		__debug_info("fat_remove_entry", "name:%s clus:%d off:%d i:%d\n", ip->entry->filename, clus, off, i);
		if (fat_rw_clus(ip->sb, clus, 1, 0, (uint64)&flag, off, 1) != 1)
			goto fail;
		if ((off += 32) >= fat->byts_per_clus && i + 1 < entry->ent_cnt) {
			if ((clus = read_fat(ip->sb, clus)) == 0 || clus >= FAT32_EOC) {
				__debug_info("fat_remove_entry", "read_fat fail! clus=%d\n", clus);
				goto fail;
			}
			off %= fat->byts_per_clus;
			__debug_info("fat_remove_entry", "entries over clus! new_clus:%d new_off:%d\n", clus, off);
		}
	}

	return 0;

fail:
	__alert_fs_err("fat_remove_entry");
	return -EIO;
}

/**
 * Update the inode info to disk. On FAT32, it's actually
 * update the directory entry. We only update filesize
 * and first cluster in our case.
 */
int fat_update_entry(struct inode *ip)
{
	if (!(ip->state & I_STATE_DIRTY))
		return 0;
	if (ip->state & I_STATE_FREE)
		return -1;
	
	struct fat32_sb *fat = sb2fat(ip->sb);
	struct fat32_entry *entry = i2fat(ip);
	uint32 clus = ip->inum >> 32;
	uint32 off = (ip->inum & 0xffffffff) + ((entry->ent_cnt - 1) << 5);
	
	union fat_disk_entry de;

	// The short entry steps over the cluster.
	if (off >= fat->byts_per_clus) {
		clus = read_fat(ip->sb, clus);
		if (clus == 0) { goto fail; }
		off %= fat->byts_per_clus;
	}

	// Read from disk.
	if (fat_rw_clus(ip->sb, clus, 0, 0, (uint64)&de, off, sizeof(de)) != sizeof(de))
		goto fail;
	de.sne.fst_clus_hi = (uint16)(entry->first_clus >> 16);
	de.sne.fst_clus_lo = (uint16)(entry->first_clus & 0xffff);
	de.sne.file_size = entry->file_size;
	// Wirte back.
	if (fat_rw_clus(ip->sb, clus, 1, 0, (uint64)&de, off, sizeof(de)) != sizeof(de))
		goto fail;
	ip->state &= ~I_STATE_DIRTY;
	
	return 0;

fail:
	__alert_fs_err("fat_update_entry");
	return -1;
}

/**
 * Read filename from directory entry.
 * @param   buffer      pointer to the array that stores the name
 * @param   d           pointer to the raw format entry buffer
 */
static void read_entry_name(char *buffer, union fat_disk_entry *d)
{
	if (d->lne.attr == ATTR_LONG_NAME) {                       // long entry branch
		wchar temp[NELEM(d->lne.name1)];
		memmove(temp, d->lne.name1, sizeof(temp));
		snstr(buffer, temp, NELEM(d->lne.name1));
		buffer += NELEM(d->lne.name1);
		snstr(buffer, d->lne.name2, NELEM(d->lne.name2));
		buffer += NELEM(d->lne.name2);
		snstr(buffer, d->lne.name3, NELEM(d->lne.name3));
	} else {
		// assert: only "." and ".." will enter this branch
		memset(buffer, 0, CHAR_SHORT_NAME + 2); // plus '.' and '\0'
		int i;
		for (i = 0; d->sne.name[i] != ' ' && i < 8; i++) {
			buffer[i] = d->sne.name[i];
		}
		if (d->sne.name[8] != ' ') {
			buffer[i++] = '.';
		}
		for (int j = 8; j < CHAR_SHORT_NAME; j++, i++) {
			if (d->sne.name[j] == ' ') { break; }
			buffer[i] = d->sne.name[j];
		}
	}
}

/**
 * Read entry_info from directory entry.
 * @param   entry       pointer to the structure that stores the entry info
 * @param   d           pointer to the raw format entry buffer
 */
static void read_entry_info(struct fat32_entry *entry, union fat_disk_entry *d)
{
	entry->attribute = d->sne.attr;
	entry->first_clus = ((uint32)d->sne.fst_clus_hi << 16) | d->sne.fst_clus_lo;

	entry->create_time_tenth = d->sne.crt_time_tenth;
	entry->create_time = d->sne.crt_time;
	entry->create_date = d->sne.crt_date;
	entry->last_access_date = d->sne.lst_acce_date;
	entry->last_write_time = d->sne.lst_wrt_time;
	entry->last_write_date = d->sne.lst_wrt_date;
	
	entry->file_size = d->sne.file_size;
}

/**
 * Read a directory from off, parse the next entry(ies) associated with one file, or find empty entry slots.
 * Caller must hold dir->lock.
 * @param   dir     the directory inode
 * @param   ep      the struct to be written with info
 * @param   off     offset off the directory
 * @param   count   to write the count of entries
 * @return  -1      meet the end of dir
 *          0       find empty slots
 *          1       find a file with all its entries
 */
int fat_read_entry(struct inode *dir, struct fat32_entry *ep, char *namebuf, uint off, int *count)
{
	if (dir->state & I_STATE_FREE)
		return -1;

	struct fat32_entry *dp = i2fat(dir);
	if (!(dp->attribute & ATTR_DIRECTORY))
		panic("enext not dir");
	if (off % 32)
		panic("enext not align");

	union fat_disk_entry de;
	uint32 const bpc = sb2fat(dir->sb)->byts_per_clus;
	int empty = 0, cnt = 0;
	memset(namebuf, 0, FAT32_MAX_FILENAME + 1);
	for (uint32 clus; (clus = reloc_clus(dir, off, 0)) != 0; off += sizeof(de))
	{
		if (fat_rw_clus(dir->sb, clus, 0, 0, (uint64)&de, off % bpc, sizeof(de)) != sizeof(de)
			|| de.lne.order == END_OF_DIR) {
			return -1;
		}
		if (de.lne.order == EMPTY_ENTRY) {
			if (cnt) {      // We have met a l-n-e, but why we come across an empty one?
				goto excep; // Maybe the entry is being removed by its so-call inode.
			}
			empty++;
			continue;
		} else if (empty) {
			*count = empty;
			return 0;
		}
		if (de.lne.attr == ATTR_LONG_NAME) {
			if (de.lne.order & LAST_LONG_ENTRY) {
				cnt = (de.lne.order & ~LAST_LONG_ENTRY) + 1;   // plus the s-n-e;
				*count = cnt;
			} else if (cnt == 0) {    // Should not occur unless some inode is removing this entry
				goto excep;
			}
			read_entry_name(namebuf + (--cnt - 1) * CHAR_LONG_NAME, &de);
		} else {
			if (cnt == 0) {     // Without l-n-e
				*count = 1;
				read_entry_name(namebuf, &de);
			}
			read_entry_info(ep, &de);
			__debug_info("fat_dir_next", "get %s\n", namebuf);
			return 1;
		}
	}
	return -1;

excep:
	*count = 0;     // Caller will re-read
	__debug_warn("fat_dir_next", "??? from %d\n", off);
	return 0;
}

int fat_rename_entry(struct inode *ip, struct inode *dp, char *name)
{
	if ((name = formatname(name)) == NULL)
		return -EINVAL;

	uint off;
	struct fat32_entry *dst;
	dst = fat_lookup_dir_ent(dp, name, &off);
	if (dst != NULL) { // file exists
		kfree(dst);
		panic("fat_rename_entry"); // vfs should have checked this, what happened?
		return -EEXIST;
	}
	if (fat_make_entry(dp, i2fat(ip), name, off, 1) < 0) {
		return -ENOSPC;
	}
	int ret = fat_remove_entry(ip);
	if (ret < 0) {
		return ret;
	}

	safestrcpy(ip->entry->filename, name, FAT32_MAX_FILENAME + 1); // this is with no lock, dangerous
	ip->nlink++; // fat_remove_entry decreased this, add it back
	uint64 clus = reloc_clus(dp, off, 0);
	ip->inum = (clus << 32) | (off % sb2fat(dp->sb)->byts_per_clus);

	return 0;
}
