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
#define LCD_MINOR       234

struct lcd_pv_kernel_config {
	char  *file_name;
	unsigned int file_name_length;
	__u64 length;
} __attribute__((packed));

struct lcd_blob_info {
	unsigned char *blob;
	unsigned int blob_order;
} __attribute__((packed));

#define LCD_LOAD_PV_KERNEL	 _IOR(LCD_MINOR, 0x01, struct lcd_pv_kernel_config)
#define LCD_RUN_BLOB             _IOR(LCD_MINOR, 0x02, struct lcd_blob_info)

#endif /* LCD_DOMAINS_H */
