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

	cspace->cnode_table = kmem_cache_alloc(cspace->cnode_table_cache, 0);
	for (i = 0; i < MAX_SLOTS_PER_TABLE; i++)
	{
		spin_lock_init(&(cspace->cnode_table->cnode[i].lock));
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
				cnode = &(table->cnode[levelId]);
				if (alloc) {
					spin_lock_init(&cnode->lock);
					cnode->cap = cap;
					cnode->cspace = cspace;
				}
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
	spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
	spin_unlock_irqrestore(&cspace->lock, flags);		
	
	cnode->cap    = cap;
	cnode->cspace = cspace;
	cnode->parent = NULL;
	cnode->next   = NULL;
	cnode->prev   = NULL;
	cnode->child  = NULL;
	cnode->type   = type;
	cnode->cdt_root = get_cdt_root();
	cnode->cdt_root->cnode      = cnode;
	cnode->cdt_root->node_count = 1;

	//lcd_cnode_release(cnode);
	spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);	
	return 0; 
}
EXPORT_SYMBOL(lcd_cap_insert);

void lcd_cap_delete(struct cspace *cspace, capability_t cap) {

	struct cnode *cnode; 
	unsigned long flags, flags_cnode_lock;
	bool done = false, last_node;
	struct cdt_root_node *cdt_node = NULL;

	do {
		spin_lock_irqsave(&cspace->lock, flags);
		if (cspace->state != ALLOCATION_VALID) {
			printk(KERN_INFO "lcd_cap_delete cspace allocation is not valid\n");
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return;
		}

		cnode = lcd_cnode_lookup(cspace, cap, false); 
		if(cnode == NULL)
		{
			printk(KERN_INFO "lcd_cap_delete cnode not found\n");
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return; 
		}

		spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
		spin_unlock_irqrestore(&cspace->lock, flags);
		if (cnode->type == LCD_CAP_TYPE_FREE) {
			printk(KERN_INFO "lcd_cap_delete cnode is already free\n");
			spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
			return;
		}
		
		if (spin_trylock(&cnode->cdt_root->lock)) {
			cdt_node = cnode->cdt_root;
			last_node = _lcd_delete_node(cnode);
			if (last_node) {	
				cdt_node->state = ALLOCATION_MARKED_FOR_DELETE;
			}
			cnode->cdt_root = NULL;
			spin_unlock(&(cdt_node->lock));
			done = true;
		}
		spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
		
		if (done) {
			if (last_node) {	
				free_cdt_root(cdt_node);	
			}
			lcd_free_cap(&(cspace->cap_cache), cap);
		} else {
			// someone is using CDT root
			// wait for him to finish
			msleep(1);
		}
	} while (!done);
};
EXPORT_SYMBOL(lcd_cap_delete);

