/*
 * allocator.c
 *
 * LCD page allocator implementation.
 *
 * Draws from design and code of Linux buddy allocator.
 *
 * Our lcd_page_allocator is like a Linux zone. (Linux uses
 * a buddy allocator per zone.)
 *
 * Copyright: University of Utah
 */

#include <lcd-domains/liblcd.h>

/* MISC ------------------------------------------------------------ */

static unsigned int pa_nr_free_lists(struct lcd_page_allocator *pa)
{
	return pa->max_order - pa->min_order + 1;
}

static unsigned long pa_nr_page_blocks(struct lcd_page_allocator *pa)
{
	/* 2^nr_pages_order  /  2^min_order */
	return (1UL << (pa->nr_pages_order - pa->min_order));
}

static int pb_is_free(struct lcd_page_block *pb)
{
	return !list_empty(&buddy->buddy_list);
}

/* ALLOC ------------------------------------------------------------ */

static int pb_backing(struct lcd_page_allocator *pa, 
		struct lcd_page_block *pb)
{
	struct lcd_resource_node *n;
	unsigned long offset;
	int ret;
	/*
	 * If:
	 *     1 - Caller wants us to do demand paging (callback is not NULL)
	 *     2 - The page block chunk we just allocated falls on a
	 *         max order boundary
	 *     3 - The first page block does not have a resource node (the
	 *         memory is not backed)
	 *
	 * then invoke the alloc callback.
	 */
	if (!pa->alloc_map_regular_mem_chunk) /* no demand paging */
		return 0;
	offset = lcd_page_block_to_offset(pa, pb);
	if (offset & ((1UL << pa->max_order) - 1)) /* not on boundary */
		return 0;
	if (pb->n) /* already backed */
		return 0;

	ret = pa->cbs.alloc_map_regular_mem_chunk(pa, pb, offset, 
						pa->max_order, &n);
	if (ret) {
		LIBLCD_ERR("failed to get backing mem for page blocks");
		return ret;
	}
	/*
	 * Install resource node in page block
	 */
	pb->n = n;

	return 0;
}

/* Compare with mm/page_alloc.c: expand */ 
static void pb_expand(struct lcd_page_allocator *pa, struct lcd_page_block *pb,
		unsigned int lo, unsigned int hi, struct list_head *free_list)
{

	unsigned long size = 1UL << high;
	/*
	 * Split up into smaller chunks
	 */
	while (high > low) {
		/*
		 * Shift to next smaller free list
		 */
		free_list--;
		high--;
		size >>= 1;
		/*
		 * This will split off the end
		 */
		list_add(&pb[size].buddy_list, free_list);
		pa->nr_page_blocks_free += size;
		pb[size].order = high;
	}
}

/* Compare with mm/page_alloc.c: __rmqueue_smallest */
static lcd_page_block *
alloc_page_blocks(struct lcd_page_allocator *pa, unsigned int order)
{
	unsigned int current_order;
	unsigned int idx;
	struct lcd_page_block *pb;
	struct list_head *free_list;
	/*
	 * Look for a non-empty free list, starting from the smallest
	 * possible.
	 */
	for (current_order = order; 
	     current_order <= pa->max_order; 
	     current_order++) {

		free_list_idx = current_order - pa->min_order;
		free_list = &pa->free_lists[free_list_idx];

		if (list_empty(free_list))
			continue;
		/*
		 * Found one
		 */
		pb = list_entry(free_list.next, struct lcd_page_block,
				buddy_list);
		/* 
		 * Remove page blocks from free list, and do any
		 * necessary splitting. (It's important to do
		 * list_del_init, because for us, we check if a page block's
		 * buddy list is empty to see if it's free.)
		 */
		list_del_init(&pb->buddy_list);
		pa->nr_page_blocks_free -= (1UL << current_order);
		pb_expand(pa, pb, order, current_order, free_list);

		return pb;
	}

fail:
	return NULL;
}

/* FREE ------------------------------------------------------------ */

static void pb_unbacking(struct lcd_page_allocator *pa, 
			struct lcd_page_block *pb, unsigned int order)
{
	unsigned long offset;
	/*
	 * If:
	 *      1 - the page block is at the beginning of a resource node
	 *      2 - its order is at least as big as the resource node's order
	 *      3 - the caller provided a free call back
	 *
	 * invoke the free callback so that the caller can free / unmap the
	 * backing memory.
	 */
	if (!(pb->n != NULL && 
			order >= pb->n->order &&
			pa->cbs.free_unmap_regular_mem_chunk))
		return;

	offset = lcd_page_block_to_offset(pa, pb);

