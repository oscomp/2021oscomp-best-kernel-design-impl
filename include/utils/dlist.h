#ifndef __DLIST_H
#define __DLIST_H

#include "types.h"

struct d_list {
	struct d_list *prev, *next;
};

#define	dlist_entry(ptr, type, member) \
				container_of(ptr, type, member)


/**
 * Auxiliary method. Don't recommend to use directly.
 */

static inline void __dlist_insert(struct d_list *node, struct d_list *prev, struct d_list *next) {
	prev->next = next->prev = node;
	node->next = next;
	node->prev = prev;
}

static inline void __dlist_link(struct d_list *prev, struct d_list *next) {
	prev->next = next;
	next->prev = prev;
}


/**
 * Simple functions. We don't check legality inside them.
 */

static inline void dlist_init(struct d_list *node) {
	node->prev = node->next = node;
}

static inline int dlist_empty(struct d_list *head) {
	return head->next == head;
}

static inline void dlist_add_after(struct d_list *afterme, struct d_list *node) {
	__dlist_insert(node, afterme, afterme->next);
}

static inline void dlist_add_before(struct d_list *beforeme, struct d_list *node) {
	__dlist_insert(node, beforeme->prev, beforeme);
}

static inline void dlist_del(struct d_list *node) {
	__dlist_link(node->prev, node->next);
}

static inline struct d_list *dlist_prev(struct d_list *node) {
	return node->prev;
}

static inline struct d_list *dlist_next(struct d_list *node) {
	return node->next;
}


#endif