int lcd_cap_grant(struct cspace *cspacesrc, capability_t capsrc, struct cspace *cspacedst, capability_t capdst)
{
	struct cnode *cnodesrc, *cnodedst; 
	unsigned long flagssrc, flagsdst, flags_cnodesrc_lock, flags_cnodedst_lock;
	bool done = false;
	struct cdt_root_node *cdt_node;

	do {
		// src cspace and cnode
		spin_lock_irqsave(&cspacesrc->lock, flagssrc);
		if (cspacesrc->state != ALLOCATION_VALID) {
			spin_unlock_irqrestore(&cspacesrc->lock, flagssrc);
			printk(KERN_ERR "lcd_cap_grant source cspace allocation is invalid");
			return -EIDRM;
		}

		cnodesrc = lcd_cnode_lookup(cspacesrc, capsrc, false); 
		if(cnodesrc == NULL)
		{
			spin_unlock_irqrestore(&cspacesrc->lock, flagssrc);		
			printk(KERN_ERR "lcd_cap_grant cnode for source capability: %llu not found", capsrc);
			return -EINVAL; 
		}

		spin_lock_irqsave(&cnodesrc->lock, flags_cnodesrc_lock);
		spin_unlock_irqrestore(&cspacesrc->lock, flagssrc);

		if (cnodesrc->type == LCD_CAP_TYPE_FREE) {  
			spin_unlock_irqrestore(&cnodesrc->lock, flags_cnodesrc_lock);
			printk(KERN_ERR "lcd_cap_grant cnode for source capability: %llu is invalid", capsrc);
			return -EINVAL;
		}

		// dst cspace and cnode
		spin_lock_irqsave(&cspacedst->lock, flagsdst);
		if (cspacedst->state != ALLOCATION_VALID) {
			spin_unlock_irqrestore(&cnodesrc->lock, flags_cnodesrc_lock);
			spin_unlock_irqrestore(&cspacedst->lock, flagsdst);		
			printk(KERN_ERR "lcd_cap_grant destination cspace allocation is invalid");
			return -EIDRM;
		}

		cnodedst = lcd_cnode_lookup(cspacedst, capdst, true); 
		if(cnodedst == NULL)
		{
			spin_unlock_irqrestore(&cnodesrc->lock, flags_cnodesrc_lock);
			spin_unlock_irqrestore(&cspacedst->lock, flagsdst);		
			printk(KERN_ERR "lcd_cap_grant cnode for destination capability: %llu not found", capdst);
			return -EINVAL; 
		}

		spin_lock_irqsave(&cnodedst->lock, flags_cnodedst_lock);
		spin_unlock_irqrestore(&cspacedst->lock, flagsdst);

		if (cnodedst->type != LCD_CAP_TYPE_FREE) {  
			spin_unlock_irqrestore(&cnodesrc->lock, flags_cnodesrc_lock);
			spin_unlock_irqrestore(&cnodedst->lock, flags_cnodedst_lock);
			printk(KERN_ERR "lcd_cap_grant cnode for destination capability: %llu is invalid", capdst);
			return -EINVAL;
		}
		
		if (spin_trylock(&cnodesrc->cdt_root->lock)) {
			cdt_node = cnodesrc->cdt_root;
			cdt_node->node_count++;
			// source and destination cnodes and cdt root is locked.
			cnodedst->parent = cnodesrc;
			cnodedst->type = cnodesrc->type;
			cnodedst->object = cnodesrc->object;
			cnodedst->cap   = capdst;
			cnodedst->cspace = cspacedst;
			cnodedst->cdt_root = cnodesrc->cdt_root;
			if (cnodesrc->child) {
				struct cnode *childnode = cnodesrc->child;
				cnodedst->prev = childnode->prev;
				childnode->prev = cnodedst;
				cnodedst->next = childnode;
				
			} else {
				cnodedst->prev = NULL;
				cnodedst->next = NULL;
				cnodedst->child = NULL;
			}
			cnodesrc->child = cnodedst;
			done = true;
			spin_unlock(&(cdt_node->lock));
		}

		spin_unlock_irqrestore(&cnodesrc->lock, flags_cnodesrc_lock);
		spin_unlock_irqrestore(&cnodedst->lock, flags_cnodedst_lock);

		if (!done) {
			// someone is using CDT root
			// wait for him to finish
			msleep(1);
		}
	} while (!done);
	return 0;
};
EXPORT_SYMBOL(lcd_cap_grant);

int lcd_cap_revoke(struct cspace *cspace, capability_t cap) {
	struct cnode *cnode, *childnode; 
	unsigned long flags, flags_cnode_lock;
	bool done = false, last_node;
	struct cdt_root_node *cdt_node = NULL;

	do {
		spin_lock_irqsave(&cspace->lock, flags);
		if (cspace->state != ALLOCATION_VALID) {
			// TBD: not clear what needs to be done here - return with error or continue revoke operation down the hierarchy?
			// AI: Anton, Muktesh
			// current state: simply return if the root cspace is not valid.
			printk(KERN_INFO "lcd_cap_revoke cspace allocation is not valid\n");
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return -EINVAL;
		}

		cnode = lcd_cnode_lookup(cspace, cap, false); 
		if(cnode == NULL)
		{
			printk(KERN_INFO "lcd_cap_delete cnode not found\n");
			spin_unlock_irqrestore(&cspace->lock, flags);		
			return -EINVAL; 
		}

		spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
		spin_unlock_irqrestore(&cspace->lock, flags);
		if (cnode->type == LCD_CAP_TYPE_FREE) {
			printk(KERN_INFO "lcd_cap_delete cnode is already free\n");
			spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
			return -EINVAL;
		}
		
		if (spin_trylock(&cnode->cdt_root->lock)) {
			cdt_node = cnode->cdt_root;
			// get the subtree rooted at cnode out of CDT
			if (cdt_node->cnode == cnode) {
				if (cnode->next) {
					// this is not possible untill we implement capability mint/duplicate feature
					cdt_node->cnode = cnode->next;
					cnode->next->prev = NULL;
					cnode->next = NULL;
				} else {
					cdt_node->cnode = NULL;
				}
			} else {
				if (cnode->parent) {
					if (cnode->parent->child == cnode) {
						if (cnode->next) {
							cnode->parent->child = cnode->next;
							cnode->next->prev = NULL;
							cnode->next = NULL;
						} else {
							cnode->parent->child = NULL;
						}
					} else {
						if (cnode->prev) {
							cnode->prev->next = cnode->next;
						}
						if (cnode->next) {
							cnode->next->prev = cnode->prev;
						}
						cnode->next = NULL;
						cnode->prev = NULL;
					}
					cnode->parent = NULL;
				}
			} 
			// cnode is now isolated from CDT, before unlocking mark it invalid
			cnode->type = LCD_CAP_TYPE_INVALID;
			spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);

			// no one else can modify the CDT relationships as long as we hold CDT lock
			while (cnode->child) {
				childnode = cnode->child;
				spin_lock_irqsave(&childnode->lock, flags_cnode_lock);
				cspace = childnode->cspace;
				cap = childnode->cap;
				_lcd_delete_node(childnode);
				childnode->cdt_root = NULL;
				spin_unlock_irqrestore(&childnode->lock, flags_cnode_lock);
				lcd_free_cap(&(cspace->cap_cache), cap);
			}
			
			spin_lock_irqsave(&cnode->lock, flags_cnode_lock);
			cspace = cnode->cspace;
			cap = cnode->cap;
			last_node = _lcd_delete_node(cnode);
			if (last_node) {	
				cdt_node->state = ALLOCATION_MARKED_FOR_DELETE;
			}
			cnode->cdt_root = NULL;
			spin_unlock(&(cdt_node->lock));
			done = true;
		} 
		spin_unlock_irqrestore(&cnode->lock, flags_cnode_lock);
		
		
		if (done) {
			if (last_node) {	
				free_cdt_root(cdt_node);
			}
			lcd_free_cap(&(cspace->cap_cache), cap);
		} else {
			// someone is using CDT root
			// wait for him to finish
			msleep(1);
		}
	} while (!done);

	return 0;
};
EXPORT_SYMBOL(lcd_cap_revoke);

