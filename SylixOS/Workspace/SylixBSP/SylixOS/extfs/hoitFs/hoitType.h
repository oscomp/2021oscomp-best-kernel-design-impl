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
** 文   件   名: hoitType.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 03 月 19 日
**
** 描        述: 全局类型文件，防止引用循环
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_HOITFS_HOITTYPE_H_
#define SYLIXOS_EXTFS_HOITFS_HOITTYPE_H_
#define  __SYLIXOS_KERNEL
#include "stdio.h"
#include "SylixOS.h"
#include "../tools/list/list_interface.h"
#include "../tools/crc/crc32.h"

//!2021-06-06 修改块级管理结构（三个链表） by zn
/*********************************************************************************************************
  HoitFs Lib 相关测试
*********************************************************************************************************/
//#define LIB_DEBUG
/*********************************************************************************************************
  HoitFs Cache 相关测试
*********************************************************************************************************/
//#define HOIT_CACHE_TEST
/*********************************************************************************************************
  HoitFs GC 相关测试
*********************************************************************************************************/
// #define GC_DEBUG
// #define GC_TEST
/*********************************************************************************************************
  HoitFs 红黑树 相关测试
*********************************************************************************************************/
#define RB_TEST
/*********************************************************************************************************
  HoitFs FragTree 相关测试
*********************************************************************************************************/
//#define FT_TEST
//#define FT_DEBUG
#define FT_OBSOLETE_TREE_LIST             

/*********************************************************************************************************
  HoitFs LOG 相关测试
*********************************************************************************************************/
//#define DEBUG_LOG
#define  LOG_TEST
/*********************************************************************************************************
  HoitFs 特性宏控
*********************************************************************************************************/
#define  MULTI_THREAD_ENABLE      /* 启用多线程 */
#define  EBS_ENABLE               /* 启用EBS */
#define  WRITE_BUFFER_ENABLE      /* 启用WriteBuffer */
#define  BACKGOURND_GC_ENABLE     /* 启用后台GC */
#define  CRC_DATA_ENABLE          /*  CRC DATA特性 */
//! 07-18 ZN 暂时注释log
// #define  LOG_ENABLE

/*********************************************************************************************************
  HoitFs Error Type
*********************************************************************************************************/
#define LOG_APPEND_OK            0
#define LOG_APPEND_NO_SECTOR    -1
#define LOG_APPEND_BAD_ENTITY   -2

/*********************************************************************************************************
  HoitFs宏定义
*********************************************************************************************************/
#define HOIT_MAGIC_NUM                      0x05201314
#define HOIT_FIELD_TYPE                     0xE0000000          //前3位作为TYPE域

#define HOIT_FLAG_TYPE_INODE                0x20000000     //raw_inode类型 0010
#define HOIT_FLAG_TYPE_DIRENT               0x40000000     //raw_dirent类型  0100
#define HOIT_FLAG_TYPE_LOG                  0x80000000     //raw_log类型 1000

#define HOIT_FLAG_NOT_OBSOLETE              0x00000001     //Flag的最后一位用来表示是否过期，1是没过期，0是过期
#define HOIT_FLAG_OBSOLETE                  0x00000000
#define HOIT_ERROR                          100
#define HOIT_ROOT_DIR_INO                   1   /* HoitFs的根目录的ino为1 */
#define HOIT_MAX_DATA_SIZE                  (56*16)
#define __HOIT_IS_OBSOLETE(pRawHeader)      ((pRawHeader->flag & HOIT_FLAG_NOT_OBSOLETE)    == 0)
#define __HOIT_IS_TYPE_INODE(pRawHeader)    ((pRawHeader->flag & HOIT_FLAG_TYPE_INODE)  != 0)
#define __HOIT_IS_TYPE_DIRENT(pRawHeader)   ((pRawHeader->flag & HOIT_FLAG_TYPE_DIRENT) != 0)
#define __HOIT_IS_TYPE_LOG(pRawHeader)      ((pRawHeader->flag & HOIT_FLAG_TYPE_LOG)    != 0)      

#define __HOIT_VOLUME_LOCK(pfs)             API_SemaphoreMPend(pfs->HOITFS_hVolLock, \
                                            LW_OPTION_WAIT_INFINITE)
