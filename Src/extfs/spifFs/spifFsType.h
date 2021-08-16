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
** 文   件   名: spifFsType.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 01日
**
** 描        述: Spiffs文件系统类型
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_SPIFFS_SPIFFSTYPE_H_
#define SYLIXOS_EXTFS_SPIFFS_SPIFFSTYPE_H_
#include "spifFsConfig.h"
/*********************************************************************************************************
 * 相关宏定义
*********************************************************************************************************/
#define SPIFFS_OK                       0
#define SPIFFS_ERR_NOT_MOUNTED          -10000
#define SPIFFS_ERR_FULL                 -10001
#define SPIFFS_ERR_NOT_FOUND            -10002
#define SPIFFS_ERR_END_OF_OBJECT        -10003
#define SPIFFS_ERR_DELETED              -10004
#define SPIFFS_ERR_NOT_FINALIZED        -10005
#define SPIFFS_ERR_NOT_INDEX            -10006
#define SPIFFS_ERR_OUT_OF_FILE_DESCS    -10007
#define SPIFFS_ERR_FILE_CLOSED          -10008
#define SPIFFS_ERR_FILE_DELETED         -10009
#define SPIFFS_ERR_BAD_DESCRIPTOR       -10010
#define SPIFFS_ERR_IS_INDEX             -10011
#define SPIFFS_ERR_IS_FREE              -10012
#define SPIFFS_ERR_INDEX_SPAN_MISMATCH  -10013
#define SPIFFS_ERR_DATA_SPAN_MISMATCH   -10014
#define SPIFFS_ERR_INDEX_REF_FREE       -10015
#define SPIFFS_ERR_INDEX_REF_LU         -10016
#define SPIFFS_ERR_INDEX_REF_INVALID    -10017
#define SPIFFS_ERR_INDEX_FREE           -10018
#define SPIFFS_ERR_INDEX_LU             -10019
#define SPIFFS_ERR_INDEX_INVALID        -10020
#define SPIFFS_ERR_NOT_WRITABLE         -10021
#define SPIFFS_ERR_NOT_READABLE         -10022
#define SPIFFS_ERR_CONFLICTING_NAME     -10023
#define SPIFFS_ERR_NOT_CONFIGURED       -10024

#define SPIFFS_ERR_NOT_A_FS             -10025
#define SPIFFS_ERR_MOUNTED              -10026
#define SPIFFS_ERR_ERASE_FAIL           -10027
#define SPIFFS_ERR_MAGIC_NOT_POSSIBLE   -10028

#define SPIFFS_ERR_NO_DELETED_BLOCKS    -10029

#define SPIFFS_ERR_FILE_EXISTS          -10030

#define SPIFFS_ERR_NOT_A_FILE           -10031
#define SPIFFS_ERR_RO_NOT_IMPL          -10032
#define SPIFFS_ERR_RO_ABORTED_OPERATION -10033
#define SPIFFS_ERR_PROBE_TOO_FEW_BLOCKS -10034
#define SPIFFS_ERR_PROBE_NOT_A_FS       -10035
#define SPIFFS_ERR_NAME_TOO_LONG        -10036

#define SPIFFS_ERR_IX_MAP_UNMAPPED      -10037
#define SPIFFS_ERR_IX_MAP_MAPPED        -10038
#define SPIFFS_ERR_IX_MAP_BAD_RANGE     -10039

#define SPIFFS_ERR_SEEK_BOUNDS          -10040


#define SPIFFS_ERR_INTERNAL             -10050

#define SPIFFS_ERR_TEST                 -10100

#define SPIFFS_ERR_CACHE_NO_INIT        -10200
#define SPIFFS_ERR_CACHE_NO_MEM         -10201
#define SPIFFS_ERR_CACHE_OVER_RD        -10202
/*********************************************************************************************************
 * 一些标志位
*********************************************************************************************************/
#define SPIFFS_OBJ_ID_IX_FLAG           ((SPIFFS_OBJ_ID)(1UL<<(8*sizeof(SPIFFS_OBJ_ID)-1)))

#define SPIFFS_OBJ_ID_DELETED           ((SPIFFS_OBJ_ID)0)
#define SPIFFS_OBJ_ID_FREE              ((SPIFFS_OBJ_ID)-1)         /* Norflash干净的时候均为1 */

