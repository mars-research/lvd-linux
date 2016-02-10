/*
 * LCD BUG() and oops test. This LCD crashes to generate a
 * kernel oops. 
 *
 * IMPORTANT: This code *does not* expect to be booted so that
 * it can use kmalloc, page alloc, etc.
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

#include <linux/jiffies.h>

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>



static int __noreturn __init test_init(void) 
{
	int r;

	register long rax asm ("rax");
	register long rbx asm ("rbx");
	register long rcx asm ("rcx");
	register long rdx asm ("rdx");
	register long rdi asm ("rdi");
	register long rsi asm ("rsi");
	register long r8 asm ("r8");
	register long r9 asm ("r9");
	register long r10 asm ("r10");
	register long r11 asm ("r11");
	register long r12 asm ("r12");
	register long r13 asm ("r13");
	register long r14 asm ("r14");
	register long r15 asm ("r15");


	r = lcd_enter();
	if (r)
		goto fail1;

	rax = 1; 
	rbx = 2;
	rcx = 3;
	rdx = 4;
	rsi = 5;
	rdi = 6;
	r8 = 7;
	r9 = 8;
	r10 = 9;
	r11 = 0xa;
	r12 = 0xb;
	r13 = 0xc;
	r14 = 0xd;
	r15 = 0xe;

	jiffies_to_clock_t(100000);

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
