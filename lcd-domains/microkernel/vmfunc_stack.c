#include <linux/compiler.h>
#include <liblcd/liblcd.h>
#include <linux/vmalloc.h>

void *cpuid_page;
EXPORT_SYMBOL(cpuid_page);

static int num_pages;
static void **cpuid_pages;

int
create_cpuid_pages(void)
{
	int i = 0;
	unsigned int num_cpus = num_online_cpus();
	gpa_t cpuid_base;
	hpa_t *cpuid_hpas;

	cpuid_pages = vmalloc(sizeof(void*) * num_cpus);
	cpuid_hpas = vmalloc(sizeof(hpa_t) * num_cpus);

	for(; i < num_cpus; i++) {
		cpuid_pages[i] = vmalloc(PAGE_SIZE);
		if (!cpuid_pages[i])
			printk("Error allocating memory\n");
		/* write cpuid on top of the page */
		*(unsigned long*)(cpuid_pages[i]) = i;
		/* TODO: Make this page read only */
		cpuid_hpas[i] = __hpa(__pa(cpuid_pages[i]));
	}
	/* for simplicity, pick the first page's gpa as the gpa for all EPTs */
	cpuid_base = __gpa(virt_to_phys(cpuid_pages[0]));
	cpuid_page = cpuid_pages[0];
	num_pages = num_cpus;
	//vmm_add_cpuid_pages(cpuid_base, cpuid_hpas, num_cpus);
	return 0;
}

void
destroy_cpuid_pages(void)
{
	int i = 0;
	for (; i < num_pages; i++) {
		vfree(cpuid_pages[i]);
	}
}
