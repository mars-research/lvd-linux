/*
 * boot.c - non-isolated boot module
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>

static int do_recv(cptr_t endpoint, char **data)
{
	int ret;
	cptr_t page_cptr;
	unsigned long offset;
	gva_t page;
	/*
	 * Prepare to receive a page that contains data
	 */
	ret = lcd_cptr_alloc(&page_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr failed");
		goto fail1;
	}
	lcd_set_cr1(page_cptr);
	/*
	 * Listen for message from lcd
	 */
	ret = lcd_sync_recv(endpoint);
	if (ret) {
		LIBLCD_ERR("recv failed");
		goto fail2;
	}
	/*
	 * Extract info
	 */
	offset = lcd_r0();
	/*
	 * Map page
	 */
	ret = lcd_map_virt(page_cptr, 0, &page);
	if (ret) {
		LIBLCD_ERR("failed to map page");
		goto fail3;
	}
	/*
	 * Get data
	 *
	 * gva == hva for non-isolated
	 */
	*data = (char *)(gva_val(page) + offset);

	return 0;

fail3:
	lcd_cap_delete(page_cptr);
fail2:
	lcd_cptr_free(page_cptr);
fail1:
	return ret;
}

static int do_send(cptr_t endpoint, char *data, int len)
{
	int ret;
	unsigned long size, offset;
	cptr_t page;
	/*
	 * Get the cptr for the page that contains data
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)data), &page, 
			&size, &offset);
	if (ret) {
		LIBLCD_ERR("lcd virt addr to page cptr failed");
		goto fail1;
	}
	/*
	 * Set up message for grant
	 */
	lcd_set_cr1(page);
	lcd_set_r0(offset);
	ret = lcd_sync_send(endpoint);
	if (ret) {
		LIBLCD_ERR("send failed");
		goto fail2;
	}

	return 0;

fail2:
fail1:
	return ret;
}

static int do_stuff(cptr_t endpoint)
{
	int ret;
	struct page *p;
	char *str;
	char *data;
	/*
	 * Allocate a page
	 */
	p = lcd_alloc_pages(GFP_KERNEL, 0);
	if (!p) {
		LIBLCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(lcd_page_address(p), 0, PAGE_SIZE);
	/*
	 * Write some data at an offset into the page
	 */
	str = lcd_page_address(p) + 64;
	*(str + 0) = 'h';
	*(str + 1) = 'e';
	*(str + 2) = 'l';
	*(str + 3) = 'l';
	*(str + 4) = 'o';
	*(str + 5) = 0;
	/*
	 * "send" the string to the lcd
	 */
	ret = do_send(endpoint, str, 6);
	if (ret) {
		LIBLCD_ERR("do first send failed");
		goto fail2;
	}
	/*
	 * receive it back
	 */
	ret = do_recv(endpoint, &data);
	if (ret) {
		LIBLCD_ERR("do recv failed");
		goto fail3;
	}
	LIBLCD_MSG("boot recvd string = %s (should be 'hello')", data);
	if (strcmp(data, "hello")) {
		LIBLCD_ERR("bad data");
		goto fail4;
	}
	/*
	 * send it again
	 */
	ret = do_send(endpoint, data, strlen(data));
	if (ret) {
		LIBLCD_ERR("do second send failed");
		goto fail5;
	}
	/*
	 * Free page (don't worry about other tedious clean up, not
	 * really necessary for this simple example)
	 */
	lcd_free_pages(p, 0);

	return 0;

	
fail5:
fail4:
fail3:
fail2:
	lcd_free_pages(p, 0);
fail1:
	return ret;
}

static int boot_main(void)
{
	int ret;
	cptr_t lcd;
	struct lcd_create_ctx *ctx;
	cptr_t endpoint, dest;
	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create an endpoint
	 */
	ret = lcd_create_sync_endpoint(&endpoint);
	if (ret) {
		LIBLCD_ERR("failed to create endpoint");
		goto fail2;
	}
	/*
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(LCD_DIR("string_example/lcd"),
				"lcd_test_mod_string_example_lcd",
				&lcd, 
				&ctx);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail3;
	}
	/*
	 * Allocate a cptr for the lcd to hold the endpoint
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), &dest);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail4;
	}
	/*
	 * Grant access to endpoint
	 */
	ret = lcd_cap_grant(lcd, endpoint, dest);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd");
		goto fail5;
	}
	/*
	 * Set up boot info
	 */
	lcd_to_boot_info(ctx)->cptrs[0] = dest;
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail6;
	}
	/* -------------------------------------------------- */
	/* -------------------------------------------------- */

	ret = do_stuff(endpoint);
	if (ret) {
		LIBLCD_ERR("do stuff failed");
		goto fail7;
	}
	/*
	 * Hang out for a second
	 */
	msleep(1000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail7:
fail6:
fail5:
fail4:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx);
fail3:
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
