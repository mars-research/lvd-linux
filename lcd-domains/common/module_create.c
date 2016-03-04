/*
 * module_load.c
 *
 * For creating an empty LCD and loading a kernel
 * module in it.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <linux/mm.h>
#include <libcap.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

/* MEMORY REGIONS -------------------------------------------------- */

/*
 * This assumes the PAT has Writeback in PAT0, Uncacheable in PAT1.
 *
 * See Intel SDM V3 11.12.3 for how PAT indexing is done.
 */
#define LCD_UNCACHEABLE_FLAGS (_PAGE_PRESENT | _PAGE_RW | _PAGE_PWT)
#define LCD_WRITEBACK_FLAGS (_PAGE_PRESENT | _PAGE_RW)

struct lcd_mem_region {
	const char *name;
	unsigned long offset;
	unsigned long size;
	pteval_t flags;
};

#define LCD_NR_MEM_REGIONS 6

static struct lcd_mem_region lcd_mem_regions[LCD_NR_MEM_REGIONS] = {
	{
		"miscellaneous",
		.offset = LCD_MISC_REGION_OFFSET,
		.size =   LCD_MISC_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"stack",
		.offset = LCD_STACK_REGION_OFFSET,
		.size =   LCD_STACK_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"heap",
		.offset = LCD_HEAP_REGION_OFFSET,
		.size =   LCD_HEAP_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"ram map",
		.offset = LCD_RAM_MAP_REGION_OFFSET,
		.size =   LCD_RAM_MAP_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"ioremap",
		.offset = LCD_IOREMAP_REGION_OFFSET,
		.size =   LCD_IOREMAP_REGION_SIZE,
		.flags =  LCD_UNCACHEABLE_FLAGS,
	},
	{
		"kernel module",
		.offset = LCD_KERNEL_MODULE_REGION_OFFSET,
		.size =   LCD_KERNEL_MODULE_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
};

/* PHYSICAL ADDRESS SPACE -------------------------------------------------- */

static int do_grant_and_map_for_mem(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map(lcd, mo, *dest, map_base);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}

static int do_kernel_module_grant_map(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr,
				unsigned long m_init_size,
				unsigned long m_core_size)
{
	unsigned long offset;
	cptr_t *c;
	int ret;
	/*
	 * Map module init and core at correct offsets
	 *
	 * XXX: Assumes init and core link addresses come from
	 * the host's module mem area. 
	 * (LCD_KERNEL_MODULE_REGION_GV_ADDR = 
	 */
	offset = gva_val(m_init_link_addr) - 
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.module_init);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_init_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail1;

	offset = gva_val(m_core_link_addr) -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.module_core);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_core_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail2;

	lcd_to_boot_info(ctx)->module_init_base = m_init_link_addr;
	lcd_to_boot_info(ctx)->module_core_base = m_core_link_addr;
	lcd_to_boot_info(ctx)->module_init_size = m_init_size;
	lcd_to_boot_info(ctx)->module_core_size = m_core_size;

fail2:
fail1:
	return ret;
}

static int setup_phys_addr_space(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr,
				unsigned long m_init_size,
				unsigned long m_core_size)
{
	int ret;
	cptr_t *c;
	/*
	 * Map and grant bootstrap pages, page tables, and stack
	 */
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.boot_pages);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->lcd_boot_info,
				LCD_BOOTSTRAP_PAGES_GP_ADDR, c);
	if (ret)
		goto fail1;
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.gv);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->gv_pg_tables,
				LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, c);
	if (ret)
		goto fail2;
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.stack);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->stack,
				LCD_STACK_GP_ADDR, c);
	if (ret)
		goto fail3;
	/*
	 * Map and grant kernel module
	 */
	ret = do_kernel_module_grant_map(lcd, ctx,
					m_init_link_addr, m_core_link_addr,
					m_init_size, m_core_size);
	if (ret)
		goto fail4;

	return 0;

fail4:  /* Just return; caller should kill new LCD and free up resources. */
fail3:
fail2:
fail1:
	return ret;
}

/* VIRTUAL ADDRESS SPACE ------------------------------ */

