#include <asm/msr.h>
#include <asm/ptrace.h>
#include <asm/current.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/lcd_trace.h>

DEFINE_PER_CPU_PAGE_ALIGNED(unsigned char, ring_buffer[RING_BUFFER_SIZE]);
EXPORT_PER_CPU_SYMBOL(ring_buffer);

DEFINE_PER_CPU(unsigned char, ring_head);
EXPORT_PER_CPU_SYMBOL(ring_head);

#define NUM_TRACE_ENTRIES	(RING_BUFFER_SIZE / sizeof(struct ring_trace_entry))

static const char *event_type_to_string(unsigned type)
{
	switch (type) {
		case EVENT_XMIT:
			return "XMIT";

		case EVENT_MSIX_HANDLER:
			return "MSIX_HANDLER";

		case EVENT_NAPI_COMPLETE_DONE:
			return "NAPI_COMP_DONE";

		case EVENT_IRQ:
			return "IRQ";

		case EVENT_NMI:
			return "NMI";

		case EVENT_EXCEPTION:
			return "EXCEPTION";

		case EVENT_IRQ_EXIT:
			return "IRQ_EXIT";

		case EVENT_SOFTIRQ_POLL:
			return "SOFTIRQ_POLL";

		case EVENT_NET_RX_ACTION:
			return "NET_RX_ACTION";

		case EVENT_VMFUNC_TRAMP_ENTRY:
			return "TRAMP_ENTRY";

		case EVENT_VMFUNC_TRAMP_EXIT:
			return "TRAMP_EXIT";

		case EVENT_VMFUNC_SBOARD_KLCD_ENTER:
			return "SBOARD_ENTER";

		case EVENT_VMFUNC_SBOARD_KLCD_LEAVE:
			return "SBOARD_LEAVE";

		case EVENT_DO_PAGE_FAULT:
			return "DO_PAGE_FAULT";

		case EVENT_DO_PAGE_FAULT_LEAVE:
			return "DO_PAGE_FAULT_LEAVE";

		case EVENT_DO_INT3:
			return "DO_INT3";

		case EVENT_DO_INT3_LEAVE:
			return "DO_INT3_LEAVE";

		case EVENT_NMI_LEAVE:
			return "EVENT_NMI_LEAVE";

		case EVENT_NMI_FULL: 
			return "EVENT_NMI_FULL";

		default:
			return "Undefined item";
	}
}

asmlinkage __visible notrace void __add_trace_entry(unsigned type,
		unsigned long rdi, unsigned long rsp, unsigned long eflags,
		unsigned long ip)
{
	int cpu = raw_smp_processor_id();
	unsigned char *ring_buf_base = per_cpu(ring_buffer, cpu);
	struct ring_trace_entry *ring_buf = (struct ring_trace_entry*) ring_buf_base;
	unsigned char head_idx = __this_cpu_read(ring_head);
	struct ring_trace_entry *entry;

	__this_cpu_write(ring_head, (++head_idx % NUM_TRACE_ENTRIES));

	entry = (struct ring_trace_entry*) &ring_buf[head_idx];
	entry->rip = ip;
	entry->rsp = rsp;
	entry->rdi = rdi;
	entry->eflags = eflags;
	entry->lcd_stack = (unsigned long) current->lcd_stack;
	entry->lcd_stack_bit = current->lcd_stack_bit;
	entry->lcd_nc = current->nested_count;
	entry->pid = current->pid;
	entry->gsbase = native_read_msr(MSR_GS_BASE);
	entry->context = (!!in_irq() << IN_IRQ_SHIFT)
				| (!!in_softirq() << IN_SOFTIRQ_SHIFT)
				| (!!in_nmi() << IN_NMI_SHIFT);
	entry->type = type;
	snprintf(entry->name, PROC_NAME_MAX, current->comm);
}

__asm__(
	"	.text		\n\t"
	"	.align 16	\n\t"
	"	.globl add_trace_entry	\n\t"
	"	.type add_trace_entry, @function  \n\t"
	"add_trace_entry:	\n\t"
	/* rdi has event_type */
	/* rsi has the arg rdi */
	"  movq %rsp, %rdx	\n\t"
	"  pushfq		\n\t"
	"  pop %rcx		\n\t"
	"  movq 0(%rsp), %r8	\n\t"
	"  call __add_trace_entry	\n\t"
	"  ret			\n\t"
);
EXPORT_SYMBOL(add_trace_entry);


asmlinkage __visible notrace void add_trace_entry_tf(struct pt_regs *regs,
		unsigned type)
{
	__add_trace_entry(type, regs->di, regs->sp, regs->flags, regs->ip);
}

int __init ring_trace_init(void)
{
	int cpu;
	for_each_online_cpu(cpu) {
		per_cpu(ring_head, cpu) = 0;
	}
	printk("LCD ring trace buffer initialized for %d cpus\n", num_online_cpus());
	return 0;
}
late_initcall(ring_trace_init);

asmlinkage __visible notrace void dump_ring_trace_buffer(void)
{
	unsigned char head_idx = __this_cpu_read(ring_head);
	unsigned char *this_ring = per_cpu(ring_buffer, raw_smp_processor_id());
	struct ring_trace_entry *trace_entries = (struct ring_trace_entry*) this_ring;
	int i;

	printk("head:%d\n", head_idx); 

	for (i = 0; i < NUM_TRACE_ENTRIES; i++, head_idx--) {
		struct ring_trace_entry *entry = &trace_entries[head_idx % NUM_TRACE_ENTRIES];
		if (i == 0)
			printk("head ==> ");
		printk("type:%16s(%d) cpu: %d [%c|%c|%c] comm: %s pid: %d rip: %16lx rsp: %16lx "
				"rdi: %09lx gsbase: %16lx lcd_stack: %16lx[bmap: %x nc:%u] "
				"eflags: %08lx [IF: %d]\n",
				event_type_to_string(entry->type),
				entry->type,
				raw_smp_processor_id(),
				entry->context & (IN_NMI) ? 'N' : '-',
				entry->context & (IN_SOFTIRQ) ? 'S' : '-',
				entry->context & (IN_IRQ) ? 'I' : '-',
				entry->name, entry->pid, entry->rip,
				entry->rsp, entry->rdi, entry->gsbase, entry->lcd_stack,
				entry->lcd_stack_bit, entry->lcd_nc, entry->eflags,
				!!(entry->eflags & IF_FLAG));
	}
}
EXPORT_SYMBOL(dump_ring_trace_buffer);
