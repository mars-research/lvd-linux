#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_setup)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_get_drvinfo)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_init)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_uninit)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_start_xmit)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_set_rx_mode)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_get_stats64)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ndo_change_carrier)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_validate)
}

INSERT AFTER .text
