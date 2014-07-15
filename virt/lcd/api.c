/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/slab.h>
#include <lcd/api.h>
#include <lcd/cap-cache.h>
#include <lcd/cap.h>
#include <linux/sched.h>
#include <linux/kthread.h>

struct cap_cache cap_cache;  

int lcd_api_create_sync_endpoint(capability_t reply_cap) {
	int ret; 
	struct sync_ipc *rvp; 
	capability_t free_cap; 
	struct cnode *cnode;
	struct message_info *msg = &current->utcb->msg_info; 

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		ret = -ENOMEM;
		goto err;
	};

	ret = lcd_alloc_cap(&cap_cache, &free_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		ret = -ENOSPC;
		kfree(rvp);
		goto err;
	};

        cnode = lcd_cnode_lookup(&current->cspace, free_cap);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		ret = -ENOMEM;
		goto err;
	};

	cnode->type = LCD_TYPE_SYNC_EP;
	cnode->object = rvp; 

	lcd_cnode_release(cnode);

	msg->cap_regs[0] = free_cap;
	msg->valid_cap_regs = 1;
	ipc_reply(reply_cap, msg);

	lcd_cap_drop(&current->cspace, free_cap);

	lcd_free_cap(&cap_cache, free_cap); 
	return 0;
err:
	msg->err = ret; 
	ipc_reply(reply_cap, msg);
	return ret;
};

int lcd_api_execution_loop(void) {


	return 0;
};

int lcd_api_thread(void *p) {
	int ret;

	return lcd_api_execution_loop();
};

int lcd_api_init(void) {
	int ret;
	struct task_struct *t;

	ret = lcd_init_list_cache(&cap_cache, LCD_MAX_CAPS);
	if (ret) {
		printk(KERN_ERR "Failed to initialize cap cache:%d\n", ret);
		return ret;

	}

	t = kthread_create(lcd_api_thread, NULL, "lcd-api");
	if (!t) {
		printk(KERN_ERR "Failed to create LCD API thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}



