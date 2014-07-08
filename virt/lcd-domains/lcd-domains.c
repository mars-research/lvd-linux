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

#include <linux/lcd-domains.h>
#include <asm/lcd-domains-arch.h>
#include <lcd-domains/lcd-domains.h>
#include <lcd-domains/syscall.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* Guest Virtual -------------------------------------------------- */

/**
 * Initializes guest virtual address space info in lcd, and
 * sets gva root pointer (e.g., x86 %cr3).
 *
 * Must be called before mapping any gva's.
 */
static int lcd_mm_gva_init(struct lcd *lcd, u64 gv_paging_mem_start_gpa,
		u64 gv_paging_mem_end_gpa)
{
	u64 root;
	int ret;

	/*
	 * Set start / end (we will use one page for the
	 * global page dir)
	 */
	lcd->gv.paging_mem_bot = gv_paging_mem_start_gpa;
	lcd->gv.paging_mem_brk = gv_paging_mem_start_gpa + PAGE_SIZE;
	lcd->gv.paging_mem_top = gv_paging_mem_end_gpa;
	
	if (lcd->gv.paging_mem_brk > lcd->gv.paging_mem_top) {
		printk(KERN_ERR "lcd_mm_gva_init: not enough room in gpa for paging\n");
		ret = -EINVAL;
		goto fail1;
	}

	/*
	 * Allocate a page for the global page dir.
	 */
	root = __get_free_page(GFP_KERNEL);
	if (!root) {
		printk(KERN_ERR "lcd_mm_gva_init: error allocating global page dir\n");
		ret = -ENOMEM;
		goto fail2;
	}
	memset((void *)root, 0, PAGE_SIZE);
	lcd->gv.root_hva = root;
	
	/*
	 * Map global page dir's page in lcd's ept
	 */
	ret = lcd_arch_ept_map_range(lcd->lcd_arch, 
				gv_paging_mem_start_gpa, root, 1);
	if (ret) {
		printk("lcd_mm_gva_init: error mapping global page dir\n");
		goto fail3;
	}

	/*
	 * Mark root_hva as valid
	 */
	lcd->gv.present = 1;

	return 0;

fail3:
	free_page(root);
fail2:
fail1:
	return ret;
}

static int lcd_mm_pt_destroy(struct lcd *lcd, pmd_t *pmd_entry)
{
	u64 gpa;
	u64 hpa;
	int ret;
	
	/*
	 * Get hpa of page table, using gpa stored in pmd_entry.
	 */
	gpa = pmd_pfn(*pmd_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pt_destroy: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
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
	free_page((u64)__va(hpa));

	return 0;
}

static int lcd_mm_pmd_destroy(struct lcd *lcd, pud_t *pud_entry)
{
	pmd_t *pmd;
	u64 gpa;
	u64 hpa;
	int i;
	int ret;

	/*
	 * Get hpa of pmd, using gpa stored in pud_entry.
	 */
	gpa = pud_pfn(*pud_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pmd_destroy: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
	}

	pmd = (pmd_t *)__va(hpa);

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
	free_page((u64)pmd);

	return 0;
}

static int lcd_mm_pud_destroy(struct lcd *lcd, pgd_t *pgd_entry)
{
	pud_t *pud;
	u64 gpa;
	u64 hpa;
	int i;
	int ret;

	/*
	 * Get hpa of pud, using gpa stored in pgd_entry.
	 */
	gpa = pgd_pfn(*pgd_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_pud_destroy: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
	}

	pud = (pud_t *)__va(hpa);

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
	free_page((u64)pud);

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

	pgd = (pgd_t *)lcd->gv.root_hva;
	
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
	free_page((u64)pgd);

	/*
	 * Mark as invalid
	 */
	lcd->gv.present = 0;
}

/**
 * Allocates a host physical page and guest physical
 * page (in the lcd's guest phys address space) for
 * storing a paging structure.
 */
static int lcd_mm_gva_alloc(struct lcd *lcd, u64 *gpa, u64 *hpa)
{
	u64 hva;
	int ret;

	/*
	 * Check watermark, and bump it.
	 */
	if (lcd->gv.paging_mem_brk >= lcd->gv.paging_mem_top) {
		printk(KERN_ERR "lcd_mm_gva_alloc: exhausted paging mem\n");
		ret = -ENOMEM;
		goto fail1;
	}
	*gpa = lcd->gv.paging_mem_brk;
	lcd->gv.paging_mem_brk += PAGE_SIZE;

	/*
	 * Allocate a host physical page
	 */
	hva = __get_free_page(GFP_KERNEL);
	if (!hva) {
		printk(KERN_ERR "lcd_mm_gva_alloc: no host phys mem\n");
		ret = -ENOMEM;
		goto fail2;
	}
	memset((void *)hva, 0, PAGE_SIZE);
	*hpa = __pa(hva);

	/*
	 * Map in ept
	 */
	ret = lcd_arch_ept_map_range(lcd->lcd_arch, *gpa, *hpa, 1);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_alloc: couldn't map gpa %lx to hpa %lx\n",
			(unsigned long)*gpa,
			(unsigned long)*hpa);
		goto fail3;
	}

	return 0;

