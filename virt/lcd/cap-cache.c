#include <linux/slab.h>
#include <lcd/cap-cache.h>
#include <lcd/cap.h>

int lcd_init_list_cache(struct cap_cache *list_cache, uint64_t size) {

	uint32_t i; 

	list_cache->cap = kmalloc(sizeof(*list_cache->cap)*size, GFP_KERNEL); 
	if(!list_cache->cap)
		return -ENOMEM;

	list_cache->head = 0; 
	for(i = 0; i < size; i++) {
		list_cache->cap[i] = i + 1; 
	}

	list_cache->cap[size - 2] = size - 2;
    spin_lock_init(&list_cache->lock);
	return 0; 
}

void lcd_free_list_cache(struct cap_cache *list_cache) {
	kfree(list_cache->cap);
	return; 
};

int lcd_alloc_cap(struct cap_cache *cap_cache, capability_t *free_cap) {
	
	unsigned long flags;
	uint32_t head;

	spin_lock_irqsave(&cap_cache->lock, flags); 
	if(cap_cache->head == cap_cache->cap[cap_cache->head]) {
		spin_unlock_irqrestore(&cap_cache->lock, flags);
		return -ENOMEM;
	};
	
	head = cap_cache->head; 

	*free_cap = (capability_t)head; 
	cap_cache->head = cap_cache->cap[head];
	
	spin_unlock_irqrestore(&cap_cache->lock, flags);
	return 0; 
};

void lcd_free_cap(struct cap_cache *cap_cache, capability_t free_cap) {
	unsigned long flags;

	spin_lock_irqsave(&cap_cache->lock, flags); 
	
	cap_cache->cap[(uint32_t)free_cap] = cap_cache->head;
	cap_cache->head = (uint32_t)free_cap; 

	spin_unlock_irqrestore(&cap_cache->lock, flags);
	return 0; 
}

