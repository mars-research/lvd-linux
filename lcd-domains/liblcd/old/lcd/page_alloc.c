
#include <lcd-domains/liblcd-config.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <lcd-domains/liblcd.h>
#include <lcd-domains/types.h>

#include <lcd-domains/liblcd-hacks.h>

/* DATA ------------------------------------------------------------ */

/* Initialized in mem_init */
#define LCD_GV_PAGING_MEM_BASE LCD_GV_PAGING_MEM_GPA
#define LCD_GV_PAGING_MEM_BMAP_SIZE (LCD_GV_PAGING_MEM_SIZE >> PAGE_SHIFT)

#define LCD_FREE_MEM_BASE LCD_MODULE_GPA
#define LCD_FREE_MEM_SIZE (16 << 20)
#define LCD_FREE_MEM_BMAP_SIZE (LCD_FREE_MEM_SIZE >> PAGE_SHIFT)

/* Controls the biggest chunk of pages we can alloc. This is ultimately
 * configured in liblcd-config.h under CONFIG_FORCE_MAX_ZONEORDER.
 * NOTE: This needs to be small enough so that the stack frame for
 * allocating pages isn't too big. */
#define LCD_FREE_MEM_MAX_ORDER MAX_ORDER

/* Bit-map to track free space in ioremap'ble address space */
#define LCD_IOREMAP_BMAP_SIZE (LCD_IOREMAP_GPA_SIZE >> PAGE_SHIFT)
static DECLARE_BITMAP(ioremap_gpa_bmap, LCD_IOREMAP_BMAP_SIZE);
static cptr_t ioremap_gpa_phys2cptr[LCD_FREE_MEM_BMAP_SIZE];

static DECLARE_BITMAP(paging_mem_bmap, LCD_GV_PAGING_MEM_BMAP_SIZE);
static cptr_t paging_mem_page2cptr[LCD_GV_PAGING_MEM_BMAP_SIZE];

static DECLARE_BITMAP(free_mem_bmap, LCD_FREE_MEM_BMAP_SIZE);
static cptr_t free_mem_page2cptr[LCD_FREE_MEM_BMAP_SIZE];
static pgd_t *root;
static struct page page_structs[LCD_FREE_MEM_BMAP_SIZE];

static cptr_t boot_cptrs[1 << LCD_BOOT_PAGES_ORDER];
static cptr_t stack_cptrs[1 << LCD_STACK_PAGES_ORDER];


/* From boot info page */
unsigned num_boot_mem_pi;
unsigned num_stack_mem_pi;
unsigned num_paging_mem_pi;
unsigned num_free_mem_pi;
struct lcd_boot_info_for_page *boot_mem_pi_start;
struct lcd_boot_info_for_page *stack_mem_pi_start;
struct lcd_boot_info_for_page *paging_mem_pi_start;
struct lcd_boot_info_for_page *free_mem_pi_start;

/* LOW LEVEL MICROKERNEL INTERFACE ------------------------------ */

int lcd_page_alloc(cptr_t *slot_out)
{
	cptr_t page;
	int ret;
	/*
	 * Alloc a cptr
	 */
	ret = lcd_alloc_cptr(&page);
	if (ret) {
		lcd_printk("lcd_page_alloc: no cptrs left");
		goto fail1;
	}
	/*
	 * Do sys call
	 */
	lcd_set_cr0(page);
	ret = LCD_DO_SYSCALL(LCD_SYSCALL_PAGE_ALLOC);
	if (ret) {
		lcd_printk("lcd_page_alloc: syscall error");
		goto fail2;
	}

	*slot_out = page;

	return 0;

fail2:
	lcd_free_cptr(page);
fail1:
	return ret;
}

int lcd_page_map(cptr_t page, gpa_t gpa)
{
	/*
	 * Call to microkernel
	 */
	lcd_set_r0(gpa_val(gpa));
	lcd_set_cr0(page);
	return LCD_DO_SYSCALL(LCD_SYSCALL_PAGE_MAP);
}

int lcd_page_unmap(cptr_t page, gpa_t gpa)
{
	/*
	 * Call to microkernel
	 */
	lcd_set_r0(gpa_val(gpa));
	lcd_set_cr0(page);
	return LCD_DO_SYSCALL(LCD_SYSCALL_PAGE_UNMAP);
}

/* GUEST PHYSICAL PAGES ---------------------------------------- */

