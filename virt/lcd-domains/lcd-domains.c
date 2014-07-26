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

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

#define LCD_PAGING_MEM_SIZE (4 * (1 << 20)) /* 4 MBs */

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

/* Guest Virtual -------------------------------------------------- */

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
 * Allocates a host physical page and guest physical
 * page (in the lcd's guest phys address space) for
 * storing a paging structure.
 */
static int lcd_mm_gva_alloc(struct lcd *lcd, gpa_t *ga_out, hpa_t *ha_out)
{
	hva_t hva;
	gpa_t gpa;
	hpa_t hpa;
	int ret;

	if (!lcd->gv.present) {
		printk(KERN_ERR "lcd_mm_gva_alloc: gv paging not present\n");
		ret = -EINVAL;
		goto fail1;
	}

	/*
	 * Check watermark, and bump it.
	 */
	if (gpa_val(lcd->gv.paging_mem_brk) >= 
		gpa_val(lcd->gv.paging_mem_top)) {
		printk(KERN_ERR "lcd_mm_gva_alloc: exhausted paging mem\n");
		ret = -ENOMEM;
		goto fail1;
	}
	gpa = lcd->gv.paging_mem_brk;
	lcd->gv.paging_mem_brk = gpa_add(lcd->gv.paging_mem_brk, PAGE_SIZE);

	/*
	 * Allocate a host physical page
	 */
	hva = __hva(__get_free_page(GFP_KERNEL));
	if (!hva_val(hva)) {
		printk(KERN_ERR "lcd_mm_gva_alloc: no host phys mem\n");
		ret = -ENOMEM;
		goto fail2;
	}
	memset(hva2va(hva), 0, PAGE_SIZE);
	hpa = hva2hpa(hva);

	/*
	 * Map in ept
	 */
	ret = lcd_arch_ept_map_range(lcd->lcd_arch, gpa, hpa, 1);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_alloc: couldn't map gpa %lx to hpa %lx\n",
			gpa_val(gpa),
			hpa_val(hpa));
		goto fail3;
	}

	*ga_out = gpa;
	*ha_out = hpa;

	return 0;

fail3:
	free_page(hva_val(hva));
fail2:
fail1:
	return ret;
}

/**
 * Initializes guest virtual address space info in lcd, and
 * sets gva root pointer (e.g., x86 %cr3).
 *
 * Must be called before mapping any gva's.
 */
static int lcd_mm_gva_init(struct lcd *lcd, gpa_t gv_paging_mem_start,
			gpa_t gv_paging_mem_end)
{
	gpa_t gpa;
	hpa_t hpa;
	int ret;

	/*
	 * Set start / end
	 */
	lcd->gv.paging_mem_bot = gv_paging_mem_start;
	lcd->gv.paging_mem_brk = gv_paging_mem_start;
	lcd->gv.paging_mem_top = gv_paging_mem_end;

	/*
	 * Mark paging as present
	 */
	lcd->gv.present = 1;

	/*
	 * Alloc a page for the pgd
	 */
	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_init: error alloc'ing\n");
		goto fail1;
	}
	
	/*
	 * Store the root pointer
	 */
	lcd->gv.root = (pgd_t *)hpa2va(hpa);
	lcd_arch_set_gva_root(lcd->lcd_arch, gpa);

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
	int i;
	
	/*
	 * Get hpa of page table, using gpa stored in pmd_entry.
	 */
	gpa = pmd_gpa(pmd_entry);
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pt_destroy: error looking up gpa %lx\n",
			gpa_val(gpa));
		return ret;
	}

	pt = (pte_t *)hpa2va(hpa);

	/*
	 * Check for any potential memory leaks
	 */
	for (i = 0; i < PTRS_PER_PTE; i++) {
		if (pte_present(pt[i])) {
			printk(KERN_ERR "lcd_mm_pt_destroy: possible memory leak for gpa %lx (pt idx %d)\n",
				gpa_val(pte_gpa(&pt[i])), i);
			dump_stack();
		}
	}

	/*
	 * Unmap page table
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pt_destroy: error unmapping pt\n");
		return ret;
	}

	/*
	 * Free page table
	 */
	free_page((unsigned long)pt);

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
		printk(KERN_ERR "lcd_mm_pmd_destroy: error looking up gpa %lx\n",
			gpa_val(gpa));
		return ret;
	}

	pmd = (pmd_t *)hpa2va(hpa);

	/*
	 * Free all present page tables
	 */
	for (i = 0; i < PTRS_PER_PMD; i++) {
		if (pmd_present(pmd[i])) {
			ret = lcd_mm_pt_destroy(lcd, &pmd[i]);
			if (ret) {
				printk(KERN_ERR "lcd_mm_pmd_destroy: error destroying child pt\n");
				return ret;
			}
		}
	}

	/*
	 * Unmap pmd
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pmd_destroy: error unmapping pmd\n");
		return ret;
	}

	/*
	 * Free pmd
	 */
	free_page((unsigned long)pmd);

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
		printk(KERN_ERR "lcd_mm_pud_destroy: error looking up gpa %lx\n",
			gpa_val(gpa));
		return ret;
	}

	pud = (pud_t *)hpa2va(hpa);

	/*
	 * Destroy all present pmd's
	 */
	for (i = 0; i < PTRS_PER_PUD; i++) {
		if (pud_present(pud[i])) {
			ret = lcd_mm_pmd_destroy(lcd, &pud[i]);
			if (ret) {
				printk(KERN_ERR "lcd_mm_pud_destroy: error destroying child pmd\n");
				return ret;
			}
		}
	}

	/*
	 * Unmap pud
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);

	/*
	 * Free pud
	 */
	free_page((unsigned long)pud);

	return 0;
}

