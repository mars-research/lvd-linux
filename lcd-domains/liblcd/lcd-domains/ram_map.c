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

#include <lcd_config/post_hook.h>

struct lcd_page_allocator *ram_map_allocator;

/* LOW-LEVEL SYSTEM CALLS ---------------------------------------- */

int _lcd_mmap(cptr_t mo, unsigned int order, gpa_t base)
{
	int ret;
	/*
	 * Do low level syscall to map memory object
	 */
	ret = lcd_syscall_mmap(mo, base);
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

void _lcd_munmap(cptr_t mo, gpa_t base)
{
	int ret;
	struct lcd_resource_node *n;
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
	lcd_syscall_munmap(mo);
}

/* RAM MAP INTERNALS ---------------------------------------- */

static int 
ram_alloc_map_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
			unsigned int alloc_order,
			unsigned long metadata_sz,
			void **metadata_addr)
{
	int ret;
	/*
	 * Just do a kmalloc
	 */
	*metadata_addr = kmalloc(metadata_sz, GFP_KERNEL);
	if (!*metadata_addr) {
		LIBLCD_ERR("error getting metadata mem for ram map");
		ret = -ENOMEM;
		goto fail1;
	}
	return 0;

fail1:
	return ret;
}

static void
ram_free_unmap_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
			void *metadata_addr,
			unsigned long metadata_sz,
			unsigned int alloc_order)
{
	/*
	 * We alloc'd metadata with kmalloc (above)
	 */
	kfree(metadata_addr);
}

static inline gva_t ram_page_block_to_addr(struct lcd_page_block *pb)
{
	return gva_add(LCD_RAM_MAP_GV_ADDR, 
		lcd_page_block_to_offset(ram_map_allocator, pb));
}

static inline struct lcd_page_block *ram_addr_to_page_block(gva_t addr)
{
	return lcd_offset_to_page_block(
		ram_map_allocator,
		gva_val(addr) - gva_val(LCD_RAM_MAP_GV_ADDR));
}

/* INTERFACE -------------------------------------------------- */

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
	/*
	 * Insert address range into tree so we can do address -> cptr
	 * translation.
	 */
	ret = __liblcd_mem_itree_insert(
		addr,
		(1UL << (order + PAGE_SHIFT)),
		pages);
	if (ret) {
		LIBLCD_ERR("failed to insert in mem itree");
		goto fail3;
	}

	*base_out = addr;

	return 0;

fail3:
	_lcd_munmap(pages, addr);
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
	 * Resolve address to resource node
	 */
	ret = lcd_phys_to_resource_node(base, &n);
	if (ret) { 
		LIBLCD_ERR("error looking up resource node");
		return;
	}
	pages = n->cptr;
	/*
	 * Remove from tree
	 */
	__liblcd_mem_itree_delete(n);
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

/* INIT/EXIT ---------------------------------------- */

struct lcd_page_allocator_cbs ram_map_page_allocator_cbs = {
	.alloc_map_metadata_memory = ram_alloc_map_metadata_memory,
	.free_unmap_metadata_memory = ram_free_unmap_metadata_memory,
};

int __liblcd_ram_map_init(void)
{
	int ret;
	/*
	 * Create the RAM map allocator
	 */
	ret = lcd_page_allocator_create(LCD_RAM_MAP_NR_PAGES_ORDER,
					LCD_RAM_MAP_MIN_ORDER,
					LCD_RAM_MAP_MAX_ORDER,
					&ram_map_page_allocator_cbs,
					0, /* don't embed metadata */
					&ram_map_allocator);
	if (ret) {
		LIBLCD_ERR("failed to initialize RAM map allocator");
		goto fail1;
	}

	return 0;

fail1:
	return ret;
}
