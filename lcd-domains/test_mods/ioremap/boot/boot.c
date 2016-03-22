/*
 * boot.c - non-isolated boot module
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <linux/pci.h>

/* The below vendor and device id corresponds to 
 * Broadcom Corporation NetXtreme II BCM5709 
 * Gigabit Ethernet controller */
unsigned int vendor = 0x14e4;
unsigned int device = 0x1639;

static int boot_main(void)
{
	int ret;
	cptr_t lcd;
	struct lcd_create_ctx *ctx;
	struct pci_dev *dev = NULL;
	unsigned long bar = 0;
	cptr_t dev_mem;
	cptr_t dest_devmem;

	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}


	dev = pci_get_device(vendor, device, NULL);
	if(!dev) {
		LIBLCD_ERR("cannot obtain pci_dev! \n");
		goto fail2;
	}

	bar = pci_resource_start(dev, 0);
	printk("bar address - %lx \n", bar);		

	printk("volunteer dev mem \n");
	ret = lcd_volunteer_dev_mem(__gpa(bar), 0, &dev_mem);
	if(ret) {
		LIBLCD_ERR("cannot insert dev_mem capability into lcd");
		goto fail2;
	}

	printk("volunteer dev mem done \n");
	/*
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(LCD_DIR("ioremap/lcd"),
				"lcd_test_mod_ioremap_lcd",
				&lcd, 
				&ctx);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail3;
	}
	
	printk("cptr_alloc \n");
        /*
         * Allocate a cptr for the lcd to hold the endpoint
         */
        ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), &dest_devmem);
        if (ret) {
                LIBLCD_ERR("failed to alloc dest slot");
                goto fail4;
        }
	printk("cptr_alloc ok \n");
       
	printk("cap grant \n");
	/*
         * Grant access to endpoint
         */
        ret = lcd_cap_grant(lcd, dev_mem, dest_devmem);
        if (ret) {
                LIBLCD_ERR("failed to grant endpoint to lcd");
                goto fail5;
        }

	printk("cap grant done \n");
       
	/*
         * Store cptr in boot area
         */
        lcd_to_boot_info(ctx)->cptrs[0] = dest_devmem;


	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail3;
	}
	/*
	 * Hang out for four seconds
	 */
	msleep(4000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail5:
fail4:
fail3:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx);
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