#define __HOIT_VOLUME_UNLOCK(pfs)           API_SemaphoreMPost(pfs->HOITFS_hVolLock)

#define __HOIT_DIRTY_LOCK(pfs)              API_SemaphoreMPend(pfs->HOITFS_dirtyLock, \
                                            LW_OPTION_WAIT_INFINITE)
#define __HOIT_DIRTY_UNLOCK(pfs)           API_SemaphoreMPost(pfs->HOITFS_dirtyLock)
#define __HOIT_CLEAN_LOCK(pfs)              API_SemaphoreMPend(pfs->HOITFS_cleanLock, \
                                            LW_OPTION_WAIT_INFINITE)
#define __HOIT_CLEAN_UNLOCK(pfs)           API_SemaphoreMPost(pfs->HOITFS_cleanLock)
#define __HOIT_FREE_LOCK(pfs)              API_SemaphoreMPend(pfs->HOITFS_freeLock, \
                                            LW_OPTION_WAIT_INFINITE)
#define __HOIT_FREE_UNLOCK(pfs)           API_SemaphoreMPost(pfs->HOITFS_freeLock)


#define GET_FREE_LIST(pfs)                  pfs->HOITFS_freeSectorList
#define GET_DIRTY_LIST(pfs)                 pfs->HOITFS_dirtySectorList
#define GET_CLEAN_LIST(pfs)                 pfs->HOITFS_cleanSectorList
#define __HOIT_MIN_4_TIMES(value)           ((value+3)/4*4) /* 将value扩展到4的倍数 */

/*********************************************************************************************************
  文件卷锁操作
*********************************************************************************************************/
#define __HOITFS_VOL_LOCK(pfs)        API_SemaphoreMPend(pfs->HOITFS_hVolLock, \
                                        LW_OPTION_WAIT_INFINITE)
#define __HOITFS_VOL_UNLOCK(pfs)      API_SemaphoreMPost(pfs->HOITFS_hVolLock)

/*********************************************************************************************************
  检测路径字串是否为根目录或者直接指向设备
*********************************************************************************************************/
#define __STR_IS_ROOT(pcName)               ((pcName[0] == PX_EOS) || (lib_strcmp(PX_STR_ROOT, pcName) == 0))


/*********************************************************************************************************
  C语言结构体提前声明
*********************************************************************************************************/
typedef struct HOIT_VOLUME                HOIT_VOLUME;
typedef struct HOIT_RAW_HEADER            HOIT_RAW_HEADER;
typedef struct HOIT_RAW_INODE             HOIT_RAW_INODE;
typedef struct HOIT_RAW_DIRENT            HOIT_RAW_DIRENT;
typedef struct HOIT_RAW_INFO              HOIT_RAW_INFO;
typedef struct HOIT_FULL_DNODE            HOIT_FULL_DNODE;
typedef struct HOIT_FULL_DIRENT           HOIT_FULL_DIRENT;
typedef struct HOIT_INODE_CACHE           HOIT_INODE_CACHE;
typedef struct HOIT_INODE_INFO            HOIT_INODE_INFO;
typedef struct HOIT_ERASABLE_SECTOR       HOIT_ERASABLE_SECTOR;
typedef struct HOIT_ERASABLE_SECTOR_REF   HOIT_ERASABLE_SECTOR_REF;
typedef struct HOIT_LOG_SECTOR            HOIT_LOG_SECTOR;
typedef struct hoit_rb_node               HOIT_RB_NODE;
typedef struct hoit_rb_tree               HOIT_RB_TREE;
typedef struct hoit_frag_tree             HOIT_FRAG_TREE;
typedef struct hoit_frag_tree_node        HOIT_FRAG_TREE_NODE;
typedef struct hoit_frag_tree_list_node   HOIT_FRAG_TREE_LIST_NODE;
typedef struct hoit_frag_tree_list_header HOIT_FRAG_TREE_LIST_HEADER;

typedef struct HOIT_CACHE_BLK             HOIT_CACHE_BLK;
typedef struct HOIT_CACHE_HDR             HOIT_CACHE_HDR;
typedef struct HOIT_EBS_ENTRY             HOIT_EBS_ENTRY;

