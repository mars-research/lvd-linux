SECTIONS
{
#include <liblcd/trampoline.h>
	LCD_TRAMPOLINE_LINKER_SECTION(irq_handler_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(irq_handler_tnread_trampoline)
}
INSERT AFTER .text;
