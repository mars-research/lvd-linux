/**
 * util.h - common test helper stuff.
 */

#ifndef LCD_DOMAINS_TESTS_UTIL_H
#define LCD_DOMAINS_TESTS_UTIL_H

#include <linux/kernel.h>

#define RUN_TEST(func, counter) {				\
		if (func()) {					\
			printk("test %s failed\n", #func);	\
		} else {					\
			counter++;				\
		}						\
	}

#endif /* LCD_DOMAINS_TESTS_UTIL_H */
