/*
 * module_load.c
 *
 * For creating an empty LCD and loading a kernel
 * module in it.
 *
 * Copyright: University of Utah
 */

static int init_create_ctx(struct lcd_create_ctx **ctx_out)
{
	struct lcd_create_ctx *ctx;
	struct page *p;
	void *boot_pages;
	unsigned int order1, order2;
	/*
	 * Alloc ctx
	 */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		LIBLCD_ERR("kzalloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Alloc boot pages
	 */
	order1 = ilog(LCD_BOOTSTRAP_PAGE_SIZE >> PAGE_SHIFT);
	p = lcd_alloc_pages(0, order1);
	if (!p) {
		LIBLCD_ERR("alloc pages failed");
		ret = -ENOMEM;
		goto fail2;
	}
	boot_pages = lcd_page_address(p);
	/*
	 * Get creator cptr
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)boot_pages),
			&ctx->creator_cptrs.boot_pages_cptr,
			&order2);
	if (ret) {
		LIBLCD_ERR("error looking up cptr for boot pages");
		goto fail3;
	}
	if (order1 != order2) {
		LIBLCD_ERR("order1 = %d, while order2 = %d; internal error",
			order1, order2);
		goto fail4;
	}
	
	ctx->lcd_boot_info = boot_pages;

	*ctx_out = ctx;

	return 0;

fail4:
fail3:
	lcd_free_pages(p, order1);
fail2:
	kfree(ctx);
fail1:
	return ret;
}

int lcd_create_module_lcd(char *mdir, char *mname, cptr_t *lcd_out,
			struct lcd_create_ctx **ctx_out)
{
	int ret;
	void *m_init_bits, *m_core_bits;
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
	 * Load kernel module into caller's address space
	 */
	ret = lcd_module_load(mdir, mname,
			&m_init_bits, &m_core_bits,
			&ctx->creator_cptrs.module_init_cptr,
			&ctx->creator_cptrs.module_core_cptr,
			&m_init_link_addr, &m_core_link_addr);
	if (ret) {
		LIBLCD_ERR("error loading kernel module");
		goto fail2;
	}
	/*
	 * Initialize empty LCD
	 */
	ret = lcd_create(&lcd);
	if (ret) {
		LIBLCD_ERR("error creating empty LCD");
		goto fail3;
	}
	/*
	 * Set up address spaces
	 */
	ret = setup_addr_spaces(lcd, cxt, m_init_link_addr, 
				m_core_link_addr);
	if (ret) {
		LIBLCD_ERR("error setting up address spaces");
		goto fail4;
	}
	/*
	 * Configure initial control registers, etc. for LCD
	 */

	/*
	 * Return context
	 */
	*cxt_out = cxt;
	*lcd_out = lcd;

	return 0;

fail5:
fail4:
	lcd_cap_delete(lcd);
fail3:
	lcd_module_release(m_init_bits, m_core_bits);
fail2:
	destroy_create_ctx(ctx);
fail1:
	return ret;
}
