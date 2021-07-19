#ifndef __BUF_H
#define __BUF_H

#define BSIZE 512
#define BNUM  2000

#include "sleeplock.h"
#include "utils/list.h"

struct buf {
	list_node_t hash;
	int valid;
	int disk;		// does disk "own" buf? 
	uint dev;
	uint sectorno;	// sector number 
	struct sleeplock lock;
	uint refcnt;
	struct buf *prev;
	struct buf *next;
	uchar data[BSIZE];
};

void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
void            bprint();

#endif
