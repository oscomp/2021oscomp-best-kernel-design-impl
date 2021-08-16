/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: HoitFsLib.h
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 03 月 20 日
**
** 描        述: Hoit文件系统内部函数.
*********************************************************************************************************/
#ifndef __HOITFSLIB_H
#define __HOITFSLIB_H

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

//#include "../SylixOS/kernel/include/k_kernel.h"
//#include "../SylixOS/system/include/s_system.h"
//#include "../SylixOS/fs/include/fs_fs.h"


#include "hoitType.h"
typedef struct scanThreadAttr { /* 一个局部定义的结构体, 只用来给scan_single_sector传参 */
    PHOIT_VOLUME    pfs;
    UINT8           sector_no;
}ScanThreadAttr;

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0 //&& LW_CFG_RAMFS_EN > 0

PHOIT_INODE_INFO        __hoit_just_open(PHOIT_INODE_INFO pdir, PCHAR pName);
UINT                    __hoit_name_hash(CPCHAR pcName);
UINT                    __hoit_free_full_dirent(PHOIT_VOLUME pfs, PHOIT_FULL_DIRENT pDirent);
PHOIT_INODE_INFO        __hoit_get_full_file(PHOIT_VOLUME pfs, UINT ino);
PHOIT_INODE_CACHE       __hoit_get_inode_cache(PHOIT_VOLUME pfs, UINT ino);
VOID                    __hoit_add_dirent(PHOIT_INODE_INFO pFatherInode, PHOIT_FULL_DIRENT pSonDirent, UINT needLog);
UINT                    __hoit_alloc_ino(PHOIT_VOLUME pfs);
UINT8                   __hoit_read_flash(PHOIT_VOLUME pfs, UINT phys_addr, PVOID pdata, UINT length);
UINT8                   __hoit_write_flash(PHOIT_VOLUME pfs, PVOID pdata, UINT length, UINT* phys_addr, UINT needLog);
UINT8                   __hoit_write_flash_thru(PHOIT_VOLUME pfs, PVOID pdata, UINT length, UINT phys_addr);
UINT8                   __hoit_add_to_inode_cache(PHOIT_INODE_CACHE pInodeCache, PHOIT_RAW_INFO pRawInfo);
UINT8                   __hoit_add_to_cache_list(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeCache);
UINT8                   __hoit_add_to_dents(PHOIT_FULL_DIRENT* ppFatherDents, PHOIT_FULL_DIRENT pFullDirent);
PHOIT_FULL_DIRENT       __hoit_search_in_dents(PHOIT_INODE_INFO pInodeFather, UINT ino, PCHAR pName);
UINT8                   __hoit_del_raw_info(PHOIT_INODE_CACHE pInodeCache, PHOIT_RAW_INFO pRawInfo);
UINT8                   __hoit_del_raw_data(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pRawInfo);
UINT8                   __hoit_del_full_dirent(PHOIT_FULL_DIRENT* ppFatherDents, PHOIT_FULL_DIRENT pFullDirent);
UINT8                   __hoit_del_inode_cache(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeCache);
BOOL                    __hoit_scan_single_sector(ScanThreadAttr* pThreadAttr);
PHOIT_INODE_INFO        __hoit_new_inode_info(PHOIT_VOLUME pfs, mode_t mode, CPCHAR pcLink);
VOID                    __hoit_get_nlink(PHOIT_INODE_INFO pInodeInfo);
BOOL                    __hoit_add_to_sector_list(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector);
PCHAR                   __hoit_get_data_after_raw_inode(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pInodeInfo);
VOID                    __hoit_add_raw_info_to_sector(PHOIT_ERASABLE_SECTOR pSector, PHOIT_RAW_INFO pRawInfo);
BOOL                    __hoit_move_home(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pRawInfo);

PHOIT_INODE_INFO        __hoit_open(PHOIT_VOLUME  pfs,
                                    CPCHAR       pcName,
                                    PHOIT_INODE_INFO* ppInodeFather,
                                    PHOIT_FULL_DIRENT* ppFullDirent,
                                    BOOL* pbRoot,
                                    BOOL* pbLast,
                                    PCHAR* ppcTail);

