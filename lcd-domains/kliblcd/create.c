/*
 * create.c - Basic code for creating an LCD.
 *
 * Copyright: University of Utah
 */

#include <liblcd.h>
#include <lcd-domains/liblcd.h>
#include "../microkernel/internal.h"

int lcd_create(cptr_t *lcd)
{
	cptr_t slot;
	int ret;
	/*
	 * Alloc slot for new object
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Make LCD
	 */
	ret = __lcd_create(current->lcd, slot);
	if (ret) {
		LIBLCD_ERR("lcd create failed");
		goto fail2;
	}

	*lcd = slot;

	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int lcd_create_klcd(cptr_t *klcd)
{
	cptr_t slot;
	int ret;
	/*
	 * Alloc slot for new object
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Make kLCD
	 */
	ret = __lcd_create_klcd(current->lcd, slot);
	if (ret) {
		LIBLCD_ERR("klcd create failed");
		goto fail2;
	}

	*lcd = slot;

	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int lcd_config_registers(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
			gpa_t utcb_page)
{
	return __lcd_config(current->lcd, lcd, pc, sp, gva_root, utcb_page);
}

int lcd_memory_grant_and_map(cptr_t lcd, cptr_t mo, cptr_t dest_slot,
			gpa_t base);
{
	return __lcd_memory_grant_and_map(current->lcd, lcd, mo,
					dest_slot, base);
}

int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return __lcd_cap_grant(current->lcd, lcd, src, dest);
}

int lcd_run(cptr_t lcd)
{
	return __lcd_run(current->lcd, lcd);
}
