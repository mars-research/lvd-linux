/*
 * lcd.c - code for isolated LCD in liblcd test
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
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

	
	goto out;

out:
	lcd_exit(ret);
}

static void liblcd_test_lcd_exit(void)
{
	return;
}

module_init(liblcd_test_lcd_init);
module_exit(liblcd_test_lcd_exit);
