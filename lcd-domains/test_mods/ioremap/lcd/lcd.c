/* ioremap test - this test emulates access to 
 * device memory using normal pages. Creating a
 * test setup to test actual device memory
 * access is not impossible, but challenging 
 * with the current environment setup */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

gpa_t addr;
cptr_t page;

static int __init ioremap_test_lcd_init(void)
{
	unsigned long phys_addr;
	void *virt_addr;
	int ret = 0;
	
	ret = lcd_enter();

#if 0
	/* only a single page */
	ret = _lcd_alloc_pages(0, 0, &page);
	if(ret) {
		LIBLCD_ERR("low level alloc failed");	
		goto exit;
	}
#endif
	page = lcd_get_boot_info()->cptrs[0];
	ret = lcd_ioremap_phys(page, 4096, &addr);
	if(ret) {
	 	LIBLCD_ERR("__ioremap failed with %d", ret);
		goto ioremap_exit;
	}

	phys_addr = gpa_val(addr);
	virt_addr = lcd_ioremap(phys_addr, 4096);
	if(!virt_addr) {
		LIBLCD_ERR("va returned from ioremap is null!");
		goto ioremap_va_exit;	
	}
	
	printk("virt_addr %p \n",virt_addr);
	/*We should be able to touch address here */
	//memset(virt_addr,0xAB, 4096);
	printk("val @ virtaddr- %x \n", *((unsigned int *)(virt_addr+0x808)));
	
	lcd_exit(ret);
	return ret;

ioremap_va_exit:
	lcd_iounmap_phys(addr, 4096);

ioremap_exit:
	lcd_cap_delete(page);
//exit:
	lcd_exit(ret);
	return ret;
}

static void ioremap_test_lcd_exit(void)
{
	return;
}
module_init(ioremap_test_lcd_init);
module_exit(ioremap_test_lcd_exit);