typedef struct HOIT_LOG_INFO              HOIT_LOG_INFO;
typedef struct HOIT_RAW_LOG               HOIT_RAW_LOG;

typedef struct hoit_merge_buffer          HOIT_MERGE_BUFFER;
typedef struct hoit_merge_entry           HOIT_MERGE_ENTRY;

typedef HOIT_VOLUME*                      PHOIT_VOLUME;
typedef HOIT_RAW_HEADER*                  PHOIT_RAW_HEADER;
typedef HOIT_RAW_INODE*                   PHOIT_RAW_INODE;
typedef HOIT_RAW_DIRENT*                  PHOIT_RAW_DIRENT;
typedef HOIT_RAW_INFO*                    PHOIT_RAW_INFO;
typedef HOIT_FULL_DNODE*                  PHOIT_FULL_DNODE;
typedef HOIT_FULL_DIRENT*                 PHOIT_FULL_DIRENT;
typedef HOIT_INODE_CACHE*                 PHOIT_INODE_CACHE;
typedef HOIT_INODE_INFO*                  PHOIT_INODE_INFO;
typedef HOIT_ERASABLE_SECTOR*             PHOIT_ERASABLE_SECTOR;
typedef HOIT_ERASABLE_SECTOR_REF*         PHOIT_ERASABLE_SECTOR_REF;
typedef HOIT_LOG_SECTOR*                  PHOIT_LOG_SECTOR;
typedef HOIT_RB_NODE *                    PHOIT_RB_NODE;
typedef HOIT_RB_TREE *                    PHOIT_RB_TREE;
typedef HOIT_FRAG_TREE *                  PHOIT_FRAG_TREE;
typedef HOIT_FRAG_TREE_NODE *             PHOIT_FRAG_TREE_NODE;
typedef HOIT_FRAG_TREE_LIST_NODE *        PHOIT_FRAG_TREE_LIST_NODE;
typedef HOIT_FRAG_TREE_LIST_HEADER *      PHOIT_FRAG_TREE_LIST_HEADER;


typedef HOIT_CACHE_BLK *                  PHOIT_CACHE_BLK;
typedef HOIT_CACHE_HDR *                  PHOIT_CACHE_HDR;
typedef HOIT_EBS_ENTRY *                  PHOIT_EBS_ENTRY;

typedef HOIT_LOG_INFO *                   PHOIT_LOG_INFO;
typedef HOIT_RAW_LOG *                    PHOIT_RAW_LOG;

typedef HOIT_MERGE_BUFFER *               PHOIT_MERGE_BUFFER;
typedef HOIT_MERGE_ENTRY *                PHOIT_MERGE_ENTRY;
DEV_HDR          HOITFS_devhdrHdr;

