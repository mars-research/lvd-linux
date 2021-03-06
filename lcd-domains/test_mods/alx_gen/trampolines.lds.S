#include <liblcd/trampoline_link.h>

SECTIONS{
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_set_settings)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_settings)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_msglevel)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_set_msglevel)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_thread_fn)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_handler)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_probe)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_remove)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_link)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_pauseparam)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_set_pauseparam)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_strings)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_ethtool_stats)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_ethtool_ops_get_sset_count)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_open)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_stop)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_start_xmit)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_set_rx_mode)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_set_mac_address)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_do_ioctl)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_change_mtu)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_tx_timeout)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_get_stats64)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_fix_features)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_mdio_read)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_mdio_write)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_net_device_ops_ndo_validate_addr)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_poll)
	LCD_TRAMPOLINE_LINKER_SECTION(trmp_work_fn)
}

INSERT AFTER .text