/*********************************************************************************************************
 * 一些操作标志宏
*********************************************************************************************************/
#define SPIFFS_OP_T_OBJ_LU    (0<<0)    /* 00 */
#define SPIFFS_OP_T_OBJ_LU2   (1<<0)    /* 01 不缓存读取*/
#define SPIFFS_OP_T_OBJ_IX    (2<<0)    /* 10 */
#define SPIFFS_OP_T_OBJ_DA    (3<<0)    /* 11 */
#define SPIFFS_OP_TYPE_MASK   (3<<0)    /* 11 */

#define SPIFFS_OP_C_DELE      (0<<2)    /* 000/00 */
#define SPIFFS_OP_C_UPDT      (1<<2)    /* 001/00 */
#define SPIFFS_OP_C_MOVS      (2<<2)    /* 010/00 Move Src*/
#define SPIFFS_OP_C_MOVD      (3<<2)    /* 011/00 Move To Dest*/
#define SPIFFS_OP_C_FLSH      (4<<2)    /* 100/00 */
#define SPIFFS_OP_C_READ      (5<<2)    /* 101/00 */
#define SPIFFS_OP_C_WRTHRU    (6<<2)    /* 110/00 */
#define SPIFFS_OP_COM_MASK    (7<<2)    /* 111/00 */

/*********************************************************************************************************
 * 页面标志位
*********************************************************************************************************/
// if 0, this page is written to, else clean
#define SPIFFS_PH_FLAG_USED   (1<<0)    /* PH 代表 pageHeader */
// if 0, writing is finalized, else under modification
#define SPIFFS_PH_FLAG_FINAL  (1<<1)
// if 0, this is an index page, else a data page
#define SPIFFS_PH_FLAG_INDEX  (1<<2)
// if 0, page is deleted, else valid
#define SPIFFS_PH_FLAG_DELET  (1<<7)
// if 0, this index header is being deleted
#define SPIFFS_PH_FLAG_IXDELE (1<<6)
/*********************************************************************************************************
 * SPIFFS文件操作类型
*********************************************************************************************************/
/* Any write to the filehandle is appended to end of the file */
#define SPIFFS_APPEND                   (1<<0)
#define SPIFFS_O_APPEND                 SPIFFS_APPEND
/* If the opened file exists, it will be truncated to zero length before opened */
#define SPIFFS_TRUNC                    (1<<1)
#define SPIFFS_O_TRUNC                  SPIFFS_TRUNC
/* If the opened file does not exist, it will be created before opened */
#define SPIFFS_CREAT                    (1<<2)
#define SPIFFS_O_CREAT                  SPIFFS_CREAT
/* The opened file may only be read */
#define SPIFFS_RDONLY                   (1<<3)
#define SPIFFS_O_RDONLY                 SPIFFS_RDONLY
/* The opened file may only be written */
#define SPIFFS_WRONLY                   (1<<4)
#define SPIFFS_O_WRONLY                 SPIFFS_WRONLY
/* The opened file may be both read and written */
#define SPIFFS_RDWR                     (SPIFFS_RDONLY | SPIFFS_WRONLY)
#define SPIFFS_O_RDWR                   SPIFFS_RDWR
/* Any writes to the filehandle will never be cached but flushed directly */
#define SPIFFS_DIRECT                   (1<<5)
#define SPIFFS_O_DIRECT                 SPIFFS_DIRECT
/* If SPIFFS_O_CREAT and SPIFFS_O_EXCL are set, SPIFFS_open() shall fail if the file exists */
#define SPIFFS_EXCL                     (1<<6)
#define SPIFFS_O_EXCL                   SPIFFS_EXCL

#define SPIFFS_SEEK_SET                 (0)
#define SPIFFS_SEEK_CUR                 (1)
#define SPIFFS_SEEK_END                 (2)

#define SPIFFS_TYPE_FILE                (1)
#define SPIFFS_TYPE_DIR                 (2)
#define SPIFFS_TYPE_HARD_LINK           (3)
#define SPIFFS_TYPE_SOFT_LINK           (4)
/*********************************************************************************************************
 * SPIFFS基本数据类型
*********************************************************************************************************/
typedef INT16   SPIFFS_FILE;        /* SPIFFS文件，必须为带符号 */
typedef UINT16  SPIFFS_FLAGS;       /* SPIFFS标志位 */
typedef UINT16  SPIFFS_MODE;        /* SPIFFS文件类型 */
typedef UINT8   SPIFFS_OBJ_TYPE;    //TODO: SPIFFS Object类型，可为File，Index，Data等 ?????

struct spiffs_volume;
struct spiffs_config;

