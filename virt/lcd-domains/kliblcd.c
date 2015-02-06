/**
 * kliblcd.c - Code for microkernel interface for non-isolated code.
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/types.h>
#include <linux/mutex.h>
#include "internal.h"

/* CPTR CACHE -------------------------------------------------- */

struct cptr_cache {
	DECLARE_BITMAP(bmap, LCD_MAX_CPTRS);
	struct mutex lock;
};

static int cptr_cache_init(struct cptr_cache **out)
{
	*out = kzalloc(sizeof(**out), GFP_KERNEL);
	if (!(*out))
		return -ENOMEM;
	mutex_init(&(*out)->lock);
	return 0;
}

static void cptr_cache_destroy(struct cptr_cache *cache)
{
	kfree(cache);
}

static int __lcd_alloc_cptr(struct cptr_cache *cptr_cache, cptr_t *free_cptr)
{
	int ret;

	ret = mutex_lock_interruptible(&cptr_cache->lock); 
	if (ret) {
		LCD_ERR("interrupted");
		goto fail1;
	}
	/*
	 * Find next zero bit
	 */
	*free_cptr = __cptr(
		find_first_zero_bit(cptr_cache->bmap, 
				LCD_MAX_CPTRS));
	if (cptr_val(*free_cptr) >= LCD_MAX_CPTRS) {
		LCD_ERR("out of cptrs");
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Set bit to mark cptr as in use
	 */
	set_bit(cptr_val(*free_cptr), cptr_cache->bmap);
	
	mutex_unlock(&cptr_cache->lock);

	return 0; 

fail2:
	mutex_unlock(&cptr_cache->lock);
fail1:
	return ret;
}

void __lcd_free_cptr(struct cptr_cache *cptr_cache, cptr_t c)
{
	int ret;
	/*
	 * Ensure cptr isn't too big
	 */ 
	if (cptr_val(c) >= LCD_MAX_CPTRS) {
		LCD_ERR("cptr %llu bigger than max cptrs %llu",
			cptr_val(c), LCD_MAX_CPTRS);
		return;
	}

	ret = mutex_lock_interruptible(&cptr_cache->lock);
	if (ret) {
		LCD_ERR("interrupted");
		return;
	}
	/*
	 * Clear the bit in the bitmap
	 */
	clear_bit(cptr_val(c), cptr_cache->bmap);

	mutex_unlock(&cptr_cache->lock);

	return; 
}

int lcd_alloc_cptr(cptr_t *free_slot)
{
	return __lcd_alloc_cptr(current->cptr_cache, free_slot);
}

void lcd_free_cptr(cptr_t c)
{
	__lcd_free_cptr(current->cptr_cache, c);
}


/* KLCD SPECIAL HANDLING -------------------------------------------------- */

int klcd_enter(void)
{
	int ret;
	/*
	 * Set up cptr cache
	 */
	ret = cptr_cache_init(&current->cptr_cache);
	if (ret) {
		LCD_ERR("cptr cache init");
		goto fail1;
	}
	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail2;
	}

	return 0;
fail2:
	cptr_cache_destroy(current->cptr_cache);
fail1:
	return ret;
}

void klcd_exit(void)
{
	/*
	 * Exit from lcd mode
	 */
	__klcd_exit();
	/*
	 * Destroy cptr cache
	 */
	cptr_cache_destroy(current->cptr_cache);
}

