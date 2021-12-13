#ifndef XHCI_LCD_H
#define XHCI_LCD_H

#include "../../xhci.h"

#define trace_xhci_address_ctx(x, y, z)		do { } while(0)
#define trace_xhci_cmd_completion(x, y)		do { } while(0)

#if 0
#define xhci_debug_ring(x, y)			do { } while(0)
#define xhci_debug_segment(x, y)			do { } while(0)
#define xhci_debug_trb(x, y)			do { } while(0)

#define xhci_dbg_ring_ptrs(x, y)		do { } while(0)
#define xhci_dbg_ep_rings(...)		do { } while(0)
#define xhci_dbg_cmd_ptrs(x)		do { } while(0)
#define xhci_dbg_erst(x, y)		do { } while(0)
#define xhci_dbg_ctx(x, y, z)		do { } while(0)
#define xhci_dbg_regs(x)		do { } while(0)

#define xhci_print_trb_offsets(...)	do { } while(0)
#define xhci_print_ir_set(...)		do { } while(0)	
#define xhci_print_registers(...)	do { } while(0)
#define xhci_print_run_regs(...)	do { } while(0)
#define xhci_get_slot_state(...)	"none"
#endif		/* if 0 */


#define xhci_dbg_trace(xhci, trace, ...)	printk(__VA_ARGS__)

#undef xhci_dbg
#define xhci_dbg(xhci, ...)	printk(__VA_ARGS__)

#ifdef LCD_ISOLATE
#undef setup_timer
#define setup_timer	lvd_setup_timer

typedef void (*fptr_timer_func)(unsigned long data);
void lvd_setup_timer(struct timer_list* timer, fptr_timer_func func, unsigned long);

#undef init_completion
#define init_completion	lvd_init_completion

void lvd_init_completion(struct completion *c);

#endif		/* LCD_ISOLATE */


#endif	/* XHCI_LCD_H */
