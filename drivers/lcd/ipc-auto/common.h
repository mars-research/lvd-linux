/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#define AU_ERR(msg...) __au_err(__FILE__, __LINE__, msg)
static inline void __au_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_err("lcd-ipc-auto: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define AU_MSG(msg...) __au_msg(__FILE__, __LINE__, msg)
static inline void __au_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_info("lcd-ipc-auto: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define AU_WARN(msg...) __au_warn(__FILE__, __LINE__, msg)
static inline void __au_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	pr_warn("lcd-ipc-auto: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
