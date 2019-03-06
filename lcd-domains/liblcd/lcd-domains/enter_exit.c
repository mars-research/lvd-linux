/*
 * enter_exit.c
 *
 * Boot-up code for isolated LCDs, and exit code.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/enter_exit.h>
#include <thc.h>
#include <libfipc.h>
#include <lcd_domains/liblcd.h>
#include <asm/lcd_domains/liblcd.h>
#include <linux/slab.h>

#include <lcd_config/post_hook.h>

static int thc_initialized;
atomic_t lcd_initialized;

static int lcds_entered = 0;

int 
LIBLCD_FUNC_ATTR
lcd_enter(void)
{
	int ret;

	if (atomic_read(&lcd_initialized) == 0) {
		atomic_set(&lcd_initialized, 1);
	} else {
		lcd_printk("===============");
		lcd_printk("  Child LCD BOOTED   ");
		lcd_printk("===============");

		ret = _lcd_create_sync_endpoint(LCD_CPTR_CALL_ENDPOINT);
		if (ret) {
			LIBLCD_ERR("creating call endpoint");
			goto fail;
		}
		LIBLCD_MSG("call endpoint created and installed for child LCD");

		return 0;
	}

	/*
	 * Aside from the call endpoint, order is important ...
	 *
	 * We just return on failure, since the LCD should just exit
	 * on failure (and everything will get torn down and freed
	 * then).
	 *
         * Create our call endpoint (for call/reply interactions)                       */
        ret = _lcd_create_sync_endpoint(LCD_CPTR_CALL_ENDPOINT);
        if (ret) {
                LIBLCD_ERR("creating call endpoint");
                goto fail;
        }
        LIBLCD_MSG("call endpoint created and installed");
	/*
	 * Set up internal memory interval tree / resource tree
	 */
	ret = __liblcd_mem_itree_init(lcds_entered);
	if (ret) {
		LIBLCD_ERR("failed to init memory interval tree");
		goto fail;
	}
	LIBLCD_MSG("memory interval tree initialized");
	/*
	 * Initialize heap and kmalloc
	 */
	ret = __liblcd_heap_init();
	if (ret) {
		LIBLCD_ERR("failed to initialize heap and/or kmalloc");
		goto fail;
	}
	LIBLCD_MSG("heap and kmalloc initialized");
	/*
	 * Initialize RAM map
	 */
	ret = __liblcd_ram_map_init();
	if (ret) {
		LIBLCD_ERR("failed to initialize RAM map");
		goto fail;
	}
	LIBLCD_MSG("RAM map initialized");

	/*
	 * Initialize ioremap map
	 */
	ret = __liblcd__ioremap_init();
        if (ret) {
                LIBLCD_ERR("failed to initialize ioremap map");
                goto fail;
        }
        LIBLCD_MSG("ioremap map initialized");

	lcd_get_boot_info()->lcd_current_task = (struct task_struct*)kmalloc(sizeof(struct task_struct), GFP_KERNEL);

	if (!lcd_get_boot_info()->lcd_current_task) {
		LIBLCD_ERR("allocating current_task failed");
	}
	/*
	 * Initialize libcap
	 */
	ret = cap_init();
	if (ret) {
		LIBLCD_ERR("failed to init libcap");
		goto fail;
	}
	LIBLCD_MSG("libcap initialized");
	/*
	 * Initialize libfipc
	 */
	ret = fipc_init();
	if (ret) {
		LIBLCD_ERR("failed to init libfipc");
		goto fail;
	}
	/*
	 * Set up async runtime
	 */
	ret = thc_global_init();
	if (ret) {
		LIBLCD_ERR("failed to init libasync");
		goto fail;
	}
	thc_init();
	thc_initialized = 1;
	LIBLCD_MSG("async runtime initialized");

	lcd_printk("===============");
	lcd_printk("  LCD BOOTED   ");
	lcd_printk("===============");

	++lcds_entered;

	return 0;

fail:
	return ret;
}

void 
LIBLCD_FUNC_ATTR
__noreturn lcd_exit(int retval)
{
	lcd_printk("=================");
	lcd_printk("LCD SHUTTING DOWN");
	lcd_printk("=================");

	/*
	 * For now, just tear down async so we can make sure
	 * it all worked.
	 */
	if (thc_initialized)
		thc_done();

	LIBLCD_MSG("exiting");

	lcd_syscall_exit(retval); /* doesn't return */
}

void __noreturn lcd_abort(void)
{
	lcd_syscall_exit(-EIO); /* doesn't return */
}

