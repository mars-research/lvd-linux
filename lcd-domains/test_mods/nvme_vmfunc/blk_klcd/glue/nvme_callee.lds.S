#include <liblcd/trampoline_link.h>

SECTIONS
{
	LCD_TRAMPOLINE_LINKER_SECTION(ndo_open_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(ndo_stop_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(probe_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(remove_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(sync_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(unsync_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(poll_trampoline)
}
INSERT AFTER .text;
