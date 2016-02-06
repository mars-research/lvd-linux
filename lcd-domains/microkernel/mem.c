/*
 * mem.c -- Architecture-independent memory related stuff - page allocation,
 *          mapping, etc.
 *
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include "internal.h"

/* MEMORY OBJECT HELPERS -------------------------------------------------- */

unsigned long __lcd_memory_object_start(struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		return va2hpa(page_address(mo->object));
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return (unsigned long)mo->object;
	default:
		LCD_ERR("unexpected memory object type %d",
			mo->sub_type);
		/* Return an invalid physical address */
		return ~(0UL);
	}
}

unsigned long __lcd_memory_object_size(struct lcd_memory_object *mo)
{
	return (1UL << mo->order) << PAGE_SHIFT;
}

unsigned long __lcd_memory_object_last(struct lcd_memory_object *mo)
{
	return __lcd_memory_object_start(mo) + 
		__lcd_memory_object_size(mo) - 1;
}

int __lcd_insert_memory_object(struct lcd *caller, void *mem_obj,
			unsigned int order,
			enum lcd_microkernel_type_id sub_type,
			struct lcd_memory_object **mo_out)
{
	struct lcd_memory_object *mo;
	struct lcd_mapping_metadata *meta;
	struct cnode *cnode;
	/*
	 * Set up metadata
	 */
	mo = kmalloc(sizeof(*mo), GFP_KERNEL);
	if (!mo) {
		LCD_ERR("kmalloc error");
		ret = -ENOMEM;
		goto fail1;
	}
	meta = kzalloc(sizeof(*meta), GFP_KERNEL); /* set is_mapped = 0 */
	if (!meta) {
		LCD_ERR("kmalloc error");
		ret = -ENOMEM;
		goto fail2;
	}
	mo->sub_type = sub_type;
	mo->object = mem_obj;
	mo->order = order;
	/*
	 * Insert memory object into global interval tree
	 */
	ret = __lcd_mem_itree_insert(mo, 0);
	if (ret) {
		LCD_ERR("memory itree insert failed");
		goto fail3;
	}
	/*
	 * Insert into caller's cspace
	 */
	ret = cap_insert(caller->cspace, slot, 
			mo, 
			__lcd_get_libcap_type(sub_type));
	if (ret) {
		LCD_ERR("insert");
		goto fail4;
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

	*mo_out = mo;

	return 0;

fail4:
	__lcd_mem_itree_delete(mo);
fail3:
	kfree(meta);
fail2:
	kfree(mo);
fail1:
	return ret;
}

/* ALLOC -------------------------------------------------- */

int __lcd_alloc_pages_exact_node(struct lcd *caller, cptr_t slot, int nid,
				unsigned int flags, unsigned int order)
{
	int ret;
	struct page *p;
	struct lcd_memory_object *unused;
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
	ret = __lcd_insert_memory_object(caller, p, order,
					LCD_MICROKERNEL_TYPE_ID_PAGE,
					&unused);
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
	struct lcd_memory_object *unused;
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
	ret = __lcd_insert_memory_object(caller, p, order,
					LCD_MICROKERNEL_TYPE_ID_PAGE,
					&unused);
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
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE) &&
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM) &&
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM)) {
		LCD_ERR("not a memory object");
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

static int contiguous_memory_object_hpa(struct lcd_memory_object *mo,
					hpa_t *hpa_base)
{
	hpa_t hpa_base;

	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
		*hpa_base = va2hpa(page_address(mo->object));
		break;
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		*hpa_base = hpa_val((unsigned long)mo->object);
		break;
	default:
		LCD_ERR("unexpected memory object type: %d\n",
			mo->sub_type);
		return -EINVAL;
	}

	return 0;
}

static int isolated_map_vmalloc_mem(struct lcd *lcd, 
				struct lcd_memory_object *vmalloc_mo,
				gpa_t base)
{
	int ret, err_ret;
	hva_t vmalloc_base, hva;
	gpa_t gpa;
	unsigned long i, j;
	struct page *p;
	/*
	 * The base host virtual address is stored in the object
	 */
	vmalloc_base = __hva((unsigned long)vmalloc_mo->object);
	/*
	 * Map each page, one at a time
	 */
	for (i = 0; i < (1UL << mo->order); i++) {
		hva = hva_add(vmalloc_base, i * PAGE_SIZE);
		gpa = gpa_add(base, i * PAGE_SIZE);
		/*
		 * Get the corresponding host page 
		 */
		p = vmalloc_to_page(hva2va(hva));
		/*
		 * Map it in the LCD's guest physical
		 */
		ret = lcd_arch_ept_map(lcd->lcd_arch, gpa,
				va2hpa(page_address(p)),
				1, 0);
		if (ret) {
			LCD_ERR("error mapping vmalloc page in LCD");
			goto fail1;
		}
	}
	
	return 0;

fail1:
	/*
	 * Unmap pages that were mapped
	 */
	for (j = 0; j < i; j++) {
		hva = hva_add(vmalloc_base, j * PAGE_SIZE);
		gpa = gpa_add(base, j * PAGE_SIZE);
		/*
		 * Get the corresponding host page 
		 */
		p = vmalloc_to_page(hva2va(hva));
		/*
		 * Unmap it from the LCD's guest physical
		 */
		err_ret = lcd_arch_ept_unmap(lcd->lcd_arch, gpa);
		if (err_ret)
			LCD_ERR("double fault: error unmapping vmalloc page in LCD");
	}

	return ret;
}

