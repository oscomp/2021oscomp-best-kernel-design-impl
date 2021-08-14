#ifndef __DISK_H
#define __DISK_H

#include "fs/buf.h"

void disk_init(void);
int disk_read(struct buf *b);
void disk_write(struct buf *b);
int disk_submit(struct buf *b);
void disk_write_start(void);
void disk_intr(void);

#endif
