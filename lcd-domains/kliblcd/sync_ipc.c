/*
 * sync_ipc.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd-domains/liblcd.h>

struct lcd_utcb *lcd_get_utcb(void)
{
	return current->lcd->utcb;
}

int _lcd_create_sync_endpoint(cptr_t slot)
{
	return __lcd_create_sync_endpoint(current->lcd, slot);
}

int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_cptr_alloc(slot_out);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Get endpoint
	 */
	ret = _lcd_create_sync_endpoint(*slot_out);
	if (ret) {
		LIBLCD_ERR("sync endpoint create failed");
		goto fail2;
	}

	return 0;

fail2:
	lcd_cptr_free(*slot_out);
fail1:
	return ret;
}

int lcd_sync_send(cptr_t endpoint)
{
	return __lcd_send(current->lcd, endpoint);
}

int lcd_sync_recv(cptr_t endpoint)
{
	return __lcd_recv(current->lcd, endpoint);
}

int lcd_sync_call(cptr_t endpoint)
{
	return __lcd_call(current->lcd, endpoint);
}

int lcd_sync_reply(void)
{
	return __lcd_reply(current->lcd, endpoint);
}

