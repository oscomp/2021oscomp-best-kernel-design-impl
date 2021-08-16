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
** 文   件   名: HoitMergeBuffer.c
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 07 月 10 日
**
** 描        述: Hoit文件系统为每个文件建立的MergeBuffer的函数库, 用于合并写的小数据
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#include "hoitMergeBuffer.h"
#include "hoitFsLib.h"
#include "hoitFsTree.h"

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0

#ifndef HOITFSLIB_DISABLE

/*********************************************************************************************************
** 函数名称: __hoit_new_merge_buffer
** 功能描述: 为一个文件新建MergeBuffer
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_new_merge_buffer(PHOIT_INODE_INFO pInodeInfo) {
    PHOIT_VOLUME    pfs = pInodeInfo->HOITN_volume;
    if (pInodeInfo->HOITN_pMergeBuffer == LW_NULL) {
        pInodeInfo->HOITN_pMergeBuffer              = (PHOIT_MERGE_BUFFER)hoit_malloc(pfs, sizeof(HOIT_MERGE_BUFFER));
        pInodeInfo->HOITN_pMergeBuffer->pList       = LW_NULL;
        pInodeInfo->HOITN_pMergeBuffer->threshold   = HOIT_MERGE_BUFFER_THRESHOLD;
        pInodeInfo->HOITN_pMergeBuffer->size        = 0;
    }
    return LW_TRUE;
}
/*********************************************************************************************************
** 函数名称: __hoit_new_merge_entry
** 功能描述: 为一个文件的一个红黑树节点新建一个WriteEntry
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_new_merge_entry(PHOIT_INODE_INFO pInodeInfo, PHOIT_MERGE_BUFFER pMergeBuffer, PHOIT_FRAG_TREE_NODE pTreeNode) {
    /* 没有相关的Entry, 新建一个Entry */
    PHOIT_VOLUME      pfs = pInodeInfo->HOITN_volume;
    PHOIT_MERGE_ENTRY pMergeEntry = (PHOIT_MERGE_ENTRY)hoit_malloc(pfs, sizeof(HOIT_MERGE_ENTRY));
    pMergeEntry->pTreeNode = pTreeNode;
    pMergeEntry->pNext = LW_NULL;
    pMergeEntry->pPrev = LW_NULL;
    pTreeNode->pMergeEntry = pMergeEntry;

    if (pMergeBuffer->pList == LW_NULL) {
        pMergeBuffer->pList = pMergeEntry;
        pMergeBuffer->size += 1;
    }
    else {
        pMergeEntry->pNext = pMergeBuffer->pList;
        pMergeBuffer->pList->pPrev = pMergeEntry;
        pMergeBuffer->pList = pMergeEntry;
        pMergeBuffer->size += 1;
    }
    if (pMergeBuffer->size >= pMergeBuffer->threshold) {    /* 当MergeBuffer中链接的数据节点数目大于阈值时进行合并 */
        __hoit_refresh_merge_buffer(pInodeInfo);
    }

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_del_merge_entry
** 功能描述: 
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_del_merge_entry(PHOIT_VOLUME pfs, PHOIT_MERGE_BUFFER pMergeBuffer, PHOIT_MERGE_ENTRY pMergeEntry) {
    if (pMergeEntry == LW_NULL) {
        return LW_FALSE;
    }
    if (pMergeEntry->pPrev) {
        pMergeEntry->pPrev->pNext = pMergeEntry->pNext;
    }
    if (pMergeEntry->pNext) {
        pMergeEntry->pNext->pPrev = pMergeEntry->pPrev;
    }

    if (pMergeBuffer->pList == pMergeEntry) {
        pMergeBuffer->pList = pMergeEntry->pNext;
    }

    hoit_free(pfs, pMergeEntry, sizeof(HOIT_MERGE_ENTRY));
    pMergeBuffer->size -=1 ;
    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_free_merge_buffer
** 功能描述:
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_free_merge_buffer(PHOIT_INODE_INFO pInodeInfo) {
    PHOIT_VOLUME    pfs = pInodeInfo->HOITN_volume;
    if (pInodeInfo == LW_NULL || pInodeInfo->HOITN_pMergeBuffer == LW_NULL) {
        return LW_FALSE;
    }

    PHOIT_MERGE_BUFFER pMergeBuffer     = pInodeInfo->HOITN_pMergeBuffer;
    PHOIT_MERGE_ENTRY pNowWriteEntry    = pMergeBuffer->pList;
    PHOIT_MERGE_ENTRY pNextWriteEntry   = LW_NULL;
    while (pNowWriteEntry) {
        pNextWriteEntry = pNowWriteEntry->pNext;
        hoit_free(pfs, pNowWriteEntry, sizeof(HOIT_MERGE_ENTRY));
        pNowWriteEntry = pNextWriteEntry;
    }
    hoit_free(pfs, pMergeBuffer, sizeof(HOIT_MERGE_BUFFER));
    pInodeInfo->HOITN_pMergeBuffer = LW_NULL;

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_clear_merge_buffer
** 功能描述: 将MergeBuffer中的所有Entry清空, 避免链表过长耗时太大
** 输　入  : 
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_clear_merge_buffer(PHOIT_INODE_INFO pInodeInfo){
    PHOIT_VOLUME       pfs = pInodeInfo->HOITN_volume; 
    if (pInodeInfo == LW_NULL || pInodeInfo->HOITN_pMergeBuffer == LW_NULL) {
        return LW_FALSE;
    }

    PHOIT_MERGE_BUFFER pMergeBuffer     = pInodeInfo->HOITN_pMergeBuffer;
    PHOIT_MERGE_ENTRY pNowWriteEntry    = pMergeBuffer->pList;
    PHOIT_MERGE_ENTRY pNextWriteEntry   = LW_NULL;
    while (pNowWriteEntry) {
        pNextWriteEntry = pNowWriteEntry->pNext;
        hoit_free(pfs, pNowWriteEntry, sizeof(HOIT_MERGE_ENTRY));
        pNowWriteEntry = pNextWriteEntry;
    }

    pMergeBuffer->pList = LW_NULL;
    pMergeBuffer->size = 0;

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_refresh_merge_buffer
** 功能描述: 将MergeBuffer中的已有的所有的数据进行相邻合并
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_refresh_merge_buffer(PHOIT_INODE_INFO pInodeInfo) {
    PHOIT_MERGE_BUFFER pMergeBuffer = pInodeInfo->HOITN_pMergeBuffer;
    PHOIT_VOLUME       pfs          = pInodeInfo->HOITN_volume;
    INT32   i;
    if (pMergeBuffer == LW_NULL) {
        return LW_FALSE;
    }
    /* 冒泡排序 */
    for (i = 0; i < pMergeBuffer->size-1; i++) {
        PHOIT_MERGE_ENTRY pNowEntry = pMergeBuffer->pList;
        PHOIT_MERGE_ENTRY pNextEntry = LW_NULL;
        while(pNowEntry)
        {
            pNextEntry = pNowEntry->pNext;
            /* 修复small write越来越慢的原因: MergeBuffer没有删除那些已经失效的Entry */
            if(pNowEntry->pTreeNode == LW_NULL){
                __hoit_del_merge_entry(pfs, pMergeBuffer, pNowEntry);
                pNowEntry = pNextEntry;
                continue;
            }
            if (pNextEntry) {
                if (pNowEntry->pTreeNode->uiOfs > pNextEntry->pTreeNode->uiOfs) {
                    PHOIT_FRAG_TREE_NODE pTempNode = pNowEntry->pTreeNode;
                    pNowEntry->pTreeNode = pNextEntry->pTreeNode;
                    pNextEntry->pTreeNode = pTempNode;
                }
            }
            pNowEntry = pNextEntry;
        }
    }

    /* TODO */
    PHOIT_MERGE_ENTRY pNowEntry     = pMergeBuffer->pList;
    PHOIT_MERGE_ENTRY pLastEntry    = LW_NULL;
    UINT32 left                     = -1;
    UINT32 right                    = -1;
    UINT32 count                    = 0;
    for (; pNowEntry; pNowEntry = pNowEntry->pNext)
    {
        pLastEntry = pNowEntry; /* pLastEntry负责记录最后一个Entry */

        if (left == -1) {
            left    = pNowEntry->pTreeNode->uiOfs;
            right   = left + pNowEntry->pTreeNode->uiSize;
            count   = 1;
        }
        else {
            if (right == pNowEntry->pTreeNode->uiOfs) {
                right += pNowEntry->pTreeNode->uiSize;
                count += 1;
            }
            else {
                if (count > 1) {    /* 有多个节点可以进行合并 */
                    int jump_count = count;
                    while (jump_count) {
                        pLastEntry = pLastEntry->pPrev;
                        jump_count -= 1;
                    }
                    char* pvBuffer = (char*)hoit_malloc(pfs, right - left);
                    hoitFragTreeRead(pInodeInfo->HOITN_rbtree, left, right-left, pvBuffer);
                    __hoit_write(pInodeInfo, pvBuffer, right - left, left, 0);
                    lib_free(pvBuffer);
                }
                left = pNowEntry->pTreeNode->uiOfs;
                right = left + pNowEntry->pTreeNode->uiSize;
                count = 1;
            }
        }
    }
    if (count > 1) {    /* 有多个节点可以进行合并 */
        int jump_count = count;
        while (jump_count) {
            pLastEntry = pLastEntry->pPrev;
            jump_count -= 1;
        }
        char* pvBuffer = (char*)hoit_malloc(pfs, right - left);
        hoitFragTreeRead(pInodeInfo->HOITN_rbtree, left, right - left, pvBuffer);
        __hoit_write(pInodeInfo, pvBuffer, right - left, left, 0);
        lib_free(pvBuffer);
    }
    /* MergeBuffer过长, 采取全部情空的策略 */
    if(pMergeBuffer->size > 100){
        __hoit_clear_merge_buffer(pInodeInfo);
    }
    return LW_TRUE;
}


#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
#endif //HOITFSLIB_DISABLE
