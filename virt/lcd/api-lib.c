
struct cap_cache *cap_cache() {
	return &current->cap_cache;
};

int lcd_api_alloc_reply_cap(capability_t api, capability_t api_reply, capability_t *reply) {
	
        ret = lcd_alloc_cap(cap_cache(), reply); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	ret = lcd_api_create_sync_endpoint(api_cap, reply_cap, reply); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	return 0; 
}
