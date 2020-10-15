/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2019 Institute of Computing Technology, CAS
 *               Author : Wang Luming (email : wangluming@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                          simplified version of Linux's futex
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
