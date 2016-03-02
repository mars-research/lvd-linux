/**
 * boot.c - non-isolated kernel module, does setup and
 *          send/recv
 *
 */

#include <lcd-domains/kliblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

static int do_recv(cptr_t endpoint, cptr_t *page_cptr_out,
		char **data)
{
	int ret;
	cptr_t page_cptr;
	unsigned long offset;
	gva_t page;
	/*
	 * Prepare to receive a page that contains data
	 */
	ret = lcd_alloc_cptr(&page_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr failed");
		goto fail1;
	}
	lcd_set_cr1(page_cptr);
	/*
	 * Listen for message from lcd
	 */
	ret = lcd_recv(endpoint);
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
	ret = lcd_map_page_both(page_cptr, &page);
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

	*page_cptr_out = page_cptr;

	return 0;

fail3:
	lcd_cap_delete(page_cptr);
fail2:
	lcd_free_cptr(page_cptr);
fail1:
	return ret;
}

static int do_send(cptr_t endpoint, char *data, int len,
		cptr_t *page_out)
{
	int ret;
	cptr_t p;
	unsigned long offset;
	/*
	 * Get the cptr for the page that contains data
	 */
	ret = lcd_virt_addr_to_page_cptr(data, len, &p, &offset);
	if (ret) {
		LIBLCD_ERR("lcd virt addr to page cptr failed");
		goto fail1;
	}
	/*
	 * Set up message for grant
	 */
	lcd_set_cr1(p);
	lcd_set_r0(offset);
	ret = lcd_send(endpoint);
	if (ret) {
		LIBLCD_ERR("send failed");
		goto fail2;
	}
	/*
	 * Return cptr to page involved in lookup.
	 */
	*page_out = p;

	return 0;

fail2:
	lcd_addr_to_page_cptr_cleanup(p);	
fail1:
	return ret;
}

static int do_stuff(cptr_t endpoint)
{
	int ret;
	struct page *p;
	char *str;
	char *data;
	cptr_t page1, page2, page3;
	/*
	 * Allocate a page
	 */
	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LIBLCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(page_address(p), 0, PAGE_SIZE);
	/*
	 * Write some data at an offset into the page
	 */
	str = page_address(p) + 64;
	*(str + 0) = 'h';
	*(str + 1) = 'e';
	*(str + 2) = 'l';
	*(str + 3) = 'l';
	*(str + 4) = 'o';
	*(str + 5) = 0;
	/*
	 * "send" the string to the lcd
	 */
	ret = do_send(endpoint, str, 6, &page1);
	if (ret) {
		LIBLCD_ERR("do first send failed");
		goto fail2;
	}
	/*
	 * receive it back
	 */
	ret = do_recv(endpoint, &page2, &data);
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
	ret = do_send(endpoint, data, strlen(data), &page3);
	if (ret) {
		LIBLCD_ERR("do second send failed");
		goto fail5;
	}
	/*
	 * Free page (don't worry about other tedious clean up, not
	 * really necessary for this simple example)
	 */
	__free_page(p);

	return 0;

	
fail5:
fail4:
fail3:
fail2:
	__free_page(p);
fail1:
	return ret;
}

static int boot_main(void)
{
	int ret;
	cptr_t endpoint;
	cptr_t lcd;
	struct lcd_info *mi;
	cptr_t dest;
	/*
	 * Enter lcd mode
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
	ret = lcd_create_module_lcd(&lcd, 
				LCD_DIR("string_example"),
				"lcd_test_mod_string_example_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail3;
	}
	/*
	 * Allocate a cptr for the lcd to hold the endpoint
	 */
	ret = __lcd_alloc_cptr(mi->cache, &dest);
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
	ret = lcd_dump_boot_info(mi);
	if (ret) {
		LIBLCD_ERR("dump boot info");
		goto fail6;
	}
	to_boot_info(mi)->cptrs[0] = dest;
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail7;
	}

	/* -------------------------------------------------- */
	/* -------------------------------------------------- */

	ret = do_stuff(endpoint);
	if (ret) {
		LIBLCD_ERR("do stuff failed");
		goto fail8;
	}
	/*
	 * Hang out for a sec for lcd to exit, then tear everything down
	 */
	msleep(2000);
	ret = 0;
	goto out;

out:
fail8:
fail7:
fail6:
fail5:
	/* 
	 * No need to "ungrant" - everything is taken care of during tear
	 * down
	 */
fail4:
	/* 
	 * This call is necessary because we need to tear down the module
	 * on the host (otherwise, an lcd_cap_delete would be sufficient
	 */
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
fail3:
fail2:
	lcd_exit(0); /* will free endpoint */
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