/* 一些函数指针 */
/* spi read call function type */
typedef INT32 (*spiffsRead)(UINT32 uiAddr, UINT32 uiSize, PUCHAR pucDst);
/* spi write call function type */
typedef INT32 (*spiffsWrite)(UINT32 uiAddr, UINT32 uiSize, PUCHAR pucSrc);
/* spi erase call function type */
typedef INT32 (*spiffsErase)(UINT32 uiAddr, UINT32 uiSize);

/* 文件系统定义类型检查 */
typedef enum spiffs_check_type
{
    SPIFFS_CHECK_LOOKUP = 0,
    SPIFFS_CHECK_INDEX,
    SPIFFS_CHECK_PAGE
} SPIFFS_CHECK_TYPE;

/* 文件系统检查返回值 */
typedef enum spiffs_check_report{
    SPIFFS_CHECK_PROGRESS = 0,
    SPIFFS_CHECK_ERROR,
    SPIFFS_CHECK_FIX_INDEX,
    SPIFFS_CHECK_FIX_LOOKUP,
    SPIFFS_CHECK_DELETE_ORPHANED_INDEX,
    SPIFFS_CHECK_DELETE_PAGE,
    SPIFFS_CHECK_DELETE_BAD_FILE
} SPIFFS_CHECK_REPORT;

typedef enum spiffs_fileop_type{
    /* the file has been created */
    SPIFFS_CB_CREATED = 0,
    /* the file has been updated or moved to another page */
    SPIFFS_CB_UPDATED,
    /* the file has been deleted */
    SPIFFS_CB_DELETED
} SPIFFS_FILEOP_TYPE;

typedef VOID (*spiffsCheckCallback)(SPIFFS_CHECK_TYPE type, SPIFFS_CHECK_REPORT report,
                                    UINT32 arg1, UINT32 arg2);

typedef VOID (*spiffsFileCallback)(struct spiffs_volume* pfs, SPIFFS_FILEOP_TYPE op, 
                                   SPIFFS_OBJ_ID objId, SPIFFS_PAGE_IX pageIX);

