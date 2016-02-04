/*
 * mem.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd-domains/liblcd.h>
#include "../microkernel/internal.h"

/* RESOURCE TREES -------------------------------------------------- */

/* 
 * There are two trees: One for RAM and device memory (host physical
 * addresses), one for vmalloc (host virtual addresses for memory that
 * need not be contiguous).
 *
 * For now, resource trees are per-thread (these are not the same
 * thing as the global memory interval tree). So, we don't use any
 * locks to protect them.
 */
#define LCD_RESOURCE_TREE_RAM_IDX      0
#define LCD_RESOURCE_TREE_DEV_MEM_IDX  0
#define LCD_RESOURCE_TREE_VMALLOC_IDX  1

int lcd_alloc_init_resource_tree(struct lcd_resource_tree **t_out)
{
	int ret;
	struct lcd_resource_tree *t;
	/*
	 * Create an empty resource tree for address -> cptr
	 * translation.
	 */
	t = kzalloc(sizeof(struct lcd_resource_tree),
					GFP_KERNEL);
	if (!t){
		LCD_ERR("alloc'ing resource tree");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Initialize it
	 */
	ret = lcd_resource_tree_init(t);
	if (ret) {
		LCD_ERR("error initializing resource tree");
		goto fail2;
	}

	*t_out = t;

	return 0;

fail2:
	kfree(t);
fail1:
	return ret;
}

void lcd_destroy_free_resource_tree(struct lcd_resource_tree *t)
{
	struct lcd_resource_node *n, *next;
	/*
	 * Destroy all of the nodes that are still lying around in the
	 * tree.
	 */
	n = lcd_resource_tree_first(t);
	while (n) {
		/*
		 * If this node was kmalloc'd, kfree it.
		 */
		next = lcd_resource_tree_next(n);
		if (lcd_resource_node_flags(n) & LCD_RESOURCE_NODE_KMALLOC)
			kfree(n);
		n = next;
	}
	/*
	 * Free the root (for kliblcd, this is always kmalloc'd)
	 */
	kfree(t);
}

/* LOW-LEVEL PAGE ALLOC -------------------------------------------------- */

int _lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
				unsigned int order, cptr_t *slot_out)
{
	struct lcd *lcd = current->lcd;
	cptr_t slot;
	int ret;
	/*
	 * Allocate a slot in caller's cspace
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Allocate pages
	 */
	ret = __lcd_alloc_pages_exact_node(lcd, slot, nid, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages exact node failed");
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
	struct lcd *lcd = current->lcd;
	cptr_t slot;
	int ret;
	/*
	 * Allocate a slot in caller's cspace
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Allocate pages
	 */
	ret = __lcd_alloc_pages(lcd, slot, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages failed");
		goto fail2;
	}

	*slot_out = slot;
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

/* LOW-LEVEL MAP -------------------------------------------------- */

static int mo_insert_in_tree(struct lcd_resource_tree *t, 
			struct lcd_memory_object *mo,
			cptr_t mo_cptr)
{
	struct lcd_resource_node *n;
	int ret;
	/*
	 * Create and init node
	 */
	n = kzalloc(sizeof(*n), GFP_KERNEL);
	if (!n) {
		LIBLCD_ERR("out of mem");
		ret = -ENOMEM;
		goto fail1;
	}
	n->cptr = mo_cptr;
	n->nr_pages_order = mo->order;
	n->it_node.start = __lcd_memory_object_start(mo);
	n->it_node.last = __lcd_memory_object_last(mo);
	/*
	 * Insert into tree
	 */
	lcd_resource_tree_insert(t, n);

	return 0;

fail1:
	return ret;
}

static int mo_insert_in_trees(struct lcd *lcd, struct lcd_memory_object *mo,
			cptr_t mo_cptr)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_insert_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo,
			mo_cptr);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_insert_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo,
			mo_cptr);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_insert_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
			mo,
			mo_cptr);
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		return -EINVAL;
	}
}

static int mo_in_tree(struct lcd_resource_tree *t, 
		struct lcd_memory_object *mo)
{
	struct lcd_resource_node *unused;
	/*
	 * Returns zero if search found something
	 */
	return lcd_resource_tree_search(t,
					__lcd_memory_object_start(mo),
					&unused) == 0;
}

