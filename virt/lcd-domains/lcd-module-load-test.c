#include <linux/module.h>
#include <linux/kernel.h>

/*
 * Force no inline, so we get a non-trivial jump. Use
 * a hack to turn on no inline.
 */
#ifdef noinline
#define lcd_noinline_set
#define lcd_old_noinline noinline
#endif
#undef noinline

static __attribute__ ((noinline)) int foo(int x)
{
	return x + 5;
}

#ifdef lcd_noinline_set
#define noinline lcd_old_noinline
#endif

static void lcd_yield(void)
{
	asm volatile("mov $6, %rax \n\t"
		"vmcall");
	/* shouldn't return */
}

static int __init test_init(void)
{
	int r;
	r = foo(10);

	lcd_yield();

	/* make compiler happy */
	/* (if we actually return, will probably cause ept fault, as we will
	 * jump to a potentially random place)
	 */
	return 0;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit test_exit(void)
{
	return;
}

module_init(test_init);
module_exit(test_exit);
