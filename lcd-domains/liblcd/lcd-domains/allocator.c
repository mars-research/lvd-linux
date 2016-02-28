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

#include <liblcd/liblcd.h>

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
	return !list_empty(&pb->buddy_list);
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
	if (!pa->cbs->alloc_map_regular_mem_chunk) /* no demand paging */
		return 0;
	offset = lcd_page_block_to_offset(pa, pb);
	if (offset & ((1UL << (pa->max_order + PAGE_SHIFT)) - 1)) {
		/* not on boundary */
		return 0;
	}
	if (pb->n) /* already backed */
		return 0;

	ret = pa->cbs->alloc_map_regular_mem_chunk(pa, pb, offset, 
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

	unsigned long size = 1UL << hi;
	/*
	 * Split up into smaller chunks
	 *
	 * (Note: Unlike the original Linux expand, this is in terms
	 * of *page blocks* and not pages. Recall that each page block
	 * represents 2^min_order pages.)
	 */
	while (hi > lo) {
		/*
		 * Shift to next smaller free list
		 */
		free_list--;
		hi--;
		size >>= 1;
		/*
		 * This will split off the end
		 */
		list_add(&pb[size].buddy_list, free_list);
		pa->nr_page_blocks_free += size;
		pb[size].block_order = hi;
	}
}

/* Compare with mm/page_alloc.c: __rmqueue_smallest */
static struct lcd_page_block *
alloc_page_blocks(struct lcd_page_allocator *pa, unsigned int order)
{
	unsigned int current_order, block_order;
	struct lcd_page_block *pb;
	struct list_head *free_list;
	/*
	 * Look for a non-empty free list, starting from the smallest
	 * possible.
	 */
	for (current_order = order; 
	     current_order <= pa->max_order; 
	     current_order++) {

		block_order = current_order - pa->min_order;
		free_list = &pa->free_lists[block_order];

		if (list_empty(free_list))
			continue;
		/*
		 * Found one
		 */
		pb = list_entry(free_list->next, struct lcd_page_block,
				buddy_list);
		/* 
		 * Remove page blocks from free list, and do any
		 * necessary splitting. (It's important to do
		 * list_del_init, because for us, we check if a page block's
		 * buddy list is empty to see if it's free.)
		 */
		list_del_init(&pb->buddy_list);
		pa->nr_page_blocks_free -= (1UL << block_order);
		pb_expand(pa, pb, order - pa->min_order, block_order, 
			free_list);

		return pb;
	}

	/* No luck */
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
			(1UL << order) >= lcd_resource_node_size(pb->n) &&
			pa->cbs->free_unmap_regular_mem_chunk))
		return;

	offset = lcd_page_block_to_offset(pa, pb);

	pa->cbs->free_unmap_regular_mem_chunk(pa, pb, pb->n, offset, order);
	/*
	 * NULL out the page block's resource node so we know it's not
	 * backed anymore
	 */
	pb->n = NULL;
}

/* mm/page_alloc.c: __find_buddy_index */
static inline unsigned long
__find_buddy_index(unsigned long pb_idx, unsigned int block_order)
{
	return pb_idx ^ (1 << block_order);
}

