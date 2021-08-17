#ifndef __BUF_H
#define __BUF_H

#define BSIZE 512
// #define BNUM  500
#define BNUM 	2000

#include "sync/sleeplock.h"
#include "utils/list.h"
#include "utils/dlist.h"


struct buf {
	// The below fields are only modifed
	// when refcnt is 0, protected by
	// @bcachelock. So anyone who has a
	// ref is safe to read them directly.
	uint32				dev;		// device number
	uint32				sectorno;	
	// Protected by @bcachelock.
	uint32				refcnt;
	// Protected by its lock.
	uint16				valid;		// of data
	// Those depend on cases.
	uint8				dirty;		// for disk driver
	uint8				disk;		// for driver, whether on its way to disk
	struct d_list		list;		// can be hanged on different lists
	// Protected by @bcachelock.
	list_node_t			hash;
	
	struct sleeplock	lock;
	uchar				data[BSIZE];
};

void			binit(void);
struct buf*		bget(uint dev, uint sectorno);
struct buf*		bread(uint dev, uint sectorno);
int				breads(struct buf *bufs[], int nbuf);
void			bwrite(struct buf *b);
void			brelse(struct buf *b);
void			bput(struct buf *b);
void			bsync(void);
void			bprint();

#endif
