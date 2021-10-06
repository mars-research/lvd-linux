#ifndef GLUE_USER_H
#define GLUE_USER_H

#include "nvme_lcd/nvme.h"
#include <linux/aer.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/priv_mempool.h>
#include "../../block/blk-mq-tag.h"

extern void *bdata_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

struct workqueue_struct;

int __module_lcd_init(void);
void __module_lcd_exit(void);

#ifndef LCD_ISOLATE
#define get_jiffies()		({ jiffies; })
#define get_loops_per_jiffy()		({ loops_per_jiffy; })
#define lvd_alloc_workqueue		alloc_workqueue
#define lvd_setup_timer		setup_timer
#define lvd_init_work		INIT_WORK
#define lvd_queue_work		queue_work
#endif

struct atomic64_t {
	long counter;
};

#endif
