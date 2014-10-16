/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include <linux/delay.h>
#include <lcd/cap.h>

int lcd_init_cspace(struct cspace *cspace) {

	int i = 0;
	spin_lock_init(&cspace->lock);

	lcd_init_list_cache(&(cspace->cap_cache), LCD_MAX_CAPS);
	if (!cspace->cap_cache.cap) {
		printk(KERN_ERR "lcd_cap Failed to allocate cap_cache\n");
		return -ENOMEM;
	}

	cspace->cnode_table_cache = KMEM_CACHE(cnode_table, 0);
	if(!cspace->cnode_table_cache){
		printk(KERN_ERR "lcd_cap Failed to allocate cnode_table slab\n");
		return -ENOMEM;
	};

	cspace->cdt_root_cache = KMEM_CACHE(cdt_root_node, 0);
	if(!cspace->cdt_root_cache){
		printk(KERN_ERR "lcd_cap Failed to allocate cdt_root slab\n");
		return -ENOMEM;
	};

	cspace->cnode_table = kmem_cache_alloc(cspace->cnode_table_cache, 0);
	for (i = 0; i < MAX_SLOTS_PER_TABLE; i++)
	{
		cspace->cnode_table->cnode[i].type = LCD_CAP_TYPE_FREE;
		cspace->cnode_table->table_level = 0;
	}
	cspace->state = ALLOCATION_VALID;
	
	return 0;
};
EXPORT_SYMBOL(lcd_init_cspace);


// expects the cspace to be locked by the caller.
struct cnode *lcd_cnode_lookup(struct cspace *cspace, capability_t cap, bool alloc) {

	bool more_levels;
	capability_t levelId;
	struct cnode *cnode;
	struct cnode_table *table, *newtable;
	int i = 0;
	if(cap >= LCD_MAX_CAPS) 
		return NULL;

	// spin_lock_irqsave(&cspace->lock, flags);
	table = cspace->cnode_table;
	do {
		more_levels = _get_level_bits(table->table_level, cap, &levelId);
		if (more_levels) {
			capability_t index = levelId + MAX_CAP_SLOTS;
			if (table->cnode[index].type != LCD_CAP_TYPE_CNODE) {
				if (alloc) {
					table->cnode[index].type   = LCD_CAP_TYPE_CNODE;
					newtable = kmem_cache_alloc(cspace->cnode_table_cache, 0);
					table->cnode[index].object = (void*)newtable;
					newtable->table_level = table->table_level + 1;
					printk(KERN_ERR "lcd_cap Created a new table with level: %u", newtable->table_level);
					for (i = 0; i < MAX_SLOTS_PER_TABLE; i++)
					{
						newtable->cnode[i].type = LCD_CAP_TYPE_FREE;
					}
					table->cnode[index].cspace = cspace;
					table = newtable;
				} else {
					cnode = NULL;
					break;
				}
			} else {
				table = (struct cnode_table *)table->cnode[index].object;
			}
		} else {
			if ((alloc && table->cnode[levelId].type == LCD_CAP_TYPE_FREE) || 
				 (!alloc && table->cnode[levelId].type != LCD_CAP_TYPE_FREE)) {
				printk(KERN_ERR "lcd_cap Lookup got a cnode");
				cnode = &(table->cnode[levelId]);
			} else {
				printk(KERN_ERR "lcd_cap Lookup failed to get a cnode");
				cnode = NULL;
				break;
			}
		}
	} while (more_levels == true); 

	// lcd_cnode_lock(&cspace->cnode[cap]);
	
	//spin_unlock_irqrestore(&cspace->lock, flags);		

	return cnode;
};
EXPORT_SYMBOL(lcd_cnode_lookup);

int lcd_cap_insert(struct cspace *cspace, capability_t cap, 
		void *object, enum lcd_cap_type type) 
{
	unsigned long flags, flags_cnode_lock;
	struct cnode *cnode;

	spin_lock_irqsave(&cspace->lock, flags);
	if (cspace->state != ALLOCATION_VALID)
	{
		spin_unlock_irqrestore(&cspace->lock, flags);		
		return -EIDRM;
	}
	cnode = lcd_cnode_lookup(cspace, cap, true);
	if(cnode == NULL) {
		spin_unlock_irqrestore(&cspace->lock, flags);		
		printk(KERN_ERR "Failed to create capability\n");
		return -ENOMEM;
	};

	cnode->object = object; 
	spin_lock_init(&cnode->lock);
	spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
	spin_unlock_irqrestore(&cspace->lock, flags);		
	
	cnode->cspace = cspace;
	cnode->parent = NULL;
	cnode->next   = NULL;
	cnode->prev   = NULL;
	cnode->child  = NULL;
	cnode->type   = type;
	cnode->cdt_root = kmem_cache_alloc(cspace->cdt_root_cache, 0);
	spin_lock_init(&(cnode->cdt_root->lock));
	cnode->cdt_root->cnode      = cnode;
	cnode->cdt_root->node_count = 1;
	cnode->cdt_root->state      = ALLOCATION_VALID;

	//lcd_cnode_release(cnode);
	spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);	
	return 0; 
}
EXPORT_SYMBOL(lcd_cap_insert);

