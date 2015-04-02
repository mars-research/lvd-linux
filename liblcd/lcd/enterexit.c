
#include <lcd-domains/liblcd-config.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

int lcd_enter(void)
{
	/*
	 * For now, we don't do anything.
	 */
	return 0;
}

void __noreturn lcd_exit(int retval)
{
	lcd_set_r0(retval);
	for(;;)
		LCD_DO_SYSCALL(LCD_SYSCALL_EXIT); /* doesn't return */
}
