/**
 * ipc.c
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include "../microkernel/internal.h"

/* KLCD REGISTER ACCESS -------------------------------------------------- */

#define __KLCD_MK_REG_ACCESS(idx)					\
u64 __klcd_r##idx(struct lcd *lcd)				        \
{									\
        return __lcd_r##idx(lcd->utcb);					\
}									\
void __klcd_set_r##idx(struct lcd *lcd, u64 val)			\
{									\
	__lcd_set_r##idx(lcd->utcb, val);				\
}									\
cptr_t __klcd_cr##idx(struct lcd *lcd)					\
{									\
        return __lcd_cr##idx(lcd->utcb);				\
}								        \
void __klcd_set_cr##idx(struct lcd *lcd, cptr_t val)			\
{									\
	__lcd_set_cr##idx(lcd->utcb, val);				\
}									\
EXPORT_SYMBOL(__klcd_r##idx);						\
EXPORT_SYMBOL(__klcd_set_r##idx);					\
EXPORT_SYMBOL(__klcd_cr##idx);						\
EXPORT_SYMBOL(__klcd_set_cr##idx);

__KLCD_MK_REG_ACCESS(0)
__KLCD_MK_REG_ACCESS(1)
__KLCD_MK_REG_ACCESS(2)
__KLCD_MK_REG_ACCESS(3)
__KLCD_MK_REG_ACCESS(4)
__KLCD_MK_REG_ACCESS(5)
__KLCD_MK_REG_ACCESS(6)
__KLCD_MK_REG_ACCESS(7)

/* UTCB -------------------------------------------------- */

struct lcd_utcb *klcd_get_utcb(void)
{
	return current->lcd->utcb;
}

/* IPC -------------------------------------------------- */

int klcd_create_sync_endpoint(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret) {
		LCD_ERR("cptr alloc");
		goto fail1;
	}
	/*
	 * Get new endpoint
	 */
	ret = __lcd_create_sync_endpoint(current->lcd, *slot_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int klcd_send(cptr_t endpoint)
{
	return __lcd_send(current->lcd, endpoint);
}

int klcd_recv(cptr_t endpoint)
{
	return __lcd_recv(current->lcd, endpoint);
}

int klcd_call(cptr_t endpoint)
{
	return __lcd_call(current->lcd, endpoint);
}

int klcd_reply(void)
{
	return __lcd_reply(current->lcd);
}

/* EXPORTS -------------------------------------------------- */

/* (Register access exports are at the top.) */

EXPORT_SYMBOL(klcd_create_sync_endpoint);
EXPORT_SYMBOL(klcd_send);
EXPORT_SYMBOL(klcd_recv);
EXPORT_SYMBOL(klcd_call);
EXPORT_SYMBOL(klcd_reply);
EXPORT_SYMBOL(klcd_get_utcb);