/*********************************************************************************************************
 * SPIFFS配置信息
*********************************************************************************************************/
typedef struct spiffs_config{
    // physical read function
    spiffsRead halReadFunc;             /* 硬件读函数 */
    // physical write function
    spiffsWrite halWriteFunc;           /* 硬件写函数 */
    // physical erase function
    spiffsErase halEraseFunc;           /* 硬件擦函数 */
    // physical size of the spi flash
    UINT32 uiPhysSize;                  /* 物理硬件大小 */
    // physical offset in spi flash used for spiffs,
    // must be on blkIX boundary
    UINT32 uiPhysAddr;                  /* 物理硬件起始偏移 */
    // physical size when erasing a blkIX
    UINT32 uiPhysEraseBlkSize;          /* 物理硬件擦除块大小 */
    // logical size of a blkIX, must be on physical
    // blkIX size boundary and must never be less than
    // a physical blkIX
    UINT32 uiLogicBlkSize;              /* 逻辑块大小 */
    // logical size of a page, must be at least
    // log_block_size / 8
    UINT32 uiLogicPageSize;             /* 逻辑页面大小 */
} SPIFFS_CONFIG;
typedef SPIFFS_CONFIG * PSPIFFS_CONFIG;
/*********************************************************************************************************
 * SPIFFS文件头等信息定义
*********************************************************************************************************/
typedef struct spiffs_volume
{
    // file system configuration
    SPIFFS_CONFIG       cfg;                    /* 配置文件 */
    // number of logical blocks
    LW_OBJECT_HANDLE    hVolLock;               /*  卷操作锁                    */
    UINT32              uiBlkCount;                  /* 逻辑块个数 */
    // cursor for free blocks, blkIX index
    SPIFFS_BLOCK_IX     blkIXFreeCursor;    /* Free Block的索引 */
    // cursor for free blocks, iEntry index (lu => lookup)
    INT                 objLookupEntryFreeCursor;       /* Free Block的LookUp Entry索引 */
    // cursor when searching, blkIX index
    SPIFFS_BLOCK_IX     blkIXCursor;        /* Block遍历索引 */
    // cursor when searching, iEntry index
    INT                 objLookupEntryCursor;           /* Lookup Entry遍历索引 */

    // primary work buffer, size of a logical page
    PUCHAR              pucLookupWorkBuffer;         /* 缓存LookUp Entry，大小为一个逻辑页面 */
    // secondary work buffer, size of a logical page
    PUCHAR              pucWorkBuffer;               /* 缓存各种页面，大小为一个逻辑页面 */
    // file descriptor memory area
    PUCHAR              pucFdSpace;                  /* 文件描述符存储位置 */
    // available file descriptors
    UINT32              uiFdCount;                   /* 可用的文件描述符 */

    // last error
    INT32               uiErrorCode;                  /* SPIFFS上一个错误码 */

    // current number of free blocks
    UINT32              uiFreeBlks;                  /* 空闲块 */
    // current number of busy pages
    UINT32              uiStatsPageAllocated;        /* 当前已分配的页面数 */
    // current number of deleted pages
    UINT32              uiStatsPageDeleted;          /* 当前已删除的页面数 */
    // flag indicating that garbage collector is cleaning
    UINT8               uiCleaningFlag;               /* 标志干净，GC使用? */
    // max erase count amongst all blocks
    SPIFFS_OBJ_ID       uiMaxEraseCount;      /* 所有Block中最大的EraseCount */
    //TODO:干嘛的？
    UINT32              uiStatsGCRuns;               //! ???
    
    // cache memory
    PVOID               pCache;                       /* Cache内存空间 */           
    // cache size
    UINT32              uiCacheSize;                 /* Cache大小 */
    UINT32              uiCacheHits;                 /* Cache命中次数 */
    UINT32              uiCacheMisses;               /* Cache缺失次数 */

    //TODO:暂时没发现有什么用
    // check callback function       
    spiffsCheckCallback checkCallbackFunc;
    // file callback function
    spiffsFileCallback  fileCallbackFunc;
    
    // mounted flag
    UINT8               uiMountedFlag;                /* 是否挂载？ */
    // user data
    //TODO:有什么用
    PVOID               pUserData;                    /* 用户数据 */
    // config magic
    UINT32              uiConfigMagic;               /* 幻数 */
} SPIFFS_VOLUME;
typedef SPIFFS_VOLUME * PSPIFFS_VOLUME;
/*********************************************************************************************************
 * SPIFFS文件状态描述
*********************************************************************************************************/
typedef struct spiffs_stat {
    SPIFFS_OBJ_ID objId;
    UINT32 uiSize;
    SPIFFS_OBJ_TYPE objType;
    SPIFFS_PAGE_IX pageIX;
    UCHAR ucName[SPIFFS_OBJ_NAME_LEN];
} SPIFFS_STAT;
typedef SPIFFS_STAT * PSPIFFS_STAT;
/*********************************************************************************************************
 * SPIFFS目录文件状态
*********************************************************************************************************/
typedef struct spiffs_dirent {
    SPIFFS_OBJ_ID objId;
    UINT32 uiSize;
    SPIFFS_OBJ_TYPE objType;
    SPIFFS_PAGE_IX pageIX;
    UCHAR ucName[SPIFFS_OBJ_NAME_LEN];
} SPIFFS_DIRENT;
typedef SPIFFS_DIRENT * PSPIFFS_DIRENT;
/*********************************************************************************************************
 * SPIFFS目录描述
*********************************************************************************************************/
typedef struct spiffs_DIR{
    PSPIFFS_VOLUME pfs;
    SPIFFS_BLOCK_IX blkIX;
    INT uiEntry;
} SPIFFS_DIR;
typedef SPIFFS_DIR * PSPIFFS_DIR;
/*********************************************************************************************************
 * SPIFFS Index Page内存映射结构
*********************************************************************************************************/
typedef struct spiffs_ix_map{
    // buffer with looked up data pixes
    SPIFFS_PAGE_IX *pPageIXMapBuffer;
    // precise file byte offset
    UINT32 uiOffset;
    // start data span index of lookup buffer
    SPIFFS_SPAN_IX spanIXStart;
    // end data span index of lookup buffer
    SPIFFS_SPAN_IX spanIXEnd;
} SPIFFS_IX_MAP;
typedef SPIFFS_IX_MAP * PSPIFFS_IX_MAP;
/*********************************************************************************************************
 * SPIFFS Cache Page结构
*********************************************************************************************************/
typedef struct spiffs_cache_page{
    // cache flags
    UINT8 flags;                            /* Cache的类型，供Union使用 */
    // cache page index
    UINT8 uiIX;                             /* 在Cache中的Index */
    // last access of this cache page
    UINT32 uiLastAccess;                    /* 这个Cache Page上次访问时间 */
    //TODO: 这是什么捏？
    union {
      // type Read cache
      struct {
        // read cache page index
        SPIFFS_PAGE_IX pageIX;
      };
      // type Write cache
      struct {
        // write cache
        SPIFFS_OBJ_ID objId;
        // offset in cache page
        UINT32 uiOffset;
        // size of cache page
        UINT16 uiSize;
      };
    };
} SPIFFS_CACHE_PAGE;
typedef SPIFFS_CACHE_PAGE * PSPIFFS_CACHE_PAGE; 
/*********************************************************************************************************
 * SPIFFS Cache 结构
*********************************************************************************************************/
// cache struct
typedef struct spiffs_cache{
    UINT8   uiCpageCount;       /* 页面数 */
    UINT32  uiLastAccess;       /* 上一次访问 */
    UINT32  uiCpageUseMap;      /* CachePage使用位图 */
    UINT32  uiCpageUseMask;     /* 代表总共可容纳的Cache Page */
    PUCHAR  Cpages;             /* 页面缓存 */
} SPIFFS_CACHE;
typedef SPIFFS_CACHE * PSPIFFS_CACHE; 
/*********************************************************************************************************
 * SPIFFS 文件描述符结构
*********************************************************************************************************/
// spiffs nucleus file descriptor
typedef struct spiffs_fd{
    // the filesystem of this descriptor
    PSPIFFS_VOLUME pfs;                 /* 文件头 */
    // number of file descriptor - if 0, the file descriptor is closed, 下标？
    SPIFFS_FILE fileN;                  /* 文件描述符编号，理解为文件句柄 */
    // object id - if SPIFFS_OBJ_ID_ERASED, the file was deleted
    SPIFFS_OBJ_ID objId;                /* 该文件的ObjID */
    // size of the file         
    UINT32 uiSize;                      /* 文件大小 */
    // cached object index header page index
    SPIFFS_PAGE_IX pageIXObjIXHdr;      /* 第0个（SpanIX = 0）IndexPage的Page IX */
    // cached offset object index page index
    SPIFFS_PAGE_IX pageIXObjIXCursor;   /* 当前最后一个Page的Page IX？ */
    // cached offset object index span index
    SPIFFS_SPAN_IX spanIXObjIXCursor;   /* 当前最后一个Page的Span IX */
    // current absolute offset
    UINT32 uiOffset;                    /* 当前的绝对偏移 */
    // current file descriptor offset (cached)
    UINT32 uiFdOffset;                  /* 相对偏移吗？（可能是文件内部指针） */
    // fd flags
    SPIFFS_FLAGS flags;                 /* 文件标志位 */
    PSPIFFS_CACHE_PAGE pCachePage;      /* 缓存的页面 */
    // djb2 hash of filename
    UINT32 uiNameHash;                  /* 文件名Hash */
    // hit score (score == 0 indicates never used fd)
    UINT16 uiScore;                     /* 得分 */
    // spiffs index map, if 0 it means unmapped
    //TODO: Map
    PSPIFFS_IX_MAP pIXMap;              /* Fast Map，暂时未 */
} SPIFFS_FD;
typedef SPIFFS_FD * PSPIFFS_FD;
/*********************************************************************************************************
 * SPIFFS 普通页面内容头部
*********************************************************************************************************/
typedef struct spiffs_page_header {
  // object id
  SPIFFS_OBJ_ID objId;
  // object span index
  SPIFFS_SPAN_IX spanIX;
  // flags
  UINT8 flags;
} SPIFFS_PAGE_HEADER;
typedef SPIFFS_PAGE_HEADER * PSPIFFS_PAGE_HEADER;
/*********************************************************************************************************
 * SPIFFS IX页面内容头部
*********************************************************************************************************/
/* spanIX = 0的Index Page头部 */
typedef struct spiffs_page_object_ix_header         /* SpanIndex为0时，即Index header */
{
  // common page header
  SPIFFS_PAGE_HEADER pageHdr;
  // alignment
  UINT8 __align[4 - ((sizeof(SPIFFS_PAGE_HEADER) & 3) == 0 ? 4 : (sizeof(SPIFFS_PAGE_HEADER) & 3))];
  // size of object
  UINT32 uiSize;
  // type of object
  SPIFFS_OBJ_TYPE type;
  // name of object
  UCHAR ucName[SPIFFS_OBJ_NAME_LEN];
} SPIFFS_PAGE_OBJECT_IX_HEADER;
typedef SPIFFS_PAGE_OBJECT_IX_HEADER * PSPIFFS_PAGE_OBJECT_IX_HEADER;

