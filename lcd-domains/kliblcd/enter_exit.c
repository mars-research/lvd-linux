/*
 * enterexit.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>
#include <thc.h>

struct task_struct *klcd_thread;
EXPORT_SYMBOL(klcd_thread);

int lcd_enter(void)
{
	int ret;
	struct lcd *lcd;
	struct cptr_cache *cache;
	struct lcd_resource_tree *t;
	cptr_t unused;
	/*
	 * This sets up the runtime environment for non-isolated
	 * threads.
	 *
	 * First, if we weren't created by another klcd, then we
	 * need to init our lcd, cspace, utcb, and so on.
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
	ret = cptr_cache_alloc(&cache);
	if (ret) {
		LCD_ERR("cptr cache alloc");
		goto fail2;
	}
	klcd_thread = current;
	printk("%s current %p\n", __func__, current);
	current->cptr_cache = cache;
	ret = cptr_cache_init(cache);
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

#ifndef CONFIG_LVD
	/*
	 * Create our call endpoint (for receiving rpc replies)
	 */
	ret = _lcd_create_sync_endpoint(LCD_CPTR_CALL_ENDPOINT);
        if (ret) {
                LCD_ERR("creating call endpoint");
                goto fail6;
        }
#endif
	/*
	 * Set up resource trees
	 */
	ret = lcd_alloc_init_resource_tree(&t);
	if (ret) {
		LCD_ERR("creating resource tree");
		goto fail7;
	}
	current->lcd_resource_trees[0] = t;	
	ret = lcd_alloc_init_resource_tree(&t);
	if (ret) {
		LCD_ERR("creating resource tree");
		goto fail8;
	}
	current->lcd_resource_trees[1] = t;
	/*
	 * Set up thc runtime
	 */
	thc_init();

	return 0;

fail8:
fail7:
#ifndef CONFIG_LVD
fail6:
#endif
fail5:
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
		/*
		 * The guy who created us will tear our LCD down
		 */
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
	cptr_cache_destroy(c);
	cptr_cache_free(c);
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
	if (current->lcd_resource_trees[0])
		lcd_destroy_free_resource_tree(current->lcd_resource_trees[0]);
	if (current->lcd_resource_trees[1])
		lcd_destroy_free_resource_tree(current->lcd_resource_trees[1]);
	if (current->ptstate)
		thc_done();

	current->lcd = NULL;
	current->cptr_cache = NULL;
	current->lcd_resource_trees[0] = NULL;
	current->lcd_resource_trees[1] = NULL;
	current->ptstate = NULL;
	/* (Call endpoint should be auto-destroyed when we destroy
	 * the LCD because this will destroy its cspace. So long as
	 * the LCD didn't grant the endpoint to someone.) */
}

void lcd_abort(void)
{
	lcd_exit(-EIO);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_enter);
EXPORT_SYMBOL(lcd_exit);
EXPORT_SYMBOL(lcd_abort);
