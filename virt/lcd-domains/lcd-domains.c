/**
 * lcd-domains.c - Main file for the LCD module
 *
 *
 * Authors:
 *   Anton Burtsev     <aburtsev@flux.utah.edu>
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/kthread.h>
#include <linux/kmod.h>
#include <linux/mm.h>

#include <linux/lcd-domains.h>
#include <asm/lcd-domains-arch.h>
#include <lcd-domains/lcd-domains.h>
#include <lcd-domains/syscall.h>
#include <lcd-domains/ipc.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* DEBUG -------------------------------------------------- */

#define LCD_DEBUG 0

#define LCD_ERR(msg...) __lcd_err(__FILE__, __LINE__, msg)
static inline void __lcd_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_MSG(msg...) __lcd_msg(__FILE__, __LINE__, msg)
static inline void __lcd_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_WARN(msg...) __lcd_warn(__FILE__, __LINE__, msg)
static inline void __lcd_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

/* MEMORY ALLOCATION -------------------------------------------------- */

/**
 * Converts a guest physical "page frame number" in the lcd's page heap
 * into a guest physical address.
 */
static inline gpa_t lcd_mm_gp_pfn_to_gpa(unsigned long gp_pfn)
{
	return __gpa((gp_pfn << PAGE_SHIFT) + LCD_GP_MEM_START);
}

/**
 * Converts a guest physical address to a "page frame number" in the lcd's 
 * page heap.
 */
static inline unsigned long lcd_mm_gpa_to_gp_pfn(gpa_t gpa)
{
	return (gpa_val(gpa) - LCD_GP_MEM_START) >> PAGE_SHIFT;
}

/**
 * Allocates a page in the lcd's guest physical address space and maps it to
 * the host physical page at hpa.
 *
 * Note: Guest physical and host allocation are decoupled so we can allocate
 * guest physical memory for things already in the host, like modules.
 *
 * If gpa_out is non-NULL, returns the guest physical address of the
 * page.
 */
static int lcd_mm_gp_alloc_map(struct lcd *lcd, hpa_t hpa, gpa_t *gpa_out)
{
	unsigned long gp_pfn;
	gpa_t gpa;
	int ret;
	/*
	 * Find the next free guest physical page
	 */
	gp_pfn = find_first_zero_bit(lcd->gp_paging.bmap, LCD_GP_BMAP_NBITS);
	if (gp_pfn >= LCD_GP_BMAP_NBITS) {
		LCD_ERR("out of guest physical mem");
		ret = -ENOMEM;
		goto fail1;
	}
	gpa = lcd_mm_gp_pfn_to_gpa(gp_pfn);
	/*
	 * Map gpa to hpa
	 */
	ret = lcd_arch_ept_map(lcd->lcd_arch, gpa, hpa, 1, 0);
	if (ret) {
		LCD_ERR("couldn't map gpa %lx to hpa %lx\n",
			gpa_val(gpa),
			hpa_val(hpa));
		goto fail2;
	}
	/*
	 * Set bit to mark page as alloc'd
	 */
	set_bit(gp_pfn, lcd->gp_paging.bmap);

	if (gpa_out)
		*gpa_out = gpa;
	
	return 0;

fail2:
	clear_bit(gp_pfn, lcd->gp_paging.bmap);
fail1:
	return ret;
}

/**
 * Allocates a zeroed out host page *and* a guest physical page. Maps the
 * host page in the lcd's guest physical address space.
 *
 * If hva_out and/or gpa_out are non-NULL, returns host virtual/guest physical
 * address of allocated page, for convenience.
 */
static int lcd_mm_gp_gfp(struct lcd *lcd, hva_t *hva_out, gpa_t *gpa_out)
{
	hva_t hva;
	hpa_t hpa;
	int ret;
	/*
	 * Allocate a host physical page
	 */
	hva = __hva(get_zeroed_page(GFP_KERNEL));
	if (!hva_val(hva)) {
		LCD_ERR("no host mem");
		ret = -ENOMEM;
		goto fail1;
	}
	hpa = hva2hpa(hva);
	/*
	 * Allocate a guest physical page and map it to host page
	 */
	ret = lcd_mm_gp_alloc_map(lcd, hpa, gpa_out);
	if (ret) {
		LCD_ERR("map in guest phys");
		goto fail2;
	}

	if (hva_out)
		*hva_out = hva;

	return 0;

fail2:
	free_page(hva_val(hva));
fail1:
	return ret;
}

/**
 * Unmaps the page at gpa in lcd's guest physical address space.
 *
 * If hpa_out is non-NULL, returns host physical address of page.
 */
static void lcd_mm_gp_dealloc_unmap(struct lcd *lcd, gpa_t gpa, hpa_t *hpa_out)
{
	int ret;
	unsigned long gp_pfn;
	/*
	 * Convert the gpa to a "page frame"
	 */
	gp_pfn = lcd_mm_gpa_to_gp_pfn(gpa);
	BUG_ON(gp_pfn >= LCD_GP_BMAP_NBITS);
	/*
	 * Clear the bit in the bitmap
	 */
	clear_bit(gp_pfn, lcd->gp_paging.bmap);
	/*
	 * Unmap in the lcd's ept
	 */
	ret = lcd_arch_ept_unmap2(lcd->lcd_arch, gpa, hpa_out);
	if (ret)
		LCD_ERR("ept unmap2");
}

