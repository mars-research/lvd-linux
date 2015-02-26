/**
 * liblcd.c - Code for microkernel interface for isolated code.
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */


#include <asm/lcd-domains/liblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>

/* CPTR CACHE -------------------------------------------------- */

/* This is not needed yet. */

#if 0
/*
 * XXX: This is hardwired for a depth of 3 (4 levels) so that we
 * don't have to use kmalloc. If you change the cspace depth, table size, 
 * etc., you will need to change this.
 *
 * XXX: We don't use a lock because this is only used by one thread.
 */
#define LCD_BMAP0_SIZE (1 << (LCD_CPTR_SLOT_BITS + 0 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP1_SIZE (1 << (LCD_CPTR_SLOT_BITS + 1 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP2_SIZE (1 << (LCD_CPTR_SLOT_BITS + 2 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP3_SIZE (1 << (LCD_CPTR_SLOT_BITS + 3 * LCD_CPTR_FANOUT_BITS))
struct cptr_cache {
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
 * XXX: For now, each kernel module has one thread, so we can use one cptr
 * cache. (Avoid using kmalloc.)
 */
static struct cptr_cache cptr_cache;

static void cptr_cache_init(struct cptr_cache *cache)
{
	/*
	 * Only need to set the null cptr's bit so we don't
	 * allocate that
	 */
	set_bit(0, cache->bmap0);
}

static void cptr_cache_destroy(struct cptr_cache *cache)
{
	/* no op for now - no kmalloc */
	return;
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

static int __lcd_alloc_cptr(struct cptr_cache *cache, cptr_t *free_cptr)
{
	int ret;
	int depth;
	int done;
	unsigned long *bmap;
	unsigned long idx;
	int size;
	/*
	 * Can't use a loop since we didn't kmalloc the bitmaps
	 */
	done = __lcd_alloc_cptr_from_bmap(cache->bmap0, LCD_BMAP0_SIZE, &idx);
	if (done) {
		depth = 0;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap1, LCD_BMAP1_SIZE, &idx);
	if (done) {
		depth = 0;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap2, LCD_BMAP2_SIZE, &idx);
	if (done) {
		depth = 0;
		goto found;
	}
	done = __lcd_alloc_cptr_from_bmap(cache->bmap3, LCD_BMAP3_SIZE, &idx);
	if (done) {
		depth = 0;
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

fail2:
fail1:
	return ret;
}

void __lcd_free_cptr(struct cptr_cache *cache, cptr_t c)
{
	int ret;
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
	return __lcd_alloc_cptr(cptr_cache, free_slot);
}

void lcd_free_cptr(cptr_t c)
{
	__lcd_free_cptr(cptr_cache, c);
}
#endif

int lcd_alloc_cptr(cptr_t *free_slot)
{
	return -ENOSYS;
}

void lcd_free_cptr(cptr_t c)
{
	return;
}


/* BOOT INFO FRAME -------------------------------------------------- */




/* GUEST PHYSICAL ALLOC -------------------------------------------------- */






/* GUEST VIRTUAL ALLOC / MAP / ETC ---------------------------------------- */






/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */


int lcd_page_alloc(cptr_t *slot_out, gpa_t gpa)
{
	return -ENOSYS;
}

int lcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out)
{
	return -ENOSYS;
}

/* LCD ENTER / EXIT -------------------------------------------------- */


int lcd_enter(void)
{
	/*
	 * For now, we don't do anything. In the future, we could set up
	 * the cptr cache, page allocation, etc.
	 */
	return 0;
}

void __noreturn lcd_exit(int retval)
{
	lcd_set_r0(retval);
	for(;;)
		LCD_DO_SYSCALL(LCD_SYSCALL_EXIT); /* doesn't return */
}


/* IPC -------------------------------------------------- */


int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	return -ENOSYS;
}

int lcd_send(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_SEND);
}

int lcd_recv(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_RECV);
}

int lcd_call(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_CALL);
}

int lcd_reply(void)
{
	return LCD_DO_SYSCALL(LCD_SYSCALL_REPLY);
}

/* LCD CREATE / SETUP -------------------------------------------------- */


int lcd_create(cptr_t *slot_out, gpa_t stack)
{
	return -ENOSYS;
}

int lcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root)
{
	return -ENOSYS;
}

int lcd_run(cptr_t lcd)
{
	return -ENOSYS;
}

int lcd_suspend(cptr_t lcd)
{
	return -ENOSYS;
}


/* CAPABILITIES -------------------------------------------------- */


int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return -ENOSYS;
}

int lcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, gpa_t gpa)
{
	return -ENOSYS;
}

void lcd_cap_delete(cptr_t slot)
{
	return;
}

int lcd_cap_revoke(cptr_t slot)
{
	return -ENOSYS;
}


/* EXTRAS -------------------------------------------------- */


/* TODO : not implemented for regular lcd's yet */
