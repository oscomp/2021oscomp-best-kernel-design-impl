
#ifndef FUTEX_H
#define FUTEX_H

#include <os/list.h>
#include <os/lock.h>
#include <type.h>

#define FUTEX_BUCKETS 100

typedef uint64_t futex_key_t;

typedef struct futex_node
{
    futex_key_t futex_key;
    list_node_t list;
    list_head block_queue;
}futex_node_t;

typedef list_head futex_bucket_t;

extern futex_bucket_t futex_buckets[FUTEX_BUCKETS];

extern void init_system_futex();
/* Block if *val_addr == val else do nothing.
 * wait until a futex_wakeup is called.
 */
extern void futex_wait(volatile uint64_t *val_addr, uint64_t val);
/* Wake up at most `num_wakeup` threads. */
extern void futex_wakeup(volatile uint64_t *val_addr, int num_wakeup);

#endif /* FUTEX_H */
