#ifndef __BUF_H
#define __BUF_H

#define BSIZE 512
#define BNUM  500

#define BWRITE_BACK		0
#define BWRITE_THROUGH	1

#include "sync/sleeplock.h"
#include "utils/list.h"
#include "utils/dlist.h"


struct buf {
	int valid;
	int disk;		// does disk "own" buf? 
	uint dev;
	uint sectorno;	// sector number 
	uint refcnt;
	uint dirty;
	struct sleeplock lock;
	list_node_t hash;
	struct d_list lru;
	uchar data[BSIZE];
};

void            binit(void);
struct buf*		bget(uint, uint);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void			bwrite(struct buf *, int through);
void			bsync(void);
void			bclean(struct buf *b);
struct buf*		bdirty(void);
void			bhalt(struct buf *b);
void            bprint();

#endif