static void dump_pg_table(unsigned long *pgtable)
{
	unsigned int i;
	for (i = 0; i < 512; i++)
		printk("     %03u: 0x%016lx\n", i, pgtable[i]);
}

void lcd_dump_virt_addr_space(struct lcd_create_ctx *ctx)
{
	unsigned int i;
	LIBLCD_MSG("  DUMP LCD GUEST VIRTUAL PAGE TABLES:");
	
	for (i = 0; i < LCD_BOOTSTRAP_PAGE_TABLES_SIZE >> PAGE_SHIFT; i++) {
		/*
		 * XXX: Assumes 512 entries per table, 8 bytes/entry
		 */
		printk("\n     page table %d\n     -------------------\n",
			i);
		dump_pg_table(ctx->gv_pg_tables + i * 512 * 8);
	}
}

static void setup_lcd_pmd(struct lcd_mem_region *reg, pmd_t *pmd,
			unsigned int gigabyte_idx)
{
	unsigned int k;
	unsigned long gp;

	for (k = 0; k < 512; k++) {
		/*
		 * Guest physical address we put in the table entry is:
		 *
		 *    base + 
		 *    offset to the 1GB region for this pmd +
		 *    offset for this table entry (some multiple of 2MBs)
		 */
		gp = LCD_PHYS_BASE + gigabyte_idx * (1UL << 30) +
			k * (1UL << 21);
		set_pmd(&pmd[k], __pmd(gp | reg->flags | _PAGE_PSE));
	}
}

static void setup_lcd_pmds(pmd_t *pmds)
{
	unsigned int i, j, lo, hi;
	pmd_t *pmd_entry;
	struct lcd_mem_region *reg;
	/*
	 * Map each memory region, filling out entire pmd's so that we're
	 * mapping multiples of 1GB
	 */
	pmd_entry = pmds;
	for (i = 0; i < LCD_NR_MEM_REGIONS; i++) {
		reg = &lcd_mem_regions[i];
		/*
		 * Set up enough pmd's to fill in memory region
		 */
		lo = reg->offset >> 30;
		hi = lo + (reg->size >> 30);
		for (j = lo; j < hi; j++) {
			setup_lcd_pmd(reg, pmd_entry, j);
			pmd_entry += 512;
		}
	}

}

static void setup_lcd_pud(pud_t *pud)
{
	unsigned int i, j, lo, hi;
	pud_t *pud_entry;
	struct lcd_mem_region *reg;
	gpa_t pmd_gpa;
	/*
	 * pmd's come after pgd and pud
	 */
	pmd_gpa = gpa_add(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, 2 * PAGE_SIZE);
	/*
	 * Point to correct pmd's for each memory region
	 */
	for (i = 0; i < LCD_NR_MEM_REGIONS; i++) {
		reg = &lcd_mem_regions[i];
		/*
		 * Point to correct pmd's
		 */
		lo = reg->offset >> 30;
		hi = lo + (reg->size >> 30);
		for (j = lo; j < hi; j++) {
			pud_entry = &pud[j];
			set_pud(pud_entry,
				__pud(gpa_val(pmd_gpa) | reg->flags));
			pmd_gpa = gpa_add(pmd_gpa, PAGE_SIZE);
		}
	}
}

static void setup_lcd_pgd(pgd_t *pgd)
{
	pgd_t *pgd_entry;
	gpa_t pud_gpa;
	pteval_t flags = 0;

	pgd_entry = &pgd[511]; /* only map last pud for high 512 GBs */

	/* pud comes after pgd */
	pud_gpa = gpa_add(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, PAGE_SIZE);

	flags |= _PAGE_PRESENT;
	flags |= _PAGE_RW;

	set_pgd(pgd_entry,
		__pgd(gpa_val(pud_gpa) | flags));
}

static void setup_virt_addr_space(struct lcd_create_ctx *ctx)
{
	/*
	 * XXX: Assumes 512 entries / table
	 */
	void *cursor = ctx->gv_pg_tables;
	/*
	 * page tables should already be zero'd out
	 *
	 * Set up root pgd
	 */
	setup_lcd_pgd(cursor);
	cursor += 512 * sizeof(pgd_t);
	/*
	 * Set up pud (only one for high 512 GBs)
	 * (skip over pgd)
	 */
	setup_lcd_pud(cursor);
	cursor += 512 * sizeof(pud_t);
	/*
	 * Set up pmd's (one for each 1GB region)
	 * (skip over pgd and pud)
	 */
	setup_lcd_pmds(cursor);
}

