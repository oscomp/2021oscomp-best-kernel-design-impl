#ifndef __DISK_H
#define __DISK_H

#include "fs/buf.h"

void disk_init(void);
void disk_read(struct buf *b);
void disk_write(struct buf *b, uint sector);
void disk_intr(void);

#endif
