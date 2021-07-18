#ifndef __UTIL_LIST_H
#define __UTIL_LIST_H

#include "types.h"

// this struct should never be used directly 
struct __list_node {
	struct __list_node *next;
	struct __list_node **pprev;
};
typedef struct __list_node list_node_t;

list_node_t *_list_push_front(list_node_t **phead, list_node_t *node);
list_node_t *_list_push_back(list_node_t **phead, list_node_t *node);
list_node_t *_list_insert(list_node_t *prev, list_node_t *node);
void _list_remove(list_node_t *node);

#define list_push_front(phead, node) \
	(_list_push_front((list_node_t**)(phead), (list_node_t*)(node)))
#define list_push_back(phead, node) \
	(_list_push_back((list_node_t**)(phead), (list_node_t*)(node)))
#define list_insert(prev, node) \
	(_list_insert((list_node_t*)(prev), (list_node_t*)(node)))
#define list_remove(node) \
	_list_remove((list_node_t**)(node))

// get each field of node 
#define __next_field(node) \
	(((list_node_t*)(node))->next)
#define __prev_field(node) \
	(((list_node_t*)(node))->prev)

// do not check whether node == NULL
#define list_next(type, node) \
	((type*)__next_field(node))
#define list_prev(type, node) \
	((type*)( \
		NULL != __prev_field(node) ? \
		*__prev_field(node): \
		NULL \
	))

#endif 