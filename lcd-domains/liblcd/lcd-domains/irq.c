/*
 * irq.c
 */

#include <lcd_config/pre_hook.h>

#include <asm/lcd_domains/liblcd.h>
#include <liblcd/irq.h>

#include <lcd_config/post_hook.h>

void lcd_irq_disable(void)
{
	/* Disable interrupts inside the VM */
	__asm__ ("cli");

	/* Configure LCD so that external interrupts do not cause vm exit */
	lcd_syscall_irq_disable();
}


void lcd_irq_enable(void)
{
	/* Disable interrupts inside the VM */
	__asm__ ("sti");

	/* Configure LCD so that external interrupts *do* cause a vm exit */
	lcd_syscall_irq_enable();
}