int klcd_add_page(struct page *p, cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	/*
	 * Insert page
	 */
	ret = __lcd_cap_insert(&current->lcd->cspace, *slot_out, p,
			LCD_CAP_TYPE_KPAGE);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

void klcd_rm_page(cptr_t slot)
{
	/*
	 * Delete page from my cspace
	 */
	lcd_cap_delete(slot);
	/*
	 * Return cptr to cache
	 */
	lcd_free_cptr(slot);
}

/* LOW LEVEL PAGE ALLOCATION ---------------------------------------- */

static int __lcd_page_alloc(cptr_t *slot_out, hpa_t *hpa_out, hva_t *hva_out)
{
	int ret;
	/*
	 * Allocate a cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	/*
	 * Get free page
	 */
	ret = __klcd_page_zalloc(current->lcd, *slot_out, hpa_out, hva_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int lcd_page_alloc(cptr_t *slot_out)
{
	hpa_t hpa;
	hva_t hva;
	return __lcd_page_alloc(slot_out, &hpa, &hva);
}

int lcd_page_map(cptr_t page, gpa_t gpa)
{
	/*
	 * Not allowed in a klcd for now
	 */
	return -ENOSYS;
}

int lcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out)
{
	hpa_t hpa;
	hva_t hva;
	int ret;
	ret = __lcd_page_alloc(slot_out, &hpa, &hva);
	if (ret)
		goto fail1;
	*gpa_out = __gpa(hpa_val(hpa));
	*gva_out = __gva(hva_val(hva));

	return 0;

fail1:
	return ret;
}

/* IPC -------------------------------------------------- */

int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	/*
	 * Get new endpoint
	 */
	ret = __lcd_create_sync_endpoint(current->lcd, *slot_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int lcd_send(cptr_t endpoint)
{
	return __lcd_send(current->lcd, endpoint);
}

int lcd_recv(cptr_t endpoint)
{
	return __lcd_recv(current->lcd, endpoint);
}

int lcd_call(cptr_t endpoint)
{
	return __lcd_call(current->lcd, endpoint);
}

int lcd_reply(void)
{
	return __lcd_reply(current->lcd);
}


/* LCD CREATE / CONFIG -------------------------------------------------- */


int lcd_create(cptr_t *slot_out, gpa_t stack)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	ret = __lcd_create(current->lcd, *slot_out, stack);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int lcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root)
{
	return __lcd_config(current->lcd, lcd, pc, sp, gva_root);
}

int lcd_run(cptr_t lcd)
{
	return __lcd_run(current->lcd, lcd);
}

int lcd_suspend(cptr_t lcd)
{
	return __lcd_suspend(current->lcd, lcd);
}

/* CAPABILITIES -------------------------------------------------- */

int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return __lcd_cap_grant_cheat(current->lcd, lcd, src, dest);
}

int lcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, gpa_t gpa)
{
	return __lcd_cap_page_grant_map_cheat(current->lcd, lcd, page, dest, 
					gpa);
}

void lcd_cap_delete(cptr_t slot)
{
	/*
	 * Delete capability from cspace
	 */
	__lcd_cap_delete(&current->lcd->cspace, slot);
	/*
	 * Return cptr
	 */
	lcd_free_cptr(slot);
}

int lcd_cap_revoke(cptr_t slot)
{
	/*
	 * Revoke child capabilities
	 *
	 * XXX: How do the lcd's know these slots are now free? The microkernel
	 * won't tell them.
	 */
	return __lcd_cap_revoke(&current->lcd->cspace, slot);
}

/* MODULE LOADING -------------------------------------------------- */

/**
 * Loads module into host address space, and stores pointer to
 * struct module in lcd.
 */
static int get_module(char *module_name, struct module **m)
{
	int ret;
	/*
	 * Load the requested module
	 */
	ret = request_lcd_module(module_name);
	if (ret < 0) {
		LCD_ERR("load module");
		goto fail1;
	}
	/*
	 * Find loaded module, and inc its ref counter; must hold module mutex
	 * while finding module.
	 */
	mutex_lock(&module_mutex);
	*m = find_module(module_name);
	mutex_unlock(&module_mutex);	
	if (!(*m)) {
		LCD_ERR("couldn't find module");
		goto fail2;
	}
	if(!try_module_get(*m)) {
		LCD_ERR("incrementing module ref count");
		goto fail3;
	}

	return ret;

fail3:
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
	(*m) = NULL;
fail2:
fail1:
	return ret;
}

static int get_module_pages(hva_t hva, unsigned long size, 
			struct list_head *mpage_list)
{
	int ret;
	unsigned long mapped;
	struct page *p;
	cptr_t pg_cptr;
	struct lcd_module_page *mp;

