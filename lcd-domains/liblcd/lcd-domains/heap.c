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
#include <lcd_domains/liblcd.h>

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
	ret = lcd_syscall_alloc_pages_exact_node(slot, nid, flags, order);
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
	ret = lcd_syscall_alloc_pages(slot, flags, order);
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

int _lcd_vmalloc(unsigned long nr_pages, cptr_t *slot_out)
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
	ret = lcd_syscall_vmalloc(slot, nr_pages);
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

/* PAGE ALLOCATOR INTERNALS ---------------------------------------- */

static void __do_one_heap_free(struct lcd_resource_node *n)
{
	cptr_t pages = n->cptr;
	gpa_t base = __gpa(lcd_resource_node_start(n));
	/*
	 * Unmap from guest physical
	 */
	_lcd_munmap(pages, base);
	/*
	 * Free pages from host
	 */
	lcd_cap_delete(pages);
}

static void do_one_heap_free(gpa_t dest)
{
	int ret;
	struct lcd_resource_node *n;
	/*
	 * Use resource tree to look up node for memory object
	 */
	ret = lcd_phys_to_resource_node(dest, &n);
	if (ret) {
		LIBLCD_ERR("error looking up resource node");
		return;
	}
	__do_one_heap_free(n);
}

static int do_one_heap_alloc(gpa_t dest, unsigned int alloc_order,
			struct lcd_resource_node **n_out)
{
	int ret;
	cptr_t pages;
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
	 * Get the resource node for the mapped memory
	 */
	ret = lcd_phys_to_resource_node(dest, n_out);
	if (ret) {
		LIBLCD_ERR("error getting resource node");
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

static int 
heap_alloc_map_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
				unsigned int alloc_order,
				unsigned long metadata_sz,
				void **metadata_addr)
{
	int ret;
	unsigned long i, j;
	unsigned long nr_allocs;
	unsigned total;
	gpa_t dest;
	struct lcd_resource_node *unused;

	/*
	 * Since we are embedding, we need to allocate in 2^alloc_order
	 * chunks of pages.
	 */
	total = ALIGN(metadata_sz, (1UL << (alloc_order + PAGE_SHIFT)));
	nr_allocs = total >> (alloc_order + PAGE_SHIFT); /* > 0 */

	for (i = 0; i < nr_allocs; i++) {

		dest = gpa_add(LCD_HEAP_GP_ADDR, 
			i * (1UL << (alloc_order + PAGE_SHIFT)));

		ret = do_one_heap_alloc(dest, alloc_order, &unused);
		if (ret) {
			LIBLCD_ERR("metadata alloca failed at i = %lx", i);
			goto fail1;
		}

	}
	
	*metadata_addr = (void *)gva_val(LCD_HEAP_GV_ADDR);

	return 0;

fail1:
	for (j = 0; j < i; j++) {

		dest = gpa_add(LCD_HEAP_GP_ADDR, 
			j * (1UL << (alloc_order + PAGE_SHIFT)));

		do_one_heap_free(dest);

	}

	return ret;
}

static void
heap_free_unmap_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
				void *metadata_addr,
				unsigned long metadata_sz,
				unsigned int alloc_order)
{
	unsigned long i;
	unsigned long nr_frees;
	unsigned total;
	gpa_t dest;

	/*
	 * Ignore the metadata address; should be at predefined spot.
	 *
	 * We alloc'd the metadata in chunks; free them.
	 */

	total = ALIGN(metadata_sz, (1UL << (alloc_order + PAGE_SHIFT)));
	nr_frees = total >> (alloc_order + PAGE_SHIFT); /* > 0 */

	for (i = 0; i < nr_frees; i++) {
		dest = gpa_add(LCD_HEAP_GP_ADDR, 
			i * (1UL << (alloc_order + PAGE_SHIFT)));
		do_one_heap_free(dest);
	}

	return;
}

