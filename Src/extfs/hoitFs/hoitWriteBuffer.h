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
** 文   件   名: HoitWriteBuffer.h
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 07 月 10 日
**
** 描        述: Hoit文件系统为每个文件建立的WriteBuffer的函数库, 用于合并写的小数据
*********************************************************************************************************/
#ifndef __HOITFSWRITEBUFFER_H
#define __HOITFSWRITEBUFFER_H

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#include "SylixOS.h"
#include "hoitType.h"
/*
*	初步设定WriteBuffer的阈值为16个节点，且每个节点都是数据量小于16B的;
*	目前就在open创建inodeinfo时初始化writebuffer，新写一段数据时检测小数据并加入到writebuffer;
*	同时在红黑树节点fix删除节点时记得删除回指指针;
*/
#define HOIT_WRITE_BUFFER_THRESHOLD 16	// WriteBuffer触发合并动作的节点数
#define HOIT_WRITE_BUFFER_FRAGSIZE	16

BOOL __hoit_new_write_buffer(PHOIT_INODE_INFO pInodeInfo);
BOOL __hoit_new_write_entry(PHOIT_INODE_INFO pInodeInfo, PHOIT_WRITE_BUFFER pWriteBuffer, PHOIT_FRAG_TREE_NODE pTreeNode);
BOOL __hoit_del_write_entry(PHOIT_WRITE_BUFFER pWriteBuffer, PHOIT_WRITE_ENTRY pWriteEntry);
BOOL __hoit_refresh_write_buffer(PHOIT_INODE_INFO pInodeInfo);
BOOL __hoit_free_write_buffer(PHOIT_INODE_INFO pInodeInfo);

#endif
