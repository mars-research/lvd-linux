#ifndef XHCI_LCD_H
#define XHCI_LCD_H

#include "../../xhci.h"


#define trace_xhci_address_ctx(x, y, z)		do { } while(0)
#define trace_xhci_cmd_completion(x, y)		do { } while(0)

#define xhci_debug_ring(x, y)			do { } while(0)
#define xhci_debug_segment(x, y)			do { } while(0)
#define xhci_debug_trb(x, y)			do { } while(0)

#define xhci_dbg_ring_ptrs(x, y)		do { } while(0)
#define xhci_dbg_ep_rings(...)		do { } while(0)
#define xhci_dbg_cmd_ptrs(x)		do { } while(0)
#define xhci_dbg_erst(x, y)		do { } while(0)
#define xhci_dbg_ctx(x, y, z)		do { } while(0)
#define xhci_dbg_regs(x)		do { } while(0)
#define xhci_dbg_trace(xhci, trace, ...)	LIBLCD_MSG(__VA_ARGS__)

#undef xhci_dbg
#define xhci_dbg(xhci, ...)	LIBLCD_MSG(__VA_ARGS__)

#define xhci_print_trb_offsets(...)	do { } while(0)
#define xhci_print_ir_set(...)		do { } while(0)	
#define xhci_print_registers(...)	do { } while(0)
#define xhci_print_run_regs(...)	do { } while(0)
#define xhci_get_slot_state(...)	"none"

#endif
