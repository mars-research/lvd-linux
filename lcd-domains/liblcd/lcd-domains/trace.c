/*
 * trace.c
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/trace.h>
#include <asm/lcd_domains/liblcd.h>

#include <lcd_config/post_hook.h>

void lcd_dump_stack(void)
{
	lcd_syscall_dump_stack();
}
