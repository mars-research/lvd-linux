/**
 * enterexit.c
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include "../microkernel/internal.h"

int klcd_enter(void)
{
	int ret;
	/*
	 * This sets up the runtime environment for non-isolated
	 * threads.
	 *
	 * First, if we weren't created by another klcd, then
	 * we should do __klcd_enter to set up our cspace, utcb,
	 * and so on.
	 */
	if (!current->lcd) {
		ret = __klcd_enter();
		if (ret) {
			LCD_ERR("enter");
			goto fail1;
		}
	}
	/*
	 * Set up our cptr cache
	 */
	ret = lcd_init_cptr();
	if (ret) {
		LCD_ERR("cptr cache init");
		goto fail2;
	}
	/*
	 * Create our call endpoint (for receing rpc replies)
	 */
	ret = __lcd_create_sync_endpoint(current->lcd, LCD_CPTR_CALL_ENDPOINT);
        if (ret) {
                LIBLCD_ERR("creating call endpoint");
                goto fail3;
        }

	return 0;

fail3:
fail2:
fail1:
	/* This will do teardown */
	klcd_exit(ret);
	return ret;
}

void klcd_exit(int retval)
{
	/*
	 * Return value is ignored for now
	 */

	if (current->lcd) {
		/*
		 * Exit from lcd mode
		 */
		__klcd_exit();
	}
	if (current->cptr_cache) {
		/*
		 * Destroy cptr cache
		 */
		lcd_destroy_cptr();
	}

	/* (Call endpoint should be auto-destroyed when we do
	 * __klcd_exit and destroy the cspace.) */
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_enter);
EXPORT_SYMBOL(klcd_exit);
