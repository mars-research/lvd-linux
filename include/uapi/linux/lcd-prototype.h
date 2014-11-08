#ifndef LCD_PROTOTYPE_H
#define LCD_PROTOTYPE_H

/*
 * lcd-prototype.h - public header for LCD Prototype support
 */

#include <linux/types.h>

/* Fixme: Needs to jive with numbers in include/linux/miscdevice.h */
#define LCD_PROTOTYPE_MINOR 254

struct lcd_prototype_module_args {
	/* syscall arguments to init_module */
	void *module_image;
	unsigned long len;
	const char *param_values;
} __attribute__((packed));

#define LCD_PROTOTYPE_INIT_MODULE _IOR(LCD_PROTOTYPE_MINOR, 0x01, struct lcd_prototype_module_args)

#endif /* LCD_PROTOTYPE_H */