	mapped = 0;
	while (mapped < size) {
		/*
		 * Get module page
		 */
		p = vmalloc_to_page(hva2va(hva));
		/*
		 * Add page to klcd
		 */
		ret = klcd_add_page(p, &pg_cptr);
		if (ret)
			goto fail1;
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
		list_add(&mp->list, mpage_list);
		/*
		 * Increment ...
		 */
		mapped += PAGE_SIZE;
		hva = hva_add(hva, PAGE_SIZE);
	}
	return 0;

fail2:
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

int lcd_load_module(char *mname, cptr_t mloader_endpoint, 
		struct lcd_module_info **mi)
{
	int ret;
	/*
	 * Ignore mloader_endpoint - we will use standard module loading
	 * code.
	 */
	struct module *m;
	
	*mi = kmalloc(sizeof(mi), GFP_KERNEL);
	if (!*mi) {
		ret = -ENOMEM;
		goto fail0;
	}
	/*
	 * Load module in host
	 */
	ret = get_module(mname, &m);
	if (ret)
		goto fail1;
	/*
	 * Get init and core pages
	 */
	ret = get_module_pages(va2hva(m->module_init), 
			m->init_size, &(*mi)->mpages_list);
	if (ret) {
		goto fail2;
	}
	ret = get_module_pages(va2hva(m->module_core), 
			m->core_size, &(*mi)->mpages_list);
	if (ret) {
		goto fail3;
	}
	/*
	 * Copy name and module init address
	 */
	(*mi)->init = __gva(hva_val(va2hva(m->module_init)));
	strncpy((*mi)->mname, mname, LCD_MODULE_NAME_MAX);
	/*
	 * Init cptr counter to 1, to skip over null cptr
	 */
	(*mi)->cptr_counter = 1;