// object index page header
/* 普通Index 页面头部 */
typedef struct spiffs_page_object_ix {
 SPIFFS_PAGE_HEADER pageHdr;
 UINT8 __align[4 - ((sizeof(SPIFFS_PAGE_HEADER) & 3) == 0 ? 4 : (sizeof(SPIFFS_PAGE_HEADER) & 3))];
} SPIFFS_PAGE_OBJECT_IX;
typedef SPIFFS_PAGE_OBJECT_IX * PSPIFFS_PAGE_OBJECT_IX;
/*********************************************************************************************************
 * 寻找FreeObjId时运用到状态结构体
*********************************************************************************************************/
typedef struct spiffs_free_obj_id_state{
  SPIFFS_OBJ_ID objIdMin;
  SPIFFS_OBJ_ID objIdMax;
  UINT32 uiCompaction;
  PUCHAR pucConflictingName;
} SPIFFS_FREE_OBJ_ID_STATE;
typedef SPIFFS_FREE_OBJ_ID_STATE * PSPIFFS_FREE_OBJ_ID_STATE;
/*********************************************************************************************************
 * 算术宏
*********************************************************************************************************/
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
/*********************************************************************************************************
 * SPIFFS 文件系统文件头相关宏
*********************************************************************************************************/
#define SPIFFS_CONFIG_MAGIC             (0x52AA1314)        /* AA = A星星 */

