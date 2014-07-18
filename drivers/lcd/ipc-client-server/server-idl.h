#ifndef _LCD_IPC_CLIENT_SERVER_SERVER_H_
#define _LCD_IPC_CLIENT_SERVER_SERVER_H_

enum server_interface {
	server_interface_test_func1,
	server_interface_test_func2,
}

/* Caller stubs */
int test_func1_caller(capability_t server, unsigned long a, long b, char c);
int test_func2_caller(capability_t server, unsigned long a);

/* Callee stubs */
int test_func1_callee(struct message_info *msg, capability_t reply_cap);
int test_func2_callee(struct message_info *msg, capability_t reply_cap);

/* Introduction code */
extern int accept_client_introduction(void);

#endif
