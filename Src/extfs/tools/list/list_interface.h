/*
 * list_interface.h
 *
 *  Created on: Jun 2, 2021
 *      Author: Administrator
 */

#ifndef SYLIXOS_EXTFS_TOOLS_LIST_LIST_INTERFACE_H_
#define SYLIXOS_EXTFS_TOOLS_LIST_LIST_INTERFACE_H_
#include "list_template.h"
/********************************************************************************************************* 
  列表接口声明                                                                                          
*********************************************************************************************************/ 
#define List(TYPE) List##TYPE*
#define InitList_(list, NAMESPACE, TYPE)\
{\
    list = (List##TYPE *)lib_malloc(sizeof(List##TYPE));\
    list->listHeader.next = LW_NULL;\
    list->listHeader.prev = LW_NULL;\
    list->append = NAMESPACE##listAppend##TYPE;\
    list->insert = NAMESPACE##listInsert##TYPE;\
    list->removeIndex = NAMESPACE##listRemoveIndex##TYPE;\
    list->removeObject = NAMESPACE##listRemoveObject##TYPE;\
    list->size = NAMESPACE##listSize##TYPE;\
}
#define FreeList(list)\
{\
    UINT uiSize = list->uiSize;\
    UINT i;\
    for (i = 0; i < uiSize; i++)\
    {\
        list->removeIndex(list, 0);\
    }\
    lib_free(list);\
}

#define InitList(list,NAMESPACE, TYPE)    InitList_(list,NAMESPACE, TYPE)
/********************************************************************************************************* 
  迭代器声明                                                                                          
*********************************************************************************************************/ 
#define Iterator(TYPE) Iterator##TYPE*
#define InitIterator_(iter,NAMESPACE, TYPE)\
{\
    iter = (Iterator##TYPE*)lib_malloc(sizeof(Iterator##TYPE));\
    iter->begin = NAMESPACE##iterBegin##TYPE;\
    iter->get = NAMESPACE##iterGet##TYPE;\
    iter->isValid = NAMESPACE##iterIsValid##TYPE;\
    iter->next = NAMESPACE##iterNext##TYPE;\
}
#define FreeIterator(iter) lib_free(iter);

#define InitIterator(iter,NAMESPACE, TYPE)    InitIterator_(iter,NAMESPACE, TYPE)
#endif /* SYLIXOS_EXTFS_TOOLS_LIST_LIST_INTERFACE_H_ */
