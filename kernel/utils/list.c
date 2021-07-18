// A generic two-direction list definition

#include "utils/list.h"

list_node_t *_list_push_front(list_node_t **phead, list_node_t *node) {
	if (NULL == node) return NULL;

	node->pprev = phead;
	node->next = *phead;
	if (NULL != *phead) {
		(*phead)->pprev = &(node->next);
	}
	*phead = node;

	return node;
}

list_node_t *_list_push_back(list_node_t **phead, list_node_t *node) {
	if (NULL == node) return NULL;

	list_node_t *tmp = *phead;
	while (NULL != tmp) {
		phead = &(tmp->next);
		tmp = tmp->next;
	}

	// now tmp is the last node 
	node->pprev = phead;
	node->next = NULL;
	*phead = node;

	return node;
}

list_node_t *_list_insert(list_node_t *prev, list_node_t *node) {
	if (NULL == node) return NULL;

	node->pprev = &(prev->next);
	node->next = prev->next;
	if (NULL != prev->next) {
		prev->next->pprev = &(node->next);
	}
	prev->next = node;

	return node;
}

void _list_remove(list_node_t *node) {
	if (NULL == node) return ;

	//! node must be in a list
	*(node->pprev) = node->next;
	if (NULL != node->next) {
		node->next->pprev = node->pprev;
	}
	node->pprev = NULL;
	node->next = NULL;
}