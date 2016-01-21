/**
 * cptr_cache.c
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/slab.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include <linux/mutex.h>
#include "../microkernel/internal.h"

static int cptr_cache_init(struct cptr_cache **out)
{
	struct cptr_cache *cache;
	int ret;
	int i, j;
	int nbits;
	/*
	 * Allocate the container
	 */
	cache = kzalloc(sizeof(*cache), GFP_KERNEL);
	if (!cache) {
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Allocate the bitmaps
	 */
	for (i = 0; i < (1 << LCD_CPTR_DEPTH_BITS); i++) {
		/*
		 * For level i, we use the slot bits plus i * fanout bits
		 *
		 * So e.g. for level 0, we use only slot bits, so there
		 * are only 2^(num slot bits) cap slots at level 0.
		 */
		nbits = 1 << (LCD_CPTR_SLOT_BITS + i * LCD_CPTR_FANOUT_BITS);
		/*
		 * Alloc bitmap
		 */
		cache->bmaps[i] = kzalloc(sizeof(unsigned long) *
					BITS_TO_LONGS(nbits),
					GFP_KERNEL);
		if (!cache->bmaps[i]) {
			ret = -ENOMEM;
			goto fail2; /* i = level we failed at */
		}
	}
	/*
	 * Mark reserved cptr's as allocated
	 */
	set_bit(0, cache->bmaps[0]);
	set_bit(1, cache->bmaps[0]);
	set_bit(2, cache->bmaps[0]);

	*out = cache;

	return 0;

fail2:
	for (j = 0; j < i; j++)
		kfree(cache->bmaps[j]);
	kfree(cache);
fail1:
	return ret;
}

static void cptr_cache_destroy(struct cptr_cache *cache)
{
	int i;
	/*
	 * Free bitmaps
	 */
	for (i = 0; i < (1 << LCD_CPTR_DEPTH_BITS); i++)
		kfree(cache->bmaps[i]);
	/*
	 * Free container
	 */
	kfree(cache);
}

static int __lcd_alloc_cptr_from_bmap(unsigned long *bmap, int size,
				unsigned long *out)
{
	unsigned long idx;
	/*
	 * Find next zero bit
	 */
	idx = find_first_zero_bit(bmap, size);
	if (idx >= size)
		return 0; /* signal we are full */
	/*
	 * Set bit to mark cptr as in use
	 */
	set_bit(idx, bmap);

	*out = idx;

	return 1; /* signal we are done */
}

int __klcd_alloc_cptr(struct cptr_cache *cptr_cache, cptr_t *free_cptr)
{
	int ret;
	int depth;
	int done;
	unsigned long *bmap;
	unsigned long idx;
	int size;

	depth = 0;
	do {
		bmap = cptr_cache->bmaps[depth];
		size = 1 << (LCD_CPTR_SLOT_BITS + 
			depth * LCD_CPTR_FANOUT_BITS);
		done = __lcd_alloc_cptr_from_bmap(bmap, size, &idx);
		depth++;
	} while (!done && depth < (1 << LCD_CPTR_DEPTH_BITS));

	if (!done) {
		/*
		 * Didn't find one
		 */
		LCD_ERR("out of cptrs");
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Found one; dec depth back to what it was, and encode
	 * depth in cptr
	 */
	depth--;
	idx |= (depth << LCD_CPTR_LEVEL_SHIFT);
	*free_cptr = __cptr(idx);

	return 0; 

fail2:
	return ret;
}

void __klcd_free_cptr(struct cptr_cache *cptr_cache, cptr_t c)
{
	unsigned long *bmap;
	unsigned long bmap_idx;
	unsigned long level;

	/*
	 * Get the correct level bitmap
	 */
	level = lcd_cptr_level(c);
	bmap = cptr_cache->bmaps[level];
	/*
	 * The bitmap index includes all fanout bits and the slot bits
	 */
	bmap_idx = ((1 << (LCD_CPTR_FANOUT_BITS * level + LCD_CPTR_SLOT_BITS))
		- 1) & cptr_val(c);
	/*
	 * Clear the bit in the bitmap
	 */
	clear_bit(bmap_idx, bmap);

	return; 
}

int klcd_alloc_cptr(cptr_t *free_slot)
{
	return __lcd_alloc_cptr(current->cptr_cache, free_slot);
}

void klcd_free_cptr(cptr_t c)
{
	__lcd_free_cptr(current->cptr_cache, c);
}

int klcd_init_cptr(struct cptr_cache **c_out)
{
	return cptr_cache_init(c_out);
}

void klcd_destroy_cptr(struct cptr_cache *c)
{
	cptr_cache_destroy(c);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(__klcd_alloc_cptr);
EXPORT_SYMBOL(__klcd_free_cptr);
EXPORT_SYMBOL(klcd_alloc_cptr);
EXPORT_SYMBOL(klcd_free_cptr);
