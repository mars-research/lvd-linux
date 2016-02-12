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

static int do_grant_and_map_for_mem(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest)
{
	int ret;
	cptr_t mo;
	unsigned int order;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &order);
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
				gva_t m_init_link_addr, gva_t m_core_link_addr)
{
	unsigned long offset;
	cptr_t *c;
	int ret;
	/*
	 * Map module init and core at correct offsets
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

fail2:
fail1:
	return ret;
}

static int setup_phys_addr_space(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr)
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
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->gv_pgd,
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
					m_init_link_addr, m_core_link_addr);
	if (ret)
		goto fail4;
	
	return 0;

fail4:  /* Just return; caller should kill new LCD and free up resources. */
fail3:
fail2:
fail1:
	return ret;
}

static void setup_lcd_pud(struct lcd_create_ctx *ctx)
{
	/*
	 * This assumes the PAT has Writeback in PAT0, Uncacheable in PAT1.
	 *
	 * See Intel SDM V3 11.12.3 for how PAT indexing is done.
	 */
	unsigned int i;
	unsigned int ioremap_offset = LCD_IOREMAP_REGION_OFFSET >> 30;
	unsigned int after_ioremap_offset = (ioremap_offset +
					LCD_IOREMAP_REGION_SIZE) >> 30;
	pteval_t wb_flags = _PAGE_PRESENT | _PAGE_RW | _PAGE_PSE;
	pteval_t uc_flags = _PAGE_PRESENT | _PAGE_RW | _PAGE_PSE | _PAGE_PWT;
	pud_t *pud_entry;
	/*
	 * Map first GBs as write back
	 */
	for (i = 0; i < ioremap_offset; i++) {
		pud_entry = &ctx->gv_pud[i];
		set_pud(pud_entry,
			__pud((LCD_PHYS_BASE + i * (1UL << 30)) | wb_flags));
	}
	/*
	 * Map ioremap GBs as uncacheable
	 */
	for (i = ioremap_offset; i < after_ioremap_offset; i++) {
		pud_entry = &ctx->gv_pud[i];
		set_pud(pud_entry,
			__pud((LCD_PHYS_BASE + i * (1UL << 30)) | uc_flags));
	}
	/*
	 * Map remaining GBs as write back
	 */
	for (i = after_ioremap_offset; i < 512; i++) {
		pud_entry = &ctx->gv_pud[i];
		set_pud(pud_entry,
			__pud((LCD_PHYS_BASE + i * (1UL << 30)) | wb_flags));
	}
}

static void setup_lcd_pgd(struct lcd_create_ctx *ctx)
{
	pgd_t *pgd_entry;
	gpa_t pud_gpa;
	pteval_t flags = 0;

	pgd_entry = &ctx->gv_pgd[511]; /* only map last pud for high 512 GBs */

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
	 * pgd and pud (PML4 and PDPT) should already be zero'd out
	 *
	 * Set up root pgd
	 */
	setup_lcd_pgd(ctx);
	/*
	 * Set up pud (only one for high 512 GBs)
	 */
	setup_lcd_pud(ctx);
}

static int setup_addr_spaces(cptr_t lcd, struct lcd_create_ctx *ctx,
			gva_t m_init_link_addr, gva_t m_core_link_addr)
{
	int ret;
	/*
	 * Set up physical address space (except UTCB - that's done
	 * via lcd_config)
	 */
	ret = setup_phys_addr_space(lcd, ctx, m_init_link_addr, 
				m_core_link_addr);
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
	if (ctx->gv_pgd)
		lcd_free_pages(lcd_virt_to_head_page(ctx->gv_pgd),
			LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (ctx->lcd_boot_info)
		lcd_free_pages(lcd_virt_to_head_page(ctx->lcd_boot_info),
			LCD_BOOTSTRAP_PAGES_ORDER);
	/*
	 * Free the ctx
	 */
	kfree(ctx);
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
	ret = cptr_cache_init(lcd_to_boot_cptr_cache(ctx));
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
	ctx->gv_pgd = lcd_page_address(p2);
	ctx->gv_pud = lcd_page_address(p2 + 1);
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
			&m_init_link_addr, &m_core_link_addr);
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
				m_core_link_addr);
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

	/*
	 * Return context and lcd
	 */
	*ctx_out = ctx;
	*lcd_out = lcd;

	return 0;

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
