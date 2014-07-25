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

int lcd_api_create_sync_endpoint(struct message_info *msg, struct lcd_api *api, capability_t reply_cap) {
	int ret; 
	struct sync_ipc *rvp; 
	capability_t rvp_cap; 
	struct cnode *cnode;
	struct message_info *msg = &current->utcb->msg_info; 

	rvp = lcd_create_sync_ep();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		ret = -ENOMEM;
		goto err;
	};

	ret = lcd_alloc_cap(&current->cap_cache, &rvp_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		ret = -ENOSPC;
		kfree(rvp);
		goto err;
	};

        ret = lcd_cap_insert_object(&current->cspace, rvp_cap, rvp, LCD_TYPE_SYNC_EP);
	if(cnode == 0) {
		lcd_destroy_rvp(rvp);
		goto err;
	};

	msg->cap_regs[0] = rvp_cap;
	msg->valid_cap_regs = 1;
	ipc_reply(reply_cap, msg);

	lcd_cap_drop(&current->cspace, rvp_cap);

	lcd_free_cap(&current->cap_cache, rvp_cap); 
	return 0;
err:
	msg->err = ret; 
	ipc_reply(reply_cap, msg);
	return ret;
};

int lcd_api_create_lcd(struct message_info *msg, struct lcd_api *api, capability_t reply_cap) {
	int ret;
	int i; 
	capability_t lcd_cap; 
	struct cnode *cnode;
	char *module_name; 

	module_name = lcd_cap_marshal_string(&msg->regs[1], LCD_MAX_REGS - 1);
	if (!module_name) {
		printk(KERN_ERR "Failed to unmarshal LCD name\n");
		ret = -EINVAL;
		goto err;
	};

	lcd_task = lcd_create_as_module(module_name);
	if(!lcd_task) {
		printk(KERN_ERR "Failed to create LCD from a module:%s\n", module_name);
		ret = -ENOMEM;
		goto err;
	};

	ret = lcd_alloc_cap(&current->cap_cache, &lcd_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		ret = -ENOSPC;
		lcd_destroy(lcd_task);
		goto err;
	};

        ret = lcd_cap_insert_object(&current->cspace, lcd_cap, lcd_task, LCD_TYPE_LCD);
	if(cnode == 0) {
		lcd_destroy(lcd_task); 
		goto err;
	};

	for (int i = 0; i < cap_regs - 2; i++) {
		
		capability_t boot_cap;

		ret = lcd_alloc_cap(&lcd_task->cap_cache, &boot_cap); 
		if(ret) {
			printk(KERN_ERR "Failed to allocate free capability\n");
			ret = -ENOSPC;
			lcd_destroy(lcd_task);
			goto err;
		};

        	ret = lcd_cap_move_object(&lcd_task->cspace, boot_cap, &current->cspace, msg->cap_regs[i]);
		if(ret) {
			printk(KERN_ERR "Failed to move boot capability number:d\n", i);
			ret = -ENOSPC;
			lcd_destroy(lcd_task); 
			goto err;
		};

		lcd_task->utcb->boot_info.boot_caps[i] = boot_cap;
		lcd_drop_cap(&current->cspace, msg->cap_regs[i]);
	
	};

	msg->cap_regs[0] = lcd_cap;
	msg->valid_cap_regs = 1;
	ipc_reply(reply_cap, msg);

	lcd_cap_drop(&current->cspace, lcd_cap);

	lcd_free_cap(&current->cap_cache, lcd_cap); 
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
	int i; 

	for (i = 0; i < LCD_MAX_CAP_REGS; i++) {

		ret = lcd_alloc_cap(&current->cap_cache, 
				&msg->cap_regs[i]); 
		if(ret) {
			printk(KERN_ERR "Failed to allocate free capability\n");
			return -ENOSPC;
		};
	};

	while (!lcd_api->exit_flag) 
	{
		msg->valid_regs = sizeof(msg->regs)/sizeof(msg->regs[0]); 
		ret = ipc_recv(lcd_api->rvp, msg);
		if (ret) {
			printk(KERN_ERR "lcd_api: recv failed:%d\n", ret);
			continue; 
		}

		reply_cap = msg->cap_regs[msg->valid_cap_regs];

		// We know we're serving a call invocation, reply cap is 
		// verified by the ipc_call()

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
			ret = lcd_api_create_sync_endpoint(msg, lcd_api, reply_cap); 
			break; 
		case LCD_CREATE_LCD: 
			ret = lcd_api_create_lcd(msg, lcd_api, reply_cap); 
			break; 

		default: 
			printk(KERN_ERR "lcd_api: invalid call number:%d\n", call);
			msg->valid_regs = 0; 
			msg->valid_cap_regs = 0; 
			ipc_reply(reply_cap, msg); 
			lcd_cap_drop(&current->cspace, reply_cap);
		};

	};

	for (i = 0; i < LCD_MAX_CAP_REGS; i++) {

		ret = lcd_free_cap(&current->cap_cache, msg->cap_regs[i]); 
	};

	return 0;
};

int lcd_api_thread(void *p) {
	int ret; 
	struct lcd_api *api = (struct lcd_api*) p;


	ret = lcd_enter();
	if(ret) {
		printk(KERN_ERR "Failed to enter LCD:%d\n", ret);
		return ret;
	};

	api->ep = lcd_make_sync_endpoint(&current->cspace, &current->cap_cache, &api->ep_cap);
	if(api->ep) {
		printk(KERN_ERR "Failed to create API endpoint:%d\n", ret);
		return -EINVAL;
	};

	
	ret = lcd_api_execution_loop(api);

	return ret; 
};

int lcd_api_init(struct lcd_api *api) {
	int ret;
	struct task_struct *t;
	
	t = kthread_create(lcd_api_thread, api, "lcd-api");
	if (!t) {
		printk(KERN_ERR "Failed to create LCD API thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

int lcd_api_exit(struct lcd_api *api) {
	api->exit_flag = 1; 
};