/**
 * Unmaps guest virtual paging structures in lcd's ept, and
 * frees host physical memory associated with paging structures.
 *
 * Note! Does not free host physical memory mapped (via guest
 * physical addresses) by gva; just the paging structures themselves.
 */
static void lcd_mm_gva_destroy(struct lcd *lcd)
{
	pgd_t *pgd;
	int i;
	int ret;

	pgd = lcd->gv.root;
	
	/*
	 * Free all present pud's
	 */
	for (i = 0; i < PTRS_PER_PGD; i++) {
		if (pgd_present(pgd[i])) {
			ret = lcd_mm_pud_destroy(lcd, &pgd[i]);
			if (ret) {
				printk(KERN_ERR "lcd_mm_gva_destroy: error freeing pud at idx %d\n",
					i);
				return;
			}
		}
	}

	/*
	 * Unmap in ept
	 */
	ret = lcd_arch_ept_unmap_range(lcd->lcd_arch, 
				lcd->gv.paging_mem_bot, 1);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_destroy: error unmapping pgd\n");
		return;
	}

	/*
	 * Free pgd
	 */
	free_page((unsigned long)pgd);
	lcd->gv.root = NULL;

	/*
	 * Mark as invalid
	 */
	lcd->gv.present = 0;
}


/**
 * Get host virtual address of pte
 * for gva and pmd_entry.
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
		printk(KERN_ERR "lcd_mm_gva_lookup_pte: error looking up gpa %lx\n",
			gpa_val(gpa));
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
		printk(KERN_ERR "lcd_mm_gva_walk_pt: error looking up pte\n");
		return ret;
	}

	*pte_out = entry;

	return 0;
}

/**
 * Get host virtual address of pmd entry
 * for gva and pud_entry.
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
		printk(KERN_ERR "lcd_mm_gva_lookup_pmd: error looking up gpa %lx\n",
			gpa_val(gpa));
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
	hpa_t hpa;

	ret = lcd_mm_gva_lookup_pmd(lcd, gva, pud_entry, &entry);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_walk_pmd: error looking up pmd\n");
		return ret;
	}

	if (!pmd_present(*entry)) {
		/*
		 * Alloc and map a page table
		 */
		ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
		if (ret) {
			printk(KERN_ERR "lcd_mm_gva_walk_pmd: error alloc'ing\n");
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
 * Get host virtual address of pud entry
 * for gva and pgd_entry.
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
		printk(KERN_ERR "lcd_mm_gva_lookup_pud: error looking up gpa %lx\n",
			gpa_val(gpa));
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
	hpa_t hpa;

	ret = lcd_mm_gva_lookup_pud(lcd, gva, pgd_entry, &entry);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_walk_pud: error looking up pud\n");
		return ret;
	}

	if (!pud_present(*entry)) {
		/*
		 * Alloc and map a pmd
		 */
		ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
		if (ret) {
			printk(KERN_ERR "lcd_mm_gva_walk_pud: error alloc'ing\n");
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
	hpa_t hpa;

	entry = lcd->gv.root + pgd_index(gva_val(gva));
	if (!pgd_present(*entry)) {
		/*
		 * Alloc and map a pud
		 */
		ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
		if (ret) {
			printk(KERN_ERR "lcd_mm_gva_walk_pgd: error alloc'ing\n");
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
 * way.
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
		printk(KERN_ERR "lcd_mm_gva_walk: err walking pgd\n");
		return ret;
	}

	/*
	 * Get pud entry for pmd
	 */
	ret = lcd_mm_gva_walk_pud(lcd, gva, pgd, &pud);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_walk: err walking pud\n");
		return ret;
	}

	/*
	 * Get pmd entry for page table
	 */
	ret = lcd_mm_gva_walk_pmd(lcd, gva, pud, &pmd);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_walk: err walking pmd\n");
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
		printk(KERN_ERR "lcd_mm_gva_map: error getting pte\n");
		return ret;
	}

	if (pte_present(*pte)) {
		printk(KERN_ERR "lcd_mm_gva_map: remap gva %lx to gpa %lx (was %lx)\n",
			gva_val(gva), gpa_val(gpa),
			gpa_val(lcd_mm_gva_get(pte)));
		return -EINVAL;
	}

	lcd_mm_gva_set(pte, gpa);

	return 0;
}

