#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/platform_device.h>
#include <linux/pci.h>
#include <linux/cpu.h>
#include <linux/hwmon.h>
#include <asm/cpu_device_id.h>

#define INIT_FIPC_MSG(msg)              memset(msg, 0x0, sizeof(*msg))

int __module_lcd_init(void);
void __module_lcd_exit(void);
 
struct dev_attribute_group {
	struct device_attribute	**dev_attrs;
};

struct dev_attribute_group2 {
	struct device_attribute	*dev_attrs[5];
	struct attribute	*attrs[5];
};
#endif
