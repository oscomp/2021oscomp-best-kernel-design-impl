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
** 文   件   名: HoitWriteBuffer.c
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 07 月 10 日
**
** 描        述: Hoit文件系统为每个文件建立的WriteBuffer的函数库, 用于合并写的小数据
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#include "hoitWriteBuffer.h"
#include "hoitFsLib.h"
#include "hoitFsTree.h"

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0

#ifndef HOITFSLIB_DISABLE

/*********************************************************************************************************
** 函数名称: __hoit_new_write_buffer
** 功能描述: 为一个文件新建WriteBuffer
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_new_write_buffer(PHOIT_INODE_INFO pInodeInfo) {
    if (pInodeInfo->HOITN_pWriteBuffer == LW_NULL) {
        pInodeInfo->HOITN_pWriteBuffer              = (PHOIT_WRITE_BUFFER)lib_malloc(sizeof(HOIT_WRITE_BUFFER));
        pInodeInfo->HOITN_pWriteBuffer->pList       = LW_NULL;
        pInodeInfo->HOITN_pWriteBuffer->threshold   = HOIT_WRITE_BUFFER_THRESHOLD;
        pInodeInfo->HOITN_pWriteBuffer->size        = 0;
    }
    return LW_TRUE;
}
/*********************************************************************************************************
** 函数名称: __hoit_new_write_entry
** 功能描述: 为一个文件的一个红黑树节点新建一个WriteEntry
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_new_write_entry(PHOIT_INODE_INFO pInodeInfo, PHOIT_WRITE_BUFFER pWriteBuffer, PHOIT_FRAG_TREE_NODE pTreeNode) {
    /* 没有相关的Entry, 新建一个Entry */
    PHOIT_WRITE_ENTRY pWriteEntry = (PHOIT_WRITE_ENTRY)lib_malloc(sizeof(HOIT_WRITE_ENTRY));
    pWriteEntry->pTreeNode = pTreeNode;
    pWriteEntry->pNext = LW_NULL;
    pWriteEntry->pPrev = LW_NULL;
    pTreeNode->pWriteEntry = pWriteEntry;

    if (pWriteBuffer->pList == LW_NULL) {
        pWriteBuffer->pList = pWriteEntry;
        pWriteBuffer->size += 1;
    }
    else {
        pWriteEntry->pNext = pWriteBuffer->pList;
        pWriteBuffer->pList->pPrev = pWriteEntry;
        pWriteBuffer->pList = pWriteEntry;
        pWriteBuffer->size += 1;
    }
    if (pWriteBuffer->size >= pWriteBuffer->threshold) {    /* 当WriteBuffer中链接的数据节点数目大于阈值时进行合并 */
        __hoit_refresh_write_buffer(pInodeInfo);
    }

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_del_write_entry
** 功能描述: 
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_del_write_entry(PHOIT_WRITE_BUFFER pWriteBuffer, PHOIT_WRITE_ENTRY pWriteEntry) {
    if (pWriteEntry == LW_NULL) {
        return LW_FALSE;
    }
    if (pWriteEntry->pPrev) {
        pWriteEntry->pPrev->pNext = pWriteEntry->pNext;
    }
    if (pWriteEntry->pNext) {
        pWriteEntry->pNext->pPrev = pWriteEntry->pPrev;
    }

    if (pWriteBuffer->pList == pWriteEntry) {
        pWriteBuffer->pList = pWriteEntry->pNext;
    }

    __SHEAP_FREE(pWriteEntry);

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_free_write_buffer
** 功能描述:
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_free_write_buffer(PHOIT_INODE_INFO pInodeInfo) {
    if (pInodeInfo == LW_NULL || pInodeInfo->HOITN_pWriteBuffer == LW_NULL) {
        return LW_FALSE;
    }

    PHOIT_WRITE_BUFFER pWriteBuffer     = pInodeInfo->HOITN_pWriteBuffer;
    PHOIT_WRITE_ENTRY pNowWriteEntry    = pWriteBuffer->pList;
    PHOIT_WRITE_ENTRY pNextWriteEntry   = LW_NULL;
    while (pNowWriteEntry) {
        pNextWriteEntry = pNowWriteEntry->pNext;
        __SHEAP_FREE(pNowWriteEntry);
        pNowWriteEntry = pNextWriteEntry;
    }
    __SHEAP_FREE(pWriteBuffer);
    pInodeInfo->HOITN_pWriteBuffer = LW_NULL;

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_refresh_write_buffer
** 功能描述: 将WriteBuffer中的已有的所有的数据进行相邻合并
** 输　入  : 
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_refresh_write_buffer(PHOIT_INODE_INFO pInodeInfo) {
    PHOIT_WRITE_BUFFER pWriteBuffer = pInodeInfo->HOITN_pWriteBuffer;
    INT32   i;
    if (pWriteBuffer == LW_NULL) {
        return LW_FALSE;
    }
    /* 先排序 */
    for (i = 0; i < pWriteBuffer->size-1; i++) {
        PHOIT_WRITE_ENTRY pNowEntry = pWriteBuffer->pList;
        PHOIT_WRITE_ENTRY pNextEntry = LW_NULL;
        while(pNowEntry)
        {
            pNextEntry = pNowEntry->pNext;
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
    PHOIT_WRITE_ENTRY pNowEntry     = pWriteBuffer->pList;
    PHOIT_WRITE_ENTRY pLastEntry    = LW_NULL;
    UINT32 left     = -1;
    UINT32 right    = -1;
    UINT32 count    = 0;
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
                    char* pvBuffer = (char*)lib_malloc(right - left);
                    hoitFragTreeRead(pInodeInfo->HOITN_rbtree, left, right-left, pvBuffer);
                    __hoit_write(pInodeInfo, pvBuffer, right - left, left, 0);
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
        char* pvBuffer = (char*)lib_malloc(right - left);
        hoitFragTreeRead(pInodeInfo->HOITN_rbtree, left, right - left, pvBuffer);
        __hoit_write(pInodeInfo, pvBuffer, right - left, left, 0);
    }
    return LW_TRUE;
}


#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
#endif //HOITFSLIB_DISABLE
