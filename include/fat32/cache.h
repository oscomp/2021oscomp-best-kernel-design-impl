#ifndef INCLUDE_FAT32_CACHE_H
#define INCLUDE_FAT32_CACHE_H

#include <type.h>

/* data cache */
#define _1st(a) (((a) & 0xf000lu) >> 12)
#define _2nd(a) (((a) & 0x0f00lu) >> 8)
#define _3rd(a) (((a) & 0x00f0lu) >> 4)
#define _4th(a) (((a) & 0x000flu))
#define IO_CACHE_BUFSIZ (1ul << 16)
#define IO_CACHE_LINESIZ NORMAL_PAGE_SIZE
#define IO_CACHE_BLOCKNUM (IO_CACHE_BUFSIZ/IO_CACHE_LINESIZ)
#define IO_CACHE_WAYNUM 4       // 4路组相联
#define IO_CACHE_BLOCKSIZ (IO_CACHE_WAYNUM*IO_CACHE_LINESIZ)
#define IO_CACHE_RECNUM (IO_CACHE_BUFSIZ/(IO_CACHE_WAYNUM*IO_CACHE_LINESIZ))

extern unsigned char iobuf[IO_CACHE_BUFSIZ];
extern uint16 ioseqrec[IO_CACHE_RECNUM];
extern uint8 iodirty[IO_CACHE_RECNUM][IO_CACHE_WAYNUM];
extern uint32_t iosecnum[IO_CACHE_RECNUM][IO_CACHE_WAYNUM];

extern void do_iocache_write_back();
void init_iocache();
int sd_write_to_cache(char *buf, uint32_t sec);
int sd_read_from_cache(char* buf, uint32_t sec);

/* meta data cache */

#define IO_META_CACHE_BUFSIZ (1ul << 15)
#define IO_META_CACHE_LINESIZ (512)
#define IO_META_CACHE_BLOCKNUM (IO_META_CACHE_BUFSIZ/IO_META_CACHE_LINESIZ)
#define IO_META_CACHE_WAYNUM 4      // 4路组相联
#define IO_META_CACHE_BLOCKSIZ (IO_META_CACHE_WAYNUM*IO_META_CACHE_LINESIZ)
#define IO_META_CACHE_RECNUM (IO_META_CACHE_BUFSIZ/(IO_META_CACHE_WAYNUM*IO_META_CACHE_LINESIZ))

extern unsigned char metabuf[IO_META_CACHE_BUFSIZ];
extern uint16 metaseqrec[IO_META_CACHE_RECNUM];
extern uint8 metadirty[IO_META_CACHE_RECNUM][IO_META_CACHE_WAYNUM];
extern uint32_t metasecnum[IO_META_CACHE_RECNUM][IO_META_CACHE_WAYNUM];

void init_metacache();
void do_meta_cache_write_back();
int sd_read_from_meta_cache(char* buf, isec_t sec);
int sd_write_to_meta_cache(char *buf, isec_t sec);

#endif