DECLARE_LIST_TEMPLATE(HOIT_ERASABLE_SECTOR_REF);
// USE_LIST_TEMPLATE(hoitType, HOIT_FRAG_TREE_NODE);
/*********************************************************************************************************
  HoitFs super block类型
*********************************************************************************************************/
typedef struct HOIT_VOLUME{
    DEV_HDR                 HOITFS_devhdrHdr;                                /*  HoitFs 文件系统设备头        */
    LW_OBJECT_HANDLE        HOITFS_hVolLock;                                 /*  卷操作锁                    */
    LW_LIST_LINE_HEADER     HOITFS_plineFdNodeHeader;                        /*  fd_node 链表                */
    PHOIT_INODE_INFO        HOITFS_pRootDir;                                 /*  根目录文件暂定为一直打开的  */
    PHOIT_FULL_DIRENT       HOITFS_pTempRootDirent;

    BOOL                    HOITFS_bForceDelete;                             /*  是否允许强制卸载卷          */
    BOOL                    HOITFS_bValid;

    uid_t                   HOITFS_uid;                                      /*  用户 id                     */
    gid_t                   HOITFS_gid;                                      /*  组   id                     */
    mode_t                  HOITFS_mode;                                     /*  文件 mode                   */
    time_t                  HOITFS_time;                                     /*  创建时间                    */
    ULONG                   HOITFS_ulCurBlk;                                 /*  当前消耗内存大小            */
    ULONG                   HOITFS_ulMaxBlk;                                 /*  最大内存消耗量              */

    PHOIT_INODE_CACHE       HOITFS_cache_list;
    UINT                    HOITFS_highest_ino;
    UINT                    HOITFS_highest_version;

    PHOIT_ERASABLE_SECTOR   HOITFS_now_sector;
    
                                                                           /*! GC 相关 */
    PHOIT_ERASABLE_SECTOR           HOITFS_erasableSectorList;                  /* 可擦除Sector列表 */

    List(HOIT_ERASABLE_SECTOR_REF)      HOITFS_dirtySectorList;                     /* 含有obsolete的块 */ 
    List(HOIT_ERASABLE_SECTOR_REF)      HOITFS_cleanSectorList;                     /* 不含obsolete的块 */
    List(HOIT_ERASABLE_SECTOR_REF)      HOITFS_freeSectorList;                      /* 啥都不含的块 */
    Iterator(HOIT_ERASABLE_SECTOR_REF)  HOITFS_sectorIterator;                      /* 统一sector迭代器 */
    
    LW_OBJECT_HANDLE                    HOITFS_dirtyLock;                 /* dirty 列表锁 */
    LW_OBJECT_HANDLE                    HOITFS_cleanLock;                 /* dirty 列表锁 */
    LW_OBJECT_HANDLE                    HOITFS_freeLock;                  /* dirty 列表锁 */

    PHOIT_ERASABLE_SECTOR   HOITFS_curGCSector;                            /* 当前正在GC的Sector */
    LW_OBJECT_HANDLE        HOITFS_GCMsgQ;                                 /* GC线程消息队列*/
    LW_OBJECT_HANDLE        HOITFS_hGCThreadId;                            /* GC总线程ID */
    BOOL                    HOITFS_bShouldKillGC;                          /* 是否停止后台GC */
    
    ULONG                   ulGCForegroundTimes;                           /* GC前台计数 */
    ULONG                   ulGCBackgroundTimes;                           /* GC后台计数 */
    
    size_t                  HOITFS_totalUsedSize;                          /* hoitfs已使用Flash大小 */
    size_t                  HOITFS_totalSize;                              /* 总Flash大小 */
    
                                                                           /*! Cache */
    PHOIT_CACHE_HDR         HOITFS_cacheHdr;                               /* hoitfs的cache头结构 */

                                                                           /*! Log相关 */
    PHOIT_LOG_INFO          HOITFS_logInfo;

} HOIT_VOLUME;


/*********************************************************************************************************
  HoitFs 数据实体的公共Header类型
*********************************************************************************************************/
struct HOIT_RAW_HEADER{ //32B
    UINT32              magic_num;
    UINT32              flag;
    UINT32              totlen;
    mode_t              file_type;
    UINT                ino;
    UINT                version;
    UINT32              crc;
};


/*********************************************************************************************************
  HoitFs raw inode类型
*********************************************************************************************************/
struct HOIT_RAW_INODE { //32B
    UINT32              magic_num;
    UINT32              flag;
    UINT32              totlen;     /* 包含头部及数据长度 */
    mode_t              file_type;
    UINT                ino;
    UINT                version;
    UINT32              crc;
    UINT                offset;     /* 记录文件内部偏移 */
};

/*********************************************************************************************************
  HoitFs raw dirent类型
*********************************************************************************************************/
struct HOIT_RAW_DIRENT{
    UINT32              magic_num;
    UINT32              flag;
    UINT32              totlen;
    mode_t              file_type;
    UINT                ino;
    UINT                version;
    UINT32              crc;
    UINT                pino;
};

struct HOIT_RAW_INFO{ //32B
    UINT                phys_addr;
    UINT                totlen;
    PHOIT_RAW_INFO      next_phys;                                     /* 物理上邻接的下一个 */
    PHOIT_RAW_INFO      next_logic;                                    /* 同属一个ino的下一个 */
    UINT                is_obsolete;
};                                     

struct HOIT_FULL_DNODE{//32B
    PHOIT_FULL_DNODE    HOITFD_next;
    PHOIT_RAW_INFO      HOITFD_raw_info;
    UINT                HOITFD_offset;                                  /*在文件里的偏移量*/
    UINT                HOITFD_length;                                  /*有效的数据长度*/
    mode_t              HOITFD_file_type;                               /*文件的类型*/
    UINT                HOITFD_version;
};

