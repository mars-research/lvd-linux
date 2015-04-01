/*
 *  Based on lib/vsprintf.c in Linux.
 */

#include <stdarg.h>

static int lcd_isdigit(int c)
{
	return c >= ((int)('0')) && c <= ((int)('9'));
}

static void *lcd_memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}


static noinline_for_stack
int skip_atoi(const char **s)
{
	int i = 0;

	while (lcd_isdigit(**s))
		i = i*10 + *((*s)++) - '0';

	return i;
}

/* Decimal conversion is by far the most typical, and is used
 * for /proc and /sys data. This directly impacts e.g. top performance
 * with many processes running. We optimize it for speed
 * using ideas described at <http://www.cs.uiowa.edu/~jones/bcd/divide.html>
 * (with permission from the author, Douglas W. Jones).
 */

#if BITS_PER_LONG != 32 || BITS_PER_LONG_LONG != 64
/* Formats correctly any integer in [0, 999999999] */
static noinline_for_stack
char *put_dec_full9(char *buf, unsigned q)
{
	unsigned r;

	/*
	 * Possible ways to approx. divide by 10
	 * (x * 0x1999999a) >> 32 x < 1073741829 (multiply must be 64-bit)
	 * (x * 0xcccd) >> 19     x <      81920 (x < 262149 when 64-bit mul)
	 * (x * 0x6667) >> 18     x <      43699
	 * (x * 0x3334) >> 17     x <      16389
	 * (x * 0x199a) >> 16     x <      16389
	 * (x * 0x0ccd) >> 15     x <      16389
	 * (x * 0x0667) >> 14     x <       2739
	 * (x * 0x0334) >> 13     x <       1029
	 * (x * 0x019a) >> 12     x <       1029
	 * (x * 0x00cd) >> 11     x <       1029 shorter code than * 0x67 (on i386)
	 * (x * 0x0067) >> 10     x <        179
	 * (x * 0x0034) >>  9     x <         69 same
	 * (x * 0x001a) >>  8     x <         69 same
	 * (x * 0x000d) >>  7     x <         69 same, shortest code (on i386)
	 * (x * 0x0007) >>  6     x <         19
	 * See <http://www.cs.uiowa.edu/~jones/bcd/divide.html>
	 */
	r      = (q * (uint64_t)0x1999999a) >> 32;
	*buf++ = (q - 10 * r) + '0'; /* 1 */
	q      = (r * (uint64_t)0x1999999a) >> 32;
	*buf++ = (r - 10 * q) + '0'; /* 2 */
	r      = (q * (uint64_t)0x1999999a) >> 32;
	*buf++ = (q - 10 * r) + '0'; /* 3 */
	q      = (r * (uint64_t)0x1999999a) >> 32;
	*buf++ = (r - 10 * q) + '0'; /* 4 */
	r      = (q * (uint64_t)0x1999999a) >> 32;
	*buf++ = (q - 10 * r) + '0'; /* 5 */
	/* Now value is under 10000, can avoid 64-bit multiply */
	q      = (r * 0x199a) >> 16;
	*buf++ = (r - 10 * q)  + '0'; /* 6 */
	r      = (q * 0xcd) >> 11;
	*buf++ = (q - 10 * r)  + '0'; /* 7 */
	q      = (r * 0xcd) >> 11;
	*buf++ = (r - 10 * q) + '0'; /* 8 */
	*buf++ = q + '0'; /* 9 */
	return buf;
}
#endif

/* Similar to above but do not pad with zeros.
 * Code can be easily arranged to print 9 digits too, but our callers
 * always call put_dec_full9() instead when the number has 9 decimal digits.
 */
static noinline_for_stack
char *put_dec_trunc8(char *buf, unsigned r)
{
	unsigned q;

	/* Copy of previous function's body with added early returns */
	while (r >= 10000) {
		q = r + '0';
		r  = (r * (uint64_t)0x1999999a) >> 32;
		*buf++ = q - 10*r;
	}

	q      = (r * 0x199a) >> 16;	/* r <= 9999 */
	*buf++ = (r - 10 * q)  + '0';
	if (q == 0)
		return buf;
	r      = (q * 0xcd) >> 11;	/* q <= 999 */
	*buf++ = (q - 10 * r)  + '0';
	if (r == 0)
		return buf;
	q      = (r * 0xcd) >> 11;	/* r <= 99 */
	*buf++ = (r - 10 * q) + '0';
	if (q == 0)
		return buf;
	*buf++ = q + '0';		 /* q <= 9 */
	return buf;
}

