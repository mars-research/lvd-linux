/* Author: Anton Burtsev
 * Copyright: University of Utah */

#ifndef __LCD_CAP_H__
#define __LCD_CAP_H__

#include <linux/types.h>
#include <linux/spinlock.h>

#include <uapi/linux/lcd-cap.h>

enum lcd_cap_type
{
	LCD_TYPE_INVALID,
	LCD_TYPE_FREE,
	LCD_TYPE_SYNC_EP,
};

struct cnode {
	enum lcd_cap_type type;
	void *object;
};

struct cspace {
	spinlock_t lock; 
	struct cnode *cnode;
};

int lcd_cap_init(void);
int lcd_cap_exit(void);

void lcd_cnode_release(struct cnode *cnode);

int lcd_init_cspace(struct cspace *cspace);
int lcd_cnode_insert(struct cspace *cspace, capability_t cap, struct cnode *cnode);
struct cnode *lcd_cnode_lookup(struct cspace *cspace, capability_t cap);

#endif
