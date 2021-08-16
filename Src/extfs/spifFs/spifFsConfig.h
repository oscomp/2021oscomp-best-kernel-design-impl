/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: spifFsConfig.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统配置层
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSCONFIG_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSCONFIG_H_
#define  __SYLIXOS_KERNEL
#define  __SYLIXOS_STDIO
#include "SylixOS.h"
/*********************************************************************************************************
  输出类型声明
*********************************************************************************************************/
// some general signed number
#ifndef _SPIPRIi
#define _SPIPRIi   "%d"             /* 带符号数 */
#endif
// address
#ifndef _SPIPRIad
#define _SPIPRIad  "%08x"           /* 地址 */
#endif
// block
#ifndef _SPIPRIbl
#define _SPIPRIbl  "%04x"           /* Block块号 */
#endif
// page
#ifndef _SPIPRIpg
#define _SPIPRIpg  "%04x"           /* 页号 */
#endif
// span index
#ifndef _SPIPRIsp
#define _SPIPRIsp  "%04x"           /* Span号 */
#endif
// file descriptor
#ifndef _SPIPRIfd
#define _SPIPRIfd  "%d"             /* 文件描述符 */
#endif
// file object id
#ifndef _SPIPRIid
#define _SPIPRIid  "%04x"           /* Object号 */
#endif
// file flags
#ifndef _SPIPRIfl
#define _SPIPRIfl  "%02x"           /* Flag */
#endif

/*********************************************************************************************************
  垃圾回收器 评分估值
  Reference:
    Garbage collecting examines all pages in a block which and sums up
to a block score. Deleted pages normally gives positive score and
used pages normally gives a negative score (as these must be moved).
To have a fair wear-leveling, the erase age is also included in score,
whose factor normally is the most positive.
The larger the score, the more likely it is that the block will
picked for garbage collection.
*********************************************************************************************************/
// Garbage collecting heuristics - weight used for deleted pages.
#ifndef SPIFFS_GC_HEUR_W_DELET
#define SPIFFS_GC_HEUR_W_DELET          (5)             /* 被删除页面提供5分 */
#endif
// Garbage collecting heuristics - weight used for used pages.
#ifndef SPIFFS_GC_HEUR_W_USED
#define SPIFFS_GC_HEUR_W_USED           (-1)            /* 已被占用的也买你提供给-1分 */
#endif
// Garbage collecting heuristics - weight used for time between
// last erased and erase of this block.
#ifndef SPIFFS_GC_HEUR_W_ERASE_AGE
#define SPIFFS_GC_HEUR_W_ERASE_AGE      (50)            /* 年龄提供50分 */
#endif

/*********************************************************************************************************
  Object名的最大长度，注意：名字字符串长度 = SPIFFS_OBJ_NAME_LEN - 1 = 31
*********************************************************************************************************/
#ifndef SPIFFS_OBJ_NAME_LEN
#define SPIFFS_OBJ_NAME_LEN             (32)
#endif
/*********************************************************************************************************
  可分配Buffer大小，越小意味着更多的读写次数，但并不意味着它必须大于逻辑页面大小
  Reference:
    Size of buffer allocated on stack used when copying data.
Lower value generates more read/writes. No meaning having it bigger
than logical page size.
*********************************************************************************************************/
#ifndef SPIFFS_COPY_BUFFER_STACK
#define SPIFFS_COPY_BUFFER_STACK        (64)
#endif

/*********************************************************************************************************
  设备配置
  在SylixOS中我们仅移植Singleton
*********************************************************************************************************/
#ifndef SPIFFS_CFG_PHYS_SZ
#define SPIFFS_CFG_PHYS_SZ(ignore)        (1024 * 1024 * 2)
#endif
#ifndef SPIFFS_CFG_PHYS_ERASE_SZ
#define SPIFFS_CFG_PHYS_ERASE_SZ(ignore)  (65536)
#endif
#ifndef SPIFFS_CFG_PHYS_ADDR
#define SPIFFS_CFG_PHYS_ADDR(ignore)      (0)
#endif
#ifndef SPIFFS_CFG_LOGIC_PAGE_SZ
#define SPIFFS_CFG_LOGIC_PAGE_SZ(ignore)    (256)
#endif
#ifndef SPIFFS_CFG_LOGIC_BLOCK_SZ
#define SPIFFS_CFG_LOGIC_BLOCK_SZ(ignore)   (65536)
#endif
/*********************************************************************************************************
  是否允许永久的文件缓存？
  Reference:
    Enable this to add a temporal file cache using the fd buffer.
The effects of the cache is that SPIFFS_open will find the file faster in
certain cases. It will make it a lot easier for spiffs to find files
opened frequently, reducing number of readings from the spi flash for
finding those files.
This will grow each fd by 6 bytes. If your files are opened in patterns
with a degree of temporal locality, the system is optimized.
Examples can be letting spiffs serve web content, where one file is the css.
The css is accessed for each html file that is opened, meaning it is
accessed almost every second time a file is opened. Another example could be
a log file that is often opened, written, and closed.
The size of the cache is number of given file descriptors, as it piggybacks
on the fd update mechanism. The cache lives in the closed file descriptors.
When closed, the fd know the whereabouts of the file. Instead of forgetting
this, the temporal cache will keep handling updates to that file even if the
fd is closed. If the file is opened again, the location of the file is found
directly. If all available descriptors become opened, all cache memory is
lost.
（与我的预热区思想一样吗？）
*********************************************************************************************************/
#ifndef SPIFFS_TEMPORAL_FD_CACHE
#define SPIFFS_TEMPORAL_FD_CACHE              1
#endif

