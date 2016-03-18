/*
 * sync_ipc.c
 *
 * Synchronous IPC redefines
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/sync_ipc.h>

#include <lcd_config/post_hook.h>

int 
LIBDEKER_FUNC_ATTR
lcd_sync_send(cptr_t c)
{
	return 12345;
}
