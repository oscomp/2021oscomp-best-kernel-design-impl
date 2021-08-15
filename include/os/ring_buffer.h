#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <assert.h>
#include <os/mm.h>
#include <os/lock.h>
#include <type.h>

#define RING_BUFFER_SIZE NORMAL_PAGE_SIZE
#pragma pack(8)
struct ring_buffer {
	mutex_lock_t lock; /* FOR NOW no use */
	size_t size;		// for future use
	int32_t head;		// read from head
	int32_t tail;		// write from tail
	char buf[RING_BUFFER_SIZE + 1]; // left 1 byte
};
#pragma pack()

int wait_ring_buffer_read(struct ring_buffer *rbuf, time_t final_ticks);
int wait_ring_buffer_write(struct ring_buffer *rbuf, time_t final_ticks);

static inline void init_ring_buffer(struct ring_buffer *rbuf)
{
	// there is always one byte which should not be read or written
	memset(rbuf, 0, sizeof(struct ring_buffer)); /* head = tail = 0 */
	rbuf->size = RING_BUFFER_SIZE;
	do_mutex_lock_init(&rbuf->lock);
	return ;
}

static inline int ring_buffer_used(struct ring_buffer *rbuf)
{
	return (rbuf->tail - rbuf->head + rbuf->size) % (rbuf->size);
}

static inline int ring_buffer_free(struct ring_buffer *rbuf)
{
	// let 1 byte to distinguish empty buffer and full buffer
	return rbuf->size - ring_buffer_used(rbuf) - 1;
}

static inline int ring_buffer_empty(struct ring_buffer *rbuf)
{
	return ring_buffer_used(rbuf) == 0;
}

static inline int ring_buffer_full(struct ring_buffer *rbuf)
{
	return ring_buffer_free(rbuf) == 0;
}

#ifndef min
#define min(x,y) ((x)<(y) ? (x) : (y))
#endif

static inline size_t read_ring_buffer(struct ring_buffer *rbuf, char *buf, size_t size)
{
	int32_t len = min(ring_buffer_used(rbuf), size);
	if (len > 0) {
		if (rbuf->head + len > rbuf->size) {
			int32_t right = rbuf->size - rbuf->head,
				left = len - right;
			memcpy(buf, rbuf->buf + rbuf->head, right);
			memcpy(buf + right, rbuf->buf, left);
		}
		else {
			memcpy(buf, rbuf->buf + rbuf->head, len);
		}

		rbuf->head = (rbuf->head + len) % (rbuf->size);
	}
	else if (len < 0)
		assert(0);

	return len;
}

// rbuf should have enough space for buf
static inline size_t write_ring_buffer(struct ring_buffer *rbuf, char *buf, size_t size)
{
	int32_t len = min(ring_buffer_free(rbuf), size);
	if (len > 0){
		if (rbuf->tail + len > rbuf->size) {
			int32_t right = rbuf->size - rbuf->tail,
				left = len - right;
			memcpy(rbuf->buf + rbuf->tail, buf, right);
			if (left > 0)
				memcpy(rbuf->buf, buf + right, left);
		}
		else {
			memcpy(rbuf->buf + rbuf->tail, buf, len);
		}

		rbuf->tail = (rbuf->tail + len) % (rbuf->size);
	}
	else if (len < 0)
		assert(0);

	return len;
}

#endif