fail3:
	free_page((u64)hva);
fail2:
	lcd->gv.paging_mem_brk -= PAGE_SIZE;
fail1:
	return ret;
}

/**
 * Get host virtual address of pte
 * for gva and pmd_entry.
 */
static int lcd_mm_gva_lookup_pte(struct lcd *lcd, u64 gva, pmd_t *pmd_entry,
				pte_t **pte_out)
{
	int ret;
	u64 gpa;
	u64 hpa;
	pte_t *entry;

	/*
	 * Get hpa of page table, using gpa stored in pmd_entry.
	 */
	gpa = pmd_pfn(*pmd_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_lookup_pte: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
	}
	/*
	 * Look up entry in page table
	 */
	entry = ((pte_t *)__va(hpa)) + pte_index(gva);
	
	*pte_out = entry;
	return 0;
}

/**
 * Look up pte for the page frame containing gva,
 * using the page table referenced by pmd_entry.
 */
static int lcd_mm_gva_walk_pt(struct lcd *lcd, u64 gva, pmd_t *pmd_entry,
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
static int lcd_mm_gva_lookup_pmd(struct lcd *lcd, u64 gva, pud_t *pud_entry,
				pmd_t **pmd_out)
{
	int ret;
	u64 gpa;
	u64 hpa;
	pmd_t *entry;

	/*
	 * Get hpa of pmd, using gpa stored in pud_entry.
	 */
	gpa = pud_pfn(*pud_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_lookup_pmd: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
	}
	/*
	 * Look up entry in pmd
	 */
	entry = ((pmd_t *)__va(hpa)) + pmd_index(gva);
	
	*pmd_out = entry;
	return 0;
}

/**
 * Look up pmd entry for the page table for gva,
 * using the pmd referenced by pud_entry.
 */
static int lcd_mm_gva_walk_pmd(struct lcd *lcd, u64 gva, pud_t *pud_entry,
				pmd_t **pmd_out)
{
	int ret;
	pmd_t *entry;
	u64 gpa;
	u64 hpa;

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
		set_pmd(entry, __pmd(gpa | _KERNPG_TABLE));

	}

	*pmd_out = entry;

	return 0;
}

/**
 * Get host virtual address of pud entry
 * for gva and pgd_entry.
 */
static int lcd_mm_gva_lookup_pud(struct lcd *lcd, u64 gva, pgd_t *pgd_entry,
				pud_t **pud_out)
{
	int ret;
	u64 gpa;
	u64 hpa;
	pud_t *entry;

	/*
	 * Get hpa of pud, using gpa stored in pgd_entry.
	 */
	gpa = pgd_pfn(*pgd_entry) << PAGE_SHIFT;
	ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd_mm_gva_lookup_pud: error looking up gpa %lx\n",
			(unsigned long)gpa);
		return ret;
	}
	/*
	 * Look up entry in pud
	 */
	entry = ((pud_t *)__va(hpa)) + pud_index(gva);
	
	*pud_out = entry;
	return 0;
}

/**
 * Look up pud entry for the pmd for gva, using
 * the pud referenced by pgd_entry.
 */
