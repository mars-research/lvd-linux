/*
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef _LCD_TEST_CLIENT_SERVER_CLIENT_H_
#define _LCD_TEST_CLIENT_SERVER_CLIENT_H_

struct server_interface {
	int (*test_func1) (unsigned long a, long b, char c);
	int (*test_func2) (unsigned long a);
};

int register_server(struct server_interface *s);

#endif
