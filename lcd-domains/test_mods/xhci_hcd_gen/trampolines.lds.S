#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_thread_fn)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_handler)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_get_quirks)
}

INSERT AFTER .text
