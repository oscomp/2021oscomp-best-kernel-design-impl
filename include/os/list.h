
#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

#include <type.h>

// double-linked list
typedef struct list_node
{
    struct list_node *next, *prev;
    ptr_t ptr;
    pid_t tid;
} list_node_t;

typedef list_node_t list_head;

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define list_entry(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })

// LIST_HEAD is used to define the head of a list.
#define LIST_HEAD(name) struct list_node name = {.next = &(name), .prev = &(name), .ptr = (ptr_t)&pid0_pcb}

static inline void init_list_head(list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void _list_add(
    list_node_t *node, list_node_t *prev, list_node_t *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

static inline void _list_del(list_node_t *prev, list_node_t *next)
{
    next->prev = prev;
    prev->next = next;
}
static inline void list_del(list_node_t *entry)
{
    if (entry->prev != NULL && entry->next != NULL) {
        _list_del(entry->prev, entry->next);
        entry->prev = NULL;
        entry->next = NULL;
    }
}

static inline void list_add(list_node_t *node, list_node_t *head)
{
    _list_add(node, head, head->next);
}

static inline void list_add_tail(
    list_node_t *node, list_node_t *head)
{
    _list_add(node, head->prev, head);
}

static inline void list_move(list_node_t *node, list_head *head)
{
    list_del(node);
    list_add(node, head);
}

static inline int list_empty(const list_head *head)
{
    return head->next == head;
}

// get the *real* node from the list node
#define list_entry(ptr, type, member) \
        (type *)((char *)ptr - offsetof(type, member))

// iterate the list 
#define list_for_each_entry(pos, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = list_entry(pos->member.next, typeof(*pos), member)) 

// iterate the list safely, during which node could be added or removed in the list
#define list_for_each_entry_safe(pos, q, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member), \
            q = list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = q, q = list_entry(pos->member.next, typeof(*q), member))


#endif