int lcd_cap_destroy_cspace(struct cspace *cspace) {
	
	int res;
	unsigned long flags;
	spin_lock_irqsave(&cspace->lock, flags);
	if (cspace->state != ALLOCATION_VALID)
	{
		spin_unlock_irqrestore(&cspace->lock, flags);		
		return -EIDRM;
	}
	cspace->state = ALLOCATION_MARKED_FOR_DELETE;
	spin_unlock_irqrestore(&cspace->lock, flags);
	res = _lcd_delete_table(cspace, cspace->cnode_table);
	return res;
};
EXPORT_SYMBOL(lcd_cap_destroy_cspace);

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
bool _lcd_delete_node (struct cnode *cnode){

	bool last_node;
	cnode->object = NULL; 
	cnode->type = LCD_CAP_TYPE_FREE;

	cnode->cdt_root->node_count--;
	if (cnode->cdt_root->cnode == cnode) {
		if (cnode->child) {
			cnode->cdt_root->cnode = cnode->child;
		} else if (cnode->next) {
			cnode->cdt_root->cnode = cnode->next;
		} else {
			cnode->cdt_root->cnode = NULL;
		}
	}
	if (cnode->cdt_root->node_count > 0) {
		last_node = false;
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
		last_node = true;
	} 	
	return last_node;
}  

int _lcd_delete_table(struct cspace *cspace, struct cnode_table *table) {

	unsigned long flags;
	capability_t index = MAX_CAP_SLOTS;
	struct cnode *cnode;
	struct cdt_root_node *cdt_node;
	bool last_node = false;

	if (table == NULL || table->cnode[index].type == LCD_CAP_TYPE_FREE) {
		return -EINVAL;
	}
	while (index < MAX_SLOTS_PER_TABLE) {
		_lcd_delete_table(cspace, table->cnode[index].object);
		// traversal through cnode_table happens only after locking cspace and checking if cspace is valid.
		// we have already marked cspace as invalid so insert, revoke and delete cannot reach here.
		table->cnode[index].type = LCD_CAP_TYPE_FREE;
		index++;
	}

	index = 0;
	while (index < MAX_CAP_SLOTS) {
		spin_lock_irqsave(&(table->cnode[index].lock), flags); 
		if (table->cnode[index].type == LCD_CAP_TYPE_FREE) {
			index++;
			spin_unlock_irqrestore(&(table->cnode[index].lock), flags);
			continue;
		}
		cnode = &(table->cnode[index]);
		if (spin_trylock(&(cnode->cdt_root->lock))) {
			cdt_node = cnode->cdt_root;
			last_node = _lcd_delete_node(cnode);
			if (last_node) {	
				cdt_node->state = ALLOCATION_MARKED_FOR_DELETE;
			}
			cnode->cdt_root = NULL;
			spin_unlock(&(cdt_node->lock));
			spin_unlock_irqrestore(&(cnode->lock), flags);
			lcd_free_cap(&(cspace->cap_cache), cnode->cap);
			index++;
		} else {
			spin_unlock_irqrestore(&(table->cnode[index].lock), flags);	
			msleep(1);
		}
		if (last_node) {	
			free_cdt_root(cdt_node);	
			last_node = false;
		} 
	}

	// no locks are being held till here
	spin_lock_irqsave(&cspace->lock, flags);
	kmem_cache_free(cspace->cnode_table_cache, table);
	spin_unlock_irqrestore(&cspace->lock, flags);
	return 0;
}