/*
 * create.c - Basic code for creating an LCD.
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
			if (ret) {
				LCD_ERR("dup failed");
				goto fail1;
			}
			did_dup = 1;
		} else {
			/*
			 * No: the protected data is not in this page.
			 *
			 * Just add the page to caller's cspace
			 */
			ret = klcd_add_page(p, &pg_cptr);
			if (ret) {
				LCD_ERR("add failed");
				goto fail1;
			}
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
	if (ret) {
		LCD_ERR("init lcd info failed");
		goto fail0;
	}
	/*
	 * Load module in host
	 */
	ret = __klcd_get_module(mdir, mname, &m);
	if (ret) {
		LCD_ERR("loading module into host failed");
		goto fail1;
	}
	/*
	 * Get init and core pages
	 */
	ret = get_module_pages(va2hva(m->module_init), 
			m->init_size, &(*mi)->mpages_list,
			NULL);
	if (ret) {
		LCD_ERR("loading init pages failed");
		goto fail2;
	}
	ret = get_module_pages(va2hva(m->module_core), 
			m->core_size, &(*mi)->mpages_list,
			m);
	if (ret) {
		LCD_ERR("loading core pages failed");
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

#include <liblcd.h>
#include <lcd-domains/liblcd.h>
#include "../microkernel/internal.h"

int lcd_create(cptr_t *lcd)
{
	cptr_t slot;
	int ret;
	/*
	 * Alloc slot for new object
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Make LCD
	 */
	ret = __lcd_create(current->lcd, slot);
	if (ret) {
		LIBLCD_ERR("lcd create failed");
		goto fail2;
	}

	*lcd = slot;

	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int lcd_create_klcd(cptr_t *klcd)
{
	cptr_t slot;
	int ret;
	/*
	 * Alloc slot for new object
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Make kLCD
	 */
	ret = __lcd_create_klcd(current->lcd, slot);
	if (ret) {
		LIBLCD_ERR("klcd create failed");
		goto fail2;
	}

	*lcd = slot;

	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int lcd_config_registers(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
			gpa_t utcb_page)
{
	return __lcd_config(current->lcd, lcd, pc, sp, gva_root, utcb_page);
}

int lcd_memory_grant_and_map(cptr_t lcd, cptr_t mo, cptr_t dest_slot,
			gpa_t base);
{
	return __lcd_memory_grant_and_map(current->lcd, lcd, mo,
					dest_slot, base);
}

int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return __lcd_cap_grant(current->lcd, lcd, src, dest);
}

int lcd_run(cptr_t lcd)
{
	return __lcd_run(current->lcd, lcd);
}
