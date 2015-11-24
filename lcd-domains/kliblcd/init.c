/**
 * init.c
 *
 * These are invoked by the microkernel when it boots,
 * so that individual parts can initialize.
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <lcd-domains/kliblcd.h>

int __kliblcd_init(void)
{
	/* Initialize data store code */
	lcd_dstore_init();

	return 0;
}

void __kliblcd_exit(void)
{
	/* Tear down data store code */
	lcd_dstore_exit();

	return;
}
