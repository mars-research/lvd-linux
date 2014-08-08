/**
 * client.c - client for the client-server IPC test 
 *
 *
 * Authors: Anton Burtsev   <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include "common.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD client-server test client");

struct server_interface *server;

int client_register_server(struct server_interface *s)
{
	server = s;
	return 0;
}
EXPORT_SYMBOL(client_register_server);

int client_do_work(void)
{
	struct engine *e;
	struct automobile *a;
	int ret;
	/*
	 * Make an 8 cylinder engine
	 */
	e = server->mk_engine(8);
	if (!e || e->cylinders != 8) {
		CS_ERR("client: bad engine");
		return -1;
	}
	/*
	 * Make a 4-door car with e as its engine.
	 */
	a = server->mk_automobile(e, 4);
	if (!a || a->engine != e || a->doors != 4) {
		CS_ERR("client: bad auto");
		return -1;
	}
	/*
	 * Free the data
	 */
	server->free_engine(e);
	server->free_automobile(a);
	return 0;
}
EXPORT_SYMBOL(client_do_work);

#if CONFIG_RUN_IN_LCD
int client_init(void)
#else
static int __init client_init(void)
#endif
{
	CS_MSG("client initialized");
	return 0;
}

#if CONFIG_RUN_IN_LCD
void client_exit(void)
#else
static void __init client_exit(void)
#endif
{
	CS_MSG("client exited");
}

#if !CONFIG_RUN_IN_LCD
module_init(client_init);
module_exit(client_exit);
#endif
