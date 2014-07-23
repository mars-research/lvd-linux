/*
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 *
 * Factories provide a safe way to create LCDs with a controlled set of 
 * capabilities. Right now I don't know how the factory code will look. For 
 * now it's just a couple of dumb introduction functions. 
 */

int lcd_prepare_introduction(capability_t api_cap, capabiity_t reply_cap, sync_ipc *rvp, capability_t *rvp_cap) {
	
        ret = lcd_alloc_cap(&current->cap_cache, rvp_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	ret = lcd_api_create_sync_endpoint(api_cap, reply_cap, *rvp_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};


	cnode = lcd_cnode_lookup(&current->cspace, *rvp_cap);
	if(cnode == NULL || cnode->type != LCD_TYPE_SYNC_EP) {
		printk(KERN_ERR "Failed to create capability\n");
		return -ENOMEM;
	};

	rvp = cnode->object; 
	lcd_cnode_release(cnode);
	return 0;
};

int lcd_accept_introduction(struct sync_ipc *rvp, capability_t *rvp_cap) {
	int ret; 

	ret = lcd_alloc_cap(&current->cap_cache, rvp_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	return lcd_cap_insert_object(&current->cspace, *rvp_cap, rvp, LCD_TYPE_SYNC_EP);

};


