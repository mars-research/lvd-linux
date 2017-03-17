#include <liblcd/trampoline_link.h>

SECTIONS
{
	LCD_TRAMPOLINE_LINKER_SECTION(probe_trampoline)
}
INSERT AFTER .text;
