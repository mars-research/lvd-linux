/**
 * create.c - Code for creating an LCD.
 *
 * This file is kind of giant because of all the code for setting up
 * the LCD's address space (page tables). The address space set up
 * code is logically part of kliblcd instead of the microkernel so that
 * the microkernel remains simple. (The microkernel shouldn't need to
 * know how to set up the LCD's address space. That's up to the creating
 * LCD/KLCD.)
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include <linux/mutex.h>
#include "../microkernel/internal.h"

int klcd_create(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	ret = __lcd_create(current->lcd, *slot_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int klcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root, 
		gpa_t utcb_page)
{
	return __lcd_config(current->lcd, lcd, pc, sp, gva_root, utcb_page);
}

int klcd_run(cptr_t lcd)
{
	return __lcd_run(current->lcd, lcd);
}

/* MODULE LOADING -------------------------------------------------- */

static inline size_t mpath_size(char *mdir, char *module_name)
{
	size_t psize = 0;
	/* mdir/ */
	psize += strlen(mdir) + 1;
	/* module_name.ko */
	psize += strlen(module_name) + 3;
        /* Allow for trailing \0 */
	return psize + 1;
}

/**
 * Loads module into host address space, and stores pointer to
 * struct module in lcd.
 */
int __klcd_get_module(char *mdir, char *module_name, struct module **m)
{
	int ret;
	struct module *m1;
	char *mpath;
	size_t psize;
	/*
	 * Form module path
	 */
	psize = mpath_size(mdir, module_name);
	if (psize >= LCD_MPATH_SIZE) { 
		LCD_ERR("path to lcd's module .ko is too big");
		LCD_ERR("   mdir = %s, name = %s, size = %llu",
			mdir, module_name, psize);
		ret = -EINVAL;
		goto fail;
	}
	mpath = kmalloc(psize, GFP_KERNEL);
	if (!mpath) {
		LCD_ERR("malloc module path");
		ret = -ENOMEM;
		goto fail0;
	}
	snprintf(mpath, psize, "%s/%s.ko", mdir, module_name);
	/*
	 * Load the requested module
	 */
	ret = request_lcd_module(mpath);
	if (ret < 0) {
		LCD_ERR("load module failed for %s (ret = %d)", mpath, ret);
		goto fail1;
	}
	/*
	 * Find loaded module, and inc its ref counter; must hold module mutex
	 * while finding module.
	 */
	mutex_lock(&module_mutex);
	m1 = find_module(module_name);
	mutex_unlock(&module_mutex);	
	if (!m1) {
		LCD_ERR("couldn't find module");
		ret = -EIO;
		goto fail2;
	}
	if(!try_module_get(m1)) {
		LCD_ERR("incrementing module ref count");
		ret = -EIO;
		goto fail3;
	}

	*m = m1;

	kfree(mpath);

	return ret;

fail3:
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
fail2:
fail1:
	*m = NULL;
	kfree(mpath);
fail0:
fail:
	return ret;
}

void __klcd_put_module(char *module_name)
{
	int ret;
	struct module *m;
	/*
	 * Delete module
	 *
	 * We need to look it up so we can do a put
	 */
 	mutex_lock(&module_mutex);
 	m = find_module(module_name);
 	mutex_unlock(&module_mutex);
	if (!m) {
		LCD_ERR("couldn't find module");
		goto fail;
	}
	module_put(m);
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret) {
		LCD_ERR("deleting module");
		goto fail;
	}

	goto out;
out:
fail:
	return;
}

/* Helper used by get_module_pages. */
static int page_contains(hva_t page_hva, void *protected_ptr)
{
	return (hva_val(page_hva) == 
		(((unsigned long)(protected_ptr)) & PAGE_MASK));
}

/* Helper used by get_module_pages. */
static int dup_page(hva_t hva_src, cptr_t *page_cptr)
{
	gpa_t gpa;
	gva_t gva;
	hva_t hva_dest;
	int ret;
	/*
	 * Allocate a new page
	 */
	ret = lcd_gfp(page_cptr, &gpa, &gva);
	if (ret)
		goto fail1;
	/*
	 * For the host, gva == hva
	 */
	hva_dest = __hva(gva_val(gva));
	/*
	 * Copy over data
	 */
	memcpy(hva2va(hva_dest), hva2va(hva_src), PAGE_SIZE);

	return 0;

fail1:
	return ret;
}

/**
 * Adds the kernel module's pages to the caller's cspace,
 * using klcd_add_page. Creates a list of metadata that
 * represent the pages that make up the module.
 *
 * There is one exception: when protected_data is non-null, we check
 * to see which module page it belongs to. For *this page*,
 * we make a *copy*.
 *
 * Motivation: The struct module for a kernel module is allocated
 * in the module's core. We don't want the host and the LCD
 * to use the same copy. So, we make a copy of the page that
 * contains the struct module. The host and LCD will now use
 * their own copies.
 *
 * Fortunately, the page we alloc to do the copy is under
 * capability access control, and when the caller deletes the
 * last capability, the page will be freed.
 */
static int get_module_pages(hva_t hva, unsigned long size, 
			struct list_head *mpage_list,
			void *protected_data)
{
	int ret;
	unsigned long mapped;
	struct page *p;
	cptr_t pg_cptr;
	struct lcd_module_page *mp;
	int did_dup = 0;

	mapped = 0;
	while (mapped < size) {
		/*
		 * Get module page
		 */
		p = vmalloc_to_page(hva2va(hva));
		/*
		 * Check if need to make a copy of the page
		 */
		if (protected_data && page_contains(hva, protected_data)) {
			/*
			 * Yes: the protected data is in this page.
			 *
			 * Make a copy
			 */
			ret = dup_page(hva, &pg_cptr);
			if (ret)
				goto fail1;
			did_dup = 1;
		} else {
			/*
			 * No: the protected data is not in this page.
			 *
			 * Just add the page to caller's cspace
			 */
			ret = klcd_add_page(p, &pg_cptr);
			if (ret)
				goto fail1;
			did_dup = 0;
		}
		/*
		 * Record in list of pages
		 */
		mp = kmalloc(sizeof(*mp), GFP_KERNEL);
		if (!mp) {
			ret = -ENOMEM;
			LCD_ERR("no mem");
			goto fail2;
		}
		mp->cptr = pg_cptr;
		mp->gva = __gva(hva_val(hva)); /* use same address */
		INIT_LIST_HEAD(&mp->list);
		list_add_tail(&mp->list, mpage_list);
		/*
		 * Increment ...
		 */
		mapped += PAGE_SIZE;
		hva = hva_add(hva, PAGE_SIZE);

		did_dup = 0;
	}
	return 0;

fail2:
	/* Differentiating the two doesn't matter right now, but it
	 * may in the future. (In both cases, we just delete the capability
	 * and free the slot in the cptr cache.) */
	if (did_dup)
		lcd_free_page(pg_cptr);
	else
		klcd_rm_page(pg_cptr);
fail1:
	return ret; /* caller will free lcd_module_page's, etc. */
}

