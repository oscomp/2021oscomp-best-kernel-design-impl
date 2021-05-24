#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
//#include "../SylixOS/kernel/include/k_internal.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/fs/include/fs_fs.h"
//#include "../SylixOS/system/include/s_class.h"
//#include "../SylixOS/kernel/include/k_const.h"
#include "../driver/mtd/nor/fake_nor.h"
#include "../driver/mtd/nor/fake_nor_cmd.h"

/*********************************************************************************************************
  检测路径字串是否为根目录或者直接指向设备
*********************************************************************************************************/

#define __STR_IS_ROOT(pcName)           ((pcName[0] == PX_EOS) || (lib_strcmp(PX_STR_ROOT, pcName) == 0))

/*********************************************************************************************************
  补充宏定义
*********************************************************************************************************/
/* global sector_no */
#define GET_NOR_OFFSET_FROM_SECN(sector_no)                 (sector_no*NOR_FLASH_SECTORSZ)
#define GET_NOR_OFFSET_FROM_BLKN(block_no)                  (block_no*NOR_FLASH_BLKSZ)
/* local sector_no */
#define GET_OFFSET_FROM_BLK_AND_SECN(block_no,sector_no)    (block_no*NOR_FLASH_BLKSZ+sector_no*NOR_FLASH_SECTORSZ)
#define GET_BLKN_OF_SECN(sector_no)                         (sector_no/NOR_FLASH_NBLK)
#define GET_SEC_IDX_IN_BLK(sector_no)                       (sector_no%NOR_FLASH_NBLK)
/* 每个sector可包含多少个inode */
#define INODE_PSEC                                          (NOR_FLASH_SECTORSZ/sizeof(LFS_INODE))

#define ASSIGNABLE_BLOCKS               13              /* 16 - 1(CPR) - 2(FileMap) */
#define MAX_FILE                        1023            /* 最大文件数量 */
#define MAX_DATA_SECTOR                 32              /* 单个文件数据所占最大sector数量 */
#define CPR_FN_1_BLK                    0               /* 第一块CheckpointRegion_FileNameMap */
#define CPR_FN_2_BLK                    1               /* 第二块CheckpointRegion_FileNameMap */

/********************************     Summary sector     **********************************************/
#define SUMMARY_INODE_TYPE              1024
#define SUMMARY_IMAP_TYPE               1025            /* summary 中 secotr 记录 imap 标志 */
#define SUMMARY_SUMMARY_TYPE            1026            /* summary 中 secotr 记录 summary sector 标志 */

/********************************     FileMap               ******************************************/
#define FN_ENTRY_PBLK                   512             /* 每个Block中FileNameEntry的个数 */
#define FN_ENTRY_SZ                     128             /* 每个FileNameEntry的大小为128B */
#define MAX_FN_SZ                       126             /* 一个文件名最大为126B */

#define BLK_CLEAN                       0               /* CPR.free_sec中干净标志 */
#define BLK_DIRTY                       1               /* CPR.free_sec中脏标志 */

//#define UINT_MAX                        4294967295      /* inode,block,secotr等序号找不到时的返回值 */

/*********************************************************************************************************
  类型
*********************************************************************************************************/
typedef struct 
{
    UINT                imap_sec;                 /* imap 所在sector的序号    */
    UINT                summary_sec;              /* summary 所在sector的序号 */
    UINT8               free_sec[NOR_FLASH_NBLK]; /* 当前block可用sector      */              
}CHECKPOINT_REGION;
typedef CHECKPOINT_REGION *PCHECKPOINT_REGION;

typedef struct 
{
    LW_DEV_HDR          LFS_devhdrHdr;                            /* lfs 设备头   */
    LW_OBJECT_HANDLE    LFS_hVolLock;                             /* 设备锁       */ 
    LW_LIST_LINE_HEADER LFS_plineFdNodeHeader;                  /*  fd_node 链表                */
    /* LFS全局变量 */
    UINT                LFS_blockSummary[NOR_FLASH_SECTPBLK \
                                            *NOR_FLASH_NBLK][2];  /* 所有block共享一个summary secotr，第一个为inode号，第二个为有效位 */
    CHAR                LFS_secBuf[NOR_FLASH_SECTORSZ];           /* 以sector为单位的缓冲区 */
    CHAR                LFS_blkBuf[NOR_FLASH_BLKSZ];              /* Block Buffer */
    
    BOOL                LFS_bValid;                               /* 是否有效（是否没在卸载） */
    BOOL                LFS_bForceDelete;                         /* 是否允许强制删除 */
    PCHECKPOINT_REGION  LFS_cpr;                                  /* CPR结构体指针 */
    UINT                LFS_imap[MAX_FILE+1];                     /* IMAP也在内存中维护 */
    UINT                LFS_curBlockNo;                           /* 当前Block，范围(2~31)，0，1被占用 */
    UINT                LFS_availableSector;                      /* 在当前BLOCK_NO中，下一个可用sector，范围(0~15) */
    UINT8               LFS_cleanBlock[NOR_FLASH_NBLK];           /* 查看哪块BLOCK是否是脏的，0为干净，1为脏 */
    UINT8               LFS_inodeMap[MAX_FILE];                   /* inode占用情况 */

    INT                 LFS_availableFile;                        /* 可用文件数量 */
    UINT                LFS_ulCurSector;                         /* 文件系统当前占用sector */
   UINT                 LFS_ulMaxSector;                          /* 最大文件占用sector数量 */

    mode_t              LFS_mode;                 /* 文件mode     */
    time_t              LFS_time;                 /* 创建时间     */  
    uid_t               LFS_uid;                  /* 用户id       */
    gid_t               LFS_gid;                  /* 组id         */
}LFS_VOLUME;
typedef LFS_VOLUME *PLFS_VOLUME;



