/*
 * mem.c
 *
 * Copyright: University of Utah
 */

#include <linux/slab.h>
#include <linux/mm.h>
#include <libcap.h>
#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>

/* RESOURCE TREES -------------------------------------------------- */

/* 
 * There are two trees: One for contiguous memory (RAM and device memory)
 * and one for non-contiguous memory (vmalloc).
 *
 * For now, resource trees are per-thread (these are not the same
 * thing as the global memory interval tree). So, we don't use any
 * locks to protect them.
 *
 * We also expect non-isolated code to not be too tricky. For example,
 * we don't expect it to insert host memory as RAM and VMALLOC memory
 * simultaneously. Non-isolated code is trusted after all ...
 */
#define LCD_RESOURCE_TREE_CONTIGUOUS      0
#define LCD_RESOURCE_TREE_NON_CONTIGUOUS  1

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
	if (__lcd_memory_object_is_contiguous(mo))
		return mo_insert_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_CONTIGUOUS],
			mo,
			mo_cptr);
	else
		return mo_insert_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_NON_CONTIGUOUS],
			mo,
			mo_cptr);
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
	if (__lcd_memory_object_is_contiguous(mo))
		return mo_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_CONTIGUOUS],
			mo);
	else
		return mo_in_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_NON_CONTIGUOUS],
			mo);
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
	if (__lcd_memory_object_is_contiguous(mo))
		mo_remove_from_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_CONTIGUOUS],
			mo);
	else
		mo_remove_from_tree(
			t->lcd_resource_trees[LCD_RESOURCE_TREE_NON_CONTIGUOUS],
			mo);
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

int _lcd_vmalloc(unsigned long nr_pages, cptr_t *slot_out)
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
	 * Allocate vmalloc memory
	 */
	ret = __lcd_vmalloc(lcd, slot, nr_pages);
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

