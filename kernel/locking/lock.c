#include <os/lock.h>
#include <os/sched.h>
#include <atomic.h>


/* all kinds of locks */
mutex_lock_t mutex_lock;

void spin_lock_init(spin_lock_t *lock)
{
    /* TODO */
}

int spin_lock_try_acquire(spin_lock_t *lock)
{
    /* TODO */
}

void spin_lock_acquire(spin_lock_t *lock)
{
    /* TODO */
}

void spin_lock_release(spin_lock_t *lock)
{
    /* TODO */
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
    lock->lock.status = UNLOCKED;
    lock->block_queue.next = &lock->block_queue; lock->block_queue.prev = &lock->block_queue;
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
    if (LOCKED == atomic_cmpxchg(UNLOCKED, LOCKED, &lock->lock.status) )
    {
    	// printk("id [%d] will be blocked.\n\r",current_running->pid);
    	do_block(&current_running->list, &lock->block_queue);
    }
    else
    	// printk("id [%d] got the lock.\n\r",current_running->pid)
    	;
}

void do_mutex_lock_release(mutex_lock_t *lock)
{
    if (UNLOCKED == atomic_cmpxchg(LOCKED, UNLOCKED, &lock->lock.status))
    {
    	//exit
    	;
    }
    if (lock->block_queue.next != &lock->block_queue)
    {
    	// pcb_t *block_no_1 = lock->block_queue.next->pt_pcb;
    	// printk("id [%d] will be added.\n\r", block_no_1->pid);
    	do_unblock(lock->block_queue.next);
    }
}
