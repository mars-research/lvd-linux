#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/lcd_domains/libvmfunc.h>
/* percpu stack pages and stack pointers */
static void **stack_pages;
static void **stack_ptrs;

/* percpu response buffers */
struct fipc_message **responses;

/* exported by the microkernel. We trust that it's sane */
extern void *cpuid_page;

rpc_handler_t klcd_handler;

int
vmfunc_init(void *stack_page, rpc_handler_t rpc_handler)
{
	/* TODO: this num_online_cpus macro accesses __cpu_online_mask
	 * Make sure we map this page on LCDs EPT
	 */
	stack_pages = kmalloc(sizeof(void*) * num_online_cpus(), GFP_KERNEL);
	stack_ptrs = kmalloc(sizeof(void*) * num_online_cpus(), GFP_KERNEL);
	stack_pages[0] = stack_page;
	stack_ptrs[0] = stack_pages[0];
	current->lcd_stack = stack_page;
	klcd_handler = rpc_handler;
#if 0
	int i, j;
	for (i = 0; i < num_online_cpus(); i++) {
		stack_pages[i] = kmalloc(PAGE_SIZE * 2, GFP_KERNEL);
		/* store stack pointers */
		stack_ptrs[i] = stack_pages[i];
		if (!stack_pages[i])
			goto clean;
	}

	responses = kmalloc(sizeof(struct fipc_message*) * num_online_cpus(), GFP_KERNEL);
	for (j = 0; j < num_online_cpus(); j++) {
		responses[j] = kmalloc(sizeof(struct fipc_message), GFP_KERNEL);
		if (!responses[j])
			goto clean2;
	}

	return 0;

clean2:
	for (; j >=0; j--)
		kfree(responses[j]);
clean:
	for (; i >=0; i--)
		kfree(stack_pages[i]);

	return -ENOMEM;
#endif
	return 0;
}
EXPORT_SYMBOL(vmfunc_init);

void
vmfunc_exit(void)
{
	int i = 0;

	for (; i < num_online_cpus(); i++) {
		if (stack_pages[i])
			kfree(stack_pages[i]);
	}
#if 0
	for (; i < num_online_cpus(); i++)
		kfree(responses[i]);
#endif
}

