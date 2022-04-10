#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/desc.h>
#include <asm/irq_vectors.h>
#include <asm/lcd_domains/bflank.h>
#include <linux/slab.h>
#include "marshal_callee.h"

#include <lcd_config/post_hook.h>

extern int callee_main(void);

#define NUM_IST_STACKS	7
//#define CONFIG_DUMP_IRQ_REGS 1
#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

#define ALLOC_SZ	152

//#define TRIGGER_BREAKPOINT

void *hw_addr;
struct perf_event *hwbp;

unsigned long noinline
null_invocation(struct fipc_message *msg)
{
	int i;
	fipc_set_reg1(msg, 0xbad);
	for (i = 0; i < 10; i++) {
		void *rsp_ptr;
		asm volatile("mov %%rsp, %[rsp_ptr]"
			: [rsp_ptr]"=r"(rsp_ptr));

		printk("%s, called, rsp %p", __func__, rsp_ptr);
	}
#ifdef TRIGGER_BREAKPOINT
	if (hw_addr)
		*((unsigned int*)hw_addr) = 0xdead;
#endif
	return 0;
}

int val;

unsigned long noinline
marshal_one(struct fipc_message *msg)
{
	val = fipc_get_reg0(msg);
	return 0;
}

#ifdef CONFIG_DUMP_IRQ_REGS
void dump_tss(struct tss_struct *tss, int cpu)
{
	int i;
	printk("Dumping TSS for cpu %d\n", cpu);

	for (i = 0; i < NUM_IST_STACKS; i++) {
		printk("tss->x86_tss.ist[%d] = %llx", i, tss->x86_tss.ist[i]);
	}
}

void dump_gdt(struct gdt_page *gdt, int cpu)
{
	int i;
	printk("--> Dumping GDT for cpu %d<--\n", cpu);

	for (i = 0; i < GDT_ENTRIES; i++) {
		printk("gdt->gdt[%d] = %x:%x base: %lx", i,
				gdt->gdt[i].a,
				gdt->gdt[i].b,
				(unsigned long) gdt->gdt[i].base0 | (gdt->gdt[i].base1 << 16) | (gdt->gdt[i].base2 << 24));
	}
}

void dump_idt(struct gate_struct64 *idt)
{
	int i;
	printk("--> Dumping IDT <--\n");

	for (i = 0; i < NR_VECTORS; i++) {
		printk("idt_base[%d].offset = %lx", i,
				(unsigned long) idt[i].offset_low | (unsigned long)(idt[i].offset_middle << 16) | (unsigned long)((unsigned long)idt[i].offset_high << 32));
	}
}

void write_exception_stack(void *base, unsigned long sz)
{
	int i = 0;
	int num_pages = sz >> PAGE_SHIFT;

	for (i = 0; i < num_pages; i++) {
		/* touch every page */
		*((unsigned long*)(base - i * PAGE_SIZE)) = 0xaabbccdd + i;
		/* read back */
		printk("data @ %p = %lx", base - i * PAGE_SIZE, *((unsigned long*)(base - i * PAGE_SIZE)));
	}
}
#endif

