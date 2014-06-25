/**
 * lcd-domains.c - Main file for the LCD module
 *
 *
 * Authors:
 *   Anton Burtsev   <aburtsev@flux.utah.edu>
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include <linux/lcd-domains.h>
#include <asm/lcd-domains-arch.h>
#include <lcd-domains/lcd-domains.h>
#include <lcd-domains/syscall.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* run blob -------------------------------------------------- */

static int lcd_do_run_blob_once(struct lcd *lcd)
{
	int r;
	int syscall_id;

	r = lcd_arch_run(lcd->lcd_arch);
	if (r < 0) {
		printk(KERN_ERR "lcd_do_run_blob_once: error running blob\n");
		goto out;
	}

	switch(r) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		/*
		 * Paging shouldn't be needed for blob (blob shouldn't
		 * use instructions that access memory, for now)
		 */
		printk(KERN_ERR "lcd_run_blob: page fault\n");
		r = -EIO;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		printk(KERN_ERR "lcd_run_blob: got external intr\n");
		r = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		/*
		 * EPT should everything mapped the blob needs, so
		 * there's a problem. Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: ept fault\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		/*
		 * %cr3 shouldn't be accessed for simple blobs (for
		 * now). Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: cr3 access\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		/*
		 * Only allow yield syscalls for now
		 */
		syscall_id = LCD_ARCH_GET_SYSCALL_NUM(lcd->lcd_arch);
		printk(KERN_ERR "lcd_run_blob: handling syscall %d\n",
			syscall_id);
		if (syscall_id != LCD_SYSCALL_YIELD) {
			printk(KERN_ERR "lcd_run_blob: unexpected syscall id %d\n",
				syscall_id);
			r = -EIO;
			goto out;
		} else {
			printk(KERN_ERR "lcd_run_blob: lcd yielded, exiting lcd...\n");
			r = 0;
			goto out;
		}
	}

out:
	return r;
}

static int lcd_do_run_blob(struct lcd *lcd)
{
	int r;
	while (1) {
		r = lcd_do_run_blob_once(lcd);
		if (r)
			return r;
	}
}

static int lcd_load_blob(struct lcd *lcd, unsigned char *blob,
	unsigned int blob_order)
{
	u64 blob_hpa_base;
	u64 blob_gpa_base;
	u64 off;
	unsigned int blob_size;
	/*
	 * Map blob in lcd, starting at LCD_ARCH_FREE
	 */
	blob_size = 1 << blob_order;
	blob_hpa_base = __pa((u64)blob);
	blob_gpa_base = LCD_ARCH_FREE;
	for (off = 0; off < blob_size; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd->lcd_arch,
							/* gpa */
							blob_gpa_base + off,
							/* hpa */
							blob_hpa_base + off,
							/* create */
							1,
							/* no overwrite */
							0)) {
			printk(KERN_ERR "lcd_load_blob: error mapping blob at offset %lx\n",
				(unsigned long)off);
			return -EIO;
		}
	}

	/*
	 * Set program counter to start of free mem
	 */
	if (lcd_arch_set_pc(lcd->lcd_arch, LCD_ARCH_FREE)) {
		printk(KERN_ERR "lcd_load_blob: error setting prgm ctr.\n");
		return -EIO;
	}

	return 0;
}

static int lcd_run_blob(struct lcd_blob_info *bi)
{
	struct lcd *lcd;
	int r;
	unsigned char *blob;

	return 0;
	/*
	 * Sanity check blob order
	 */
	if (bi->blob_order > 4) {
		printk(KERN_ERR "lcd_run_blob: blob is bigger than 16 pgs\n");
		r = -EINVAL;
		goto fail1;
	}
	
	/*
	 * Load blob mem
	 */
	blob = (unsigned char *)__get_free_pages(GFP_KERNEL, bi->blob_order);
	if (!blob) {
		printk(KERN_ERR "lcd_run_blob: couldn't alloc mem for blob\n");
		r = -ENOMEM;
		goto fail2;
	}

	/*
	 * Copy blob
	 */
	r = copy_from_user(blob, (void __user *)bi->blob, 
			(1 << bi->blob_order));
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error copying blob\n");
		goto fail3;
	}

	/*
	 * Alloc and init lcd
	 */
	lcd = (struct lcd *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd_run_blob: error alloc'ing lcd\n");
		r = -ENOMEM;
		goto fail4;
	}
	lcd->lcd_arch = lcd_arch_create();
	if(!lcd->lcd_arch) {
		printk(KERN_ERR "lcd_run_blob: error alloc'ing lcd_arch\n");
		r = -ENOMEM;
		goto fail5;
	}

	/*
	 * Load blob in lcd
	 */
	r = lcd_load_blob(lcd, blob, bi->blob_order);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error loading blob in lcd\n");
		r = -EIO;
		goto fail6;
	}

	/*
	 * Run blob until it yields or fails
	 */
	r = lcd_do_run_blob(lcd);
	goto done;

done:
fail6:
	lcd_arch_destroy(lcd->lcd_arch);
fail5:
	kfree(lcd);
fail4:
fail3:
	free_pages((u64)blob, bi->blob_order);
fail2:
fail1:
	return r;
}

/* ioctl -------------------------------------------------- */

static long lcd_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;
	struct lcd_pv_kernel_config conf;
	struct lcd_blob_info bi;

	switch (ioctl) {
	case LCD_LOAD_PV_KERNEL:
		r = copy_from_user(&conf, (int __user *) arg,
				sizeof(struct lcd_pv_kernel_config));
		if (r) {
			r = -EIO;
			goto out;
		}
		/* create LCD with a PV Linux kernel */
		goto out;
		break;
	case LCD_RUN_BLOB:
		r = copy_from_user(&bi, (void __user *)arg, sizeof(bi));
		if (r) {
			printk(KERN_ERR "lcd: error loading blob info\n");
			r = -EIO;
			goto out;
		}
		r = lcd_run_blob(&bi);
		printk(KERN_ERR "lcd: addr=%lx, order=%d\n",
			(unsigned long)bi.blob, bi.blob_order);
		if (r) {
			printk(KERN_ERR "lcd: error running blob\n");
			goto out;
		}
		r = 0;
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

static int __init lcd_init(void)
{
	int r;

	printk(KERN_ERR "LCD module loaded\n");

	if ((r = lcd_arch_init())) {
		printk(KERN_ERR "lcd: failed to initialize vmx\n");
		return r;
	}

	r = misc_register(&lcd_dev);
	if (r) {
		printk(KERN_ERR "lcd: misc device register failed\n");
		
	}

	return r;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	lcd_arch_exit();
}

module_init(lcd_init);
module_exit(lcd_exit);