struct HOIT_FULL_DIRENT{
    PHOIT_FULL_DIRENT   HOITFD_next;
    PHOIT_RAW_INFO      HOITFD_raw_info;
    UINT                HOITFD_nhash;
    PCHAR               HOITFD_file_name;
    UINT                HOITFD_ino;                                     /* 目录项指向的文件inode number      */
    UINT                HOITFD_pino;                                    /* 该目录项所属的目录文件inode number*/
    mode_t              HOITFD_file_type;
    UINT                HOITFD_version;
};


struct HOIT_INODE_CACHE{  
    UINT                HOITC_ino;
    PHOIT_INODE_CACHE   HOITC_next;
    PHOIT_RAW_INFO      HOITC_nodes;
    UINT                HOITC_nlink;
};


struct HOIT_INODE_INFO{
    mode_t              HOITN_mode;                                     /*  文件 mode                   */
    PHOIT_INODE_CACHE   HOITN_inode_cache;
    PHOIT_FULL_DIRENT   HOITN_dents;
    PHOIT_FULL_DNODE    HOITN_metadata;
    PHOIT_FRAG_TREE     HOITN_rbtree;
    PHOIT_VOLUME        HOITN_volume;
    UINT                HOITN_ino;                                      /*  规定根目录的ino为1          */
    PCHAR               HOITN_pcLink;
    PHOIT_MERGE_BUFFER  HOITN_pMergeBuffer;

    uid_t               HOITN_uid;                                      /*  用户 id                     */
    gid_t               HOITN_gid;                                      /*  组   id                     */
    time_t              HOITN_timeCreate;                               /*  创建时间                    */
    time_t              HOITN_timeAccess;                               /*  最后访问时间                */
    time_t              HOITN_timeChange;                               /*  最后修改时间                */
    size_t              HOITN_stSize;                                   /*  当前文件大小 (可能大于缓冲) */
    size_t              HOITN_stVSize;                                  /*  lseek 出的虚拟大小          */
};



struct HOIT_ERASABLE_SECTOR{  //100B
    PHOIT_ERASABLE_SECTOR         HOITS_next;                                     /* 链表信息管理 */
    UINT                          HOITS_bno;                                      /* Setcor号block number              */
    UINT                          HOITS_addr;
    UINT                          HOITS_length;
    UINT                          HOITS_offset;                                   /* 当前在写物理地址 = addr+offset  */
    UINT                          HOITS_uiObsoleteEntityCount;                    /* sector当前包含过期实体数量 */
    UINT                          HOITS_uiAvailableEntityCount;                   /* sector当前包含有效实体数量 */
    //TODO: 写入、GC时，需要持有该锁
    spinlock_t                    HOITS_lock;                                     /* 每个Sector持有一个? */ 

    UINT                          HOITS_uiUsedSize;
    UINT                          HOITS_uiFreeSize;
    
    ////UINT                      HOITS_uiNTotalRawInfo;                          /* 该可擦除Sector中RawInfo的总数 */
    PHOIT_RAW_INFO                HOITS_pRawInfoFirst;                            /* 指向可擦除Sector中第一个数据实体 */
    PHOIT_RAW_INFO                HOITS_pRawInfoLast;                             /* 指向可擦除Sector中最后一个数据实体，通过next_phys获取下一个数据实体 */
    
    PHOIT_RAW_INFO                HOITS_pRawInfoCurGC;                            /* 当前即将回收的数据实体，注：一次仅回收一个数据实体 */
    PHOIT_RAW_INFO                HOITS_pRawInfoPrevGC;
    PHOIT_RAW_INFO                HOITS_pRawInfoLastGC;                           /* 最后一块应当被GC的Raw Info */
  
    ULONG                         HOITS_tBornAge;                                 /* 当前Sector的出生时间 */                        
};

struct HOIT_ERASABLE_SECTOR_REF {
    PHOIT_ERASABLE_SECTOR  pErasableSetcor;
};

struct HOIT_LOG_SECTOR{
    PHOIT_LOG_SECTOR        pErasableNextLogSector;
    
