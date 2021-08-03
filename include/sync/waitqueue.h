#ifndef __WAIT_QUEUE_H
#define __WAIT_QUEUE_H

#include "utils/dlist.h"
#include "sync/spinlock.h"


/**
 * A simple wait queue implemented via doubly list.
 * When getting resource, join a wait_node to the
 * wait_queue. If we are the first node in the queue,
 * then we can access the resource, if not, then we
 * go to sleep. Once we are done, we remove our node
 * from the queue and wake up the next node.
 */

struct wait_queue {
	struct spinlock lock;
	struct d_list head;
};

struct wait_node {
	void *chan;
	struct d_list list;
};


/**
 * Simple inline functions that don't bother to check arguments.
 */

static inline void wait_queue_init(struct wait_queue *wq, char *str)
{
	initlock(&wq->lock, str);
	dlist_init(&wq->head);
}

static inline int wait_queue_empty(struct wait_queue *wq) {
	return dlist_empty(&wq->head);
}

static inline struct wait_node *
wait_queue_next(struct wait_queue *wq) {
	return container_of(wq->head.next, struct wait_node, list);
}

static inline int wait_queue_is_first(struct wait_queue *wq, struct wait_node *node) {
	return wq->head.next == &node->list;
}

// This goes without locking. Add node to the last.
static inline void wait_queue_add(struct wait_queue *wq, struct wait_node *node) {
	dlist_add_before(&wq->head, &node->list);
}

static inline void wait_queue_del(struct wait_node *node) {
	dlist_del(&node->list);
}

static inline void wait_queue_add_locked(struct wait_queue *wq, struct wait_node *node)
{
	acquire(&wq->lock);
	dlist_add_before(&wq->head, &node->list);
	release(&wq->lock);
}

static inline void wait_queue_del_locked(struct wait_queue *wq, struct wait_node *node)
{
	acquire(&wq->lock);
	dlist_del(&node->list);
	release(&wq->lock);
}


#endif