/**
 * Simple routine combining walk and set. Never
 * overwrites.
 */
static int lcd_mm_gva_unmap(struct lcd *lcd, gva_t gva)
{
	int ret;
	pte_t *pte;

	ret = lcd_mm_gva_walk(lcd, gva, &pte);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_unmap: error getting pte\n");
		return ret;
	}

	if (!pte_present(*pte)) {
		printk(KERN_ERR "lcd_mm_gva_unmap: no mapping for gva %lx\n",
			gva_val(gva));
		return -EINVAL;
	}

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
		printk(KERN_ERR "lcd_mm_gva_to_gpa: error getting pte\n");
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
			printk(KERN_ERR "lcd_mm_gva_map_range: error mapping gva %lx to gpa %lx\n",
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
			printk(KERN_ERR "lcd_mm_gva_unmap_range: error unmapping gva %lx\n",
				gva_val(gva_add(gva_start,off)));
			return -EIO;
		}
	}

	return 0;
}

/* lcd create / destroy ---------------------------------------- */

static int lcd_create(struct lcd **lcd_out)
{
	struct lcd *lcd;
	int r;

	/*
	 * Alloc and init lcd
	 */
	lcd = (struct lcd *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd_create: error alloc'ing lcd\n");
		r = -ENOMEM;
		goto fail1;
	}
	memset(lcd, 0, sizeof(*lcd)); /* sets status to unformed */

	lcd->lcd_arch = lcd_arch_create();
	if(!lcd->lcd_arch) {
		printk(KERN_ERR "lcd_create: error creating lcd_arch\n");
		r = -ENOMEM;
		goto fail2;
	}

	*lcd_out = lcd;

	return 0;

fail2:
	kfree(lcd);
fail1:
	return r;

}

static void lcd_destroy(struct lcd *lcd)
{
	/*
	 * Order is important ...
	 */
	if (lcd->gv.present)
		lcd_mm_gva_destroy(lcd);
	lcd_arch_destroy(lcd->lcd_arch);
	kfree(lcd);
}


/* BLOBs -------------------------------------------------- */

static int lcd_do_run_blob_once(struct lcd *lcd)
{
	int r;
	int syscall_id;

	r = lcd_arch_run(lcd->lcd_arch);
	if (r < 0) {
		printk(KERN_ERR "lcd_do_run_blob_once: error running blob\n");
		goto out;
	}

	switch(r) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		/*
		 * Paging shouldn't be needed for blob (blob shouldn't
		 * use instructions that access memory, for now)
		 */
		printk(KERN_ERR "lcd_run_blob: page fault\n");
		r = -EIO;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		printk(KERN_ERR "lcd_run_blob: got external intr\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		/*
		 * EPT should everything mapped the blob needs, so
		 * there's a problem. Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: ept fault\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		/*
		 * %cr3 shouldn't be accessed for simple blobs (for
		 * now). Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: cr3 access\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		/*
		 * Only allow yield syscalls for now
		 */
		syscall_id = LCD_ARCH_GET_SYSCALL_NUM(lcd->lcd_arch);
		printk(KERN_ERR "lcd_run_blob: handling syscall %d\n",
			syscall_id);
		if (syscall_id != LCD_SYSCALL_YIELD) {
			printk(KERN_ERR "lcd_run_blob: unexpected syscall id %d\n",
				syscall_id);
			r = -EIO;
			goto out;
		} else {
			printk(KERN_ERR "lcd_run_blob: lcd yielded, exiting lcd...\n");
			r = -EIO;
			goto out;
		}
	}

