#include <os/futex.h>
#include <os/irq.h>
#include <os/mm.h>
#include <assert.h>

futex_bucket_t futex_buckets[FUTEX_BUCKETS];

void init_system_futex()
{
    disable_preempt();
    for (int i = 0; i < FUTEX_BUCKETS; ++i) {
        init_list_head(&futex_buckets[i]);
    }
    enable_preempt();
}

static int futex_hash(uint64_t x)
{
    // a simple hash function
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ul;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebul;
    x = x ^ (x >> 31);
    return x % FUTEX_BUCKETS;
}

static futex_node_t* get_node(volatile uint64_t *val_addr, int create)
{
    int key = futex_hash((uint64_t)val_addr);
    list_node_t *head = &futex_buckets[key];
    for (list_node_t *p = head->next; p != head; p = p->next) {
        futex_node_t *node = list_entry(p, futex_node_t, list);
        if (node->futex_key == (uint64_t)val_addr) {
            return node;
        }
    }

    if (create) {
        futex_node_t *node = (futex_node_t*) kmalloc(sizeof(futex_node_t));
        node->futex_key = (uint64_t)val_addr;
        init_list_head(&node->block_queue);
        list_add_tail(&node->list, &futex_buckets[key]);
        return node;
    }

    return NULL;
}

void futex_wait(volatile uint64_t *val_addr, uint64_t val)
{
    disable_preempt();

    futex_node_t *node = get_node(val_addr,1);
    assert(node != NULL);

    if (*val_addr == val) {
        do_block(&current_running->list, &node->block_queue);
        do_scheduler();
    }

    enable_preempt();
}

void futex_wakeup(volatile uint64_t *val_addr, int num_wakeup)
{
    disable_preempt();
    futex_node_t *node = get_node(val_addr, 0);

    if (node != NULL) {
        for (int i = 0; i < num_wakeup; ++i) {
            if (list_empty(&node->block_queue)) break;

            do_unblock(node->block_queue.next);
        }
    }

    do_scheduler();
    enable_preempt();
}
