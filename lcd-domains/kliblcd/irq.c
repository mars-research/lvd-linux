/*
 * irq.c
 */
#include <liblcd/irq.h>

void lcd_irq_disable(void)
{
	return; /* no op */
}

void lcd_irq_enable(void)
{
	return; /* no op */
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_irq_disable);
EXPORT_SYMBOL(lcd_irq_enable);
