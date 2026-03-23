/*
 * libfipc.h
 *
 * Stub header for FIPC compatibility. Modern LVD modules only use the
 * struct fipc_message definition for VMFUNC-based RPC, not the actual
 * FIPC ring buffer library.
 *
 * The FIPC ring buffer library (fipc_init, fipc_send_msg, fipc_recv_msg, etc)
 * has been removed. Only struct definitions remain.
 *
 * Copyright: University of Utah
 */
#ifndef LIBFIPC_H
#define LIBFIPC_H

#include <libfipc_types.h>

#endif /* LIBFIPC_H */
