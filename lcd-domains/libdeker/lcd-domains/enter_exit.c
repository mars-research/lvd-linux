/*
 * enter_exit.c
 *
 * LCD boot redefines
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/enter_exit.h>

#include <lcd_config/post_hook.h>

int 
LIBDEKER_FUNC_ATTR
lcd_enter(void)
{
	/* No-op */
	return 0;
}

void 
LIBDEKER_FUNC_ATTR
__noreturn lcd_exit(int ret)
{
	/* No-op */
	for (;;)
		;
}