out:
	return r;
}

static int lcd_do_run_blob(struct lcd *lcd)
{
	int r;

	while (1) {
		r = lcd_do_run_blob_once(lcd);
		if (r)
			return r;
	}
}

static int lcd_init_blob(struct lcd *lcd, unsigned char *blob,
			unsigned int blob_order)
{
	int r;
	unsigned long paging_mem_size;
	unsigned long npages;

	/*
	 * (initial)
	 * Blob Memory Layout
	 * ==================
	 *
	 * The layout below reflects the guest physical *and* virtual memory
	 * layout with the exception that not all paging memory is mapped in
	 * in the guest physical address space (for efficiency). 
	 *
	 * Guest physical addresses are mapped one-to-one to the same guest 
	 * virtual addresses.
	 *
	 * All allocated guest physical memory--including the arch-dependent 
	 * region, guest virtual page tables, and the lcd's code--is mapped
	 * in the guest virtual address space.
	 *
	 *                   +---------------------------+
	 *                   |                           |
	 *                   :                           :
	 *                   :      Free / Unmapped      :
	 *                   |                           |
	 *                   +---------------------------+
	 *                   |           Blob            | (max 16 pgs)	 
	 * blob entry------> +---------------------------+ 
	 *                   |       Guest Virtual       | (4 MBs)
	 *                   |       Paging Memory       |
	 * LCD_ARCH_FREE---> +---------------------------+
	 *                   |                           |
	 *                   :   Reserved Arch Memory    :
	 *                   |                           |
	 *                   +---------------------------+ 0x0000 0000 0000 0000
	 */
	
	paging_mem_size = 4 * (1 << 20); /* 4 MBs */

	/*
	 * Initialize guest virtual paging
	 */
	r = lcd_mm_gva_init(lcd, LCD_ARCH_FREE, 
			gpa_add(LCD_ARCH_FREE, paging_mem_size));
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting up gva\n");
		goto fail1;
	}

	/*
	 * Map blob in guest physical, after paging mem
	 */
	r = lcd_arch_ept_map_range(lcd->lcd_arch, 
				gpa_add(LCD_ARCH_FREE, paging_mem_size), 
				va2hpa(blob),
				(1 << blob_order));
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error mapping blob in gpa\n");
		goto fail2;
	}

	/*
	 * Map gpa from 0 to top of blob in lcd's gva
	 */
	npages = (gpa_val(LCD_ARCH_FREE) + paging_mem_size) >> PAGE_SHIFT;
	npages += (1 << blob_order);
	r = lcd_mm_gva_map_range(lcd, 
				/* gva start */
				__gva(0), 
				/* gpa start */
				__gpa(0), 
				/* num pages */
				npages);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting up initial gva\n");
		goto fail3;
	}

	/*
	 * Initialize program counter to blob entry point (just after
	 * guest virtual paging mem).
	 */
	r = lcd_arch_set_pc(lcd->lcd_arch, 
			__gva(gpa_val(gpa_add(LCD_ARCH_FREE, 
							paging_mem_size))));
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting prgm counter\n");
		goto fail4;
	}

	r = lcd_do_run_blob(lcd);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error running blob: err %d\n",
			r);
		goto fail4;
	}

	r = 0;
	goto done;
done:
fail4:
	lcd_mm_gva_unmap_range(lcd, __gva(0), npages);
fail3:
	lcd_arch_ept_unmap_range(lcd->lcd_arch, 
				gpa_add(LCD_ARCH_FREE, paging_mem_size), 
				(1 << blob_order));
fail2:
	lcd_mm_gva_destroy(lcd);
fail1:
	return r;
}

