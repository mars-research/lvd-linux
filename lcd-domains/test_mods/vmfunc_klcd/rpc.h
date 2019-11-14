/*
 * rpc.h
 *
 * Internal defs
 *
 * Copyright: University of Utah
 */
#ifndef LCD_ASYNC_RPC_TEST_H
#define LCD_ASYNC_RPC_TEST_H

#include <libfipc.h>

enum fn_type {
	MODULE_INIT,
	NULL_INVOCATION, 
	CALLEE,
	FOO,
	BAR,
	MARSHAL_ONE,
	MEMTEST,
	IRQ_HANDLER,
	DUMMY,
};
int callee(struct fipc_message *);
#define TRANSACTIONS 1000000

#define INIT_FIPC_MSG(msg)             memset(msg, 0x0, sizeof(*msg))

#endif /* LCD_ASYNC_RPC_TEST_H */
