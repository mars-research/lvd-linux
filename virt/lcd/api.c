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


int lcd_api_create_sync_endpoint(struct lcd_api *api, capability_t reply_cap) {
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

	ret = lcd_alloc_cap(&api->cap_cache, &free_cap); 
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

	lcd_free_cap(&api->cap_cache, free_cap); 
	return 0;
err:
	msg->err = ret; 
	ipc_reply(reply_cap, msg);
	return ret;
};

int lcd_api_execution_loop(struct lcd_api *lcd_api) {
	struct message_info *msg = &current->utcb->msg_info; 
	capability_t reply_cap;
	enum lcd_api_calls call; 
	int ret; 

	ret = lcd_alloc_cap(&lcd_api->cap_cache, &reply_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	while (!lcd_api->api_thread_exit) 
	{
		msg->valid_regs = sizeof(msg->regs)/sizeof(msg->regs[0]); 
		ret = ipc_recv(lcd_api->rvp, msg);
		if (ret) {
			printk(KERN_ERR "lcd_api: recv failed:%d\n", ret);
			continue; 
		}

		// We know we're serving a call invocation, reply cap must 
		// be there

		if(msg->valid_regs == 0) {
			printk(KERN_ERR "lcd_api: invalid invocation\n");
			msg->valid_regs = 0; 
			msg->valid_cap_regs = 0; 
			ipc_reply(reply_cap, msg); 
			lcd_cap_drop(&current->cspace, reply_cap);
			continue; 
		};

		call = msg->regs[0];

		switch (call) {
		case LCD_CREATE_SYNC_ENDPOINT: 
			ret = lcd_api_create_sync_endpoint(lcd_api, reply_cap); 
			break; 
		default: 
			printk(KERN_ERR "lcd_api: invalid call number:%d\n", call);
			msg->valid_regs = 0; 
			msg->valid_cap_regs = 0; 
			ipc_reply(reply_cap, msg); 
			lcd_cap_drop(&current->cspace, reply_cap);
		};

	};

	lcd_free_cap(&lcd_api->cap_cache, reply_cap); 

	return 0;
};

int lcd_api_thread(void *p) {

	return lcd_api_execution_loop((struct lcd_api*) p);
};

int lcd_api_init(struct lcd_api *api) {
	int ret;
	struct task_struct *t;
	struct sync_ipc *rvp; 
	struct cnode *cnode;

	ret = lcd_init_list_cache(&api->cap_cache, LCD_MAX_CAPS);
	if (ret) {
		printk(KERN_ERR "Failed to initialize cap cache:%d\n", ret);
		return ret;

	}

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		return -ENOMEM;
	};

	ret = lcd_alloc_cap(&api->cap_cache, &api->rvp); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		kfree(rvp);
		return -ENOSPC;
	};

        cnode = lcd_cnode_lookup(&current->cspace, api->rvp);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		return -ENOMEM;
	};

	cnode->type = LCD_TYPE_SYNC_EP;
	cnode->object = rvp; 

	lcd_cnode_release(cnode);

	t = kthread_create(lcd_api_thread, api, "lcd-api");
	if (!t) {
		printk(KERN_ERR "Failed to create LCD API thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

int lcd_api_exit(struct lcd_api *api) {
	api->api_thread_exit = 1; 
};

