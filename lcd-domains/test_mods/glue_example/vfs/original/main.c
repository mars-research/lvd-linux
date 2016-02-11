/**
 * main.c - implementation of fake vfs
 */

#ifdef CONFIG_ISOLATE_GLUE_EXAMPLE
#include <lcd-domains/liblcd-config.h>
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "../../include/vfs.h"

#ifdef CONFIG_ISOLATE_GLUE_EXAMPLE
#include <lcd-domains/liblcd-hacks.h>
#endif

static struct fs *minix_fs = NULL;

void unregister_fs(struct fs *fs) {
	if (fs->id == 15 && fs->size == 511) {

		minix_fs = NULL;
		return;

	} else {

		printk(KERN_ERR "fake vfs: unregister fs with unexpected fs:\n");
		printk(KERN_ERR "  got id = %d and size = %d\n",
			fs->id, fs->size);
		return;

	}
}

int register_fs(struct fs *fs) {
	if (fs->id == 15 && fs->size == 512) {

		minix_fs = fs;
		minix_fs->size = 511;
		return 0;

	} else {

		printk(KERN_ERR "fake vfs: register fs with unexpected fs:\n");
		printk(KERN_ERR "  got id = %d and size = %d\n",
			fs->id, fs->size);
		return -EINVAL;

	}
}

int 
#ifndef CONFIG_ISOLATE_GLUE_EXAMPLE
__init 
#endif
original_vfs_lcd_init(void) {
	/*
	 * Do nothing.
	 */
	return 0;
}

void 
#ifndef CONFIG_ISOLATE_GLUE_EXAMPLE
__exit 
#endif
original_vfs_lcd_exit(void) {
	/*
	 * Do nothing.
	 */
	return;
}

/* There can only be one pair of init/exit. The glue will define an
 * init/exit when we isolate. */
#ifndef CONFIG_ISOLATE_GLUE_EXAMPLE
module_init(original_vfs_lcd_init);
module_exit(original_vfs_lcd_exit);
#endif

/* HACK -------------------------------------------------- */

void do_stuff(void)
{
	struct file *f;
	int ret;

	/* Normally we would have another thread invoke/exercise the
	 * registered functions from minix. The dispatch loop just calls
	 * this code instead right after the register. */

	if (!minix_fs) {
		LIBLCD_ERR("null minix fs");
		return;
	}

	ret = minix_fs->fs_operations->new_file(179, &f);
	if (ret) {
		LIBLCD_ERR("non-zero ret = %d", ret);
		return;
	}

	minix_fs->fs_operations->rm_file(f);

	return;
}

/* EXPORTS ---------------------------------------- */

EXPORT_SYMBOL(register_fs);
EXPORT_SYMBOL(unregister_fs);
