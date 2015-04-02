
#include <lcd-domains/liblcd-config.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	return -ENOSYS;
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
