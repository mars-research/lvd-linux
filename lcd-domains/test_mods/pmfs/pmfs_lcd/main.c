/*
 * main.c - runs when pmfs lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "internal.h"

#include <lcd_config/post_hook.h>

/* LOOP ---------------------------------------- */


/* INIT/EXIT -------------------------------------------------- */

int init_pmfs_fs(void);
void exit_pmfs_fs(void);

struct lcd_sync_channel_group group;

static int __noreturn pmfs_lcd_init(void) 
{
	int r = 0;
	cptr_t vfs_chnl;

	r = lcd_enter();
	if (r)
		goto fail1;
	/*
	 * Initialize dispatch loop context
	 */
	lcd_sync_channel_group_init(&group);
	/*
	 * Get the vfs channel cptr from boot info
	 */
	vfs_chnl = lcd_get_boot_info()->cptrs[0];
	/*
	 * Initialize vfs glue
	 */
	r = glue_vfs_init(vfs_chnl, &group);
	if (r) {
		LIBLCD_ERR("vfs init");
		goto fail2;
	}

	/* EXECUTE REAL CODE ---------------------------------------- */

	/*
	 * Initialize pmfs
	 */
	r = init_pmfs_fs();
	if (r)
		goto fail3;


	LIBLCD_MSG("SUCCESSFULLY REGISTERED PMFS!");

	/*
	 * Tear down pmfs
	 */

	exit_pmfs_fs();

	LIBLCD_MSG("SUCCESSFULLY UNREGISTERED PMFS!");

	/* REAL CODE DONE; CLEAN UP ---------------------------------------- */

	glue_vfs_exit();

	lcd_exit(0); /* doesn't return */

fail3:
	glue_vfs_exit();
fail2:
fail1:
	lcd_exit(r);
}

static int __pmfs_lcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = pmfs_lcd_init();

		});

	return ret;
}

static void __exit pmfs_lcd_exit(void)
{
	return;
}

module_init(__pmfs_lcd_init);
module_exit(pmfs_lcd_exit);
MODULE_LICENSE("GPL");
