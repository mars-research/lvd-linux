/*
 * allocator.h
 *
 * Code for creating instances of general buddy
 * allocators for mem mgt of different memory area
 * types.
 *
 * An lcd_page_allocator is similar in some ways to
 * a Linux zone - it's for managing one contiguous
 * chunk of similar data.
 *
 * This code provides the option for demand paging like
 * functionality. You might wonder - why not fault in memory
 * on EPT faults? Because this would bring the microkernel
 * into the picture, and we don't want the microkernel doing
 * internal LCD mem mgt. (It's possible for an LCD to handle 
 * certain EPT faults as a virtualization exception,
 * but that's for another day ... and that may not be efficient for
 * a general allocator? would also need background thingamajig for
 * shrinking faulted in mem that's not being used).
 */

#include <linux/list.h>
#include <lcd-domains/liblcd.h>

/* STRUCT DEFS -------------------------------------------------- */

struct lcd_page_block {
	unsigned int order;
	struct lcd_resource_node *n;
	struct list_head buddy_list;
};

struct lcd_page_allocator_cbs {

	int (*alloc_map_metadata_memory_chunk)(
		struct lcd_page_allocator_cbs*,
		unsigned int,
		unsigned long,
		struct lcd_resource_node**);

	void (*free_unmap_metadata_memory_chunk)(
		struct lcd_page_allocator_cbs*,
		struct lcd_resource_node*);
					
	int (*alloc_map_regular_mem_chunk)(
		struct lcd_page_allocator *,
		struct lcd_page_block *,
		unsigned long offset,
		unsigned int order,
		struct lcd_resource_node **);

	void (*free_unmap_regular_mem_chunk)(
		struct lcd_page_allocator *,
		struct lcd_page_block *,
		struct lcd_resource_node *,
		unsigned long,
		unsigned int);
};

struct lcd_page_allocator {

	unsigned int nr_pages_order;
	unsigned int min_order;
	unsigned int metadata_malloc_order;
	unsigned int max_order;

	int embed_metadata;
	unsigned long first_non_metadata_pb_idx;

	struct list_head *free_lists;
	unsigned long nr_page_blocks_free;

	struct lcd_page_block *pb_array;

	const struct lcd_page_allocator_cbs *cbs;
};

/* INTERFACE -------------------------------------------------- */

int lcd_page_allocator_create(unsigned long nr_pages_order,
			unsigned int min_order,
			unsigned int metadata_malloc_order,
			unsigned int max_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			struct lcd_page_allocator **pa_out);

void lcd_page_allocator_destroy(struct lcd_page_allocator *pa);

struct lcd_page_block*
lcd_page_allocator_alloc(struct lcd_page_allocator *pa,
			unsigned int order);

void lcd_page_allocator_free(struct lcd_page_allocator *pa,
			struct lcd_page_block *pb,
			unsigned int order);

unsigned long lcd_page_block_to_offset(struct lcd_page_allocator *pa,
				struct lcd_page_block *pb);

struct lcd_page_block*
lcd_offset_to_page_block(struct lcd_page_allocator *pa,
			unsigned long offset);
