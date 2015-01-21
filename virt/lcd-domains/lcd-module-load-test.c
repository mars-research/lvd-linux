#include <linux/module.h>
#include <linux/kernel.h>

/*
 * IMPORTANT: This module should be compiled without
 * optimizations (see Makefile in this directory).
 * Otherwise, some things will be optimized away and
 * we won't fully test the code.
 *
 * You may want to inspect the .ko before running with
 * objdump -d.
 *
 * NOTE: If you disassemble the .ko, you will notice
 * callq's that are not patched to jump to the correct
 * place (e.g., they just jump to the next instruction). 
 * This is not an error. The module loading code
 * patches these calls so that they work when the module
 * is loaded. You can look at the relocation info in the
 * .ko elf file with readelf -a.
 */

static int foo(int x)
{
	while (1)
		x = 2;

	return x + 5;
}

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
