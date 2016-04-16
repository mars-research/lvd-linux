/*
 * trace.c
 */

#include <liblcd/trace.h>
#include <linux/kernel.h>

void lcd_dump_stack(void)
{
	dump_stack();
}
