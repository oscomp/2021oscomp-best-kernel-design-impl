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
** 文   件   名: hoitFsTreeUtil.h
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 28 日
**
** 描        述: JFFS2-Like 红黑树数据结构
*********************************************************************************************************/

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#ifndef SYLIXOS_EXTFS_HOITFS_HOITFSTREEUTIL_H_
#define SYLIXOS_EXTFS_HOITFS_HOITFSTREEUTIL_H_


#include "hoitType.h"

#define RB_BLACK    1
#define RB_RED      0


static inline PHOIT_RB_NODE newHoitRbNode(INT32 iKey){
    PHOIT_RB_NODE pRbn = (PHOIT_RB_NODE)lib_malloc(sizeof(HOIT_RB_NODE));
    pRbn->iKey = iKey;
    return pRbn;
}

PHOIT_RB_TREE     hoitRbInitTree(VOID);                             /* 初始化RB树 */
PHOIT_RB_NODE     hoitRbInsertNode(PHOIT_RB_TREE, PHOIT_RB_NODE);   /* 插入一个节点 */
PHOIT_RB_NODE     hoitRbSearchNode(PHOIT_RB_TREE, INT32);           /* 查找一个节点 */
BOOL              hoitRbDeleteNode(PHOIT_RB_TREE, PHOIT_RB_NODE);   /* 删除一个节点 */

#ifdef RB_TEST
VOID hoitRbTreeTest();
#endif // DEBUG

#endif /* SYLIXOS_EXTFS_HOITFS_HOITFSTREEUTIL_H_ */