/* There are two algorithms to print larger numbers.
 * One is generic: divide by 1000000000 and repeatedly print
 * groups of (up to) 9 digits. It's conceptually simple,
 * but requires a (unsigned long long) / 1000000000 division.
 *
 * Second algorithm splits 64-bit unsigned long long into 16-bit chunks,
 * manipulates them cleverly and generates groups of 4 decimal digits.
 * It so happens that it does NOT require long long division.
 *
 * If long is > 32 bits, division of 64-bit values is relatively easy,
 * and we will use the first algorithm.
 * If long long is > 64 bits (strange architecture with VERY large long long),
 * second algorithm can't be used, and we again use the first one.
 *
 * Else (if long is 32 bits and long long is 64 bits) we use second one.
 */

#if BITS_PER_LONG != 32 || BITS_PER_LONG_LONG != 64

/* First algorithm: generic */

static
char *put_dec(char *buf, unsigned long long n)
{
	if (n >= 100*1000*1000) {
		while (n >= 1000*1000*1000)
			buf = put_dec_full9(buf, do_div(n, 1000*1000*1000));
		if (n >= 100*1000*1000)
			return put_dec_full9(buf, n);
	}
	return put_dec_trunc8(buf, n);
}

#else

/* Second algorithm: valid only for 64-bit long longs */

/* See comment in put_dec_full9 for choice of constants */
static noinline_for_stack
void put_dec_full4(char *buf, unsigned q)
{
	unsigned r;
	r      = (q * 0xccd) >> 15;
	buf[0] = (q - 10 * r) + '0';
	q      = (r * 0xcd) >> 11;
	buf[1] = (r - 10 * q)  + '0';
	r      = (q * 0xcd) >> 11;
	buf[2] = (q - 10 * r)  + '0';
	buf[3] = r + '0';
}

/*
 * Call put_dec_full4 on x % 10000, return x / 10000.
 * The approximation x/10000 == (x * 0x346DC5D7) >> 43
 * holds for all x < 1,128,869,999.  The largest value this
 * helper will ever be asked to convert is 1,125,520,955.
 * (d1 in the put_dec code, assuming n is all-ones).
 */
static
unsigned put_dec_helper4(char *buf, unsigned x)
{
        uint32_t q = (x * (uint64_t)0x346DC5D7) >> 43;

        put_dec_full4(buf, x - q * 10000);
        return q;
}

/* Based on code by Douglas W. Jones found at
 * <http://www.cs.uiowa.edu/~jones/bcd/decimal.html#sixtyfour>
 * (with permission from the author).
 * Performs no 64-bit division and hence should be fast on 32-bit machines.
 */
static
char *put_dec(char *buf, unsigned long long n)
{
	uint32_t d3, d2, d1, q, h;

	if (n < 100*1000*1000)
		return put_dec_trunc8(buf, n);

	d1  = ((uint32_t)n >> 16); /* implicit "& 0xffff" */
	h   = (n >> 32);
	d2  = (h      ) & 0xffff;
	d3  = (h >> 16); /* implicit "& 0xffff" */

	q   = 656 * d3 + 7296 * d2 + 5536 * d1 + ((uint32_t)n & 0xffff);
	q = put_dec_helper4(buf, q);

	q += 7671 * d3 + 9496 * d2 + 6 * d1;
	q = put_dec_helper4(buf+4, q);

	q += 4749 * d3 + 42 * d2;
	q = put_dec_helper4(buf+8, q);

	q += 281 * d3;
	buf += 12;
	if (q)
		buf = put_dec_trunc8(buf, q);
	else while (buf[-1] == '0')
		--buf;

	return buf;
}

#endif

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* use lowercase in hex (must be 32 == 0x20) */
#define SPECIAL	64		/* prefix hex with "0x", octal with "0" */

enum format_type {
	FORMAT_TYPE_NONE, /* Just a string part */
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
	FORMAT_TYPE_PTR,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_INVALID,
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
	FORMAT_TYPE_USHORT,
	FORMAT_TYPE_SHORT,
	FORMAT_TYPE_UINT,
	FORMAT_TYPE_INT,
	FORMAT_TYPE_NRCHARS,
	FORMAT_TYPE_SIZE_T,
	FORMAT_TYPE_PTRDIFF
};

