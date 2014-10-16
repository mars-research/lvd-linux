/*
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */

#ifndef _LCD_CAP_CACHE_H_
#define _LCD_CAP_CACHE_H_

#include <linux/spinlock.h>
#include <uapi/linux/lcd-cap.h>

struct cap_cache {
	uint32_t *cap;
	uint32_t head;
	spinlock_t lock;
};

int lcd_init_list_cache(struct cap_cache *list_cache, uint64_t size);
void lcd_free_list_cache(struct cap_cache *list_cache);
int lcd_alloc_cap(struct cap_cache *cap_cache, capability_t *free_cap);
void lcd_free_cap(struct cap_cache *cap_cache, capability_t free_cap);

#endif
