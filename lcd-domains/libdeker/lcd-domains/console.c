/*
 * console.c
 *
 * printk redefines
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/console.h>

#include <lcd_config/post_hook.h>

void 
LIBDEKER_FUNC_ATTR
lcd_printk(const char *fmt, ...)
{
	return; /* no op */
}