/**
 * Frees the gpa in the guest physical address space of lcd, *and* on the
 * host (so there better be a host page corresponding to gpa, or else
 * you'll get a double free!).
 *
 * To unmap/free only in guest physical, do lcd_mm_gp_dealloc_unmap.
 */
static void lcd_mm_gp_free_page(struct lcd *lcd, gpa_t gpa)
{
	hpa_t hpa;
	/*
	 * Unmap in ept
	 */
	lcd_mm_gp_dealloc_unmap(lcd, gpa, &hpa);
	/*
	 * Free page
	 *
	 * XXX: hpa may be bogus if the above call failed
	 */
	free_page(hva_val(hpa2hva(hpa)));
}

/**
 * Frees any pages still mapped in the guest physical address space.
 *
 * XXX: Tries to free underlying host pages. Beware! If you leave something
 * mapped, you could get double frees for something already freed, or you
 * may have pages freed that you didn't mean to.
 */
static void lcd_mm_gp_free_all(struct lcd *lcd)
{
	unsigned long gp_pfn;
	gpa_t gpa;

	gp_pfn = 0;

	while (1) {
		/*
		 * Find the next mapped guest physical page (find_next_bit
		 * starts searching at gp_pfn).
		 */
		gp_pfn = find_next_bit(lcd->gp_paging.bmap, LCD_GP_BMAP_NBITS,
				gp_pfn);
		if (gp_pfn >= LCD_GP_BMAP_NBITS)
			break; /* no more set bits */
		/*
		 * Convert to guest physical, and free it
		 */
		gpa = lcd_mm_gp_pfn_to_gpa(gp_pfn);
		lcd_mm_gp_free_page(lcd, gpa);
	}
}

/**
 * *** THIS IS FOR SETTING UP THE BOOT GUEST VIRTUAL ADDRESS SPACE ONLY ***
 *
 * The LCD is responsible for allocating guest virtual pages thereafter.
 *
 * Allocates a host page for the lcd's guest virtual paging hierarchy. Tracks
 * how many of these are allocated using lcd's counter.
 *
 * Returns guest physical and host virtual addresses of page.
 */
static int lcd_mm_gv_gfp(struct lcd *lcd, gpa_t *gpa_out, hva_t *hva_out)
{
	int ret;
	/*
	 * Check counter
	 */
	if (lcd->gv_paging.counter >= LCD_GV_MEM_SIZE) {
		LCD_ERR("exhausted guest virtual paging mem");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Allocate a host page
	 */
	ret = lcd_mm_gp_gfp(lcd, hva_out, gpa_out);
	if (ret) {
		LCD_ERR("getting free page");
		goto fail2;
	}
	/*
	 * Bump counter etc.
	 */
	lcd->gv_paging.counter += PAGE_SIZE;

	return 0;

fail2:
fail1:
	return ret;
}

/* GUEST VIRTUAL PAGING (FOR BOOT) ---------------------------------------- */

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

/**
 * Initializes root page directory for guest virtual paging in lcd,
 * and sets the root directory pointer (on x86, %cr3).
 *
 * Must be called before mapping any gva's, or else you'll get a kernel
 * oops on the NULL %cr3 when we try to do a page walk.
 */
static int lcd_mm_gv_init(struct lcd *lcd)
{
	gpa_t gpa;
	hva_t hva;
	int ret;
	/*
	 * Alloc a page for the pgd. 
	 */
	ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
	if (ret) {
		LCD_ERR("alloc root page directory");
		goto fail1;
	}
	/*
	 * Store the root pointer
	 */
	lcd->gv_paging.root = hva2va(hva);
	lcd->gv_paging.root_gpa = gpa;

	return 0;

fail1:
	return ret;
}

static int lcd_mm_pt_destroy(struct lcd *lcd, pmd_t *pmd_entry)
{
	gpa_t gpa;
	hpa_t hpa;
	int ret;
	pte_t* pt;
#if LCD_DEBUG
	int i;
#endif
	
	/*
	 * Get hpa of page table, using gpa stored in pmd_entry.
	 */
	gpa = pmd_gpa(pmd_entry);
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx\n", gpa_val(gpa));
		return ret;
	}

	pt = hpa2va(hpa);

#if LCD_DEBUG
	/*
	 * Check for any potential memory leaks
	 */
	for (i = 0; i < PTRS_PER_PTE; i++) {
		if (pte_present(pt[i])) {
			LCD_ERR("possible memory leak for gpa %lx (pt idx %d)\n",
				gpa_val(pte_gpa(&pt[i])), i);
			dump_stack();
		}
	}
#endif

	/*
	 * Unmap and free page table
	 */
	lcd_mm_gp_free_page(lcd, gpa);

	return 0;
}

static int lcd_mm_pmd_destroy(struct lcd *lcd, pud_t *pud_entry)
{
	pmd_t *pmd;
	gpa_t gpa;
	hpa_t hpa;
	int i;
	int ret;

	/*
	 * Get hpa of pmd, using gpa stored in pud_entry.
	 */
	gpa = pud_gpa(pud_entry);
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx\n",	gpa_val(gpa));
		return ret;
	}

	pmd = hpa2va(hpa);

	/*
	 * Free all present page tables
	 */
	for (i = 0; i < PTRS_PER_PMD; i++) {
		if (pmd_present(pmd[i])) {
			ret = lcd_mm_pt_destroy(lcd, &pmd[i]);
			if (ret) {
				LCD_ERR("destroying child pt");
				return ret;
			}
		}
	}

	/*
	 * Unmap and free pmd
	 */
	lcd_mm_gp_free_page(lcd, gpa);

	return 0;
}

