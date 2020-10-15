/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                              A Mini PThread-like library
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

#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <stdint.h>
#include <stdatomic.h>

/* on success, these functions return zero. Otherwise, return an error number */
#define EBUSY  1 /* the lock is busy(for example, it is locked by another thread) */
#define EINVAL 2 /* the lock is invalid */

typedef atomic_int mthread_spinlock_t;

/* A stupid implementation, this will be slow. */
typedef struct mthread_mutex
{
    atomic_long data;
} mthread_mutex_t;

int mthread_spin_init(mthread_spinlock_t *lock);
int mthread_spin_destroy(mthread_spinlock_t *lock);
int mthread_spin_trylock(mthread_spinlock_t *lock);
int mthread_spin_lock(mthread_spinlock_t *lock);
int mthread_spin_unlock(mthread_spinlock_t *lock);

int mthread_mutex_init(mthread_mutex_t *lock);
int mthread_mutex_destroy(mthread_mutex_t *lock);
int mthread_mutex_trylock(mthread_mutex_t *lock);
int mthread_mutex_lock(mthread_mutex_t *lock);
int mthread_mutex_unlock(mthread_mutex_t *lock);

#endif
