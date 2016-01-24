/*
 * enterexit.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd-domains/liblcd.h>
#include "../microkernel/internal.h"

int lcd_enter(void)
{
	int ret;
	struct lcd *lcd;
	struct cptr_cache *cache;
	cptr_t unused;
	/*
	 * This sets up the runtime environment for non-isolated
	 * threads.
	 *
	 * First, if we weren't created by another klcd, then we
	 * need to init our cspace, utcb, and so on.
	 */
	if (!current->lcd) {
		ret = __lcd_create_no_vm_no_thread(&lcd);
		if (ret) {
			LCD_ERR("create lcd");
			goto fail1;
		}
		current->lcd = lcd;
		lcd->kthread = current;
		set_lcd_status(lcd, LCD_STATUS_RUNNING);
		/*
		 * Set type as "top" - a "root" LCD owned by no one (gasp!)
		 */
		lcd->type = LCD_TYPE_TOP;
	}
	/*
	 * Set up our cptr cache
	 */
	ret = cap_cptr_cache_alloc(&cache);
	if (ret) {
		LCD_ERR("cptr cache alloc");
		goto fail2;
	}
	current->cptr_cache = cache;
	ret = cap_cptr_cache_init(cache);
	if (ret) {
		LCD_ERR("cptr cache init");
		goto fail3;
	}
	/*
	 * Reserve first two slots for call/reply caps (just alloc them)
	 */
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LCD_ERR("cptr cache alloc1");
		goto fail4;
	}
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LCD_ERR("cptr cache alloc2");
		goto fail5;
	}
	/*
	 * Create our call endpoint (for receiving rpc replies)
	 */
	ret = __lcd_create_sync_endpoint(current->lcd, LCD_CPTR_CALL_ENDPOINT);
        if (ret) {
                LCD_ERR("creating call endpoint");
                goto fail6;
        }

	return 0;

fail4:
fail3:
fail2:
fail1:
	/* This will do teardown */
	lcd_exit(ret);
	return ret;
}

static void do_destroy_lcd(struct lcd *lcd)
{
	switch (lcd->type) {
	case LCD_TYPE_NONISOLATED:
		__lcd_destroy_no_vm(lcd);
		return;
	case LCD_TYPE_TOP:
		__lcd_destroy_no_vm_no_thread(lcd);
		return;
	default:
		LCD_ERR("unexpected lcd type %d", lcd->type);
		return;
	}
}

static void do_destroy_cptr_cache(struct cptr_cache *c)
{
	cap_cptr_cache_destroy(c);
	cap_cptr_cache_free(c);
}

void lcd_exit(int retval)
{
	/*
	 * Return value is ignored for now
	 */
	if (current->lcd)
		do_destroy_lcd(current->lcd);
	if (current->cptr_cache)
		do_destroy_cptr_cache(current->cptr_cache);
	current->lcd = NULL;
	current->cptr_cache = NULL;
	/* (Call endpoint should be auto-destroyed when we destroy
	 * the LCD because this will destroy its cspace. So long as
	 * the LCD didn't grant the endpoint to someone.) */
}

void lcd_abort(void)
{
	lcd_exit(-EIO);
}
