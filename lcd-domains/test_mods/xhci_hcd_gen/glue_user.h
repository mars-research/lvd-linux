#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/etherdevice.h>
#include <net/rtnetlink.h>
#include <linux/pci.h>
#include <linux/dmi.h>
#include <linux/aer.h>


#include "xhci.h"

void usb_disable_xhci_ports(struct pci_dev *xhci_pdev);

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

#ifndef LCD_ISOLATE
#define lvd_setup_timer		setup_timer
#define lvd_init_completion	init_completion
#endif

struct atomic64_t {
	long counter;
};

#endif