#define SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)               ((pfs)->cfg.uiLogicPageSize)
#define SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs)              ((pfs)->cfg.uiLogicBlkSize)
#define SPIFFS_CFG_PHYS_SZ(pfs)                     ((pfs)->cfg.uiPhysSize)
#define SPIFFS_CFG_PHYS_ERASE_SZ(pfs)               ((pfs)->cfg.uiPhysEraseBlkSize)
#define SPIFFS_CFG_PHYS_ADDR(pfs)                   ((pfs)->cfg.uiPhysAddr)

#define SPIFFS_MAX_PAGES(pfs)                            (SPIFFS_CFG_PHYS_SZ(pfs)/SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) )
/* 每个块有不同的 MAGIC NUM */
#define SPIFFS_MAGIC(pfs, blkIX)                    ((SPIFFS_OBJ_ID)(SPIFFS_CONFIG_MAGIC ^ SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) ^\
                                                    ((pfs)->uiBlkCount - (blkIX))))
#define SPIFFS_UNDEFINED_LEN            (UINT32)(-1)
/*********************************************************************************************************
 * SPIFFS 地址转换相关宏
*********************************************************************************************************/
#define SPIFFS_PADDR_TO_PAGE(pfs, uiPhysAddr)           (((uiPhysAddr) -  SPIFFS_CFG_PHYS_ADDR(pfs)) / SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))
#define SPIFFS_PADDR_TO_PAGE_OFFSET(pfs, uiPhysAddr)    (((uiPhysAddr) - SPIFFS_CFG_PHYS_ADDR(pfs)) % SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))
#define SPIFFS_PAGE_TO_PADDR(pfs, pageIX)               (SPIFFS_CFG_PHYS_ADDR(pfs) + (pageIX) * SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))

#define SPIFFS_BLOCK_TO_PADDR(pfs, blkIX)               (SPIFFS_CFG_PHYS_ADDR(pfs) + (blkIX)* SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs))
#define SPIFFS_PAGES_PER_BLOCK(pfs)                     (SPIFFS_CFG_LOGIC_BLOCK_SZ(pfs) / SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))
/* 将页面转化为所在块号 */
#define SPIFFS_BLOCK_FOR_PAGE(pfs, pageIX)              ((pageIX) / SPIFFS_PAGES_PER_BLOCK(pfs))
/*********************************************************************************************************
 * SPIFFS Look Up Page相关
*********************************************************************************************************/
/* LOOK UP page占有的页数 */
#define SPIFFS_OBJ_LOOKUP_PAGES(pfs)                    (MAX(1, (SPIFFS_PAGES_PER_BLOCK(pfs) * sizeof(SPIFFS_OBJ_ID)) \
                                                        / SPIFFS_CFG_LOGIC_PAGE_SZ(pfs)) )
/* 一个Blk的最大Entry数 */
#define SPIFFS_OBJ_LOOKUP_MAX_ENTRIES(pfs)              (SPIFFS_PAGES_PER_BLOCK(pfs) - SPIFFS_OBJ_LOOKUP_PAGES(pfs))
#define SPIFFS_OBJ_LOOKUP_ENTRY_TO_PIX(pfs, blkIX, iEntry)\
                                                        ((blkIX)*SPIFFS_PAGES_PER_BLOCK(pfs) + (SPIFFS_OBJ_LOOKUP_PAGES(pfs) + iEntry))
