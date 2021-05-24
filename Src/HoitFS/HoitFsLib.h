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
** 创   建   人: Hoit Group
**
** 文件创建日期: 2021 年 03 月 20 日
**
** 描        述: Hoit文件系统内部函数.
*********************************************************************************************************/

#ifndef __HOITFSLIB_H
#define __HOITFSLIB_H

#include "SylixOS.h"                                                    /*  操作系统                    */

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0 //&& LW_CFG_RAMFS_EN > 0

/*********************************************************************************************************
  HoitFs宏定义
*********************************************************************************************************/
#define HOITFS_MAGIC_NUM 0x05201314
#define HOITFS_FIELD_TYPE 0xE0000000    //前3位作为TYPE域

#define HOITFS_TYPE_INODE 0x20000000    //raw_inode类型 001
#define HOITFS_TYPE_DIRENT 0x40000000   //raw_dirent类型  010

#define __HOITFS_SB_LOCK(pfs)        API_SemaphoreMPend(pfs->HOITFS_hVolLock, \
                                        LW_OPTION_WAIT_INFINITE)
#define __HOITFS_SB_UNLOCK(pfs)      API_SemaphoreMPost(pfs->HOITFS_hVolLock)

/*********************************************************************************************************
  检测路径字串是否为根目录或者直接指向设备
*********************************************************************************************************/
#define __STR_IS_ROOT(pcName)           ((pcName[0] == PX_EOS) || (lib_strcmp(PX_STR_ROOT, pcName) == 0))

/*********************************************************************************************************
  HoitFs super block类型
*********************************************************************************************************/
typedef struct {
    LW_DEV_HDR          HOITFS_devhdrHdr;                                /*  HoitFs 文件系统设备头        */
    LW_OBJECT_HANDLE    HOITFS_hVolLock;                                 /*  卷操作锁                    */
    LW_LIST_LINE_HEADER HOITFS_plineFdNodeHeader;                        /*  fd_node 链表                */
    LW_LIST_LINE_HEADER HOITFS_plineSon;                                 /*  儿子链表                    */

    BOOL                HOITFS_bForceDelete;                             /*  是否允许强制卸载卷          */
    BOOL                HOITFS_bValid;

    uid_t               HOITFS_uid;                                      /*  用户 id                     */
    gid_t               HOITFS_gid;                                      /*  组   id                     */
    mode_t              HOITFS_mode;                                     /*  文件 mode                   */
    time_t              HOITFS_time;                                     /*  创建时间                    */
    ULONG               HOITFS_ulCurBlk;                                 /*  当前消耗内存大小            */
    ULONG               HOITFS_ulMaxBlk;                                 /*  最大内存消耗量              */
} HOIT_SB;
typedef HOIT_SB* PHOIT_SB;

/*********************************************************************************************************
  HoitFs 数据实体的公共Header类型
*********************************************************************************************************/
typedef struct {
    UINT32 MAGIC_NUM;
    UINT32 FLAG;
} HOIT_RAW_HEADER;
/*********************************************************************************************************
  HoitFs raw inode类型
*********************************************************************************************************/
typedef struct {
    UINT32 MAGIC_NUM;
    UINT32 FLAG;
} HOIT_RAW_INODE;
typedef HOIT_RAW_INODE* PHOIT_RAW_INODE;

/*********************************************************************************************************
  HoitFs raw dirent类型
*********************************************************************************************************/
typedef struct {
    UINT32 MAGIC_NUM;
    UINT32 FLAG;
} HOIT_RAW_DIRENT;
typedef HOIT_RAW_DIRENT* PHOIT_RAW_DIRENT;

#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
#endif                                                                  /*  __HOITFSLIB_H                */
