#ifndef HOST_IPC_H
#define HOST_IPC_H

#include <uapi/linux/lcd-ipc.h>
#include <lcd/cap.h>

struct sync_ipc {
	struct list_head senders;
        struct list_head receivers;
        spinlock_t lock;

};

/* Per thread kernel stack unified on a single page. */
union utcb_union {
	struct utcb utcb;
	char kstack[PAGE_SIZE];
};


struct sync_ipc * alloc_sync_ipc(void);

int lcd_ipc_init(void);
int lcd_ipc_exit(void);

int ipc_send(capability_t cap, struct message_info *msg);
int ipc_recv(capability_t rvp_cap, struct message_info *msg);

#endif
