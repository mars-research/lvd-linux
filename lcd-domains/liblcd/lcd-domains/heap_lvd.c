/*
 * mem.c
 *
 * Isolated liblcd page allocator and related
 * code.
 */

#include <lcd_config/pre_hook.h>

#include <linux/mm.h>
#include <linux/slab.h>
#include <liblcd/mem.h>
#include <liblcd/allocator.h>
#include <asm/lcd_domains/liblcd.h>
#ifdef CONFIG_LVD
#include <asm/lcd_domains/liblcd_vmfunc.h>
#endif
#include <lcd_domains/liblcd.h>

#include <libfipc.h>

#include <lcd_config/post_hook.h>

/* LOW-LEVEL SYSCALLS -------------------------------------------------- */

struct page *_lvd_alloc_pages_exact_node(int nid, unsigned int flags, unsigned int order,
				cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
	struct fipc_message msg;
	struct page *p;

	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lvd_syscall_alloc_pages_exact_node(&msg, slot, nid, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
		goto fail2;
	}

	p = (struct page*) msg.regs[0];

	*slot_out = slot;

	return p;

fail2:
	lcd_cptr_free(slot);
fail1:
	return NULL;
}

struct page* _lvd_alloc_pages(unsigned int flags, unsigned int order,
		cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
	struct fipc_message msg;
	struct page *p;

	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lvd_syscall_alloc_pages(&msg, slot, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
		goto fail2;
	}

	p = (struct page*) msg.regs[0];

	*slot_out = slot;

	return p;

fail2:
	lcd_cptr_free(slot);
fail1:
	return NULL;
}


void* _lvd_vmalloc(unsigned long nr_pages, cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
	struct fipc_message msg;
	void *addr;

	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lvd_syscall_vmalloc(&msg, slot, nr_pages);
	if (ret) {
		LIBLCD_ERR("vmalloc syscall failed");
		goto fail2;
	}

	addr = (void*) msg.regs[0];

	*slot_out = slot;

	return addr;

fail2:
	lcd_cptr_free(slot);
fail1:
	return NULL;
}


/* PAGE ALLOC INTERFACE ---------------------------------------- */

struct page *lvd_alloc_pages_node(int nid, unsigned int flags,
					unsigned int order)
{
	/*
	 * For now, we ignore the node id (not numa aware).
	 */
	return lvd_alloc_pages(flags, order);
}

struct page *lvd_alloc_pages_exact_node(int nid, unsigned int flags, 
					unsigned int order)
{
	/*
	 * For now, we ignore the node id (not numa aware).
	 */
	return lvd_alloc_pages(flags, order);
}

struct page *lvd_alloc_pages(unsigned int flags, unsigned int order)
{
	cptr_t slot;
	return _lvd_alloc_pages(flags, order, &slot);
}

void lvd_free_pages(struct page *base, unsigned int order)
{
	struct fipc_message msg;
	lvd_syscall_free_pages(&msg, base, order);
}

void* lvd_vmalloc(unsigned long sz)
{
	/* Not implemented for now */
	BUG();
	return NULL;
}

void lvd_vfree(void *ptr)
{
	/* Not implemented for now */
	BUG();
}

struct page *lvd_virt_to_head_page(const void *addr)
{
	return virt_to_head_page(addr);
}

void *lvd_page_address(const struct page *page)
{
	return page_address(page);
}

void lvd_free_memcg_kmem_pages(struct page *p, unsigned int order)
{
	lvd_free_pages(p, order);
}

#if 0
/* VOLUNTEERING -------------------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size, unused;
	return lcd_phys_to_cptr(
		lcd_gva2gpa(__gva((unsigned long)lcd_page_address(base))), 
		slot_out, &size, &unused);
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	return;
}

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size, unused;
	return lcd_phys_to_cptr(base, slot_out, &size, &unused);
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	return;
}

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned long nr_pages,
			cptr_t *slot_out)
{
	unsigned long size, unused;
	return lcd_virt_to_cptr(base, slot_out, &size, &unused);
}

void lcd_unvolunteer_vmalloc_mem(cptr_t vmalloc_mem)
{
	return;
}
#endif

void cpucache_init(void);

static void __ref kmalloc_init(void)
{
	kmem_cache_init();
	kmem_cache_init_late();
	cpucache_init();
}

int __liblvd_heap_init(void)
{
	/*
	 * Initialize kmalloc
	 */
	kmalloc_init();
	/*
	 * Inform mem itree the page and slab allocators are up (and so
	 * it can start using kmalloc for allocating nodes)
	 */
	__liblcd_mem_itree_booted();

	return 0;
}

void *__lvd_get_free_pages(gfp_t mask, unsigned int order)
{
	struct page *p = lvd_alloc_pages(mask, order);
	return p ? lvd_page_address(p) : NULL;
}

void __lvd_free_pages(unsigned long addr, unsigned int order)
{
	if (addr)
		lvd_free_pages(virt_to_page((void*)addr), order);
}