static int lcd_mm_gva_walk_pud(struct lcd *lcd, u64 gva, pgd_t *pgd_entry,
				pud_t **pud_out)
{
	int ret;
	pud_t *entry;
	u64 gpa;
	u64 hpa;

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
		set_pud(entry, __pud(gpa | _KERNPG_TABLE));

	}

	*pud_out = entry;

	return 0;
}

/**
 * Look up pgd entry for the pud for gva.
 */
static int lcd_mm_gva_walk_pgd(struct lcd *lcd, u64 gva, pgd_t **pgd_out)
{
	int ret;
	pgd_t *entry;
	u64 gpa;
	u64 hpa;

	entry = (pgd_t *)(lcd->gv.root_hva + pgd_index(gva));
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
		set_pgd(entry, __pgd(gpa | _KERNPG_TABLE));
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
static int lcd_mm_gva_walk(struct lcd *lcd, u64 gva, pte_t **pte_out)
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

static void lcd_mm_gva_set(pte_t *pte, u64 gpa)
{
	set_pte(pte, __pte(gpa | _KERNPG_TABLE));
}

static u64 lcd_mm_gva_get(pte_t *pte)
{
	return (u64)(pte_pfn(*pte) << PAGE_SHIFT);
}

/**
 * Simple routine combining walk and set. Never
 * overwrites.
 */
static int lcd_mm_gva_map(struct lcd *lcd, u64 gva, u64 gpa)
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
			(unsigned long)gva, (unsigned long)gpa,
			(unsigned long)pte_val(*pte));
		return -EINVAL;
	}

	lcd_mm_gva_set(pte, gpa);

	return 0;
}

/**
 * Simple routine combining walk and get. Never
 * overwrites.
 */
static int lcd_mm_gva_to_gpa(struct lcd *lcd, u64 gva, u64 *gpa)
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
static int lcd_mm_gva_map_range(struct lcd *lcd, u64 gva_start, u64 gpa_start, 
				u64 npages)
{
	u64 off;
	u64 len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_mm_gva_map(lcd,
					/* gva */
					gva_start + off,
					/* gpa */
					gpa_start + off)) {
			printk(KERN_ERR "lcd_mm_gva_map_range: error mapping gva %lx to gpa %lx\n",
				(unsigned long)(gva_start + off),
				(unsigned long)(gpa_start + off));
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
	memset(lcd, 0, sizeof(*lcd));

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
		r = 0;
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
			r = 0;
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
	u64 paging_mem_size;
	u64 npages;

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
			LCD_ARCH_FREE + paging_mem_size);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting up gva\n");
		goto fail1;
	}

	/*
	 * Map blob in guest physical, after paging mem
	 */
	r = lcd_arch_ept_map_range(lcd->lcd_arch, LCD_ARCH_FREE + paging_mem_size, 
			__pa((u64)blob), (1 << blob_order));
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error mapping blob in gpa\n");
		goto fail2;
	}

	/*
	 * Map gpa from 0 to top of blob in lcd's gva
	 */
	npages = (LCD_ARCH_FREE + paging_mem_size) >> PAGE_SHIFT;
	npages += (1 << blob_order);
	r = lcd_mm_gva_map_range(lcd, 
			/* gva start */
			0, 
			/* gpa start */
			0, 
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
	r = lcd_arch_set_pc(lcd->lcd_arch, LCD_ARCH_FREE + paging_mem_size);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting prgm counter\n");
		goto fail4;
	}

	return 0;

fail4:
	/* gva destroy will handle clean up after this failure */
fail3:
	lcd_arch_ept_unmap_range(lcd->lcd_arch, 
				LCD_ARCH_FREE + paging_mem_size, 
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

	/*
	 * Run blob until it yields or fails
	 */
	r = lcd_do_run_blob(lcd);
	goto done;

done:
fail5:
	lcd_destroy(lcd);
fail4:
fail3:
	free_pages((u64)blob, bi->blob_order);
fail2:
fail1:
	return r;
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

/* DEBUGGING ---------------------------------------- */

#include "lcd-tests.c"
