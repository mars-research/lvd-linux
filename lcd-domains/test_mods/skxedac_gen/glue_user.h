#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/pci.h>
#include <asm/cpu_device_id.h>
#include <asm/mce.h>
#include "skx_lcd/edac_core.h"

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

#endif
