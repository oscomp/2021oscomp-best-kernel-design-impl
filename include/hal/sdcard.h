#ifndef __SDCARD_H
#define __SDCARD_H

#include "types.h"

void sdcard_init(void);

void sdcard_read_sector(uint8 *buf, int sectorno);

void sdcard_write_sector(uint8 *buf, int sectorno);

void test_sdcard(void);

int sdcard_read(struct buf *b);
int sdcard_submit(struct buf *b);
void sdcard_write_start(void);
void sdcard_intr(void);

#endif 
