/*
 * mem.c -- Architecture-independent memory related stuff - page allocation,
 *          mapping, etc.
 *
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include "internal.h"

/* ALLOC -------------------------------------------------- */

static void do_insert_pages(struct lcd *caller, struct page *p, 
			unsigned int order)
{
	struct lcd_memory_object *mo;
	struct lcd_mapping_metadata *meta;
	struct cnode *cnode;
	/*
	 * Set up metadata
	 */
	mo = kmalloc(sizeof(*mo));
	if (!mo) {
		LCD_ERR("kmalloc error");
		ret = -ENOMEM;
		goto fail1;
	}
	meta = kzalloc(sizeof(*meta)); /* set is_mapped = 0 */
	if (!meta) {
		LCD_ERR("kmalloc error");
		ret = -ENOMEM;
		goto fail2;
	}
	mo->sub_type = LCD_MICROKERNEL_TYPE_ID_PAGE;
	mo->object = p;
	mo->order = order;
	/*
	 * Insert into caller's cspace
	 */
	ret = cap_insert(caller->cspace, slot, 
			mo, 
			__lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_PAGE));
	if (ret) {
		LCD_ERR("insert");
		goto fail3;
	}
	/*
	 * ================================================
	 * After this point, mo is managed by libcap, and will be
	 * freed when the last capability to it goes away. In particular,
	 * if we fail below and do cap_delete, this will automatically
	 * delete mo (in the delete callback). This is subtle, so be
	 * careful.
	 *
	 * Store metadata
	 */
	ret = cap_cnode_get(caller->cspace, slot, &cnode);
	if (ret) {
		LCD_ERR("metadata");
		/* don't use "label" chain */
		cap_delete(caller->cspace, slot);
		kfree(mo);
		return ret; 
	}
	cap_cnode_set_metadata(cnode, meta);
	cap_cnode_put(cnode);
	/*
	 * ==================================================
	 * After this point, meta is now also managed implicitly
	 * by libcap.
	 */
	return 0;

fail3:
	kfree(meta);
fail2:
	kfree(mo);
fail1:
	return ret;
}

int __lcd_alloc_pages_exact_node(struct lcd *caller, cptr_t slot, int nid,
				unsigned int flags, unsigned int order)
{
	int ret;
	struct page *p;
	/*
	 * Allocate zero'd pages on node
	 */
	p = alloc_pages_exact_node(nid, flags | __GFP_ZERO, order);
	if (!p) {
		LCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Insert into caller's cspace
	 */
	ret = do_insert_pages(caller, p, order);
	if (ret) {
		LCD_ERR("failed to insert page capability into caller's cspace");
		goto fail2;
	}

	return 0;

fail2:
	__free_pages(p, order);
fail1:
	return ret;
}

int __lcd_alloc_pages(struct lcd *caller, cptr_t slot,
		unsigned int flags, unsigned int order)
{
	int ret;
	struct page *p;
	/*
	 * Allocate zero'd pages
	 */
	p = alloc_pages(flags | __GFP_ZERO, order);
	if (!p) {
		LCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Insert into caller's cspace
	 */
	ret = do_insert_pages(caller, p, order);
	if (ret) {
		LCD_ERR("failed to insert page capability into caller's cspace");
		goto fail2;
	}

	return 0;

fail2:
	__free_pages(p, order);
fail1:
	return ret;
}

/* LOOKUP -------------------------------------------------- */

static int lookup_memory_object(struct cspace *cspace, cptr_t slot, 
				struct cnode **cnode)
{
	int ret;
	cap_type_t t;
	/*
	 * Look up
	 */
	ret = cap_cnode_get(cspace, slot, cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm it's a memory object
	 */
	t = cap_cnode_type(*cnode);
	if (t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_PAGE) &&
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_KPAGE) &&
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_DEV_MEM)) {
		LCD_ERR("not a page");
		goto fail2;
	}

	return 0;

fail2:
	cap_cnode_put(*cnode);
fail1:
	return ret;
}

int __lcd_get_memory_object(struct lcd *caller, cptr_t mo_cptr, 
			struct cnode **cnode, 
			struct lcd_memory_object **mo_out)
{
	int ret;
	/*
	 * Look up and lock cnode containing memory object
	 */
	ret = lookup_memory_object(cspace, mo_cptr, cnode);
	if (ret)
		goto fail1;
	*mo_out = cap_cnode_object(*cnode);
	/*
	 * XXX: for now, we assume we won't be touching any of the 
	 * memory in a way that could race with others - so
	 * it's not necessary to take some other lock.
	 */
	
	return 0; /* caller should match with __lcd_put_memory_object */

fail1:
	return ret;
}

