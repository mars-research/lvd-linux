#ifndef LCD_DOMAINS_COMMON_H
#define LCD_DOMAINS_COMMON_H

/* DEBUG -------------------------------------------------- */

#define LCD_DEBUG 0

#define LCD_ERR(msg...) __lcd_err(__FILE__, __LINE__, msg)
static inline void __lcd_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_MSG(msg...) __lcd_msg(__FILE__, __LINE__, msg)
static inline void __lcd_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_WARN(msg...) __lcd_warn(__FILE__, __LINE__, msg)
static inline void __lcd_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

#endif /* LCD_DOMAINS_COMMON_H */
