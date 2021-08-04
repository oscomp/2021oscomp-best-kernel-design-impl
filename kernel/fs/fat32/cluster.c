#ifndef __DEBUG_cluster
#undef DEBUG 
#endif 

#define __module_name__ "fat32"

#include "types.h"
#include "errno.h"
#include "printf.h"
#include "fat32.h"
#include "sync/sleeplock.h"
#include "mm/kmalloc.h"
#include "utils/string.h"
#include "utils/rbtree.h"
#include "utils/debug.h"


uint32 alloc_clus(struct superblock *sb)
{
	// should we keep a free cluster list? instead of searching fat every time.
	struct fat32_sb *fat = sb2fat(sb);
	uint32 fat_sec = fat->bpb.rsvd_sec_cnt;
	uint32 const ent_per_sec = fat->bpb.byts_per_sec / sizeof(uint32);
	
	char *buf = kmalloc(fat->bpb.byts_per_sec);
	if (buf == NULL) {
		return 0;
	}
	uint32 clus = 0;
	acquiresleep(&sb->sb_lock);
	for (uint32 i = 0; i < fat->bpb.fat_sz; i++, fat_sec++) {
		sb->op.read(sb, 0, buf, fat_sec, 0, fat->bpb.byts_per_sec);
		for (uint32 j = 0; j < ent_per_sec; j++) {
			if (((uint32 *)buf)[j] == 0) {
				uint32 flag = FAT32_EOC + 7;
				sb->op.write(sb, 0, (char*)&flag, fat_sec, j * sizeof(uint32), sizeof(uint32));

				clus = i * ent_per_sec + j;
				uint32 sec = first_sec_of_clus(fat, clus);
				sb->op.clear(sb, sec, fat->bpb.sec_per_clus);
				goto end;
			}
		}
	}
end:
	releasesleep(&sb->sb_lock);
	kfree(buf);
	return clus;
	// panic("no clusters");
}

int free_clus(struct superblock *sb, uint32 cluster)
{
	return write_fat(sb, cluster, 0);
}


#define CLUS_TABLE_SIZE 32

struct clus_table {
	struct rb_node rb;	
	uint32 base;
	uint32 num;
	uint32 clus[CLUS_TABLE_SIZE];
};

static inline struct clus_table *rb_clus_table(struct rb_node *node)
{
	return rb_entry(node, struct clus_table, rb);
}

// static struct clus_table *search_clus_table(struct rb_node *node, uint64

static int fill_clus_table(struct superblock *sb, struct clus_table *table, uint32 preclus, int alloc_end_idx)
{
	uint32 clus;
	int i;
	if ((i = table->num) > 0) {		// If table is not empty...
		preclus = table->clus[i - 1];
	} else if (preclus == 0) {		// or take the arg preclus
		panic("fill_clus_table: bad pre-clus");
	}

	for (; i < CLUS_TABLE_SIZE; i++) {
		clus = read_fat(sb, preclus);	// get next cluster number
		// __debug_info("fill_clus_table", "read: i=%d, clus=0x%x, base=%d\n", i, clus, table->base);
		if (clus >= FAT32_EOC) {
			while (i <= alloc_end_idx) {
				if ((clus = alloc_clus(sb)) == 0 || write_fat(sb, preclus, clus) < 0) {
					if (clus)
						write_fat(sb, clus, 0);
					table->num = i;
					return -1;
				}
				preclus = table->clus[i] = clus;
				// __debug_info("fill_clus_table", "write: i=%d, clus=0x%x\n", i, clus);
				i++;
			}
			break;
		} else {
			table->clus[i] = clus;
		}
		preclus = clus;
	}

	// __debug_info("fill_clus_table", "end: i=%d, alloc_end_idx=%d\n", i, alloc_end_idx);
	table->num = i;
	return i;
}

static struct clus_table *expand_rb_clus(struct inode *ip, uint off, int alloc)
{
	struct fat32_entry *entry = i2fat(ip);
	struct fat32_sb *fat = sb2fat(ip->sb);
	struct rb_node *parent = rb_last(&entry->rb_clus);

	uint const clus_cnt = off / fat->byts_per_clus;
	uint const dst_key = clus_cnt / CLUS_TABLE_SIZE;
	int const dst_idx = clus_cnt % CLUS_TABLE_SIZE;

	// __debug_info("expand_rb_clus", "clus_cnt=%d, dstkey=%d, dstidx=%d, parent=%p\n",
	// 			clus_cnt, dst_key, dst_idx, parent);

	struct clus_table *table;
	struct rb_node **p;
	uint base_key;
	uint32 clus;
	int idx;

	// First, locate the last clus-table to get last base and index
	if (parent == NULL) { // no nodes in rb_tree
		table = kmalloc(sizeof(struct clus_table));
		if (table == NULL)
			goto fail;

		memset(table->clus, 0, sizeof(table->clus));
		table->base = 0;
		table->num = 1;
		table->clus[0] = entry->first_clus;
		__debug_info("expand_rb_clus", "first: clus=%d\n", entry->first_clus);
		parent = &table->rb;
		rb_link_node(parent, NULL, &entry->rb_clus.rb_node);
		rb_insert_color(parent, &entry->rb_clus);
	} else {
		table = rb_clus_table(parent);
	}

