#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/etherdevice.h>
#include <net/rtnetlink.h>
#include <linux/pci.h>
#include <linux/aer.h>

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

struct atomic64_t {
	long counter;
};
static const char __alx_gstrings_stats[][ETH_GSTRING_LEN] = {
	"rx_packets",
	"rx_bcast_packets",
	"rx_mcast_packets",
	"rx_pause_packets",
	"rx_ctrl_packets",
	"rx_fcs_errors",
	"rx_length_errors",
	"rx_bytes",
	"rx_runt_packets",
	"rx_fragments",
	"rx_64B_or_less_packets",
	"rx_65B_to_127B_packets",
	"rx_128B_to_255B_packets",
	"rx_256B_to_511B_packets",
	"rx_512B_to_1023B_packets",
	"rx_1024B_to_1518B_packets",
	"rx_1519B_to_mtu_packets",
	"rx_oversize_packets",
	"rx_rxf_ov_drop_packets",
	"rx_rrd_ov_drop_packets",
	"rx_align_errors",
	"rx_bcast_bytes",
	"rx_mcast_bytes",
	"rx_address_errors",
	"tx_packets",
	"tx_bcast_packets",
	"tx_mcast_packets",
	"tx_pause_packets",
	"tx_exc_defer_packets",
	"tx_ctrl_packets",
	"tx_defer_packets",
	"tx_bytes",
	"tx_64B_or_less_packets",
	"tx_65B_to_127B_packets",
	"tx_128B_to_255B_packets",
	"tx_256B_to_511B_packets",
	"tx_512B_to_1023B_packets",
	"tx_1024B_to_1518B_packets",
	"tx_1519B_to_mtu_packets",
	"tx_single_collision",
	"tx_multiple_collisions",
	"tx_late_collision",
	"tx_abort_collision",
	"tx_underrun",
	"tx_trd_eop",
	"tx_length_errors",
	"tx_trunc_packets",
	"tx_bcast_bytes",
	"tx_mcast_bytes",
	"tx_update",
};

#define __ALX_NUM_STATS ARRAY_SIZE(__alx_gstrings_stats)

#endif