static int mo_in_trees(struct lcd *lcd, struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_in_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
			mo);
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		return -EINVAL;
	}
}

int _lcd_mmap(cptr_t mo, gpa_t base)
{
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	int ret;
	/*
	 * Ignore gpa arg - non-isolated code cannot change physical
	 * mappings
	 *
	 *	
	 * Get and lock the memory object
	 */
	ret = __lcd_get_memory_object(current->lcd, mo,	&cnode, &mo);
	if (ret) {
		LIBLCD_ERR("error looking up memory object");
		goto fail1;
	}
	/*
	 * Check if it's already in a resource tree
	 */
	if (mo_in_trees(current->lcd, mo)) {
		LIBLCD_ERR("memory object already mapped");
		goto fail2;
	}
	/*
	 * No; put it in the correct one, depending on its type
	 */
	ret = mo_insert_in_trees(current->lcd, mo, mo_cptr);
	if (ret) {
		LIBLCD_ERR("insert into resource tree failed");
		goto fail3;
	}
	/*
	 * Release locks
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);

	return 0;

fail3:
fail2:
	__lcd_put_memory_object(current->lcd, cnode, mo);
fail1:
	return ret;
}

static void mo_remove_from_tree(struct lcd_resource_tree *tree, 
				struct lcd_memory_object *mo)
{
	struct lcd_resource_node *node;
	int ret;
	/*
	 * Look up node in tree
	 */
	ret = lcd_resource_tree_search(tree,
				__lcd_memory_object_start(mo),
				&node);
	if (ret) {
		LIBLCD_ERR("couldn't find memory object, not mapped?");
		return;
	}
	/*
	 * Remove it from the tree and delete it
	 */
	lcd_resource_tree_remove(tree, node);
	kfree(node);
}

static void mo_remove_from_trees(struct lcd *lcd, struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_remove_from_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_remove_from_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_remove_from_tree(
			lcd->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
			mo);
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		return;
	}
}

