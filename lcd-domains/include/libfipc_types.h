/*
 * libfipc_types.h
 *
 * Essential FIPC message structure definitions for VMFUNC-based IPC.
 * Extracted from libasync/fast-ipc-module.
 *
 * Copyright: University of Utah
 */
#ifndef LIBFIPC_TYPES_H
#define LIBFIPC_TYPES_H

#include <linux/types.h>

/**
 * Assumed cacheline size, in bytes.
 */
#define FIPC_CACHE_LINE_SIZE 64

// Type modifier that aligns the variable to the cache line
#ifndef CACHE_ALIGNED
	#define CACHE_ALIGNED __attribute__((aligned(FIPC_CACHE_LINE_SIZE)))
#endif

/**
 * struct fipc_message
 *
 * This is the data structure used for VMFUNC-based RPC calls between
 * LCDs and kLCDs. It fits into one cache line. All fields are available
 * for use, except msg_status - this is reserved for tracking message
 * status in ring buffer implementations.
 *
 * Modern LVD modules populate this struct on the stack and pass it
 * directly via VMFUNC, without using the FIPC ring buffer library.
 */
#define FIPC_NR_REGS 7
struct fipc_message {
	union {
		struct {
			/**
			 * Reserved. Used internally to track message status.
			 */
			volatile uint32_t msg_status;
			/**
			 * Not touched by libfipc.
			 */
			uint32_t flags;
		};
		struct {
			union {
				uint32_t syscall_nr;
				uint32_t rpc_id;
			};
			uint32_t vmfunc_id;
		};
		unsigned long id;
	};
	/**
	 * Not touched by libfipc.
	 */
	unsigned long regs[FIPC_NR_REGS];
} CACHE_ALIGNED ;

#endif /* LIBFIPC_TYPES_H */
