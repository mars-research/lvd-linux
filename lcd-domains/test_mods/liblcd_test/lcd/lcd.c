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
		ret = lcd_alloc_cptr(&cptrs[i]);
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
		lcd_free_cptr(cptrs[j]);

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