static int setup_addr_spaces(cptr_t lcd, struct lcd_create_ctx *ctx,
			gva_t m_init_link_addr, gva_t m_core_link_addr,
			unsigned long m_init_size,
			unsigned long m_core_size)
{
	int ret;
	/*
	 * Set up physical address space (except UTCB - that's done
	 * via lcd_config)
	 */
	ret = setup_phys_addr_space(lcd, ctx, m_init_link_addr, 
				m_core_link_addr,
				m_init_size, m_core_size);
	if (ret) {
		LIBLCD_ERR("error setting up phys addr space");
		goto fail1;
	}
	/*
	 * Set up virtual address space
	 */
	setup_virt_addr_space(ctx);

	return 0;

fail1: /* just return non-zero ret; caller will free mem */
	return ret;
}

/* -------------------------------------------------- */

static int init_create_ctx(struct lcd_create_ctx **ctx_out, char *mname)
{
	struct lcd_create_ctx *ctx;
	int ret;
	/*
	 * Alloc ctx
	 */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		LIBLCD_ERR("kzalloc failed");
		ret = -ENOMEM;
		goto fail1;
	}

	strncpy(ctx->mname, mname, LCD_MODULE_NAME_MAX);

	*ctx_out = ctx;

	return 0;

fail1:
	return ret;
}

static void destroy_create_ctx(struct lcd_create_ctx *ctx)
{
	/*
	 * Remove pages from our address space and delete our
	 * capabilities to them. If these are the last caps,
	 * pages will be freed by microkernel.
	 */
	if (ctx->m_init_bits)
		lcd_release_module(ctx->m_init_bits, ctx->m_core_bits);
	if (ctx->stack)
		lcd_free_pages(lcd_virt_to_head_page(ctx->stack),
			LCD_STACK_ORDER);
	if (ctx->gv_pg_tables)
		lcd_free_pages(lcd_virt_to_head_page(ctx->gv_pg_tables),
			LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (ctx->lcd_boot_info)
		lcd_free_pages(lcd_virt_to_head_page(ctx->lcd_boot_info),
			LCD_BOOTSTRAP_PAGES_ORDER);
	/*
	 * Free the ctx
	 */
	kfree(ctx);
}

static int do_cptr_cache_init(struct cptr_cache *cache)
{
	int ret;
	cptr_t unused;

	ret = cptr_cache_init(cache);
	if (ret) {
		LIBLCD_ERR("error init'ing cptr cache");
		return ret;
	}
	/*
	 * Reserve first two slots for call/reply caps (just alloc them)
	 */
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc1");
		return ret;
	}
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc2");
		return ret;
	}

	return 0;
}

