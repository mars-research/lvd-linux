/*
 * lcd.c - code for isolated LCD in liblcd test
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

static int cptr_test(void)
{
	cptr_t cptrs[10];
	int i, j;
	int ret;

	for (i = 0; i < 10; i++) {
		ret = lcd_cptr_alloc(&cptrs[i]);
		if (ret) {
			LIBLCD_ERR("failed at i = %d", i);
			goto fail;
		}
		if (cptr_is_null(cptrs[i])) {
			LIBLCD_ERR("got null cptr");
			i++;
			goto fail;
		}
	}

	ret = 0;
	goto out;

out:
fail:
	for (j = 0; j < i; j++)
		lcd_cptr_free(cptrs[j]);

	return ret;
}

static int low_level_page_alloc_test(void)
{
	int ret;
	cptr_t pages;
	/*
	 * Allocate pages exact node
	 */
	ret = _lcd_alloc_pages_exact_node(0, 0, 3, &pages);
	if (ret) {
		LIBLCD_ERR("alloc pages exact node failed");
		goto out;
	}
	lcd_cap_delete(pages);
	/*
	 * Regular alloc pages
	 */
	ret = _lcd_alloc_pages(0, 3, &pages);
	if (ret) {
		LIBLCD_ERR("alloc pages failed");
		goto out;
	}
	lcd_cap_delete(pages);
	/*
	 * Vmalloc
	 */
	ret = _lcd_vmalloc(7, &pages);
	if (ret) {
		LIBLCD_ERR("alloc pages failed");
		goto out;
	}
	lcd_cap_delete(pages);

	ret = 0;
	goto out;

out:
	return ret;
}

static int page_alloc_test(void)
{
	int ret;
	struct page *allocs[15];
	unsigned int orders[15] = { 0, 1, 1, 1, 2, 3, 4, 4, 5, 6, 7, 7, 7, 8, 9 };
	unsigned int alloc_order[15] = { 12, 5, 4, 11, 2, 9, 1, 10, 8, 0, 13, 7, 6, 14, 3 };
	unsigned int order;
	int i, j, k;
	unsigned long n;
	unsigned char *ptr;

	/*
	 * This test is config dependent, unfortunately ...
	 */

	for (i = 0; i < 15; i++) {
		allocs[i] = lcd_alloc_pages(0, orders[alloc_order[i]]);
		if (!allocs[i]) {
			LIBLCD_ERR("page alloc order = %d, iteration %d failed",
				orders[alloc_order[i]], i);
			goto fail;
		}
		
		/*
		 * Fill with some data
		 */
		memset(lcd_page_address(allocs[i]), orders[alloc_order[i]], 
			(1UL << (orders[alloc_order[i]] + PAGE_SHIFT)));
	}
	/*
	 * Check them
	 */
	for (k = 0; k < 15; k++) {

		order = orders[alloc_order[k]];
		ptr = lcd_page_address(allocs[k]);

		for (n = 0; n < (1UL << (order + PAGE_SHIFT)); n++) {
			if (ptr[n] != order) {
				LIBLCD_ERR("bad byte at idx 0x%lx for order %d: expected %d, but found %d",
					n, order, order, ptr[n]);
			}
		}

	}

	ret = 0;
	/*
	 * Free em
	 */
	goto out;
	
out:
fail:
	for (j = 0; j < i; j++)
		lcd_free_pages(allocs[j], orders[alloc_order[j]]);
	return ret;
}

static int big_page_alloc_test(void)
{
	struct page *base;
	/*
	 * Try to allocate the maximum (should succeed)
	 */
	base = lcd_alloc_pages(0, MAX_ORDER - 1);
	if (!base) {
		LIBLCD_ERR("big page alloc failed");
		return -1;
	}
	/*
	 * Touch all of it
	 */
	memset(lcd_page_address(base), 0, 
		(1UL << (MAX_ORDER - 1 + PAGE_SHIFT)));
	/*
	 * Free em
	 */
	lcd_free_pages(base, MAX_ORDER - 1);

	return 0;
}

struct a {
	int x, y, z;
	char buff[512];
};

static int kmalloc_test(void)
{
	struct a *x;
	int i;
	int ret;
	/*
	 * Alloc 10 copies of struct a
	 */
	x = kmalloc(sizeof(struct a) * 10, GFP_KERNEL);
	if (!x) {
		LIBLCD_ERR("kmalloc failed");
		ret = -1;
		goto fail1;
	}
	/*
	 * Touch all of the alloc'd memory
	 */
	for (i = 0; i < 10; i++) {
		x[i].x = 2 * i + 1;
		x[i].y = 3 * i + 1;
		x[i].z = 4 * i + 1;
		memset(&x[i].buff, 0, 512);
	}
	kfree(x);

	return 0;

fail1:
	return ret;
}

struct bstruct {
	int x, y, z;
	char buff[512];
};

static int kmem_cache_test(void)
{
	struct kmem_cache *cache;
	int i, j, k;
	int ret;
	struct bstruct *bs[8];
	/*
	 * Set up kmem cache
	 */
	cache = KMEM_CACHE(bstruct, 0);
	if (!cache) {
		LIBLCD_ERR("kmem cache create failed");
		ret = -1;
		goto fail1;
	}
	/*
	 * Alloc 8 b's
	 */
	for (i = 0; i < 8; i++) {
		bs[i] = kmem_cache_zalloc(cache, GFP_KERNEL);
		if (!bs[i]) {
			LIBLCD_ERR("kmem cache alloc failed at idx %d", i);
			ret = -1;
			goto fail2;
		}
	}
	/*
	 * Touch all the mem
	 */
	for (k = 0; k < 8; k++)
		memset(bs[k], 0, sizeof(struct bstruct));

	ret = 0;
	goto out;

out:
fail2:
	for (j = 0; j < i; j++)
		kmem_cache_free(cache, bs[j]);
	kmem_cache_destroy(cache);
fail1:
	return ret;
}

static int __noreturn __init liblcd_test_lcd_init(void) 
{
	int ret = 0;
	ret = lcd_enter();

	ret = cptr_test();
	if (ret) {
		LIBLCD_ERR("cptr tests failed!");
		goto out;
	}
	LIBLCD_MSG("cptr tests passed!");

	ret = low_level_page_alloc_test();
	if (ret) {
		LIBLCD_ERR("low level page alloc tests failed!");
		goto out;
	}
	LIBLCD_MSG("low level page alloc tests passed!");

	ret = page_alloc_test();
	if (ret) {
		LIBLCD_ERR("page alloc tests failed!");
		goto out;
	}
	LIBLCD_MSG("page alloc tests passed!");

	ret = big_page_alloc_test();
	if (ret) {
		LIBLCD_ERR("big page alloc test failed!");
		goto out;
	}
	LIBLCD_MSG("big page alloc test passed!");

	ret = kmalloc_test();
	if (ret) {
		LIBLCD_ERR("kmalloc test failed!");
		goto out;
	}
	LIBLCD_MSG("kmalloc tests passed!");

	ret =  kmem_cache_test();
	if (ret) {
		LIBLCD_ERR("kmem cache tests failed!");
		goto out;
	}
	LIBLCD_MSG("kmem cache tests passed!");

	LIBLCD_MSG("ALL LIBLCD TESTS PASSED!");
	
	goto out;

out:
	if (ret)
		LIBLCD_ERR("AT LEAST ONE LIBLCD TEST FAILS!");
	lcd_exit(ret);
}

static void liblcd_test_lcd_exit(void)
{
	return;
}

module_init(liblcd_test_lcd_init);
module_exit(liblcd_test_lcd_exit);