	if (table->num < CLUS_TABLE_SIZE) {	// step 1: fill the last table
		idx = alloc ? 
				(table->base == dst_key ? dst_idx : CLUS_TABLE_SIZE - 1) : -1;
		
		idx = fill_clus_table(ip->sb, table, 0, idx);
		if (idx < 0)						// case: allocation fail
			goto fail;
		else if (idx < CLUS_TABLE_SIZE)		// case: meet end of fat chain
			return table;
	}

	base_key = table->base + 1;
	clus = table->clus[CLUS_TABLE_SIZE - 1];
	p = &parent->rb_right;

	while (base_key <= dst_key) {	// allocate new table
		table = kmalloc(sizeof(struct clus_table));
		if (table == NULL)
			goto fail;

		table->base = base_key;
		memset(table->clus, 0, sizeof(table->clus));
		table->num = 0;

		// __debug_info("expand_rb_clus", "newtable: base=%d, pre-clus=0x%x\n", base_key, clus);
		idx = alloc ? 
				(table->base == dst_key ? dst_idx : CLUS_TABLE_SIZE - 1) : -1;
		idx = fill_clus_table(ip->sb, table, clus, idx);
		if (idx < 0) {						// case: allocation fail
			if (table->num == 0)
				kfree(table);
			goto fail;
		} else if (idx == 0) {				// case: meet end of fat chain
			kfree(table);					// don't keep empty table
			return rb_clus_table(parent);
		}

		rb_link_node(&table->rb, parent, p);
		rb_insert_color(&table->rb, &entry->rb_clus);

		if (table->num < CLUS_TABLE_SIZE)	// not full, meet end of fat chain
			break;

		base_key++;
		clus = table->clus[CLUS_TABLE_SIZE - 1];
		parent = &table->rb;		// table is now the right-most node
		p = &table->rb.rb_right;	// the next node is the next right-most one
	}

	return table;

fail: // If fail, no need to free tables that have been allocated (for future use).
	__debug_warn("expand_rb_clus", "fail\n");
	return NULL;
}


/**
 * for the given entry, relocate the cur_clus field based on the off
 * @param   ip          to get its fat32_entry and modify the cur_clus field
 * @param   off         the offset from the beginning of the relative file
 * @param   alloc       whether alloc new cluster when meeting end of FAT chains
 * @return              the next cluster number
 */
uint32 reloc_clus(struct inode *ip, uint off, int alloc)
{
	struct fat32_entry *entry = i2fat(ip);
	struct fat32_sb *fat = sb2fat(ip->sb);
	uint const clus_cnt = off / fat->byts_per_clus;
	uint const rb_key = clus_cnt / CLUS_TABLE_SIZE;

	struct rb_node *node = entry->rb_clus.rb_node;
	struct clus_table *table = entry->cur_clus;

	// __debug_info("reloc_clus", "begin: node=%p\n", node);
	if (!table || table->base != rb_key) {
		while (node) {
			// __debug_info("reloc_clus", "search: node=%p\n", node);
			table = rb_clus_table(node);
			if (rb_key < table->base)
				node = node->rb_left;
			else if (rb_key > table->base)
				node = node->rb_right;
			else
				break;
		}
	}

	int const idx = clus_cnt % CLUS_TABLE_SIZE;
	if (!node) {
		table = expand_rb_clus(ip, off, alloc);
		// __debug_info("reloc_clus", "get expand, base=%d, off=%p\n", table ? table->base : -1, off);
		if (table == NULL || table->base != rb_key)
			goto fail;
	} else if (!table->clus[idx]) {
		// __debug_info("reloc_clus", "get table, base=%d, off=%p\n", table->base, off);
		if (!alloc)
			goto fail;
		if (fill_clus_table(ip->sb, table, 0, idx) < 0)
			goto fail;
	}

	// __debug_info("reloc_clus", "end: clus=%d, off=%d\n", entry->cur_clus, off);
	entry->cur_clus = table;
	return table->clus[idx];

fail:
	return 0;
}

void free_clus_cache(struct fat32_entry *entry)
{
	struct rb_node *node = entry->rb_clus.rb_node;
	struct rb_node *parent = NULL;
	struct clus_table *table;

	while (node != NULL) {
		if (node->rb_left)
			node = node->rb_left;
		else if (node->rb_right)
			node = node->rb_right;
		else {
			parent = rb_parent(node);
			if (parent) {
				if (node == parent->rb_left)
					parent->rb_left = NULL;
				else
					parent->rb_right = NULL;
			}

			table = rb_clus_table(node);
			__debug_info("free_clus_cache", "base=%d, num=%d\n", table->base, table->num);
			kfree(table);
			node = parent;
		}
	}

	entry->rb_clus.rb_node = NULL;
}