void __lcd_put_memory_object(struct lcd *caller, struct cnode *cnode, 
			struct lcd_memory_object *mo)
{
	/*
	 * (no unlock on memory object - see __lcd_get_memory_object)
	 *
	 * Release cnode
	 */
	cap_cnode_put(cnode);
}

/* MAPPING -------------------------------------------------- */

static int memory_object_hpa(struct lcd_memory_object *mo,
			hpa_t *hpa_base)
{
	hpa_t hpa_base;

	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		*hpa_base = va2hpa(page_address(mo->object));
		break;
	case LCD_MICROKERNEL_TYPE_ID_DEV_MEM:
		*hpa_base = hpa_val((unsigned long)mo->object);
		break;
	default:
		LCD_ERR("unexpected memory object type: %d\n",
			mo->sub_type);
		return -EINVAL;
	}

	return 0;
}

int __lcd_map_pages(struct lcd *caller, gpa_t gpa, hpa_t hpa,
		unsigned int num_pages)
{
	/*
	 * Only isolated VMs should be landing here.
	 */
	BUG_ON(caller->type != LCD_ISOLATED);
	/* 
	 * Create, and do not overwrite
	 */
	return lcd_arch_ept_map_range(caller->lcd_arch, gpa, hpa, num_pages);
}

void __lcd_unmap_pages(struct lcd *caller, gpa_t gpa, unsigned int num_pages)
{
	/*
	 * Only isolated VMs should be landing here.
	 */
	BUG_ON(caller->type != LCD_ISOLATED);
	int ret;
	ret = lcd_arch_ept_unmap_range(caller->lcd_arch, gpa, num_pages);
	if (ret)
		LCD_DEBUG(LCD_DEBUG_ERR, 
			"some error unmapping");
}

int __lcd_map_memory_object(struct lcd *caller, cptr_t mo_cptr, gpa_t base)
{
	int ret;
	struct lcd_memory_object *mo;
	struct lcd_mapping_metadata *meta;
	struct cnode *cnode;
	hpa_t base_hpa;
	/*
	 * Look up memory object in caller's cspace
	 */
	ret = __lcd_get_memory_object(caller->cspace, mo_cptr, &cnode, &mo);
	if (ret)
		goto fail1;
	meta = cap_cnode_metadata(cnode);
	/*
	 * If memory object is already mapped, fail
	 */
	if (!meta) {
		LCD_ERR("lookup before meta set?");
		ret = -EIO;
		goto fail2;
	}
	if (meta->is_mapped) {
		LCD_ERR("memory object already mapped");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * Get host physical address of start of memory object
	 */
	ret = memory_object_hpa(mo, &hpa_base);
	if (ret) {
		LCD_ERR("invalid memory object");
		goto fail2;
	}
	/*
	 * Map memory object.
	 *
	 * XXX: No matter what the memory object is, we map it
	 * the same way. This is kind of subtle. We can get away
	 * with it because we allow the LCD to use the PAT to control
	 * caching, and we always map memory as WB in guest physical.
	 */
	ret = __lcd_map_pages(lcd, gpa, hpa_base, 1 << mo->order)
	if (ret) {
		LCD_ERR("map");
		goto fail2;
	}
	/*
	 * Mark page as mapped, and where it's mapped
	 */
	meta->is_mapped = 1;
	meta->where_mapped = gpa;
	/*
	 * Release cnode, etc.
	 */
	__lcd_put_memory_object(caller, cnode, mo);

	return 0;

fail2:
	__lcd_put_memory_object(caller, cnode, mo);
fail1:
	return ret;
}

void __lcd_unmap_memory_object(struct lcd *caller, cptr_t mo_cptr)
{
	int ret;
	gpa_t gpa;
	struct lcd_memory_object *mo;
	struct lcd_mapping_metadata *meta;
	struct cnode *cnode;
	/*
	 * Look up memory object in caller's cspace
	 */
	ret = __lcd_get_memory_object(caller->cspace, mo_cptr, &cnode, &mo);
	if (ret)
		goto fail1;
	meta = cap_cnode_metadata(cnode);
	/*
	 * If memory object is not mapped, fail
	 */
	if (!meta) {
		LCD_ERR("lookup before meta set?");
		goto fail2;
	}
	if (!meta->is_mapped) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"memory object not mapped");
		goto fail2;
	}
	/*
	 * Unmap memory object
	 */
	__lcd_unmap_pages(caller, meta->where_mapped, 1 << mo->order);
	/*
	 * Mark memory object as not mapped
	 */
	meta->is_mapped = 0;
	/*
	 * Release cnode, etc.
	 */
	__lcd_put_memory_object(caller, cnode, mo);

	return;

fail2:
	__lcd_put_memory_object(caller, cnode, mo);
fail1:
	return;
}