static int do_map(struct lcd *lcd, struct lcd_memory_object *mo, 
		struct cnode *cnode, cptr_t mo_cptr)
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
	ret = __lcd_do_map_memory_object(lcd, mo, cap_cnode_metadata(cnode),
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

int _lcd_mmap(cptr_t mo_cptr, unsigned int order, gpa_t base)
{
	gpa_t unused;
	/*
	 * We cheat and use lcd_map_phys since all it does is add
	 * the memory object to the physical address space resource
	 * tree.
	 */
	return lcd_map_phys(mo_cptr, order, &unused);
}

static void do_phys_unmap(struct lcd *lcd, struct lcd_memory_object *mo,
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
	__lcd_do_unmap_memory_object(current->lcd, mo,
				cap_cnode_metadata(mo_cnode));
}

void _lcd_munmap(cptr_t mo_cptr, gpa_t base)
{
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	int ret;
	/*
	 * Ignore base
	 *
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
	do_phys_unmap(current->lcd, mo, cnode);
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
	gpa_t unused;
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
	ret = lcd_map_phys(slot, order, &unused);
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
	/* gpa is ignored for non-isolated */
	_lcd_munmap(slot, __gpa(0));
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
	gpa_t unused;
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
	ret = lcd_map_phys(slot, order, &unused);
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
	/* gpa is ignored for non-isolated */
	_lcd_munmap(slot, __gpa(0));
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
	unsigned long actual_size, unused;
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
	ret = lcd_phys_to_cptr(gpa, &page_cptr, &actual_size, &unused);
	if (ret) {
		LIBLCD_ERR("warning: pages not found, so not freed");
		return;
	}
	if (order != ilog2(actual_size >> PAGE_SHIFT))
		LIBLCD_ERR("warning: order doesn't match actual order");
	/*
	 * Remove pages from resource tree ("unmap")
	 *
	 * gpa is ignored for non-isolated
	 */
	_lcd_munmap(page_cptr, __gpa(0));
	/*
	 * Free pages
	 */
	lcd_cap_delete(page_cptr);
}

void* lcd_vmalloc(unsigned long sz)
{
	struct lcd *lcd = current->lcd;
	unsigned long nr_pages;
	int ret;
	cptr_t slot;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	void *vptr = NULL;
	gva_t unused;
	/*
	 * Convert to number of pages, rounding up
	 */
	nr_pages = ALIGN(sz, PAGE_SIZE) >> PAGE_SHIFT;
	/*
	 * Do lower-level vmalloc so that mem goes into caller's cspace
	 */
	ret = _lcd_vmalloc(nr_pages, &slot);
	if (ret) {
		LIBLCD_ERR("lower level vmalloc failed");
		goto fail1;
	}
	/*
	 * "Map" memory object in caller's physical address space
	 * (all this really does is put the allocated vmalloc mem in the
	 * proper resource tree)
	 *
	 * Order is ignored for now for non-isolated
	 */
	ret = lcd_map_virt(slot, 0xbad, &unused);
	if (ret) {
		LIBLCD_ERR("internal error: putting mem obj in tree");
		goto fail2;
	}
	/*
	 * Look up memory object so we can get vptr
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail3;
	}
	vptr = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return vptr;

fail3:
	/* gpa is ignored for non-isolated */
	_lcd_munmap(slot, __gpa(0));
fail2:
	lcd_cap_delete(slot); /* frees vmalloc mem, etc. */
fail1:
	return NULL;
}

void lcd_vfree(void *vptr)
{
	cptr_t vmalloc_mem_cptr;
	int ret;
	unsigned long unused1, unused2;
	/*
	 * Resolve vptr to cptr
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)vptr),
			&vmalloc_mem_cptr, &unused1, &unused2);
	if (ret) {
		LIBLCD_ERR("warning: vmaloc mem not found, so not freed");
		return;
	}
	/*
	 * Remove vmalloc mem from resource tree ("unmap")
	 *
	 * gpa is ignored for non-isolated
	 */
	_lcd_munmap(vmalloc_mem_cptr, __gpa(0));
	/*
	 * Free vmalloc mem
	 */
	lcd_cap_delete(vmalloc_mem_cptr);
}

/* HIGH-LEVEL MAP/UNMAP ---------------------------------------- */

int lcd_map_phys(cptr_t pages, unsigned int order, gpa_t *base_out)
{
	int ret;
	struct page *p;
	struct cnode *cnode;
	struct lcd_memory_object *mo;
	/*
	 * Ignore order, etc., since non-isolated code already has
	 * access to all physical memory. (We could maybe check that
	 * the order is correct.)
	 *
	 * Look up memory object so we can get struct page pointer,
	 * and then translate to physical address
	 */
	ret = __lcd_get_memory_object(current->lcd, pages, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail1;
	}
	/*
	 * Ensure the memory object is for contiguous memory
	 */
	if (!__lcd_memory_object_is_contiguous(mo)) {
		LIBLCD_ERR("memory object is not contiguous; use lcd_map_virt instead");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * "Map" the pages (adds pages to proper resource tree)
	 */
	ret = do_map(current->lcd, mo, cnode, pages);
	if (ret) {
		LIBLCD_ERR("error mapping pages in resource tree");
		goto fail3;
	}
	p = mo->object;
	/* guest physical == host physical for non-isolated */
	*base_out = __gpa(hpa_val(va2hpa(page_address(p))));
	/*
	 * Release memory object
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);
	
	return 0;

fail3:
fail2:
	__lcd_put_memory_object(current->lcd, cnode, mo);
fail1:
	return ret;
}

int lcd_map_virt(cptr_t pages, unsigned int order, gva_t *gva_out)
{
	int ret;
	struct cnode *cnode;
	struct lcd_memory_object *mo;
	/*
	 * Ignore order
	 *
	 * Look up memory object so we can get virtual address.
	 */
	ret = __lcd_get_memory_object(current->lcd, pages, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail1;
	}
	/*
	 * Ensure this is RAM mem (lcd_map_virt doesn't do ioremap)
	 */
	if (!__lcd_memory_object_is_ram(mo)) {
		LIBLCD_ERR("cannot use lcd_map_virt for dev mem");
		goto fail2;
	}
	/*
	 * "Map" the memory (adds pages to proper resource tree)
	 */
	ret = do_map(current->lcd, mo, cnode, pages);
	if (ret) {
		LIBLCD_ERR("error mapping pages in resource tree");
		goto fail3;
	}
	
	/* guest virtual == host virtual for non-isolated */
	*gva_out = __gva(hva_val(__lcd_memory_object_hva(mo)));

	/*
	 * Release memory object
	 */
	__lcd_put_memory_object(current->lcd, cnode, mo);
	
	return 0;

fail3:
fail2:
	__lcd_put_memory_object(current->lcd, cnode, mo);
fail1:
	return ret;
}

void lcd_unmap_phys(gpa_t base, unsigned int order)
{
	int ret;
	cptr_t mo_cptr;
	unsigned long mo_size, unused;
	/*
	 * No real unmapping needs to be done, but we need to
	 * update the resource tree.
	 *
	 * Look up cptr for physical memory
	 */
	ret = lcd_phys_to_cptr(base, &mo_cptr, &mo_size, &unused);
	if (ret) {
		LIBLCD_ERR("phys not mapped?");
		return;
	}
	/*
	 * Remove from resource tree
	 *
	 * gpa is ignored for non-isolated
	 */
	_lcd_munmap(mo_cptr, __gpa(0));
}

void lcd_unmap_virt(gva_t base, unsigned int order)
{
	int ret;
	cptr_t mo_cptr;
	unsigned long mo_size, unused;
	/*
	 * No real unmapping needs to be done, but we need to
	 * update the resource tree.
	 *
	 * Look up cptr for virtual memory
	 */
	ret = lcd_virt_to_cptr(base, &mo_cptr, &mo_size, &unused);
	if (ret) {
		LIBLCD_ERR("virt not mapped?");
		return;
	}
	/*
	 * Remove memory object from resource tree
	 *
	 * gpa is ignored for non-isolated
	 */
	_lcd_munmap(mo_cptr, __gpa(0));
}

/* VOLUNTEER HELPERS -------------------------------------------------- */

static int volunteer_mem_obj(struct task_struct *t,
			void *mem_obj, unsigned long addr,
			unsigned long nr_pages,
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
	ret = __lcd_insert_memory_object(t->lcd, slot, mem_obj,
					nr_pages, sub_type, &mo);
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
	lcd_cptr_free(slot);
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
				page_to_pfn(base), (1UL << order),
				LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE,
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
				gpa_val(base), (1UL << order),
				LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM,
				slot_out);
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	unvolunteer_mem_obj(devmem);
}

/* "VOLUNTEERING" VMALLOC MEMORY ---------------------------------------- */

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned long nr_pages,
			cptr_t *slot_out)
{
	return volunteer_mem_obj(current, (void *)gva_val(base),
				gva_val(base), nr_pages,
				LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM,
				slot_out);
}

