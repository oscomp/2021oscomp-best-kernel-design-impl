#include <os/fat32.h>
#include <hash.h>
#include <os/stdio.h>

unsigned char metabuf[IO_META_CACHE_BUFSIZ];
uint16 metaseqrec[IO_META_CACHE_RECNUM];
uint8 metadirty[IO_META_CACHE_RECNUM][IO_META_CACHE_WAYNUM];
uint32_t metasecnum[IO_META_CACHE_RECNUM][IO_META_CACHE_WAYNUM];

void init_metacache()		// call this function in fat32_init
{
    for(int i = 0; i < IO_META_CACHE_RECNUM; i++){
        metaseqrec[i] = 0xffff;
        for (int j = 0; j < IO_META_CACHE_WAYNUM; j++){
            metadirty[i][j] = 0;
            metasecnum[i][j] = 0xffffffff;
        }
    }
}

/* write as many sectors as we can */
static void sd_write_through_meta(char *buf, uint32_t sec)
{
    // printk_port("write through\n");
    // printk_port("sec: %d\n", sec);
    sd_write_sector(buf, sec, 1);
}

void do_meta_cache_write_back()
{
    int i, j;
    for(i = 0; i < IO_META_CACHE_RECNUM; i++)
        for(j = 0; j < 4; j++)
            if(metadirty[i][j] == 1){
                sd_write_through_meta(metabuf + i * IO_META_CACHE_BLOCKSIZ + j * IO_META_CACHE_LINESIZ, metasecnum[i][j]);
                metadirty[i][j] = 0;
            }
}

/* read sectors from cache */
/* success return 0, fail return 1*/
int sd_read_from_meta_cache(char* buf, isec_t sec)
{
    uint8 key = hash8(&sec, sizeof(isec_t));
    key = key % IO_META_CACHE_RECNUM;
    int i;
    int hit = -1;
    for(i = 0; i < 4; i++)
        if(metasecnum[key][i] == sec)
		{
            hit = i;
			break;
		}
    if(hit == -1)   // not hit!
    {
        sd_read_sector(buf, sec, 1);
        if(_1st(metaseqrec[key]) != 0xf) {        // all cache lines are valid, replace the _1st(metaseqrec[key]) line of the cache block
            if(metadirty[key][_1st(metaseqrec[key])] == 1) {        // the line to be replaced is dirty
                sd_write_through_meta(metabuf + key * IO_META_CACHE_BLOCKSIZ + _1st(metaseqrec[key]) * IO_META_CACHE_LINESIZ, metasecnum[key][_1st(metaseqrec[key])]);
                metadirty[key][_1st(metaseqrec[key])] = 0;      
            }
            memcpy(metabuf + key * IO_META_CACHE_BLOCKSIZ + _1st(metaseqrec[key]) * IO_META_CACHE_LINESIZ, buf, IO_META_CACHE_LINESIZ);
        } else {        // not all cache lines are valid
            int i, empty = -1;
            // find the empty cache line
            for(i = 0; i < 4; i++)
                if(metasecnum[key][i] == 0xffffffff){
                    empty = i;
                    break;
                }
            assert(empty != -1);
            memcpy(metabuf + key * IO_META_CACHE_BLOCKSIZ + empty * IO_META_CACHE_LINESIZ, buf, IO_META_CACHE_LINESIZ);
            metaseqrec[key] &= 0x0fff;
            metaseqrec[key] |= (empty << 12);     // let _1st(metaseqrec[key]) to be the value of empty
        } 

        // reorder the sequence: 1st->4th, 2st->1st, 3st->2st, 4st->3st
        uint8 temp = _1st(metaseqrec[key]);
        metaseqrec[key] <<= 4;
        metaseqrec[key] |= temp;
        // record the section number
        metasecnum[key][_4th(metaseqrec[key])] = sec;
        // log2(0, "read sec is %d", sec);
    } else {    // hit!
        memcpy(buf, metabuf + key * IO_META_CACHE_BLOCKSIZ + hit * IO_META_CACHE_LINESIZ, IO_META_CACHE_LINESIZ);
		
		// renew the sequence: hit->4th, others left shift
		if(_1st(metaseqrec[key]) == hit) {
			uint8 temp = _1st(metaseqrec[key]);
			metaseqrec[key] <<= 4;
			metaseqrec[key] |= temp;
		} else if(_2nd(metaseqrec[key]) == hit) {
			uint8 temp = _2nd(metaseqrec[key]);
			metaseqrec[key] = ((metaseqrec[key] & 0x00ff) << 4) | (metaseqrec[key] & 0xf000) | temp;
		} else if(_3rd(metaseqrec[key]) == hit) {
			uint8 temp = _3rd(metaseqrec[key]);
			metaseqrec[key] = ((metaseqrec[key] & 0x000f) << 4) | (metaseqrec[key] & 0xff00) | temp;
		} else 
			;		// no need to modify sequence 
        
    }
    
    return 0;
}

