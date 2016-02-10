/*
 * module_load.c
 *
 * For creating an empty LCD and loading a kernel
 * module in it.
 *
 * Copyright: University of Utah
 */

static int setup_phys_addr_space(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr)
{
	int ret;






}

static int setup_addr_spaces(cptr_t lcd, struct lcd_create_ctx *ctx,
			gva_t m_init_link_addr, gva_t m_core_link_addr)
{
	int ret;
	/*
	 * Set up physical address space
	 */
	ret = setup_phys_addr_space(lcd, cxt, m_init_link_addr, 
				m_core_link_addr);
	if (ret) {
		LIBLCD_ERR("error setting up phys addr space");
		goto fail1;
	}
	/*
	 * Set up virtual address space
	 */
	ret = setup_virt_addr_space(lcd, cxt, m_init_link_addr, 
				m_core_link_addr);
	if (ret) {
		LIBLCD_ERR("error setting up virt addr space");
		goto fail2;
	}

	return 0;

fail2:  /* just return non-zero ret; caller will free mem */
fail1:
	return ret;
}

static int init_create_ctx(struct lcd_create_ctx **ctx_out)
{
	struct lcd_create_ctx *ctx;
	/*
	 * Alloc ctx
	 */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		LIBLCD_ERR("kzalloc failed");
		ret = -ENOMEM;
		goto fail1;
	}

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
		lcd_module_release(ctx->m_init_bits, ctx->m_core_bits);
	if (ctx->stack)
		lcd_free_pages(lcd_virt_to_page(ctx->stack),
			LCD_STACK_ORDER);
	if (ctx->gv_pgd)
		lcd_free_pages(lcd_virt_to_page(ctx->gv_pgd),
			LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (ctx->lcd_boot_info)
		lcd_free_pages(lcd_virt_to_page(ctx->lcd_boot_info),
			LCD_BOOTSTRAP_PAGES_ORDER);
	/*
	 * Free the ctx
	 */
	kfree(ctx);
}

static int get_pages_for_lcd(struct lcd_create_ctx *ctx)
{
	struct page *p1, *p2, *p3;
	/*
	 * Alloc boot pages
	 */
	p1 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGES_ORDER);
	if (!p1) {
		LIBLCD_ERR("alloc boot pages failed");
		ret = -ENOMEM;
		goto fail1;
	}
	ctx->lcd_boot_info = lcd_page_address(p1);
	/*
	 * Alloc boot page tables
	 */
	p2 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (!p) {
		LIBLCD_ERR("alloc boot page tables failed");
		ret = -ENOMEM;
		goto fail2;
	}
	ctx->gv_pgd = lcd_page_address(p2);
	ctx->gv_pud = lcd_page_address(p2 + 1);
	/*
	 * Alloc stack
	 */
	p3 = lcd_alloc_pages(0, LCD_STACK_ORDER);
	if (!p3) {
		LIBLCD_ERR("alloc stack pages failed");
		ret = -ENOMEM;
		goto fail3;
	}
	ctx->stack = lcd_page_address(p3);

	return 0;

fail3:
	lcd_free_pages(p2, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
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
	ret = init_create_ctx(&ctx);
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
	ret = lcd_module_load(mdir, mname,
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
	ret = setup_addr_spaces(lcd, cxt, m_init_link_addr, 
				m_core_link_addr);
	if (ret) {
		LIBLCD_ERR("error setting up address spaces");
		goto fail5;
	}
	/*
	 * Configure initial control registers, etc. for LCD
	 */

	/*
	 * Return context and lcd
	 */
	*cxt_out = cxt;
	*lcd_out = lcd;

	return 0;

fail5:
	lcd_cap_delete(lcd);
fail4:
fail3:
fail2:
	destroy_create_ctx(ctx);
fail1:
	return ret;
}
