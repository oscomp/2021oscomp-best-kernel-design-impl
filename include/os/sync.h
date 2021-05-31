
#ifndef INCLUDE_SYNC_H_
#define INCLUDE_SYNC_H_

#include <os/lock.h>

/* locks */
#define NUM_MAX_MUTEX_LOCK 100
#define NUM_MAX_BARRIER 4
#define NUM_MAX_COND 4
#define NUM_MAX_SEM 4

/* binsem op */
#define BINSEM_OP_LOCK 0 // mutex acquire
#define BINSEM_OP_UNLOCK 1 // mutex release

/* all kinds of locks */
extern mutex_lock_t mutex_lock[NUM_MAX_MUTEX_LOCK];
extern barrier_t barrier[NUM_MAX_BARRIER];
extern cond_t cond[NUM_MAX_COND];
extern semaphore_t semaphore[NUM_MAX_SEM];

unsigned do_barrier_get(unsigned count);
int do_barrier_wait(unsigned barrier_id);
int do_barrier_destroy(int barrier_id);


int do_cond_get_u(int key);
int do_cond_wait_u(int cond_id,int binsem_id);
int do_cond_signal_u(int cond_id);
int do_cond_broadcast_u(int cond_id);
int do_cond_destroy_u(int cond_id);

void do_cond_init(cond_t *condv);
int do_cond_wait(cond_t *condv,mutex_lock_t *lock);
int do_cond_signal(cond_t *condv);
int do_cond_broadcast(cond_t *condv);
int do_cond_destroy(cond_t *condv);


int do_semaphore_get(int key,int val);
int do_semaphore_up(int sem_id);
int do_semaphore_down(int sem_id);
int do_semaphore_destroy(int sem_id);

#endif
