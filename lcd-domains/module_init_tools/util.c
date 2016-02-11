#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <sys/types.h>
#include <regex.h>
#include "logging.h"
#include "util.h"

/*
 * Read one logical line from a configuration file.
 *
 * Line endings may be escaped with backslashes, to form one logical line from
 * several physical lines.  No end of line character(s) are included in the
 * result.
 *
 * If linenum is not NULL, it is incremented by the number of physical lines
 * which have been read.
 */
char *getline_wrapped(FILE *file, unsigned int *linenum)
{
	int size = 256;
	int i = 0;
	char *buf = NOFAIL(malloc(size));
	for(;;) {
		int ch = getc_unlocked(file);

		switch(ch) {
		case EOF:
			if (i == 0) {
				free(buf);
				return NULL;
			}
			/* else fall through */

		case '\n':
			if (linenum)
				(*linenum)++;
			if (i == size)
				buf = NOFAIL(realloc(buf, size + 1));
			buf[i] = '\0';
			return buf;

		case '\\':
			ch = getc_unlocked(file);

			if (ch == '\n') {
				if (linenum)
					(*linenum)++;
				continue;
			}
			/* else fall through */

		default:
			buf[i++] = ch;

			if (i == size) {
				size *= 2;
				buf = NOFAIL(realloc(buf, size));
			}
		}
	}
}

/*
 * Convert filename to the module name.  Works if filename == modname, too.
 */
void filename2modname(char *modname, const char *filename)
{
	const char *afterslash;
	unsigned int i;

	afterslash = my_basename(filename);

	/* Convert to underscores, stop at first . */
	for (i = 0; afterslash[i] && afterslash[i] != '.'; i++) {
		if (afterslash[i] == '-')
			modname[i] = '_';
		else
			modname[i] = afterslash[i];
	}
	modname[i] = '\0';
}

/*
 * Replace dashes with underscores.
 * Dashes inside character range patterns (e.g. [0-9]) are left unchanged.
 */
char *underscores(char *string)
{
	unsigned int i;

	if (!string)
		return NULL;

	for (i = 0; string[i]; i++) {
		switch (string[i]) {
		case '-':
			string[i] = '_';
			break;

		case ']':
			warn("Unmatched bracket in %s\n", string);
			break;

		case '[':
			i += strcspn(&string[i], "]");
			if (!string[i])
				warn("Unmatched bracket in %s\n", string);
			break;
		}
	}
	return string;
}

/*
 * strtbl_add - add a string to a string table.
 *
 * @str: string to add
 * @tbl: current string table. NULL = allocate new table
 *
 * Allocates an array of pointers to strings.
 * The strings themselves are not actually kept in the table.
 *
 * Returns reallocated and updated string table. NULL = out of memory.
 *
 * Implementation note: The string table is designed to be lighter-weight
 * and faster than a more conventional linked list that stores the strings
 * in the list elements, as it does far fewer malloc/realloc calls
 * and avoids copying entirely.
 */
struct string_table *strtbl_add(const char *str, struct string_table *tbl)
{
	if (tbl == NULL) {
		const char max = 100;
		tbl = malloc(sizeof(*tbl) + sizeof(char *) * max);
		if (!tbl)
			return NULL;
		tbl->max = max;
		tbl->cnt = 0;
	}
	if (tbl->cnt >= tbl->max) {
		tbl->max *= 2;
		tbl = realloc(tbl, sizeof(*tbl) + sizeof(char *) * tbl->max);
		if (!tbl)
			return NULL;
	}
	tbl->str[tbl->cnt] = str;
	tbl->cnt += 1;

	return tbl;
}

/*
 * strtbl_free - string table destructor
 */
void strtbl_free(struct string_table *tbl)
{
	free(tbl);
}

/*
 * Get the basename in a pathname.
 * Unlike the standard implementation, this does not copy the string.
 */
char *my_basename(const char *path)
{
	const char *base = strrchr(path, '/');
	if (base)
		return (char *) base + 1;
	return (char *) path;
}

/*
 * Find the next string in an ELF section.
 */
const char *next_string(const char *string, unsigned long *secsize)
{
	/* Skip non-zero chars */
	while (string[0]) {
		string++;
		if ((*secsize)-- <= 1)
			return NULL;
	}

	/* Skip any zero padding. */
	while (!string[0]) {
		string++;
		if ((*secsize)-- <= 1)
			return NULL;
	}
	return string;
}

/*
 * Get CPU endianness. 0 = unknown, 1 = ELFDATA2LSB = little, 2 = ELFDATA2MSB = big
 */
int __attribute__ ((pure)) native_endianness()
{
	/* Encoding the endianness enums in a string and then reading that
	 * string as a 32-bit int, returns the correct endianness automagically.
	 */
	return (char) *((uint32_t*)("\1\0\0\2"));
}

/*
 * Compare "string" with extended regex "pattern". Include backward compatible
 * matching of "*" as a wildcard by replacing it with ".*" automatically.
 */
int regex_match(const char *string, const char *pattern)
{
	int status;
	regex_t re;
	char *fix_pattern;

	/* backward compatibility with old "match" code */
	if (strncmp("*", pattern, 1) != 0)
		fix_pattern = (char *)pattern;
	else
		fix_pattern = ".*"; /* match everything */

	if (regcomp(&re, fix_pattern, REG_EXTENDED|REG_NOSUB) != 0)
		return 0;	/* alloc failure */

	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);

	if (status != 0)
		return 0;	/* no match */

	return 1;	/* match */
}
