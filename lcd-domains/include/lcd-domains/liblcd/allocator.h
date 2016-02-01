/*
 * allocator.h
 *
 * Code for creating instances of general buddy
 * allocators. Allocator metadata embedded inside memory
 * region (but not in the alloc'd chunks).
 *
 * Can't use page fault in, because this would be EPT violation.
 * (Maybe can have LCD handle EPT fault as virtualization exception,
 * but that's for another day ... and that may not be efficient for
 * a general allocator? would also need background thingamajig for
 * shrinking faulted in mem that's not being used)
 */

#include <linux/page.h>
#include <linux/list.h>

/* CAN'T ALLOC IN MEM REGION? - WHAT IF REGION IS UNCACHEABLE? */

/* Make it optional / possible to insert into mem region during init.
 *  -- initializer calls out to get memory for metadata
 *  -- callee allocs mem and maps it; maybe return value signals
 *     to insert into itself - takes mem, zeros it out, init resource
 *     tree, insert mem into tree; what about when allocator is freed?
 *                   - during destroy, 
 *     -- problem - alloc tree nodes during heap init . */

struct lcd_free_lists {
	unsigned int min_order, notify_order, max_order;
	struct lcd_mem_chunk *free_lists;
	unsigned int nr_free;

	/* Called when trying to alloc mc on 2^(backing order) boundary and 
	 * n is 
	 * null. (e.g. not backed by memory)). This function
	 * can be null.
	 */
	int (*alloc_notify)(struct lcd_free_lists *fl,
			struct lcd_mem_chunk *mc);
	/* Called when freeing mc, n is non-null, and mc's order is >= 
	 * n's order. This
	 * gives user chance to e.g. free mem that was backing this chunk
	 * of memory */
	void (*free_notify)(struct lcd_free_lists *fl,
			struct lcd_mem_chunk *mc);
};

struct lcd_page_block {
	struct list_head buddy_list;
	unsigned int order;
	struct lcd_resource_node *n;
};

struct allocator_callbacks {
	/* alloc notify */
	/* free notify */
	/* init alloc */
	/* exit dealloc */
};

struct lcd_page_allocator {

	unsigned int nr_pages_order;
	unsigned int min_order;
	unsigned int metadata_malloc_order;
	unsigned int max_order;
	int embed_metadata;

	struct list_head *free_lists;
	unsigned long nr_page_blocks_free;

	struct lcd_page_block *pb_array;

	const struct lcd_page_allocator_cbs *cbs;
};


int mk_page_allocator(
	unsigned long nr_pages_order,
	unsigned int min_order,
	unsigned int backing_order,
	unsigned int max_order,
	const struct allocator_callbacks *cbs);

/* Alloc 2^order mem chunks */
struct lcd_mem_chunk* lcd_page_allocator_alloc(struct lcd_page_allocator *a,
					unsigned int order);

void lcd_page_allocator_free(struct lcd_page_allocator *a,
			struct lcd_mem_chunk *base,
			unsigned int order);

void lcd_page_allocator_destroy(struct lcd_page_allocator *a);

unsigned long to_offset(struct lcd_mem_chunk *c);

struct lcd_mem_chunk* to_chunk(unsigned long offset);


/*
 * chicken and egg - tree root and node(s) before allocator is set up; provide
 * special slab user can alloc tree root and nodes from.
 *
 * what about tear down? user may malloc later nodes, so will use kfree; and
 * as user is tearing down tree, what if they delete the cap to pages that
 * contain nodes in the tree? ans: need to use flags on nodes to indicate
 * how they were allocated; if node is flagged as e.g. "embedded", don't use
 * kfree, only way to really "free" it is to destroy the page allocator
 * and return pages to microkernel.
 */


/* Usage examples */


/* heap setup: nr pages order = 24 (16 mb's); min order = 1, 
 *                        backing order = 10, max order = 11
 *
 *        init alloc: allocator says give me 2^10 pages of mem;
 *                    2^10 = 1 MB which is enough to hold meta data with
 *                    some left over
 *
 *                    allocator then writes metadata to pages, then
 *                    calls alloc on itself for required pages to hold
 *                    metadata
 *
 *  after allocator init'd, heap alloc's enough mem for struct page array
 *  for all 16 MBs, zero's it out.
 *
 *       alloc notify: allocator calls when tries to alloc page on
 *                     2^(notify order) boundary; heap will alloc
 *                     2^(notify order) pages from mk, put in resource
 *                     tree, and store in mem chunk
 *
 *       free notify: free them
 *
 *
 *   lcd_page_alloc --> lcd_page_allocator_alloc, then heap does
 *         to_offset on chunk, then idx's into struct page array
 *         note that size of chunks must be 1
 *
 *   lcd_page_free --> get page idx, then calc offset into heap, then
 *         use to_chunk to get lcd_mem_chunk; then lcd_page_allocator_free,
 *         passing along order
 *
 * virt_to_head_page --> virt = phys; phys - heap base = offset; 
 *             offset >> PAGE_SHIFT; then idx into page array
 *
 * page_address --> (page - page base) * 4096 = offset; 
 *       phys = heap base + offset; virt = phys
 */

/*
 * ioremap: nr pages order = 34 (16 gb's); min order = 10 (1 MB), 
 *          backing order = 12 (don't alloc notify), max order = 16 (64 MBs)
 *
 * (I don't know now that ioremap needs to be this big ...)
 *
 *         alloc_notify = null
 *
 *   ioremap_phys --> lcd_page_allocator_alloc( size to fit map )
 *          then insert resource into tree, attach node to lcd_mem_chunk
 *          that is returned
 *           
 *         free_notify = ioremapper removes node from tree
 * 
 *   iounmap_phys: phys addr - ioremap base = offset; to_chunk(offset) = c;
 *         lcd_page_allocator_free(c); fires free_notify, we get tree
 *         lookup fast and free (of course, we could have done the removal
 *         ourselves after looking up c)
 */
