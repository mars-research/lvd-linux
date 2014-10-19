/* Author: Anton Burtsev
 * Copyright: University of Utah */

#ifndef __LCD_CAP_H__
#define __LCD_CAP_H__

#include <linux/types.h>
#include <linux/spinlock.h>
#include "cap-cache.h"

#include <uapi/linux/lcd-cap.h>

#define LCD_MAX_CAPS 32

#define CAP_IDENTIFIER_BITS 2
#define TABLE_IDENTIFIER_BITS 1

// should always be 2^CAP_IDENTIFIER_BITS
#define MAX_CAP_SLOTS 4	
// should always be 2^TABLE_IDENTIFIER_BITS
#define MAX_TABLE_ID_SLOTS 2
// should always be 2^CAP_IDENTIFIER_BITS + 2^TABLE_IDENTIFIER_BITS
#define MAX_SLOTS_PER_TABLE 6

struct cdt_root_node;
struct cnode;
struct cspace;

extern struct cdt_root_node * get_cdt_root(void);
extern void free_cdt_root(struct cdt_root_node *cdt_node);

enum lcd_cap_type
{
	LCD_CAP_TYPE_INVALID,
	LCD_CAP_TYPE_FREE,
	LCD_CAP_TYPE_SYNC_EP,
	LCD_CAP_TYPE_CNODE
};

enum allocation_state {
	ALLOCATION_INVALID,
	ALLOCATION_VALID,
	ALLOCATION_MARKED_FOR_DELETE,
	ALLOCATION_REMOVED
};

struct cdt_root_node {
	spinlock_t lock;
	struct cnode *cnode;
	unsigned long node_count;
	enum allocation_state state;
};

struct cnode {
	enum lcd_cap_type type;
	void *object;
	spinlock_t lock;
	struct cdt_root_node *cdt_root;
	struct cnode *parent, *child, *prev, *next;
	capability_t cap;
	struct cspace *cspace;
};

struct cnode_table {
	struct cnode cnode[MAX_SLOTS_PER_TABLE];
	uint8_t table_level;
};

struct cspace {
	spinlock_t lock; 
	enum allocation_state state;
	struct cnode_table *cnode_table;
	struct kmem_cache *cnode_table_cache;
	struct cap_cache cap_cache;
};

bool _get_level_bits(int table_level, capability_t cap, capability_t *levelId);
bool _lcd_delete_node (struct cnode *cnode);
int _lcd_delete_table(struct cspace *cspace, struct cnode_table *table);

int lcd_cap_init_cspace(struct cspace *cspace);
struct cnode *lcd_cnode_lookup(struct cspace *cspace, capability_t cap, bool alloc);
int lcd_cap_insert(struct cspace *cspace, capability_t cap, 
		void *object, enum lcd_cap_type type);
void lcd_cap_delete(struct cspace *cspace, capability_t cap);
int lcd_cap_grant(struct cspace *cspacesrc, capability_t capsrc, struct cspace *cspacedst, capability_t capdst);
int lcd_cap_revoke(struct cspace *cspace, capability_t cap);
int lcd_cap_destroy_cspace(struct cspace *cspace);
#endif
