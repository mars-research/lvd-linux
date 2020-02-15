#include <asm/msr.h>
#include <asm/ptrace.h>
#include <asm/current.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/lcd_trace.h>

DEFINE_PER_CPU_PAGE_ALIGNED(struct ring_trace_buffer, ring_buffer) = {
	.header.size = NUM_TRACE_ENTRIES,
	.header.head = 0,
};

EXPORT_PER_CPU_SYMBOL(ring_buffer);

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

		case EVENT_CTX_SWITCH:
			return "EVENT_CTX_SWITCH";

		default:
			return "Undefined item";
	}
}

asmlinkage __visible notrace void __add_trace_entry(unsigned type,
		unsigned long rdi, unsigned long rsp, unsigned long eflags,
		unsigned long ip)
{
	int cpu = raw_smp_processor_id();
	struct ring_trace_buffer *ring_buf = &per_cpu(ring_buffer, cpu);
	struct ring_trace_entry *entry;
	unsigned long idx = ring_buf->header.head;
       	
	ring_buf->header.head += 1; 

	entry = (struct ring_trace_entry*) &ring_buf->entries[idx % ring_buf->header.size];
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
	entry->ts = rdtsc();
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
//	int cpu;
//	for_each_possible_cpu(cpu) {
//		struct ring_trace_buffer *ring_buf = &per_cpu(ring_buffer, cpu);
//		ring_buf->header.head = 0; 
//		ring_buf->header.size = NUM_TRACE_ENTRIES;
//	}
	printk("LCD ring trace buffer initialized for %d cpus\n", num_online_cpus());
	return 0;
}
late_initcall(ring_trace_init);

asmlinkage __visible notrace void dump_ring_trace_buffer(void)
{
	struct ring_trace_buffer *ring_buf = this_cpu_ptr(&ring_buffer);
	unsigned long idx = ring_buf->header.head;

	int i;

	printk("head:%ld\n", idx);
	idx--;

	for (i = 0; i < ring_buf->header.size; i++, idx--) {
		struct ring_trace_entry *entry = &ring_buf->entries[idx % ring_buf->header.size];
		if (i == 0)
			printk("head ==> ");
		printk("type:%16s(%d) ts:%llu cpu: %d [%c|%c|%c] comm: %s pid: %d rip: %16lx rsp: %16lx "
				"rdi: %09lx gsbase: %16lx lcd_stack: %16lx[bmap: %x nc:%u] "
				"eflags: %08lx [IF: %d]\n",
				event_type_to_string(entry->type),
				entry->type,
				entry->ts,
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

asmlinkage __visible notrace void dump_ring_trace_buffer_cpu(int cpu)
{
	struct ring_trace_buffer *ring_buf = per_cpu_ptr(&ring_buffer, cpu);
	unsigned long idx = ring_buf->header.head;

	int i;

	printk("head:%ld\n", idx);
	idx--;

	for (i = 0; i < ring_buf->header.size; i++, idx--) {
		struct ring_trace_entry *entry = &ring_buf->entries[idx % ring_buf->header.size];
		if (i == 0)
			printk("head ==> ");
		printk("type:%16s(%d) ts:%llu cpu: %d [%c|%c|%c] comm: %s pid: %d rip: %16lx rsp: %16lx "
				"rdi: %09lx gsbase: %16lx lcd_stack: %16lx[bmap: %x nc:%u] "
				"eflags: %08lx [IF: %d]\n",
				event_type_to_string(entry->type),
				entry->type,
				entry->ts,
				cpu,
				entry->context & (IN_NMI) ? 'N' : '-',
				entry->context & (IN_SOFTIRQ) ? 'S' : '-',
				entry->context & (IN_IRQ) ? 'I' : '-',
				entry->name, entry->pid, entry->rip,
				entry->rsp, entry->rdi, entry->gsbase, entry->lcd_stack,
				entry->lcd_stack_bit, entry->lcd_nc, entry->eflags,
				!!(entry->eflags & IF_FLAG));
	}
}
EXPORT_SYMBOL(dump_ring_trace_buffer_cpu);
