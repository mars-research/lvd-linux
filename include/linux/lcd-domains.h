/*
 * lcd.h - public header for LCD support
 */

#include <linux/types.h>

/*
 * IOCTL interface
 */

/* FIXME: this must be reserved in miscdevice.h */
#define LCD_MINOR       234

#define LCD_LOAD_PV_KERNEL	 _IOR(LCD_MINOR, 0x01, struct lcd_pv_kernel_config)

struct lcd_pv_kernel_config {
	char  file[PATH_MAX];
	__u64 length;
} __attribute__((packed));

