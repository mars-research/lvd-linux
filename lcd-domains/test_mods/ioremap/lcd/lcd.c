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

static int ioremap_test_lcd_init(void)
{
	unsigned long phys_addr;
	void *virt_addr;

	
	/* only a single page */
	ret = _lcd_alloc_pages(0, 0, &page);
	if(ret) {
		LIBLCD_ERR("low level alloc failed");	
		goto exit;
	}

	ret = _lcd_ioremap(page, 4096, &addr);
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
	memset(virt_addr,0xAB, 4096);
	printk("val @ virtaddr- %x \n", *(virt_addr+40));
	

ioremap_va_exit:
	_lcd_iounmap(addr, 4096);

ioremap_exit:
	lcd_cap_delete(page);
exit:
	return ret;
}

static void ioremap_test_lcd_exit(void)
{
	lcd_cap_delete(page);
	_lcd_iounmap(addr, 4096);
}
module_init(ioremap_test_lcd_init);
module_exit(ioremap_test_lcd_exit);
