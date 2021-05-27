#ifndef __MMAP_H
#define __MMAP_H

#include "types.h"
#include "memlayout.h"
#include "vm.h"
#include "pm.h"
#include "kmalloc.h"
#include "usrmm.h"
#include "file.h"
#include "proc.h"
#include "string.h"
#include "debug.h"

struct mapped{
	int offset;
	uint64 ph_addr;
	int n_ref;
	struct mapped *next;
};

struct mapped* add_map(struct inode* ip, int off, int flags);
void del_map(struct inode* ip, int off);
uint64 _mmap(uint64 start, int len, int prot, int flags, int fd, int off);

#endif