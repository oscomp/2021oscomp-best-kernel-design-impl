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
** 文   件   名: hoitFsTree.h
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 28 日
**
** 描        述: JFFS2 Like Frag Tree实现
*********************************************************************************************************/


#ifndef SYLIXOS_EXTFS_HOITFS_HOITFSTREE_H_
#define SYLIXOS_EXTFS_HOITFS_HOITFSTREE_H_

#include "hoitType.h"
#include "hoitFsTreeUtil.h"
#include "hoitFsLib.h"


/*********************************************************************************************************
  上层接口 - 供测试
*********************************************************************************************************/
#ifdef FT_TEST
typedef struct hoit_raw_header
{
    UINT32              magic_num;
    UINT32              flag;
    UINT32              totlen;
    mode_t              file_type;
    UINT                ino;
} HOIT_RAW_HEADER;
typedef HOIT_RAW_HEADER * PHOIT_RAW_HEADER;

typedef struct hoit_raw_info {
    struct hoit_raw_info *     next_phys;
    UINT                       phys_addr;
    UINT                       totlen;
} HOIT_RAW_INFO;
typedef HOIT_RAW_INFO * PHOIT_RAW_INFO;

typedef struct hoit_full_dnode{
    struct hoit_full_dnode *  HOITFD_next;
    PHOIT_RAW_INFO            HOITFD_raw_info;
    UINT                      HOITFD_offset;                                  /* 在文件里的偏移量 */
    UINT                      HOITFD_length;                                  /* 有效的数据长度 */
    UINT                      HOITFD_version;                                 /* 版本号 */
} HOIT_FULL_DNODE;
typedef HOIT_FULL_DNODE* PHOIT_FULL_DNODE;

//typedef struct hoit_volume{
//
//} HOIT_VOLUME;
//typedef HOIT_VOLUME * PHOIT_VOLUME;

PHOIT_FULL_DNODE __hoit_truncate_full_dnode(PHOIT_VOLUME pfs, PHOIT_FULL_DNODE pFDnode, UINT uiOffset, UINT uiSize);
BOOL             __hoit_delete_full_dnode(PHOIT_VOLUME pfs, PHOIT_FULL_DNODE pFDnode, BOOL bDoDelete);

#endif // FT_TEST

static inline PHOIT_FRAG_TREE_NODE newHoitFragTreeNode(PHOIT_VOLUME pfs,  PHOIT_FULL_DNODE pFDnode, UINT32 uiSize, UINT32 uiOfs, UINT32 iKey){
    PHOIT_FRAG_TREE_NODE pFTn = (PHOIT_FRAG_TREE_NODE)hoit_malloc(pfs, sizeof(HOIT_FRAG_TREE_NODE));
    pFTn->pRbn.iKey = iKey;
    pFTn->uiOfs = uiOfs;
    pFTn->uiSize = uiSize;
    pFTn->pFDnode = pFDnode;
    pFTn->pMergeEntry = LW_NULL;    /* 07-18 By HZS */
    return pFTn;
}
#ifndef FT_OBSOLETE_TREE_LIST
/*********************************************************************************************************
 PHOIT_FRAG_TREE_LIST_NODE构造函数
*********************************************************************************************************/
static inline PHOIT_FRAG_TREE_LIST_NODE newFragTreeListNode(PHOIT_FRAG_TREE_NODE pFTn){
   PHOIT_FRAG_TREE_LIST_NODE pFTlistNode;
   pFTlistNode = (PHOIT_FRAG_TREE_LIST_NODE)lib_malloc(sizeof(HOIT_FRAG_TREE_LIST_NODE));
   pFTlistNode->pFTn = pFTn;
   pFTlistNode->pFTlistNext = LW_NULL;
   return pFTlistNode;
}