	pa->cbs.free_unmap_regular_mem_chunk(pa, pb, pb->n, offset, order);
	/*
	 * NULL out the page block's resource node so we know it's not
	 * backed anymore
	 */
	pb->n = NULL;
}

/* mm/page_alloc.c: __find_buddy_index */
static inline unsigned long
__find_buddy_index(unsigned long pb_idx, unsigned int order)
{
	return pb_idx ^ (1 << order);
}

/* mm/page_alloc.c: page_is_buddy */
static inline int pb_is_buddy(struct lcd_page_block *pb, 
			struct lcd_page_block *buddy,
			int order)
{
	if (!pb_is_free(buddy)) {
		/* Not in a free list; buddy is in use. */
		return 0;
	}
	if (buddy->order != order) {
		/* Buddy sitting on smaller free list, so we can't
		 * coalesce yet. */
		return 0;
	}

	return 1;
}

static void do_free(struct lcd_page_allocator *pa,
		struct lcd_page_block *pb,
		unsigned int order)
{
	unsigned long pb_idx;
	unsigned long combined_idx;
	unsigned long buddy_idx;
	struct lcd_page_block *buddy;
	/*
	 * Calculate index of page block in giant array. Should
	 * be a multiple of 2^order.
	 */
	pb_idx = (pb - pa->pb_array) / sizeof(struct lcd_page_block);
	BUG_ON(pb_idx & ((1 << order) - 1));
	/*
	 * Coalesce
	 */
	while (order < pa->max_order) {
		/*
		 * Get buddy page block
		 */
		buddy_idx = __find_buddy_index(pb_idx, order);
		buddy = pb + (buddy_idx - pb_idx);
		if (!pb_is_buddy(pb, buddy, order))
			break;
		/*
		 * Our buddy is free; merge with it and move up one order.
		 *
		 * 1 - Remove from its current free list
		 */
		list_del(&buddy->buddy_list);
		/*
		 * 2 - Dec the total number of free page blocks as we
		 *     are removing it from its current free list. (We will
		 *     inc the total at the end.)
		 */
		pa->nr_page_blocks_free -= (1UL << order);
		/*
		 * 3 - Reset its order to 0
		 */
		buddy->order = 0;
		/*
		 * 4 - Throw away the lowest order set bit to give the
		 *     index of pb + buddy in the array
		 */
		combined_idx = buddy_idx & pb_idx;
		/*
		 * 5 - Shift to that new point in the array (this should be
		 *     either buddy or pb, depending on which comes first),
		 *     and go to the next order level.
		 */
		pb = pb + (combined_idx - pb_idx);
		pb_idx = combined_idx;
		order++;
		/*
		 * ----------------------------------------
		 *
		 * Check if we need to free / unmap the backing memory
		 */
		pb_unbacking(pa, pb, order);
	}
	/*
	 * Check one more time if we need to free / unmap the backing memory
	 * (in case the resource node's order is max order)
	 */
	pb_unbacking(pa, pb, order);
	/*
	 * Install coalesced page block in correct free list. (It's
	 * important we do list_add_tail because the init code
	 * expects the page block chunks to be in memory order.)
	 */
	pb->order = order;
	list_add_tail(&pb->buddy_list, 
		&pa->free_lists[order - pa->min_order]);
	pa->nr_page_blocks_free += (1UL << order);
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
	pa->pb_array = cursor;
	/*
	 * Initialize each one's buddy list head, and add it to the 
	 * min order free list. (We have to add it to the min free
	 * list because the allocator won't consider a page block
	 * free if its buddy list is empty.) Then free each one to trigger full
	 * coalescing. (Freeing just the first page block won't trigger
	 * full coalescing, because the buddy allocator assumes that
	 * maximal coalescing has been done when free is called.)
	 */
	for (i = 0; i < pa_nr_page_blocks(pa); i++) {
		INIT_LIST_HEAD(&cursor[i].buddy_list);
		list_add(&cursor[i].buddy_list, &pa->free_lists[0]);
	}
	for (i = 0; i < pa_nr_page_blocks(pa); i++) {
		lcd_page_allocator_free(pa, 
					&cursor[i],
					0);
	}
}