static int lcd_run_blob(struct lcd_blob_info *bi)
{
	struct lcd *lcd;
	int r;
	unsigned char *blob;

	/*
	 * Sanity check blob order
	 */
	if (bi->blob_order > 4) {
		printk(KERN_ERR "lcd_run_blob: blob is bigger than 16 pgs\n");
		r = -EINVAL;
		goto fail1;
	}
	
	/*
	 * Load blob mem
	 */
	blob = (unsigned char *)__get_free_pages(GFP_KERNEL, bi->blob_order);
	if (!blob) {
		printk(KERN_ERR "lcd_run_blob: couldn't alloc mem for blob\n");
		r = -ENOMEM;
		goto fail2;
	}

	/*
	 * Copy blob
	 */
	r = copy_from_user(blob, (void __user *)bi->blob, 
			(1 << bi->blob_order) * PAGE_SIZE);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error copying blob\n");
		goto fail3;
	}

	/*
	 * Alloc and init lcd
	 */
	r = lcd_create(&lcd);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error creating lcd\n");
		goto fail4;
	}

	/*
	 * Initialize lcd for blob
	 */
	r = lcd_init_blob(lcd, blob, bi->blob_order);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error loading blob in lcd\n");
		r = -EIO;
		goto fail5;
	}


	r = 0;
	goto done;
done:
fail5:
	lcd_destroy(lcd);
fail4:
fail3:
	free_pages((unsigned long)blob, bi->blob_order);
fail2:
fail1:
	return r;
}

/* module loading ---------------------------------------- */

struct lcd_module_load {
	struct module *module;
	struct completion done;
};

/**
 * Walks physical pages in module chunk (init or core), and fires callback
 * with the values that should be used to either map or unmap the pages.
 * Because module memory is allocated with vmalloc, there's 
 * no guarantee the physical pages are contiguous, hence why we need to
 * do this.
 *
 * Code assumes chunk is page aligned, and the entire chunk is a group of
 * pages.
 *
 * (Jithu, I owe you one for implementing this in the original code.)
 */
static int lcd_module_mem_walk(struct lcd *lcd, hva_t chunk, unsigned long size,
			int (*cb)(struct lcd *, hpa_t, gpa_t, gva_t))
{
	int r;
	hpa_t hpa;
	gpa_t gpa;
	gva_t gva;
	unsigned long mapped;

	mapped = 0;
	while (mapped < size) {
		/*
		 * Convert hva to correct hpa
		 */
		hpa = __hpa(vmalloc_to_pfn(hva2va(chunk)) << PAGE_SHIFT);
		/*
		 * Map gpa = hpa ---> hpa, and gva = hva ---> gpa
		 *
		 * (So, overall, we will have hva ---> hpa, inside the lcd.)
		 *
		 * XXX: Guest physical address = host physical address, for
		 * simplicity. This shouldn't overlap with arch-dependent guest
		 * physical mem, but be aware...
		 */
		gpa = __gpa(hpa_val(hpa));
		gva = __gva(hva_val(chunk));
		r = cb(lcd, hpa, gpa, gva);
		if (r)
			return r;

		mapped += PAGE_SIZE;
		chunk = hva_add(chunk, PAGE_SIZE);
	}
	return 0;
}

/**
 * Check module code alignment before we do anything.
 *
 * If the base or top is not page aligned, the lcd could peek into
 * the host kernel.
 */
static int lcd_module_check_chunk(hva_t base, unsigned long size)
{
	if (hva_val(base) & ~(PAGE_MASK))
		return -EINVAL;
	if ((hva_val(base) + size) & ~(PAGE_MASK))
		return -EINVAL;
	return 0;
}	

static int lcd_module_map_page(struct lcd *lcd, hpa_t hpa, gpa_t gpa,
			gva_t gva)
{
	int r;

	r = lcd_arch_ept_map(lcd->lcd_arch, gpa, hpa, 1, 0);
	if (r) {
		LCD_ERR("error mapping into ept");
		return r;
	}
	r = lcd_mm_gva_map(lcd, gva, gpa);
	if (r) {
		LCD_ERR("error mapping into gv");
		return r;
	}
	return 0;
}

static int lcd_module_unmap_page(struct lcd *lcd, hpa_t hpa, gpa_t gpa,
				gva_t gva)
{
	int r;

	r = lcd_arch_ept_unmap(lcd->lcd_arch, gpa);
	if (r) {
		LCD_ERR("error unmapping gpa in ept");
		return r;
	}
	r = lcd_mm_gva_unmap(lcd, gva);
	if (r) {
		LCD_ERR("error unmapping gva in gv");
		return r;
	}
	return 0;
}

