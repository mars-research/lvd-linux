#include <liblcd/trampoline_link.h>

SECTIONS
{
	LCD_TRAMPOLINE_LINKER_SECTION(notifier_call_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(probe_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(remove_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(show_trampoline)
}
INSERT AFTER .text;
