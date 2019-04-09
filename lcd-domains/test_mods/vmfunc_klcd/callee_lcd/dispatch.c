#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/desc.h>
#include <asm/irq_vectors.h>

#include <lcd_config/post_hook.h>

extern int callee_main(void);

#define NUM_IST_STACKS	7
//#define CONFIG_DUMP_IRQ_REGS 1
#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

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
	struct fipc_message kmsg = {0};
	int ret = 0;
#ifdef CONFIG_DUMP_IRQ_REGS
	void *lvd_stack;
	struct tss_struct *tss;
	struct gdt_page *gdt;
	struct gate_struct64 *idt_base;

	unsigned long sz;
	int cpu;

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

		for(i = 0; i < 100; i++) {
			asm volatile("mov %%rsp, %[rsp_ptr]"
					: [rsp_ptr]"=r"(rsp_ptr));
			printk("rsp before int 0xf3 %p", rsp_ptr);

			asm volatile("int $0xf3");

			asm volatile("mov %%rsp, %[rsp_ptr]"
					: [rsp_ptr]"=r"(rsp_ptr));
			printk("rsp before int 0xf3 %p", rsp_ptr);
		}
		
		i = 0;
		do {
			asm volatile("nop");
			i++;
		} while (i < 1000000000);

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

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case MODULE_INIT:
		callee_main();
		break;
	case NULL_INVOCATION:
		null_invocation(msg);
		break;
	case FOO:
		foo(msg);
		break;
	case CALLEE:
		callee(msg);
		break;
	default:
		break;
	};
	return 0;
}
