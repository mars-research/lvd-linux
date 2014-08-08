/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#define CS_ERR(msg...) __cs_err(__FILE__, __LINE__, msg)
static inline void __cs_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_err("cs: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define CS_MSG(msg...) __cs_msg(__FILE__, __LINE__, msg)
static inline void __cs_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_info("cs: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define CS_WARN(msg...) __cs_warn(__FILE__, __LINE__, msg)
static inline void __cs_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_warn("cs: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
