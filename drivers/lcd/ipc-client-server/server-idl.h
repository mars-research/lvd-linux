#ifndef _LCD_IPC_CLIENT_SERVER_SERVER_H_
#define _LCD_IPC_CLIENT_SERVER_SERVER_H_

enum server_interface {
	server_interface_test_func1,
	server_interface_test_func2,
}

/* Caller stubs */
int test_func1(unsigned long a, long b, char c);
int test_func2(unsigned long a);

/* Callee stubs */
int test_func1_srv(unsigned long a, long b, char c);
int test_func2_srv(unsigned long a);

#endif
