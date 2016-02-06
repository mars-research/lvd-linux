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
 *
 * We also expect non-isolated code to not be too tricky. For example,
 * we don't expect it to insert host memory as RAM and VMALLOC memory
 * simultaneously (one piece of physical memory, but also mapped via
 * vmalloc). Non-isolated code is trusted after all ...
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

static int mo_insert_in_trees(struct task_struct *t, 
			struct lcd_memory_object *mo,
			cptr_t mo_cptr)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_insert_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo,
			mo_cptr);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_insert_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo,
			mo_cptr);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_insert_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
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

static int mo_in_trees(struct task_struct *t, struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
			mo);
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		return -EINVAL;
	}
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

static void mo_remove_from_trees(struct task_struct *t, 
				struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return mo_remove_from_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return mo_remove_from_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_DEV_MEM_IDX],
			mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return mo_remove_from_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_MEM_IDX],
			mo);
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		return;
	}
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

int __lcd_mmap(struct lcd *lcd, struct lcd_memory_object *mo, 
	cptr_t mo_cptr, gpa_t base)
{
	int ret;
	/*
	 * Check if it's already in a resource tree
	 */
	if (mo_in_trees(current, mo)) {
		LIBLCD_ERR("memory object already mapped");
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * No; put it in the correct one, depending on its type
	 */
	ret = mo_insert_in_trees(current, mo, mo_cptr);
	if (ret) {
		LIBLCD_ERR("insert into resource tree failed");
		goto fail2;
	}
	/*
	 * "Map" in physical address space (this is a no-op for
	 * non-isolated code right now)
	 */
	ret = __lcd_mem_object_map(lcd, mo, cap_cnode_metadata(cnode),
				__gpa(0));
	if (ret) {
		LIBLCD_ERR("physical map failed");
		goto fail3;
	}

	return 0;

fail3:
	mo_remove_from_trees(current, mo);
fail2:
fail1:
	return ret;
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
	 * Do the map
	 */
	ret = __lcd_mmap(lcd, mo, mo_cptr, base);
	if (ret) {
		LIBLCD_ERR("error mapping mem object");
		goto fail2;
	}
	/*
	 * Release locks
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);

	return 0;

fail2:
	__lcd_put_memory_object(current->lcd, cnode, mo);
fail1:
	return ret;
}

void __lcd_munmap(struct lcd *lcd, struct lcd_memory_object *mo,
		struct cnode *mo_cnode)
{
	/*
	 * Remove from resource trees
	 */
	mo_remove_from_trees(current, mo);
	/*
	 * "Unmap" from physical address space (this is a no-op for
	 * non-isolated code right now)
	 */
	__lcd_mem_object_unmap(current->lcd, mo,
			cap_cnode_metadata(cnode));
}

void _lcd_munmap(cptr_t mo_cptr)
{
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	int ret;
	/*
	 * Get and lock the memory object
	 */
	ret = __lcd_get_memory_object(current->lcd, mo_cptr, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("error looking up memory object");
		goto fail1;
	}
	/*
	 * Do the unmap
	 */
	__lcd_munmap(current->lcd, mo, cnode);
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
	 * "Map" memory object in caller's physical address space
	 * (all this really does is put the allocated pages in the
	 * proper resource tree)
	 */
	ret = _lcd_mmap(slot, __gpa(0));
	if (ret) {
		LIBLCD_ERR("internal error: putting mem obj in tree");
		goto fail2;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail3;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail3:
	_lcd_munmap(slot);
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
	 * "Map" memory object in caller's physical address space
	 * (all this really does is put the allocated pages in the
	 * proper resource tree)
	 */
	ret = _lcd_mmap(slot, __gpa(0));
	if (ret) {
		LIBLCD_ERR("internal error: putting mem obj in tree");
		goto fail2;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail3;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail3:
	_lcd_munmap(slot);
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
	 * Remove pages from resource tree ("unmap")
	 */
	_lcd_munmap(page_cptr);
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
	 * Look up memory object so we can get struct page pointer,
	 * and then translate to physical address
	 */
	ret = __lcd_get_memory_object(current->lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail1;
	}
	/*
	 * "Map" the pages (adds pages to proper resource tree)
	 */
	ret = __lcd_mmap(current->lcd, mo, cnode, __gpa(0));
	if (ret) {
		LIBLCD_ERR("error mapping pages in resource tree");
		goto fail2;
	}

	p = mo->object;
	/* guest physical == host physical for non-isolated */
	*base_out = __gpa(hpa_val(va2hpa(page_address(p))));
	/*
	 * Release memory object
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);
	
	return 0;

fail2:
	__lcd_put_memory_object(current->lcd, cnode, mo);
fail1:
	return ret;
}

int lcd_map_virt(gpa_t base, unsigned int order, gva_t *gva_out)
{
	int ret;
	cptr_t mo_cptr;
	unsigned int mo_order;
	/*
	 * On x86_64, all RAM is mapped already.
	 *
	 * But we still check to ensure non-isolated code has
	 * access to physical address via capabilities and has
	 * it mapped in its resource tree.
	 *
	 * XXX: For arch's with smaller virtual address spaces, 
	 * we need to kmap or similar.
	 *
	 * guest virtual == host virtual for non-isolated
	 */
	ret = lcd_phys_to_cptr(base, &mo_cptr, &mo_order);
	if (ret) {
		LIBLCD_ERR("phys not mapped?");
		goto fail1;
	}
	/*
	 * If we got a "hit", then there's no need to get the actual
	 * memory object (base is guest physical == host physical for
	 * non-isolated)
	 */
	
	*gva_out = __gva(hva_val(pa2hva(gpa_val(base))));
	
	return 0;

fail1:
	return ret;
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
	int ret;
	cptr_t mo_cptr;
	unsigned int mo_order;
	/*
	 * No real unmapping needs to be done, but we need to
	 * update the resource tree.
	 *
	 * Look up cptr for physical memory
	 */
	ret = lcd_phys_to_cptr(base, &mo_cptr, &mo_order);
	if (ret) {
		LIBLCD_ERR("phys not mapped?");
		return;
	}
	/*
	 * Remove from resource tree
	 */
	_lcd_munmap(mo_cptr);
}

void lcd_unmap_virt(gva_t base, unsigned int order)
{
	/*
	 * This is truly a no-op because all we did in lcd_map_virt
	 * was just check if the caller has the physical address in
	 * its resource tree.
	 *
	 * XXX: At least for x86_64. For arch's with smaller virtual
	 * address spaces, we need to kunmap or similar.
	 */
}

void lcd_unmap_both(gva_t base, unsigned int order)
{
	gpa_t base_gpa;
	/*
	 * Get base as a physical address
	 *
	 * guest virtual == host virtual, and
	 * guest physical == host physical for non-isolated
	 */
	base_gpa = __gpa(hpa_val(hva2hpa(__hva(gva_val(base)))));
	/*
	 * Unmap in virtual
	 */
	lcd_unmap_virt(base, order);
	/*
	 * Unmap in physical
	 */
	lcd_unmap_phys(base_gpa, order);
}

/* VOLUNTEER HELPERS -------------------------------------------------- */

static int volunteer_mem_obj(struct task_struct *t,
			void *mem_obj, unsigned long addr,
			unsigned int order,
			enum lcd_microkernel_type_id sub_type,
			cptr_t *slot_out)
{
	int ret;
	struct lcd_mem_itree_node *n;
	cptr_t slot;
	struct lcd_memory_object *mo;
	/*
	 * Check if object has already been volunteered
	 *
	 * XXX: Admittedly, this check is weak because someone else
	 * may have volunteered some subset of the pages now being
	 * volunteered, etc. etc. Non-isolated code is trusted ...
	 */
	ret = __lcd_mem_itree_get(addr, sub_type, &n);
	if (!ret) {
		__lcd_mem_itree_put(n);
		LIBLCD_ERR("memory has already been volunteered");
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Memory object not volunteered; alloc a cptr and insert
	 * into caller's cspace and global memory interval tree
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	ret = __lcd_insert_memory_object(t->lcd, base, order, sub_type, &mo);
	if (ret) {
		LIBLCD_ERR("error inserting new memory object");
		goto fail3;
	}
	/*
	 * ==================================================
	 * capability subystem owns pages after this point;
	 * to free up everything, call lcd_cap_delete on the cptr
	 */

	*slot_out = slot;

fail3:
	lcd_free_cptr(slot);
fail2:
fail1:
	return ret;
}

static void unvolunteer_mem_obj(cptr_t mo_cptr)
{
	/*
	 * So long as the caller revoked all access to the memory
	 * object, this triggers removal from the memory interval tree,
	 * and frees up internal data (struct lcd_memory_object,
	 * metadata, and so on).
	 */
	lcd_cap_delete(mo_cptr);
}

/* "VOLUNTEERING" PAGES ---------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	return volunteer_mem_obj(current, base,
				page_pfn(base), order,
				LCD_MICROKERNEL_TYPE_ID_PAGE,
				slot_out);
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	unvolunteer_mem_obj(pages);
}

/* "VOLUNTEERING" DEVICE MEMORY ---------------------------------------- */

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	return volunteer_mem_obj(current, (void *)gpa_val(base),
				gpa_val(base), order,
				LCD_MICROKERNEL_TYPE_ID_DEV_MEM,
				slot_out);
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	unvolunteer_mem_obj(devmem);
}

/* "VOLUNTEERING" VMALLOC MEMORY ---------------------------------------- */

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned int order,
			cptr_t *slot_out)
{
	return volunteer_mem_obj(current, (void *)gva_val(base),
				gva_val(base), order,
				LCD_MICROKERNEL_TYPE_ID_VMALLOC_MEM,
				slot_out);
}

void lcd_unvolunteer_vmalloc_mem(cptr_t vmalloc_mem)
{
	unvolunteer_mem_obj(vmalloc_mem);
}

/* ADDRESS -> CPTR TRANSLATION ---------------------------------------- */

int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned int *order_out)
{
	int ret;
	struct lcd_resource_node *n;
	/*
	 * Look in RAM/devmem tree
	 *
	 * (For vmalloc mem, should use lcd_virt_to_cptr.)
	 */
	ret = lcd_resource_tree_search(
		current->lcd_resource_trees[LCD_RESOURCE_TREE_RAM_IDX],
		gpa_val(paddr),
		&n);
	if (ret) {
		LIBLCD_ERR("address not found");
		goto fail1;
	}
	/*
	 * Pull out memory object cptr and order
	 */
	*c_out = n->cptr;
	*order_out = n->order;

	return 0;

fail1:
	return ret;
}

int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned int *order_out)
{
	gpa_t gpa;
	struct lcd_resource_node *n;
	/*
	 * Look in vmalloc tree first
	 */
	ret = lcd_resource_tree_search(
		current->lcd_resource_trees[LCD_RESOURCE_TREE_VMALLOC_IDX],
		gva_val(vaddr),
		&n);
	if (ret) {
		/* 
		 * Not found in vmalloc tree
		 *
		 * Translate virtual-> physical and try physical tree.
		 * guest phys = host phys.
		 */
		gpa = __gpa(hpa_val(hva2hpa(__hva(gva_val(vaddr)))));
		return lcd_phys_to_cptr(gpa, c_out, order_out);
	}
	/*
	 * Pull out memory object cptr and order
	 */
	*c_out = n->cptr;
	*order_out = n->order;

	return 0;
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
