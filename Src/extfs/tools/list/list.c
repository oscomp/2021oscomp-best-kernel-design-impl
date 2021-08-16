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
** 文   件   名: list.h
**
** 创   建   人: 潘延麒
**
** 文件创建日期: 2021 年 06 月 02日
**
** 描        述: 链表模板实现
*********************************************************************************************************/
#ifndef SYLIXOS_EXTFS_TOOLS_LIST_LIST_H_
#define SYLIXOS_EXTFS_TOOLS_LIST_LIST_H_
#include "common.h"


List* initList() {
    List* list = (List *)lib_malloc(sizeof(List));
    list->listHeader.next = LW_NULL;
    list->listHeader.prev = LW_NULL;

    list->append = listAppend;
    list->insert = listInsert;
    list->removeIndex = listRemoveIndex;
    list->removeObject = listRemoveObject;
    list->size = listSize;
    return list;
}

UINT listSize(struct list* self) {
    return self->uiSize;
}

BOOL listInsert(struct list* self, TYPE* data, UINT uiIndex) {
    ListNode* traverse = &self->listHeader;
    ListNode* newNode;
    UINT      uiCounter = 0;

    if(uiIndex > self->uiSize){
        return LW_FALSE;
    }

    newNode = (ListNode *)lib_malloc(sizeof(ListNode));

    if(newNode == LW_NULL){
        return LW_FALSE;
    }

    while (traverse->next != LW_NULL)
    {
        if(uiCounter == uiIndex) break;
        traverse = traverse->next;
        uiCounter++;
    }
    
    newNode->prev = traverse;
    newNode->next = traverse->next;
    newNode->listData = data;
    if(traverse->next != LW_NULL)
        traverse->next->prev = newNode;
    traverse->next = newNode;
    
    self->uiSize += 1;

    return LW_TRUE;
}

BOOL listAppend(struct list* self, TYPE* data) {
    return listInsert(self, data, self->uiSize);
}

BOOL listRemoveObject(struct list* self, TYPE* data) {
    ListNode* traverse = self->listHeader.next;
    while (traverse != LW_NULL)
    {
        if(traverse->listData == data){
            traverse->prev->next = traverse->next;
            if(traverse->next != LW_NULL)
                traverse->next->prev = traverse->prev;
            lib_free(traverse->listData);
            lib_free(traverse);
            self->uiSize -= 1;
            return LW_TRUE;
        }
        traverse = traverse->next;
    }
    printf("%s not find data\n", __func__);
    return LW_FALSE;
}

BOOL listRemoveIndex(struct list* self, UINT uiIndex){
    ListNode* traverse = self->listHeader.next;
    UINT      uiCounter = 0;
    if(uiIndex >= self->uiSize){
        return LW_FALSE;
    }
    while (traverse != LW_NULL)
    {
        if(uiCounter == uiIndex){
            traverse->prev->next = traverse->next;
            if(traverse->next != LW_NULL)
                traverse->next->prev = traverse->prev;
            lib_free(traverse->listData);
            lib_free(traverse);
            self->uiSize -= 1;
            return LW_TRUE;
        }
        traverse = traverse->next;
        uiCounter++;
    }
    return LW_FALSE;
}

VOID freeList(List* list){
    UINT uiSize = list->uiSize;
    UINT i;

    for (i = 0; i < uiSize; i++)
    {
        list->removeIndex(list, 0);
    }
    lib_free(list);
}


#endif /* SYLIXOS_EXTFS_TOOLS_LIST_LIST_H_ */
