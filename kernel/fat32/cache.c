#include <os/fat32.h>
#include <hash.h>
#include <os/stdio.h>

char iobuf[IO_CACHE_BUFSIZ];
uint16 ioseqrec[IO_CACHE_BUFSIZ/2048];
uint8 iodirty[IO_CACHE_BUFSIZ/2048][4];
uint32_t iosecnum[IO_CACHE_BUFSIZ/2048][4];

void init_iocache()
{
    int i;
    for(i = 0; i < IO_CACHE_BUFSIZ/2048; i++)
        ioseqrec[i] = 0xffff;
    memset(iosecnum, 0xff, sizeof(iosecnum));
    memset(iodirty, 0, sizeof(iodirty));
}

/* write as many sectors as we can */
void sd_write_through(char *buf, uint32_t sec)
{
    printk_port("write through\n");
    printk_port("sec: %d", sec);
    for (int i = 0; i < READ_BUF_CNT; ++i)
    {
        sd_write_sector(buf, sec, 1);
        buf += SECTOR_SIZE;
        sec++;
    }    
}

void do_iocache_write_back()
{
    int i, j;
    for(i = 0; i < IO_CACHE_RECNUM; i++)
        for(j = 0; j < 4; j++)
            if(iodirty[i][j] == 1){
                // sd_write_through(iobuf + i * IO_CACHE_BLOCKSIZ + j * IO_CACHE_LINESIZ, iosecnum[i][j]);
                // iodirty[i][j] = 0;
            }
}

/* read sectors from cache */
/* success return 0, fail return 1*/
int sd_read_from_cache(char* buf, uint32_t sec)
{
    uint8 key = hash8(&sec, 32);
    key = key % IO_CACHE_RECNUM;
    int i;
    int hit = -1;
    for(i = 0; i < 4; i++)
        if(iosecnum[key][i] == sec)
            hit = i;
    if(hit == -1)   // not hit!
    {
        sd_read_sector(buf, sec, READ_BUF_CNT);
        if(_1st(ioseqrec[key]) != 0xf) {        // all cache lines are valid, replace the _1st(ioseqrec[key]) line of the cache block
            if(iodirty[key][_1st(ioseqrec[key])] == 1) {        // the line to be replaced is dirty
                sd_write_through(iobuf + key * IO_CACHE_BLOCKSIZ + _1st(ioseqrec[key]) * IO_CACHE_LINESIZ, iosecnum[key][_1st(ioseqrec[key])]);
                iodirty[key][_1st(ioseqrec[key])] = 0;      
            }
            memcpy(iobuf + key * IO_CACHE_BLOCKSIZ + _1st(ioseqrec[key]) * IO_CACHE_LINESIZ, buf, IO_CACHE_LINESIZ);
        } else {        // not all cache lines are valid
            int i, empty = -1;
            // find the empty cache line
            for(i = 0; i < 4; i++)
                if(iosecnum[key][i] == 0xffffffff){
                    empty = i;
                    break;
                }
            assert(empty != -1);
            memcpy(iobuf + key * IO_CACHE_BLOCKSIZ + empty * IO_CACHE_LINESIZ, buf, IO_CACHE_LINESIZ);
            ioseqrec[key] &= 0x0fff;
            ioseqrec[key] |= (empty << 12);     // let _1st(ioseqrec[key]) to be the value of empty
        } 

        // reorder the sequence: 1st->4th, 2st->1st, 3st->2st, 4st->3st
        uint8 temp = _1st(ioseqrec[key]);
        ioseqrec[key] <<= 4;
        ioseqrec[key] |= temp;
        // record the section number
        iosecnum[key][_4th(ioseqrec[key])] = sec;
    } else {    // hit!
        log2(0, "hit is %d, key is %d", hit, key);
        log2(0, "hit sec: %d", iosecnum[key][hit]);
        memcpy(buf, iobuf + key * IO_CACHE_BLOCKSIZ + hit * IO_CACHE_LINESIZ, IO_CACHE_LINESIZ);
    }
    
    return 0;
}

/* write sectors to cache */
/* success return 0, fail return 1*/
int sd_write_to_cache(char *buf, uint32_t sec)
{
    uint8 key = hash8(&sec, 32);
    key = key % IO_CACHE_RECNUM;
    int i;
    int hit = -1;
    for(i = 0; i < 4; i++)
        if(iosecnum[key][i] == sec)
            hit = i;
    if(hit == -1)   // not hit!
    {
        if(_1st(ioseqrec[key]) != 0xf) {        // all cache lines are valid, replace the _1st(ioseqrec[key]) line of the cache block
            if(iodirty[key][_1st(ioseqrec[key])] == 1) {        // the line to be replaced is dirty
                sd_write_through(iobuf + key * IO_CACHE_BLOCKSIZ + _1st(ioseqrec[key]) * IO_CACHE_LINESIZ, iosecnum[key][_1st(ioseqrec[key])]);
                iodirty[key][_1st(ioseqrec[key])] = 0;      
            }
            memcpy(iobuf + key * IO_CACHE_BLOCKSIZ + _1st(ioseqrec[key]) * IO_CACHE_LINESIZ, buf, IO_CACHE_LINESIZ);
            iodirty[key][_1st(ioseqrec[key])] = 1;
        } else {        // not all cache lines are valid
            int i, empty = -1;
            // find the empty cache line
            for(i = 0; i < 4; i++)
                if(iosecnum[key][i] == 0xffffffff){
                    empty = i;
                    break;
                }
            assert(empty != -1);
            memcpy(iobuf + key * IO_CACHE_BLOCKSIZ + empty * IO_CACHE_LINESIZ, buf, IO_CACHE_LINESIZ);
            iodirty[key][empty] = 1;
            ioseqrec[key] &= 0x0fff;
            ioseqrec[key] |= (empty << 12);     // let _1st(ioseqrec[key]) to be the value of empty
        }

        // reorder the sequence: 1st->4th, 2st->1st, 3st->2st, 4st->3st
        uint8 temp = _1st(ioseqrec[key]);
        ioseqrec[key] <<= 4;
        ioseqrec[key] |= temp;
        // record the section number
        iosecnum[key][_4th(ioseqrec[key])] = sec;
    } else {        // hit !
        log2(0, "hit is %d, key is %d", hit, key);
        log2(0, "hit sec: %d", iosecnum[key][hit]);
        memcpy(iobuf + key * IO_CACHE_BLOCKSIZ + hit * IO_CACHE_LINESIZ, buf, IO_CACHE_LINESIZ);
        iodirty[key][hit] = 1;
    }

    return 0;
}
