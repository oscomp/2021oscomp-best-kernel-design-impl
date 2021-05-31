
#ifndef INCLUDE_LOCK_H_
#define INCLUDE_LOCK_H_

#include <os/list.h>

typedef enum {
    UNLOCKED,
    LOCKED,
} lock_status_t;

typedef struct spin_lock
{
    volatile lock_status_t status;
} spin_lock_t;

typedef struct mutex_lock
{
    spin_lock_t lock;
    list_head block_queue;
    pid_t pid;
    int valid;
} mutex_lock_t;

typedef struct barrier
{
	list_head block_queue;
	unsigned block_count;
	unsigned block_num;
	int valid;
}barrier_t;

typedef struct cond
{
	list_head block_queue;
	int valid;
}cond_t;

typedef struct semaphore
{
	list_head block_queue;
	volatile int val;
	int valid;
}semaphore_t;

/* init lock */
void spin_lock_init(spin_lock_t *lock);
int spin_lock_try_acquire(spin_lock_t *lock);
void spin_lock_acquire(spin_lock_t *lock);
void spin_lock_release(spin_lock_t *lock);

void do_mutex_lock_init(mutex_lock_t *lock);
void do_mutex_lock_acquire(mutex_lock_t *lock);
void do_mutex_lock_release(mutex_lock_t *lock);
void do_mutex_lock_destroy(mutex_lock_t *lock);

int do_binsemget(int key);
int do_binsemop(int binsem_id,int op);
int do_binsem_destroy(int binsem_id);

int is_bq_empty(list_node_t *head);

#endif