unsigned long noinline
foo(struct fipc_message *msg)
{
	struct fipc_message kmsg;
	int ret = 0;
#ifdef CONFIG_DUMP_IRQ_REGS
	void *lvd_stack;
	struct tss_struct *tss;
	struct gdt_page *gdt;
	struct gate_struct64 *idt_base;

	unsigned long sz;
	int cpu;

	INIT_FIPC_MSG(&kmsg);

	cpu = fipc_get_reg5(msg);

	printk("====== cpu %d ========", cpu);

	idt_base = (struct gate_struct64 *) fipc_get_reg0(msg);
	printk("%s, idt_table %p", __func__, idt_base);
	dump_idt(idt_base);

	tss = (struct tss_struct *) fipc_get_reg1(msg);
	printk("%s, cpu_tss %p", __func__, tss);
	dump_tss(tss, cpu);

	gdt = (struct gdt_page *) fipc_get_reg2(msg);
	printk("%s, gdt_page %p", __func__, gdt);
	dump_gdt(gdt, cpu);

	lvd_stack = (void*) fipc_get_reg3(msg);
	printk("%s, lvd_stack %p", __func__, lvd_stack);
	sz = fipc_get_reg4(msg);

	write_exception_stack(lvd_stack, sz);
#endif

#if 1
	{
		int i = 0;
		void *rsp_ptr;
		unsigned long rsp_top;
		u64 s_exits = bfcall_dump_perf(), e_exits; 
		u64 start = rdtsc(), end;

		int num_iterations = 1000000;

		for(i = 0; i < num_iterations; i++) {
			//printk("rsp before int 0xf3 %p", rsp_ptr);

			asm volatile("int $0xf3");

			//asm volatile("mov %%rsp, %[rsp_ptr]"
			//		: [rsp_ptr]"=r"(rsp_ptr));
			//printk("rsp before int 0xf3 %p", rsp_ptr);
		}
		end = rdtsc();

		e_exits = bfcall_dump_perf(); 

		printk("%d iterations of int 0xf3 back-to-back took %llu cycles (avg: %llu cycles, total exits:%llu)\n",
				num_iterations, end - start, (end - start) / num_iterations, e_exits - s_exits);

		for(i = 0; i < 10; i++) {
			unsigned long ecx = 0xc0000101, edx, eax;

			asm volatile("movq %0, %%rcx \t\n"
				     "rdmsr         \t\n"
					: "=a" (eax),
	     				  "=d" (edx),
			      		  "=c" (ecx)
					: "0" (ecx) : "memory");

		}

		i = 0;
		do {
			asm volatile("nop");
			i++;
		} while (i < 1000000000);

		asm volatile("mov %%rsp, %[rsp_ptr]" : [rsp_ptr]"=r"(rsp_ptr));

		rsp_top = PGROUNDUP((unsigned long)rsp_ptr);

		{
			void *cur = rsp_ptr;
			printk("stack dump:\nbase: %lx", rsp_top);

			while ((unsigned long) cur < rsp_top) {
				printk("%p: %lx", cur, *((unsigned long*)cur));
				cur += sizeof(void *);
			}
		}
		// Touch NULL to trigger a page fault
		//asm volatile ("movq %rax, 0x0");

	}
#endif
	kmsg.vmfunc_id = VMFUNC_RPC_CALL;
	kmsg.rpc_id = BAR;

	fipc_set_reg1(&kmsg, 0xaa);
	fipc_set_reg2(&kmsg, 0xbb);
	fipc_set_reg3(&kmsg, 0xcc);
	fipc_set_reg4(&kmsg, 0xdd);
	fipc_set_reg5(&kmsg, 0xee);
	fipc_set_reg6(&kmsg, 0xff);

	vmfunc_wrapper(&kmsg);

	ret = fipc_get_reg1(&kmsg);
	printk("%s,got ret val in reg1 = %x. expected = 0x100", __func__, ret);
	fipc_set_reg1(msg, 0xaa + ret);
	fipc_set_reg2(msg, 0xbb + ret);
	fipc_set_reg3(msg, 0xcc + ret);
	fipc_set_reg4(msg, 0xdd + ret);
	fipc_set_reg5(msg, 0xee + ret);
	fipc_set_reg6(msg, 0xff + ret);

	return 0;
}

int memtest(struct fipc_message *msg)
{
	int pages = 0;
	printk("%s, Running memory exhaustion test", __func__);
	do {
		struct page *p;
		p = lcd_alloc_pages(GFP_KERNEL, 0);
		printk("%s, page %d : %p", __func__, ++pages, p);
		if (!p) {
			LIBLCD_ERR("Page alloc failed at %d pages", pages);
			break;
		}
	} while (1);
	return 0;
}

void irq_handler(struct fipc_message *msg)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	r.vmfunc_id = VMFUNC_RPC_CALL;
	r.rpc_id = NULL_INVOCATION;

	printk("%s called", __func__);
	vmfunc_wrapper(request);
}

