/*
 * ram_map.c
 *
 * Code for mapping RAM memory objects in LCD's address
 * spaces.
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <liblcd/allocator.h>
#include <liblcd/mem.h>
#include <asm/lcd_domains/liblcd.h>
#include <lcd_domains/liblcd.h>
#include <libfipc.h>

#ifdef CONFIG_LVD
#include <asm/lcd_domains/liblcd_vmfunc.h>
#endif

#include <lcd_config/post_hook.h>


/* LOW-LEVEL SYSTEM CALLS ---------------------------------------- */

int _lvd_mmap(cptr_t mo, unsigned int order, gpa_t base)
{
	int ret;
	struct fipc_message msg;
	/*
	 * BEWARE: This code is a bit fragile. You must do the actual
	 * map *before* inserting into the memory interval tree. This
	 * is because the mem itree code uses kmalloc (after we've
	 * booted). Scenario: the heap is calling this function to map
	 * fresh pages; mem itree is going to call kmalloc before
	 * this function returns; kmalloc may need to grow the slab
	 * cache, which leads into the heap again; but if we've alloc'd
	 * and mapped the pages, it should all be OK. Just a bit
	 * of scary and risky recursion.
	 *
	 * Do low level syscall to map memory object
	 */
	ret = lvd_syscall_mmap(&msg, mo, base);
	if (ret) {
		LIBLCD_ERR("low level mmap failed");
		goto fail1;
	}
	/*
	 * Insert into resource tree (unlike kliblcd, all of our
	 * memory objects are always contiguous in guest physical)
	 */
	ret = __liblcd_mem_itree_insert(base, (1UL << (PAGE_SHIFT + order)),
					mo);
	if (ret) {
		LIBLCD_ERR("error inserting into mem itree");
		goto fail2;
	}

	return 0;

fail2:
	lcd_syscall_munmap(mo);
fail1:
	return ret;
}

void _lvd_munmap(cptr_t mo, gpa_t base)
{
	int ret;
	struct lcd_resource_node *n;
	struct fipc_message msg;
	/*
	 * Look up resource node for memory object in itree
	 */
	ret = lcd_phys_to_resource_node(base, &n);
	if (ret) {
		LIBLCD_ERR("couldn't find memory object in tree; base gpa is 0x%lx",
			gpa_val(base));
		return;
	}
	/*
	 * Remove from tree
	 */
	__liblcd_mem_itree_delete(n);
	/*
	 * Unmap memory object
	 */
	lvd_syscall_munmap(&msg, mo);
}

/* INTERFACE -------------------------------------------------- */
#if 0
static int do_map_into_phys(cptr_t pages, unsigned int order, gpa_t *base_out)
{
	int ret;
	struct lcd_page_block *pb;
	gpa_t addr;
	/*
	 * Adjust order so that it's >= LCD_RAM_MAP_MIN_ORDER
	 */
	if (order < LCD_RAM_MAP_MIN_ORDER)
		order = LCD_RAM_MAP_MIN_ORDER;
	/*
	 * Try to alloc a block of phys address space
	 */
	pb = lcd_page_allocator_alloc(ram_map_allocator, order);
	if (!pb) {
		LIBLCD_ERR("failed to get free ram map region");
		ret = -ENOMEM;
		goto fail1;
	}
	addr = lcd_gva2gpa(ram_page_block_to_addr(pb));
	/*
	 * Got some; map in guest physical
	 */
	ret = _lcd_mmap(pages, order, addr);
	if (ret) {
		LIBLCD_ERR("failed to map in guest physical");
		goto fail2;
	}

	*base_out = addr;

	return 0;

fail2:
	lcd_page_allocator_free(ram_map_allocator, pb, order);
fail1:
	return ret;
}

int lcd_map_phys(cptr_t pages, unsigned int order, gpa_t *base_out)
{
	return do_map_into_phys(pages, order, base_out);
}

int lcd_map_virt(cptr_t pages, unsigned int order, gva_t *gva_out)
{
	gpa_t gpa;
	int ret;

	ret = do_map_into_phys(pages, order, &gpa);
	if (ret)
		return ret;
	*gva_out = lcd_gpa2gva(gpa);

	return 0;
}

static void do_unmap_from_phys(gpa_t base, unsigned int order)
{
	int ret;
	struct lcd_resource_node *n;
	cptr_t pages;
	/*
	 * Adjust order so that it's >= LCD_RAM_MAP_MIN_ORDER
	 */
	if (order < LCD_RAM_MAP_MIN_ORDER)
		order = LCD_RAM_MAP_MIN_ORDER;
	/*
	 * Resolve address to resource node
	 */
	ret = lcd_phys_to_resource_node(base, &n);
	if (ret) { 
		LIBLCD_ERR("error looking up resource node");
		return;
	}
	pages = n->cptr;
	/*
	 * Free address block from RAM region
	 */
	lcd_page_allocator_free(
		ram_map_allocator,
		ram_addr_to_page_block(lcd_gpa2gva(base)),
		order);
	/*
	 * Unmap from guest physical
	 */
	_lcd_munmap(pages, base);
}

void lcd_unmap_phys(gpa_t base, unsigned int order)
{
	do_unmap_from_phys(base, order);
}

void lcd_unmap_virt(gva_t base, unsigned int order)
{
	gpa_t gp_base = lcd_gva2gpa(base);
	do_unmap_from_phys(gp_base, order);
}

void *lcd_ioremap_cache(unsigned long phys_addr, unsigned long size)
{
	/* 
	 * Assumes someone already mapped the memory; we just
	 * translate phys -> virt.
	 */
	return (void *)gva_val(isolated_lcd_gpa2gva(__gpa(phys_addr)));
}

void lcd_iounmap(void *addr)
{
	/*
	 * No-op. Someone (like the glue) will call a lower-level
	 * phys unmap.
	 */
	return;
}
#endif
int __liblvd_ram_map_init(void)
{
	return 0;
}
