/*
 * mem.c
 *
 * Isolated liblcd page allocator and related
 * code.
 */

#include <lcd_config/pre_hook.h>

#include <linux/mm.h>
#include <liblcd/mem.h>
#include <liblcd/allocator.h>

#include <lcd_config/post_hook.h>

struct lcd_page_allocator *heap_allocator;
struct page *heap_page_array;

/* LOW-LEVEL SYSCALLS -------------------------------------------------- */

int _lcd_alloc_pages_exact_node(int nid, unsigned int flags, unsigned int order,
				cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
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
	ret = lcd_syscall_alloc_pages_exact_node(nid, flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
		goto fail2;
	}

	*slot_out = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int _lcd_alloc_pages(unsigned int flags, unsigned int order,
		cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
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
	ret = lcd_syscall_alloc_pages(flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
		goto fail2;
	}

	*slot_out = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int _lcd_vmalloc(unsigned int order, cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
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
	ret = lcd_syscall_vmalloc(order, &slot);
	if (ret) {
		LIBLCD_ERR("vmalloc syscall failed");
		goto fail2;
	}

	*slot_out = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

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

/* PAGE ALLOCATOR INTERNALS ---------------------------------------- */

static int 
heap_alloc_map_metadata_memory_chunk(struct lcd_page_allocator_cbs *cbs,
				unsigned long mapping_offset,
				unsigned int alloc_order,
				struct lcd_resource_node **n_out)
{
	int ret;
	cptr_t pages;
	gpa_t dest = gpa_add(LCD_HEAP_GP_ADDR, mapping_offset);
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

struct lcd_page_allocator_cbs heap_page_allocator_cbs = {
	.alloc_map_metadata_memory_chunk = heap_alloc_map_metadata_memory_chunk,
	.free_unmap_metadata_memory_chunk = heap_free_unmap_metadata_memory_chunk,
	.alloc_map_regular_mem_chunk = heap_alloc_map_regular_mem_chunk,
	.free_unmap_regular_mem_chunk = heap_free_unmap_regular_mem_chunk,
};

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

static int setup_struct_page_array(void)
{
	struct lcd_page_block *pb;
	unsigned int order;
	unsigned long bytes;
	/*
	 * Compute number of struct pages we need
	 */
	bytes = roundup_pow_of_two((1UL << LCD_HEAP_NR_PAGES_ORDER) *
				sizeof(struct page));
	order = ilog2(bytes >> PAGE_SHIFT);
	/*
	 * Do the alloc
	 */
	pb = lcd_page_allocator_alloc(heap_allocator, order);
	if (!pb) {
		LIBLCD_ERR("error setting up struct page array for heap");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Zero out the array (unnecessary right now, but just in case)
	 */
	heap_page_array = (void *)gva_val(lcd_page_block_to_addr(pb));
	memset(heap_page_array,
		0,
		(1 << (order + PAGE_SHIFT)));

	return 0;

fail1:
	return ret;
}

void cpucache_init(void);

static void __init_refok kmalloc_init(void)
{
	kmem_cache_init();
	kmem_cache_init_late();
	cpucache_init();
}

static int init_heap(void)
{
	int ret;
	/*
	 * Create new page allocator in heap region
	 */
	ret = lcd_page_allocator_create(LCD_HEAP_SIZE,
					LCD_HEAP_MIN_ORDER,
					LCD_HEAP_MAX_ORDER,
					LCD_HEAP_MAX_ORDER,
					&heap_page_allocator_cbs,
					1, /* embed metadata */
					&heap_allocator);
	if (ret) {
		LIBLCD_ERR("error initializing heap allocator");
		goto fail1;
	}
	/*
	 * Set up struct page array
	 */
	ret = setup_struct_page_array();
	if (ret) {
		LIBLCD_ERR("error setting up struct page array for heap");
		goto fail2;
	}
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

fail2:
	lcd_page_allocator_destroy(heap_allocator);
	heap_allocator = NULL;
fail1:
	return ret;
}

/* VOLUNTEERING -------------------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_phys_to_cptr(
		lcd_gva2gpa(__gva((unsigned long)lcd_page_address(base))), 
		slot_out, &size);
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	return;
}

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_phys_to_cptr(base, slot_out, &size);
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	return;
}

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_virt_to_cptr(base, slot_out, &size);
}

void lcd_unvolunteer_vmalloc_mem(cptr_t vmalloc_mem)
{
	return;
}

/* ADDRESS TRANSLATION -------------------------------------------------- */

gpa_t lcd_gva2gpa(gva_t gva)
{
	return __gpa(LCD_PHYS_BASE + (gva_val(gva) - LCD_VIRT_BASE));
}

gva_t lcd_gpa2gva(gpa_t gpa)
{
	return __gva(LCD_VIRT_BASE + (gpa_val(gpa) - LCD_PHYS_BASE));
}

/* INIT/EXIT -------------------------------------------------- */

int __liblcd_mem_init(void)
{
	int ret;
	
	ret = init_heap();
	if (ret) {
		LIBLCD_ERR("heap init failed");
		goto fail1;
	}

	return 0;

fail1:
	return ret;
}