/*********************************************************************************************************
  命中得分，寻找最常用文件 =-=，这不差不多一样吗
  Reference:
    Temporal file cache hit score. Each time a file is opened, all cached files
will lose one point. If the opened file is found in cache, that entry will
gain SPIFFS_TEMPORAL_CACHE_HIT_SCORE points. One can experiment with this
value for the specific access patterns of the application. However, it must
be between 1 (no gain for hitting a cached entry often) and 255.
be between 1 (no gain for hitting a cached entry often) and 255.
*********************************************************************************************************/
#ifndef SPIFFS_TEMPORAL_CACHE_HIT_SCORE
#define SPIFFS_TEMPORAL_CACHE_HIT_SCORE       4
#endif

/*********************************************************************************************************
  是否允许将所有Index页面映射到内存中
  Reference:
    This allows for faster and more deterministic reading if cases of reading
large files and when changing file offset by seeking around a lot.
When mapping a file's index, the file system will be scanned for index pages
and the info will be put in memory provided by user. When reading, the
memory map can be looked up instead of searching for index pages on the
medium. This way, user can trade memory against performance.
Whole, parts of, or future parts not being written yet can be mapped. The
memory array will be owned by spiffs and updated accordingly during garbage
collecting or when modifying the indices. The latter is invoked by when the
file is modified in some way. The index buffer is tied to the file
descriptor.
*********************************************************************************************************/
#ifndef SPIFFS_EN_IX_MAP
#define SPIFFS_EN_IX_MAP                         0
#endif

/*********************************************************************************************************
  一些可视化设置
*********************************************************************************************************/
#ifndef spiffs_printf
#define spiffs_printf(...)                printf(__VA_ARGS__)
#endif
// spiffs_printf argument for a free page
#ifndef SPIFFS_TEST_VIS_FREE_STR
#define SPIFFS_TEST_VIS_FREE_STR          "_"       /* 代表FreePage */
#endif
// spiffs_printf argument for a deleted page
#ifndef SPIFFS_TEST_VIS_DELE_STR
#define SPIFFS_TEST_VIS_DELE_STR          "/"       /* 代表DeletedPage */
#endif
// spiffs_printf argument for an index page for given object id
#ifndef SPIFFS_TEST_VIS_INDX_STR
#define SPIFFS_TEST_VIS_INDX_STR(id)      "i"       /* 代表IndexPage */
#endif
// spiffs_printf argument for a data page for given object id
#ifndef SPIFFS_TEST_VIS_DATA_STR
#define SPIFFS_TEST_VIS_DATA_STR(id)      "d"       /* 代表DataPage */
#endif

#define SPIFFS_GC_DBG
#define SPIFFS_DBG
#define SPIFFS_CHECK_DBG
#define SPIFFS_API_DBG


#ifndef SPIFFS_DBG
#define SPIFFS_DBG(_f, ...)   printf(_f, ## __VA_ARGS__)
#endif
// Set spiffs debug output call for garbage collecting.
#ifndef SPIFFS_GC_DBG
#define SPIFFS_GC_DBG(_f, ...) printf(_f, ## __VA_ARGS__)
#endif
// Set spiffs debug output call for caching.
#define SPIFFS_CACHE_DBG

#ifndef SPIFFS_CACHE_DBG
#define SPIFFS_CACHE_DBG(_f, ...) printf(_f, ## __VA_ARGS__)
#endif
// Set spiffs debug output call for system consistency checks.
#ifndef SPIFFS_CHECK_DBG
#define SPIFFS_CHECK_DBG(_f, ...) printf(_f, ## __VA_ARGS__)
#endif
// Set spiffs debug output call for all api invocations.
#ifndef SPIFFS_API_DBG
#define SPIFFS_API_DBG(_f, ...) printf(_f, ## __VA_ARGS__)
#endif

/*********************************************************************************************************
  SPIFFS内置类型设置
*********************************************************************************************************/
typedef UINT16 SPIFFS_BLOCK_IX;                     /* Block Index类型 */
typedef UINT16 SPIFFS_PAGE_IX;                      /* Page  Index类型 */
typedef UINT16 SPIFFS_OBJ_ID;                       /* Object   ID类型 */
typedef UINT16 SPIFFS_SPAN_IX;                      /* Span  Index类型 */

#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSCONFIG_H_ */