struct printf_spec {
	u8	type;		/* format_type enum */
	u8	flags;		/* flags to number() */
	u8	base;		/* number base, 8, 10 or 16 only */
	u8	qualifier;	/* number qualifier, one of 'hHlLtzZ' */
	s16	field_width;	/* width of output field */
	s16	precision;	/* # of digits/chars */
};

static noinline_for_stack
char *number(char *buf, char *end, unsigned long long num,
	     struct printf_spec spec)
{
	/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
	static const char digits[16] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */

	char tmp[66];
	char sign;
	char locase;
	int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10);
	int i;
	bool is_zero = num == 0LL;

	/* locase = 0 or 0x20. ORing digits or letters with 'locase'
	 * produces same digits or (maybe lowercased) letters */
	locase = (spec.flags & SMALL);
	if (spec.flags & LEFT)
		spec.flags &= ~ZEROPAD;
	sign = 0;
	if (spec.flags & SIGN) {
		if ((signed long long)num < 0) {
			sign = '-';
			num = -(signed long long)num;
			spec.field_width--;
		} else if (spec.flags & PLUS) {
			sign = '+';
			spec.field_width--;
		} else if (spec.flags & SPACE) {
			sign = ' ';
			spec.field_width--;
		}
	}
	if (need_pfx) {
		if (spec.base == 16)
			spec.field_width -= 2;
		else if (!is_zero)
			spec.field_width--;
	}

	/* generate full string in tmp[], in reverse order */
	i = 0;
	if (num < spec.base)
		tmp[i++] = digits[num] | locase;
	/* Generic code, for any base:
	else do {
		tmp[i++] = (digits[do_div(num,base)] | locase);
	} while (num != 0);
	*/
	else if (spec.base != 10) { /* 8 or 16 */
		int mask = spec.base - 1;
		int shift = 3;

		if (spec.base == 16)
			shift = 4;
		do {
			tmp[i++] = (digits[((unsigned char)num) & mask] | locase);
			num >>= shift;
		} while (num);
	} else { /* base 10 */
		i = put_dec(tmp, num) - tmp;
	}

	/* printing 100 using %2d gives "100", not "00" */
	if (i > spec.precision)
		spec.precision = i;
	/* leading space padding */
	spec.field_width -= spec.precision;
	if (!(spec.flags & (ZEROPAD+LEFT))) {
		while (--spec.field_width >= 0) {
			if (buf < end)
				*buf = ' ';
			++buf;
		}
	}
	/* sign */
	if (sign) {
		if (buf < end)
			*buf = sign;
		++buf;
	}
	/* "0x" / "0" prefix */
	if (need_pfx) {
		if (spec.base == 16 || !is_zero) {
			if (buf < end)
				*buf = '0';
			++buf;
		}
		if (spec.base == 16) {
			if (buf < end)
				*buf = ('X' | locase);
			++buf;
		}
	}
	/* zero or space padding */
	if (!(spec.flags & LEFT)) {
		char c = (spec.flags & ZEROPAD) ? '0' : ' ';
		while (--spec.field_width >= 0) {
			if (buf < end)
				*buf = c;
			++buf;
		}
	}
	/* hmm even more zero padding? */
	while (i <= --spec.precision) {
		if (buf < end)
			*buf = '0';
		++buf;
	}
	/* actual digits of result */
	while (--i >= 0) {
		if (buf < end)
			*buf = tmp[i];
		++buf;
	}
	/* trailing space padding */
	while (--spec.field_width >= 0) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}

	return buf;
}

static size_t lcd_strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

static noinline_for_stack
char *lcd_string(char *buf, char *end, const char *s, struct printf_spec spec)
{
	int len, i;

	if ((unsigned long)s < PAGE_SIZE)
		s = "(null)";

	len = lcd_strnlen(s, spec.precision);

	if (!(spec.flags & LEFT)) {
		while (len < spec.field_width--) {
			if (buf < end)
				*buf = ' ';
			++buf;
		}
	}
	for (i = 0; i < len; ++i) {
		if (buf < end)
			*buf = *s;
		++buf; ++s;
	}
	while (len < spec.field_width--) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}

	return buf;
}

/*
 * Helper function to decode printf style format.
 * Each call decode a token from the format and return the
 * number of characters read (or likely the delta where it wants
 * to go on the next call).
 * The decoded token is returned through the parameters
 *
 * 'h', 'l', or 'L' for integer fields
 * 'z' support added 23/7/1999 S.H.
 * 'z' changed to 'Z' --davidm 1/25/99
 * 't' added for ptrdiff_t
 *
 * @fmt: the format string
 * @type of the token returned
 * @flags: various flags such as +, -, # tokens..
 * @field_width: overwritten width
 * @base: base of the number (octal, hex, ...)
 * @precision: precision of a number
 * @qualifier: qualifier of a number (long, size_t, ...)
 */
