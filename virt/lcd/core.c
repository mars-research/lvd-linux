/**
 * lcd.c - Main file for the LCD module
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
#include <asm/uaccess.h>
#include <linux/slab.h>

#include <lcd/lcd.h>
#include <lcd/cap.h>
#include <lcd/api.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

struct lcd_api lcd_api; 

int lcd_enter(void) {
	
	current->utcb = kmalloc(sizeof(struct utcb), GFP_KERNEL); 
	if (!current->utcb) 
		return -ENOMEM;

	return lcd_init_cspace(&current->cspace);
};
EXPORT_SYMBOL(lcd_enter);

static long lcd_dev_ioctl(struct file *filp ,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case LCD_LOAD_PV_KERNEL: 
	{
		int error;
		struct lcd *lcd;
		struct filename *filename;
		struct lcd_pv_kernel_config conf;

		r = copy_from_user(&conf, (int __user *) arg,
				   sizeof(conf));
		if (r) {
			r = -EIO;
			goto out;
		}

		/* create LCD with a PV Linux kernel */
		lcd = lcd_create();
		if (!lcd) {
			r = -ENOMEM;
			goto out;		
		}

		filename = getname((const char __user *) conf.file);
		error = PTR_ERR(filename);
		if (IS_ERR(filename))
			goto out;
 
		lcd_setup_vmlinux(lcd, filename->name);

		putname(filename);

		lcd_run(lcd);
		break;
        } 
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
	int r = 0;

	r = lcd_cap_init();
	if(r)
		return r;

	r = lcd_ipc_init();
	if(r)
		return r;

	r = lcd_api_init(&lcd_api); 
	if(r)
		return r;



#if 0

	printk(KERN_ERR "LCD module loaded\n");

	if ((r = lcd_vmx_init())) {
		printk(KERN_ERR "lcd: failed to initialize vmx\n");
		return r;
	}

	r = misc_register(&lcd_dev);
	if (r) {
		printk(KERN_ERR "lcd: misc device register failed\n");
		
	}
#endif
	return r;
}

static void __exit lcd_exit(void)
{
	lcd_cap_exit();
	lcd_api_exit(&lcd_api);


#if 0
	misc_deregister(&lcd_dev);
	lcd_vmx_exit();
#endif
	return; 
}

module_init(lcd_init);
module_exit(lcd_exit);
