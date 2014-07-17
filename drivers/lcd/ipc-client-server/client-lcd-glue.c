struct sync_ipc *rvp; 

int lcd_client_server_init_client(void) {
	int ret; 
	capability_t server;
	struct cnode *cnode;
	
	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	ret = lcd_api_connect(lcd_get_api());
	if (ret) {
		printk(KERN_ERR "Failed to connect to LCD API\n");
		return ret;
	};

	api_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_API_CAP];
	reply_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_REPLY_CAP];


        ret = lcd_alloc_cap(&current->cap_cache, &server); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	ret = lcd_api_create_sync_endpoint(api_cap, reply_cap, server); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0] = server;

	cnode = lcd_cnode_lookup(&current->cspace, server);
	if(cnode == NULL || cnode->type != LCD_TYPE_SYNC_EP) {
		printk(KERN_ERR "Failed to create capability\n");
		return -ENOMEM;
	};

	rvp = cnode->object; 
	lcd_cnode_release(cnode);

	return 0;
};

int lcd_client_server_init_server(void) 
{
	int ret; 
	capability_t client;

	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	ret = lcd_api_connect(lcd_get_api());
	if (ret) {
		printk(KERN_ERR "Failed to connect to LCD API\n");
		return ret;
	};

        ret = lcd_alloc_cap(&current->cap_cache, &client); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	lcd_cap_insert_object(&current->cspace, client, rvp, LCD_TYPE_SYNC_EP);

	current->utcb->boot_caps[LCD_BOOT_FREE_CAP0] = client; 
	return 0;
};
EXPORT_SYMBOL(lcd_client_server_init_server); 

int module_execution_loop(void) {

	int ret; 
	capability_t api_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_API_CAP];
	capability_t reply_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_REPLY_CAP];
	capability_t server = current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0];

	capability_t server_interface;
	capability_t server_reply; 
	
	struct message_info *msg = &current->utcb->msg_info;


        ret = lcd_alloc_cap(&current->cap_cache, &server_interface); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	msg->cap_regs[0] = server_interface;
	msg->valid_cap_regs = 1;

	ret = ipc_recv(server, msg);
	if (ret) {
		printk(KERN_ERR "client failed to receive from the server:%d\n", ret);
		return ret;
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


	return 0;
};

int lcd_thread(void *p) {
	int ret;

	ret = lcd_client_server_init_client();
	if (ret) {
		printk(KERN_ERR "client failed to initialize client-server environtment\n");
		return -EINVAL;
	}

	return lcd_execution_loop();
};

int lcd_thread_init(void) {

	struct task_struct *t;

	t = kthread_create(lcd_thread, NULL, "cs-client");
	if (!t) {
		printk(KERN_ERR "Failed to create module thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