typedef struct
{                                     
  UINT                  LFSN_inodeNo;
  PCHAR                 LFSN_pcname;                                    /* 文件名字 */

  mode_t                LFSN_mode; 
  uid_t                 LFSN_uid;                                       /*  用户 id                     */
  gid_t                 LFSN_gid;  

  INT                   LFSN_bChange;                                   /*  是否被修改                  */
  time_t                LFSN_timeCreate;                                /*  创建时间                    */
  time_t                LFSN_timeAccess;                                /*  最后访问时间                */
  time_t                LFSN_timeChange;                                /*  最后修改时间                */

  size_t                LFSN_stSize;                                /* 文件大小，以字节为单位，最大为32个sector，即128KB */
  size_t                LFSN_stVSize;                               /* lseek 出的虚拟大小 */
  UINT                  LFSN_Cnt;                                   /* 文件占用sector大小 */

  UINT                  LFSN_sectorLocation[MAX_DATA_SECTOR];       /* 保存数据的sector的全局位置 */
  UINT                  LFSN_other[16];                             /* 保留区域，为了对齐128KB */
}LFS_INODE;
typedef LFS_INODE *PLFS_INODE;


/*********************************************************************************************************
  函数清单
*********************************************************************************************************/
VOID initBlocks();                                          /* 初始化每个block，以及summary sector */
VOID initCPR_FNMap();                                       /* 初始化CPR_FNMap区域，即CheckpointRegion_FileNameMap。 */
VOID initCleanBlock();                                      /* 初始化CleanBlock */
INT initLFS(PLFS_VOLUME plfs);                              /* 初始化LFS的各种缓冲区 */

/*  中间所有函数可能需要修改    */
INT readInCheckpointRegion(PLFS_VOLUME plfs);               /* 读取CPR到内存 yes*/
INT findNextAvailableSector();                              /* 找下一个空闲Block，把结果保存在全局变量BLOCK_NO和AVAILABLE_SECTOR中 */
INT readSummary();                                          /* 读取当前summary sector到BLOCK_SUMMARY中 */
INT readInSector(PLFS_VOLUME plfs, UINT sector_no);         /* 读取当前sector到SEC_BUF中 */
INT readInBlock(PLFS_VOLUME plfs, UINT blk_no);             /* 读当前block到BLK_BUF中 */
UINT readInImapSector(UINT address, UINT fragment_no);      /* 读imap sector到IMAP中 */

INT updateCheckpointRegion(PLFS_VOLUME plfs);                            /* 回写CheckoutPoint Region  */
UINT findNextCleanBlock(PLFS_VOLUME plfs);                  /* 寻找下一个空闲的Segment，在CPR中查找 */
UINT writeOutSector(PLFS_VOLUME plfs);                      /* 将当前SEC_BUF内容写回LFS_curBlockNo的sector中 */
VOID writeOutBlock(PLFS_VOLUME plfs, UINT block_no);        /* 将当前BLK_BUF内容写回LFS_curBlockNo指定的block中 */
UINT nextInodeNumber(PLFS_VOLUME plfs);                     /* 寻找下一个Inode Number，通过FileMap来查找 */
INT  writeInode(PLFS_VOLUME plfs, 
                  const PLFS_INODE pnode, 
                  UINT inode_nor);                          /* 追加写Inode到空闲块 */
INT updateFilemap(PLFS_VOLUME plfs,
                    UINT inode_number,
                    PCHAR lfs_filename);                    /* 更新Filemap */
INT updateImap(PLFS_VOLUME plfs, 
                UINT inode_number, 
                UINT sector_position);                      /* 更新imap */

INT updateSummary(PLFS_VOLUME plfs);

PLFS_INODE getInodeFromInumber(PLFS_VOLUME plfs,
                                UINT inode_number);         /* 获取inode，返回inode指针。如果失败，返回Null */
UINT getInodeNumberOfFile(PLFS_VOLUME plfs,
                          PCHAR pcname);                    /* 根据文件名获取inode序号 */
INT getFileNameOfInodeNumber( PLFS_VOLUME plfs,
                              PCHAR pcname,
                              UINT inode_number);
VOID printSector(UINT sector_no);                           /* 输出Sector */

/* lFs.c交互函数 */
PLFS_INODE __lfs_open(  PLFS_VOLUME     plfs,
                        PCHAR           pcName,
                        BOOL            *pbRoot);
/* lfs 创建一个文件 */
PLFS_INODE  __lfs_maken( PLFS_VOLUME     plfs,
                        PCHAR           pcname,
                        mode_t          mode);
/* lfs 获取文件状态 */                        
VOID        __lfs_stat( PLFS_INODE plfsn, 
                        PLFS_VOLUME plfs, 
                        struct stat  *pstat);
/* LFS 删除一个文件 */
VOID        __lfs_close(PLFS_INODE plfsn, INT iFlag);

/* LFS删除一个文件 */
INT         __lfs_unlink(PLFS_VOLUME plfs, PLFS_INODE plfsn);

/* LFS卸载文件系统 */
VOID        __lfs_unmount(PLFS_VOLUME plfs);

/* LFS 写入文件内容 */
ssize_t     __lfs_write (  PLFS_VOLUME plfs,
                          PLFS_INODE  plfsn, 
                          CPVOID  pvBuffer, 
                          size_t  stNBytes, 
                          size_t  stOft);

ssize_t     __lfs_read( PLFS_VOLUME plfs,
                        PLFS_INODE plfsn,
                        PVOID pvBuffer,
                        size_t stSize,
                        size_t stOft);
INT __lfs_move (PLFS_VOLUME plfs, 
                PLFS_INODE plfsn, 
                PCHAR pcNewName);