static void free_module_pages(struct list_head *mpages_list)
{
	struct list_head *cursor, *next;
	struct lcd_module_page *p;

	list_for_each_safe(cursor, next, mpages_list) {
		p = list_entry(cursor, struct lcd_module_page, list);
		/*
		 * Remove from capability system
		 */
		klcd_rm_page(p->cptr);
		/*
		 * Free struct
		 */
		kfree(p);
	}
}

static int init_lcd_info(struct lcd_info **mi)
{
	struct lcd_info *i;
	int ret;
	/*
	 * Alloc 
	 */
	i = kzalloc(sizeof(*i), GFP_KERNEL);
	if (!i) {
		ret = -ENOMEM;
		goto fail0;
	}
	/*
	 * Init list for module pages
	 */
	INIT_LIST_HEAD(&i->mpages_list);
	/*
	 * Initialize cap cache
	 */
	ret = klcd_init_cptr(&i->cache);
	if (ret)
		goto fail1;
	/*
	 * Init list for boot page infos
	 */
	INIT_LIST_HEAD(&i->boot_mem_list);
	INIT_LIST_HEAD(&i->stack_mem_list);
	INIT_LIST_HEAD(&i->paging_mem_list);
	INIT_LIST_HEAD(&i->free_mem_list);

	*mi = i;

	return 0;

fail1:
	kfree(i);
fail0:
	return ret;
}

static void free_lcd_info(struct lcd_info *i)
{
	struct list_head *cursor, *next;
	struct lcd_page_info_list_elem *e;
	/*
	 * Destroy cptr cache
	 */
	klcd_destroy_cptr(i->cache);
	/*
	 * Free page info's
	 */
	list_for_each_safe(cursor, next, &i->boot_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		kfree(e);
	}
	list_for_each_safe(cursor, next, &i->stack_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		kfree(e);
	}
	list_for_each_safe(cursor, next, &i->paging_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		kfree(e);
	}
	list_for_each_safe(cursor, next, &i->free_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		kfree(e);
	}
	/*
	 * Any remaining pages allocated will be freed when the lcd is torn 
	 * down (e.g., boot pages) because it is the sole owner (for example,
	 * when we allocated pages for the stack, we deleted the cap in our
	 * cspace so that the lcd is the sole owner)
	 */

	/*
	 * Free lcd info
	 */
	kfree(i);
}

int klcd_load_module(char *mdir, char *mname, cptr_t mloader_endpoint, 
		struct lcd_info **mi)
{
	int ret;
	/*
	 * Ignore mloader_endpoint - we will use standard module loading
	 * code.
	 */
	struct module *m;

	ret = init_lcd_info(mi);
	if (ret)
		goto fail0;
	/*
	 * Load module in host
	 */
	ret = __klcd_get_module(mdir, mname, &m);
	if (ret)
		goto fail1;
	/*
	 * Get init and core pages
	 */
	ret = get_module_pages(va2hva(m->module_init), 
			m->init_size, &(*mi)->mpages_list,
			NULL);
	if (ret) {
		goto fail2;
	}
	ret = get_module_pages(va2hva(m->module_core), 
			m->core_size, &(*mi)->mpages_list,
			m);
	if (ret) {
		goto fail3;
	}
	LCD_MSG("module %s init @ 0x%lx core @ 0x%lx",
		mname, m->module_init, m->module_core);
	/*
	 * Copy name and module init address
	 */
	(*mi)->init = __gva(hva_val(va2hva(m->module_init)));
	strncpy((*mi)->mname, mname, LCD_MODULE_NAME_MAX);

	return 0;

fail3:
fail2:
	free_module_pages(&(*mi)->mpages_list);
fail1:
	free_lcd_info(*mi);
fail0:
	return ret;
}

void klcd_unload_module(struct lcd_info *mi, cptr_t mloader_endpoint)
{
	/*
	 * module loader endpoint ignored; use standard module loading system
	 *
	 * Remove module pages from capability system
	 */
	free_module_pages(&mi->mpages_list);
	/*
	 * Delete module
	 */
	__klcd_put_module(mi->mname);
	/*
	 * Free lcd module info
	 */
	free_lcd_info(mi);
}

/* GUEST VIRTUAL PAGING SETUP ----------------------------------- */

static inline gpa_t pte_gpa(pte_t *pte)
{
	return __gpa(pte_pfn(*pte) << PAGE_SHIFT);
}
static inline gpa_t pmd_gpa(pmd_t *pmd_entry)
{
	return __gpa(pmd_pfn(*pmd_entry) << PAGE_SHIFT);
}
static inline gpa_t pud_gpa(pud_t *pud_entry)
{
	return __gpa(pud_pfn(*pud_entry) << PAGE_SHIFT);
}
static inline gpa_t pgd_gpa(pgd_t *pgd_entry)
{
	return __gpa(pgd_pfn(*pgd_entry) << PAGE_SHIFT);
}
static inline void set_pte_gpa(pte_t *pte, gpa_t gpa)
{
	set_pte(pte, __pte(gpa_val(gpa) | _KERNPG_TABLE));
}
static inline void set_pmd_gpa(pmd_t *entry, gpa_t gpa)
{
	set_pmd(entry, __pmd(gpa_val(gpa) | _KERNPG_TABLE));
}
static inline void set_pud_gpa(pud_t *entry, gpa_t gpa)
{
	set_pud(entry, __pud(gpa_val(gpa) | _KERNPG_TABLE));
}
static inline void set_pgd_gpa(pgd_t *entry, gpa_t gpa)
{
	set_pgd(entry, __pgd(gpa_val(gpa) | _KERNPG_TABLE));
}

struct hpa_cptr_tuple {
	hpa_t hpa;
	cptr_t cptr;
};

struct create_module_cxt {
	struct hpa_cptr_tuple gpa2hpacptr[LCD_GV_PAGING_MEM_SIZE >> 
					PAGE_SHIFT];
	unsigned int counter;
	pgd_t *root;
	struct cptr_cache *cache;
	struct list_head paging_mem_infos;
};

