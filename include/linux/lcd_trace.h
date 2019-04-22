#ifndef LCD_TRACE_H
#define LCD_TRACE_H

#define PROC_NAME_MAX		16
#define IF_FLAG			(1 << 9)
#define RING_BUFFER_SIZE	(1 * PAGE_SIZE)

#define IN_IRQ_SHIFT		0
#define IN_SOFTIRQ_SHIFT	1
#define IN_NMI_SHIFT		2

#define IN_IRQ			(1 << IN_IRQ_SHIFT)
#define IN_SOFTIRQ		(1 << IN_SOFTIRQ_SHIFT)
#define IN_NMI			(1 << IN_NMI_SHIFT)


typedef enum {
	EVENT_XMIT = 1,
	EVENT_MSIX_HANDLER,
	EVENT_NAPI_COMPLETE_DONE,
	EVENT_IRQ = 4,
	EVENT_NMI,
	EVENT_EXCEPTION,
	EVENT_IRQ_EXIT,
	EVENT_SOFTIRQ_POLL = 8,
	EVENT_NET_RX_ACTION,
	EVENT_VMFUNC_TRAMP_ENTRY = 10,
	EVENT_VMFUNC_TRAMP_EXIT,
	EVENT_VMFUNC_SBOARD_KLCD_ENTER = 12,
	EVENT_VMFUNC_SBOARD_KLCD_LEAVE = 13,
} trace_event_t;

struct ring_trace_entry {
	unsigned long rip;
	unsigned long eflags;
	unsigned long rsp;
	unsigned long rdi;
	unsigned long lcd_stack;
	unsigned long gsbase;
	unsigned char context;
	unsigned char lcd_stack_bit;
	unsigned char lcd_nc;
	unsigned short pid;
	trace_event_t type;
	char name[PROC_NAME_MAX];
};

void add_trace_entry(trace_event_t type, unsigned long rdi);
void dump_ring_trace_buffer(void);
asmlinkage __visible notrace void add_trace_entry_tf(struct pt_regs *regs, trace_event_t type);

DECLARE_PER_CPU_PAGE_ALIGNED(unsigned char, ring_buffer[RING_BUFFER_SIZE]);
DECLARE_PER_CPU(unsigned char, ring_head);

#endif /* LCD_TRACE_H */
