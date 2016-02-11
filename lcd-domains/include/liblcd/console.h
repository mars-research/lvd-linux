/*
 * console.h
 *
 * Primitive printk for LCDs.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_CONSOLE_H
#define LCD_DOMAINS_CONSOLE_H

/**
 * lcd_printk -- Print a message to the host kernel log
 * @fmt: format string
 *
 * Note that the entire buffer you print is limited to
 * LCD_PRINTK_BUFF_SIZE; the rest is truncated.
 *
 * Most of the standard format escapes are supported, but
 * not the more fancy ones.
 */
void lcd_printk(char *fmt, ...);

/**
 * Some helpful macros for printing file and line numbers.
 */
#define LIBLCD_ERR(msg...) do {						\
		lcd_printk("error @ %s:%d: ", __FILE__, __LINE__);	\
		lcd_printk(msg);					\
		lcd_printk("end of error");				\
	} while (0)
#define LIBLCD_MSG(msg...) do {						\
		lcd_printk("msg @ %s:%d: ", __FILE__, __LINE__);	\
		lcd_printk(msg);					\
		lcd_printk("end of msg");				\
	} while (0)
#define LIBLCD_WARN(msg...) do {					\
		lcd_printk("warn @ %s:%d: ", __FILE__, __LINE__);	\
		lcd_printk(msg);					\
		lcd_printk("end of warn");				\
	} while (0)

#endif /* LCD_DOMAINS_CONSOLE_H */
