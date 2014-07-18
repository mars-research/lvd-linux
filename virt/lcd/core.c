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
#include <lcd/boot.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

struct lcd_api lcd_api; 

struct lcd_api *lcd_get_api(void) {
	return &lcd_api;
};
EXPORT_SYMBOL(lcd_get_api);


int lcd_enter(void) {
	int ret; 

	current->utcb = kmalloc(sizeof(struct utcb), GFP_KERNEL); 
	if (!current->utcb) 
		return -ENOMEM;

	ret = lcd_init_list_cache(&current->cap_cache, LCD_MAX_CAPS);
	if (ret) {
		printk(KERN_ERR "Failed to initialize cap cache:%d\n", ret);
		return ret;

	}

	ret = lcd_init_cspace(&current->cspace);
	if(ret) {
		printk(KERN_ERR "Failed to init cspace:%d\n", ret); 
		return ret;
	};

	return 0;

};
EXPORT_SYMBOL(lcd_enter);

int lcd_api_connect(struct lcd_api *api) {
	capability_t reply_cap;
	capability_t api_cap;

	/* Frist: we take an existing endpoint for the API and insert it into 
	 * the connecting cspace */
	ret = lcd_alloc_cap(&current->cap_cache, &api_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		kfree(rvp);
		return -ENOSPC;
	};

        ret = lcd_cap_insert_object(&current->cspace, api_cap, api->ep, LCD_TYPE_SYNC_EP); 
	if(ret) {
		printk(KERN_ERR "Failed to insert endpoint into the cspace:%d\n", ret);
		kfree(rvp);
		return -ENOSPC;
	};

	/* Second: make a new endpoint so the connecting cspace can use call/reply
	 * primitives */
	ret = lcd_make_sync_endpoint(&current->cspace, &current->cap_cache, &reply_cap);
	if(ret) {
		printk(KERN_ERR "Failed to create API endpoint:%d\n", ret);
		return ret;
	};

	current->utcb->boot_info.boot_caps[LCD_BOOT_API_CAP] = api_cap;
	current->utcb->boot_info.boot_caps[LCD_BOOT_REPLY_CAP] = reply_cap;

	return 0;
};
EXPORT_SYMBOL(lcd_api_connect);

int lcd_init_current(void) {
	int ret; 
	struct cnode *cnode;
	
	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	ret = lcd_api_connect(lcd_get_api());
	if (ret) {
		printk(KERN_ERR "Failed to connect to LCD API\n");
		return ret;
	};

	return 0;
};
EXPORT_SYMBOL(lcd_init_current); 

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