void lcd_unvolunteer_vmalloc_mem(cptr_t vmalloc_mem)
{
	unvolunteer_mem_obj(vmalloc_mem);
}

/* ADDRESS -> CPTR TRANSLATION ---------------------------------------- */

int lcd_phys_to_resource_node(gpa_t paddr, struct lcd_resource_node **n)
{
	/*
	 * Look in RAM/devmem tree
	 *
	 * (For vmalloc mem, should use lcd_virt_to_resource_node.)
	 */
	return lcd_resource_tree_search(
		current->lcd_resource_trees[LCD_RESOURCE_TREE_CONTIGUOUS],
		gpa_val(paddr),
		n);
}

int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned long *size_out,
		unsigned long *offset_out)
{
	int ret;
	struct lcd_resource_node *n;
	/*
	 * Resolve to resource node
	 */
	ret = lcd_phys_to_resource_node(paddr, &n);
	if (ret) {
		LIBLCD_ERR("address not found");
		goto fail1;
	}
	/*
	 * Pull out memory object cptr and order
	 */
	*c_out = n->cptr;
	*size_out = lcd_resource_node_size(n);
	*offset_out = gpa_val(paddr) - lcd_resource_node_start(n);

	return 0;

fail1:
	return ret;
}

int virt_to_resource_node(gva_t vaddr, struct lcd_resource_node **n,
			int *tree_id)
{
	gpa_t gpa;
	int ret;
	/*
	 * Look in non-contiguous mem first
	 */
	ret = lcd_resource_tree_search(
		current->lcd_resource_trees[LCD_RESOURCE_TREE_NON_CONTIGUOUS],
		gva_val(vaddr),
		n);
	if (ret) {
		/* 
		 * Not found in vmalloc tree
		 *
		 * Translate virtual-> physical and try physical tree.
		 * guest phys = host phys.
		 */
		gpa = __gpa(hpa_val(hva2hpa(__hva(gva_val(vaddr)))));
		*tree_id = LCD_RESOURCE_TREE_CONTIGUOUS;
		return lcd_phys_to_resource_node(gpa, n);
	}

	*tree_id = LCD_RESOURCE_TREE_NON_CONTIGUOUS;

	return ret;
}

