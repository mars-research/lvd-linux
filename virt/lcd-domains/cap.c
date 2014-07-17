/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include <lcd/cap.h>


struct kmem_cache *cnode_cache;
struct kmem_cache *cdtnode_cache;

int lcd_cap_init(void){
	cnode_cache = KMEM_CACHE(cnode, 0);
	if(!cnode_cache){
		printk(KERN_ERR "Failed to allocate cnode slab\n");
		return -ENOMEM;
	};

	//cdtnode_cache = KMEM_CACHE(cap_derivation_tree, 0);
	//if(!cdt_cache){
	//	printk(KERN_ERR "Failed to allocate cte slab\n");
	//	return -ENOMEM;
	//};

	return 0;
};

int lcd_cap_exit(void) {
	if (cnode_cache)
		kmem_cache_destroy(cnode_cache);
	//if (cdt_cache)
	//	kmem_cache_destroy(cdt_cache);
	return 0;
}

int lcd_cap_insert_object(struct cspace *cspace, capability_t cap, 
		void *object, enum lcd_cap_type type) 
{
	struct cnode *cnode;

	cnode = lcd_cnode_lookup(cspace, cap);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		return -ENOMEM;
	};

	cnode->type = type;
	cnode->object = object; 

	lcd_cnode_release(cnode);
	return 0; 
}

struct sync_ipc * lcd_cap_make_sync_endpoint(struct cspace *cspace, struct cap_cache *cache, capability_t *cap) {
	struct sync_ipc *rvp;

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		return NULL;
	};

	ret = lcd_alloc_cap(cache, cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		kfree(rvp);
		return NULL;
	};

        ret = lcd_cap_insert_object(cspace, *cap, rvp, LCD_TYPE_SYNC_EP); 
	if(ret) {
		printk(KERN_ERR "Failed to insert endpoint into the cspace:%d\n", ret);
		kfree(rvp);
		return NULL;
	};

	return rvp;
};

int lcd_init_cspace(struct cspace *cspace) {

	cspace->cnode = kmalloc(sizeof(struct cnode) * LCD_MAX_CAPS, GFP_KERNEL);
	if(!cspace->cnode){
		printk(KERN_ERR "Failed to allocate memory for cnodes\n");
		return -ENOMEM;
	};

	spin_lock_init(&cspace->lock);
	return 0;
};
EXPORT_SYMBOL(lcd_init_cspace);

int lcd_cnode_insert(struct cspace *cspace, capability_t cap, struct cnode *cnode) {
	/* XXX: not clear */
	return 0;
};

static inline void lcd_cnode_lock(struct cnode *cnode) {
	/* XXX: not clear what to do yet */
	return;	
};

void lcd_cnode_release(struct cnode *cnode) {
	/* XXX: not clear what to do yet */
	return;	
};
EXPORT_SYMBOL(lcd_cnode_release);

struct cnode *lcd_cnode_lookup(struct cspace *cspace, capability_t cap) {

	unsigned long flags;
	
	if(cap >= LCD_MAX_CAPS) 
		return NULL;

	spin_lock_irqsave(&cspace->lock, flags);
	lcd_cnode_lock(&cspace->cnode[cap]);
	spin_unlock_irqrestore(&cspace->lock, flags);		

	return &cspace->cnode[cap];
};
EXPORT_SYMBOL(lcd_cnode_lookup);

void lcd_cap_drop(struct cspace *cspace, capability_t cap) {
	struct cnode *cnode; 

	cnode = lcd_cnode_lookup(cspace, cap); 
	if(!cnode)
		return; 
	cnode->type = LCD_TYPE_FREE; 
	cnode->object = NULL; 
	lcd_cnode_release(cnode); 
	return; 
};