	return 0;

fail3:
fail2:
	free_module_pages(&(*mi)->mpages_list);
fail1:
	kfree(*mi);
fail0:
	return ret;
}

void lcd_unload_module(struct lcd_module_info *mi, cptr_t mloader_endpoint)
{
	int ret;
	/*
	 * module loader endpoint ignored; use standard module loading system
	 */
	struct module *m;
	/*
	 * Remove module pages from capability system
	 */
	free_module_pages(&mi->mpages_list);
	/*
	 * Delete module
	 *
	 * We need to look it up so we can do a put
	 */
 	mutex_lock(&module_mutex);
 	m = find_module(mi->mname);
 	mutex_unlock(&module_mutex);
	if (!m) {
		LCD_ERR("couldn't find module");
		goto free_mi;
	}
	module_put(m);
	ret = do_sys_delete_module(mi->mname, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
free_mi:
	/*
	 * Free lcd module info
	 */
	kfree(mi);
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

struct gv_cxt {
	struct hpa_cptr_tuple gpa2hpacptr[LCD_GV_PAGING_MEM_SIZE >> PAGE_SHIFT];
	unsigned int counter;
	pgd_t *root;
};

static int gv_init(struct gv_cxt **cxt_out)
{
	/*
	 * Allocate context
	 *
	 * XXX: This is a big chunk of memory. But it will do for now ...
	 */
	*cxt_out = kzalloc(sizeof(struct gv_cxt), GFP_KERNEL);
	if (!*cxt_out) {
		LCD_ERR("no mem");
		return -ENOMEM;
	}

	return 0;
}

static void gv_destroy(struct gv_cxt *cxt)
{
	int i;
	cptr_t c;
	/*
	 * Delete pages from our cspace
	 */
	for (i = 0; i < (LCD_GV_PAGING_MEM_SIZE >> PAGE_SHIFT); i++) {
		c = cxt->gpa2hpacptr[i].cptr;
		if (!cptr_val(c))
			break; /* reached end of used paging mem */
		lcd_cap_delete(c);
	}
	/*
	 * Free cxt
	 */
	kfree(cxt);
}

static int gv_gfp(cptr_t lcd, struct gv_cxt *cxt, struct lcd_module_info *mi,
		gpa_t *gpa_out)
{
	cptr_t slot;
	int ret;
	gpa_t gpa;
	gva_t gva;
	hpa_t hpa;
	hva_t hva;
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
	hva = __hva(gva_val(gva));
	/*
	 * Grant and map in lcd
	 *
	 * The capability will go in slot cptr_counter in the lcd.
	 *
	 * The page will be mapped at the gpa given below (use the counter
	 * as an offset into the chunk of the guest physical address space
	 * reserved for paging memory).
	 */
	*gpa_out = gpa_add(LCD_GV_PAGING_MEM_GPA,
			cxt->counter * PAGE_SIZE);
	ret = lcd_cap_page_grant_map(lcd, slot, __cptr(mi->cptr_counter),
				*gpa_out);
	if (ret) {
		LCD_ERR("mapping page");
		goto fail3;
	}
	/*
	 * Store correspondence from lcd gpa to the caller's address space
	 */
	cxt->gpa2hpacptr[cxt->counter].hpa = hpa;
	cxt->gpa2hpacptr[cxt->counter].cptr = slot;
	/*
	 * Bump counters
	 */
	cxt->counter++;
	mi->cptr_counter++;

	return 0;

fail3:
	lcd_cap_delete(slot); /* will free page */
fail2:
fail1:
	return ret;	
}

static int gv_gpa2hpa(struct gv_cxt *cxt, gpa_t gpa, hpa_t *hpa_out)
{
	int ret;
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
	 * Determine guest physical page frame
	 */
	pfn = gpa_val(gpa) >> PAGE_SHIFT;
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
static int gv_setup_pgd(cptr_t lcd, struct gv_cxt *cxt, 
			struct lcd_module_info *mi)
{
	gpa_t gpa;
	hpa_t hpa;
	int ret;
	ret = gv_gfp(lcd, cxt, mi, &gpa);
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
static int gv_lookup_pte(struct gv_cxt *cxt, gva_t gva, pmd_t *pmd_entry, 
			pte_t **pte_out)
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
static int gv_walk_pt(struct gv_cxt *cxt, gva_t gva, pmd_t *pmd_entry,
		pte_t **pte_out)
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
static int gv_lookup_pmd(struct gv_cxt *cxt, gva_t gva, pud_t *pud_entry,
			pmd_t **pmd_out)
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
static int gv_walk_pmd(cptr_t lcd, struct gv_cxt *cxt, 
		struct lcd_module_info *mi, gva_t gva, 
		pud_t *pud_entry, pmd_t **pmd_out)
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
		ret = gv_gfp(lcd, cxt, mi, &gpa);
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
static int gv_lookup_pud(struct gv_cxt *cxt, gva_t gva, pgd_t *pgd_entry,
				pud_t **pud_out)
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
static int gv_walk_pud(cptr_t lcd, struct gv_cxt *cxt, 
		struct lcd_module_info *mi, gva_t gva, 
		pgd_t *pgd_entry, pud_t **pud_out)
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
		ret = gv_gfp(lcd, cxt, mi, &gpa);
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
static int gv_walk_pgd(cptr_t lcd, struct lcd_module_info *mi, 
			struct gv_cxt *cxt, gva_t gva, 
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
		ret = gv_gfp(lcd, cxt, mi, &gpa);
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
static int gv_walk(cptr_t lcd, struct gv_cxt *cxt, struct lcd_module_info *mi,
		gva_t gva, pte_t **pte_out)
{
	int ret;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	/*
	 * Get pgd entry for pud
	 */
	ret = gv_walk_pgd(lcd, mi, cxt, gva, &pgd);
	if (ret) {
		LCD_ERR("walking pgd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pud entry for pmd
	 */
	ret = gv_walk_pud(lcd, cxt, mi, gva, pgd, &pud);
	if (ret) {
		LCD_ERR("walking pud for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pmd entry for page table
	 */
	ret = gv_walk_pmd(lcd, cxt, mi, gva, pud, &pmd);
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
static int gv_map(cptr_t lcd, struct gv_cxt *cxt, struct lcd_module_info *mi,
		gva_t gva, gpa_t gpa)
{
	int ret;
	pte_t *pte;

	ret = gv_walk(lcd, cxt, mi, gva, &pte);
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
static int gv_map_range(cptr_t lcd, struct gv_cxt *cxt, 
			struct lcd_module_info *mi, gva_t gva_start, 
			gpa_t gpa_start, unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (gv_map(lcd, cxt, mi,
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

static int map_module(cptr_t lcd, struct gv_cxt *cxt,
		struct lcd_module_info *mi)
{
	struct list_head *cursor;
	struct lcd_module_page *mp;
	unsigned long offset;
	gpa_t gpa;
	int ret = 0;

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
		 * Grant and map in lcd's guest physical
		 */
		ret = lcd_cap_page_grant_map(lcd, mp->cptr, 
					__cptr(mi->cptr_counter),
					gpa);
		if (ret) {
			LCD_ERR("couldn't map module page in lcd's gp");
			goto fail1;
		}
		/*
		 * Map in lcd's guest virtual
		 */
		ret = gv_map(lcd, cxt, mi, mp->gva, gpa);
		if (ret) {
			LCD_ERR("couldn't map in lcd's gv");
			goto fail2;
		}
		/*
		 * Bump counter / offset
		 */
		mi->cptr_counter++;
		offset += PAGE_SIZE;
	}

fail2:
fail1:
	return ret;	
}

static int map_gv_memory_and_stack(cptr_t lcd, struct gv_cxt *cxt, 
				struct lcd_module_info *mi)
{
	/*
	 * Map paging mem and stack/utcb
	 */
	return gv_map_range(lcd, cxt, mi,
			LCD_GV_PAGING_MEM_GVA,
			LCD_GV_PAGING_MEM_GPA,
			(gpa_val(LCD_MODULE_GPA) - 
				gpa_val(LCD_GV_PAGING_MEM_GPA)) >> PAGE_SHIFT);
}

static int setup_addr_space(cptr_t lcd, struct lcd_module_info *mi)
{
	struct gv_cxt *cxt;
	int ret;
	/*
	 * Set up guest virtual cxt
	 */
	ret = gv_init(&cxt);
	if (ret)
		goto fail1;
	/*
	 * Set up root page directory
	 */
	ret = gv_setup_pgd(lcd, cxt, mi);
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
	 * Map guest virtual paging memory and stack/utcb
	 */
	ret = map_gv_memory_and_stack(lcd, cxt, mi);
	if (ret) {
		LCD_ERR("mapping paging mem");
		goto fail4;
	}
	/*
	 * Remove our references to the guest virtual paging memory, so
	 * the pages will be freed when the lcd is torn down.
	 */
	gv_destroy(cxt);

fail4:
fail3:
fail2:
	/* gv_destroy just removes our caps to the gv paging pages; gv paging 
	 * mem will be freed when lcd is destroyed.
	 *
	 * module pages will be freed when lcd_unload_module is called
	 */
	gv_destroy(cxt);
fail1:
	return ret;		
}

int lcd_create_module_lcd(cptr_t *slot_out, char *mname, 
			cptr_t mloader_endpoint, struct lcd_module_info **mi)
			
{
	int ret;
	/*
	 * Create an empty lcd
	 */
	ret = lcd_create(slot_out, LCD_STACK_GPA);
	if (ret) {
		LCD_ERR("lcd create failed");
		goto fail0;
	}
	/*
	 * Load module
	 */
	ret = lcd_load_module(mname, mloader_endpoint, mi);
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
			gva_add(LCD_STACK_GVA, (4 << 10) - 1),
			LCD_GV_PAGING_MEM_GPA);
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

void lcd_destroy_module_lcd(cptr_t lcd, struct lcd_module_info *mi,
			cptr_t mloader_endpoint)
{
	/*
	 * See tear down comments in lcd_create_module_lcd
	 */
	lcd_unload_module(mi, mloader_endpoint);
	lcd_cap_delete(lcd);
}

/* INIT / EXIT -------------------------------------------------- */

static int kliblcd_tests(void);

int __kliblcd_init(void)
{
	/* nothing else for now */
	return kliblcd_tests();
}

void __kliblcd_exit(void)
{
	return;
}

#include "tests/kliblcd-tests.c"
