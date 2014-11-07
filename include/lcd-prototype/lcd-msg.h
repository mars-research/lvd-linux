/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */
#ifndef LCD_PROTOTYPE_LCD_MSG_H
#define LCD_PROTOTYPE_LCD_MSG_H

#define LCD_ERR(fmt, args...) __lcd_err(__FILE__, __LINE__, fmt "\n" , ##args)
static inline void __lcd_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_err("lcd-proto: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_MSG(fmt, args...) __lcd_msg(__FILE__, __LINE__, fmt "\n" , ##args)
static inline void __lcd_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_info("lcd-proto: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_WARN(fmt, args...) __lcd_warn(__FILE__, __LINE__, fmt "\n" , ##args)
static inline void __lcd_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_warn("lcd-proto: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

#endif /* LCD_PROTOTYPE_LCD_MSG_H */
