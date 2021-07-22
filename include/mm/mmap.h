#ifndef __MMAP_H
#define __MMAP_H

#include "types.h"
#include "fs/fs.h"
#include "fs/file.h"
#include "mm/usrmm.h"

#define PROT_NONE		0
#define PROT_READ		1
#define PROT_WRITE		2
#define PROT_EXEC		4
#define PROT_GROWSDOWN	0x01000000
#define PROT_GROWSUP	0x02000000
#define PROT_ALL (PROT_READ|PROT_WRITE|PROT_EXEC|PROT_GROWSDOWN|PROT_GROWSUP)

#define MAP_FILE		0
#define MAP_SHARED		0x01
#define MAP_PRIVATE		0x02
#define MAP_FIXED		0x10 
#define MAP_ANONYMOUS	0x20
#define MAP_FAILED ((void *) -1)

struct mapped {
	uint32 offset;
	uint16 len;
	uint16 n_ref;
	uint64 ph_addr;
	struct mapped *next;
	struct mapped **pprev;
};

void del_map(struct mapped *p, int off, int npages);
void del_segmap(struct seg *seg);
void dup_segmap(struct seg *seg);
uint64 do_mmap(uint64 start, uint64 len, int prot, int flags, struct file *f, int64 off);
int do_munmap(uint64 start, uint64 len);

#endif