static noinline_for_stack
int lcd_format_decode(const char *fmt, struct printf_spec *spec)
{
	const char *start = fmt;

	/* we finished early by reading the field width */
	if (spec->type == FORMAT_TYPE_WIDTH) {
		if (spec->field_width < 0) {
			spec->field_width = -spec->field_width;
			spec->flags |= LEFT;
		}
		spec->type = FORMAT_TYPE_NONE;
		goto precision;
	}

	/* we finished early by reading the precision */
	if (spec->type == FORMAT_TYPE_PRECISION) {
		if (spec->precision < 0)
			spec->precision = 0;

		spec->type = FORMAT_TYPE_NONE;
		goto qualifier;
	}

	/* By default */
	spec->type = FORMAT_TYPE_NONE;

	for (; *fmt ; ++fmt) {
		if (*fmt == '%')
			break;
	}

	/* Return the current non-format string */
	if (fmt != start || !*fmt)
		return fmt - start;

	/* Process flags */
	spec->flags = 0;

	while (1) { /* this also skips first '%' */
		bool found = true;

		++fmt;

		switch (*fmt) {
		case '-': spec->flags |= LEFT;    break;
		case '+': spec->flags |= PLUS;    break;
		case ' ': spec->flags |= SPACE;   break;
		case '#': spec->flags |= SPECIAL; break;
		case '0': spec->flags |= ZEROPAD; break;
		default:  found = false;
		}

		if (!found)
			break;
	}

	/* get field width */
	spec->field_width = -1;

	if (lcd_isdigit(*fmt))
		spec->field_width = skip_atoi(&fmt);
	else if (*fmt == '*') {
		/* it's the next argument */
		spec->type = FORMAT_TYPE_WIDTH;
		return ++fmt - start;
	}

precision:
	/* get the precision */
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (lcd_isdigit(*fmt)) {
			spec->precision = skip_atoi(&fmt);
			if (spec->precision < 0)
				spec->precision = 0;
		} else if (*fmt == '*') {
			/* it's the next argument */
			spec->type = FORMAT_TYPE_PRECISION;
			return ++fmt - start;
		}
	}

qualifier:
	/* skip */
#if 0
	/* get the conversion qualifier */
	spec->qualifier = -1;
	if (*fmt == 'h' || _tolower(*fmt) == 'l' ||
	    _tolower(*fmt) == 'z' || *fmt == 't') {
		spec->qualifier = *fmt++;
		if (unlikely(spec->qualifier == *fmt)) {
			if (spec->qualifier == 'l') {
				spec->qualifier = 'L';
				++fmt;
			} else if (spec->qualifier == 'h') {
				spec->qualifier = 'H';
				++fmt;
			}
		}
	}
#endif

	/* default base */
	spec->base = 10;
	switch (*fmt) {
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;

	case 's':
		spec->type = FORMAT_TYPE_STR;
		return ++fmt - start;

	case 'p':
		spec->type = FORMAT_TYPE_PTR;
		return fmt - start;
		/* skip alnum */

	case 'n':
		spec->type = FORMAT_TYPE_NRCHARS;
		return ++fmt - start;

	case '%':
		spec->type = FORMAT_TYPE_PERCENT_CHAR;
		return ++fmt - start;

	/* integer number formats - set up the flags and "break" */
	case 'o':
		spec->base = 8;
		break;

	case 'x':
		spec->flags |= SMALL;

	case 'X':
		spec->base = 16;
		break;

	case 'd':
	case 'i':
		spec->flags |= SIGN;
	case 'u':
		break;

	default:
		spec->type = FORMAT_TYPE_INVALID;
		return fmt - start;
	}

	if (spec->qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (spec->qualifier == 'l') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_LONG;
		else
			spec->type = FORMAT_TYPE_ULONG;
#if 0
	} else if (_tolower(spec->qualifier) == 'z') {
		spec->type = FORMAT_TYPE_SIZE_T;
#endif
	} else if (spec->qualifier == 't') {
		spec->type = FORMAT_TYPE_PTRDIFF;
	} else if (spec->qualifier == 'H') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_BYTE;
		else
			spec->type = FORMAT_TYPE_UBYTE;
	} else if (spec->qualifier == 'h') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_SHORT;
		else
			spec->type = FORMAT_TYPE_USHORT;
	} else {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_INT;
		else
			spec->type = FORMAT_TYPE_UINT;
	}

	return ++fmt - start;
}

