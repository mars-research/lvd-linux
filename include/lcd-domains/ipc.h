
/* NOT CURRENTLY USED */

#ifndef LCD_DOMAINS_IPC_H
#define LCD_DOMAINS_IPC_H

#include <linux/list.h>
#include <linux/mutex.h>

struct sync_endpoint {
	struct list_head senders;
        struct list_head receivers;
        struct mutex lock;
};

#endif /* LCD_DOMAINS_IPC_H */