static void lcd_module_addr_free(struct lcd *lcd, struct module *m)
{
	int r;
	unsigned long npages;

	r = lcd_module_mem_walk(lcd, va2hva(m->module_init), 
				m->init_size, lcd_module_unmap_page);
	if (r) {
		LCD_ERR("unmap module init");
		goto fail;
	}
	r = lcd_module_mem_walk(lcd, va2hva(m->module_core), 
				m->core_size, lcd_module_unmap_page);
	if (r) {
		LCD_ERR("unmap module core");
		goto fail;
	}
	npages = (gpa_val(LCD_ARCH_FREE) + LCD_PAGING_MEM_SIZE) >> PAGE_SHIFT; 
	r = lcd_mm_gva_unmap_range(lcd, __gva(0), npages);
	if (r) {
		LCD_ERR("unmap arch-dep and paging mem\n");
		goto fail;
	}		
	lcd_mm_gva_destroy(lcd);

	return;	
fail:
	return;
}

/**
 * Initializes the lcd's guest virtual address space, and maps
 * the kernel module m into the lcd's guest physical / virtual
 * address space.
 */
static int lcd_module_load(struct lcd *lcd, struct module *m)
{
	int r = -EINVAL;
	unsigned long npages;
	/*
	 * Module Memory Layout
	 * ====================
	 *
	 * The layout below reflects the guest physical *and* virtual memory
	 * layout with the exception that not all paging memory is mapped in
	 * in the guest physical address space (for efficiency). 
	 *
	 * Guest physical addresses are mapped one-to-one to the same guest 
	 * virtual addresses.
	 *
	 * All allocated guest physical memory--including the arch-dependent 
	 * region, guest virtual page tables, and the lcd's code--is mapped
	 * in the guest virtual address space.
	 *
	 * The module is mapped to the same guest physical / guest virtual
	 * address space as the host, to avoid relocating symbols.
	 *
	 *                  +---------------------------+
	 *   module mapped  |                           |
	 *   somewhere in   :                           :
	 *    here ------>  :                           :
	 *                  |                           |
	 *                  +---------------------------+ 
	 *                  |       Guest Virtual       | (4 MBs)
	 *                  |       Paging Memory       |
	 * LCD_ARCH_FREE--> +---------------------------+
	 *                  |                           |
	 *                  :   Reserved Arch Memory    :
	 *                  |                           |
	 *                  +---------------------------+ 0x0000 0000 0000 0000
	 */

	/*
	 * Check module memory alignment
	 */
	r = lcd_module_check_chunk(va2hva(m->module_init), m->init_size);
	if (r)
		LCD_WARN("mod init code not page aligned; parts of host kernel will be visible inside lcd");

	r = lcd_module_check_chunk(va2hva(m->module_core), m->core_size);
	if (r)
		LCD_WARN("mod core code not page aligned; parts of host kernel will be visible inside lcd");

	/*
	 * Initialize guest virtual paging
	 */
	r = lcd_mm_gva_init(lcd, LCD_ARCH_FREE, 
			gpa_add(LCD_ARCH_FREE, LCD_PAGING_MEM_SIZE));
	if (r) {
		LCD_ERR("setting up gva");
		goto fail;
	}

	/*
	 * Map arch-dependent mem and paging mem in guest virtual
	 */
	npages = (gpa_val(LCD_ARCH_FREE) + LCD_PAGING_MEM_SIZE) >> PAGE_SHIFT;
	r = lcd_mm_gva_map_range(lcd, __gva(0), __gpa(0), npages);
	if (r) {
		LCD_ERR("mapping arch-dep code and paging mem");
		goto fail;
	}

	/*
	 * Map module init and core
	 */
	r = lcd_module_mem_walk(lcd, va2hva(m->module_init), m->init_size,
				lcd_module_map_page);
	if (r) {
		LCD_ERR("error mapping init code");
		goto fail;
	}
	r = lcd_module_mem_walk(lcd, va2hva(m->module_core), m->core_size,
				lcd_module_map_page);
	if (r) {
		LCD_ERR("error mapping core code");
		goto fail;
	}
	
	/*
	 * Initialize program counter to module init
	 */
	r = lcd_arch_set_pc(lcd->lcd_arch, 
			__gva(hva_val(va2hva(m->module_init))));
	if (r) {
		LCD_ERR("setting pgm counter");
		goto fail;
	}

	return 0;

fail:
	return r;
}

/**
 * Returns when the hypervisor says the kthread should stop.
 */
