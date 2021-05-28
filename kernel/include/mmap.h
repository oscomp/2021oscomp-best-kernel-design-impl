#ifndef __MMAP_H
#define __MMAP_H

#include "types.h"
#include "fs.h"
#include "file.h"

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define PROT_GROWSDOWN 0X01000000
#define PROT_GROWSUP 0X02000000

#define MAP_FILE 0
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0X02
#define MAP_FAILED ((void *) -1)

struct mapped {
	uint32 offset;
	uint16 len;
	uint16 n_ref;
	uint64 ph_addr;
	struct mapped *next;
	struct mapped **pprev;
};

// struct mapped* add_map(struct inode* ind, int off, int flags);
struct mapped* add_map(struct mapped **phead, int off, int share);
// void del_map(struct inode* ind, int off);
void del_map(struct mapped *p, int off, int npages);
uint64 do_mmap(uint64 start, int len, int prot, int flags, struct file *f, int off);
int do_munmap(uint64 start, uint64 len);

#endif
