/*
 * list_test.c
 *
 *  Created on: Jun 2, 2021
 *      Author: Administrator
 */
#include "common.h"

#define     NS list_test

USE_LIST_TEMPLATE(NS, TYPE);

VOID listTest(){
    INT i;
    // List* list = initList();
    // Iterator* iter = initIterator();
    List(TYPE) list;
    Iterator(TYPE) iter;
    
    InitList(list, NS, TYPE);
    InitIterator(iter, NS, TYPE);

    
    printf("[LIST TEST]\n");
    printf("[Test1: Append Test] \n");
    for (i = 0; i < 10; i++)
    {
        TYPE *data = (TYPE *)lib_malloc(sizeof(TYPE));
        data->a = i;
        list->append(list, data);
    }
    //0 1 2 .. 9
    printf("size now %d\n", list->size(list));
    for (i = 0; i < 10; i += 2)
    {
        TYPE *data = (TYPE *)lib_malloc(sizeof(TYPE));
        data->a = i + 10;
        list->insert(list, data, i);
    }
    printf("size now %d\n", list->size(list));
    for (iter->begin(iter, list);iter->isValid(iter); iter->next(iter))
    {
        TYPE * data = iter->get(iter);
        printf("%d ", data->a);
    }

    printf("\n[Test2: Remove Test ]\n");
    for (i = 0; i < 10; i++)
    {
        list->removeIndex(list, 0);
    }
    printf("size now %d\n", list->size(list));
    for (iter->begin(iter, list);iter->isValid(iter); iter->next(iter))
    {
        TYPE * data = iter->get(iter);
        printf("%d ", data->a);
    }

    printf("\n[Test3: Remove Object ]\n");
    for (iter->begin(iter, list);iter->isValid(iter); iter->next(iter))
    {
        TYPE * data = iter->get(iter);
        printf("[Removing data %d ...]\n", data->a);
        list->removeObject(list, data);
    }
    printf("\nsize now %d\n", list->size(list));

    printf("\n[Test4: Insert Again ]\n");
    for (i = 0; i < 10; i++)
    {
        TYPE *data = (TYPE *)lib_malloc(sizeof(TYPE));
        data->a = i;
        list->insert(list, data, i);
    }
    printf("size now %d\n", list->size(list));
    for (iter->begin(iter, list);iter->isValid(iter); iter->next(iter))
    {
        TYPE * data = iter->get(iter);
        printf("%d ", data->a);
    }

    printf("\n[Test5: LRU Test ]\n");
    iter->begin(iter, list);
    TYPE * data = iter->get(iter);
    TYPE * newData = (TYPE *)lib_malloc(sizeof(TYPE));
    lib_memcpy(newData, data, sizeof(TYPE));
    printf("[Moving data %d to end...]\n", data->a);
    list->removeObject(list, data);
    list->append(list, newData);
    printf("size now %d\n", list->size(list));
    for (iter->begin(iter, list);iter->isValid(iter); iter->next(iter))
    {
        TYPE * data = iter->get(iter);
        printf("%d ", data->a);
    }

    // freeIterator(iter);
    // freeList(list);

    FreeIterator(iter);
    FreeList(list);
}
