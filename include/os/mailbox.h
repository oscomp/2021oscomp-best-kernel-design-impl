
#include <os/list.h>
#include <os/lock.h>
#define NUM_MAX_MB 10

typedef struct mb
{
	char name[32];
	char msg[64];
	cond_t  cond;
	int full;
	int valid;
	mutex_lock_t lock;
}mailbox_node_t;

extern mailbox_node_t mailbox[NUM_MAX_MB];
extern mutex_lock_t mb_lock;

int do_mailbox_open(char *name);
void do_mailbox_close(int mb_id);
void do_mailbox_send(int mb_id, void *msg, int msg_length);
void do_mailbox_recv(int mb_id, void *msg, int msg_length);