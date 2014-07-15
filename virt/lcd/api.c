/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */

static inline int lcd_create_sync_endpoint(capability_t cap, capability_t rvp) {

	struct message_info *msg = &current->utcb.message_info;

	msg->regs[0] = LCD_CREATE_SYNC_ENDPOINT; 
	msg->valid_regs = 1; 

	msg->cap_regs[0] = rvp; 
	msg->valid_cap_regs = 1; 

	return ipc_send_and_receive(cap, msg); 

};



int lcd_create_sync_endpoint(capability_t otherend) {
	int ret; 
	struct sync_ipc *rvp; 
	capability_t free_cap; 

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		ret = -ENOMEM;
		goto err;
	};

	ret = lcd_alloc_free_cap(cap_cache, &free_cap); 
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
	ipc_reply(otherend, msg);

	lcd_cap_drop(free_cap);

	lcd_release_free_cap(cap_cache); 
	return 0;
err:
	msg->err = ret; 
	ipc_reply(otherend, msg);
	return ret;
};