static int 
heap_alloc_map_regular_mem_chunk(struct lcd_page_allocator *pa,
				struct lcd_page_block *dest_blocks,
				unsigned long mapping_offset,
				unsigned int alloc_order,
				struct lcd_resource_node **n_out)
{
	gpa_t dest = gpa_add(LCD_HEAP_GP_ADDR, mapping_offset);

	return do_one_heap_alloc(dest, alloc_order, n_out);
}

static void
heap_free_unmap_regular_mem_chunk(struct lcd_page_allocator *pa,
				struct lcd_page_block *page_blocks,
				struct lcd_resource_node *n_to_delete,
				unsigned long mapping_offset,
				unsigned int order)
{
	__do_one_heap_free(n_to_delete);
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

static inline gva_t heap_struct_page_to_addr(const struct page *p)
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
	const struct page *p)
{
	return heap_addr_to_page_block(
		heap_struct_page_to_addr(p));
}

/* PAGE ALLOC INTERFACE ---------------------------------------- */

struct page *lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
					unsigned int order)
{
	/*
	 * For now, we ignore the node id (not numa aware).
	 */
	return lcd_alloc_pages(flags, order);
}

struct page *lcd_alloc_pages(unsigned int flags, unsigned int order)
{
	struct lcd_page_block *pb;
	/*
	 * Do heap alloc. Flags are ignored for now.
	 */
	pb = lcd_page_allocator_alloc(heap_allocator, order);
	if (!pb) {
		LIBLCD_ERR("alloc failed");
		goto fail1;
	}
	/*
	 * Convert to struct page
	 */
	return heap_page_block_to_struct_page(pb);

fail1:
	return NULL;
}

void lcd_free_pages(struct page *base, unsigned int order)
{
	lcd_page_allocator_free(heap_allocator,
				heap_struct_page_to_page_block(base),
				order);
}

void* lcd_vmalloc(unsigned long sz)
{
	/* Not implemented for now */
	BUG();
	return NULL;
}

void lcd_vfree(void *ptr)
{
	/* Not implemented for now */
	BUG();
}

struct page *lcd_virt_to_head_page(const void *addr)
{
	unsigned long i_addr = (unsigned long)addr;

	/* If outside heap, error out and return NULL */
	if (i_addr < gva_val(LCD_HEAP_GV_ADDR) ||
		i_addr >= gva_val(LCD_HEAP_GV_ADDR) + LCD_HEAP_SIZE) {
		LIBLCD_ERR("address %p falls outside heap; only addresses in the heap can be converted to a struct page pointer (we don't use the costly struct page metadata for all bits of the physical address space)", addr);
		return NULL;
	}

	return heap_addr_to_struct_page(__gva((unsigned long)addr));
}

void *lcd_page_address(const struct page *page)
{
	return (void *)gva_val(heap_struct_page_to_addr(page));
}

void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order)
{
	lcd_free_pages(heap_addr_to_struct_page(__gva(addr)), order);
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

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned long nr_pages,
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

static int setup_struct_page_array(void)
{
	struct lcd_page_block *pb;
	unsigned int order;
	unsigned long bytes;
	int ret;
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
	heap_page_array = (void *)gva_val(heap_page_block_to_addr(pb));
	memset(heap_page_array,
		0,
		(1UL << (order + PAGE_SHIFT)));

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

struct lcd_page_allocator_cbs heap_page_allocator_cbs = {
	.alloc_map_metadata_memory = heap_alloc_map_metadata_memory,
	.free_unmap_metadata_memory = heap_free_unmap_metadata_memory,
	.alloc_map_regular_mem_chunk = heap_alloc_map_regular_mem_chunk,
	.free_unmap_regular_mem_chunk = heap_free_unmap_regular_mem_chunk,
};

int __liblcd_heap_init(void)
{
	int ret;
	/*
	 * Create new page allocator in heap region
	 */
	ret = lcd_page_allocator_create(LCD_HEAP_NR_PAGES_ORDER,
					LCD_HEAP_MIN_ORDER,
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
	lcd_page_allocator_destroy(heap_allocator); /* frees metadata */
	heap_allocator = NULL;
fail1:
	return ret;
}