/* mm/page_alloc.c: page_is_buddy */
static inline int pb_is_buddy(struct lcd_page_block *pb, 
			struct lcd_page_block *buddy,
			int block_order)
{
	if (!pb_is_free(buddy)) {
		/* Not in a free list; buddy is in use. */
		return 0;
	}
	if (buddy->block_order != block_order) {
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
	unsigned int block_order;
	/*
	 * Calculate order in terms of blocks (e.g.,
	 * order = min_order corresponds to block_order = 0)
	 */
	block_order = order - pa->min_order;
	/*
	 * Calculate index of page block in giant array. Should
	 * be a multiple of 2^order.
	 */
	pb_idx = pb - pa->pb_array;
	BUG_ON(pb_idx & ((1 << block_order) - 1));
	/*
	 * Coalesce
	 */
	while (order < pa->max_order) {
		/*
		 * Get buddy page block
		 */
		buddy_idx = __find_buddy_index(pb_idx, block_order);
		buddy = pb + (buddy_idx - pb_idx);
		if (!pb_is_buddy(pb, buddy, block_order))
			break;
		/*
		 * Our buddy is free; merge with it and move up one order.
		 *
		 * 1 - Remove from its current free list
		 */
		list_del_init(&buddy->buddy_list);
		/*
		 * 2 - Dec the total number of free page blocks as we
		 *     are removing it from its current free list. (We will
		 *     inc the total at the end.)
		 */
		pa->nr_page_blocks_free -= (1UL << block_order);
		/*
		 * 3 - Reset its block order to 0
		 */
		buddy->block_order = 0;
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
		block_order++;
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
	pb->block_order = block_order;
	list_add_tail(&pb->buddy_list, 
		&pa->free_lists[block_order]);
	pa->nr_page_blocks_free += (1UL << block_order);
}

/* ALLOCATOR INIT -------------------------------------------------- */

#if 0
static void check_free_lists(struct lcd_page_allocator *pa)
{
	unsigned idx;
	struct list_head *cursor;
	struct lcd_page_block *pb;

	for (idx = pa->min_order; idx <= pa->max_order; idx++) {
		if (list_empty(&pa->free_lists[idx])){
			LIBLCD_ERR("LIST order=%d EMPTY", idx);
			continue;
		}
		LIBLCD_ERR("LIST order=%d NOT EMPTY", idx);

		if (idx >= pa->max_order - 2) {
			list_for_each(cursor, &pa->free_lists[idx - pa->min_order]) {

				pb = list_entry(cursor, struct lcd_page_block,
						buddy_list);
				LIBLCD_ERR("head pb idx 0x%lx, group order %d",
					pb - pa->pb_array,
					pb->block_order);
			}
		}

	}


	LIBLCD_ERR("DONE");

}
#endif

static struct lcd_page_allocator* 
init_struct_pa(void *metadata_addr,
	unsigned long metadata_sz,
	unsigned long nr_pages_order,
	unsigned int min_order,
	unsigned int max_order,
	const struct lcd_page_allocator_cbs *cbs,
	int embed_metadata)
{
	struct lcd_page_allocator *pa = metadata_addr;

	pa->metadata_sz = metadata_sz;
	pa->nr_pages_order = nr_pages_order;
	pa->min_order = min_order;
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
	free_lists = (struct list_head *)ALIGN(
		(unsigned long)(((void *)pa) + sizeof(*pa)),
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
		(unsigned long)(
			(void *)(pa->free_lists) +
			pa_nr_free_lists(pa) * sizeof(struct list_head)),
		sizeof(struct lcd_page_block));
	/*
	 * Zero out the array
	 */
	memset(cursor, 0, 
		pa_nr_page_blocks(pa) * sizeof(struct lcd_page_block));
	pa->pb_array = cursor;
	/*
	 * Initialize each one's buddy list head, and put the
	 * max_order page block boundaries in the largest-sized
	 * free list.
	 */
	for (i = 0; i < pa_nr_page_blocks(pa); i++)
		INIT_LIST_HEAD(&cursor[i].buddy_list);

	for (i = 0; 
	     i < pa_nr_page_blocks(pa); 
	     i += (1UL << (pa->max_order - pa->min_order))) {
		cursor[i].block_order = pa->max_order - pa->min_order;
		list_add_tail(&cursor[i].buddy_list,
			&pa->free_lists[pa->max_order - pa->min_order]);
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
	int ret;
	/*
	 * If we encounter an error, we just fail because this will
	 * trigger a full page allocator tear down.
	 *
	 * --------------------------------------------------
	 *
	 * pa points to beginning of metadata. Remember,
	 * guest virtual == guest physical inside an LCD.
	 */
	metadata_base = lcd_gva2gpa(__gva((unsigned long)pa));
	/*
	 * Allocate the big chunks first
	 */
	while (metadata_sz > (1UL << (pa->max_order + PAGE_SHIFT))) {
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
		metadata_sz -= (1UL << (pa->max_order + PAGE_SHIFT));
	}
	/*
	 * Allocate the remainder, rounded up to the closest power of 2
	 * page blocks. (This means the remaining page blocks will be used
	 * for real allocs.)
	 */
	metadata_sz = ALIGN(metadata_sz,
			(1UL << (pa->min_order + PAGE_SHIFT)));
	tail_order = ilog2(metadata_sz >> (pa->min_order + PAGE_SHIFT));
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
		pb + (1UL << tail_order) - pa->pb_array;
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
			unsigned int max_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			struct lcd_page_allocator **pa_out)
{
	int ret;
	struct lcd_page_allocator *pa;
	/*
	 * Init top level struct
	 */
	pa = init_struct_pa(metadata_addr,
			metadata_sz,
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
			unsigned long metadata_sz,
			unsigned int alloc_order,
			const struct lcd_page_allocator_cbs *cbs)
{
	cbs->free_unmap_metadata_memory(cbs, metadata_addr, metadata_sz,
					alloc_order);
}

/* METADATA MALLOC -------------------------------------------------- */

static void* malloc_metadata(unsigned int alloc_order,
			unsigned long metadata_sz,
			const struct lcd_page_allocator_cbs *cbs)
{
	int ret;
	void *out;
	/*
	 * Get memory
	 */
	ret = cbs->alloc_map_metadata_memory(cbs, alloc_order, metadata_sz,
					&out);
	if (ret) {
		LIBLCD_ERR("failed to get metadata memory");
		return NULL;
	}
	return out;
}

/* METADATA SIZE CALCULATIONS ------------------------------ */

static unsigned long calc_nr_lcd_page_blocks(unsigned int nr_pages_order,
					unsigned int min_order)
{
	return (1UL << (nr_pages_order - min_order));
}

static unsigned long calc_metadata_size(unsigned int nr_pages_order,
					unsigned int min_order,
					unsigned int max_order,
					int embed_metadata,
					unsigned long *metadata_sz_out)
{
	unsigned int i;
	unsigned long rslt = 0;
	/*
	 * Sanity check values before we do any calculations. Must have:
	 *
	 *    min_order <= max_order <= nr_pages_order
	 *
	 * We don't use BUG_ON's so that we don't crash the caller.
	 */
	if (unlikely(min_order > max_order)) {
		LIBLCD_ERR("min order > max order, impossible");
		return -EINVAL;
	}
	if (unlikely(max_order > nr_pages_order)) {
		LIBLCD_ERR("max order > total memory area order, impossible");
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
	rslt = ALIGN(rslt, sizeof(struct list_head));
	/*
	 * Free list list_heads, one for each allowed order.
	 */
	for (i = min_order; i <= max_order; i++)
		rslt += sizeof(struct list_head);
	/*
	 * Align for struct lcd_page_block
	 */
	rslt = ALIGN(rslt, sizeof(struct lcd_page_block));
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
	if (embed_metadata && rslt > (1UL << (nr_pages_order + PAGE_SHIFT))) {
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
				max_order,
				embed_metadata,
				&metadata_sz);
	if (ret)
		goto fail1;
	/*
	 * Get memory for metadata
	 */
	metadata_addr = malloc_metadata(max_order,
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
			max_order,
			cbs,
			embed_metadata,
			pa_out);
	if (ret)
		goto fail3;

	return 0;

fail3:
	free_metadata(metadata_addr, metadata_sz, max_order, cbs);
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
	for (i = pa->first_non_metadata_pb_idx; 
	     i < pa_nr_page_blocks(pa); i++) {
		if (!pb_is_free(&cursor[i]))
			lcd_page_allocator_free(pa, &cursor[i], 0);
	}
	/*
	 * Free metadata (pa becomes invalid as this call is executed)
	 */
	free_metadata(pa, pa->metadata_sz, pa->max_order, pa->cbs);
	pa = NULL;

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
	int ret;
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

	offset = pb - pa->pb_array;
	
	/* Each page block is 2^(min_order + PAGE_SHIFT) bytes of mem */
	return offset << (pa->min_order + PAGE_SHIFT);
}

struct lcd_page_block*
lcd_offset_to_page_block(struct lcd_page_allocator *pa,
			unsigned long offset)
{
	return &pa->pb_array[offset >> (pa->min_order + PAGE_SHIFT)];
}
