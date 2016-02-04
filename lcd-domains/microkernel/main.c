/**
 * main.c - Contains module init/exit for LCD microkernel
 *
 * Authors:
 *   Anton Burtsev     <aburtsev@flux.utah.edu>
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/compat.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/sysrq.h>

#include <lcd-domains/types.h>
#include <uapi/lcd-domains.h>
#include "internal.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* IOCTL -------------------------------------------------- */

/**
 * Does insmod syscall on behalf of user code call to ioctl.
 */
static int lcd_init_module(void __user *_mi)
{
	int r;
	struct lcd_init_module_args mi;
	
	/*
	 * Grab info
	 */
	r = copy_from_user(&mi, _mi, sizeof(mi));
	if (r) {
		LCD_ERR("copy_from_user");
		return r;
	}

	/*
	 * Init module (for_lcd = 1)
	 */
	return do_sys_init_module(mi.module_image, mi.len, 
				mi.param_values, 1);
}

static long lcd_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case LCD_INIT_MODULE:
		r = lcd_init_module((void __user *)arg);
		goto out;
	default:
		return -ENOTTY;
	}



out:
	return r;
}

static const struct file_operations lcd_chardev_ops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= lcd_dev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= lcd_dev_ioctl,
#endif
	.llseek		= noop_llseek,
};

static struct miscdevice lcd_dev = {
	LCD_MINOR,
	"lcd",
	&lcd_chardev_ops,
};

/* SYSRQ debugging -------------------------------------------------- */

static void sysrq_handle_showlcds(int key)
{
	printk(KERN_ERR "==========BEGIN LCD SYSRQ INFO==========\n");
	printk(KERN_ERR "Is there an LCD in non-root? %s\n",
		lcd_in_non_root ? "yes" : "no");
	if (lcd_in_non_root) {
		printk(KERN_ERR "And it's running on cpu %d\n",
			lcd_on_cpu);
	}
	printk(KERN_ERR "==========END LCD SYSRQ INFO==========\n");
}

static struct sysrq_key_op sysrq_showlcds_op = {
	.handler	= sysrq_handle_showlcds,
	.help_msg	= "show-lcd-info(x)",
	.action_msg	= "Show LCD Info",
};

/* Init / Exit ---------------------------------------- */

static int __init lcd_init(void)
{
	int ret;

	/*
	 * Register sysrq handler before we do *anything*
	 */
	ret = register_sysrq_key('x', &sysrq_showlcds_op);
	if (ret) {
		LCD_ERR("failed to register sysrq key");
		goto failkey;
	}
	/*
	 * Initialize each of the subsystems
	 */
	ret = lcd_arch_init();
	if (ret) {
		LCD_ERR("failed to init arch-dependent code");
		goto fail0;
	}
	ret = cap_init();
	if (ret) {
		LCD_ERR("failed to init capability subsystem");
		goto fail1;
	}
	ret = __lcd_init_cap_types();
	if (ret) {
		LCD_ERR("failed to init capability types");
		goto fail2;
	}
	ret = __lcd_ipc_init();
	if (ret) {
		LCD_ERR("failed to init ipc subsystem");
		goto fail3;
	}
	ret = __lcd_mem_init();
	if (ret) {
		LCD_ERR("failed to init memory subsystem");
		goto fail4;
	}
	ret = __lcd_run_init();
	if (ret) {
		LCD_ERR("failed to init run loop subsystem");
		goto fail5;
	}
	ret = __lcd_console_init();
	if (ret) {
		LCD_ERR("failed to init console subsystem");
		goto fail6;
	}
	ret = misc_register(&lcd_dev);
	if (ret) {
		LCD_ERR("misc device register failed, ret = %d", ret);
		goto fail7;
	}

	LCD_MSG("lcd microkernel initialized");

	return 0;

fail7:
	__lcd_console_exit();
fail6:
	__lcd_run_exit();
fail5:
	__lcd_mem_exit();
fail4:
	__lcd_ipc_exit();
fail3:
	__lcd_exit_cap_types();
fail2:
	cap_fini();
fail1:
	lcd_arch_exit();
fail0:
	unregister_sysrq_key('x', &sysrq_showlcds_op);
failkey:
	return ret;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	__lcd_console_exit();
	__lcd_run_exit();
	__lcd_mem_exit();
	__lcd_ipc_exit();
	__lcd_exit_cap_types();
	cap_fini();
	lcd_arch_exit();
	unregister_sysrq_key('x', &sysrq_showlcds_op);

	LCD_MSG("lcd microkernel exited");
}

/* EXPORTS ---------------------------------------- */

module_init(lcd_init);
module_exit(lcd_exit);
