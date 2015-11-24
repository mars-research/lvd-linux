#ifndef MODINITTOOLS_LOGGING_H
#define MODINITTOOLS_LOGGING_H

/* Do we use syslog for messages or stderr? */
extern int logging;

/* Do we want to silently drop all warnings? */
extern int quiet;

/* Do we want informative messages as well as errors? */
extern int verbose;

#ifdef __GNUC__
#define _printf __attribute__((format(printf, 1, 2)))
#else
#define _printf
#endif

extern void _printf fatal(const char *fmt, ...);
extern void _printf error(const char *fmt, ...);
extern void _printf warn(const char *fmt, ...);
extern void _printf info(const char *fmt, ...);

typedef void _printf (*errfn_t)(const char *fmt, ...);

static inline void grammar(const char *cmd,
			   const char *filename, unsigned int line)
{
	warn("%s line %u: ignoring bad line starting with '%s'\n",
	     filename, line, cmd);
}

#define NOFAIL(ptr)  do_nofail((ptr), __FILE__, __LINE__, #ptr)

#define nofail_asprintf(ptr, ...)				\
	do { if (asprintf((ptr), __VA_ARGS__) < 0) 		\
		do_nofail(NULL, __FILE__, __LINE__, #ptr);	\
	} while(0)

static inline void *do_nofail(void *ptr, const char *file, int line, const char *expr)
{
	if (!ptr) {
		fatal("Memory allocation failure %s line %d: %s.\n",
		      file, line, expr);
	}
	return ptr;
}

#endif /* MODINITTOOLS_LOGGING_H */
