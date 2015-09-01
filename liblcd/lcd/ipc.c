
#include <lcd-domains/liblcd-config.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret) {
		LIBLCD_ERR("cptr alloc");
		goto fail1;
	}
	/*
	 * Get new endpoint
	 */
	lcd_set_cr0(*slot_out);
	ret = LCD_DO_SYSCALL(LCD_SYSCALL_SYNC_EP);
	if (ret) {
		LIBLCD_ERR("create sync ep syscall");
		goto fail2;
	}

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int lcd_send(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_SEND);
}

int lcd_recv(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_RECV);
}

int lcd_call(cptr_t endpoint)
{
	lcd_set_cr0(endpoint);
	return LCD_DO_SYSCALL(LCD_SYSCALL_CALL);
}

int lcd_reply(void)
{
	return LCD_DO_SYSCALL(LCD_SYSCALL_REPLY);
}
