#ifndef LCD_DOMAINS_H
#define LCD_DOMAINS_H

/*
 * lcd.h - interface to LCD domains
 *
 * Author: Anton Burtsev 
 */

/*
 * IOCTL interface
 */

/* FIXME: this must be reserved in miscdevice.h */
#define LCD_MINOR       234

#define LCD_LOAD_PV_KERNEL	 _IOR(LCD_MINOR, 0x01, struct lcd_pv_kernel_config)

struct lcd_pv_kernel_config {
	char  *file;
} __attribute__((packed));

#endif /* LCD_DOMAINS_H */