void lcd_cap_delete(struct cspace *cspace, capability_t cap) {

	struct cnode *cnode; 
	unsigned long flags, flags_cnode_lock;
	bool done = false;

	do {
		spin_lock_irqsave(&cspace->lock, flags);
		if (cspace->state != ALLOCATION_VALID) {
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return;
		}

		cnode = lcd_cnode_lookup(cspace, cap, false); 
		if(cnode == NULL)
		{
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return; 
		}

		spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
		spin_unlock_irqrestore(&cspace->lock, flags);
		if (cnode->type == LCD_CAP_TYPE_FREE) {
			spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
			return;
		}
		
		if (spin_trylock(&cnode->cdt_root->lock)) {
			_lcd_delete_node(cspace, cnode);
			if (cnode->cdt_root) {
				spin_unlock(&cnode->cdt_root->lock);
				cnode->cdt_root = NULL;
			}
			
			lcd_free_cap(&(cspace->cap_cache), cap);
			done = true;
		}
		spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
		
		if (!done) {
			// someone is using CDT root
			// wait for him to finish
			msleep(5);
		}
	} while (!done);
};
EXPORT_SYMBOL(lcd_cap_delete);


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




//////////////////////////////////////////////////////////////////
//						Helper functions
//////////////////////////////////////////////////////////////////
bool _get_level_bits(int table_level, capability_t cap, capability_t *levelId)
{
	bool more_levels;
	// Get rid of capability identifer bits
	capability_t c = ((uint64_t)cap >> CAP_IDENTIFIER_BITS) >> (table_level * TABLE_IDENTIFIER_BITS);
	printk(KERN_ERR "lcd_cap cap: %llu", (uint64_t)cap); 
	printk(KERN_ERR "lcd_cap c = : %llu", (uint64_t)c);
	if (c)
	{
		int level_mask = (MAX_TABLE_ID_SLOTS - 1);
		*levelId = c & level_mask;
		more_levels = true;
	}
	else
	{
		capability_t mask = MAX_CAP_SLOTS - 1;
		*levelId = cap & mask;
		more_levels = false;
	}
	return more_levels;
}

// assumes cnode and cdt root is locked (in that order) 
void _lcd_delete_node (struct cspace *cspace, struct cnode *cnode){

	cnode->object = NULL; 
	cnode->type = LCD_CAP_TYPE_FREE;

	cnode->cdt_root->node_count--;
	if (cnode->cdt_root->node_count > 0) {
		// get cnode out of CDT
		if (cnode->child) {
			struct cnode *childnode = cnode->child;
			cnode->child = NULL;
			childnode->parent = cnode->parent;
			if (cnode->parent) {
				if (cnode->parent->child == cnode) {
					cnode->parent->child = childnode;
				}
			}
			childnode->prev = cnode->prev;
			if (cnode->prev) {
				cnode->prev->next = childnode;
			}

			while (childnode->next) {
				childnode = childnode->next;
				childnode->parent = cnode->parent;
			}
			cnode->parent = NULL;
			
			childnode->next = cnode->next;
			if (cnode->next) {
				cnode->next->prev = childnode;
			}
			cnode->next = NULL;
			cnode->prev = NULL;
		} else {
			if (cnode->parent) {
				if (cnode->parent->child == cnode) {
					cnode->parent->child = cnode->next;
				}
			}
			if (cnode->prev) {
				cnode->prev->next = cnode->next;
			}
			if (cnode->next) {
				cnode->next->prev = cnode->prev;
			}
			cnode->parent = NULL;
			cnode->prev = NULL;
			cnode->next = NULL;
		}
	} else {
		// this was the last node
		spin_unlock(&cnode->cdt_root->lock);
		kmem_cache_free(cspace->cdt_root_cache, cnode->cdt_root);
		cnode->cdt_root = NULL;
	} 	
}