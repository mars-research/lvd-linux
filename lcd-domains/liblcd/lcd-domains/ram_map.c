/*
 * ram_map.c
 *
 * Code for mapping RAM memory objects in LCD's address
 * spaces.
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/allocator.h>
#include <liblcd/mem.h>

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
		LIBLCD_ERR("couldn't find memory object in tree");
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
ram_alloc_map_metadata_memory_chunk(struct lcd_page_allocator_cbs *cbs,
				unsigned long mapping_offset,
				unsigned int alloc_order,
				
				struct lcd_resource_node **n_out)
{
	int ret;
	cptr_t pages;
	static int nr_calls = 0; /* should only be called once */
	gpa_t dest = gpa_add(LCD_RAM_MAP_GP_ADDR, mapping_offset);
	/*
	 * Just do a kmalloc
	 */




	/*
	 * Do low-level page alloc out into microkernel
	 */
	ret = _lcd_alloc_pages(0, alloc_order, &pages);
	if (ret) {
		LIBLCD_ERR("low level alloc failed");
		goto fail1;
	}
	/*
	 * Map in guest physical at the right offset into the heap region
	 */
	ret = _lcd_mmap(pages, alloc_order, dest);
	if (ret) {
		LIBLCD_ERR("low level mmap failed");
		goto fail2;
	}
	/*
	 * Look up resource node for new pages
	 */
	ret = lcd_phys_to_resource_node(dest, n_out);
	if (ret) {
		LIBLCD_ERR("failed to get new resource node");
		goto fail3;
	}

	return 0;

fail3:
	_lcd_munmap(pages, dest);
fail2:
	lcd_cap_delete(pages);
fail1:
	return ret;
}

static void
heap_free_unmap_metadata_memory_chunk(struct lcd_page_allocator_cbs *cbs,
				struct lcd_resource_node *n_to_delete)
{
	cptr_t pages = n_to_delete->cptr;
	gpa_t base = __gpa(lcd_resource_node_start(n_to_delete));
	/*
	 * Unmap from guest physical
	 */
	_lcd_munmap(pages, base);
	/*
	 * Remove from resource tree
	 */
	__liblcd_mem_itree_delete(n_to_delete);
	/*
	 * Free pages from host
	 */
	lcd_cap_delete(pages);
}

static int 
heap_alloc_map_regular_mem_chunk(struct lcd_page_allocator *pa,
				struct lcd_page_block *dest_blocks,
				unsigned long mapping_offset,
				unsigned int alloc_order,
				struct lcd_resource_node **n_out)
{
	/*
	 * For now, we just re-use the metadata func, since there
	 * is no difference.
	 */
	return heap_alloc_map_metadata_memory_chunk(&pa->cbs, mapping_offset,
						alloc_order, n_out);
}

static void
heap_free_unmap_regular_mem_chunk(struct lcd_page_allocator *pa,
				struct lcd_page_block *page_blocks,
				struct lcd_resource_node *n_to_delete,
				unsigned long mapping_offset,
				unsigned int order)
{
	/*
	 * Again, we re-use the metadata funcs since they're the same
	 * right now.
	 */
	heap_free_unmap_metadata_memory_chunk(&pa->cbs, n_to_delete);
}

static inline gva_t heap_page_block_to_addr(struct lcd_page_block *pb)
{
	return gva_add(LCD_HEAP_GV_ADDR, 
		lcd_page_block_to_offset(heap_allocator, pb));
}

static inline struct lcd_page_block *heap_addr_to_page_block(gva_t addr)
{
	return lcd_offset_to_page_block(
		heap_allocator,
		gva_val(addr) - gva_val(LCD_HEAP_GV_ADDR));
}

static inline struct page *heap_addr_to_struct_page(gva_t addr)
{
	unsigned long idx;
	idx = (gva_val(addr) - gva_val(LCD_HEAP_GV_ADDR)) >> PAGE_SHIFT;
	return &heap_page_array[idx];
}

static inline gva_t heap_struct_page_to_addr(struct page *p)
{
	unsigned long idx;
	idx = p - heap_page_array;
	return gva_add(LCD_HEAP_GV_ADDR, idx * PAGE_SIZE);
}

static inline struct page *heap_page_block_to_struct_page(
	struct lcd_page_block *pb)
{
	return heap_addr_to_struct_page(
		heap_page_block_to_addr(pb));
}

static inline struct lcd_page_block *heap_struct_page_to_page_block(
	struct page *p)
{
	return heap_addr_to_page_block(
		heap_struct_page_to_addr(p));
}

/* INIT/EXIT ---------------------------------------- */

struct lcd_page_allocator_cbs ram_map_page_allocator_cbs = {
	.alloc_map_metadata_memory_chunk = ram_alloc_map_metadata_memory_chunk,
	.free_unmap_metadata_memory_chunk = ram_free_unmap_metadata_memory_chunk,
};

int __liblcd_ram_map_init(void)
{
	int ret;
	/*
	 * Create the RAM map allocator
	 */
	ret = lcd_page_allocator_create(LCD_RAM_MAP_NR_PAGES_ORDER,
					LCD_RAM_MAP_MIN_ORDER,
					1, /* 2^1 = 2 pages */
					LCD_RAM_MAP_MAX_ORDER,
					&ram_map_page_allocator_cbs,
					0, /* don't embed metadata */
					&ram_map_allocator);
	if (ret) {
		LIBLCD_ERR("failed to initialize RAM map allocator");
		goto fail1;
	}
	
	

}
