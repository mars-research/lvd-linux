/*
 * allocator.h
 *
 * Code for creating instances of general buddy
 * allocators for mem mgt of different memory area
 * types.
 *
 * This is *NOT* the heap allocator. It's more general.
 *
 * An lcd_page_allocator is similar in some ways to
 * a Linux zone - it's for managing one contiguous
 * chunk of similar data.
 *
 * The purpose of this is to prevent us from using inefficient
 * bitmaps and because we have different memory area types
 * (at least for now - the heap and the ioremap area). This
 * is to abstract over some common bits.
 *
 * This code provides the option for demand-paging-like
 * functionality. You might wonder - why not fault in memory
 * on EPT faults? Ans: Because this would bring the microkernel
 * into the picture, and we don't want the microkernel doing
 * internal LCD mem mgt. (It's possible for an LCD to handle 
 * certain EPT faults as a virtualization exception,
 * but that's for another day ... and that may not be efficient for
 * a general allocator? Would also need a background thingamajig for
 * shrinking faulted in mem that's not being used).
 */

#include <linux/list.h>
#include <lcd-domains/liblcd/resource_tree.h>

/* STRUCT DEFS -------------------------------------------------- */

/**
 * struct lcd_page_block
 *
 * This is analogous to a Linux struct page, but it represents
 * 2^min_order memory, where min_order is the minimum order used
 * in the configuration of the lcd_page_allocator.
 *
 * When this struct is on a free list, buddy_list is non-empty.
 *
 * In free lists, block_order is set on the first page block 
 * in a free chunk of page blocks. It means that 2^block_order
 * page blocks are contiguous (or 2^(block_order + min_order)
 * pages are contiguous).
 *
 * The usage of lcd_resource_node is a bit more complex. It's
 * related to the memory used to back a chunk of address space.
 * See the documentation for lcd_page_allocator_create for
 * more details.
 */
struct lcd_page_block {
	unsigned int block_order;
	struct lcd_resource_node *n;
	struct list_head buddy_list;
};

/**
 * struct lcd_page_allocator_cbs
 *
 * These are the callbacks the user of the lcd_page_allocator
 * should provide on creation.
 *
 * See lcd_page_allocator_create for more details.
 */
struct lcd_page_allocator_cbs {

	int (*alloc_map_metadata_memory_chunk)(
		struct lcd_page_allocator_cbs*,
		unsigned long,
		unsigned int,
		struct lcd_resource_node**);

	void (*free_unmap_metadata_memory_chunk)(
		struct lcd_page_allocator_cbs*,
		struct lcd_resource_node*);
					
	int (*alloc_map_regular_mem_chunk)(
		struct lcd_page_allocator *,
		struct lcd_page_block *,
		unsigned long,
		unsigned int,
		struct lcd_resource_node **);

	void (*free_unmap_regular_mem_chunk)(
		struct lcd_page_allocator *,
		struct lcd_page_block *,
		struct lcd_resource_node *,
		unsigned long,
		unsigned int);
};

/**
 * struct lcd_page_allocator
 *
 * This is an instance of a page allocator. None
 * of its fields are intended for external use at this
 * point, but each field is documented for those
 * trying to understand the internals.
 */
struct lcd_page_allocator {
	
	/* The page allocator covers 2^nr_pages_order pages of memory. */
	unsigned int nr_pages_order;
	/* The minimum number of pages you can allocate is 2^min_order. */
	unsigned int min_order;
	/* The maximum number of pages during metadata allocation that
	 * will be attempted is 2^metadata_malloc_order. */
	unsigned int metadata_malloc_order;
	/* The maximum number of pages you can allocate is 2^max_order. */
	unsigned int max_order;

	/* If page allocator was configured to embed metadata,
	 * embed_metadata is non-zero, and first_non_metadata_pb_idx
	 * refers to the index of the first page block that does not
	 * contain metadata. */
	int embed_metadata;
	unsigned long first_non_metadata_pb_idx;

	/* This is an array of struct list_head's, one for each possible
	 * order. */
	struct list_head *free_lists;
	/* The total number of free page blocks across all free lists. */
	unsigned long nr_page_blocks_free;

	/* Pointer to the beginning of the struct lcd_page_block array. */
	struct lcd_page_block *pb_array;

	/* Pointer to the callbacks that were passed during page allocator
	 * creation. */
	const struct lcd_page_allocator_cbs *cbs;
};

/* INTERFACE -------------------------------------------------- */