    PHOIT_ERASABLE_SECTOR   pErasableSetcor;    
};
/*********************************************************************************************************
  红黑树节点
*********************************************************************************************************/
struct hoit_rb_node
{
    UINT32 uiColor;
    INT32  iKey;
    struct hoit_rb_node* pRbnLeft;
    struct hoit_rb_node* pRbnRight;
    struct hoit_rb_node* pRbnParent;
};



/*********************************************************************************************************
  红黑树结构
*********************************************************************************************************/
struct hoit_rb_tree
{
    PHOIT_RB_NODE pRbnGuard;            /* 鍝ㄥ叺 */
    PHOIT_RB_NODE pRbnRoot;             /* 鏍硅妭鐐? */
};



/*********************************************************************************************************
  INODE_INFO指向的FragTree
*********************************************************************************************************/
struct hoit_frag_tree
{
    PHOIT_RB_TREE pRbTree;
    UINT32 uiNCnt;                                               /* 该红黑树节点数目 */
    PHOIT_VOLUME pfs;

    UINT32 uiMemoryBytes;                                        /* add by PYQ 用于观测大小 */
};


/*********************************************************************************************************
  FragTree节点
*********************************************************************************************************/
struct hoit_frag_tree_node
{
    HOIT_RB_NODE pRbn;
    PHOIT_FULL_DNODE pFDnode;
    UINT32 uiSize;
    UINT32 uiOfs;
    PHOIT_MERGE_ENTRY pMergeEntry;
};
/*********************************************************************************************************
  PHOIT_FRAG_TREE_LIST_NODE 
*********************************************************************************************************/
struct hoit_frag_tree_list_node
{
    PHOIT_FRAG_TREE_NODE pFTn;
    struct hoit_frag_tree_list_node * pFTlistNext;
};


/*********************************************************************************************************
  PHOIT_FRAG_TREE_LIST_NODE_HEADER
*********************************************************************************************************/
struct hoit_frag_tree_list_header
{
    PHOIT_FRAG_TREE_LIST_NODE pFTlistHeader;
    UINT32 uiLowBound;
    UINT32 uiHighBound;
    UINT32 uiNCnt;
};
/*********************************************************************************************************
  FragTree Visitor结构
*********************************************************************************************************/
typedef enum vis_statue
{
    VIS_CONTINUE,
    VIS_END,
    VIS_ERROR
} VIS_STATUE;

typedef VIS_STATUE (*visitorHoitFragTree)(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn, PVOID pUserValue, PVOID *ppReturn);
/*********************************************************************************************************
  HOITFS cache结构
*********************************************************************************************************/
typedef struct HOIT_CACHE_BLK
{
    BOOL                        HOITBLK_bType;
    PHOIT_ERASABLE_SECTOR       HOITBLK_sector;           /* cache映射的Sector，用于获取信息 */
    UINT32                      HOITBLK_blkNo;            /* cache块号 */
    struct HOIT_CACHE_BLK       *HOITBLK_cacheListPrev;   /* 链表上上一个cache */
    struct HOIT_CACHE_BLK       *HOITBLK_cacheListNext;   /* 链表上下一个cache */
    PCHAR                       HOITBLK_buf;              /* 数据?? */
    //! 2021-08-14 PYQ 观测
    UINT32                      HOITBLK_uiCurOfs;
}HOIT_CACHE_BLK;

/*********************************************************************************************************
  HOITFS cache 管理头结构
*********************************************************************************************************/
typedef struct HOIT_CACHE_HDR
{
    PHOIT_VOLUME            HOITCACHE_hoitfsVol;
    size_t                  HOITCACHE_blockSize;    /* 单个cache大小 */
    UINT32                  HOITCACHE_blockMaxNums; /* cache最大数量 */
    UINT32                  HOITCACHE_blockNums;    /* 当前cache数量 */
    LW_OBJECT_HANDLE        HOITCACHE_hLock;        /* cache自旋锁? */
    UINT32                  HOITCACHE_flashBlkNum;  /* 将flash分块后的块数 */
    UINT32                  HOITCACHE_nextBlkToWrite;/* 下一个要输出的块 */
    PHOIT_CACHE_BLK         HOITCACHE_cacheLineHdr;  /* cache链表头，注意该节点不保存数据 */
    
    //! 2021-07-04 ZN filter层
    // size_t                  HOITCACHE_EBSEntrySize; /* EBS enty大小 */
    size_t                  HOITCACHE_CRCMagicAddr;   /* EBS 区域 CRC 校验码位置 */
    size_t                  HOITCACHE_EBSStartAddr;   /* EBS 在sector中起始地址 */
    size_t                  HOITCACHE_PageAmount;     /* 单个cache sector中的页数量，也是EBS entry的总数量(数量是1k -1) */

} HOIT_CACHE_HDR;