struct perf_event *_register_wide_hw_breakpoint(struct perf_event_attr *attr,
			    perf_overflow_handler_t triggered,
			    void *context)
{
	int ret;
	struct perf_event *func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	r.vmfunc_id = VMFUNC_RPC_CALL;
	r.rpc_id = REGISTER_WIDE_HW_BREAKPOINT;

	fipc_set_reg1(_request, attr->bp_len);
	fipc_set_reg2(_request, attr->bp_type);
	fipc_set_reg3(_request, attr->bp_addr);

	ret = vmfunc_wrapper(_request);
	func_ret = (struct perf_event *) fipc_get_reg0(_request);

	return func_ret;
}

void _unregister_wide_hw_breakpoint(struct perf_event *hwbp)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	r.vmfunc_id = VMFUNC_RPC_CALL;
	r.rpc_id = UNREGISTER_WIDE_HW_BREAKPOINT;

	fipc_set_reg0(_request, (unsigned long)hwbp);

	vmfunc_wrapper(_request);
}

void create_hwbp(void)
{
	hw_addr = (void*) kmalloc(ALLOC_SZ, GFP_KERNEL);

	if (hw_addr) {
		struct perf_event_attr attr;
		attr.bp_addr = (unsigned long) hw_addr;
		attr.bp_len = HW_BREAKPOINT_LEN_8;
		attr.bp_type = HW_BREAKPOINT_W;

		hwbp = _register_wide_hw_breakpoint(&attr, NULL, NULL);

		//*((unsigned int*)hw_addr) = 0xdeadbeef;
	}
}

#define HEAP_NUM_PAGES		8192

struct page *pages[HEAP_NUM_PAGES] = {0};

void alloc_free_pages(void)
{
	int i;

	for (i = 0; i < HEAP_NUM_PAGES; i++) {
		pages[i] = lcd_alloc_pages(GFP_KERNEL, 0);
		if (!(i % 100))
			printk("%s allocate %d pages", __func__, i);
		if (!pages[i]) {
			printk("%s allocation failed while alloc-ing %d th page", __func__, i);
			break;
		}
	}

	for (; i >= 0; i--) {
		if (pages[i])
			lcd_free_pages(pages[i], 0);
	}
}

#define DHEAP_NUM_PAGES		8192

struct page *dheap_pages[DHEAP_NUM_PAGES] = {0};
void alloc_dheap_pages(void)
{
	int i;

	for (i = 0; i < DHEAP_NUM_PAGES; i++) {
		dheap_pages[i] = lcd_dheap_alloc_pages(GFP_KERNEL, 0);
		if (!(i % 100))
			printk("%s allocate %d pages", __func__, i);
		if (!dheap_pages[i]) {
			printk("%s allocation failed while alloc-ing %d th page", __func__, i);
			break;
		}
	}

	return;
}

extern void __init_globals(void);

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case MODULE_INIT:
		__init_globals();
		callee_main();
		//create_hwbp();
		//alloc_free_pages();
		//alloc_dheap_pages();
		break;
	case NULL_INVOCATION:
		null_invocation(msg);
		break;
	case MARSHAL_ONE:
		marshal_one(msg);
		//if (hwbp)
		//	_unregister_wide_hw_breakpoint(hwbp);
		break;
	case FOO:
		foo(msg);
		break;
	case CALLEE:
		callee(msg);
		break;
	case MEMTEST:
		memtest(msg);
		break;
	case IRQ_HANDLER:
		irq_handler(msg);
		break;
	case MARSHAL_INT:
		marshal_int_callee(msg);
		break;	
	case MARSHAL_ARRAY:
		marshal_array_callee(msg);
		break;
	case MARSHAL_STRING:
		marshal_string_callee(msg);
		break;
	case MARSHAL_VOIDP:
		marshal_voidptr_callee(msg);
		break;
	case MARSHAL_UNION:
		marshal_union_callee(msg);
		break;
	default:
		break;
	};
	return 0;
}
