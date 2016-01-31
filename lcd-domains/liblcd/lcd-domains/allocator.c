/*
 * allocator.c
 *
 * LCD page allocator implementation.
 *
 * Draws from design and code of Linux buddy allocator.
 *
 * Copyright: University of Utah
 */

#include <lcd-domains/liblcd.h>

/* MISC ------------------------------------------------------------ */

unsigned int pa_nr_free_lists(struct lcd_page_allocator *pa)
{
	return pa->max_order - pa->min_order + 1;
}

unsigned long pa_nr_page_blocks(struct lcd_page_allocator *pa)
{
	/* 2^nr_pages_order  /  2^min_order */
	return (1UL << (pa->nr_pages_order - pa->min_order));
}

/* ALLOCATOR INIT -------------------------------------------------- */

static struct lcd_page_allocator* 
init_struct_pa(void *metadata_addr,
	unsigned long nr_pages_order,
	unsigned int min_order,
	unsigned int metadata_malloc_order,
	unsigned int max_order,
	const struct lcd_page_allocator_cbs *cbs,
	int embed_metadata)
{
	struct lcd_page_allocator *pa = metadata_addr;

	pa->nr_pages_order = nr_pages_order;
	pa->min_order = min_order;
	pa->metadata_malloc_order = metadata_malloc_order;
	pa->max_order = max_order;
	pa->cbs = cbs;
	pa->embed_metadata = embed_metadata;

	return pa;
}

void init_free_list_heads(struct lcd_page_allocator *pa)
{
	unsigned int i;
	struct list_head *free_lists;
	/*
	 * The free lists are just after the struct lcd_page_allocator
	 * (plus some alignment)
	 */
	free_lists = (struct list_head *)ALIGN(((void *)pa) + sizeof(*pa),
					sizeof(struct list_head));
	pa->free_lists = free_lists;
	/*
	 * Initialize each one
	 */
	for (i = 0; i < pa_nr_free_lists(pa); i++)
		INIT_LIST_HEAD(&free_lists[i]);
}

void init_page_blocks(struct lcd_page_allocator *pa)
{
	struct lcd_page_block *cursor;
	unsigned long i;
	/*
	 * The array of lcd_page_block's comes after the free list heads
	 * (plus some alignment)
	 */
	cursor = (struct lcd_page_block *)ALIGN(
		(void *)(pa->free_lists) +
		pa_nr_free_lists(pa) * sizeof(struct list_head),
		sizeof(struct lcd_page_block));
	/*
	 * Zero out the array
	 */
	memset(cursor, 0, 
		pa_nr_page_blocks(pa) * sizeof(struct lcd_page_block));
	/*
	 * Initialize each one's buddy list head, and free the page, so
	 * the allocator will build up the free lists.
	 */
	for (i = 0; i < pa_nr_page_blocks(pa); i++) {
		INIT_LIST_HEAD(&cursor[i].buddly_list);
		lcd_page_allocator_free(pa, 
					&cursor[i],
					0);
	}
}

static int allocator_init(void *metadata_addr,
			unsigned long nr_pages_order,
			unsigned int min_order,
			unsigned int metadata_malloc_order,
			unsigned int max_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			struct lcd_page_allocator **pa_out)
{
	struct lcd_page_allocator *pa;
	/*
	 * Init top level struct
	 */
	pa = init_struct_pa(metadata_addr,
			nr_pages_order,
			min_order,
			max_order,
			cbs,
			embed_metadata);
	/*
	 * Init free list heads
	 */
	init_free_list_heads(pa);
	/*
	 * Init the array of page blocks
	 */
	init_page_blocks(pa);
	/*
	 * If the metadata is embedded, allocate enough page blocks
	 * to cover it
	 */
	if (embed_metadata) {
		ret = alloc_metadata_page_blocks(pa);
		if (ret)
			goto fail1;
	}

	*pa_out = pa;

	return 0;

fail1:
	return ret;
}

/* METADATA FREE -------------------------------------------------- */

static void free_metadata(void *metadata_addr,
			unsigned int alloc_order,
			unsigned long metadata_sz,
			const struct lcd_page_allocator_cbs *cbs)
{
	struct lcd_resource_node *n;
	unsigned long nr_blocks;
	unsigned long i;
	int ret;
	gpa_t base;
	gpa_t addr;
	/*
	 * Calculate number of memory chunks we need to free
	 */
	nr_blocks = ALIGN(metadata_sz, (1UL << alloc_order)) >> alloc_order;
	/*
	 * Calculate starting guest physical address where metadata is
	 *
	 * XXX: Assumes guest virtual == guest physical
	 */
	base = __gpa((unsigned long)metadata_addr);
	/*
	 * Look up chunks, and pass to free callback
	 */
	for (i = 0; i < nr_blocks; i++) {
		addr = gpa_add(base, i * (1UL << alloc_order));
		ret = lcd_phys_to_resource_node(
			addr,
			&n);
		if (ret) {
			LIBLCD_ERR("error looking up metadata resource node for gpa = %lx", gpa_val(addr));
			continue;
		}
		cbs->free_unmap_metadata_memory_chunk(n);
	}
}

/* METADATA MALLOC -------------------------------------------------- */

static void* get_start_addr(struct lcd_resource_node *n)
{
	/* Assumes guest virtual == guest physical */
	return (void *)gpa_val(lcd_resource_node_start);
}

