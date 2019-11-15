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

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

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
	REGISTER_WIDE_HW_BREAKPOINT,
	UNREGISTER_WIDE_HW_BREAKPOINT,
};
int callee(struct fipc_message *);
#define TRANSACTIONS 1000000

#define INIT_FIPC_MSG(msg)             memset(msg, 0x0, sizeof(*msg))

#endif /* LCD_ASYNC_RPC_TEST_H */
