/*
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef _LCD_TEST_
struct server_interface {
	int (*test_func1) (unsigned long a, long b, char c);
	int (*test_func2) (unsigned long a);
};

