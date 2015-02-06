#include <linux/slab.h>
#include "internal.h"

int lcd_init_list_cache(struct cap_cache *list_cache, uint64_t size) {

	uint32_t i; 

	list_cache->cap = kmalloc(sizeof(*list_cache->cap)*size, GFP_KERNEL); 
	if(!list_cache->cap)
	{
		printk(KERN_ERR "lcd_cap Failed to allocate memory for capabilities");
		return -ENOMEM;
	}

	list_cache->head = 0; 
	for(i = 0; i < size; i++) {
		list_cache->cap[i] = i + 1; 
	}

	list_cache->cap[size - 2] = size - 2;
	mutex_init(&list_cache->lock);
	return 0; 
}

void lcd_free_list_cache(struct cap_cache *list_cache) {
	kfree(list_cache->cap);
	return; 
};
 
int lcd_alloc_cap(struct cap_cache *cap_cache, cptr_t *free_cap) {
	
	uint32_t head;
	int ret;

	ret = mutex_lock_interruptible(&cap_cache->lock); 
	if (ret) {
		LCD_ERR("interrupted");
		goto fail1;
	}
	if(cap_cache->head == cap_cache->cap[cap_cache->head]) {
		LCD_ERR("out of caps");
		ret = -ENOMEM;
		goto fail2;
	};
	
	head = cap_cache->head; 

	*free_cap = (cptr_t)head; 
	cap_cache->head = cap_cache->cap[head];
	
	mutex_unlock(&cap_cache->lock);
	return 0; 

fail2:
	mutex_unlock(&cap_cache->lock);
fail1:
	return ret;
}

void lcd_free_cap(struct cap_cache *cap_cache, cptr_t free_cap)
{
	int ret;
	ret = mutex_lock_interruptible(&cap_cache->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail1;
	}
	
	cap_cache->cap[(uint32_t)free_cap] = cap_cache->head;
	cap_cache->head = (uint32_t)free_cap; 

	mutex_unlock(&cap_cache->lock);
	return; 

fail1:
	return;
}
