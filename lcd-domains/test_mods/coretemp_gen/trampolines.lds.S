#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_platform_driver_probe)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_platform_driver_remove)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_dev_attr_show)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_notifier_call)
}

INSERT AFTER .text