/* write sectors to cache */
/* success return 0, fail return 1*/
int sd_write_to_meta_cache(char *buf, isec_t sec)
{
    uint8 key = hash8(&sec, sizeof(isec_t));
    key = key % IO_META_CACHE_RECNUM;
    int i;
    int hit = -1;
    for(i = 0; i < IO_META_CACHE_WAYNUM; i++)
        if(metasecnum[key][i] == sec)
		{
			hit = i;
			break;
		}
    if(hit == -1)   // not hit!
    {
        if(_1st(metaseqrec[key]) != 0xf) {        // all cache lines are valid, replace the _1st(metaseqrec[key]) line of the cache block
            if(metadirty[key][_1st(metaseqrec[key])] == 1) {        // the line to be replaced is dirty
                // log2(0, "must write sec: %d, first sec is %d", metasecnum[key][_1st(metaseqrec[key])], fat.first_data_sec);
                sd_write_through_meta(metabuf + key * IO_META_CACHE_BLOCKSIZ + _1st(metaseqrec[key]) * IO_META_CACHE_LINESIZ, metasecnum[key][_1st(metaseqrec[key])]);
                metadirty[key][_1st(metaseqrec[key])] = 0;      
            }
            memcpy(metabuf + key * IO_META_CACHE_BLOCKSIZ + _1st(metaseqrec[key]) * IO_META_CACHE_LINESIZ, buf, IO_META_CACHE_LINESIZ);
            metadirty[key][_1st(metaseqrec[key])] = 1;
        } else {        // not all cache lines are valid
            int i, empty = -1;
            // find the empty cache line
            for(i = 0; i < 4; i++)
                if(metasecnum[key][i] == 0xffffffff){
                    empty = i;
                    break;
                }
            assert(empty != -1);
            memcpy(metabuf + key * IO_META_CACHE_BLOCKSIZ + empty * IO_META_CACHE_LINESIZ, buf, IO_META_CACHE_LINESIZ);
            metadirty[key][empty] = 1;
            metaseqrec[key] &= 0x0fff;
            metaseqrec[key] |= (empty << 12);     // let _1st(metaseqrec[key]) to be the value of empty
        }

        // reorder the sequence: 1st->4th, 2st->1st, 3st->2st, 4st->3st
        uint8 temp = _1st(metaseqrec[key]);
        metaseqrec[key] <<= 4;
        metaseqrec[key] |= temp;
        // record the section number
        metasecnum[key][_4th(metaseqrec[key])] = sec;
    } else {        // hit !
        // log2(0, "write hit is %d, key is %d", hit, key);
        // log2(0, "write hit sec: %d, first sec is %d", metasecnum[key][hit], fat.first_data_sec);
        memcpy(metabuf + key * IO_META_CACHE_BLOCKSIZ + hit * IO_META_CACHE_LINESIZ, buf, IO_META_CACHE_LINESIZ);
        metadirty[key][hit] = 1;

		// renew the sequence: hit->4th, others left shift
		if(_1st(metaseqrec[key]) == hit) {
			uint8 temp = _1st(metaseqrec[key]);
			metaseqrec[key] <<= 4;
			metaseqrec[key] |= temp;
		} else if(_2nd(metaseqrec[key]) == hit) {
			uint8 temp = _2nd(metaseqrec[key]);
			metaseqrec[key] = ((metaseqrec[key] & 0x00ff) << 4) | (metaseqrec[key] & 0xf000) | temp;
		} else if(_3rd(metaseqrec[key]) == hit) {
			uint8 temp = _3rd(metaseqrec[key]);
			metaseqrec[key] = ((metaseqrec[key] & 0x000f) << 4) | (metaseqrec[key] & 0xff00) | temp;
		} else 
			;		// no need to modify sequence 
        // reorder sequence
    }

    return 0;
}