/**
 * lcd_page_allocator_create -- Create a new page allocator
 * @nr_pages_order: allocator will cover 2^nr_pages_order pages of memory
 * @min_order: minimum number of pages you can allocate is 2^min_order
 * @metadata_malloc_order: number of pages alloc'd during init
 * @max_order: maximum number of pages you can allocate is 2^max_order
 * @cbs: pointer to callbacks that implements lower level allocation
 * @embed_metadata: set to non-zero to init page allocator with metadata
 *                  embedded in memory area
 * @pa_out: out param, the initialized page allocator struct
 *
 * This creates a page allocator that manages a memory area using
 * the buddy algorithm, derived from Linux.
 *
 * Example: You want to dedicate 16 GBs for a heap (allocate pages from).
 * To minimize the amount of metadata, you want
 *
 *        -- minimum allocation size to be 16 KBs (4 pages)
 *        -- maximum allocation size to be 16 MBs (4096 pages)
 *
 * There will be one struct lcd_page_block per 16 KBs (rather than
 * one struct page per 4 KBs as in Linux - less metadata if you don't
 * need 4 KB granularity / internal fragmentation isn't a problem).
 *
 * Moreover, you don't want to worry too much about bootstrapping and
 * setting aside memory/address range for the page allocator metadata
 * itself (you want to *embed* the metadata inside the heap).
 *
 * Finally, you want to demand-page in host memory as you need it.
 *
 * (What is metadata_malloc_order? See alloc_map_metadata_memory_chunk below.)
 *
 * Implementing the example: In order to keep this code somewhat
 * general, there are a few obligations on the user of it. But first,
 * lets figure out some of the parameters:
 *
 *       -- nr_pages_order = log(16 GBs / 4096) = 22
 *       -- min_order = 2 (2^2 = 4 pages)
 *       -- max_order = 12 (2^12 = 4096 pages)
 *       -- embed_metadata = 1 (true)
 * 
 * Next, we need to pass 4 callbacks to implement a lower level
 * map and allocation interface that the page allocator will call out
 * to at specific times:
 *
 *	 -- alloc_map_metadata_memory_chunk
 *              This is called when the code is allocating memory for
 *              the page allocator metadata (the struct lcd_page_allocator,
 *              the giant array of page blocks, etc.). The code will
 *              only try to allocate in chunks of 2^metadata_malloc_order
 *              pages.
 *
 *              This function is expected to allocate the real memory and
 *              map it at the correct offset. The function will be provided
 *              with the size (2^order) and the offset from the beginning.
 *              The function is expected to map offsets contiguously.
 *
 *              IMPORTANT #1: If you wanted metadata embedded, you should map
 *              the memory from the beginning of the memory area. If you
 *              get this wrong, it's not a serious problem, so long as you
 *              are consistent.
 *
 *              IMPORTANT #2: You should ensure that the starting address
 *              of the metadata is aligned at a 2^max_order-page boundary
 *              (i.e., 2^max_order * 4096 byte boundary). If you don't,
 *              things will still sort of work, but when you allocate pages,
 *              they will not be page aligned anymore ... (e.g., you'll get
 *              4 KBs of memory, but it won't be aligned at multiples
 *              of 0x1000).
 *
 *              Return lcd_resource_node that refers to allocated memory
 *              as an out parameter.
 *
 *      -- free_unmap_metadata_memory_chunk
 *              Free memory allocated for metadata. This is only called
 *              in error situations during initialization to free up
 *              allocated memory.
 *
 *      -- alloc_map_regular_mem_chunk
 *              This will be called when:
 *                   1 - It is non-NULL; and
 *                   2 - 2^max_order pages are free; and
 *                   3 - We try to allocate pages, and this leads to
 *                       allocating those 2^max_order pages, or some
 *                       subset thereof
 *              This is the demand paging in action. We want to allocate
 *              pages from a 2^max_order-page region of memory, but it's
 *              not backed by real memory. This function should allocate
 *              the real memory, and map it in so the pages become backed.
 *
 *              This function will provided with the calling page allocator,
 *              the page block at the beginning of the 2^max_order chunk,
 *              the offset into the memory area where the memory should be
 *              mapped, and the size (for now, will always be 
 *              order = max_order).
 *
 *              IMPORTANT: The function is supposed to know where the
 *              page allocator's memory area is in memory (the base/starting
 *              address). The offset is into this memory area (and should be
 *              added to the base).
 *
 *              Should return the lcd resource node for the memory, as an
 *              out parameter.
 *
 *	-- free_unmap_regular_mem_chunk
 *              This is called when a page block is freed or coalesced,
 *              and its order >= the order of its corresponding resource
 *              node.
 *
 *              For example, with demand paging, the page blocks on the
 *              2^max_order boundaries store a reference to the resource
 *              node that refers to the real host memory that backs that
 *              2^max_order-page region. But when that entire region is
 *              freed and coalesced, this function is called and should
 *              free and unmap the underyling host memory.
 *
 * Further note: If embed_metadata = 1, the page blocks that contain
 * that metadata will be marked as occupied (will not be on free lists). So,
 * they won't be returned in regular alloc requests.
 *
 * If these conditions aren't satisfied, create will fail:
 *
 *     1 - min_order <= max_order <= nr_pages_order
 *
 *     2 - if embed_metadata is true (non-zero):
 *
 *               min_order <= metadata_malloc_order <= max_order
 *
 *         Moreover, if we're doing demand paging with embedded metadata,
 *
 *               metadata_malloc_order == max_order
 *
 *         Why? Because we need proper alignment inside the memory area.
 *         The demand paging logic will get mixed up if memory chunks are
 *         backed by different sizes.
 */
