#ifndef LCD_TRACE_H
#define LCD_TRACE_H

#define PROC_NAME_MAX		16
#define IF_FLAG			(1 << 9)
#define RING_BUFFER_SIZE	(2 * PAGE_SIZE)

#define IN_IRQ_SHIFT		0
#define IN_SOFTIRQ_SHIFT	1
#define IN_NMI_SHIFT		2

#define IN_IRQ			(1 << IN_IRQ_SHIFT)
#define IN_SOFTIRQ		(1 << IN_SOFTIRQ_SHIFT)
#define IN_NMI			(1 << IN_NMI_SHIFT)

#define EVENT_XMIT			1
#define EVENT_MSIX_HANDLER		2
#define EVENT_NAPI_COMPLETE_DONE	3
#define EVENT_IRQ			4
#define EVENT_NMI			5
#define EVENT_EXCEPTION			6
#define EVENT_IRQ_EXIT			7
#define EVENT_SOFTIRQ_POLL		8
#define EVENT_NET_RX_ACTION		9
#define EVENT_VMFUNC_TRAMP_ENTRY	10
#define EVENT_VMFUNC_TRAMP_EXIT		11
#define EVENT_VMFUNC_SBOARD_KLCD_ENTER	12
#define EVENT_VMFUNC_SBOARD_KLCD_LEAVE	13
#define EVENT_DO_PAGE_FAULT		14
#define EVENT_DO_PAGE_FAULT_LEAVE	15
#define EVENT_DO_INT3			16
#define EVENT_DO_INT3_LEAVE		17
#define EVENT_NMI_LEAVE			18
#define EVENT_NMI_FULL			19
#define EVENT_CTX_SWITCH		20

#ifndef __ASSEMBLY__
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
	unsigned type;
	unsigned orig_type;
	unsigned long long ts;
	char name[PROC_NAME_MAX];
};


struct ring_trace_header {
	unsigned long head;
	unsigned long size; 
};

#define NUM_TRACE_ENTRIES	((RING_BUFFER_SIZE - sizeof(struct ring_trace_header)) / sizeof(struct ring_trace_entry))

struct ring_trace_buffer {
	struct ring_trace_header header; 
	struct ring_trace_entry entries[NUM_TRACE_ENTRIES];
};

void add_trace_entry(unsigned type, unsigned long rdi);
asmlinkage __visible notrace void dump_ring_trace_buffer(void);
asmlinkage __visible notrace void dump_ring_trace_buffer_cpu(int cpu);
asmlinkage __visible notrace void add_trace_entry_tf(struct pt_regs *regs, unsigned type);


DECLARE_PER_CPU_PAGE_ALIGNED(struct ring_trace_buffer, ring_buffer);
#endif /* __ASSEMBLY__ */

#endif /* LCD_TRACE_H */