static int cxt_init(struct create_module_cxt **cxt_out,
		struct lcd_info *mi)
{
	int ret;
	/*
	 * Allocate context
	 *
	 * XXX: This is a big chunk of memory. But it will do for now ...
	 */
	*cxt_out = kzalloc(sizeof(struct create_module_cxt), GFP_KERNEL);
	if (!*cxt_out) {
		LCD_ERR("no mem");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Store ref to cptr cache
	 */
	(*cxt_out)->cache = mi->cache;

	INIT_LIST_HEAD(&(*cxt_out)->paging_mem_infos);

	return 0;

fail1:
	return ret;
}

static void cxt_destroy(struct create_module_cxt *cxt)
{
	int i;
	cptr_t c;
	/*
	 * Delete pages from our cspace
	 */
	for (i = 0; i < (LCD_GV_PAGING_MEM_SIZE >> PAGE_SHIFT); i++) {
		c = cxt->gpa2hpacptr[i].cptr;
		if (!cptr_val(c))
			continue; /* skip over, no page here */
		lcd_cap_delete(c);
	}
	/*
	 * Free cxt
	 */
	kfree(cxt);
}

static int gv_gfp(cptr_t lcd, struct create_module_cxt *cxt, gpa_t *gpa_out)
{
	cptr_t slot;
	int ret;
	gpa_t gpa;
	gva_t gva;
	hpa_t hpa;
	cptr_t dest_slot;
	struct lcd_page_info_list_elem *e;
	/*
	 * Ensure we still have room
	 */
	if (cxt->counter >= (LCD_GV_PAGING_MEM_SIZE >> PAGE_SHIFT)) {
		LCD_ERR("exhaused paging memory");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Get free page
	 */
	ret = lcd_gfp(&slot, &gpa, &gva);
	if (ret)
		goto fail2;
	/*
	 * Guest addresses = host for klcd's
	 */
	hpa = __hpa(gpa_val(gpa));
	/*
	 * Alloc a dest slot
	 */
	ret = __lcd_alloc_cptr(cxt->cache, &dest_slot);
	if (ret) {
		LCD_ERR("failed to alloc dest slot");
		goto fail3;
	}
	/*
	 * Grant and map in lcd
	 *
	 * The page will be mapped at the gpa given below (use the counter
	 * as an offset into the chunk of the guest physical address space
	 * reserved for paging memory).
	 */
	*gpa_out = gpa_add(LCD_GV_PAGING_MEM_GPA,
			cxt->counter * PAGE_SIZE);
	ret = lcd_cap_page_grant_map(lcd, slot, dest_slot, *gpa_out);
	if (ret) {
		LCD_ERR("mapping page");
		goto fail4;
	}
	/*
	 * Store correspondence from lcd gpa to the caller's address space
	 */
	cxt->gpa2hpacptr[cxt->counter].hpa = hpa;
	cxt->gpa2hpacptr[cxt->counter].cptr = slot;
	/*
	 * Bump page counter
	 */
	cxt->counter++;
	/*
	 * Store in list of page info's
	 */
	e = kmalloc(sizeof(*e), GFP_KERNEL);
	if (!e) {
		LCD_ERR("malloc page info");
		goto fail5;
	}
	INIT_LIST_HEAD(&e->list);
	e->my_cptr = dest_slot;
	e->page_gpa = *gpa_out;
	list_add(&e->list, &cxt->paging_mem_infos);

	return 0;

fail5: /* lcd should be torn down, everything will be freed then */
fail4:
	__lcd_free_cptr(cxt->cache, dest_slot);
fail3:
	lcd_cap_delete(slot); /* will free page */
fail2:
fail1:
	return ret;	
}

static int gv_gpa2hpa(struct create_module_cxt *cxt, gpa_t gpa, hpa_t *hpa_out)
{
	int ret;
	unsigned long offset;
	unsigned long pfn;
	/*
	 * Error check
	 */
	if (gpa_val(gpa) < gpa_val(LCD_GV_PAGING_MEM_GPA) ||
		gpa_val(gpa) >= gpa_val(LCD_STACK_GPA)) {
		LCD_ERR("trying to convert bad gpa %llu",
			gpa_val(gpa));
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Compute address offset into paging memory
	 */
	offset = gpa_val(gpa) - gpa_val(LCD_GV_PAGING_MEM_GPA);
	if (offset & ~PAGE_MASK) {
		LCD_ERR("offset 0x%llx not page aligned", offset);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Determine guest physical page frame
	 */
	pfn = offset >> PAGE_SHIFT;
	/*
	 * Do look up
	 */
	*hpa_out = cxt->gpa2hpacptr[pfn].hpa;

	return 0;

fail1:
	return ret;
}

/**
 * Initializes root page directory for guest virtual paging in lcd.
 *
 * Must be called before mapping any gva's, or else you'll get a kernel
 * oops on the NULL %cr3 when we try to do a page walk.
 */
static int gv_setup_pgd(cptr_t lcd, struct create_module_cxt *cxt)
{
	gpa_t gpa;
	hpa_t hpa;
	int ret;
	ret = gv_gfp(lcd, cxt, &gpa);
	if (ret)
		return ret;
	ret = gv_gpa2hpa(cxt, gpa, &hpa);
	if (ret)
		return ret;
	cxt->root = hva2va(hpa2hva(hpa));
	return 0;
}

/**
 * Get host virtual address of pte for gva and pmd_entry.
 */
static int gv_lookup_pte(struct create_module_cxt *cxt, gva_t gva, 
			pmd_t *pmd_entry, pte_t **pte_out)
{
	int ret;
	gpa_t gpa;
	hpa_t hpa;
	pte_t *entry;

	/*
	 * Get hpa of page table, using gpa stored in pmd_entry.
	 */
	gpa = pmd_gpa(pmd_entry);
	ret = gv_gpa2hpa(cxt, gpa, &hpa);
	if (ret)
		return ret;
	/*
	 * Look up entry in page table
	 */
	entry = ((pte_t *)hpa2va(hpa)) + pte_index(gva_val(gva));
	
	*pte_out = entry;
	return 0;
}

/**
 * Look up pte for the page frame containing gva,
 * using the page table referenced by pmd_entry.
 */
static int gv_walk_pt(struct create_module_cxt *cxt, gva_t gva, 
		pmd_t *pmd_entry, pte_t **pte_out)
{
	int ret;
	pte_t *entry;

	ret = gv_lookup_pte(cxt, gva, pmd_entry, &entry);
	if (ret) {
		LCD_ERR("looking up pte for gva %lx", gva_val(gva));
		return ret;
	}

	*pte_out = entry;

	return 0;
}

/**
 * Get host virtual address of pmd entry for gva and pud_entry.
 */
static int gv_lookup_pmd(struct create_module_cxt *cxt, gva_t gva, 
			pud_t *pud_entry, pmd_t **pmd_out)
{
	int ret;
	gpa_t gpa;
	hpa_t hpa;
	pmd_t *entry;

	/*
	 * Get hpa of pmd, using gpa stored in pud_entry.
	 */
	gpa = pud_gpa(pud_entry);
	ret = gv_gpa2hpa(cxt, gpa, &hpa);
	if (ret)
		return ret;
	/*
	 * Look up entry in pmd
	 */
	entry = ((pmd_t *)hpa2va(hpa)) + pmd_index(gva_val(gva));
	
	*pmd_out = entry;
	return 0;
}

/**
 * Look up pmd entry for the page table for gva,
 * using the pmd referenced by pud_entry.
 */
static int gv_walk_pmd(cptr_t lcd, struct create_module_cxt *cxt, 
		gva_t gva, pud_t *pud_entry, pmd_t **pmd_out)
{
	int ret;
	pmd_t *entry;
	gpa_t gpa;

	ret = gv_lookup_pmd(cxt, gva, pud_entry, &entry);
	if (ret) {
		LCD_ERR("looking up pmd for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pmd_present(*entry)) {
		/*
		 * Alloc and map a page table
		 */
		ret = gv_gfp(lcd, cxt, &gpa);
		if (ret) {
			LCD_ERR("alloc page table");
			return ret;
		}

		/*
		 * Map *guest physical* address into pud entry
		 */
		set_pmd_gpa(entry, gpa);
	}

	*pmd_out = entry;

	return 0;
}

/**
 * Get host virtual address of pud entry for gva and pgd_entry.
 */
static int gv_lookup_pud(struct create_module_cxt *cxt, gva_t gva, 
			pgd_t *pgd_entry, pud_t **pud_out)
{
	int ret;
	gpa_t gpa;
	hpa_t hpa;
	pud_t *entry;

	/*
	 * Get hpa of pud, using gpa stored in pgd_entry.
	 */
	gpa = pgd_gpa(pgd_entry);
	ret = gv_gpa2hpa(cxt, gpa, &hpa);
	if (ret) 
		return ret;
	/*
	 * Look up entry in pud
	 */
	entry = ((pud_t *)hpa2va(hpa)) + pud_index(gva_val(gva));
	
	*pud_out = entry;
	return 0;
}

/**
 * Look up pud entry for the pmd for gva, using
 * the pud referenced by pgd_entry.
 */
static int gv_walk_pud(cptr_t lcd, struct create_module_cxt *cxt, 
		gva_t gva, pgd_t *pgd_entry, pud_t **pud_out)
{
	int ret;
	pud_t *entry;
	gpa_t gpa;

	ret = gv_lookup_pud(cxt, gva, pgd_entry, &entry);
	if (ret) {
		LCD_ERR("looking up pud for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pud_present(*entry)) {
		/*
		 * Alloc and map a pmd
		 */
		ret = gv_gfp(lcd, cxt, &gpa);
		if (ret) {
			LCD_ERR("alloc pmd");
			return ret;
		}

		/*
		 * Map *guest physical* address into pud entry
		 */
		set_pud_gpa(entry, gpa);
	}

	*pud_out = entry;

	return 0;
}

/**
 * Look up pgd entry for the pud for gva.
 */
static int gv_walk_pgd(cptr_t lcd, struct create_module_cxt *cxt, gva_t gva, 
			pgd_t **pgd_out)
{
	int ret;
	pgd_t *entry;
	gpa_t gpa;

	entry = cxt->root + pgd_index(gva_val(gva));
	if (!pgd_present(*entry)) {
		/*
		 * Alloc and map a pud
		 */
		ret = gv_gfp(lcd, cxt, &gpa);
		if (ret) {
			LCD_ERR("alloc pud");
			return ret;
		}

		/*
		 * Map *guest physical* address into pgd entry
		 */
		set_pgd_gpa(entry, gpa);
	}

	*pgd_out = entry;

	return 0;
}

/**
 * You must initialize a gv_cxt before calling this. Use
 * gv_init.
 * 
 * Look up the page table entry for guest virtual
 * address gva, using the pgd pointed to by root_hva.
 *
 * Paging data structures are allocated along the
 * way (since this is only used when setting up the boot
 * guest virtual address space).
 *
 * Hierarchy: pgd -> pud -> pmd -> page table -> page frame
 *
 * For concreteness, on Intel 64-bit, IA-32e paging
 * is used, and
 *
 *    pgd = pml4
 *    pud = pdpt
 *    pmd = page directory
 *
 * with the `standard' 512 entries per paging structure.
 *
 * Since guest physical addresses (rather than 
 * host physical addresses) are stored in the paging
 * structures, we can't use some of the most benefical
 * macros that allow for pud- and pmd-folding
 * (e.g., pud_offset). C'est la vie ... We could define
 * some macros that do the same thing, later ...
 *
 * Punchline: Arch must have 4 paging levels.
 */
static int gv_walk(cptr_t lcd, struct create_module_cxt *cxt,
		gva_t gva, pte_t **pte_out)
{
	int ret;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	/*
	 * Get pgd entry for pud
	 */
	ret = gv_walk_pgd(lcd, cxt, gva, &pgd);
	if (ret) {
		LCD_ERR("walking pgd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pud entry for pmd
	 */
	ret = gv_walk_pud(lcd, cxt, gva, pgd, &pud);
	if (ret) {
		LCD_ERR("walking pud for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pmd entry for page table
	 */
	ret = gv_walk_pmd(lcd, cxt, gva, pud, &pmd);
	if (ret) {
		LCD_ERR("walking pmd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Finally, get page table entry
	 */
	return gv_walk_pt(cxt, gva, pmd, pte_out);
}

static void gv_set(pte_t *pte, gpa_t gpa)
{
	set_pte_gpa(pte, gpa);
}

static gpa_t gv_get(pte_t *pte)
{
	return pte_gpa(pte);
}

/**
 * Simple routine combining walk and set. Never
 * overwrites.
 */
static int gv_map(cptr_t lcd, struct create_module_cxt *cxt,
		gva_t gva, gpa_t gpa)
{
	int ret;
	pte_t *pte;

	ret = gv_walk(lcd, cxt, gva, &pte);
	if (ret) {
		LCD_ERR("getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	if (pte_present(*pte)) {
		LCD_ERR("remap gva %lx to gpa %lx (was %lx)\n",
			gva_val(gva), gpa_val(gpa),
			gpa_val(gv_get(pte)));
		return -EINVAL;
	}

	gv_set(pte, gpa);

	return 0;
}

/**
 * Maps 
 *
 *    gva_start --> gva_start + npages * PAGE_SIZE
 *
 * to
 *
 *    gpa_start --> gpa_start + npages * PAGE_SIZE
 *
 * in lcd's guest virtual paging tables.
 *
 * Note! Call lcd_mm_gva_init before mapping any gva's.
 */
static int gv_map_range(cptr_t lcd, struct create_module_cxt *cxt, 
			gva_t gva_start, gpa_t gpa_start, unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (gv_map(lcd, cxt,
				/* gva */
				gva_add(gva_start, off),
				/* gpa */
				gpa_add(gpa_start, off))) {
			LCD_ERR("mapping gva %lx to gpa %lx\n",
				gva_val(gva_add(gva_start,off)),
				gpa_val(gpa_add(gpa_start,off)));
			return -EIO;
		}
	}

	return 0;
}

/* DEBUG -------------------------------------------------- */

static void gv_debug_pt(struct create_module_cxt *cxt,
			pmd_t *pmd_entry, unsigned my_idx)
{
	unsigned idx;
	int ret;
	pte_t *entry;
	gpa_t pt_gpa;
	hpa_t pt_hpa;

	printk(KERN_ERR "\n\npt %03u entries:\n------------------\n\n",
		my_idx);	

	pt_gpa = pmd_gpa(pmd_entry);
	ret = gv_gpa2hpa(cxt, pt_gpa, &pt_hpa);
	if (ret) 
		printk(KERN_ERR "bad pt gpa address 0x%lx from pmd entry 0x%lx\n", gpa_val(pt_gpa), pmd_val(*pmd_entry));

	for (idx = 0; idx < 512; idx++) {
		entry = ((pte_t *)hpa2va(pt_hpa)) + idx;
		printk(KERN_ERR "%03u %lx\n", idx, pte_val(*entry));
	}
}

static void gv_debug_pmd(struct create_module_cxt *cxt,
			pud_t *pud_entry, unsigned my_idx)
{
	unsigned idx;
	pmd_t *entry;
	gpa_t pmd_gpa;
	hpa_t pmd_hpa;
	int ret;

	printk(KERN_ERR "\n\npmd %03u entries:\n------------------\n\n",
		my_idx);	

	pmd_gpa = pud_gpa(pud_entry);
	ret = gv_gpa2hpa(cxt, pmd_gpa, &pmd_hpa);
	if (ret) 
		printk(KERN_ERR "bad pmd gpa address 0x%lx from pud entry 0x%lx\n", gpa_val(pmd_gpa), pud_val(*pud_entry));

	for (idx = 0; idx < 512; idx++) {
		entry = ((pmd_t *)hpa2va(pmd_hpa)) + idx;
		printk(KERN_ERR "%03u %lx\n", idx, pmd_val(*entry));
	}

	for (idx = 0; idx < 512; idx++) {
		entry = ((pmd_t *)hpa2va(pmd_hpa)) + idx;
		if (pmd_present(*entry)) {
			gv_debug_pt(cxt, entry, idx);
		}
	}
}

static void gv_debug_pud(struct create_module_cxt *cxt,
			pgd_t *pgd_entry, unsigned my_idx)
{
	unsigned idx;
	pud_t *entry;
	gpa_t pud_gpa;
	hpa_t pud_hpa;
	int ret;

	printk(KERN_ERR "\n\npud %03u entries:\n------------------\n\n",
		my_idx);	

	pud_gpa = pgd_gpa(pgd_entry);
	ret = gv_gpa2hpa(cxt, pud_gpa, &pud_hpa);
	if (ret) 
		printk(KERN_ERR "bad pud gpa address 0x%lx from pgd entry 0x%lx\n", gpa_val(pud_gpa), pgd_val(*pgd_entry));

	for (idx = 0; idx < 512; idx++) {
		entry = ((pud_t *)hpa2va(pud_hpa)) + idx;
		printk(KERN_ERR "%03u %lx\n", idx, pud_val(*entry));
	}

	for (idx = 0; idx < 512; idx++) {
		entry = ((pud_t *)hpa2va(pud_hpa)) + idx;
		if (pud_present(*entry)) {
			gv_debug_pmd(cxt, entry, idx);
		}
	}
}

static void gv_debug_pgd(struct create_module_cxt *cxt)
{
	unsigned idx;
	pgd_t *entry;

	printk(KERN_ERR "pgd entries:\n------------------\n\n");

	for (idx = 0; idx < 512; idx++) {
		entry = cxt->root + idx;
		printk(KERN_ERR "%03u %lx\n", idx, pgd_val(*entry));
	}

	for (idx = 0; idx < 512; idx++) {
		entry = cxt->root + idx;
		if (pgd_present(*entry)) {
			gv_debug_pud(cxt, entry, idx);
		}
	}
}

void gv_debug(struct create_module_cxt *cxt)
{
	/*
	 * Walk pgd
	 */
	printk(KERN_ERR "dumping gv tables\n------------------------\n\n");
	gv_debug_pgd(cxt);
}

/* -------------------------------------------------- */

static int map_module(cptr_t lcd, struct create_module_cxt *cxt,
		struct lcd_info *mi)
{
	struct list_head *cursor;
	struct lcd_module_page *mp;
	unsigned long offset;
	gpa_t gpa;
	int ret = 0;
	cptr_t dest_slot;
	struct lcd_page_info_list_elem *e;

	offset = 0;

	/*
	 * Map each module page in the lcd's guest physical and virtual
	 *
	 * If we fail part way through, it's ok: When the lcd is destroyed,
	 * and the caps to the pages deleted, the pages will be unmapped / 
	 * freed.
	 */
	
	list_for_each(cursor, &mi->mpages_list) {

		mp = list_entry(cursor, struct lcd_module_page, list);

		gpa = gpa_add(LCD_MODULE_GPA, offset);
		/*
		 * Alloc slot in dest
		 */
		ret = __lcd_alloc_cptr(cxt->cache, &dest_slot);
		if (ret) {
			LCD_ERR("alloc failed");
			goto fail1;
		}
		/*
		 * Grant and map in lcd's guest physical
		 */
		ret = lcd_cap_page_grant_map(lcd, mp->cptr,
					dest_slot,
					gpa);
		if (ret) {
			LCD_ERR("couldn't map module page in lcd's gp");
			goto fail2;
		}
		/*
		 * Map in lcd's guest virtual
		 */
		ret = gv_map(lcd, cxt, mp->gva, gpa);
		if (ret) {
			LCD_ERR("couldn't map in lcd's gv");
			goto fail3;
		}
		/*
		 * Set up page info - this is going in free mem area (at least
		 * in the guest physical free mem area)
		 */
		e = kmalloc(sizeof(*e), GFP_KERNEL);
		if (!e) {
			LCD_ERR("alloc page info");
			goto fail4;
		}
		INIT_LIST_HEAD(&e->list);
		e->my_cptr = dest_slot;
		e->page_gpa = gpa;
		list_add(&e->list, &mi->free_mem_list);
		/*
		 * Bump offset
		 */
		offset += PAGE_SIZE;
	}

fail4:
fail3:
fail2:
fail1:
	return ret;	/* we failed; pages will be unmapped when lcd is
			 * destroyed, page infos, etc. freed when lcd_info
			 * destroyed */
}

static int map_gv_memory_and_stack(cptr_t lcd, struct create_module_cxt *cxt)
{
	int ret;
	gpa_t utcb_top;
	/*
	 * Map paging mem, boot mem, and utcb first
	 */
	utcb_top = gpa_add(LCD_UTCB_GPA, LCD_UTCB_SIZE);
	ret = gv_map_range(lcd, cxt,
			LCD_GV_PAGING_MEM_GVA,
			LCD_GV_PAGING_MEM_GPA,
			(gpa_val(utcb_top) - 
				gpa_val(LCD_GV_PAGING_MEM_GPA)) >> PAGE_SHIFT);
	if (ret) {
		LCD_ERR("map 1");
		goto fail1;
	}
	/*
	 * Map stack (skip over guard page)
	 */
	ret = gv_map_range(lcd, cxt,
			LCD_STACK_GVA,
			LCD_STACK_GPA,
			(LCD_STACK_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LCD_ERR("map 2");
		goto fail2;
	}

	return 0;

fail2:
fail1:
	return ret; /* everything freed when LCD is destroyed */
}

static int do_boot_pages(cptr_t lcd, struct lcd_info *mi,
			struct create_module_cxt *cxt)
{
	int i;
	int ret;
	hpa_t hp_base_out;
	hva_t hv_base_out;
	struct lcd_page_info_list_elem *e;
	unsigned long offset;
	gpa_t gpa;
	cptr_t dest_slot;
	cptr_t boot_pages_cptrs[1 << LCD_BOOT_PAGES_ORDER];
	/*
	 * Allocate the boot pages on the host
	 */
	ret = klcd_pages_alloc(boot_pages_cptrs, &hp_base_out, &hv_base_out,
			LCD_BOOT_PAGES_ORDER);
	if (ret) {
		LCD_ERR("pages alloc");
		goto out1;
	}
	mi->boot_page_base = hva2va(hv_base_out);
	/*
	 * Grant and map them in the lcd
	 */	
	offset = 0;
	for (i = 0; i < (1 << LCD_BOOT_PAGES_ORDER); i++) {
		gpa = gpa_add(LCD_BOOT_PAGES_GPA, offset);
		/*
		 * Alloc slot in dest
		 */
		ret = __lcd_alloc_cptr(cxt->cache, &dest_slot);
		if (ret) {
			LCD_ERR("alloc failed");
			goto out2;
		}
		/*
		 * Grant and map in lcd's guest physical
		 */
		ret = lcd_cap_page_grant_map(lcd, boot_pages_cptrs[i],
					dest_slot,
					gpa);
		if (ret) {
			LCD_ERR("couldn't map boot page in lcd's gp");
			goto out2;
		}
		/*
		 * Will be mapped in guest virtual along with the stack
		 * and paging mem pages ...
		 */
		/*
		 * Set up page info
		 */
		e = kmalloc(sizeof(*e), GFP_KERNEL);
		if (!e) {
			LCD_ERR("alloc page info");
			goto out2;
		}
		INIT_LIST_HEAD(&e->list);
		e->my_cptr = dest_slot;
		e->page_gpa = gpa;
		list_add(&e->list, &mi->boot_mem_list);
		/*
		 * Bump offset
		 */
		offset += PAGE_SIZE;
	}

	goto out2;

out2:
	/*
	 * Make sure we delete our capabilities to the alloc'd
	 * boot pages. Those that were granted to the LCD (perhaps
	 * during failure) will be freed when the LCD is torn down. Those
	 * that weren't will be freed right now.
	 */
	for (i = 0; i < (1 << LCD_BOOT_PAGES_ORDER); i++)
		lcd_cap_delete(boot_pages_cptrs[i]);
out1:
	return ret;	/* if we failed, pages will be unmapped when lcd is
			 * destroyed, page infos, cptrs, etc. freed when 
			 * lcd_info destroyed */
}

static int do_stack_pages(cptr_t lcd, struct lcd_info *mi,
			struct create_module_cxt *cxt)
{
	int i;
	int ret;
	hpa_t hp_base_out;
	hva_t hv_base_out;
	struct lcd_page_info_list_elem *e;
	unsigned long offset;
	gpa_t gpa;
	cptr_t dest_slot;
	cptr_t stack_pages_cptrs[1 << LCD_STACK_PAGES_ORDER];
	/*
	 * Allocate the stack pages on the host
	 */
	ret = klcd_pages_alloc(stack_pages_cptrs, &hp_base_out, &hv_base_out,
			LCD_STACK_PAGES_ORDER);
	if (ret) {
		LCD_ERR("pages alloc");
		goto out1;
	}
	/*
	 * Grant and map them in the lcd
	 */	
	offset = 0;
	for (i = 0; i < (1 << LCD_STACK_PAGES_ORDER); i++) {
		gpa = gpa_add(LCD_STACK_GPA, offset);
		/*
		 * Alloc slot in dest
		 */
		ret = __lcd_alloc_cptr(cxt->cache, &dest_slot);
		if (ret) {
			LCD_ERR("alloc failed");
			goto out2;
		}
		/*
		 * Grant and map in lcd's guest physical
		 */
		ret = lcd_cap_page_grant_map(lcd, stack_pages_cptrs[i],
					dest_slot,
					gpa);
		if (ret) {
			LCD_ERR("couldn't map stack page in lcd's gp");
			goto out2;
		}
		/*
		 * Will be mapped in guest virtual along with the boot
		 * and paging mem pages ...
		 */
		/*
		 * Set up page info
		 */
		e = kmalloc(sizeof(*e), GFP_KERNEL);
		if (!e) {
			LCD_ERR("alloc page info");
			goto out2;
		}
		INIT_LIST_HEAD(&e->list);
		e->my_cptr = dest_slot;
		e->page_gpa = gpa;
		list_add(&e->list, &mi->stack_mem_list);
		/*
		 * Bump offset
		 */
		offset += PAGE_SIZE;
	}

	goto out2;

out2:
	/*
	 * Make sure we delete our capabilities to the alloc'd
	 * pages. Those that were granted to the LCD (perhaps
	 * during failure) will be freed when the LCD is torn down. Those
	 * that weren't will be freed right now.
	 */
	for (i = 0; i < (1 << LCD_STACK_PAGES_ORDER); i++)
		lcd_cap_delete(stack_pages_cptrs[i]);
out1:
	return ret;	/* if we failed, pages will be unmapped when lcd is
			 * destroyed, page infos, cptrs, etc. freed when 
			 * lcd_info destroyed */
}

static int setup_addr_space(cptr_t lcd, struct lcd_info *mi)
{
	struct create_module_cxt *cxt;
	int ret;
	/*
	 * Set up guest virtual cxt
	 */
	ret = cxt_init(&cxt, mi);
	if (ret)
		goto fail1;
	/*
	 * Set up root page directory
	 */
	ret = gv_setup_pgd(lcd, cxt);
	if (ret)
		goto fail2;
	/*
	 * Map module
	 */
	ret = map_module(lcd, cxt, mi);
	if (ret) {
		LCD_ERR("adding module to addr space");
		goto fail3;
	}
	/*
	 * Set up boot pages
	 */
	ret = do_boot_pages(lcd, mi, cxt);
	if (ret) {
		LCD_ERR("setting up boot pages");
		goto fail4;
	}
	/*
	 * Set up stack pages
	 */
	ret = do_stack_pages(lcd, mi, cxt);
	if (ret) {
		LCD_ERR("setting up stack pages");
		goto fail5;
	}
	/*
	 * Map guest virtual paging memory, boot pages, utcb, and stack
	 */
	ret = map_gv_memory_and_stack(lcd, cxt);
	if (ret) {
		LCD_ERR("mapping paging mem");
		goto fail6;
	}
	/*
	 * Copy over list of paging mem infos before we kill cxt
	 */
	list_splice(&cxt->paging_mem_infos, &mi->paging_mem_list);
	/*
	 * Remove our references to the guest virtual paging memory, so
	 * the pages will be freed when the lcd is torn down.
	 */
	cxt_destroy(cxt);

	return 0;

fail6:
fail5:
fail4:
fail3:
fail2:
	/* gv_destroy just removes our caps to the gv paging pages; 
	 * boot pages, etc. will be freed when lcd is 
	 * destroyed.
	 *
	 * module pages will be freed when lcd_unload_module is called
	 */
	cxt_destroy(cxt);
fail1:
	return ret;		
}

int klcd_create_module_lcd(cptr_t *slot_out, char *mdir, char *mname, 
			cptr_t mloader_endpoint, struct lcd_info **mi)
			
{
	int ret;
	/*
	 * Create an empty lcd
	 */
	ret = lcd_create(slot_out);
	if (ret) {
		LCD_ERR("lcd create failed");
		goto fail0;
	}
	/*
	 * Load module
	 */
	ret = lcd_load_module(mdir, mname, mloader_endpoint, mi);
	if (ret) {
		LCD_ERR("module load failed");
		goto fail1;
	}
	/*
	 * Initialize lcd's address space with module pages
	 *
	 * We have to do clean up if this fails (partially completed)
	 */
	ret = setup_addr_space(*slot_out, *mi);
	if (ret) {
		LCD_ERR("failed to set up addr space");
		goto fail2;
	}
	/*
	 * Configure lcd
	 */
	ret = lcd_config(*slot_out, (*mi)->init, 
			gva_add(LCD_STACK_GVA, LCD_STACK_SIZE - 8), /* stack */
			LCD_GV_PAGING_MEM_GPA,
			LCD_UTCB_GPA);
	if (ret) {
		LCD_ERR("failed to config lcd");
		goto fail3;
	}
	/*
	 * Done!
	 */
	return 0;

fail3:
fail2:
	/* 
	 * Remove module from capability system, and remove from host.
	 *
	 * This will not conflict with the lcd tear down below, because
	 * the microkernel won't try to double free the module pages. See
	 * comment below.
	 */
	lcd_unload_module(*mi, mloader_endpoint);
fail1:
	/*
	 * Should destroy lcd since this is the one and only capability to
	 * it. This will free up any gv paging memory that
	 * may have been partially alloc'd and mapped before fail2. It will
	 * also delete caps to module pages, but not try to free them, since
	 * they were initially added to the capability system using
	 * klcd_add_page.
	 */
	lcd_cap_delete(*slot_out);
fail0:
	return ret;
}

void klcd_destroy_module_lcd(cptr_t lcd, struct lcd_info *mi,
			cptr_t mloader_endpoint)
{
	/*
	 * See tear down comments in lcd_create_module_lcd
	 *
	 * We *must* delete the lcd first before unloading the module.
	 * Otherwise, if the lcd is still running, it will use the freed
	 * module pages.
	 */
	lcd_cap_delete(lcd);
	lcd_unload_module(mi, mloader_endpoint);
}

/* BOOT INFO -------------------------------------------------- */

static void dump_page_infos(struct list_head *page_infos,
			struct lcd_boot_info *bi,
			struct lcd_boot_info_for_page *dest)
{
	struct lcd_page_info_list_elem *e;
	struct list_head *cursor;
	unsigned idx;
	/*
	 * Write page information into boot info pages
	 */
	idx = 0;
	list_for_each(cursor, page_infos) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		dest[idx].my_cptr = e->my_cptr;
		dest[idx].page_gpa = e->page_gpa;
		idx++;
	}
}

static void dump_cptr_cache(struct cptr_cache *cache,
			struct lcd_boot_info *bi)
{
	/*
	 * Dump cptr cache info in boot page
	 */
	memcpy(bi->bmap0, cache->bmaps[0], sizeof(bi->bmap0));
	memcpy(bi->bmap1, cache->bmaps[1], sizeof(bi->bmap1));
	memcpy(bi->bmap2, cache->bmaps[2], sizeof(bi->bmap2));
	memcpy(bi->bmap3, cache->bmaps[3], sizeof(bi->bmap3));
}

static inline size_t page_infos_offset(void)
{
	size_t info_align;
	size_t offset;
	/*
	 * This may be too paranoid.
	 *
	 * The page infos go after the struct lcd_boot_info in the boot
	 * info pages, and I want to make sure they are written with the
	 * correct alignment.
	 *
	 * I'm using a static inline here instead of a macro for ease. All of
	 * these calculations can be done at compile time.
	 *
	 * ----------
	 *
	 * Calculate the alignment of the struct. It must be a power of
	 * 2, or else the alignment trick with the bit masking won't work
	 * correctly.
	 */
	info_align = __alignof__(struct lcd_boot_info_for_page);
	BUILD_BUG_ON(info_align & (info_align - 1)); /* not a power of 2 */
	/*
	 * Calculate the number of bytes in the struct lcd_boot_info.
	 */
	offset = sizeof(struct lcd_boot_info);
	/*
	 * Calculate the address of the first byte we can start writing
	 * page infos into.
	 */ 
	info_align--;
	offset = (offset + info_align) & ~info_align;
	/*
	 * Return the offset
	 */
	return offset;
}

static int dump_check(struct lcd_info *mi,
		size_t pi_offset,
		unsigned *num_boot_mem_pi,
		unsigned *num_stack_mem_pi,
		unsigned *num_paging_mem_pi,
		unsigned *num_free_mem_pi)
{
	unsigned num;
	struct list_head *cursor;
	unsigned long sz;
	/*
	 * Count number of page infos for each category
	 */
	num = 0;
	list_for_each(cursor, &mi->boot_mem_list) {
		num++;
	}
	*num_boot_mem_pi = num;
	num = 0;
	list_for_each(cursor, &mi->stack_mem_list) {
		num++;
	}
	*num_stack_mem_pi = num;
	num = 0;
	list_for_each(cursor, &mi->paging_mem_list) {
		num++;
	}
	*num_paging_mem_pi = num;
	num = 0;
	list_for_each(cursor, &mi->free_mem_list) {
		num++;
	}
	*num_free_mem_pi = num;
	/*
	 * We will need pi_offset (this contains the lcd_boot_info struct)
	 * plus the bytes for the page infos.
	 */
	sz = pi_offset;
	sz += *num_boot_mem_pi   * sizeof(struct lcd_boot_info_for_page);
	sz += *num_stack_mem_pi  * sizeof(struct lcd_boot_info_for_page);
	sz += *num_paging_mem_pi * sizeof(struct lcd_boot_info_for_page);
	sz += *num_free_mem_pi   * sizeof(struct lcd_boot_info_for_page);
	if (sz >= ((1 << LCD_BOOT_PAGES_ORDER) << PAGE_SHIFT))
		return -EINVAL;
	else
		return 0;
}

static unsigned long adjust_addr(unsigned long old, unsigned long my_base)
{
	unsigned long offset;
	/*
	 * Calculate offset into boot pages
	 */
	offset = old - my_base;
	/*
	 * Add offset to the address of the start of the lcd's boot pages
	 */
	return gva_val(LCD_BOOT_PAGES_GVA) + offset;
}

int klcd_dump_boot_info(struct lcd_info *mi)
{
	struct lcd_boot_info *bi;
	size_t pi_offset;
	struct lcd_boot_info_for_page *dest;
	int ret;

	bi = (struct lcd_boot_info *)mi->boot_page_base;
	
	/*
	 * Calculate the offset into the boot info pages where we should
	 * write the page infos.
	 */
	pi_offset = page_infos_offset();
	/*
	 * Ensure we have enough room to dump all of the boot info
	 */
	ret = dump_check(mi, pi_offset,
			&bi->num_boot_mem_pi,
			&bi->num_stack_mem_pi,
			&bi->num_paging_mem_pi,
			&bi->num_free_mem_pi);
	if (ret) {
		LCD_ERR("not enough room to put info in boot info pages");
		goto fail1;
	}
	/*
	 * Dump cptr cache info
	 */
	dump_cptr_cache(mi->cache, bi);
	/*
	 * Dump page info's
	 */
	dest = (struct lcd_boot_info_for_page *)(((char *)bi) + pi_offset);

	bi->boot_mem_pi_start = dest;
	dump_page_infos(&mi->boot_mem_list, bi, dest);
	dest += bi->num_boot_mem_pi;

	bi->stack_mem_pi_start = dest;
	dump_page_infos(&mi->stack_mem_list, bi, dest);
	dest += bi->num_stack_mem_pi;

	bi->paging_mem_pi_start = dest;
	dump_page_infos(&mi->paging_mem_list, bi, dest);
	dest += bi->num_paging_mem_pi;

	bi->free_mem_pi_start = dest;
	dump_page_infos(&mi->free_mem_list, bi, dest);

	/*
	 * Adjust addresses for lcd address space
	 */
	bi->boot_mem_pi_start = (struct lcd_boot_info_for_page *)
		adjust_addr((unsigned long)bi->boot_mem_pi_start,
			(unsigned long)mi->boot_page_base);

	bi->stack_mem_pi_start = (struct lcd_boot_info_for_page *)
		adjust_addr((unsigned long)bi->stack_mem_pi_start,
			(unsigned long)mi->boot_page_base);

	bi->paging_mem_pi_start = (struct lcd_boot_info_for_page *)
		adjust_addr((unsigned long)bi->paging_mem_pi_start,
			(unsigned long)mi->boot_page_base);

	bi->free_mem_pi_start = (struct lcd_boot_info_for_page *)
		adjust_addr((unsigned long)bi->free_mem_pi_start,
			(unsigned long)mi->boot_page_base);

	return 0;

fail1:
	return ret;
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_create);
EXPORT_SYMBOL(klcd_config);
EXPORT_SYMBOL(klcd_run);
EXPORT_SYMBOL(klcd_load_module);
EXPORT_SYMBOL(klcd_unload_module);
EXPORT_SYMBOL(klcd_create_module_lcd);
EXPORT_SYMBOL(klcd_destroy_module_lcd);
EXPORT_SYMBOL(klcd_dump_boot_info);