static void* malloc_metadata(unsigned int alloc_order,
			unsigned long metadata_sz,
			const struct lcd_page_allocator_cbs *cbs)
{
	struct lcd_resource_node *n;
	unsigned long nr_blocks;
	unsigned long i, j;
	int ret;
	void *out;
	/*
	 * Calculate number of memory chunks we need, rounded up.
	 */
	nr_blocks = ALIGN(metadata_sz, (1UL << alloc_order)) >> alloc_order;
	
	for (i = 0; i < nr_blocks; i++) {
		/*
		 * Allocate and map a memory chunk
		 */
		ret = cbs->alloc_map_metadata_memory_chunk(alloc_order,
							i * (1UL << alloc_order), 
							&n);
		if (ret) {
			LIBLCD_ERR("error allocating metadata chunk %ul of %ul", i, nr_blocks);
			goto fail1;
		}
		/*
		 * For first chunk, remember starting address (beginning
		 * of metadata).
		 */
		if (i == 0)
			out = get_start_addr(n)
	}

	return out;

fail1:
	for (j = 0; j < i; j++)
		cbs->free_unmap_metadata_memory_chunk(n);
	return NULL;
}

/* METADATA SIZE CALCULATIONS ------------------------------ */

static unsigned long calc_nr_lcd_page_blocks(unsigned int nr_pages_order,
					unsigned int min_order)
{
	return (1UL << (nr_pages_order - min_order));
}

static unsigned long calc_metadata_size(unsigned int nr_pages_order,
					unsigned int min_order,
					unsigned int metadata_malloc_order,
					unsigned int max_order,
					int embed_metadata,
					unsigned long *metadata_sz_out)
{
	unsigned int i;
	unsigned long rslt = 0;
	unsigned int backing_order = max_order;
	/*
	 * Sanity check values before we do any calculations. Must have:
	 *
	 *    min_order <= max_order <= nr_pages_order
	 *
	 * Furthermore, if we are embedding the metadata into the memory
	 * area, the metadata malloc order must satisfy:
	 *
	 *    min_order <= metadata_malloc_order <= max_order
	 *
	 * This is because we will mark those page blocks that contain
	 * metadata as occupied. It needs to make sense for the page
	 * allocator; the page allocator uses an internal algorithm to
	 * track a correspondence between page blocks and resource nodes.
	 * A resource node cannot span less than 2^min_order memory or
	 * more than 2^max_order memory.
	 *
	 * We don't use BUG_ON's so that we don't crash the caller.
	 */
	if (unlikely(min_order > max_order)) {
		LIBLCD_ERR("min order > max order, impossible");
		return -EINVAL;
	}
	if (unlikely(embed_metadata && min_order > metadata_malloc_order)) {
		LIBLCD_ERR("trying to embed metadata, and min_order > metadata malloc order, impossible");
		return -EINVAL;
	}
	if (unlikely(embed_metadata && metadata_malloc_order > max_order)) {
		LIBLCD_ERR("trying to embed metadata, and metadata malloc order > max order, impossible");
		return -EINVAL;
	}
	if (unlikely(max_order > nr_pages_order)) {
		LIBLCD_ERR("trying to embed metadata, and max order > total memory area order, impossible");
		return -EINVAL;
	}
	/*
	 * Sum up size of metadata
	 *
	 * Top-level page allocator struct
	 */
	rslt += sizeof(struct lcd_page_allocator);
	/*
	 * Align for struct list_head
	 */
	ALIGN(rslt, sizeof(struct list_head));
	/*
	 * Free list list_heads, one for each allowed order.
	 */
	for (i = min_order; i <= max_order; i++)
		rslt += sizeof(struct list_head);
	/*
	 * Align for struct lcd_page_block
	 */
	ALIGN(rslt, sizeof(struct lcd_page_block));
	/*
	 * Giant array of struct lcd_page_blocks
	 */
	rslt += calc_nr_lcd_page_blocks(nr_pages_order, min_order) *
		sizeof(struct lcd_page_block);
	/*
	 * --------------------------------------------------
	 *
	 * Other checks. We want
	 *
	 *    1 - metadata to fit in memory area if we are embedding it
	 */
	if (embed_metadata && rslt > (1UL << nr_pages_order)) {
		LIBLCD_ERR("trying to embed metadata, but it won't fit in the memory area; either increase the memory area size, or adjust other params so there is less metadata");
		return -EINVAL;
	}
	/*
	 * --------------------------------------------------
	 */
	*metadata_sz_out = rslt;

	return 0;
}

/* INTERFACE -------------------------------------------------- */

int lcd_page_allocator_create(unsigned long nr_pages_order,
			unsigned int min_order,
			unsigned int metadata_malloc_order,
			unsigned int max_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			struct lcd_page_allocator **pa_out)
{
	int ret;
	unsigned long metadata_sz;
	void *metadata_addr;
	/*
	 * Calculate the size of the page allocator metadata (includes
	 * the giant array of lcd_page_block's). Check to make sure
	 * parameters are sane (everything fits).
	 */
	ret = calc_metadata_size(nr_pages_order,
				min_order,
				metadata_malloc_order,
				max_order,
				embed_metadata,
				&metadata_sz);
	if (ret)
		goto fail1;
	/*
	 * Get memory for metadata
	 */
	metadata_addr = malloc_metadata(metadata_malloc_order,
					metadata_sz,
					cbs);
	if (!metadata_addr) {
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Initialize allocator
	 */
	ret = allocator_init(metadata_addr, 
			nr_pages_order,
			min_order,
			metadata_malloc_order,
			max_order,
			cbs,
			embed_metadata);
	if (ret)
		goto fail3;

	return 0;

fail3:
	free_metadata(metadata_addr,
		metadata_malloc_order,
		metadata_sz,
		cbs);
fail2:
fail1:
	return ret;

}
