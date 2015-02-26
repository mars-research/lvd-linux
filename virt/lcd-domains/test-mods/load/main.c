#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/lcd-domains/liblcd.h>

#include "../../../../arch/x86/lcd-domains/liblcd.c"

/*
 * IMPORTANT: This module should be compiled without
 * optimizations (see Makefile in this directory).
 * Otherwise, the loop in foo will be optimized away and
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

static int __noreturn __init test_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto fail1;

	r = foo(10); /* never returns */


fail1:
	lcd_exit(r);
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
