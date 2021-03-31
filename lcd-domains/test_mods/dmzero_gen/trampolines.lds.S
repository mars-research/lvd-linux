#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ctr)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_map)
}

INSERT AFTER .text