void _lcd_munmap(cptr_t mo)
{
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	int ret;
	/*
	 * Get and lock the memory object
	 */
	ret = __lcd_get_memory_object(current->lcd, mo,	&cnode, &mo);
	if (ret) {
		LIBLCD_ERR("error looking up memory object");
		goto fail1;
	}
	/*
	 * Remove from resource trees
	 */
	mo_remove_from_trees(current->lcd, mo);
	/*
	 * Release locks
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);

fail1:
	return;
}

/* HIGH-LEVEL ALLOC/FREE ---------------------------------------- */

struct page *lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
					unsigned int order)
{
	struct lcd *lcd = current->lcd;
	int ret;
	cptr_t slot;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	struct page *p = NULL;
	/*
	 * Do lower-level alloc so that pages go into caller's cspace
	 */
	ret = _lcd_alloc_pages_exact_node(nid, flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("lower level alloc failed");
		goto fail1;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail2;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail2:
	lcd_cap_delete(slot); /* frees pages, etc. */
fail1:
	return NULL;
}

struct page *lcd_alloc_pages(unsigned int flags, unsigned int order)
{
	struct lcd *lcd = current->lcd;
	int ret;
	cptr_t slot;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	struct page *p = NULL;
	/*
	 * Do lower-level alloc so that pages go into caller's cspace
	 */
	ret = _lcd_alloc_pages(flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("lower level alloc failed");
		goto fail1;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail2;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail2:
	lcd_cap_delete(slot); /* frees pages, etc. */
fail1:
	return NULL;
}

void lcd_free_pages(struct page *base, unsigned int order)
{
	cptr_t page_cptr;
	int ret;
	gpa_t gpa;
	unsigned int actual_order;
	/*
	 * Translate page to physical address.
	 *
	 * This line asserts that guest physical == host physical for
	 * non-isolated code.
	 */
	gpa = __gpa(hpa_val(va2hpa(page_address(base))));
	/*
	 * Resolve pages to cptr
	 */
	ret = lcd_phys_to_cptr(gpa, &page_cptr, &actual_order);
	if (ret) {
		LIBLCD_ERR("warning: pages not found, so not freed");
		return;
	}
	if (order != actual_order)
		LIBLCD_ERR("warning: order doesn't match actual order");
	/*
	 * Free pages
	 */
	lcd_cap_delete(page_cptr);
}

/* HIGH-LEVEL MAP/UNMAP ---------------------------------------- */

int lcd_map_phys(cptr_t pages, unsigned int order, gpa_t *base_out)
{
	int ret;
	struct page *p;
	/*
	 * Ignore order, etc., since non-isolated code already has
	 * access to all physical memory. (We could maybe check that
	 * the order is correct.)
	 *
	 * XXX: It's significant that we don't call
	 * __lcd_map_memory_object. This means we won't update any
	 * mapping metadata for the non-isolated thread's pages
	 * capability, and so nothing will be done when the
	 * capability is revoked.
	 *
	 * Look up memory object so we can get struct page pointer,
	 * and then translate to physical address
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail1;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);
	
	/* guest physical == host physical for non-isolated */
	*base_out = __gpa(hpa_val(va2hpa(page_address(p))));
	
	return 0;
fail1:
	return ret;
}

int lcd_map_virt(gpa_t base, unsigned int order, gva_t *gva_out)
{
	/*
	 * On x86_64, all RAM is mapped already. Note that we
	 * don't bother checking whether the non-isolated thread
	 * actually has a capability to the RAM.
	 *
	 * XXX: For arch's with smaller virtual address spaces, 
	 * we need to kmap or similar.
	 *
	 * guest virtual == host virtual for non-isolated
	 */
	*gva_out = __gva(hva_val(pa2hva(gpa_val(base))));
	
	return 0;
}

int lcd_map_both(cptr_t pages, unsigned int order, gva_t *gva_out)
{
	int ret;
	gpa_t gpa;
	/*
	 * "Map" in physical (this is more like cptr -> phys addr)
	 */
	ret = lcd_map_phys(pages, order, &gpa);
	if (ret)
		return ret;
	/*
	 * "Map" in virtual
	 */
	return lcd_map_virt(gpa, order, gva_out);
}

void lcd_unmap_phys(gpa_t base, unsigned int order)
{
	/*
	 * No-op. No need to unmap in physical for the host.
	 *
	 * We don't mark the non-isolated thread's capability metadata
	 * as is_mapped to begin with, so no need to unset it.
	 */
}

void lcd_unmap_virt(gva_t base, unsigned int order)
{
	/*
	 * No-op.
	 *
	 * XXX: At least for x86_64. For arch's with smaller virtual
	 * address spaces, we need to kunmap or similar.
	 *
	 * We don't mark the non-isolated thread's capability metadata
	 * as is_mapped to begin with, so no need to unset it.
	 */
}

void lcd_unmap_both(gva_t base, unsigned int order)
{
	/*
	 * No-op.
	 */
}

/* "VOLUNTEERING" PAGES ---------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	/* insert into cspace */

	/* insert into *global* rb tree */
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	/* update rb tree? */

	lcd_cap_delete(pages);
}

/* "VOLUNTEERING" DEVICE MEMORY ---------------------------------------- */

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	/* insert into cspace */

	/* insert into rb tree */
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	lcd_cap_delete(devmem);
}

/* ADDRESS -> CPTR TRANSLATION ---------------------------------------- */

int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned int *order_out)
{
	/* rb tree lookup */
}

int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned int *order_out)
{
	gpa_t gpa;
	/*
	 * Translate virtual-> physical. guest phys = host phys.
	 */
	gpa = __gpa(hpa_val(hva2hpa(__hva(gva_val(vaddr)))));
	return lcd_phys_to_cptr(gpa, c_out, order_out);
}

/* MISCELLANEOUS -------------------------------------------------- */

struct page *lcd_virt_to_head_page(const void *addr)
{
	/* 
	 * Note: we don't bother checking that addr corresponds to
	 * memory that the caller has a capability to.
	 */
	return virt_to_head_page(addr);
}

void *lcd_page_address(const struct page *page)
{
	/* 
	 * Note: we don't bother checking that page corresponds to
	 * memory that the caller has a capability to.
	 */
	return page_address(page);
}

void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order)
{
	/* Non-isolated code probably won't ever call this. */
	free_memcg_kmem_pages(addr, order);
}