#define SPIFFS_OBJ_LOOKUP_ENTRY_TO_PADDR(pfs, blkIX, iEntry)\
                                                        (SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + (SPIFFS_OBJ_LOOKUP_PAGES(pfs) + iEntry) * SPIFFS_CFG_LOGIC_PAGE_SZ(pfs))
/* 倒数第二个Lookup Entry为块的擦写次数 */
#define SPIFFS_ERASE_COUNT_PADDR(pfs, blkIX)            (SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + SPIFFS_OBJ_LOOKUP_PAGES(pfs) *\
                                                        SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_OBJ_ID))
/* 倒数第二个Lookup Entry为Blk的MagicNum */
#define SPIFFS_MAGIC_PADDR(pfs, blkIX)                  (SPIFFS_BLOCK_TO_PADDR(pfs, blkIX) + SPIFFS_OBJ_LOOKUP_PAGES(pfs) *\
                                                        SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_OBJ_ID) * 2)
/* 对于给定的data page span index或者entry找到Object Index类型的span index */
#define SPIFFS_OBJ_IX_ENTRY_SPAN_IX(pfs, spanIX)        ((spanIX) < SPIFFS_OBJ_HDR_IX_LEN(pfs) ? 0 :\
                                                        (1 + ((spanIX) - SPIFFS_OBJ_HDR_IX_LEN(pfs)) / SPIFFS_OBJ_IX_LEN(pfs)))
/* 将page转化为Entry号 */
#define SPIFFS_OBJ_LOOKUP_ENTRY_FOR_PAGE(pfs, pageIX)   ((pageIX) % SPIFFS_PAGES_PER_BLOCK(pfs) - SPIFFS_OBJ_LOOKUP_PAGES(pfs))

/*********************************************************************************************************
 * SPIFFS IX 相关
*********************************************************************************************************/
// entries in an object header page index
#define SPIFFS_OBJ_HDR_IX_LEN(pfs)  ((SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_PAGE_OBJECT_IX_HEADER)) \
                                    / sizeof(SPIFFS_PAGE_IX))
// entries in an object page index
#define SPIFFS_OBJ_IX_LEN(pfs)      ((SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_PAGE_OBJECT_IX)) \
                                    / sizeof(SPIFFS_PAGE_IX))
#define SPIFFS_OBJ_IX_ENTRY(pfs, spanIX) ((spanIX) < SPIFFS_OBJ_HDR_IX_LEN(pfs) ? (spanIX) \
                                         : (((spanIX) - SPIFFS_OBJ_HDR_IX_LEN(pfs)) % SPIFFS_OBJ_IX_LEN(pfs)))
/*********************************************************************************************************
 * SPIFFS Data Page相关
*********************************************************************************************************/
#define SPIFFS_DATA_PAGE_SIZE(pfs)        (SPIFFS_CFG_LOGIC_PAGE_SZ(pfs) - sizeof(SPIFFS_PAGE_HEADER))
#define SPIFFS_PAGE_FOR_BLOCK(pfs, blkIX) ((blkIX) * SPIFFS_PAGES_PER_BLOCK(pfs) )
/*********************************************************************************************************
 * SPIFFS 检查相关
*********************************************************************************************************/
#define SPIFFS_CHECK_MOUNT(pfs) \
((pfs)->uiMountedFlag != 0)

#define SPIFFS_CHECK_RES(res) \
do { \
    if ((res) < SPIFFS_OK) return (res); \
} while (0);

#define SPIFFS_API_CHECK_MOUNT(pfs) \
if (!SPIFFS_CHECK_MOUNT((pfs))) { \
    (pfs)->uiErrorCode = SPIFFS_ERR_NOT_MOUNTED; \
    return SPIFFS_ERR_NOT_MOUNTED; \
}

#define SPIFFS_API_CHECK_RES(pfs, res) \
if ((res) < SPIFFS_OK) { \
    (pfs)->uiErrorCode = (res); \
    return (res); \
}

/* 检查objIX是否合法 */
#define SPIFFS_VALIDATE_OBJIX(pageHeader, objId, _spanIX) \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_USED) != 0) return SPIFFS_ERR_IS_FREE; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_DELET) == 0) return SPIFFS_ERR_DELETED; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_FINAL) != 0) return SPIFFS_ERR_NOT_FINALIZED; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_INDEX) != 0) return SPIFFS_ERR_NOT_INDEX; \
    if (((objId) & SPIFFS_OBJ_ID_IX_FLAG) == 0) return SPIFFS_ERR_NOT_INDEX; \
    if ((pageHeader).spanIX != (_spanIX)) return SPIFFS_ERR_INDEX_SPAN_MISMATCH;