//! 2021-7-04 ZN EBS项
/*********************************************************************************************************
  HOITFS EBS entry
*********************************************************************************************************/
typedef struct HOIT_EBS_ENTRY
{
    UINT32  HOIT_EBS_ENTRY_inodeNo;     /* 所属文件inode号，未使用时全1 */
    UINT16  HOIT_EBS_ENTRY_obsolete;    /* 过期标志，未过期时全1，过期时全0 */
    UINT16  HOIT_EBS_ENTRY_pageNo;      /* 数据实体在sector上的首个页面页号 */
}HOIT_EBS_ENTRY;

/*********************************************************************************************************
  HOITFS log 文件头
*********************************************************************************************************/
struct HOIT_LOG_INFO
{
    PHOIT_LOG_SECTOR pLogSectorList;              /* LOG Sector链表管理  */
    UINT             uiRawLogHdrAddr;             /* 该文件头指向的首 LOG地址 */
    UINT             uiLogCurAddr;
    UINT             uiLogCurOfs;
    UINT             uiLogSize;
    UINT             uiLogEntityCnt; 
};

struct HOIT_RAW_LOG
{
    UINT32              magic_num;
    UINT32              flag;
    UINT32              totlen;
    mode_t              file_type;
    UINT                ino;
    UINT                version;

    UINT                uiLogSize;
    UINT                uiLogFirstAddr;
};
/*********************************************************************************************************
  HOITFS MergeBuffer 结构
*********************************************************************************************************/
struct hoit_merge_buffer
{
    UINT32                  size;
    UINT32                  threshold;  //触发合并操作的节点数
    PHOIT_MERGE_ENTRY       pList;
};
struct hoit_merge_entry
{
    PHOIT_FRAG_TREE_NODE    pTreeNode;
    PHOIT_MERGE_ENTRY       pNext;
    PHOIT_MERGE_ENTRY       pPrev;
};

/*********************************************************************************************************
  偏移量计算
*********************************************************************************************************/
#define OFFSETOF(type, member)                                      \
        ((size_t)&((type *)0)->member)                              \
/*********************************************************************************************************
  得到ptr的容器结构
*********************************************************************************************************/
#define CONTAINER_OF(ptr, type, member)                             \
        ((type *)((size_t)ptr - OFFSETOF(type, member)))      \

#define HOIT_RAW_DATA_MAX_SIZE      4096    /* 单位为 Byte */

static inline PVOID hoit_malloc(PHOIT_VOLUME pfs, size_t stNBytes){
    pfs->HOITFS_ulCurBlk += stNBytes;
    if(pfs->HOITFS_ulCurBlk > pfs->HOITFS_ulMaxBlk){
       pfs->HOITFS_ulMaxBlk = pfs->HOITFS_ulCurBlk;
    }
    return lib_malloc(stNBytes);
}

static inline PVOID hoit_free(PHOIT_VOLUME pfs, PVOID pvPtr, size_t stNBytes){
    if(pvPtr == LW_NULL){
        return;
    }
    pfs->HOITFS_ulCurBlk -= stNBytes;
    lib_free(pvPtr);
}
/*********************************************************************************************************
  Common 公用方法区
*********************************************************************************************************/
BOOL                  hoitLogCheckIfLog(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector);
PHOIT_ERASABLE_SECTOR hoitFindAvailableSector(PHOIT_VOLUME pfs);

#ifdef CRC_DATA_ENABLE
static UINT32         hoit_crc32_le(PUCHAR p, UINT len){
    return crc32_le(p, len);
}
#else 
static UINT32         hoit_crc32_le(PUCHAR p, UINT len){

}
#endif  /* CRC_DATA_ENABLE */
#endif /* SYLIXOS_EXTFS_HOITFS_HOITTYPE_H_ */
