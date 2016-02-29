/*
 * sync_ipc.c
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/sync_ipc.h>
#include <asm/lcd_domains/liblcd.h>
#include <liblcd/cap.h>

#include <lcd_config/post_hook.h>

struct lcd_utcb *
lcd_get_utcb(void)
{
	return (struct lcd_utcb *)gva_val(LCD_UTCB_GV_ADDR);
}

int _lcd_create_sync_endpoint(cptr_t slot)
{
	return lcd_syscall_create_sync_ep(slot);
}

int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_cptr_alloc(slot_out);
	if (ret) {
		LIBLCD_ERR("cptr alloc");
		goto fail1;
	}
	/*
	 * Get new endpoint
	 */
	ret = _lcd_create_sync_endpoint(*slot_out);
	if (ret) {
		LIBLCD_ERR("create sync endpoint");
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
	return lcd_syscall_sync_send(endpoint);
}

int lcd_sync_recv(cptr_t endpoint)
{
	return lcd_syscall_sync_recv(endpoint);
}

int lcd_sync_call(cptr_t endpoint)
{
	return lcd_syscall_sync_call(endpoint);
}

int lcd_sync_reply(void)
{
	return lcd_syscall_sync_reply();
}
