/**
 * enterexit.c
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include "../internal.h"

int klcd_enter(void)
{
	int ret;
	/*
	 * Set up cptr cache
	 */
	ret = lcd_init_cptr();
	if (ret) {
		LCD_ERR("cptr cache init");
		goto fail1;
	}
	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail2;
	}

	return 0;
fail2:
	lcd_destroy_cptr();
fail1:
	return ret;
}

void klcd_exit(int retval)
{
	/*
	 * Return value is ignored for now
	 */

	/*
	 * Exit from lcd mode
	 */
	__klcd_exit();
	/*
	 * Destroy cptr cache
	 */
	lcd_destroy_cptr();
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_enter);
EXPORT_SYMBOL(klcd_exit);
