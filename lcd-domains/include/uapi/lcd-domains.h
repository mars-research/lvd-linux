#ifndef LCD_DOMAINS_H
#define LCD_DOMAINS_H

/*
 * lcd-domains.h - public header for LCD support
 */

#include <linux/types.h>

/*
 * IOCTL interface
 */

/* FIXME: this must be reserved in miscdevice.h */
#define LCD_MINOR       239

struct lcd_init_module_args {
	/* syscall arguments to init_module */
	void *module_image;
	unsigned long len;
	const char *param_values;
} __attribute__((packed));

#define LCD_INIT_MODULE          _IOR(LCD_MINOR, 0x01, struct lcd_init_module_args)

#endif /* LCD_DOMAINS_H */
