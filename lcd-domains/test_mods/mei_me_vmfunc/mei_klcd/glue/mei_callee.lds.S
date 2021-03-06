#include <liblcd/trampoline_link.h>

SECTIONS
{
	LCD_TRAMPOLINE_LINKER_SECTION(host_is_ready_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hw_is_ready_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hw_reset_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hw_start_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hw_config_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(fw_status_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(pg_state_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(pg_in_transition_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(pg_is_enabled_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(intr_clear_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(intr_enable_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(intr_disable_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hbuf_free_slots_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hbuf_is_ready_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(hbuf_max_len_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(write_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(rdbuf_full_slots_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(read_hdr_trampoline)
	LCD_TRAMPOLINE_LINKER_SECTION(read_trampoline)
}
INSERT AFTER .text;
