#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/aer.h>
#include <linux/pci.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <net/ipv6.h>

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

#ifndef LCD_ISOLATE
#define lvd_setup_timer		setup_timer
#define lvd_napi_enable		napi_enable
#define lvd_netif_trans_update	netif_trans_update
#define lvd_netif_tx_disable	netif_tx_disable
#define lvd_netif_tx_wake_all_queues netif_tx_wake_all_queues
#define lvd_napi_schedule_irqoff napi_schedule_irqoff
#define get_jiffies()		({ jiffies; })
#define get_loops_per_jiffy()		({ loops_per_jiffy; })
#endif

#endif
