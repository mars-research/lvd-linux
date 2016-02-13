/*
 * boot_info.c
 *
 * Isolated LCD bootstrap information
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/address_spaces.h>
#include <liblcd/boot_info.h>

#include <lcd_config/post_hook.h>

struct lcd_boot_info * lcd_get_boot_info(void)
{
	return (struct lcd_boot_info *)gva_val(LCD_BOOTSTRAP_PAGES_GV_ADDR);
}
