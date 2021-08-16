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
** 文   件   名: HoitMergeBuffer.h
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 07 月 10 日
**
** 描        述: Hoit文件系统为每个文件建立的MergeBuffer的函数库, 用于合并写的小数据
*********************************************************************************************************/
#ifndef __HOITFSMERGEBUFFER_H
#define __HOITFSMERGEBUFFER_H

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#include "SylixOS.h"
#include "hoitType.h"
/*
*	初步设定MergeBuffer的阈值为16个节点，且每个节点都是数据量小于16B的;
*	目前就在open创建inodeinfo时初始化MergeBuffer，新写一段数据时检测小数据并加入到MergeBuffer;
*	同时在红黑树节点fix删除节点时记得删除回指指针;
*/
#define HOIT_MERGE_BUFFER_THRESHOLD 16	// MergeBuffer触发合并动作的节点数
#define HOIT_MERGE_BUFFER_FRAGSIZE	16

BOOL __hoit_new_merge_buffer(PHOIT_INODE_INFO pInodeInfo);
BOOL __hoit_new_merge_entry(PHOIT_INODE_INFO pInodeInfo, PHOIT_MERGE_BUFFER pMergeBuffer, PHOIT_FRAG_TREE_NODE pTreeNode);
BOOL __hoit_del_merge_entry(PHOIT_VOLUME pfs, PHOIT_MERGE_BUFFER pMergeBuffer, PHOIT_MERGE_ENTRY pMergeEntry);
BOOL __hoit_refresh_merge_buffer(PHOIT_INODE_INFO pInodeInfo);
BOOL __hoit_free_merge_buffer(PHOIT_INODE_INFO pInodeInfo);
BOOL __hoit_clear_merge_buffer(PHOIT_INODE_INFO pInodeInfo);

#endif
