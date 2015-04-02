/**
 * boot.c - non-isolated kernel module
 *
 */

#include <lcd-domains/kliblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int do_call_endpoint(cptr_t lcd)
{
	cptr_t call_endpoint;
	int ret;
	/*
	 * Create an endpoint for the lcd, for its call endpoint
	 */
	ret = lcd_create_sync_endpoint(&call_endpoint);
	if (ret) {
		LIBLCD_ERR("failed to create call endpoint");
		goto fail1;
	}
	/*
	 * Grant access to call endpoint in reserved slot
	 */
	ret = lcd_cap_grant(lcd, call_endpoint, LCD_CPTR_CALL_ENDPOINT);
	if (ret) {
		LIBLCD_ERR("failed to grant call endpoint");
		goto fail2;
	}

	ret = 0;

	goto out;
out:
fail2:
	/*
	 * Delete cap to call endpoint, lcd becomes sole owner (or if 
	 * we failed, endpoint gets freed)
	 */
	lcd_cap_delete(call_endpoint);
fail1:
	return ret;
}

static int do_boot_params(cptr_t lcd, struct lcd_info *mi)
{
	struct lcd_boot_info *bi;
	struct lcd_page_info_list_elem *e;
	size_t info_align;
	unsigned long addr;
	struct list_head *cursor;
	unsigned idx;
	size_t alloc_count;

	alloc_count = sizeof(struct lcd_boot_info);

	bi = (struct lcd_boot_info *)mi->boot_page_base;
	/*
	 * Dump cptr cache info in boot page
	 */
	memcpy(bi->bmap0, mi->cache->bmaps[0], sizeof(bi->bmap0));
	memcpy(bi->bmap1, mi->cache->bmaps[1], sizeof(bi->bmap1));
	memcpy(bi->bmap2, mi->cache->bmaps[2], sizeof(bi->bmap2));
	memcpy(bi->bmap3, mi->cache->bmaps[3], sizeof(bi->bmap3));
	/*
	 * Dump page info's --------------------
	 *
	 * Need to properly align first, just in case
	 */
	info_align = __alignof__(struct lcd_boot_info_for_page);
	BUILD_BUG_ON(info_align & (info_align - 1)); /* not a power of 2 */

	addr = (unsigned long)(bi + 1);
	info_align--;
	bi->boot_mem_pi_start = (struct lcd_boot_info_for_page *)
		((addr + info_align) & ~info_align);
	/*
	 * Stack in boot page infos
	 */
	idx = 0;
	list_for_each(cursor, &mi->boot_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		bi->boot_mem_pi_start[idx].my_cptr = e->my_cptr;
		bi->boot_mem_pi_start[idx].page_gpa = e->page_gpa;
		LIBLCD_MSG("boot page gpa 0x%lx", gpa_val(e->page_gpa));
		idx++;
		alloc_count += sizeof(struct lcd_boot_info_for_page);
	}
	bi->num_boot_mem_pi = idx;

	LIBLCD_MSG("%d boot pages", idx);

	bi->paging_mem_pi_start = &bi->boot_mem_pi_start[idx];
	idx = 0;
	list_for_each(cursor, &mi->paging_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		bi->paging_mem_pi_start[idx].my_cptr = e->my_cptr;
		bi->paging_mem_pi_start[idx].page_gpa = e->page_gpa;
		LIBLCD_MSG("paging mem gpa 0x%lx", gpa_val(e->page_gpa));
		idx++;
		alloc_count += sizeof(struct lcd_boot_info_for_page);
	}
	bi->num_paging_mem_pi = idx;

	LIBLCD_MSG("%d paging mem pages", idx);

	bi->free_mem_pi_start = &bi->paging_mem_pi_start[idx];
	idx = 0;
	list_for_each(cursor, &mi->free_mem_list) {
		e = list_entry(cursor, struct lcd_page_info_list_elem, list);
		bi->free_mem_pi_start[idx].my_cptr = e->my_cptr;
		bi->free_mem_pi_start[idx].page_gpa = e->page_gpa;
		LIBLCD_MSG("free mem gpa 0x%lx", gpa_val(e->page_gpa));
		idx++;
		alloc_count += sizeof(struct lcd_boot_info_for_page);
	}
	bi->num_free_mem_pi = idx;

	LIBLCD_MSG("%d free mem pages", idx);

	if (alloc_count >= ((1 << LCD_BOOT_PAGES_ORDER) << PAGE_SHIFT)) {
		LIBLCD_ERR("uh oh ... we wrote past the end of the page");
		return -EIO;
	}

	/*
	 * Adjust addresses for lcd address space
	 */
	bi->boot_mem_pi_start = (struct lcd_boot_info_for_page *)
		(gva_val(LCD_BOOT_PAGES_GVA) +
			(unsigned long)(((char*)bi->boot_mem_pi_start) -
					mi->boot_page_base));
	bi->paging_mem_pi_start = (struct lcd_boot_info_for_page *)
		(gva_val(LCD_BOOT_PAGES_GVA) +
			(unsigned long)(((char*)bi->paging_mem_pi_start) -
					mi->boot_page_base));
	bi->free_mem_pi_start = (struct lcd_boot_info_for_page *)
		(gva_val(LCD_BOOT_PAGES_GVA) +
			(unsigned long)(((char*)(bi->free_mem_pi_start)) - 
					mi->boot_page_base));

	return 0;
}

static int boot_main(void)
{
	int ret;
	cptr_t lcd;
	struct lcd_info *mi;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(&lcd, "lcd_test_mod_mm_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail2;
	}
	/*
	 * Set up call endpoint
	 */
	ret = do_call_endpoint(lcd);
	if (ret) {
		LIBLCD_ERR("failed setup call endpoint");
		goto fail3;
	}
	/*
	 * Set up boot info
	 */
	ret = do_boot_params(lcd, mi);
	if (ret) {
		LIBLCD_ERR("failed to set up boot params");
		goto fail4;
	}
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail5;
	}
	/*
	 * Hang out for four seconds
	 */
	msleep(4000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail5:
fail4:
fail3:
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
