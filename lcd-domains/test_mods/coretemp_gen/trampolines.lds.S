#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_probe)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_remove)
}

INSERT AFTER .text
