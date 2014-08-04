#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>

struct string_table
{
	unsigned int cnt;
	unsigned int max;
	const char *str[0];
};

char *getline_wrapped(FILE *file, unsigned int *linenum);

void filename2modname(char *modname, const char *filename);
char *underscores(char *string);
char *my_basename(const char *path);

struct string_table *strtbl_add(const char *str, struct string_table *tbl);
void strtbl_free(struct string_table *tbl);

const char *next_string(const char *string, unsigned long *secsize);

/*
 * Change endianness of x if conv is true.
 */
#define END(x, conv)							\
({									\
	typeof(x) __x;							\
	if (conv) __swap_bytes(&(x), &(__x), sizeof(__x));		\
	else __x = (x);							\
	__x;								\
})

static inline void __swap_bytes(const void *src, void *dest, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
		((unsigned char*)dest)[i] = ((unsigned char*)src)[size - i-1];
}

int native_endianness(void);

#define streq(a,b) (strcmp((a),(b)) == 0)
#define strstarts(a,start) (strncmp((a),(start), strlen(start)) == 0)

int regex_match(const char *string, const char *pattern);

#endif