/*********************************************************************************************************
 初始化节点链表
*********************************************************************************************************/
static inline PHOIT_FRAG_TREE_LIST_HEADER hoitFragTreeListInit(VOID){
    PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader;
    pFTlistHeader = (PHOIT_FRAG_TREE_LIST_HEADER)lib_malloc(sizeof(HOIT_FRAG_TREE_LIST_HEADER));
    pFTlistHeader->uiHighBound = INT_MAX;
    pFTlistHeader->uiLowBound = INT_MIN;
    pFTlistHeader->pFTlistHeader = newFragTreeListNode(LW_NULL);
    pFTlistHeader->uiNCnt = 0;
    return pFTlistHeader;
}
/*********************************************************************************************************
 向链表中插入节点
*********************************************************************************************************/
static inline VOID hoitFragTreeListInsertNode(PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader, PHOIT_FRAG_TREE_LIST_NODE pTFlistNode){
    PHOIT_FRAG_TREE_LIST_NODE pFTlistTrailling;
    PHOIT_FRAG_TREE_LIST_NODE pFTlistTraverse = pFTlistHeader->pFTlistHeader;
    while (pFTlistTraverse != LW_NULL)
    {
        pFTlistTrailling = pFTlistTraverse;
        pFTlistTraverse = pFTlistTraverse->pFTlistNext;
    }
    pFTlistTrailling->pFTlistNext = pTFlistNode;
}
/*********************************************************************************************************
 删除链表中某节点，不释放内存
*********************************************************************************************************/
static inline VOID hoitFragTreeListDeleteNode(PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader, PHOIT_FRAG_TREE_LIST_NODE pTFlistNode){
    PHOIT_FRAG_TREE_LIST_NODE pFTlistTrailling;
    PHOIT_FRAG_TREE_LIST_NODE pFTlistTraverse;

    pFTlistTraverse = pFTlistHeader->pFTlistHeader;
    pFTlistTrailling = pFTlistTraverse;

    while (pFTlistTraverse != LW_NULL)
    {
        if(pFTlistTraverse == pTFlistNode){
            pFTlistTrailling->pFTlistNext = pFTlistTraverse->pFTlistNext;
            pFTlistTraverse->pFTlistNext = LW_NULL;
            break;
        }
        pFTlistTrailling = pFTlistTraverse;
        pFTlistTraverse = pFTlistTraverse->pFTlistNext;
    }
}
/*********************************************************************************************************
 删除链表
*********************************************************************************************************/
static inline VOID hoitFragTreeListFree(PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader){
    PHOIT_FRAG_TREE_LIST_NODE pFTlistTraverse;
    PHOIT_FRAG_TREE_LIST_NODE pFTlistNext;

    pFTlistTraverse = pFTlistHeader->pFTlistHeader;
    while (pFTlistTraverse != LW_NULL)
    {
        pFTlistNext = pFTlistTraverse->pFTlistNext;
        lib_free(pFTlistTraverse);
        pFTlistTraverse = pFTlistNext;
    }
    lib_free(pFTlistHeader);
}
PHOIT_FRAG_TREE_LIST_HEADER   hoitFragTreeCollectRange(PHOIT_FRAG_TREE pFTTree, INT32 iKeyLow, INT32 iKeyHigh);
#endif  /* not FT_OBSOLETE_TREE_LIST */

/*********************************************************************************************************
  FragTree基本操作 - 线程不安全 - 不同进程同时打开文件，需要做锁操作
*********************************************************************************************************/
PHOIT_FRAG_TREE               hoitInitFragTree(PHOIT_VOLUME pfs);                                                                           /* 初始化树 */
PHOIT_FRAG_TREE_NODE          hoitFragTreeInsertNode(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn);                       /* 插入一个节点 */
PHOIT_FRAG_TREE_NODE          hoitFragTreeSearchNode(PHOIT_FRAG_TREE pFTTree, INT32 iKey);                                      /* 查找一个节点 */
BOOL                          hoitFragTreeDeleteNode(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn, BOOL bDoDelete);                       /* 删除一个节点 */
BOOL                          hoitFragTreeDeleteRange(PHOIT_FRAG_TREE pFTTree, INT32 iKeyLow, INT32 iKeyHigh, BOOL bDoDelete);
BOOL                          hoitFragTreeDeleteTree(PHOIT_FRAG_TREE pFTTree, BOOL bDoDelete);
VOID                          hoitFragTreeTraverse(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot);                     /* 中序遍历FragTree */

//TODO: 获取任意一个节点
PHOIT_FRAG_TREE_NODE          hoitFragTreeGetLastNode(PHOIT_FRAG_TREE pFTTree);
VIS_STATUE                    hoitFragTreeTraverseVisitor(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot, visitorHoitFragTree visitor, PVOID pUserValue, PVOID *ppReturn);
VOID                          hoitFragTreeShowMemory(PHOIT_FRAG_TREE pFTTree);
/*********************************************************************************************************
  FragTree
*********************************************************************************************************/
typedef struct hoit_frag_tree_read_param
{
    UINT32 uiOfs;
    UINT32 uiSize;
} HOIT_FRAG_TREE_READ_PARAM;

typedef struct hoit_frag_tree_overlayfixup_param
{
    PHOIT_FRAG_TREE_NODE pFTn;
    BOOL                 bDoDelete;
} HOIT_FRAG_TREE_OVERLAY_FIXUP_RARAM;
//TODO：读取FragTree，然后向下读取数据实体，基本逻辑为先读Cache，Cache未命中再读flash
//!该部分可以移除
BOOL hoitFragTreeRead(PHOIT_FRAG_TREE pFTTree, UINT32 uiOfs, UINT32 uiSize, PCHAR pContent);


//TODO：搜索FragTree，寻找Overlay - 4种情况，然后做出相应修改， 避免掉电，每次构建树后都调用一次FixUp
BOOL hoitFragTreeOverlayFixUp(PHOIT_FRAG_TREE pFTTree);

//!该部分移除
////写入FragTree，然后调用搜索FragTree进行重叠检测与修改，然后写入Cache，Cache未命中，先读出再写入，命中直接写入，写满后Flush进flash，这里写入Cache的时候可以做一些备份，例如Trascation或者Log
//// BOOL hoitFragTreeWrite(PHOIT_FRAG_TREE pFTTree, UINT32 uiOfs, UINT32 uiSize, PCHAR pContent);

#ifdef FT_TEST
VOID hoitFTTreeTest();
#endif // DEBUG


#endif /* SYLIXOS_EXTFS_HOITFS_HOITFSTREE_H_ */
