/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Synchronous primitive related content implementation,
 *                 such as: locks, barriers, semaphores, etc.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

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