static void lcd_module_kthread_stop(void)
{
	set_current_state(TASK_INTERRUPTIBLE);
	while(!kthread_should_stop()) {
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	set_current_state(TASK_RUNNING);
}

/**
 * Tears down lcd, and waits until hypervisor tells it to stop (if
 * run_ret_val is non-zero). The entire lcd is not destroyed until
 * the hypervisor tells it to stop, so that it can check the lcd's
 * status.
 */
static int lcd_module_kthread_die(struct lcd *lcd, 
				struct lcd_module_load *info, int run_ret_val)
{
	/*
	 * XXX: For now, we're not calling the module's exit routine ...
	 */

	/*
	 * Unmap module (hypervisor / user is responsible for 
	 * deleting it)
	 */
	lcd_module_addr_free(lcd, info->module);
	/*
	 * Set status = LCD_STATUS_DEAD
	 */
	lcd->status = LCD_STATUS_DEAD;
	/*
	 * If run_ret_val is non-zero, wait until we should stop
	 */
	if (run_ret_val)
		lcd_module_kthread_stop();
	/*
	 * Destroy lcd
	 */
	lcd_destroy(lcd);
	kfree(info);
	/*
	 * Pass back the return value (to hypervisor)
	 */
	return run_ret_val;
}

static int lcd_module_run_once(struct lcd *lcd)
{
	int r;
	int syscall_id;

	r = lcd_arch_run(lcd->lcd_arch);
	if (r < 0) {
		LCD_ERR("running lcd");
		goto out;
	}

	switch(r) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		LCD_ERR("page fault");
		r = -EIO;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		LCD_MSG("got external intr");
		r = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		LCD_ERR("ept fault");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		LCD_ERR("cr3 access");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		/*
		 * Only allow yield syscalls for now
		 */
		syscall_id = LCD_ARCH_GET_SYSCALL_NUM(lcd->lcd_arch);
		LCD_MSG("handling syscall %d", syscall_id);
		if (syscall_id != LCD_SYSCALL_YIELD) {
			LCD_ERR("unexpected syscall id %d", syscall_id);
			r = -EIO;
			goto out;
		} else {
			LCD_MSG("lcd yielded, exiting lcd...");
			r = 1;
			goto out;
		}
	}
out:
	return r;
}

/**
 * Suspends kthread until lcd is marked as running.
 */
static void lcd_module_kthread_wait(struct lcd *lcd)
{
	lcd->status = LCD_STATUS_SUSPENDED;

	set_current_state(TASK_INTERRUPTIBLE);
	while(lcd->status == LCD_STATUS_SUSPENDED) {
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	set_current_state(TASK_RUNNING);
}

/**
 * The `main' function for the kthread that loads and runs a kernel
 * module in an lcd.
 */
static int lcd_module_kthread(void *_info)
{
	struct lcd_module_load *info;
	struct lcd *lcd;
	int r;

	info = (struct lcd_module_load *)_info;

	/*
	 * Alloc and init lcd
	 */
	r = lcd_create(&lcd);
	if (r) {
		LCD_ERR("creating lcd");
		goto fail1;
	}

	/*
	 * Load module in lcd
	 */
	r = lcd_module_load(lcd, info->module);
	if (r) {
		LCD_ERR("module load");
		goto fail2;
	}

	/*
	 * === past this line, all code should use lcd_module_kthread_die ===
	 */

	/*
	 * Copy name and pointer to lcd
	 */
	strncpy(lcd->name, info->module->name, MODULE_NAME_LEN);
	current->lcd = lcd;

	/*
	 * Complete, and wait until we aren't suspended
	 */
	complete(&info->done);
	lcd_module_kthread_wait(lcd);
	if (lcd->status != LCD_STATUS_RUNNABLE)
		return lcd_module_kthread_die(lcd, info, -1);

	/*
	 * Make sure lcd has valid state
	 */
	r = lcd_arch_check(lcd->lcd_arch);
	if (r)
		return lcd_module_kthread_die(lcd, info, -1);

	/*
	 * Run loop, check after each iter if we should stop
	 */
	for(;;) {
		r = lcd_module_run_once(lcd);
		if (r || kthread_should_stop()) {
			return lcd_module_kthread_die(lcd, info, r);
		}
	}
	
	/* unreachabe ... */

fail2:
	lcd_destroy(lcd);
fail1:
	/*
	 * Complete and wait until we should stop, then pass back ret val
	 */
	complete(&info->done);
	lcd_module_kthread_stop();
	kfree(info);
	return r;
}

struct task_struct * lcd_create_as_module(char *module_name)
{
	struct task_struct *t;
	struct lcd_module_load *info;
	int ret;

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		LCD_ERR("alloc info");
		goto fail;
	}

	/*
	 * Load module via usermode helper. This call blocks
	 * and returns the exit status of lcd-modprobe.
	 */
	ret = request_lcd_module(module_name);
	if (ret < 0) {
		LCD_ERR("load module");
		kfree(info);
		goto fail;
	}

	/*
	 * Find loaded module, and inc its ref counter; must hold module mutex
	 * while finding module.
	 */
	mutex_lock(&module_mutex);
	info->module = find_module(module_name);
	mutex_unlock(&module_mutex);	
	if (!info->module) {
		LCD_ERR("couldn't find module");
		kfree(info);
		goto fail;
	}
	if(!try_module_get(info->module)) {
		LCD_ERR("incrementing module ref count");
		kfree(info);
		goto fail_module;
	}

	/*
	 * Spawn hosting kernel thread, and give it the info.
	 */
	init_completion(&info->done);
	t = kthread_create(lcd_module_kthread, info, module_name);
	if (!t) {
		LCD_ERR("spawning kthread");
		kfree(info);
		goto fail_module_put;
	}
	
	/*
	 * kthread owns info after this point ...
	 */

	/*
	 * Wake kthread up, and wait until it is ready
	 */
	wake_up_process(t);
	ret = wait_for_completion_interruptible(&info->done);
	if (ret) {
		LCD_ERR("errno %d waiting for kthread", ret);
		goto fail_stop;
	}

	/*
	 * Confirm the kernel thread has an lcd and is waiting
	 *
	 * XXX: Is this vulnerable to memory barrier problems? The
	 * documentation for the complete routine states there is
	 * an implied write memory barrier of some kind.
	 */
	if (t->lcd && t->lcd->status == LCD_STATUS_SUSPENDED) {
		return t;
	} else {
		LCD_ERR("lcd failed to init");
		goto fail_stop;
	}

fail_stop:
	ret = kthread_stop(t);
	LCD_MSG("kthread retval = %d", ret);
fail_module_put:
	module_put(info->module);
fail_module:
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
fail:
	return NULL;	
}