static int lcd_mm_pud_destroy(struct lcd *lcd, pgd_t *pgd_entry)
{
	pud_t *pud;
	gpa_t gpa;
	hpa_t hpa;
	int i;
	int ret;

	/*
	 * Get hpa of pud, using gpa stored in pgd_entry.
	 */
	gpa = pgd_gpa(pgd_entry);
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx\n", gpa_val(gpa));
		return ret;
	}

	pud = hpa2va(hpa);

	/*
	 * Destroy all present pmd's
	 */
	for (i = 0; i < PTRS_PER_PUD; i++) {
		if (pud_present(pud[i])) {
			ret = lcd_mm_pmd_destroy(lcd, &pud[i]);
			if (ret) {
				LCD_ERR("destroying child pmd");
				return ret;
			}
		}
	}

	/*
	 * Unmap and free pud
	 */
	lcd_mm_gp_free_page(lcd, gpa);

	return 0;
}

/**
 * Unmaps and frees host memory used for lcd's guest virtual paging
 * hierarchy.
 *
 * NOTE: Does *not* free the mapped page frames themselves; just the
 * page tables, etc.
 *
 * XXX: This should probably get axed when we get more serious. LCDs can
 * modify their page tables.
 */
static void lcd_mm_gva_destroy(struct lcd *lcd)
{
	pgd_t *pgd;
	int i;
	int ret;

	pgd = lcd->gv_paging.root;
	/*
	 * Free all present pud's
	 */
	for (i = 0; i < PTRS_PER_PGD; i++) {
		if (pgd_present(pgd[i])) {
			ret = lcd_mm_pud_destroy(lcd, &pgd[i]);
			if (ret) {
				LCD_ERR("freeing pud at idx %d\n", i);
				return;
			}
		}
	}

	/*
	 * Unmap and free root page dir
	 */
	lcd_mm_gp_free_page(lcd, lcd->gv_paging.root_gpa);
	lcd->gv_paging.root = NULL;
}

/**
 * Get host virtual address of pte for gva and pmd_entry.
 */