int lcd_virt_to_resource_node(gva_t vaddr, struct lcd_resource_node **n)
{
	int unused;
	return virt_to_resource_node(vaddr, n, &unused);
}

int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned long *size_out)
{
	struct lcd_resource_node *n;
	int ret;
	int tree_id;
	/*
	 * Resolve to resource node
	 */
	ret = virt_to_resource_node(vaddr, &n, &tree_id);
	if (ret) {
		LIBLCD_ERR("address not found");
		goto fail1;
	}
	/*
	 * Pull out memory object cptr and order
	 */
	*c_out = n->cptr;
	*size_out = lcd_resource_node_size(n);
	if (tree_id == LCD_RESOURCE_TREE_NON_CONTIGUOUS) {
		/* Assumes only virtual addresses in non-contig tree */
		*offset_out = gva_val(vaddr) - lcd_resource_node_start(n);
	} else {
		*offset_out = gpa_val(lcd_gva2gpa(vaddr)) - 
			lcd_resource_node_start(n);
	}

	return 0;

fail1:
	return ret;
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
	/* Non-isolated code probably should never call this. */
	BUG();
}

/* ADDRESS TRANSLATION -------------------------------------------------- */

gpa_t lcd_gva2gpa(gva_t gva)
{
	return __gpa(__pa((void *)gva_val(gva)));
}

gva_t lcd_gpa2gva(gpa_t gpa)
{
	return __gva((unsigned long)__va(gpa_val(gpa)));
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_alloc_init_resource_tree);
EXPORT_SYMBOL(lcd_destroy_free_resource_tree);
EXPORT_SYMBOL(_lcd_alloc_pages_exact_node);
EXPORT_SYMBOL(_lcd_alloc_pages);
EXPORT_SYMBOL(_lcd_vmalloc);
EXPORT_SYMBOL(_lcd_mmap);
EXPORT_SYMBOL(_lcd_munmap);
EXPORT_SYMBOL(lcd_alloc_pages_exact_node);
EXPORT_SYMBOL(lcd_alloc_pages);
EXPORT_SYMBOL(lcd_free_pages);
EXPORT_SYMBOL(lcd_vmalloc);
EXPORT_SYMBOL(lcd_vfree);
EXPORT_SYMBOL(lcd_map_phys);
EXPORT_SYMBOL(lcd_map_virt);
EXPORT_SYMBOL(lcd_unmap_phys);
EXPORT_SYMBOL(lcd_unmap_virt);
EXPORT_SYMBOL(lcd_volunteer_pages);
EXPORT_SYMBOL(lcd_unvolunteer_pages);
EXPORT_SYMBOL(lcd_volunteer_dev_mem);
EXPORT_SYMBOL(lcd_unvolunteer_dev_mem);
EXPORT_SYMBOL(lcd_volunteer_vmalloc_mem);
EXPORT_SYMBOL(lcd_unvolunteer_vmalloc_mem);
EXPORT_SYMBOL(lcd_phys_to_cptr);
EXPORT_SYMBOL(lcd_phys_to_resource_node);
EXPORT_SYMBOL(lcd_virt_to_cptr);
EXPORT_SYMBOL(lcd_virt_to_resource_node);
EXPORT_SYMBOL(lcd_virt_to_head_page);
EXPORT_SYMBOL(lcd_page_address);
EXPORT_SYMBOL(lcd_free_memcg_kmem_pages);
EXPORT_SYMBOL(lcd_gva2gpa);
EXPORT_SYMBOL(lcd_gpa2gva);
