/**
 * lcd.c - code for lcd in mm test
 */

#include <lcd-domains/liblcd-config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <lcd-domains/liblcd.h>

struct a {
	int x, y, z;
	char buff[512];
};

struct bstruct {
	int x, y, z;
	char buff[512];
};

static int kmalloc_test(void)
{
	struct a *x;
	struct kmem_cache *cache;
	int i;
	struct bstruct *b;
	int ret;
	/*
	 * Kmalloc test
	 */
	x = kmalloc(sizeof(struct a) * 10, GFP_KERNEL);
	if (!x) {
		lcd_printk("kmalloc_test: fail 1");
		ret = -1;
		goto fail1;
	}
	for (i = 0; i < 10; i++) {
		x[i].x = 2 * i + 1;
		x[i].y = 3 * i + 1;
		x[i].z = 4 * i + 1;
		memset(&x[i].buff, 0, 512);
	}
	kfree(x);
	/*
	 * Kmem cache test
	 */
	cache = KMEM_CACHE(bstruct, 0);
	if (!cache) {
		lcd_printk("kmalloc_test: fail 2");
		ret = -1;
		goto fail2;
	}
	b = kmem_cache_zalloc(cache, GFP_KERNEL);
	if (!b) {
		lcd_printk("kmalloc_test: fail 3");
		ret = -1;
		goto fail3;
	}
	kmem_cache_free(cache, b);
	kmem_cache_destroy(cache);

	lcd_printk("kmalloc_tests passed!");

	return 0;

fail3:
	kmem_cache_destroy(cache);
fail2:
fail1:
	return ret;
}

static int __noreturn __init mm_lcd_init(void) 
{
	int r = 0;
	r = lcd_enter();
	if (r)
		goto out;

	lcd_init_cptr();

	lcd_mem_init();

	r = kmalloc_test();

	goto out;

out:
	lcd_exit(r);
}

static void mm_lcd_exit(void)
{
	return;
}

module_init(mm_lcd_init);
module_exit(mm_lcd_exit);
