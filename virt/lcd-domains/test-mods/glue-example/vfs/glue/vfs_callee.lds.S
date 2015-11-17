/* COMPILER: These are generated for every trampoline in the callee
 * code. */

/* NOTE: The Linux build system is gonna put a bunch of crap and warnings
 * at the top of the auto-generated vfs_callee.lds. Just ignore it really. */

#include <lcd-domains/trampoline_link.h>
 
SECTIONS
{
	LCD_TRAMPOLINE_LINKER_SECTION(new_file_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(rm_file_trampoline)
}
INSERT AFTER .text;