int lcd_run_as_module(struct task_struct *t)
{
	/*
	 * Set lcd's status to runnable, and wake up the kthread
	 */
	t->lcd->status = LCD_STATUS_RUNNABLE;
	wake_up_process(t);

	return 0;
}

void lcd_destroy_as_module(struct task_struct *t, char *module_name)
{
	int ret;
	struct module *m;
	
	/*
	 * If lcd is suspended, kill it. This happens if we want to
	 * kill the lcd before it starts running for the first time.
	 */
	if (t->lcd->status == LCD_STATUS_SUSPENDED) {
		t->lcd->status = LCD_STATUS_KILL;
		wake_up_process(t);
	}

	/*
	 * Tell kthread to stop, and delete the module when it's done.
	 */
	ret = kthread_stop(t);
	LCD_ERR("kthread retval = %d", ret);

	mutex_lock(&module_mutex);
	m = find_module(module_name);
	mutex_unlock(&module_mutex);
	if (!m) {
		LCD_ERR("finding module");
		return;
	}
	module_put(m);
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
}

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

/* ioctl -------------------------------------------------- */

static long lcd_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;
	struct lcd_pv_kernel_config conf;
	struct lcd_blob_info bi;

	switch (ioctl) {
	case LCD_LOAD_PV_KERNEL:
		r = copy_from_user(&conf, (int __user *) arg,
				sizeof(struct lcd_pv_kernel_config));
		if (r) {
			r = -EIO;
			goto out;
		}
		/* create LCD with a PV Linux kernel */
		goto out;
		break;
	case LCD_RUN_BLOB:
		r = copy_from_user(&bi, (void __user *)arg, sizeof(bi));
		if (r) {
			printk(KERN_ERR "lcd: error loading blob info\n");
			r = -EIO;
			goto out;
		}
		r = lcd_run_blob(&bi);
		if (r) {
			printk(KERN_ERR "lcd: error running blob\n");
			goto out;
		}
		r = 0;
		goto out;
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
		printk(KERN_ERR "lcd: failed to initialize vmx\n");
		return r;
	}

	r = misc_register(&lcd_dev);
	if (r) {
		printk(KERN_ERR "lcd: misc device register failed\n");
		
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

EXPORT_SYMBOL(lcd_create_as_module);
EXPORT_SYMBOL(lcd_run_as_module);
EXPORT_SYMBOL(lcd_destroy_as_module);

/* DEBUGGING ---------------------------------------- */

#include "lcd-tests.c"
