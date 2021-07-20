/*
 * list_template.h
 *
 *  Created on: Jun 2, 2021
 *      Author: Administrator
 */

#ifndef SYLIXOS_EXTFS_TOOLS_LIST_LIST_TEMPLATE_H_
#define SYLIXOS_EXTFS_TOOLS_LIST_LIST_TEMPLATE_H_
#include "SylixOS.h"

#define DECLARE_LIST_TEMPLATE(TYPE)\
/*********************************************************************************************************\
  列表相关结构定义\
*********************************************************************************************************/\ 
typedef struct node##TYPE {\
    struct node##TYPE * next;\
    struct node##TYPE * prev;\
    TYPE* listData;\
} ListNode##TYPE;\
\
typedef struct list##TYPE\
{\
    ListNode##TYPE listHeader;\
    UINT     uiSize;\
    UINT     (*size)(struct list##TYPE * self);\
    BOOL     (*append)(struct list##TYPE * self, TYPE * data);\
    BOOL     (*insert)(struct list##TYPE * self, TYPE * data, UINT uiIndex);\
    BOOL     (*removeObject)(struct list##TYPE * self, TYPE * data);\
    BOOL     (*removeIndex)(struct list##TYPE * self, UINT uiIndex);\
} List##TYPE ;\
/*********************************************************************************************************\
  迭代器结构定义\
*********************************************************************************************************/\ 
typedef struct iter##TYPE\
{\
    ListNode##TYPE  *  traverse;\
    VOID        (*begin)(struct iter##TYPE * self, List##TYPE * list##TYPE);\
    BOOL        (*next)(struct iter##TYPE * self);\
    BOOL        (*isValid)(struct iter##TYPE * self);\
    TYPE*       (*get)(struct iter##TYPE * self);\
} Iterator##TYPE ;



#define USE_LIST_TEMPLATE_(NAMESPACE, TYPE)\
/*********************************************************************************************************\
  列表方法定义，使用前需要声明\
*********************************************************************************************************/\ 
UINT NAMESPACE##listSize##TYPE(struct list##TYPE* self) {\
    return self->uiSize;\
}\
BOOL NAMESPACE##listInsert##TYPE(struct list##TYPE* self, TYPE* data, UINT uiIndex) {\
    ListNode##TYPE* traverse = &self->listHeader;\
    ListNode##TYPE* newNode;\
    UINT      uiCounter = 0;\
\
    if(uiIndex > self->uiSize){\
        return LW_FALSE;\
    }\
\
    newNode = (ListNode##TYPE *)lib_malloc(sizeof(ListNode##TYPE));\
\
    if(newNode == LW_NULL){\
        return LW_FALSE;\
    }\
\
    while (traverse->next != LW_NULL)\
    {\
        if(uiCounter == uiIndex) break;\
        traverse = traverse->next;\
        uiCounter++;\
    }\
\
    newNode->prev = traverse;\
    newNode->next = traverse->next;\
    newNode->listData = data;\
    if(traverse->next != LW_NULL)\
        traverse->next->prev = newNode;\
    traverse->next = newNode;\
\
    self->uiSize += 1;\
\
    return LW_TRUE;\
}\
\
BOOL NAMESPACE##listAppend##TYPE(struct list##TYPE* self, TYPE* data) {\
    return NAMESPACE##listInsert##TYPE(self, data, self->uiSize);\
}\
\
BOOL NAMESPACE##listRemoveObject##TYPE(struct list##TYPE* self, TYPE* data) {\
    ListNode##TYPE* traverse = self->listHeader.next;\
    while (traverse != LW_NULL)\
    {\
        if(traverse->listData == data){\
            traverse->prev->next = traverse->next;\
            if(traverse->next != LW_NULL)\
                traverse->next->prev = traverse->prev;\
            lib_free(traverse->listData);\
            lib_free(traverse);\
            self->uiSize -= 1;\
            return LW_TRUE;\
        }\
        traverse = traverse->next;\
    }\
    printf("%s not find data\n", __func__);\
    return LW_FALSE;\
}\
\
BOOL NAMESPACE##listRemoveIndex##TYPE(struct list##TYPE* self, UINT uiIndex){\
    ListNode##TYPE* traverse = self->listHeader.next;\
    UINT      uiCounter = 0;\
    if(uiIndex >= self->uiSize){\
        return LW_FALSE;\
    }\
    while (traverse != LW_NULL)\
    {\
        if(uiCounter == uiIndex){\
            traverse->prev->next = traverse->next;\
            if(traverse->next != LW_NULL)\
                traverse->next->prev = traverse->prev;\
            lib_free(traverse->listData);\
            lib_free(traverse);\
            self->uiSize -= 1;\
            return LW_TRUE;\
        }\
        traverse = traverse->next;\
        uiCounter++;\
    }\
    return LW_FALSE;\
}\
/*********************************************************************************************************\
  迭代器方法\
*********************************************************************************************************/\                       
VOID NAMESPACE##freeIterator##TYPE(Iterator##TYPE* iter){\
    lib_free(iter);\
}\
\
VOID NAMESPACE##iterBegin##TYPE(struct iter##TYPE* self, List##TYPE* list##TYPE){\
    self->traverse = list##TYPE->listHeader.next;\
}\
\
BOOL NAMESPACE##iterNext##TYPE(struct iter##TYPE* self){\
    self->traverse = self->traverse == LW_NULL ? LW_NULL : self->traverse->next;\
    return self->traverse == LW_NULL ? LW_FALSE : LW_TRUE;\
}\
\
BOOL NAMESPACE##iterIsValid##TYPE(struct iter##TYPE* self) {\
    return self->traverse == LW_NULL ? LW_FALSE : LW_TRUE;\
}\
\
TYPE* NAMESPACE##iterGet##TYPE(struct iter##TYPE* self){\
    return self->traverse->listData;\
}

#define USE_LIST_TEMPLATE(NAMESPACE, TYPE)  USE_LIST_TEMPLATE_(NAMESPACE, TYPE)
#endif /* SYLIXOS_EXTFS_TOOLS_LIST_LIST_TEMPLATE_H_ */
