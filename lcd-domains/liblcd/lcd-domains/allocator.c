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

static unsigned long calc_nr_lcd_page_blocks(unsigned int nr_pages_order,
					unsigned int min_order)
{
	return (1UL << (nr_pages_order - min_order));
}

static int calc_with_embedded_data(unsigned int nr_pages_order,
				unsigned int min_order,
				unsigned int max_order,
				unsigned int backing_order,
				unsigned int nr_extra_resource_nodes,
				unsigned long *rslt_in_out)
{
	unsigned int max_res_nodes;
	unsigned int min_res_nodes;
	unsigned long tmp;
	/*
	 * Align for struct lcd_resource_node
	 */
	ALIGN(*rslt_in_out, struct lcd_resource_node);
	if (*rslt_in_out > (1UL << backing_order)) {
		LIBLCD_ERR("trying to embed metadata in page allocator, but backing order is too small; can't even fit struct lcd_page_allocator and free list head's; choose a bigger value");
		return -EINVAL;
	}
	/*
	 * Calculate maximum number of resource nodes we can fit
	 * so that at least all metadata except the struct lcd_page_block array
	 * will fit into one 2^backing_order chunk of memory. (Integer
	 * division makes this conservative.)
	 */
	max_res_nodes = ((1UL << backing_order) - *rslt_in_out) /
		sizeof(struct lcd_resource_node);
	/*
	 * Calculate the minimum number of resource nodes we need. We're
	 * conservative here (otherwise the calculation gets even uglier)
	 * and add on the maximum amount of alignment required for struct 
	 * lcd_page_block. (We have to tweak the integer division so we
	 * stay conservative.)
	 */
	tmp = *rslt_in_out;
	tmp += __alignof__(struct lcd_page_block);
	tmp += calc_nr_lcd_page_blocks(nr_pages_order, min_order) *
		sizeof(struct lcd_page_block);
	min_res_nodes = DIV_ROUND_UP(tmp,
				(1UL << backing_order) - 
				sizeof(struct lcd_resource_node));
	/*
	 * Check if nodes will fit
	 */
	if (min_res_nodes + nr_extra_nodes > max_res_nodes) {
		LIBLCD_ERR("can't embed metadata in page allocator: the number of nodes needed is such that we can't embed the top level metadata inside one chunk of 2^backing_order memory; increase backing_order and/or lower nr_extra_resource_nodes");
		return -EINVAL;
	}
	/*
	 * Okay, coast is clear; tack on resource nodes, etc.
	 */
	*rslt_in_out += (min_res_nodes + nr_extra_resource_nodes) *
		sizeof(struct lcd_resource_node);
	ALIGN(*rslt_in_out, struct lcd_page_block);
	*rslt_in_out += calc_nr_lcd_page_blocks(nr_pages_order, min_order) *
		sizeof(struct lcd_page_block);
	/*
	 * Final check: Make sure this will all fit in the memory area
	 */
	if (*rslt_in_out > (1UL << nr_pages_order)) {
		LIBLCD_ERR("embedded metadata won't fit inside memory area; you need to increase the size of the memory area (increase nr_pages_order), or reconfigure other parts (increase min_order, or backing_order)");
		return -EINVAL;
	}

	return 0;
}

static int calc_metadata_size(unsigned int nr_pages_order,
			unsigned int min_order,
			unsigned int max_order,
			unsigned int backing_order,
			int embed_metadata,
			unsigned int nr_extra_resource_nodes,
			unsigned long *rslt_out)
{
	int ret;
	unsigned int i;
	unsigned long rslt = 0;
	unsigned long nr_page_blocks;
	unsigned int nr_res_nodes;
	/*
	 * Sanity check values. Must have:
	 *
	 *    min_order <= backing_order <= max_order <= nr_pages_order
	 */
	BUG_ON(min_order > backing_order);
	BUG_ON(backing_order > max_order);
	BUG_ON(max_order > nr_pages_order);
	/*
	 * Sum up size of metadata
	 *
	 * Top-level page allocator struct
	 */
	rslt += sizeof(struct lcd_page_allocator);
	/*
	 * Align for struct list_head
	 */
	ALIGN(rslt, struct list_head);
	/*
	 * Free list list_heads, one for each allowed order.
	 */
	for (i = min_order; i <= max_order; i++)
		rslt += sizeof(struct list_head);

	if (embed_metadata) {
		
		*rslt_out = rslt;

		return calc_with_embedded_metadata(nr_pages_order,
						min_order,
						max_order,
						backing_order,
						nr_extra_resource_nodes,
						&rslt_out);

	} else {
		/*
		 * Align for struct lcd_page_block
		 */
		ALIGN(rslt, struct lcd_page_block);
		/*
		 * Giant array of struct lcd_page_blocks
		 */
		rslt += calc_nr_lcd_page_blocks(nr_pages_order, min_order) *
			sizeof(struct lcd_page_block);

		*rslt_out = rslt;
		return 0;
	}
}

int lcd_page_allocator_create(unsigned long nr_pages_order,
			unsigned int min_order,
			unsigned int max_order,
			unsigned int backing_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			unsigned int nr_extra_resource_nodes,
			struct lcd_page_allocator **pa_out)
{
	int ret;
	unsigned long metadata_sz;
	/*
	 * Calculate the size of the page allocator metadata (includes
	 * the giant array of lcd_page_block's). Check to make sure
	 * parameters are sane (everything fits).
	 */
	ret = calc_metadata_size(nr_pages_order,
				min_order,
				max_order,
				backing_order,
				embed_metadata,
				nr_extra_resource_nodes,
				&metadata_sz);
	if (ret)
		return ret;


	/* Repeatedly invoke setup_alloc cb until we have enough for
	 * metadata */

	/*     If embed_metadata is true, insert nodes into our tree */


	/* Metadata alloc'd; init buddy allocator */


	/* If embed metadata true, init used mem chunks */


}
