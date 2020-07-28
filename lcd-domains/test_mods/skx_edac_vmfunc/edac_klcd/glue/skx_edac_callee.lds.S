SECTIONS
{
#include <liblcd/trampoline.h>
	LCD_TRAMPOLINE_LINKER_SECTION(get_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(set_trampoline)
}
INSERT AFTER .text;