int lcd_page_allocator_create(unsigned long nr_pages_order,
			unsigned int min_order,
			unsigned int metadata_malloc_order,
			unsigned int max_order,
			const struct lcd_page_allocator_cbs *cbs,
			int embed_metadata,
			struct lcd_page_allocator **pa_out);

/**
 * lcd_page_allocator_destroy -- Tear down most of the resources for allocator
 * @pa: the page allocator to tear down
 *
 * This is equivalent to calling free on every page block in the
 * memory area - except those that contain metadata (if the page allocator
 * was configured for embedding metadata).
 *
 * IMPORTANT: The caller is responsible for freeing the resources that
 * contain the page allocator metadata. This may involve traversing
 * a resource tree and deleting the nodes that refer to the pages
 * that contain the metadata (and returning the memory to the host).
 * Or it may mean returning the pages to *another* page allocator.
 */
void lcd_page_allocator_destroy(struct lcd_page_allocator *pa);

/**
 * lcd_page_allocator_alloc -- Allocate 2^order pages from memory area
 * @pa: the page allocator managing the memory area
 * @order: allocate 2^order pages
 *
 * Returns pointer to the page block for the beginning of the allocated
 * memory. (This is intended to be a low level function that other 
 * allocators - like the heap and ioremap ones - are built on top of.)
 *
 * If order < the allocator's min order, or order > allocator's max order,
 * this returns NULL.
 *
 * If out of memory, returns NULL. (There could be internal/external
 * fragmentation. Unlike Linux, we don't get fancy with migrating memory,
 * so this is more likely to happen with us.)
 */
struct lcd_page_block*
lcd_page_allocator_alloc(struct lcd_page_allocator *pa,
			unsigned int order);

/**
 * lcd_page_allocator_free -- Return 2^order pages back to allocator
 * @pa: the page allocator managing the memory area
 * @pb: the page block that refers to the beginning of the region to free
 * @order: the size (2^order pages) of the region to free
 *
 * If @pb or @order are wrong, this could lead to corrupting the page
 * allocator.
 */
void lcd_page_allocator_free(struct lcd_page_allocator *pa,
			struct lcd_page_block *pb,
			unsigned int order);

/**
 * lcd_page_block_to_offset -- Translate a page block to an offset into area
 * @pa: the page allocator managing the area
 * @pb: page block that refers to location to translate
 *
 * Each page block represents 2^min_order pages, where min_order was
 * configured when the page allocator was created. This function gives
 * the offset into the memory area that the page block refers to.
 *
 * It's similar in spirit to Linux's page_address.
 *
 * The caller is responsible for nowing the base address of the page
 * allocator's memory region. (This is intended as a low-level functions
 * other allocators are built on top of.)
 */
unsigned long lcd_page_block_to_offset(struct lcd_page_allocator *pa,
				struct lcd_page_block *pb);

/**
 * lcd_offset_to_page_block -- Look up the page block for offset into area
 * @pa: the page allocator managing the memory area
 * @offset: offset into memory area
 *
 * You have a virtual address, and you want to look up the corresponding
 * struct lcd_page_block. First, you determine the memory area it belongs
 * to (the pa). Now, you need to index into its giant array of page blocks.
 * That's what this function is for.
 *
 * Note that each page block represents 2^min_order pages of memory; so if
 * offset is not aligned to that, it will be rounded down to the nearest
 * multiple.
 *
 * Similar in spirit to Linux's virt_to_head_page.
 */
struct lcd_page_block*
lcd_offset_to_page_block(struct lcd_page_allocator *pa,
			unsigned long offset);
