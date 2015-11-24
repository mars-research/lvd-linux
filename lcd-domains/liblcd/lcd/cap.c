
#include <lcd-domains/liblcd-config.h>
#include <linux/string.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

/* CPTRS -------------------------------------------------- */

/*
 * XXX: This is hardwired for a depth of 3 (4 levels) so that we
 * don't depend kmalloc (we need the cptr cache before kmalloc is
 * up and running). If you change the cspace depth, table size, 
 * etc., you will need to change this.
 *
 * XXX: We don't use a lock because this is only used by one thread, for now.
 */

struct cptr_cache_2 {
	/* level 0 */
	unsigned long bmap0[LCD_BMAP0_SIZE];
	/* level 1 */
	unsigned long bmap1[LCD_BMAP1_SIZE];
	/* level 2 */
	unsigned long bmap2[LCD_BMAP2_SIZE];
	/* level 3 */
	unsigned long bmap3[LCD_BMAP3_SIZE];
};

/*
 * XXX: For now, each lcd has one thread, so we can use one cptr
 * cache. (Avoid using kmalloc.)
 */
static struct cptr_cache_2 cptr_cache;


int lcd_init_cptr(void)
{
	struct lcd_boot_info *bi;
	/*
	 * Read from boot param page
	 */
	bi = lcd_get_boot_info();

	memcpy(cptr_cache.bmap0, bi->bmap0, sizeof(bi->bmap0));
	memcpy(cptr_cache.bmap1, bi->bmap1, sizeof(bi->bmap1));
	memcpy(cptr_cache.bmap2, bi->bmap2, sizeof(bi->bmap2));
	memcpy(cptr_cache.bmap3, bi->bmap3, sizeof(bi->bmap3));

	return 0;
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

static int __lcd_alloc_cptr(struct cptr_cache_2 *cache, cptr_t *free_cptr)
{
	int depth;
	int done;
	unsigned long idx;
	/*
	 * Can't use a loop since we didn't kmalloc the bitmaps
	 */
	done = __lcd_alloc_cptr_from_bmap(cache->bmap0, 
					LCD_BMAP0_SIZE, &idx);
	if (done) {
		depth = 0;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap1, 
					LCD_BMAP1_SIZE, &idx);
	if (done) {
		depth = 1;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap2, 
					LCD_BMAP2_SIZE, &idx);
	if (done) {
		depth = 2;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap3, 
					LCD_BMAP3_SIZE, &idx);
	if (done) {
		depth = 3;
		goto found;
	}

	/* Didn't find one */
	return -ENOMEM;

found:
	/*
	 * Found one; dec depth back to what it was, and encode
	 * depth in cptr
	 */

	idx |= (depth << LCD_CPTR_LEVEL_SHIFT);
	*free_cptr = __cptr(idx);

	return 0; 
}

void __lcd_free_cptr(struct cptr_cache_2 *cache, cptr_t c)
{
	unsigned long *bmap;
	unsigned long bmap_idx;
	unsigned long level;
	/*
	 * Get the correct level bitmap
	 */
	level = lcd_cptr_level(c);
	switch (level) {
	case 0:
		bmap = cache->bmap0;
		break;
	case 1:
		bmap = cache->bmap1;
		break;
	case 2:
		bmap = cache->bmap2;
		break;
	case 3:
		bmap = cache->bmap3;
		break;
	default:
		/* error shouldn't happen, but if so, just return */
		return;
	}
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

int lcd_alloc_cptr(cptr_t *free_slot)
{
	return __lcd_alloc_cptr(&cptr_cache, free_slot);
}

void lcd_free_cptr(cptr_t c)
{
	__lcd_free_cptr(&cptr_cache, c);
}

/* CAPABILITIES -------------------------------------------------- */

void lcd_cap_delete(cptr_t slot)
{
	int ret;
	/*
	 * Call to microkernel
	 */
	lcd_set_cr0(slot);
	ret = LCD_DO_SYSCALL(LCD_SYSCALL_CAP_DELETE);
	if (ret)
		lcd_printk("lcd_cap_delete: syscall err");
	/*
	 * Free cptr
	 */
	lcd_free_cptr(slot);
}