static int alloc_metadata_page_blocks(struct lcd_page_allocator *pa,
				unsigned long metadata_sz)
{
	struct lcd_page_block *pb;
	struct lcd_resource_node *n;
	gpa_t metadata_base;
	unsigned long offset;
	unsigned int tail_order;
	/*
	 * If we encounter an error, we just fail because this will
	 * trigger a full page allocator tear down.
	 *
	 * --------------------------------------------------
	 *
	 * pa points to beginning of metadata. Remember,
	 * guest virtual == guest physical inside an LCD.
	 */
	metadata_base = __gpa((unsigned long)pa);
	/*
	 * Allocate the big chunks first
	 */
	while (metadata_sz > (1UL << pa->max_order)) {
		/*
		 * Allocate 2^max_order bytes
		 */
		pb = alloc_page_blocks(pa, pa->max_order);
		if (!pb) {
			LIBLCD_ERR("error allocating blocks for metadata");
			return -EIO;
		}
		/*
		 * Look up the resource node for this chunk of memory
		 */
		offset = lcd_page_block_to_offset(pa, pb);
		ret = lcd_phys_to_resource_node(
			gpa_add(metadata_base, offset),
			&n);
		if (ret) {
			LIBLCD_ERR("error looking up metadata resource node");
			return -EIO;
		}
		/*
		 * Install resource node in page block
		 */
		pb->n = n;
		/*
		 * Shift to next 2^max_order chunk
		 */
		metadata_sz -= (1UL << pa->max_order);
	}
	/*
	 * Allocate the remainder, rounded up to the closest power of 2
	 * page blocks. (This means the remaining page blocks will be used
	 * for real allocs.)
	 */
	tail_order = 0;
	while (metadata_sz >>= 1)
		tail_order++;
	pb = alloc_page_blocks(pa, tail_order);
	if (!pb) {
		LIBLCD_ERR("error allocating page blocks for metadata");
		return -EIO;
	}
	/*
	 * Look up and install resource node for remainder
	 */
	offset = lcd_page_block_to_offset(pa, pb);
	ret = lcd_phys_to_resource_node(
		gpa_add(metadata_base, offset),
		&n);
	if (ret) {
		LIBLCD_ERR("error looking up metadata resource node");
		return -EIO;
	}
	pb->n = n;
	/*
	 * Remember offset to first non-metadata page block
	 */
	pa->first_non_metadata_pb_idx = 
		(pb + (1UL << tail_order) - pa->pb_array) / 
		sizeof(struct lcd_page_block);
	/*
	 * All page blocks that contain metadata have been allocated,
	 * and resource nodes have been installed.
	 */
	return 0;
}

static int allocator_init(void *metadata_addr,
			unsigned long metadata_sz,
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
	 * to cover it (so that we don't try to use those page blocks
	 * for something else).
	 */
	if (embed_metadata) {
		ret = alloc_metadata_page_blocks(pa, metadata_sz);
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
		cbs->free_unmap_metadata_memory_chunk(cbs, n);
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
		ret = cbs->alloc_map_metadata_memory_chunk(cbs,
							alloc_order,
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
		cbs->free_unmap_metadata_memory_chunk(cbs, n);
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
			metadata_sz,
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

void lcd_page_allocator_destroy(struct lcd_page_allocator *pa)
{
	struct lcd_page_block *cursor;
	unsigned long i;

	cursor = pa->pb_array;
	/*
	 * Iterate over all page blocks, and if it's not free,
	 * free it. This may move neighbors ahead in the 
	 * giant array into the free lists.
	 *
	 * We do this so that it triggers the free callbacks.
	 *
	 * Skip over metadata (if we try to delete resources that contain
	 * metadata while we're tearing everything down, we'll crash).
	 */
	for (i = pa->embed_metadata_offset; i < pa_nr_page_blocks(pa); i++) {
		if (!pb_is_free(&cursor[i]))
			lcd_page_allocator_free(pa, &cursor[i], 0);
	}
	/*
	 * The caller is responsible for freeing the metadata
	 */
	return;
}

void lcd_page_allocator_free(struct lcd_page_allocator *pa,
			struct lcd_page_block *pb,
			unsigned int order)
{
	do_free(pa, pb, order);
}

struct lcd_page_block*
lcd_page_allocator_alloc(struct lcd_page_allocator *pa,
			unsigned int order)
{
	struct lcd_page_block *pb;
	/*
	 * Alloc the page blocks
	 */
	pb = alloc_page_blocks(pa, order);
	if (!pb)
		goto fail1;
	/*
	 * Suck in backing memory / demand page if necessary
	 */
	ret = pb_backing(pa, pb);
	if (ret)
		goto fail2;

	return pb;

fail2:
	do_free(pa, pb, order);
fail1:
	return NULL;
}

unsigned long lcd_page_block_to_offset(struct lcd_page_allocator *pa,
				struct lcd_page_block *pb)
{
	unsigned long offset;

	offset = (pb - pa->pb_array) / sizeof(struct lcd_page_block);
	
	/* Each page block is 2^min_order bytes of mem */
	return offset << pa->min_order;
}

struct lcd_page_block*
lcd_offset_to_page_block(struct lcd_page_allocator *pa,
			unsigned long offset)
{
	return &pa->pb_array[offset >> pa->min_order];
}