static int isolated_map_contiguous_mem(struct lcd *lcd, 
				struct lcd_memory_object *mo,
				gpa_t base)
{
	int ret;
	hpa_t hpa_base;
	/*
	 * Get host physical address of start of memory object
	 */
	ret = contiguous_memory_object_hpa(mo, &hpa_base);
	if (ret) {
		LCD_ERR("invalid memory object");
		goto fail1;
	}
	/*
	 * Map memory object.
	 *
	 * XXX: No matter what the memory object is, we map it
	 * the same way. This is kind of subtle. We can get away
	 * with it because we allow the LCD to use the PAT to control
	 * caching, and we always map memory as WB in guest physical.
	 */
	ret = lcd_arch_ept_map_range(lcd->lcd_arch, base, hpa_base,
				1 << mo->order);
	if (ret) {
		LCD_ERR("map");
		goto fail2;
	}
	
fail2:
fail1:
	return ret;
}

static int isolated_map_memory_object(struct lcd *lcd, 
				struct lcd_memory_object *mo,
				struct lcd_mapping_metadata *meta,
				gpa_t base)
{
	/*
	 * The mapping process depends on the memory object type
	 * (physical memory, vmalloc memory, etc.)
	 */
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return isolated_map_contiguous_mem(lcd, mo, meta, base);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return isolated_map_vmalloc_mem(lcd, mo, meta, base);
	default:
		LCD_ERR("unexpected memory object type %d", mo->sub_type);
		return -EINVAL;
	}
}

int __lcd_do_map_memory_object(struct lcd *lcd, 
			struct lcd_memory_object *mo,
			struct lcd_mapping_metadata *meta,
			gpa_t base)
{
	int ret;
	/*
	 * If memory object is already mapped, fail
	 */
	if (!meta) {
		LCD_ERR("lookup before meta set?");
		ret = -EIO;
		goto out;
	}
	if (meta->is_mapped) {
		LCD_ERR("memory object already mapped");
		ret = -EINVAL;
		goto out;
	}
	/*
	 * We need to handle mapping differently depending on
	 * the LCD type (isolated vs non-isolated)
	 */
	switch (lcd->type) {
	case LCD_TYPE_ISOLATED:
		ret = isolated_map_memory_object(lcd, mo, meta, base);
		break;
	case LCD_TYPE_NONISOLATED:
	case LCD_TYPE_TOP:
		/*
		 * For now, map is a no-op for non-isolated code. (All host 
		 * physical is available to non-isolated code. Recall that
		 * this function is about mapping in physical, not virtual.)
		 */
		ret = 0;
		goto out;
	default:
		LCD_ERR("unrecognized lcd type %d", lcd->type);
		ret = -EINVAL;
		break;
	}
	if (ret)
		goto out;
	/*
	 * Mark page as mapped, and where it's mapped
	 */
	meta->is_mapped = 1;
	meta->where_mapped = base;

	ret = 0;
	goto out;

out:
	return ret;
}

int __lcd_map_memory_object(struct lcd *caller, cptr_t mo_cptr, gpa_t base)
{
	int ret;
	struct lcd_mapping_metadata *meta;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	/*
	 * Look up memory object and metadata in caller's cspace
	 */
	ret = __lcd_get_memory_object(caller->cspace, mo_cptr, &cnode, &mo);
	if (ret)
		goto fail1;
	meta = cap_cnode_metadata(cnode);
	/*
	 * Do the map
	 */
	ret = __lcd_do_map_memory_object(caller, mo, meta, base);
	if (ret)
		goto fail3;
	/*
	 * Release cnode, etc.
	 */
	__lcd_put_memory_object(caller, cnode, mo);

	return 0;

fail3:
fail2:
	__lcd_put_memory_object(caller, cnode, mo);
fail1:
	return ret;
}	

