#ifndef _LCD_IPC_CLIENT_SERVER_CLIENT_H_
#define _LCD_IPC_CLIENT_SERVER_CLIENT_H_

enum client_interface {
	client_interface_register_server,
};


int register_server_caller(struct server_interface *server);
int __register_server_caller(capability_t client, struct server_interface *server);

int register_server_callee(struct message_info *msg, capability_t reply_cap);

#endif