/* 检查objData是否合法 */
#define SPIFFS_VALIDATE_DATA(pageHeader, objId, _spanIX) \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_USED) != 0) return SPIFFS_ERR_IS_FREE; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_DELET) == 0) return SPIFFS_ERR_DELETED; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_FINAL) != 0) return SPIFFS_ERR_NOT_FINALIZED; \
    if (((pageHeader).flags & SPIFFS_PH_FLAG_INDEX) == 0) return SPIFFS_ERR_IS_INDEX; \
    if ((objId) & SPIFFS_OBJ_ID_IX_FLAG) return SPIFFS_ERR_IS_INDEX; \
    if ((pageHeader).spanIX != (_spanIX)) return SPIFFS_ERR_DATA_SPAN_MISMATCH;
/*********************************************************************************************************
 * ANCHOR: SylixOS适配
*********************************************************************************************************/
#define SYLIX_TO_SPIFFS_PFS(pfs)        (pfs->pfs)
#define SYLIX_TO_SPIFFS_FD(pspifn)      (pspifn->pFd->fileN)
;
typedef struct spif_volume {
    LW_DEV_HDR          SPIFFS_devhdrHdr;                                /*  spiffs 文件系统设备头        */
    LW_OBJECT_HANDLE    SPIFFS_hVolLock;                                 /*  卷操作锁                    */
    LW_LIST_LINE_HEADER SPIFFS_plineFdNodeHeader;                        /*  fd_node 链表                */
    
    BOOL                SPIFFS_bForceDelete;                             /*  是否允许强制卸载卷          */
    BOOL                SPIFFS_bValid;
    
    uid_t               SPIFFS_uid;                                      /*  用户 id                     */
    gid_t               SPIFFS_gid;                                      /*  组   id                     */
    mode_t              SPIFFS_mode;                                     /*  文件 mode                   */
    time_t              SPIFFS_time;                                     /*  创建时间                    */
    ULONG               SPIFFS_ulCurBlk;                                 /*  当前消耗内存大小            */
    ULONG               SPIFFS_ulMaxBlk;                                 /*  最大内存消耗量              */
    
    PSPIFFS_VOLUME      pfs;                                             /*  Spiffs需要的文件头 */
} SPIF_VOLUME;
typedef SPIF_VOLUME     *PSPIF_VOLUME;

typedef struct spif_node {
    PSPIF_VOLUME        pfs;                                             /*  文件系统                    */

    BOOL                SPIFN_bChanged;                                  /*  文件内容是否更改            */
    mode_t              SPIFN_mode;                                      /*  文件 mode                   */
    time_t              SPIFN_timeCreate;                                /*  创建时间                    */
    time_t              SPIFN_timeAccess;                                /*  最后访问时间                */
    time_t              SPIFN_timeChange;                                /*  最后修改时间                */
    
    size_t              SPIFN_stSize;                                    /*  当前文件大小 (可能大于缓冲) */
    size_t              SPIFN_stVSize;                                   /*  lseek 出的虚拟大小          */
    
    uid_t               SPIFN_uid;                                       /*  用户 id                     */
    gid_t               SPIFN_gid;                                       /*  组   id                     */
    PCHAR               SPIFN_pcName;                                    /*  文件名称                    */
    PCHAR               SPIFN_pcLink;                                    /*  链接目标                    */
    
    PSPIFFS_FD          pFd;                                              /*  Spiffs文件描述 */
} SPIFN_NODE;
typedef SPIFN_NODE       *PSPIFN_NODE;

static inline PVOID spif_malloc(PSPIF_VOLUME pfs, size_t stNBytes){
    pfs->SPIFFS_ulCurBlk += stNBytes;
    if(pfs->SPIFFS_ulCurBlk > pfs->SPIFFS_ulMaxBlk){
       pfs->SPIFFS_ulMaxBlk = pfs->SPIFFS_ulCurBlk;
    }
    return lib_malloc(stNBytes);
}

static inline PVOID spif_free(PSPIF_VOLUME pfs, PVOID pvPtr, size_t stNBytes){
    pfs->SPIFFS_ulCurBlk -= stNBytes;
    lib_free(pvPtr);
}
/*********************************************************************************************************
  检测路径字串是否为根目录或者直接指向设备
*********************************************************************************************************/
#define __STR_IS_ROOT(pcName)               ((pcName[0] == PX_EOS) || (lib_strcmp(PX_STR_ROOT, pcName) == 0))


#endif /* SYLIXOS_EXTFS_SPIFFS_SPIFFSTYPE_H_ */