/**
 * vsnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * This function follows C99 vsnprintf, but has some extensions:
 * %pS output the name of a text symbol with offset
 * %ps output the name of a text symbol without offset
 * %pF output the name of a function pointer with its offset
 * %pf output the name of a function pointer without its offset
 * %pB output the name of a backtrace symbol with its offset
 * %pR output the address range in a struct resource with decoded flags
 * %pr output the address range in a struct resource with raw flags
 * %pM output a 6-byte MAC address with colons
 * %pMR output a 6-byte MAC address with colons in reversed order
 * %pMF output a 6-byte MAC address with dashes
 * %pm output a 6-byte MAC address without colons
 * %pmR output a 6-byte MAC address without colons in reversed order
 * %pI4 print an IPv4 address without leading zeros
 * %pi4 print an IPv4 address with leading zeros
 * %pI6 print an IPv6 address with colons
 * %pi6 print an IPv6 address without colons
 * %pI6c print an IPv6 address as specified by RFC 5952
 * %pU[bBlL] print a UUID/GUID in big or little endian using lower or upper
 *   case.
 * %*ph[CDN] a variable-length hex string with a separator (supports up to 64
 *           bytes of the input)
 * %n is ignored
 *
 * ** Please update Documentation/printk-formats.txt when making changes **
 *
 * The return value is the number of characters which would
 * be generated for the given input, excluding the trailing
 * '\0', as per ISO C99. If you want to have the exact
 * number of characters written into @buf as return value
 * (not including the trailing '\0'), use vscnprintf(). If the
 * return is greater than or equal to @size, the resulting
 * string is truncated.
 *
 * If you're not already dealing with a va_list consider using snprintf().
 */
static int lcd_vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	unsigned long long num;
	char *str, *end;
	struct printf_spec spec = {0};

	str = buf;
	end = buf + size;

	/* Make sure end is always >= buf */
	if (end < buf) {
		end = ((void *)-1);
		size = end - buf;
	}

	while (*fmt) {
		const char *old_fmt = fmt;
		int read = lcd_format_decode(fmt, &spec);

		fmt += read;

		switch (spec.type) {
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
				lcd_memcpy(str, old_fmt, copy);
			}
			str += read;
			break;
		}

		case FORMAT_TYPE_WIDTH:
			spec.field_width = va_arg(args, int);
			break;

		case FORMAT_TYPE_PRECISION:
			spec.precision = va_arg(args, int);
			break;

		case FORMAT_TYPE_CHAR: {
			char c;

			if (!(spec.flags & LEFT)) {
				while (--spec.field_width > 0) {
					if (str < end)
						*str = ' ';
					++str;

				}
			}
			c = (unsigned char) va_arg(args, int);
			if (str < end)
				*str = c;
			++str;
			while (--spec.field_width > 0) {
				if (str < end)
					*str = ' ';
				++str;
			}
			break;
		}

		case FORMAT_TYPE_STR:
			str = lcd_string(str, end, va_arg(args, char *), spec);
			break;

		case FORMAT_TYPE_PERCENT_CHAR:
			if (str < end)
				*str = '%';
			++str;
			break;

		case FORMAT_TYPE_INVALID:
			if (str < end)
				*str = '%';
			++str;
			break;

		default:
			switch (spec.type) {
			case FORMAT_TYPE_LONG_LONG:
				num = va_arg(args, long long);
				break;
			case FORMAT_TYPE_ULONG:
				num = va_arg(args, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(args, long);
				break;
			case FORMAT_TYPE_SIZE_T:
				if (spec.flags & SIGN)
					num = va_arg(args, ssize_t);
				else
					num = va_arg(args, size_t);
				break;
			case FORMAT_TYPE_PTRDIFF:
				num = va_arg(args, ptrdiff_t);
				break;
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char) va_arg(args, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char) va_arg(args, int);
				break;
			case FORMAT_TYPE_USHORT:
				num = (unsigned short) va_arg(args, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (short) va_arg(args, int);
				break;
			case FORMAT_TYPE_INT:
				num = (int) va_arg(args, int);
				break;
			default:
				num = va_arg(args, unsigned int);
			}

			str = number(str, end, num, spec);
		}
	}

	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}

	/* the trailing null byte doesn't count towards the total */
	return str-buf;
}

