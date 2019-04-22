#include <asm/ptrace.h>
#include <asm/current.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/lcd_trace.h>

DEFINE_PER_CPU_PAGE_ALIGNED(unsigned char, ring_buffer[RING_BUFFER_SIZE]);
DEFINE_PER_CPU(unsigned char, ring_head);

#define NUM_TRACE_ENTRIES	(RING_BUFFER_SIZE / sizeof(struct ring_trace_entry))

char *trace_event_type_string[] = {
	"xmit",
	"msix_hndlr",
	"napi_comp_done",
	"irq",
	"nmi",
	"exception",
	"softirq_rx_poll",
	"net_rx_action"
};

asmlinkage __visible notrace void __add_trace_entry(trace_event_t type,
		unsigned long rdi, unsigned long rsp, unsigned long eflags,
		unsigned long ip)
{
	int cpu = raw_smp_processor_id();
	unsigned char *ring_buf_base = per_cpu(ring_buffer, cpu);
	struct ring_trace_entry *ring_buf = (struct ring_trace_entry*) ring_buf_base;
	unsigned char head_idx = __this_cpu_read(ring_head);
	struct ring_trace_entry *entry = (struct ring_trace_entry*) &ring_buf[head_idx];
	entry->rip = ip;
	entry->rsp = rsp;
	entry->rdi = rdi;
	entry->eflags = eflags;
	entry->lcd_stack = (unsigned long) current->lcd_stack;
	entry->lcd_stack_bit = current->lcd_stack_bit;
	entry->pid = current->pid;
	entry->context = (!!in_irq() << IN_IRQ_SHIFT)
				| (!!in_softirq() << IN_SOFTIRQ_SHIFT)
				| (!!in_nmi() << IN_NMI_SHIFT);
	entry->type = type;
	snprintf(entry->name, PROC_NAME_MAX, current->comm);
	__this_cpu_write(ring_head, (++head_idx % NUM_TRACE_ENTRIES));
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
		trace_event_t type)
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

void dump_ring_trace_buffer(void)
{
	unsigned char head_idx = __this_cpu_read(ring_head);
	unsigned char *this_ring = per_cpu(ring_buffer, raw_smp_processor_id());
	struct ring_trace_entry *trace_entries = (struct ring_trace_entry*) this_ring;
	int i;

	for (i = 0; i < NUM_TRACE_ENTRIES; i++, head_idx--) {
		struct ring_trace_entry *entry = &trace_entries[head_idx % NUM_TRACE_ENTRIES];
		if (i == 0)
			printk("head ==> ");
		printk("type:%16s cpu: %d [%c|%c|%c] comm: %s pid: %d rip: %08lx rsp: %08lx "
				"rdi: %08lx lcd_stack: %08lx[esp_lcd_bmap: %x] "
				"eflags: %lx [IF: %d]\n",
				trace_event_type_string[entry->type - 1],
				raw_smp_processor_id(),
				entry->context & (IN_NMI) ? 'N' : '-',
				entry->context & (IN_SOFTIRQ) ? 'S' : '-',
				entry->context & (IN_IRQ) ? 'I' : '-',
				entry->name, entry->pid, entry->rip,
				entry->rsp, entry->rdi, entry->lcd_stack,
				entry->lcd_stack_bit, entry->eflags,
				!!(entry->eflags & IF_FLAG));
	}
}
EXPORT_SYMBOL(dump_ring_trace_buffer);