static int get_pages_for_lcd(struct lcd_create_ctx *ctx)
{
	struct page *p1, *p2, *p3;
	int ret;
	/*
	 * We explicity zero things out. If this code is used inside
	 * an LCD, it may not zero out the alloc'd pages.
	 *
	 * Alloc boot pages
	 */
	p1 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGES_ORDER);
	if (!p1) {
		LIBLCD_ERR("alloc boot pages failed");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(lcd_page_address(p1), 0, LCD_BOOTSTRAP_PAGES_SIZE);
	ctx->lcd_boot_info = lcd_page_address(p1);
	/*
	 * Initialize boot cptr cache
	 */
	ret = do_cptr_cache_init(lcd_to_boot_cptr_cache(ctx));
	if (ret) {
		LIBLCD_ERR("failed to init cptr cache");
		goto fail2;
	}
	/*
	 * Alloc boot page tables
	 */
	p2 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (!p2) {
		LIBLCD_ERR("alloc boot page tables failed");
		ret = -ENOMEM;
		goto fail3;
	}
	memset(lcd_page_address(p2), 0, LCD_BOOTSTRAP_PAGE_TABLES_SIZE);
	ctx->gv_pg_tables = lcd_page_address(p2);
	/*
	 * Alloc stack
	 */
	p3 = lcd_alloc_pages(0, LCD_STACK_ORDER);
	if (!p3) {
		LIBLCD_ERR("alloc stack pages failed");
		ret = -ENOMEM;
		goto fail4;
	}
	memset(lcd_page_address(p3), 0, LCD_STACK_SIZE);
	ctx->stack = lcd_page_address(p3);

	return 0;

fail4:
	lcd_free_pages(p2, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
fail3:
	cptr_cache_destroy(lcd_to_boot_cptr_cache(ctx));
fail2:
	lcd_free_pages(p1, LCD_BOOTSTRAP_PAGES_ORDER);
fail1:
	return ret;
}

int lcd_create_module_lcd(char *mdir, char *mname, cptr_t *lcd_out,
			struct lcd_create_ctx **ctx_out)
{
	int ret;
	cptr_t m_init_cptr, m_core_cptr;
	gva_t m_init_link_addr, m_core_link_addr;
	unsigned long m_init_size, m_core_size;
	unsigned long m_struct_module_core_offset;
	struct lcd_create_ctx *ctx;
	cptr_t lcd;
	/*
	 * Initialize create ctx
	 */
	ret = init_create_ctx(&ctx, mname);
	if (ret) {
		LIBLCD_ERR("error creating ctx");
		goto fail1;
	}
	/*
	 * Alloc boot pages, stack pages, etc. for LCD
	 */
	ret = get_pages_for_lcd(ctx);
	if (ret) {
		LIBLCD_ERR("error alloc'ing boot, stack pages for LCD");
		goto fail2;
	}
	/*
	 * Load kernel module into caller's address space
	 */
	ret = lcd_load_module(mdir, mname,
			&ctx->m_init_bits, &ctx->m_core_bits,
			&m_init_cptr, &m_core_cptr,
			&m_init_link_addr, &m_core_link_addr,
			&m_init_size, &m_core_size,
			&m_struct_module_core_offset);
	if (ret) {
		LIBLCD_ERR("error loading kernel module");
		goto fail3;
	}
	/*
	 * At this point, we have all of the data that will go in the LCD
	 * (the microkernel has the UTCB page)
	 *
	 * Initialize empty LCD
	 */
	ret = lcd_create(&lcd);
	if (ret) {
		LIBLCD_ERR("error creating empty LCD");
		goto fail4;
	}
	/*
	 * Set up address spaces
	 */
	ret = setup_addr_spaces(lcd, ctx, m_init_link_addr, 
				m_core_link_addr,
				m_init_size, m_core_size);
	if (ret) {
		LIBLCD_ERR("error setting up address spaces");
		goto fail5;
	}
	/*
	 * Configure initial control registers, etc. for LCD
	 */
	ret = lcd_config_registers(lcd, m_init_link_addr,
				/* implicity put a null return address */
				gva_add(LCD_STACK_GV_ADDR,
					LCD_STACK_SIZE - sizeof(void *)),
				LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR,
				LCD_UTCB_GP_ADDR);
	if (ret) {
		LIBLCD_ERR("error configuring LCDs registers");
		goto fail6;
	}
#ifndef LCD_ISOLATE
	/*
	 * For non-isolated code running this, we also remember where
	 * the struct module copy is located so we can do stack traces.
	 *
	 * Recall that gva == hva for non-isolated.
	 */
	ret = lcd_set_struct_module_hva(lcd, 
					__hva(gva_val(m_core_link_addr) +
						m_struct_module_core_offset));
	if (ret) {
		LIBLCD_ERR("error setting struct module hva");
		goto fail7;
	}
#endif
	/*
	 * Return context and lcd
	 */
	*ctx_out = ctx;
	*lcd_out = lcd;

	return 0;

#ifndef LCD_ISOLATE
fail7:
#endif
fail6:
fail5:
	lcd_cap_delete(lcd);
fail4:
fail3:
fail2:
	destroy_create_ctx(ctx);
fail1:
	return ret;
}

void lcd_destroy_create_ctx(struct lcd_create_ctx *ctx)
{
	destroy_create_ctx(ctx);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_create_module_lcd);
EXPORT_SYMBOL(lcd_destroy_create_ctx);