/* UNMAPPING -------------------------------------------------- */

static void isolated_unmap_vmalloc_mem(struct lcd *lcd, 
				struct lcd_memory_object *vmalloc_mo,
				struct lcd_mapping_metadata *meta)
{
	int ret;
	/*
	 * Unmapping is easier, because we mapped the vmalloc memory
	 * in one contiguous chunk.
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch,
				meta->where_mapped,
				vmalloc_mo->order);
	if (ret)
		LCD_DEBUG(LCD_DEBUG_ERR, 
			"some error unmapping");
}

static void isolated_unmap_contiguous_mem(struct lcd *lcd, 
					struct lcd_memory_object *mo,
					struct lcd_mapping_metadata *meta)
{
	/*
	 * Unmap memory object
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch, meta->where_mapped, 
				1UL << mo->order);
	if (ret)
		LCD_DEBUG(LCD_DEBUG_ERR, 
			"some error unmapping");
}

static void isolated_unmap_memory_object(struct lcd *lcd,
					struct lcd_memory_object *mo,
					struct lcd_mapping_metadata *meta)
{
	/*
	 * The unmapping process depends on the memory object type
	 * (physical memory, vmalloc memory, etc.)
	 */
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return isolated_unmap_contiguous_mem(lcd, mo, meta);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return isolated_unmap_vmalloc_mem(lcd, mo, meta);
	default:
		LCD_ERR("unexpected memory object type %d", mo->sub_type);
		return -EINVAL;
	}
}

void __lcd_do_unmap_memory_object(struct lcd *caller, 
				struct lcd_memory_object *mo,
				struct lcd_mapping_metadata *meta)
{
	/*
	 * If memory object is not mapped, silently do nothing
	 */
	if (!meta) {
		LCD_DEBUG(LCD_DEBUG_MSG,
			"lookup before meta set?");
		goto out;
	}
	if (!meta->is_mapped) {
		LCD_DEBUG(LCD_DEBUG_MSG,
			"memory object not mapped");
		goto out;
	}
	/*
	 * We need to handle unmapping differently depending on
	 * the LCD type (isolated vs non-isolated)
	 */
	switch (lcd->type) {
	case LCD_TYPE_ISOLATED:
		ret = isolated_unmap_memory_object(lcd, mo, meta);
		break;
	case LCD_TYPE_NONISOLATED:
	case LCD_TYPE_TOP:
		/*
		 * For now, unmap is a no-op for non-isolated code. (All host 
		 * physical is available to non-isolated code. Recall that
		 * this function is about unmapping in physical, not virtual.)
		 */
		ret = 0;
		goto out;
	default:
		LCD_ERR("unrecognized lcd type %d", lcd->type);
		ret = -EINVAL;
		break;
	}
	if (ret)
		goto out;
	/*
	 * Mark memory object as not mapped
	 */
	meta->is_mapped = 0;

	ret = 0;
	goto out;

out:
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
	 * Do the unmap
	 */
	ret = __lcd_do_unmap_memory_object(caller, mo, meta);
	if (ret)
		goto fail2;
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

/* GRANT -------------------------------------------------- */

int __lcd_grant_memory_object(struct lcd *src, struct lcd *dest,
			cptr_t c_src, cptr_t c_dest)
{
	int ret;
	struct lcd_mapping_metadata *meta;
	struct cnode *cnode;
	/*
	 * Do grant
	 */
	ret = cap_grant(src->cspace, c_src,
			dest->cspace, c_dest);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"grant failed");
		goto fail1;
	}
	/*
	 * Init metadata
	 */
	meta = kzalloc(sizeof(*meta), GFP_KERNEL); /* sets is_mapped = 0 */
	if (!meta) {
		LCD_ERR("malloc failed");
		goto fail2;
	}
	/*
	 * Look up cnode in dest so we can store metadata
	 */
	ret = cap_cnode_get(dest->cspace, c_dest, &cnode);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR, "cnode get failed");
		goto fail3;
	}
	cap_cnode_set_metadata(cnode, meta);
	cap_cnode_put(cnode);
	/*
	 * ==================================================
	 * After this point, meta is owned by libcap and will be
	 * freed when the capability is revoked (via the revoke callback).
	 */

	return 0;

fail3:
	kfree(meta);
fail2:
	cap_delete(dest->cspace, c_dest);
fail1:
	return ret;
}

/* INIT/EXIT -------------------------------------------------- */

int __lcd_mem_init(void)
{
	/*
	 * Nothing for now
	 */
	return 0;
}

void __lcd_mem_exit(void)
{
	/*
	 * Nothing for now
	 */
	return;
}