static int lcd_mm_gva_lookup_pte(struct lcd *lcd, gva_t gva, pmd_t *pmd_entry,
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
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx", gpa_val(gpa));
		return ret;
	}
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
static int lcd_mm_gva_walk_pt(struct lcd *lcd, gva_t gva, pmd_t *pmd_entry,
			pte_t **pte_out)
{
	int ret;
	pte_t *entry;

	ret = lcd_mm_gva_lookup_pte(lcd, gva, pmd_entry, &entry);
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
static int lcd_mm_gva_lookup_pmd(struct lcd *lcd, gva_t gva, pud_t *pud_entry,
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
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx", gpa_val(gpa));
		return ret;
	}
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
static int lcd_mm_gva_walk_pmd(struct lcd *lcd, gva_t gva, pud_t *pud_entry,
				pmd_t **pmd_out)
{
	int ret;
	pmd_t *entry;
	gpa_t gpa;
	hva_t hva;

	ret = lcd_mm_gva_lookup_pmd(lcd, gva, pud_entry, &entry);
	if (ret) {
		LCD_ERR("looking up pmd for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pmd_present(*entry)) {
		/*
		 * Alloc and map a page table
		 */
		ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
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
static int lcd_mm_gva_lookup_pud(struct lcd *lcd, gva_t gva, pgd_t *pgd_entry,
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
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		LCD_ERR("looking up gpa %lx\n", gpa_val(gpa));
		return ret;
	}
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
static int lcd_mm_gva_walk_pud(struct lcd *lcd, gva_t gva, pgd_t *pgd_entry,
				pud_t **pud_out)
{
	int ret;
	pud_t *entry;
	gpa_t gpa;
	hva_t hva;

	ret = lcd_mm_gva_lookup_pud(lcd, gva, pgd_entry, &entry);
	if (ret) {
		LCD_ERR("looking up pud for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pud_present(*entry)) {
		/*
		 * Alloc and map a pmd
		 */
		ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
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
static int lcd_mm_gva_walk_pgd(struct lcd *lcd, gva_t gva, pgd_t **pgd_out)
{
	int ret;
	pgd_t *entry;
	gpa_t gpa;
	hva_t hva;

	entry = lcd->gv_paging.root + pgd_index(gva_val(gva));
	if (!pgd_present(*entry)) {
		/*
		 * Alloc and map a pud
		 */
		ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
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
 * (No huge pages are used for the initial guest
 * virtual address space. But the intra-lcd code can
 * change that if it wishes.)
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
static int lcd_mm_gva_walk(struct lcd *lcd, gva_t gva, pte_t **pte_out)
{
	int ret;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	/*
	 * Get pgd entry for pud
	 */
	ret = lcd_mm_gva_walk_pgd(lcd, gva, &pgd);
	if (ret) {
		LCD_ERR("walking pgd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pud entry for pmd
	 */
	ret = lcd_mm_gva_walk_pud(lcd, gva, pgd, &pud);
	if (ret) {
		LCD_ERR("walking pud for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pmd entry for page table
	 */
	ret = lcd_mm_gva_walk_pmd(lcd, gva, pud, &pmd);
	if (ret) {
		LCD_ERR("walking pmd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Finally, get page table entry
	 */
	return lcd_mm_gva_walk_pt(lcd, gva, pmd, pte_out);
}

static void lcd_mm_gva_set(pte_t *pte, gpa_t gpa)
{
	set_pte_gpa(pte, gpa);
}

static void lcd_mm_gva_unset(pte_t *pte)
{
	*pte = __pte(0);
}

static gpa_t lcd_mm_gva_get(pte_t *pte)
{
	return pte_gpa(pte);
}

/**
 * Simple routine combining walk and set. Never
 * overwrites.
 */
static int lcd_mm_gva_map(struct lcd *lcd, gva_t gva, gpa_t gpa)
{
	int ret;
	pte_t *pte;

	ret = lcd_mm_gva_walk(lcd, gva, &pte);
	if (ret) {
		LCD_ERR("getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	if (pte_present(*pte)) {
		LCD_ERR("remap gva %lx to gpa %lx (was %lx)\n",
			gva_val(gva), gpa_val(gpa),
			gpa_val(lcd_mm_gva_get(pte)));
		return -EINVAL;
	}

	lcd_mm_gva_set(pte, gpa);

	return 0;
}

/**
 * Simple routine combining walk and unset.
 */
static int lcd_mm_gva_unmap(struct lcd *lcd, gva_t gva)
{
	int ret;
	pte_t *pte;

	ret = lcd_mm_gva_walk(lcd, gva, &pte);
	if (ret) {
		LCD_ERR("getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pte_present(*pte)) {
		LCD_ERR("no mapping for gva %lx\n", gva_val(gva));
		return -EINVAL;
	}

	lcd_mm_gva_unset(pte);

	return 0;
}

/**
 * Simple routine combining walk and unset. Returns gpa in gpa_out.
 */
static int lcd_mm_gva_unmap2(struct lcd *lcd, gva_t gva, gpa_t *gpa_out)
{
	int ret;
	pte_t *pte;

	ret = lcd_mm_gva_walk(lcd, gva, &pte);
	if (ret) {
		LCD_ERR("getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pte_present(*pte)) {
		LCD_ERR("no mapping for gva %lx\n", gva_val(gva));
		return -EINVAL;
	}

	*gpa_out = lcd_mm_gva_get(pte);

	lcd_mm_gva_unset(pte);

	return 0;
}

/**
 * Simple routine combining walk and get. Never
 * overwrites.
 */
static int lcd_mm_gva_to_gpa(struct lcd *lcd, gva_t gva, gpa_t *gpa)
{
	int ret;
	pte_t *pte;

	ret = lcd_mm_gva_walk(lcd, gva, &pte);
	if (ret) {
		LCD_ERR("getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	*gpa = lcd_mm_gva_get(pte);

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
static int lcd_mm_gva_map_range(struct lcd *lcd, gva_t gva_start, 
				gpa_t gpa_start, unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_mm_gva_map(lcd,
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

/**
 * Unmaps 
 *
 *    gva_start --> gva_start + npages * PAGE_SIZE
 *
 * in lcd's guest virtual paging tables.
 */
static int lcd_mm_gva_unmap_range(struct lcd *lcd, gva_t gva_start, 
				unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_mm_gva_unmap(lcd, gva_add(gva_start, off))) {
			LCD_ERR("unmapping gva %lx\n",
				gva_val(gva_add(gva_start,off)));
			return -EIO;
		}
	}

	return 0;
}

/* VMALLOC PAGE WALK ---------------------------------------- */

/**
 * Walks physical pages in group of vmalloc pages that start at hva and
 * end after hva + size bytes, fires callback with host virtual and
 * physical addresses for each page.
 *
 * Caller is responsible for undoing effects of callback if there's an
 * error.
 *
 * (Because module memory is allocated with vmalloc, there's 
 * no guarantee the physical pages are contiguous, hence why we need to
 * do this.)
 *
 * (Jithu, I owe you one for implementing this in the original code.)
 */
static int lcd_vmalloc_walk(struct lcd *lcd, hva_t hva, unsigned long size,
			int (*cb)(struct lcd *, hpa_t, hva_t))
{
	int ret;
	hpa_t hpa;
	unsigned long mapped;

	mapped = 0;
	while (mapped < size) {
		/*
		 * Convert hva to correct hpa
		 */
		hpa = __hpa(vmalloc_to_pfn(hva2va(hva)) << PAGE_SHIFT);
		/*
		 * Fire callback
		 */
		ret = cb(lcd, hpa, hva);
		if (ret)
			return ret;
		/*
		 * Increment ...
		 */
		mapped += PAGE_SIZE;
		hva = hva_add(hva, PAGE_SIZE);
	}
	return 0;
}

/**
 * Callback used with vmalloc walk for modules that maps a page of a module
 * into the lcd. To avoid relocating symbols, we just set gva = hva.
 */
static int lcd_module_map_page(struct lcd *lcd, hpa_t hpa, hva_t hva)
{
	int ret;
	gpa_t gpa;
	gva_t gva;

	gva = __gva(hva_val(hva));

	/*
	 * Map module page in lcd's ept
	 */
	ret = lcd_mm_gp_alloc_map(lcd, hpa, &gpa);
	if (ret) {
		LCD_ERR("error mapping into ept");
		return ret;
	}
	/*
	 * Map module page into lcd's guest virtual
	 */
	ret = lcd_mm_gva_map(lcd, gva, gpa);
	if (ret) {
		LCD_ERR("error mapping into gv");
		return ret;
	}
	return 0;
}

/**
 * Callback used with vmalloc walk to unmap module pages.
 */
static int lcd_module_unmap_page(struct lcd *lcd, hpa_t hpa, hva_t hva)
{
	int ret;
	gpa_t gpa;
	gva_t gva;

	gva = __gva(hva_val(hva));

	/*
	 * Unmap module page in lcd's guest virtual
	 */
	ret = lcd_mm_gva_unmap2(lcd, gva, &gpa);
	if (ret) {
		LCD_ERR("error unmapping gva in gv");
		return ret;
	}
	/*
	 * Unmap module page in lcd's ept
	 */
	lcd_mm_gp_dealloc_unmap(lcd, gpa, NULL);
	return 0;
}


/* LCD CREATE / DESTROY -------------------------------------------------- */

/**
 * Maps the module in the guest physical and virtual address space of
 * the lcd. This is a bit complicated because modules are mapped in the host
 * using vmalloc. Unlike kmalloc, vmalloc allocates at page granularity so we
 * don't need to worry about parts of the host "sneaking into" the lcd.
 *
 * A module is made up of two parts: init and core. Init is code and data
 * marked with __init and sit in the module's .init sections 
 * (e.g., .init.text); these are unloaded for *non-lcd modules*
 * after the module's init returns. We have patched the module loading code
 * to prevent this from happening for lcd modules. Core is basically
 * everything else in the module (code, data).
 *
 * On failure, the module may be partially mapped in the guest physical
 * and/or guest virtual address spaces. The caller is responsible for
 * taking this (e.g., during the ept/guest virtual tear down).
 */
static int lcd_setup_module_address_space(struct lcd *lcd)
{
	int ret;
	/*
	 * Map the module's init code
	 */
	ret = lcd_vmalloc_walk(lcd, va2hva(lcd->module->module_init), 
			lcd->module->init_size, lcd_module_map_page);
	if (ret) {
		LCD_ERR("mapping module's init");
		return ret;
	}
	/*
	 * Map the module's core code
	 */
	ret = lcd_vmalloc_walk(lcd, va2hva(lcd->module->module_core), 
			lcd->module->core_size, lcd_module_map_page);
	if (ret) {
		LCD_ERR("mapping module's core");
		return ret;
	}

	return 0;
}

/**
 * Maps the arch-dependent chunks of memory into the guest virtual
 * address space of the lcd. Assumes the memory has already been mapped
 * in the guest physical address space, starting at LCD_ARCH_BOTTOM.
 */
static int lcd_setup_arch_address_space(struct lcd *lcd)
{
	int ret = 0;
	unsigned int npages;
	/*
	 * Map arch-dependent chunks of memory
	 */
	npages = (LCD_ARCH_TOP - LCD_ARCH_BOTTOM) >> PAGE_SHIFT;
	if (npages > 0) {
		ret = lcd_mm_gva_map_range(lcd, 
					__gva(LCD_ARCH_BOTTOM),
					__gpa(LCD_ARCH_BOTTOM),
					npages);
	}
	return ret;
}

/**
 * Initializes the guest virtual address space, maps module and arch-dep
 * parts of memory into guest virtual address space.
 *
 * On failure, parts of the address space may be set up. The caller is
 * responsible for tearing it down.
 */
static int lcd_setup_address_space(struct lcd *lcd)
{
	int ret;
	/*
	 * Initialize guest virtual address space (root page dir)
	 */
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("setting up guest virtual addr space");
		goto fail1;
	}
	/*
	 * Map arch-dependent chunks of memory
	 */
	ret = lcd_setup_arch_address_space(lcd);
	if (ret) {
		LCD_ERR("mapping arch-dependent memory");
		goto fail2;
	}
	/*
	 * Map the module into the lcd
	 */
	ret = lcd_setup_module_address_space(lcd);
	if (ret) {
		LCD_ERR("mapping module");
		goto fail3;
	}

	return 0;

/* Caller responsible for tear down */
fail3:		
fail2:
fail1:
	return ret;
}

static struct lcd * __lcd_create(void)
{
	struct lcd *lcd;
	/*
	 * Alloc lcd data structure
	 *
	 * zero alloc sets guest paging counter to zero, etc.
	 */
	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		LCD_ERR("error alloc'ing lcd");
		goto fail1;
	}
	/*
	 * Alloc arch-dependent part
	 */
	lcd->lcd_arch = lcd_arch_create();
	if(!lcd->lcd_arch) {
		LCD_ERR("error creating lcd_arch");
		goto fail2;
	}
	/*
	 * Initialize lcd_threads list and lock
	 */
	INIT_LIST_HEAD(&lcd->lcd_threads.list);
	mutex_init(&lcd->lcd_threads.lock);
	/*
	 * Done
	 */
	return lcd;

fail2:
	kfree(lcd);
fail1:
	return NULL;
}

/**
 * Loads module into host address space, and stores pointer to
 * struct module in lcd.
 */
static int lcd_load_module(struct lcd *lcd, char *module_name)
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
	lcd->module = find_module(module_name);
	mutex_unlock(&module_mutex);	
	if (!lcd->module) {
		LCD_ERR("couldn't find module");
		goto fail2;
	}
	if(!try_module_get(lcd->module)) {
		LCD_ERR("incrementing module ref count");
		goto fail3;
	}
	/*
	 * Set the lcd name
	 */
	strncpy(lcd->name, module_name, MODULE_NAME_LEN);

	return ret;

fail3:
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
	lcd->module = NULL;
fail2:
fail1:
	return ret;
}

void lcd_destroy(struct lcd *lcd)
{
	int ret;
	/*
	 * Assume we have no running lcd_thread's ...
	 *
	 * ORDER IS IMPORTANT:
	 *
	 * (1) unmap the module
	 *
	 * (2) delete the module from the host
	 *
	 * (3) tear down guest virtual paging
	 *
	 * (4) free up any pages still in the guest physical addr space
	 *     XXX: This will attempt to free the underlying host page.
	 *                       Beware!
	 *
	 * (5) tear down the lcd arch
	 *
	 * Why? We need to unmap the module so we don't try to double free
	 * the module memory in (4). Deleting the module just needs to come
	 * somewhere after (1). (3) needs to come before (4) so we don't
	 * double free pages. (5) needs to come after (4) so that the ept
	 * is still around to tear down the guest phys addr space.
	 *
	 * If you do it in the wrong order, you can get NULLs or double
	 * frees (very fun!).
	 *
	 * I do NULL checks so that calls to __lcd_create (doesn't set up
	 * module or guest virtual) can be matched with lcd_destroy.
	 */
	if (lcd->module) {
		/*
		 * Unmap module
		 */
		ret = lcd_vmalloc_walk(lcd, va2hva(lcd->module->module_init),
				lcd->module->init_size, lcd_module_unmap_page);
		if (ret)
			LCD_ERR("unmapping module's init, continuing ...");
		ret = lcd_vmalloc_walk(lcd, va2hva(lcd->module->module_core),
				lcd->module->core_size, lcd_module_unmap_page);
		if (ret)
			LCD_ERR("unmapping module's core, continuing ...");
		/*
		 * Delete module
		 */
		module_put(lcd->module);
		ret = do_sys_delete_module(lcd->module->name, 0, 1);
		if (ret)	
			LCD_ERR("deleting module");
	}
	/*
	 * Tear down guest virtual address space (the checks allow
	 * lcd_create to call us at various points)
	 */
	if (lcd->gv_paging.root)
		lcd_mm_gva_destroy(lcd);
	/*
	 * Free any remaining pages in the guest physical address space
	 */
	lcd_mm_gp_free_all(lcd);
	/*
	 * Tear down lcd_arch (ept, ...)
	 */
	lcd_arch_destroy(lcd->lcd_arch);
	/*
	 * Finish
	 */
	kfree(lcd);
}

static int lcd_setup_initial_thread(struct lcd *lcd);

int lcd_create(char *module_name, struct lcd **out)
{
	int ret;
	struct lcd *lcd;

	/*
	 * Alloc and init basic parts of lcd
	 */
	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to init lcd");
		ret = -EIO;
		goto fail1;
	}
	/*
	 * Load the module
	 */
	ret = lcd_load_module(lcd, module_name);
	if (ret) {
		LCD_ERR("load module");
		goto fail2;
	}
	/*
	 * Set up the arch-independent part of the guest physical address
	 * space, and the entire guest virtual address space.
	 */
	ret = lcd_setup_address_space(lcd);
	if (ret) {
		LCD_ERR("set up address space");
		goto fail3;
	}
	/*
	 * Set up the first lcd_thread
	 */
	ret = lcd_setup_initial_thread(lcd);
	if (ret) {
		LCD_ERR("set up initial thread");
		goto fail4;
	}

	*out = lcd;

	return 0;

fail4:
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;	
}

/* LCD THREAD EXECUTION -------------------------------------------------- */

static int lcd_handle_syscall(struct lcd_thread *t)
{
	int syscall_id;
	
	syscall_id = LCD_ARCH_GET_SYSCALL_NUM(t->lcd_arch_thread);
	LCD_MSG("got syscall %d", syscall_id);

	switch (syscall_id) {
	case LCD_SYSCALL_YIELD:
		return 1;
		break;
	default:
		LCD_ERR("unimplemented syscall %d", syscall_id);
		return -ENOSYS;
	}
}

static int lcd_kthread_run_once(struct lcd_thread *t)
{
	int ret;

	ret = lcd_arch_run(t->lcd_arch_thread);
	if (ret < 0) {
		LCD_ERR("running lcd");
		goto out;
	}

	switch(ret) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		LCD_ERR("page fault");
		ret = -ENOSYS;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		LCD_MSG("got external intr");
		ret = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		LCD_ERR("ept fault");
		ret = -ENOSYS;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		LCD_ERR("cr3 access");
		ret = -ENOSYS;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		LCD_MSG("syscall");
		ret = lcd_handle_syscall(t);
		goto out;
	}

out:
	return ret;
}

static int lcd_kthread_main(void *data) /* data is NULL */
{
	int ret;
	struct lcd_thread *current_lcd_thread;

	current_lcd_thread = current->lcd_thread;
	/*
	 * Set my status as running
	 */
	current_lcd_thread->status = LCD_THREAD_RUNNING;
	/*
	 * Enter run loop, check after each iteration if we should stop
	 *
	 * XXX: We're not giving the thread a chance to gracefully exit
	 * for now (e.g., we could use a special upcall message to signal that 
	 * it should terminate).
	 */
	for (;;) {
		ret = lcd_kthread_run_once(current_lcd_thread);
		if (ret || kthread_should_stop())
			return ret; /* to microkernel via kthread_stop */
	}
	
	/* unreachable */
}

int lcd_thread_start(struct lcd_thread *t)
{
	/*
	 * Set lcd's status to runnable, and wake up the kthread (will
	 * start running lcd_kthread_main).
	 */
	t->status = LCD_THREAD_RUNNABLE;
	wake_up_process(t->kthread);

	return 0;
}

int lcd_thread_kill(struct lcd_thread *t)
{
	int ret;
	/*
	 * Stop the kernel thread and get return value.
	 */
	ret = kthread_stop(t->kthread);
	/*
	 * Decrement the kthread's reference count. Host kernel will clean
	 * up the rest.
	 */
	put_task_struct(t->kthread);
	/*
	 * The LCD still owns the lcd thread's stack, so we won't free it.
	 *
	 * Tear down the lcd arch thread
	 */
	lcd_arch_destroy_thread(t->lcd_arch_thread);
	/*
	 * Remove from LCD's list of threads
	 */
	if (mutex_lock_interruptible(&t->lcd->lcd_threads.lock)) {
		LCD_ERR("interrupted, skipping list removal ...");
		goto finally;
	}
	list_del(&t->lcd_threads);
	mutex_unlock(&t->lcd->lcd_threads.lock);

finally:
	/*
	 * Free t
	 */
	kfree(t);

	return ret;
}

/* LCD THREAD CREATE / DESTROY -------------------------------------------- */

/**
 * Creates a new thread to run in lcd. Thread will begin executing at
 * guest virtual address pc. It will use the stack (except for the initial
 * thread, the LCD is responsible for allocating the stack page) whose
 * top is at stack_gva/stack_gpa. 
 *
 * We require both stack_gva/stack_gpa so that the microkernel
 * doesn't rely on the guest virtual paging hierarchy inside the LCD, to
 * resolve guest virtual to guest physical. It needs the guest physical so
 * it can resolve to a host virtual address for the thread's utcb.
 *
 * For now, we assume that the LCD doesn't change the guest physical address
 * of the root guest virtual paging directory.
 *
 * The underlying kernel thread won't start until caller does
 * lcd_thread_start.
 */
static int lcd_add_thread(struct lcd *lcd, gva_t pc, gva_t stack_gva,
			gpa_t stack_gpa, struct lcd_thread **out)
{
	struct lcd_thread* t;
	hpa_t stack_hpa;
	hva_t stack_hva;
	int ret;
	/*
	 * Resolve stack's guest physical address to host virtual
	 */
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, stack_gpa, &stack_hpa);
	if (ret) {
		LCD_ERR("looking up stack gpa %lx", gpa_val(stack_gpa));
		goto fail0;
	}
	stack_hva = hpa2hva(stack_hpa);
	/*
	 * Alloc lcd_thread
	 */
	t = kzalloc(sizeof(*t), GFP_KERNEL); /* status unformed */
	if (!t) {
		LCD_ERR("failed to alloc lcd_thread");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Add t to lcd's list
	 */
	t->lcd = lcd;
	INIT_LIST_HEAD(&t->lcd_threads);
	if (mutex_lock_interruptible(&lcd->lcd_threads.lock)) {
		LCD_ERR("interrupted, cleaning up...");
		goto fail2;
	}

	list_add(&t->lcd_threads, &lcd->lcd_threads.list);

	mutex_unlock(&lcd->lcd_threads.lock);
	/*
	 * Set up utcb (at bottom of stack -- masking off lower bits)
	 */
	t->utcb = hva2va(stack_hva);
	t->utcb = (void *)(((u64)t->utcb) & PAGE_MASK);
	/*
	 * Alloc corresponding lcd_arch_thread
	 */
	t->lcd_arch_thread = lcd_arch_add_thread(lcd->lcd_arch);
	if (!t->lcd_arch_thread) {
		LCD_ERR("set up lcd_arch_thread");
		goto fail3;
	}
	/*
	 * Set up thread's environment
	 */
	ret = lcd_arch_set_pc(t->lcd_arch_thread, pc);
	if (ret) {
		LCD_ERR("setting program counter");
		goto fail4;
	}
	ret = lcd_arch_set_sp(t->lcd_arch_thread, stack_gva);
	if (ret) {
		LCD_ERR("setting stack pointer");
		goto fail5;
	}
	ret = lcd_arch_set_gva_root(t->lcd_arch_thread, 
				lcd->gv_paging.root_gpa);
	if (ret) {
		LCD_ERR("setting guest virtual root address");
		goto fail6;
	}
	/*
	 * Make sure lcd_arch_thread has valid state
	 */
	ret = lcd_arch_check(t->lcd_arch_thread);
	if (ret) {
		LCD_ERR("bad lcd_arch_thread state");
		goto fail7;
	}
	/*
	 * Create a kernel thread (won't run till we wake it up)
	 */
	t->kthread = kthread_create(lcd_kthread_main, NULL,
				"lcd:%s/%d", lcd->name, 
				lcd->lcd_threads.count++);
	if (!t->kthread) {
		LCD_ERR("failed to create kthread");
		goto fail8;
	}
	/*
	 * Bumpg reference count on kthread
	 */
	get_task_struct(t->kthread);
	/*
	 * Store back reference
	 */
	t->kthread->lcd_thread = t;

	/*
	 * DONE!
	 */
	*out = t;

	return 0;

fail8:
fail7:
fail6:
fail5:
fail4:
	lcd_arch_destroy_thread(t->lcd_arch_thread);
fail3:
	if (mutex_lock_interruptible(&lcd->lcd_threads.lock)) {
		LCD_ERR("interrupted in fail, skipping list removal...");
		goto fail2;
	}

	list_del(&t->lcd_threads);

	mutex_unlock(&lcd->lcd_threads.lock);
fail2:	
	kfree(t);
fail1:
fail0:
	return ret;
}

static int lcd_setup_initial_thread(struct lcd *lcd)
{
	struct lcd_thread *t = NULL;
	hva_t stack_page;
	gpa_t stack_page_gpa;
	gva_t stack_page_gva;
	gpa_t stack_ptr_gpa;
	gva_t stack_ptr_gva;
	int ret;
	/*
	 * Allocate a page for the initial thread's stack/utcb
	 */
	ret = lcd_mm_gp_gfp(lcd, &stack_page, &stack_page_gpa);
	if (ret) {
		LCD_ERR("alloc stack page");
		goto fail1;
	}
	/*
	 * Stack pointer points to top of stack page
	 */
	stack_ptr_gpa = gpa_add(stack_page_gpa, PAGE_SIZE - 1);
	/*
	 * Map stack in lcd's guest virtual, right after the guest virtual
	 * paging memory (ptr at top).
	 */
	stack_page_gva = __gva(LCD_GV_MEM_START + LCD_GV_MEM_SIZE);
	stack_ptr_gva = gva_add(stack_page_gva, PAGE_SIZE - 1);
	ret = lcd_mm_gva_map(lcd, stack_page_gva, stack_page_gpa);
	if (ret) {
		LCD_ERR("failed to map stack in guest virtual");
		goto fail2;
	}
	/*
	 * Create and add initial thread to lcd.
	 *
	 * Initial pc will point to the module's init. Recall that
	 * gva of the module = hva of the module, for simplicity. The
	 * code is a bit redundant, but it asserts this fact.
	 */
	ret = lcd_add_thread(lcd,
			__gva(hva_val(va2hva(lcd->module->module_init))),
			stack_ptr_gva,
			stack_ptr_gpa,
			&t);
	if (ret) {
		LCD_ERR("failed to make lcd thread");
		goto fail3;
	}
	/*
	 * Set lcd's initial thread, all done.
	 */
	lcd->init_thread = t;
	return 0;

fail3:
	lcd_mm_gva_unmap(lcd, stack_page_gva);
fail2:
	lcd_mm_gp_free_page(lcd, stack_page_gpa);
fail1:
	return ret;
}

/* IOCTL -------------------------------------------------- */

/**
 * Does insmod syscall on behalf of user code call to ioctl.
 */
static int lcd_init_module(void __user *_mi)
{
	int r;
	struct lcd_init_module_args mi;
	
	/*
	 * Grab info
	 */
	r = copy_from_user(&mi, _mi, sizeof(mi));
	if (r) {
		LCD_ERR("copy_from_user");
		return r;
	}

	/*
	 * Init module (for_lcd = 1)
	 */
	return do_sys_init_module(mi.module_image, mi.len, 
				mi.param_values, 1);
}

static long lcd_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case LCD_INIT_MODULE:
		r = lcd_init_module((void __user *)arg);
		goto out;
	default:
		return -ENOTTY;
	}

out:
	return r;
}

static const struct file_operations lcd_chardev_ops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= lcd_dev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= lcd_dev_ioctl,
#endif
	.llseek		= noop_llseek,
};

static struct miscdevice lcd_dev = {
	LCD_MINOR,
	"lcd",
	&lcd_chardev_ops,
};

/* Init / Exit ---------------------------------------- */

static void lcd_tests(void);

static int __init lcd_init(void)
{
	int r;

	printk(KERN_ERR "LCD module loaded\n");

	if ((r = lcd_arch_init())) {
		LCD_ERR("failed to initialize vmx");
		return r;
	}

	r = misc_register(&lcd_dev);
	if (r) {
		LCD_ERR("misc device register failed");
		
	}

	/*
	 * Run tests
	 */
	lcd_tests();

	return r;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	lcd_arch_exit();
}

/* EXPORTS ---------------------------------------- */

module_init(lcd_init);
module_exit(lcd_exit);

EXPORT_SYMBOL(lcd_create);
EXPORT_SYMBOL(lcd_destroy);
EXPORT_SYMBOL(lcd_thread_start);
EXPORT_SYMBOL(lcd_thread_kill);

/* DEBUGGING ---------------------------------------- */

#include "lcd-tests.c"
