#ifndef _LCD_IPC_CLIENT_SERVER_CLIENT_H_
#define _LCD_IPC_CLIENT_SERVER_CLIENT_H_

enum client_interface {
	client_interface_register_server,
};

int register_server(capability_t client, struct server_interface *server);
int register_server_callee(capability_t server, );

#endif