static int gv_gfp(gpa_t *out)
{
	cptr_t page;
	int ret;
	unsigned long idx;
	/*
	 * Find free spot in bitmap
	 */
	idx = find_first_zero_bit(paging_mem_bmap, LCD_GV_PAGING_MEM_BMAP_SIZE);
	if (idx >= LCD_GV_PAGING_MEM_BMAP_SIZE) {
		lcd_printk("gv_gfp: out of mem");
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Set bit to mark page as in use
	 */
	set_bit(idx, paging_mem_bmap);
	/*
	 * Get free page from microkernel
	 */
	ret = lcd_page_alloc(&page);
	if (ret) {
		lcd_printk("gv_gfp: err getting page");
		goto fail1;
	}
	/*
	 * Map page there in guest physical
	 */
	ret = lcd_page_map(page, 
			gpa_add(LCD_GV_PAGING_MEM_BASE, idx << PAGE_SHIFT));
	if (ret) {
		lcd_printk("gv_gfp: err mapping page");
		goto fail3;
	}
	/*
	 * Record correspondence
	 */
	paging_mem_page2cptr[idx] = page;
	/*
	 * Return guest physical address of page
	 */
	*out = gpa_add(LCD_GV_PAGING_MEM_BASE, idx << PAGE_SHIFT);

	return 0;

fail3:
	lcd_cap_delete(page);
fail2:
	clear_bit(idx, paging_mem_bmap);
fail1:
	return ret;
}

/* INLINES ------------------------------------------------------------ */

/* Fortunately, the host macros/inlines get sucked in via headers */

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

/* GUEST VIRTUAL LOOKUP ---------------------------------------- */

static int gv_lookup_pte(gva_t gva, pmd_t *pmd_entry, pte_t **pte_out)
{
	gpa_t gpa;
	gva_t pt_gva;
	pte_t *entry;
	/*
	 * Translate page table gpa -> gva
	 */
	gpa = pmd_gpa(pmd_entry);
	pt_gva = __gva(gpa_val(gpa));
	/*
	 * Look up entry in page table
	 */
	entry = ((pte_t *)gva_val(pt_gva)) + pte_index(gva_val(gva));
	
	*pte_out = entry;
	return 0;
}

static int gv_walk_pt(gva_t gva, pmd_t *pmd_entry, pte_t **pte_out)
{
	int ret;
	pte_t *entry;

	ret = gv_lookup_pte(gva, pmd_entry, &entry);
	if (ret) {
		lcd_printk("looking up pte for gva 0x%lx", gva_val(gva));
		return ret;
	}

	*pte_out = entry;

	return 0;
}

static int gv_lookup_pmd(gva_t gva, pud_t *pud_entry, pmd_t **pmd_out)
{
	gpa_t gpa;
	gva_t pmd_gva;
	pmd_t *entry;
	/*
	 * Translate pmd gpa -> gva
	 */
	gpa = pud_gpa(pud_entry);
	pmd_gva = __gva(gpa_val(gpa));
	if (gva_val(pmd_gva) < gpa_val(LCD_GV_PAGING_MEM_BASE) ||
		gva_val(pmd_gva) >= 
		gpa_val(gpa_add(LCD_GV_PAGING_MEM_BASE, 
					LCD_GV_PAGING_MEM_SIZE))) {
		lcd_printk("gv_lookup_pmd: bad pmd gpa 0x%lx",
			gpa_val(gpa));
		return -EINVAL;
	}
	/*
	 * Look up entry in pmd
	 */
	entry = ((pmd_t *)gva_val(pmd_gva)) + pmd_index(gva_val(gva));
	
	*pmd_out = entry;
	return 0;
}

/**
 * Look up pmd entry for the page table for gva,
 * using the pmd referenced by pud_entry.
 */
static int gv_walk_pmd(gva_t gva, pud_t *pud_entry, pmd_t **pmd_out)
{
	int ret;
	pmd_t *entry;
	gpa_t gpa;

	ret = gv_lookup_pmd(gva, pud_entry, &entry);
	if (ret) {
		lcd_printk("looking up pmd for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pmd_present(*entry)) {
		/*
		 * Alloc and map a page table
		 */
		ret = gv_gfp(&gpa);
		if (ret) {
			lcd_printk("alloc page table");
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

static int gv_lookup_pud(gva_t gva, pgd_t *pgd_entry, pud_t **pud_out)
{
	gpa_t gpa;
	gva_t pud_gva;
	pud_t *entry;
	/*
	 * Translate pud gpa -> gva
	 */
	gpa = pgd_gpa(pgd_entry);
	pud_gva = __gva(gpa_val(gpa));
	if (gva_val(pud_gva) < gpa_val(LCD_GV_PAGING_MEM_BASE) ||
		gva_val(pud_gva) >= 
		gpa_val(gpa_add(LCD_GV_PAGING_MEM_BASE, 
					LCD_GV_PAGING_MEM_SIZE))) {
		lcd_printk("gv_lookup_pud: bad pud gpa 0x%lx",
			gpa_val(gpa));
		return -EINVAL;
	}
	/*
	 * Look up entry in pud
	 */
	entry = ((pud_t *)gva_val(pud_gva)) + pud_index(gva_val(gva));
	
	*pud_out = entry;
	return 0;
}

static int gv_walk_pud(gva_t gva, pgd_t *pgd_entry, pud_t **pud_out)
{
	int ret;
	pud_t *entry;
	gpa_t gpa;

	ret = gv_lookup_pud(gva, pgd_entry, &entry);
	if (ret) {
		lcd_printk("looking up pud for gva %lx", gva_val(gva));
		return ret;
	}

	if (!pud_present(*entry)) {
		/*
		 * Alloc and map a pmd
		 */
		ret = gv_gfp(&gpa);
		if (ret) {
			lcd_printk("alloc pmd");
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

static int gv_walk_pgd(gva_t gva, pgd_t **pgd_out)
{
	int ret;
	pgd_t *entry;
	gpa_t gpa;

	entry = root + pgd_index(gva_val(gva));
	if (!pgd_present(*entry)) {
		/*
		 * Alloc and map a pud
		 */
		ret = gv_gfp(&gpa);
		if (ret) {
			lcd_printk("alloc pud");
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

static int gv_walk(gva_t gva, pte_t **pte_out)
{
	int ret;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	/*
	 * Get pgd entry for pud
	 */
	ret = gv_walk_pgd(gva, &pgd);
	if (ret) {
		lcd_printk("walking pgd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pud entry for pmd
	 */
	ret = gv_walk_pud(gva, pgd, &pud);
	if (ret) {
		lcd_printk("walking pud for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Get pmd entry for page table
	 */
	ret = gv_walk_pmd(gva, pud, &pmd);
	if (ret) {
		lcd_printk("walking pmd for gva %lx", gva_val(gva));
		return ret;
	}

	/*
	 * Finally, get page table entry
	 */
	return gv_walk_pt(gva, pmd, pte_out);
}

/* GUEST VIRTUAL MAP/UNMAP ---------------------------------------- */

static void gv_set(pte_t *pte, gpa_t gpa)
{
	set_pte_gpa(pte, gpa);
}

static gpa_t gv_get(pte_t *pte)
{
	return pte_gpa(pte);
}

static void gv_unset(pte_t *pte)
{
	*pte = __pte(0);
}

static int gv_map(gva_t gva, gpa_t gpa)
{
	int ret;
	pte_t *pte;

	ret = gv_walk(gva, &pte);
	if (ret) {
		lcd_printk("gv_map: err getting pte for gva %lx", gva_val(gva));
		return ret;
	}

	if (pte_present(*pte)) {
		lcd_printk("gv_map: err remap gva %lx to gpa %lx (was %lx)\n",
			gva_val(gva), gpa_val(gpa),
			gpa_val(gv_get(pte)));
		return -EINVAL;
	}

	gv_set(pte, gpa);

	return 0;
}

static int gv_unmap(gva_t gva)
{
	int ret;
	pte_t *pte;

	ret = gv_walk(gva, &pte);
	if (ret) {
		lcd_printk("gv_unmap: err getting pte for gva %lx", 
			gva_val(gva));
		return ret;
	}

	if (!pte_present(*pte)) {
		lcd_printk("gv_unmap: err nothing mapped at gva 0x%lx?",
			gva_val(gva));
		return -EINVAL;
	}

	gv_unset(pte);

	return 0;
}

/* INTERFACE HELPERS -------------------------------------------------- */

static unsigned long find_first_zero_bits(unsigned long *bmap,
					size_t bmap_sz, unsigned order)
{
	unsigned long idx = 0;
	unsigned long i;
	/*
	 * Find next free slot
	 */
retry:
	idx = find_next_zero_bit(bmap, bmap_sz, idx);
	if (idx >= bmap_sz) {
		/*
		 * No free slots at all
		 */
		return bmap_sz;
	}
	/*
	 * See if 2^order bits are available here
	 */
	for (i = idx + 1; i < (1 << order); i++) {
		if (i >= bmap_sz) {
			/*
			 * Ran out of room
			 */
			return bmap_sz;
		}
		if (test_bit(i, bmap)) {
			/*
			 * Bit is set in here; need to search for another
			 * chunk
			 */
			idx = i + 1;
			goto retry;
		}
	}
	/*
	 * Success! We found a chunk with 2^order bits cleared.
	 */
	return idx;
}

static void set_bits(unsigned long idx, unsigned long *bmap,
		unsigned order)
{
	unsigned long i;
	for (i = 0; i < (1 << order); i++)
		set_bit(idx + i, bmap);
}

static void clear_bits(unsigned long idx, unsigned long *bmap,
		unsigned order)
{
	unsigned i;
	for (i = 0; i < (1 << order); i++)
		clear_bit(idx + i, bmap);
}

static int do_mk_map_pages(cptr_t *pages, gpa_t base, unsigned order)
{
	unsigned i, j;
	int ret;

	for (i = 0; i < (1 << order); i++) {
		/*
		 * Tell microkernel to map pages there in guest physical
		 */
		ret = lcd_page_map(pages[i], gpa_add(base, i << PAGE_SHIFT));
		if (ret) {
			lcd_printk("do_mk_map_pages: failed to map in gp");
			goto fail1;
		}
	}

	return 0;

fail1:
	/* undo guest physical mappings  */
	for (j = 0; j < i; j++)
		lcd_page_unmap(pages[i], gpa_add(base, j << PAGE_SHIFT));
	return ret;
}

static int do_mk_unmap_pages(cptr_t *pages, gpa_t base, unsigned order)
{
	unsigned i;
	int ret = 0;
	for (i = 0; i < (1 << order); i++) {
		ret = lcd_page_unmap(pages[i], gpa_add(base, i << PAGE_SHIFT));
		if (ret) {
			lcd_printk("do_mk_unmap_pages: unmap failed");
			goto out;
		}
	}
	
out:
	return ret;
}

static int gv_map_pages(gva_t gv_base, gpa_t gp_base, unsigned order)
{
	unsigned i, j;
	int ret;

	for (i = 0; i < (1 << order); i++) {
		/*
		 * Map in guest virtual
		 */
		ret = gv_map(gva_add(gv_base, (i << PAGE_SHIFT)),
			gpa_add(gp_base, (i << PAGE_SHIFT)));
		if (ret) {
			lcd_printk("gv_map_pages: failed to map in gv");
			goto fail1;
		}
	}

	return 0;

fail1:
	/* undo guest virtual mappings  */
	for (j = 0; j < i; j++)
		gv_unmap(gva_add(gv_base, (j << PAGE_SHIFT)));
	return ret;
}

static int gv_unmap_pages(gva_t gv_base, unsigned order)
{
	unsigned i;
	int ret;

	for (i = 0; i < (1 << order); i++) {
		/*
		 * Unmap in guest virtual
		 */
		ret = gv_unmap(gva_add(gv_base, (i << PAGE_SHIFT)));
		if (ret) {
			lcd_printk("gv_unmap_pages: failed to unmap in gv");
			goto fail1;
		}
	}

	return 0;

fail1:
	return ret;
}

static void update_page2cptr(unsigned long idx, cptr_t *pages, unsigned order)
{
	unsigned long i;
	for (i = 0; i < (1 << order); i++)
		free_mem_page2cptr[idx + i] = pages[i];
}

static void clear_page2cptr(unsigned long idx, unsigned order)
{
	unsigned long i;
	for (i = 0; i < (1 << order); i++)
		free_mem_page2cptr[idx + i] = LCD_CPTR_NULL;
}

static int check_get_idx(gva_t base, unsigned long *out, unsigned order)
{
	unsigned long idx;
	/*
	 * Make sure we're in range (remember gva == gpa)
	 */
	if (gva_val(base) < gpa_val(LCD_FREE_MEM_BASE)) {
		lcd_printk("check_get_idx: gva 0x%lx is below free mem area",
			gva_val(base));
		return -EINVAL;
	}
	if (gva_val(gva_add(base, (1 << order) << PAGE_SHIFT)) >=
		gpa_val(gpa_add(LCD_FREE_MEM_BASE, LCD_FREE_MEM_SIZE))) {
		lcd_printk("check_get_idx: end of page range 0x%lx outside free mem area",
			gva_val(gva_add(base, (1 << order) << PAGE_SHIFT)));
		return -EINVAL;
	}
	/*
	 * Calc index in of first page in free area
	 */
	idx = (gva_val(base) - gpa_val(LCD_FREE_MEM_BASE)) >> PAGE_SHIFT;

	*out = idx;

	return 0;
}

/* INTERFACE -------------------------------------------------- */

int lcd_map_pages_phys(cptr_t *pages, gpa_t *base_out, unsigned order)
{
	unsigned long idx;
	unsigned long base;
	int ret;
	/*
	 * Find 2^order free lots
	 */
	idx = find_first_zero_bits(free_mem_bmap, LCD_FREE_MEM_BMAP_SIZE,
				order);
	if (idx >= LCD_FREE_MEM_BMAP_SIZE) {
		lcd_printk("lcd_map_pages_phys: not enough mem for %d pages",
			(1 << order));
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Set bits to mark pages as in use
	 */
	set_bits(idx, free_mem_bmap, order);

	base = gpa_val(gpa_add(LCD_FREE_MEM_BASE, idx << PAGE_SHIFT));
	/*
	 * Tell microkernel to map pages there in gp
	 */
	ret = do_mk_map_pages(pages, __gpa(base), order);
	if (ret) {
		lcd_printk("lcd_map_pages_phys: failed to map in gp");
		goto fail2;
	}
	/*
	 * Update page -> cptr correspondences
	 */
	update_page2cptr(idx, pages, order);

	*base_out = __gpa(base);
	
	return 0;

fail2:
	clear_bits(idx, free_mem_bmap, order);
fail1:
	return ret;
}

void lcd_unmap_pages_phys(cptr_t *pages, gpa_t base, unsigned order)
{
	int ret;
	unsigned long idx;
	/*
	 * Get index to base page in free area.
	 *
	 * XXX: Remember gpa == gva in heap area.
	 */
	ret = check_get_idx(__gva(gpa_val(base)), &idx, order);
	if (ret) {
		lcd_printk("lcd_unmap_pages_phys: bad base/order");
		goto out;
	}
	/*
	 * Tell microkernel to unmap in guest physical
	 */
	ret = do_mk_unmap_pages(pages, base, order);
	if (ret) {
		lcd_printk("lcd_unmap_pages_phys: failed to unmap in gp");
		goto out;
	}
	/*
	 * Clear bits, cptrs, and page structs
	 */
	clear_bits(idx, free_mem_bmap, order);
	clear_page2cptr(idx, order);
	memset(&page_structs[idx], 0, (1 << order) * sizeof(struct page));

out:
	return;
}

int lcd_map_pages_both(cptr_t *pages, gva_t *base_out, unsigned order)
{
	gpa_t gpa_base;
	int ret;
	/*
	 * Map in guest physical first
	 */
	ret = lcd_map_pages_phys(pages, &gpa_base, order);
	if (ret) {
		lcd_printk("lcd_map_pages_both: error mapping in gpa");
		goto fail1;
	}
	/*
	 * Map in guest virtual next, gv addr = gp addr
	 */
	ret = gv_map_pages(__gva(gpa_val(gpa_base)), gpa_base, order);
	if (ret) {
		lcd_printk("lcd_map_pages_both: failed to map in guest virtual");
		goto fail2;
	}

	*base_out = __gva(gpa_val(gpa_base));
	
	return 0;

fail2:
	lcd_unmap_pages_phys(pages, gpa_base, order);
fail1:
	return ret;
}

void lcd_unmap_pages_both(cptr_t *pages, gva_t base, unsigned order)
{
	int ret;
	/*
	 * Unmap in guest physical first, gpa == gva
	 */
	lcd_unmap_pages_phys(pages, __gpa(gva_val(base)), order);
	/*
	 * Unmap in gva.
	 *
	 * XXX: If we fail here, may be in inconsistent state: mappings
	 * still in guest virtual, but no mappings in guest physical.
	 */
	ret = gv_unmap_pages(base, order);
	if (ret) {
		lcd_printk("lcd_unmap_pages_both: failed to unmap in gv");
		goto out;
	}

out:
	return;
}

int lcd_alloc_pages(unsigned order, gva_t *base_out)
{
	cptr_t pages[1 << LCD_FREE_MEM_MAX_ORDER];
	int ret;
	unsigned i, j;
	/*
	 * Check that order isn't too big
	 */
	if (order > LCD_FREE_MEM_MAX_ORDER) {
		lcd_printk("lcd_alloc_pages: order %d is too big",
			order);
		return -ENOMEM;
	}
	/*
	 * Get pages from microkernel
	 */
	for (i = 0; i < (1 << order); i++) {
		ret = lcd_page_alloc(&pages[i]);
		if (ret) {
			lcd_printk("lcd_alloc_pages: failed to alloc from mk");
			goto fail1;
		}
	}
	/*
	 * Map them
	 */
	ret = lcd_map_pages_both(pages, base_out, order);
	if (ret) {
		lcd_printk("lcd_alloc_pages: failed to map");
		goto fail1;
	}
	
	return 0;

fail1:
	/* Free those we alloc'd */
	for (j = 0; j < i; j++)
		lcd_cap_delete(pages[i]);
	return ret;
}

int lcd_alloc_page(gva_t *page_out)
{
	return lcd_alloc_pages(0, page_out);
}

void lcd_free_pages(gva_t base, unsigned order)
{
	int ret;
	unsigned long idx;
	/*
	 * Get index in free area
	 */
	ret = check_get_idx(base, &idx, order);
	if (ret) {
		lcd_printk("lcd_free_pages: bad gva");
		return;
	}
	/*
	 * Do unmap
	 */
	lcd_unmap_pages_both(&free_mem_page2cptr[idx], base, order);
}

void lcd_free_page(gva_t page)
{
	lcd_free_pages(page, 0);
}

struct page * lcd_virt_to_head_page(const void *addr)
{
	unsigned long idx;
	int ret;
	/*
	 * Get index in free area
	 */
	ret = check_get_idx(__gva((unsigned long)addr), &idx, 0);
	if (ret) {
		lcd_printk("virt_to_head_page: bad address 0x%lx",
			(unsigned long)addr);
		return NULL;
	}
	return &page_structs[idx];
}

void * lcd_page_address(const struct page *p)
{
	unsigned long idx;
	/*
	 * Calculate offset in page struct array
	 */
	idx = (unsigned long)(p - &page_structs[0]);
	/*
	 * Convert idx to guest virtual address (gva = gpa)
	 */
	return (void *)gpa_val(gpa_add(LCD_FREE_MEM_BASE, 
						idx << PAGE_SHIFT));
}

struct page * lcd_alloc_pages_exact_node(int nid, gfp_t gfp_mask,
					unsigned order)
{
	int ret;
	gva_t base;
	unsigned long idx;
	int i;
	/*
	 * Alloc pages, ignore gfp and nid
	 */
	ret = lcd_alloc_pages(order, &base);
	if (ret) {
		lcd_printk("lcd_alloc_pages_exact_node: alloc error");
		return NULL;
	}
	/*
	 * Ignore ret val, since it should be valid ...
	 */
	check_get_idx(base, &idx, order);
	/*
	 * Set flags
	 */
	for (i = 0; i < (1 << order); i++)
		page_structs[idx + i].flags = (unsigned long)gfp_mask;
	/*
	 * Return address of first page
	 */
	return &page_structs[idx];
}

void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order)
{
	lcd_free_pages(__gva(addr), order);
}

int gp_ioremap(cptr_t phys_addr, unsigned long size, gpa_t *base) 
{
	
	unsigned int slots = 0;
	unsigned int index = 0;
	int ret = 0;

	slots = size >> PAGE_SHIFT;
	index = find_first_zero_bits(ioremap_gpa_bmap, LCD_IOREMAP_BMAP_SIZE, slots);
	if(index >= LCD_IOREMAP_BMAP_SIZE) {
		lcd_printk("gpa_ioremap: exhausted memory space in GPA \n");
		return -ENOMEM;		
	}
 	
	/* Size required for mapping is figured out by the microkernel as capabilities 
 	 * are associated with their size */
	*base = gpa_add(LCD_IOREMAP_GPA_BASE, index << PAGE_SHIFT);
	ret = lcd_page_map(phys_addr, *base);
        if (ret) {
                lcd_printk("gpa_ioremap: cannot map physical address to GPA \n");
                return ret;
        }

	set_bits(index, ioremap_gpa_bmap, slots);
	ioremap_gpa_phys2cptr[index] = phys_addr;
	return ret;
}

int gp_iounmap(gpa_t phys_addr, unsigned long size)
{
	unsigned int slots = 0;
	unsigned index = 0;
	int ret = 0;

	slots = size >> PAGE_SHIFT;
	index = (gpa_val(phys_addr) - gpa_val(LCD_IOREMAP_GPA_BASE)) >> PAGE_SHIFT;
	
	if(!(gpa_val(phys_addr) >= gpa_val(LCD_IOREMAP_GPA_BASE) &&
		gpa_val(gpa_add(phys_addr, index << PAGE_SHIFT)) < 
		gpa_val(gpa_add(LCD_IOREMAP_GPA_BASE, LCD_IOREMAP_GPA_SIZE)))) {

		lcd_printk("gp_iounmap: Trying to unmap invalid region of memory \n");
		return -EFAULT;
	}	
	
	ret = lcd_page_unmap(ioremap_gpa_phys2cptr[index], phys_addr);
	if(ret) {
		lcd_printk("gp_iounmap: unmap failed \n");
	}
	clear_bits(index, ioremap_gpa_bmap, slots);	
	ioremap_gpa_phys2cptr[index] = LCD_CPTR_NULL;
	return ret; 
}


/* ADDR -> CPTR TRANSLATION ---------------------------------------- */

static int fits_in_page(unsigned long data, unsigned long len)
{
	return (data & PAGE_MASK) == ((data + len) & PAGE_MASK);
}

int lcd_phys_addr_to_page_cptr(unsigned long data, unsigned long len, 
			cptr_t *page_cptr_out, unsigned long *offset_out)
{
	unsigned long pg_addr;
	unsigned long pg_idx;
	int ret;
	/*
	 * Mask off bits
	 */
	pg_addr = data & PAGE_MASK;
	/*
	 * See if in range
	 */
	if (pg_addr < gpa_val(LCD_FREE_MEM_BASE) ||
		pg_addr >= (gpa_val(LCD_FREE_MEM_BASE) + LCD_FREE_MEM_SIZE)) {
		LIBLCD_ERR("page addr %lx outside of heap, not allowed",
			pg_addr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Make sure data doesn't trail off page
	 */
	if (!fits_in_page(data, len)) {
		LIBLCD_ERR("data (pa=%lx,len=%lx) spans more than one page",
			data, len);
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * Translate to cptr
	 */
	pg_idx = (pg_addr - gpa_val(LCD_FREE_MEM_BASE)) >> PAGE_SHIFT;
	*page_cptr_out = free_mem_page2cptr[pg_idx];
	/*
	 * Calculate offset into page
	 */
	*offset_out = data & ~PAGE_MASK;

	return 0;

fail2:
fail1:
	return ret;
}

int lcd_virt_addr_to_page_cptr(char *data, unsigned long len, 
			cptr_t *page_cptr_out, unsigned long *offset_out)
{
	int ret;
	gpa_t gpa;
	pte_t *pte;
	/*
	 * Translate virtual -> physical. Unfortunately, this requires
	 * a page walk because the module and the rest of the heap are
	 * in two different parts of the virtual address space. 
	 * (For most of the heap, gva == gpa, but not the module.)
	 */
	ret = gv_walk(__gva((unsigned long)data), &pte);
	if (ret) {
		LIBLCD_ERR("err getting pte for gva %p", data);
		goto fail1;
	}
	if (!pte_present(*pte)) {
		LIBLCD_ERR("no mapping for gva %p", data);
		ret = -EINVAL;
		goto fail2;
	}
	gpa = gpa_add(gv_get(pte),
		((unsigned long)data) & ~PAGE_MASK); /* add offset into pg */
	/*
	 * Use physical address for look up
	 */
	return lcd_phys_addr_to_page_cptr(gpa_val(gpa),
					len,
					page_cptr_out, 
					offset_out);

fail2:
fail1:
	return ret;
}


/* BOOT SETUP -------------------------------------------------- */

static void get_mem_boot_info(void)
{
	struct lcd_boot_info *bi;
	/*
	 * The boot info is in the boot info page, mapped when the lcd
	 * is built.
	 */
	bi = lcd_get_boot_info();
	/*
	 * Extract the page info data, store in globals (declared in this
	 * file).
	 */
	num_boot_mem_pi = bi->num_boot_mem_pi;
	num_stack_mem_pi = bi->num_stack_mem_pi;
	num_paging_mem_pi = bi->num_paging_mem_pi;
	num_free_mem_pi = bi->num_free_mem_pi;
	boot_mem_pi_start = bi->boot_mem_pi_start;
	stack_mem_pi_start = bi->stack_mem_pi_start;
	paging_mem_pi_start = bi->paging_mem_pi_start;
	free_mem_pi_start = bi->free_mem_pi_start;
}

static void setup_pgd(void)
{
	unsigned long cr3;
	asm("mov %%cr3, %0" : "=r" (cr3) : :);
	/*
	 * Guest virtual == guest physical
	 */
	root = (pgd_t *)(cr3 & PAGE_MASK);
}

static void init_page_alloc_data(void)
{
	unsigned i;
	unsigned idx;
	/*
	 * Zero out data
	 */
	memset(boot_cptrs, 0, sizeof(boot_cptrs));
	memset(stack_cptrs, 0, sizeof(stack_cptrs));
	memset(paging_mem_bmap, 0, sizeof(paging_mem_bmap));
	memset(paging_mem_page2cptr, 0, sizeof(paging_mem_page2cptr));
	memset(free_mem_page2cptr, 0, sizeof(free_mem_page2cptr));
	memset(free_mem_bmap, 0, sizeof(free_mem_bmap));
	memset(page_structs, 0, sizeof(page_structs));
	/*
	 * Boot mem pages
	 */
	for (i = 0; i < num_boot_mem_pi; i++) {
		idx = gpa_val(boot_mem_pi_start[i].page_gpa);
		idx -= gpa_val(LCD_BOOT_PAGES_GPA);
		idx >>= PAGE_SHIFT;
		BUG_ON(idx >= (1 << LCD_BOOT_PAGES_ORDER));
		boot_cptrs[idx] = boot_mem_pi_start[i].my_cptr;
	}
	/*
	 * Stack mem pages
	 */
	for (i = 0; i < num_stack_mem_pi; i++) {
		idx = gpa_val(stack_mem_pi_start[i].page_gpa);
		idx -= gpa_val(LCD_STACK_GPA);
		idx >>= PAGE_SHIFT;
		BUG_ON(idx >= (1 << LCD_STACK_PAGES_ORDER));
		stack_cptrs[idx] = stack_mem_pi_start[i].my_cptr;
	}
	/*
	 * Set up paging mem bitmap
	 *
	 * remember that gva = gpa ...
	 */
	for (i = 0; i < num_paging_mem_pi; i++) {
		idx = gpa_val(paging_mem_pi_start[i].page_gpa);
		idx -= gpa_val(LCD_GV_PAGING_MEM_BASE);
		idx >>= PAGE_SHIFT;
		BUG_ON(idx >= LCD_GV_PAGING_MEM_BMAP_SIZE);
		set_bit(idx, paging_mem_bmap);
		paging_mem_page2cptr[idx] = paging_mem_pi_start[i].my_cptr;
	}
	/*
	 * Set up free mem bitmap
	 */
	for (i = 0; i < num_free_mem_pi; i++) {
		idx = gpa_val(free_mem_pi_start[i].page_gpa);
		idx -= gpa_val(LCD_FREE_MEM_BASE);
		idx >>= PAGE_SHIFT;
		BUG_ON(idx >= LCD_FREE_MEM_BMAP_SIZE);
		set_bit(idx, free_mem_bmap);
		free_mem_page2cptr[idx] = free_mem_pi_start[i].my_cptr;
	}
}

void cpucache_init(void);

static void __init_refok kmalloc_init(void)
{
	kmem_cache_init();
	kmem_cache_init_late();
	cpucache_init();
}

int lcd_mem_init(void)
{
	/*
	 * Extract page info from boot info page
	 */
	get_mem_boot_info();
	/*
	 * Initialize bitmap and page2cptr globals
	 */
	init_page_alloc_data();
	/*
	 * Set up the pgd pointer
	 */
	setup_pgd();
	/*
	 * Init kmalloc
	 */
	kmalloc_init();

	return 0;
}
