/*
 * lcd.h - interface to LCD domains
 *
 * Author: Anton Burtsev 
 */

#ifndef __LINUX_PUBLIC_LCD_H__
#define __LINUX_PUBLIC_LCD_H__
/*
 * IOCTL interface
 */

/* FIXME: this must be reserved in miscdevice.h */
#define LCD_MINOR       234

#define LCD_LOAD_PV_KERNEL	 _IOR(LCD_MINOR, 0x01, struct lcd_pv_kernel_config)

struct lcd_pv_kernel_config {
	char  *file;
} __attribute__((packed));

#endif /* __LINUX_PUBLIC_LCD_H__ */