PHOIT_INODE_INFO        __hoit_maken(PHOIT_VOLUME  pfs,
                                     CPCHAR       pcName,
                                     PHOIT_INODE_INFO    pInodeFather,
                                     mode_t       mode,
                                     CPCHAR       pcLink);
                                     
INT                     __hoit_unlink_regular(PHOIT_INODE_INFO pInodeFather, PHOIT_FULL_DIRENT  pDirent);
VOID                    __hoit_truncate(PHOIT_INODE_INFO  pInodeInfo, size_t  offset);
INT                     __hoit_unlink_dir(PHOIT_INODE_INFO pInodeFather, PHOIT_FULL_DIRENT  pDirent);
INT                     __hoit_move_check(PHOIT_INODE_INFO  pInode1, PHOIT_INODE_INFO  pInode2);
INT                     __hoit_move(PHOIT_INODE_INFO pInodeFather, PHOIT_INODE_INFO  pInodeInfo, PCHAR  pcNewName);
INT                     __hoit_stat(PHOIT_INODE_INFO pInodeInfo, PHOIT_VOLUME  pfs, struct stat* pstat);
INT                     __hoit_statfs(PHOIT_VOLUME  pfs, struct statfs* pstatfs);
ssize_t                 __hoit_read(PHOIT_INODE_INFO  pInodeInfo, PVOID  pvBuffer, size_t  stSize, size_t  stOft);
ssize_t                 __hoit_write(PHOIT_INODE_INFO  pInodeInfo, CPVOID  pvBuffer, size_t  stNBytes, size_t  stOft, UINT needLog);
VOID                    __hoit_unmount(PHOIT_VOLUME pfs);
VOID                    __hoit_mount(PHOIT_VOLUME  pfs);
VOID                    __hoit_redo_log(PHOIT_VOLUME  pfs);

UINT8                   __hoit_get_inode_nodes(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeInfo, PHOIT_FULL_DIRENT* ppDirentList, PHOIT_FULL_DNODE* ppDnodeList);
VOID                    __hoit_close(PHOIT_INODE_INFO  pInodeInfo, INT  iFlag);

//! Added By ZN
VOID __hoit_fix_up_sector_list(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector);
BOOL __hoit_erasable_sector_list_check_exist(PHOIT_VOLUME pfs, List(HOIT_ERASABLE_SECTOR) HOITFS_sectorList, PHOIT_ERASABLE_SECTOR pErasableSector);
VOID __hoit_mark_obsolete(PHOIT_VOLUME pfs, PHOIT_RAW_HEADER pRawHeader, PHOIT_RAW_INFO pRawInfo);


#ifdef CRC_DATA_ENABLE
static void crc32_check(PHOIT_RAW_HEADER pRawHeader) {
    if(pRawHeader == LW_NULL){
        return;
    }
    /* 检查crc校验码 */
    UINT32 uPrevCrc = pRawHeader->crc;
    if(uPrevCrc == 1938479914){
        printf("111\n");
    }
    pRawHeader->crc = 0;
    UINT32 uNowCrc = hoit_crc32_le((unsigned char*)pRawHeader, pRawHeader->totlen);
    if (uPrevCrc != uNowCrc) {
        PHOIT_RAW_INODE pp = (PHOIT_RAW_INODE)pRawHeader;
        CHAR* pChar = ((char*)pp)+sizeof(HOIT_RAW_INODE);
        snprintf("%s", 925, pChar);
        printf("\nError in CRC!\n");
    }

    pRawHeader->crc = uPrevCrc; /* 还原CRC到进入函数之前 */
}
#else 
static void crc32_check(PHOIT_RAW_HEADER pRawHeader) {

}
#endif      /* CRC_DATA_ENABLE */

#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0       */
#endif                                                                  /*  __HOITFSLIB_H                */
