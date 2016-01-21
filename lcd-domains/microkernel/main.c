/**
 * main.c - Main file for the LCD module
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
#include <linux/sysrq.h>

#include <asm/lcd-domains.h>
#include <lcd-domains/types.h>
#include <lcd-domains/syscall.h>
#include "internal.h"
#include <uapi/lcd-domains.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* QUICK LCD LOOKUP / LOCK -------------------------------------------------- */

/* static int lookup_lcd(struct cspace *cspace, cptr_t slot, struct cnode **cnode) */
/* { */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up */
/* 	 *\/ */
/* 	ret = __lcd_cnode_get(cspace, slot, cnode); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * Confirm it's an lcd or klcd */
/* 	 *\/ */
/* 	if ((*cnode)->type != LCD_CAP_TYPE_LCD && */
/* 		(*cnode)->type != LCD_CAP_TYPE_KLCD) { */
/* 		LCD_ERR("not an lcd"); */
/* 		goto fail2; */
/* 	} */

/* 	return 0; */

/* fail2: */
/* 	__lcd_cnode_put(*cnode); */
/* fail1: */
/* 	return ret; */
/* } */

/* static int get_lcd(struct cspace *cspace, cptr_t lcd, struct cnode **cnode, */
/* 		struct lcd **lcd_struct) */
/* { */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock cnode containing lcd */
/* 	 *\/ */
/* 	ret = lookup_lcd(cspace, lcd, cnode); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	*lcd_struct = (*cnode)->object; */
/* 	/\* */
/* 	 * Lock lcd_struct */
/* 	 *\/ */
/* 	ret = mutex_lock_interruptible(&(*lcd_struct)->lock); */
/* 	if (ret) { */
/* 		LCD_ERR("interrupted"); */
/* 		goto fail2; */
/* 	} */
	
/* 	return 0; /\* caller should match with put_lcd *\/ */

/* fail2: */
/* 	__lcd_cnode_put(*cnode); */
/* fail1: */
/* 	return ret; */
/* } */

/* static void put_lcd(struct cnode *cnode, struct lcd *lcd) */
/* { */
/* 	/\* */
/* 	 * Unlock lcd */
/* 	 *\/ */
/* 	mutex_unlock(&lcd->lock); */
/* 	/\* */
/* 	 * Release cnode */
/* 	 *\/ */
/* 	__lcd_cnode_put(cnode); */
/* } */

/* static int lookup_page(struct cspace *cspace, cptr_t slot, struct cnode **cnode) */
/* { */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up */
/* 	 *\/ */
/* 	ret = __lcd_cnode_get(cspace, slot, cnode); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * Confirm it's an lcd */
/* 	 *\/ */
/* 	if ((*cnode)->type != LCD_CAP_TYPE_PAGE && */
/* 		(*cnode)->type != LCD_CAP_TYPE_KPAGE) { */
/* 		LCD_ERR("not an lcd"); */
/* 		goto fail2; */
/* 	} */

/* 	return 0; */

/* fail2: */
/* 	__lcd_cnode_put(*cnode); */
/* fail1: */
/* 	return ret; */
/* } */

/* static int lcd_get_page(struct cspace *cspace, cptr_t page,  */
/* 			struct cnode **cnode, struct page **page_struct) */
/* { */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock cnode containing page */
/* 	 *\/ */
/* 	ret = lookup_page(cspace, page, cnode); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	*page_struct = (*cnode)->object; */
/* 	/\* */
/* 	 * XXX: for now, we assume we won't be touching any of the page */
/* 	 * fields - we just look up for mapping / unmapping etc. */
/* 	 *\/ */
	
/* 	return 0; /\* caller should match with put_page *\/ */

/* fail1: */
/* 	return ret; */
/* } */

/* static void lcd_put_page(struct cnode *cnode, struct page *page) */
/* { */
/* 	/\* */
/* 	 * (no unlock on page - see get_page) */
/* 	 * */
/* 	 * Release cnode */
/* 	 *\/ */
/* 	__lcd_cnode_put(cnode); */
/* } */


/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */


int __klcd_page_zalloc(struct lcd *klcd, cptr_t c, hpa_t *hpa_out,
			       hva_t *hva_out)
{
	int ret;
	struct page *p;
	/*
	 * Allocate a page
	 */
	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(page_address(p), 0, PAGE_SIZE);
	/*
	 * Insert into klcd's cspace
	 */
	ret = __lcd_cap_insert(&klcd->cspace, c, p, LCD_CAP_TYPE_PAGE);
	if (ret) {
		LCD_ERR("insert");
		goto fail2;
	}
	/*
	 * Store addresses
	 */
	*hva_out = va2hva(page_address(p));
	*hpa_out = hva2hpa(*hva_out);

	return 0;

fail2:
	__free_pages(p, 0);
fail1:
	return ret;
}

int __klcd_pages_zalloc(struct lcd *klcd, cptr_t *slots, 
			hpa_t *hp_base_out, hva_t *hv_base_out,
			unsigned order)
{
	int ret;
	struct page *p;
	int i, j;
	/*
	 * Allocate a page
	 */
	p = alloc_pages(GFP_KERNEL, order);
	if (!p) {
		LCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(page_address(p), 0, (1 << order) * PAGE_SIZE);
	/*
	 * Insert them into klcd's cspace
	 */
	for (i = 0; i < (1 << order); i++) {
		ret = __lcd_cap_insert(&klcd->cspace, slots[i], 
				p + i, LCD_CAP_TYPE_PAGE);
		if (ret) {
			LCD_ERR("insert");
			goto fail2;
		}
	}
	/*
	 * Store addresses
	 */
	*hv_base_out = va2hva(page_address(p));
	*hp_base_out = hva2hpa(*hv_base_out);

	return 0;

fail2:
	for (j = 0; j < i; j++)
		__lcd_cap_delete(&klcd->cspace, slots[i]);
	__free_pages(p, order);
fail1:
	return ret;
}

/* IPC -------------------------------------------------- */

/* in ipc.c */


/* LCD CREATE / CONFIG -------------------------------------------------- */


/* static int gp_map(struct lcd *lcd, gpa_t gpa, hpa_t hpa) */
/* { */
/* 	/\*  */
/* 	 * Create, and do not overwrite */
/* 	 *\/ */
/* 	return lcd_arch_ept_map(lcd->lcd_arch, gpa, hpa, 1, 0); */
/* } */

/* static void gp_unmap(struct lcd *lcd, gpa_t gpa) */
/* { */
/* 	int ret; */
/* 	ret = lcd_arch_ept_unmap(lcd->lcd_arch, gpa); */
/* 	if (ret) */
/* 		LCD_ERR("some error unmapping"); */
/* } */

/* int __lcd_create__(struct lcd **out) */
/* { */
/* 	struct lcd *lcd; */
/* 	int ret; */
/* 	/\* */
/* 	 * Alloc lcd data structure */
/* 	 * */
/* 	 * (Because we're doing a zalloc, this will set the type, status, */
/* 	 * and so on to "defaults".) */
/* 	 *\/ */
/* 	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL); */
/* 	if (!lcd) { */
/* 		LCD_ERR("error alloc'ing lcd"); */
/* 		ret = -ENOMEM; */
/* 		goto fail1; */
/* 	} */
/* 	/\* */
/* 	 * Init cspace */
/* 	 *\/ */
/* 	ret = __lcd_cap_init_cspace(&lcd->cspace); */
/* 	if (ret) { */
/* 		LCD_ERR("set up cspace"); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Init mutex */
/* 	 *\/ */
/* 	mutex_init(&lcd->lock); */
/* 	/\* */
/* 	 * Initialize send/recv queue list element */
/* 	 *\/ */
/* 	INIT_LIST_HEAD(&lcd->endpoint_queue); */
/* 	/\* */
/* 	 * Initialize console cursor */
/* 	 *\/ */
/* 	lcd->console_cursor = 0; */

/* 	*out = lcd; */

/* 	return 0; */

/* fail2: */
/* 	kfree(lcd); */
/* fail1: */
/* 	return ret; */
/* } */

/* static int lcd_kthread_main(void *data); */

/* int __lcd_create(struct lcd *caller, cptr_t slot) */
/* { */
/* 	struct lcd *lcd; */
/* 	hva_t addr; */
/* 	int ret; */
/* 	/\* */
/* 	 * Basic init of lcd */
/* 	 *\/ */
/* 	ret = __lcd_create__(&lcd); */
/* 	if (ret) { */
/* 		LCD_ERR("lcd create"); */
/* 		goto fail1; */
/* 	} */
/* 	/\* */
/* 	 * Alloc arch-dependent part */
/* 	 *\/ */
/* 	ret = lcd_arch_create(&lcd->lcd_arch); */
/* 	if(ret) { */
/* 		LCD_ERR("error creating lcd_arch"); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Allocate a page for the utcb */
/* 	 *\/ */
/* 	addr = __hva(get_zeroed_page(GFP_KERNEL)); */
/* 	if (!hva_val(addr)) { */
/* 		LCD_ERR("alloc page"); */
/* 		goto fail3; */
/* 	} */
/* 	/\* */
/* 	 * Store ref in lcd struct's utcb */
/* 	 *\/ */
/* 	lcd->utcb = hva2va(addr); */
/* 	/\* */
/* 	 * Create a kernel thread (won't run till we wake it up) */
/* 	 *\/ */
/* 	lcd->kthread = kthread_create(lcd_kthread_main, NULL, "lcd"); */
/* 	if (!lcd->kthread) { */
/* 		LCD_ERR("failed to create kthread"); */
/* 		goto fail4; */
/* 	} */
/* 	/\* */
/* 	 * Bump reference count on kthread */
/* 	 *\/ */
/* 	get_task_struct(lcd->kthread); */
/* 	/\* */
/* 	 * Store back reference to lcd */
/* 	 *\/ */
/* 	lcd->kthread->lcd = lcd; */
/* 	/\* */
/* 	 * Put in caller's cspace */
/* 	 *\/ */
/* 	ret = __lcd_cap_insert(&caller->cspace, slot, lcd, LCD_CAP_TYPE_LCD); */
/* 	if (ret) { */
/* 		LCD_ERR("insert"); */
/* 		goto fail5; */
/* 	} */
/* 	/\* */
/* 	 * Done */
/* 	 *\/ */
/* 	return 0; */

/* fail5: */
/* 	kthread_stop(lcd->kthread); /\* never ran, so should die *\/ */
/* 	put_task_struct(lcd->kthread); */
/* fail4: */
/* 	free_page(hva_val(addr)); */
/* fail3: */
/* 	lcd_arch_destroy(lcd->lcd_arch); */
/* fail2: */
/* 	__lcd_destroy__(lcd); */
/* fail1: */
/* 	return ret; */
/* } */

/* static int __lcd_config_lcd(struct lcd *caller, struct lcd *lcd_struct,  */
/* 			gva_t pc, gva_t sp,  */
/* 			gpa_t gva_root, gpa_t utcb_page) */
/* { */
/* 	int ret; */
/* 	hva_t utcb_page_addr; */
/* 	/\* */
/* 	 * If lcd is not an embryo, fail */
/* 	 *\/ */
/* 	if (!lcd_status_embryo(lcd_struct)) { */
/* 		LCD_ERR("cannot config: lcd not an embryo"); */
/* 		ret = -EINVAL; */
/* 		goto fail1; */
/* 	} */
/* 	/\* */
/* 	 * Set pc, sp, gva_root */
/* 	 *\/ */
/* 	lcd_arch_set_pc(lcd_struct->lcd_arch, pc); */
/* 	lcd_arch_set_sp(lcd_struct->lcd_arch, sp); */
/* 	lcd_arch_set_gva_root(lcd_struct->lcd_arch, gva_root); */
/* 	/\* */
/* 	 * Map utcb page in guest physical */
/* 	 *\/ */
/* 	utcb_page_addr = va2hva(lcd_struct->utcb); */
/* 	ret = gp_map(lcd_struct, utcb_page, hva2hpa(utcb_page_addr)); */
/* 	if (ret) { */
/* 		LCD_ERR("map"); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Make sure lcd_arch has valid state */
/* 	 *\/ */
/* 	ret = lcd_arch_check(lcd_struct->lcd_arch); */
/* 	if (ret) { */
/* 		LCD_ERR("bad lcd_arch state"); */
/* 		goto fail3; */
/* 	} */

/* 	return 0; */

/* fail3: */
/* 	gp_unmap(lcd_struct, utcb_page); */
/* fail2: */
/* fail1: */
/* 	return ret; */
/* } */

/* int __lcd_config_klcd(struct lcd *caller, struct lcd *lcd_struct,  */
/* 			gva_t pc, gva_t sp,  */
/* 			gpa_t gva_root, gpa_t utcb_page) */
/* { */
/* 	/\* */
/* 	 * For now, we ignore everything except the program counter. */
/* 	 * */
/* 	 * The program counter is assumed to be a host virtual */
/* 	 * address to a kernel module's init. */
/* 	 * */
/* 	 * (Recall that for klcd's, guest addresses = host addresses.) */
/* 	 *\/ */
/* 	lcd_struct->klcd_main = hva2va(__hva(gva_val(pc))); */

/* 	return 0; */
/* } */

/* int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp,  */
/* 		gpa_t gva_root, gpa_t utcb_page) */
/* { */
/* 	struct lcd *lcd_struct; */
/* 	struct cnode *cnode; */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock */
/* 	 *\/ */
/* 	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * Switch on the type of the lcd (a regular lcd, klcd, ...) */
/* 	 *\/ */
/* 	switch (cnode->type) { */

/* 	case LCD_CAP_TYPE_LCD: */
/* 		ret = __lcd_config_lcd(caller, lcd_struct, pc, sp, gva_root,  */
/* 				utcb_page); */
/* 		break; */
/* 	case LCD_CAP_TYPE_KLCD: */
/* 		ret = __lcd_config_klcd(caller, lcd_struct, pc, sp, gva_root,  */
/* 					utcb_page); */
/* 		break; */
/* 	default: */
/* 		/\* shouldn't happen *\/ */
/* 		LCD_ERR("unexpected cnode type: %d", */
/* 			cnode->type); */
/* 		goto fail2;		 */
/* 	} */
/* 	if (ret) { */
/* 		LCD_ERR("error config'ing lcd, ret = %d", ret); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Set status to configed */
/* 	 *\/ */
/* 	set_lcd_status(lcd_struct, LCD_STATUS_CONFIGED); */

/* 	ret = 0; */
/* 	goto out; */

/* out: */
/* fail2: */
/* 	/\* */
/* 	 * Unlock */
/* 	 *\/ */
/* 	put_lcd(cnode, lcd_struct); */
/* fail1: */
/* 	return ret; */
/* } */

/* int __lcd_run(struct lcd *caller, cptr_t lcd) */
/* { */
/* 	struct lcd *lcd_struct; */
/* 	struct cnode *cnode; */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock */
/* 	 *\/ */
/* 	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * If lcd is not in config state, fail */
/* 	 *\/ */
/* 	if (!lcd_status_configed(lcd_struct)) { */
/* 		LCD_ERR("cannot run: lcd is in state %d", */
/* 			get_lcd_status(lcd_struct)); */
/* 		ret = -EINVAL; */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * this will run the kthread for the first time */
/* 	 *\/ */
/* 	set_lcd_status(lcd_struct, LCD_STATUS_RUNNING); */
/* 	wake_up_process(lcd_struct->kthread); */
/* 	/\* */
/* 	 * Unlock */
/* 	 *\/ */
/* 	put_lcd(cnode, lcd_struct); */

/* 	return 0; */

/* fail2: */
/* 	put_lcd(cnode, lcd_struct); */
/* fail1: */
/* 	return ret; */
/* } */

/* KLCD ENTER / EXIT -------------------------------------------------- */

int __klcd_enter(void)
{
	/*
	 * A klcd doesn't have an underlying hardware vm.
	 *
	 * It has everything else though - cspace, utcb, etc.
	 *
	 * (kliblcd sets up the thread's cptr cache)
	 */
	struct lcd *lcd;
	int ret;
	/*
	 * Bare init
	 */
	ret = __lcd_create__(&lcd);
	if (ret) {
		LCD_ERR("lcd create error");
		goto fail1;
	}
	/*
	 * Allocate a page for the thread's utcb
	 */
	lcd->utcb = hva2va(__hva(get_zeroed_page(GFP_KERNEL)));
	if (!lcd->utcb) {
		LCD_ERR("utcb alloc");
		goto fail2;
	}
	/*
	 * Store in calling thread's lcd field
	 */
	current->lcd = lcd;
	/*
	 * Store reference to calling thread in lcd
	 */
	lcd->kthread = current;
	/*
	 * Mark lcd as running
	 */
	set_lcd_status(lcd, LCD_STATUS_RUNNING);
	/*
	 * Set type as "top" - a "root" LCD owned by no one (gasp!)
	 */
	lcd->type = LCD_TYPE_TOP;

	return 0;

fail2:
	__lcd_destroy__(lcd);
fail1:
	return ret;
}

void __klcd_exit(void)
{
	struct lcd *lcd;

	lcd = current->lcd;
	
	if (lcd->type != LCD_TYPE_TOP) {
		/*
		 * The caller is a klcd that was created by someone
		 * else. We will let their creator destroy them (when
		 * they delete the capability to the klcd).
		 */
		return;
	}

	BUG_ON(lcd_status_dead(lcd)); /* lcd shouldn't be dead already */

	/*
	 * Like lcd destroy, but simpler.
	 *
	 * Since the caller is the thread itself, we can assume it is
	 * not in an endpoint queue.
	 *
	 * We still use some care and mark the lcd as dead (even though
	 * the thread itself isn't going away), so that endpoints handle
	 * the lcd properly, etc.
	 */
	if (mutex_lock_interruptible(&lcd->lock)) {
		/*
		 * This is probably not ideal
		 */
		LCD_ERR("interrupted, continuing with lcd stop (gulp) ...");
		set_lcd_status(lcd, LCD_STATUS_DEAD);
		goto lock_skip;
	}
	
	set_lcd_status(lcd, LCD_STATUS_DEAD);
	
	mutex_unlock(&lcd->lock);

lock_skip:
	/*
	 * Tear down cspace.
	 */
	__lcd_cap_destroy_cspace(&lcd->cspace);
	/*
	 * Free the stack page
	 */
	free_page((unsigned long)lcd->utcb);
	/*
	 * Finish
	 */
	kfree(lcd);
	/*
	 * Null it out
	 */
	current->lcd = NULL;
}

/* CREATE A KLCD (NON-ISOLATED LCD) ------------------------------ */

/* int __klcd_create_klcd(struct lcd *caller, cptr_t slot) */
/* { */
/* 	struct lcd *lcd; */
/* 	hva_t addr; */
/* 	int ret; */
/* 	/\* */
/* 	 * Basic init of lcd */
/* 	 *\/ */
/* 	ret = __lcd_create__(&lcd); */
/* 	if (ret) { */
/* 		LCD_ERR("lcd create"); */
/* 		goto fail1; */
/* 	} */
/* 	/\* */
/* 	 * Allocate a page for the stack/utcb */
/* 	 *\/ */
/* 	addr = __hva(get_zeroed_page(GFP_KERNEL)); */
/* 	if (!hva_val(addr)) { */
/* 		LCD_ERR("alloc page"); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Store ref in lcd struct's utcb */
/* 	 *\/ */
/* 	lcd->utcb = hva2va(addr); */
/* 	/\* */
/* 	 * Create a kernel thread (won't run till we wake it up) */
/* 	 *\/ */
/* 	lcd->kthread = kthread_create(lcd_kthread_main, NULL, "lcd"); */
/* 	if (!lcd->kthread) { */
/* 		LCD_ERR("failed to create kthread"); */
/* 		goto fail3; */
/* 	} */
/* 	/\* */
/* 	 * Bump reference count on kthread */
/* 	 *\/ */
/* 	get_task_struct(lcd->kthread); */
/* 	/\* */
/* 	 * Store back reference to lcd */
/* 	 *\/ */
/* 	lcd->kthread->lcd = lcd; */
/* 	/\* */
/* 	 * Set type as non-isolated */
/* 	 *\/ */
/* 	lcd->type = LCD_TYPE_NONISOLATED; */
/* 	/\* */
/* 	 * Put in caller's cspace */
/* 	 *\/ */
/* 	ret = __lcd_cap_insert(&caller->cspace, slot, lcd,  */
/* 			LCD_CAP_TYPE_KLCD); */
/* 	if (ret) { */
/* 		LCD_ERR("insert"); */
/* 		goto fail4; */
/* 	} */
/* 	/\* */
/* 	 * Done */
/* 	 *\/ */
/* 	return 0; */

/* fail4: */
/* 	kthread_stop(lcd->kthread); /\* never ran, so should die *\/ */
/* 	put_task_struct(lcd->kthread); */
/* fail3: */
/* 	free_page(hva_val(addr)); */
/* fail2: */
/* 	__lcd_destroy__(lcd); */
/* fail1: */
/* 	return ret; */
/* } */


/* CAPABILITIES -------------------------------------------------- */

/* int __lcd_cap_grant_cheat(struct lcd *caller, cptr_t lcd, cptr_t src,  */
/* 			cptr_t dest) */
/* { */
/* 	struct lcd *lcd_struct; */
/* 	struct cnode *cnode; */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock */
/* 	 *\/ */
/* 	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * If lcd is not an embryo or in config'd state, fail - we only allow  */
/* 	 * direct grants when the lcd is being set up */
/* 	 *\/ */
/* 	if (!lcd_status_embryo(lcd_struct) && */
/* 		!lcd_status_configed(lcd_struct)) { */
/* 		LCD_ERR("lcd is not an embryo or configd"); */
/* 		ret = -EINVAL; */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Do regular grant */
/* 	 *\/ */
/* 	ret = __lcd_cap_grant(&caller->cspace, src, */
/* 			&lcd_struct->cspace, dest); */
/* 	if (ret) */
/* 		goto fail3; */
/* 	/\* */
/* 	 * Put */
/* 	 *\/ */
/* 	put_lcd(cnode, lcd_struct); */

/* 	return 0; */

/* fail3: */
/* fail2: */
/* 	put_lcd(cnode, lcd_struct); */
/* fail1: */
/* 	return ret; */
/* } */

/* int __lcd_cap_page_grant_map_cheat(struct lcd *caller, cptr_t lcd,  */
/* 				cptr_t page, cptr_t dest, gpa_t gpa) */
/* { */
/* 	struct lcd *lcd_struct; */
/* 	struct page *page_struct; */
/* 	struct cnode *lcd_cnode; */
/* 	struct cnode *page_cnode; */
/* 	int ret; */
/* 	/\* */
/* 	 * Look up and lock lcd and page, in that order (gulp) */
/* 	 *\/ */
/* 	ret = get_lcd(&caller->cspace, lcd, &lcd_cnode, &lcd_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * If lcd is not an embryo, fail - we only allow direct grants when */
/* 	 * the lcd is being set up */
/* 	 *\/ */
/* 	if (!lcd_status_embryo(lcd_struct)) { */
/* 		LCD_ERR("lcd is not an embryo"); */
/* 		ret = -EINVAL; */
/* 		goto fail2; */
/* 	} */
/* 	ret = lcd_get_page(&caller->cspace, page, &page_cnode, &page_struct); */
/* 	if (ret) */
/* 		goto fail3; */
/* 	/\* */
/* 	 * Map page */
/* 	 *\/ */
/* 	ret = gp_map(lcd_struct, gpa, va2hpa(page_address(page_struct))); */
/* 	if (ret) { */
/* 		LCD_ERR("map"); */
/* 		lcd_put_page(page_cnode, page_struct); */
/* 		goto fail4; */
/* 	} */
/* 	/\* */
/* 	 * Release page cnode, so that grant can go through */
/* 	 *\/ */
/* 	__lcd_cnode_put(page_cnode); */
/* 	/\* */
/* 	 * Do regular grant */
/* 	 *\/ */
/* 	ret = __lcd_cap_grant_page(&caller->cspace, page, */
/* 				&lcd_struct->cspace, dest, gpa); */
/* 	if (ret) */
/* 		goto fail5; */
/* 	/\* */
/* 	 * Put lcd */
/* 	 *\/ */
/* 	put_lcd(lcd_cnode, lcd_struct); */

/* 	return 0; */

/* fail5: */
/* 	gp_unmap(lcd_struct, gpa); */
/* fail4: */
/* fail3: */
/* fail2: */
/* 	put_lcd(lcd_cnode, lcd_struct); */
/* fail1: */
/* 	return ret; */
/* } */

/* rest is in cap.c */


/* END OF INTERFACE STUFF -------------------------------------------------- */


/* CHECKS -------------------------------------------------- */


/* void __lcd_page_check(struct lcd *lcd, struct page *p, int is_mapped,  */
/* 		gpa_t where_mapped) */
/* { */
/* 	/\* */
/* 	 * If it's not mapped, nothing to do */
/* 	 *\/ */
/* 	if (!is_mapped) */
/* 		return; */
/* 	/\* */
/* 	 * Unmap it */
/* 	 *\/ */
/* 	gp_unmap(lcd, where_mapped); */
/* } */

/* void __lcd_kpage_check(struct lcd *lcd, struct page *p, int is_mapped,  */
/* 		gpa_t where_mapped) */
/* { */
/* 	__lcd_page_check(lcd, p, is_mapped, where_mapped); */
/* } */

/* void __lcd_check(struct lcd *owning_lcd, struct lcd *owned_lcd) */
/* { */
/* 	/\* no-op for now *\/ */
/* 	return; */
/* } */

/* void __lcd_check_klcd(struct lcd *owning_lcd, struct lcd *owned_klcd) */
/* { */
/* 	/\* no-op for now *\/ */
/* 	return; */
/* } */

/* /\* endpoint check in ipc.c *\/ */


/* /\* DESTROY -------------------------------------------------- *\/ */


/* void __lcd_page_destroy(struct page *p) */
/* { */
/* 	/\* just free the page - it's unmapped everywhere *\/ */
/* 	free_page((unsigned long)page_address(p)); */
/* } */

/* void __lcd_kpage_destroy(struct page *p) */
/* { */
/* 	/\* do nothing - klcd is responsible for freeing it *\/ */
/* 	return; */
/* } */

/* /\* */
/*  * For lcd's with no underlying vm or kernel thread */
/*  *\/ */
/* void __lcd_destroy__(struct lcd *lcd) */
/* { */
/* 	/\* */
/* 	 * Tear down cspace. */
/* 	 *\/ */
/* 	__lcd_cap_destroy_cspace(&lcd->cspace); */
/* 	kfree(lcd); */
/* } */

/* /\* */
/*  * For lcd's with no underlying vm */
/*  *\/ */
/* void __lcd_destroy_do_non_arch(struct lcd *lcd) */
/* { */
/* 	int ret; */

/* 	BUG_ON(lcd_status_dead(lcd)); /\* lcd shouldn't be dead already *\/ */

/* 	/\* */
/* 	 * ORDER IS IMPORTANT: */
/* 	 * */
/* 	 * (0) Stop the kthread and set the LCD's status. This stops the */
/* 	 *     lcd and prevents endpoints from trying to wake up the kthread. */
/* 	 * */
/* 	 *     (Alternative: Just start tearing down the cspace and let */
/* 	 *     ipc's just fail in the lcd. After that, stop the lcd, since */
/* 	 *     it can't be in any endpoint queues.) */
/* 	 * */
/* 	 * (1) Tear down the lcd's cspace. This will free any objects */
/* 	 *     for which the lcd has the last capability (like pages). Pages */
/* 	 *     will automatically be unmapped as their caps are deleted (via */
/* 	 *     __lcd_page_check). */
/* 	 * */
/* 	 * (2) Tear down the lcd arch. (This *must* come after (1), because */
/* 	 *     the cspace tear down may lead to unmapping pages in the lcd's */
/* 	 *     ept, which requires the lcd arch.) */
/* 	 * */
/* 	 * Mark the lcd as dead */
/* 	 *\/ */
/* 	if (mutex_lock_interruptible(&lcd->lock)) { */
/* 		/\* */
/* 		 * This is probably not ideal */
/* 		 *\/ */
/* 		LCD_ERR("interrupted, continuing with lcd stop (gulp) ..."); */
/* 		set_lcd_status(lcd, LCD_STATUS_DEAD); */
/* 		goto lock_skip; */
/* 	} */
	
/* 	set_lcd_status(lcd, LCD_STATUS_DEAD); */
	
/* 	mutex_unlock(&lcd->lock); */

/* lock_skip: */
/* 	/\* */
/* 	 * Now any endpoints that still have the lcd in the queue will */
/* 	 * check and see that it is dead before trying to wake up the */
/* 	 * kthread. */
/* 	 * */
/* 	 * We now stop the kthread. */
/* 	 * */
/* 	 * XXX: it would be nice to pass the return value to someone; for */
/* 	 * now, we just print it out if it's non-zero, and then drop it */
/* 	 *\/ */
/* 	ret = kthread_stop(lcd->kthread); */
/* 	if (ret) */
/* 		LCD_ERR("got non-zero exit status %d from lcd %p", */
/* 			ret, lcd); */
/* 	/\* */
/* 	 * Decrement the kthread's reference count. Host kernel will clean */
/* 	 * up the rest. */
/* 	 *\/ */
/* 	put_task_struct(lcd->kthread); */
/* 	/\* */
/* 	 * Free the stack page */
/* 	 *\/ */
/* 	free_page((unsigned long)lcd->utcb); */
/* 	/\* */
/* 	 * Tear down cspace. */
/* 	 * */
/* 	 * We null out some stuff afterward so that if anyone tries to */
/* 	 * use these fields, they'll get a NULL deref (easier to debug than */
/* 	 * use after free). */
/* 	 *\/ */
/* 	__lcd_cap_destroy_cspace(&lcd->cspace); */
/* 	lcd->kthread = NULL; */
/* 	lcd->utcb = NULL; */
/* } */

/* void __lcd_destroy(struct lcd *lcd) */
/* { */
/* 	/\* */
/* 	 * Do bulk of it */
/* 	 *\/ */
/* 	__lcd_destroy_do_non_arch(lcd); */
/* 	/\* */
/* 	 * Tear down lcd_arch (ept, ...) */
/* 	 *\/ */
/* 	lcd_arch_destroy(lcd->lcd_arch); */
/* 	lcd->lcd_arch = NULL; */
/* 	/\* */
/* 	 * Free lcd data structure */
/* 	 *\/ */
/* 	kfree(lcd); */
/* } */

/* void __lcd_destroy_klcd(struct lcd *owned_klcd) */
/* { */
/* 	/\* */
/* 	 * Do bulk of it */
/* 	 *\/ */
/* 	__lcd_destroy_do_non_arch(owned_klcd); */
/* 	/\* */
/* 	 * Free lcd data structure */
/* 	 *\/ */
/* 	kfree(owned_klcd); */
/* } */

/* LCD EXECUTION -------------------------------------------------- */

/* static int __lcd_put_char(struct lcd *lcd) */
/* { */
/* 	char c; */
/* 	/\* */
/* 	 * Char is in r0 */
/* 	 *\/ */
/* 	c = __lcd_debug_reg(lcd->utcb); */
/* 	/\* */
/* 	 * Put char in lcd's console buff */
/* 	 *\/ */
/* 	lcd->console_buff[lcd->console_cursor] = c; */
/* 	/\* */
/* 	 * Bump cursor and check */
/* 	 *\/ */
/* 	lcd->console_cursor++; */
/* 	if (c == 0) { */
/* 		/\* */
/* 		 * End of string; printk to host and reset buff */
/* 		 *\/ */
/* 		printk(KERN_INFO "message from lcd %p: %s\n", */
/* 			lcd, lcd->console_buff); */
/* 		lcd->console_cursor = 0; */
/* 		return 0; */
/* 	} */
/* 	if (lcd->console_cursor >= LCD_CONSOLE_BUFF_SIZE - 1) { */
/* 		/\* */
/* 		 * Filled buffer; empty it. */
/* 		 *\/ */
/* 		lcd->console_buff[LCD_CONSOLE_BUFF_SIZE - 1] = 0; */
/* 		printk(KERN_INFO "(incomplete) message from lcd %p: %s\n", */
/* 			lcd, lcd->console_buff); */
/* 		lcd->console_cursor = 0; */
/* 		return 0; */
/* 	} */
/* 	return 0; */
/* } */

/* static int do_page_alloc(struct lcd *lcd) */
/* { */
/* 	int ret; */
/* 	cptr_t c; */
/* 	struct page *p; */
/* 	/\* */
/* 	 * Get dest slot */
/* 	 *\/ */
/* 	c = __lcd_cr0(lcd->utcb); */
/* 	/\* */
/* 	 * Allocate a zero'd page */
/* 	 *\/ */
/* 	p = alloc_page(GFP_KERNEL); */
/* 	if (!p) { */
/* 		LCD_ERR("alloc failed"); */
/* 		ret = -ENOMEM; */
/* 		goto fail1; */
/* 	} */
/* 	memset(page_address(p), 0, PAGE_SIZE); */
/* 	/\* */
/* 	 * Insert into lcd's cspace */
/* 	 *\/ */
/* 	ret = __lcd_cap_insert(&lcd->cspace, c, p, LCD_CAP_TYPE_PAGE); */
/* 	if (ret) { */
/* 		LCD_ERR("insert"); */
/* 		goto fail2; */
/* 	} */

/* 	return 0; */

/* fail2: */
/* 	__free_pages(p, 0); */
/* fail1: */
/* 	return ret; */
/* } */

/* static int do_page_map(struct lcd *lcd) */
/* { */
/* 	int ret; */
/* 	cptr_t page; */
/* 	gpa_t gpa; */
/* 	struct page *page_struct; */
/* 	struct cnode *page_cnode; */
/* 	/\* */
/* 	 * Get dest to map */
/* 	 *\/ */
/* 	gpa = __gpa(__lcd_r0(lcd->utcb)); */
/* 	/\* */
/* 	 * Get page cptr */
/* 	 *\/ */
/* 	page = __lcd_cr0(lcd->utcb); */
/* 	/\* */
/* 	 * Look up page in lcd's cspace */
/* 	 *\/ */
/* 	ret = lcd_get_page(&lcd->cspace, page, &page_cnode, &page_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * If page is already mapped, fail */
/* 	 *\/ */
/* 	if (page_cnode->is_mapped) { */
/* 		LCD_ERR("page already mapped"); */
/* 		ret = -EINVAL; */
/* 		lcd_put_page(page_cnode, page_struct); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Map page */
/* 	 *\/ */
/* 	ret = gp_map(lcd, gpa, va2hpa(page_address(page_struct))); */
/* 	if (ret) { */
/* 		LCD_ERR("map"); */
/* 		lcd_put_page(page_cnode, page_struct); */
/* 		goto fail3; */
/* 	} */
/* 	/\* */
/* 	 * Mark page as mapped, and where it's mapped */
/* 	 *\/ */
/* 	page_cnode->is_mapped = 1; */
/* 	page_cnode->where_mapped = gpa; */
/* 	/\* */
/* 	 * Release page cnode, etc. */
/* 	 *\/ */
/* 	lcd_put_page(page_cnode, page_struct); */

/* 	return 0; */

/* fail3: */
/* fail2: */
/* fail1: */
/* 	return ret; */
/* } */

/* static int do_page_unmap(struct lcd *lcd) */
/* { */
/* 	int ret; */
/* 	cptr_t page; */
/* 	gpa_t gpa; */
/* 	struct page *page_struct; */
/* 	struct cnode *page_cnode; */
/* 	/\* */
/* 	 * Get dest to unmap */
/* 	 *\/ */
/* 	gpa = __gpa(__lcd_r0(lcd->utcb)); */
/* 	/\* */
/* 	 * Get page cptr */
/* 	 *\/ */
/* 	page = __lcd_cr0(lcd->utcb); */
/* 	/\* */
/* 	 * Look up page in lcd's cspace */
/* 	 *\/ */
/* 	ret = lcd_get_page(&lcd->cspace, page, &page_cnode, &page_struct); */
/* 	if (ret) */
/* 		goto fail1; */
/* 	/\* */
/* 	 * If page is not mapped, fail */
/* 	 *\/ */
/* 	if (!page_cnode->is_mapped ||  */
/* 		gpa_val(page_cnode->where_mapped) != gpa_val(gpa)) { */
/* 		LCD_ERR("page not mapped or mapped elsewhere"); */
/* 		ret = -EINVAL; */
/* 		lcd_put_page(page_cnode, page_struct); */
/* 		goto fail2; */
/* 	} */
/* 	/\* */
/* 	 * Unmap page */
/* 	 *\/ */
/* 	gp_unmap(lcd, gpa); */
/* 	/\* */
/* 	 * Mark page as not mapped */
/* 	 *\/ */
/* 	page_cnode->is_mapped = 0; */
/* 	/\* */
/* 	 * Release page cnode, etc. */
/* 	 *\/ */
/* 	lcd_put_page(page_cnode, page_struct); */

/* 	return 0; */

/* fail2: */
/* fail1: */
/* 	return ret; */
/* } */

/* static int do_cap_delete(struct lcd *lcd) */
/* { */
/* 	cptr_t slot; */
/* 	/\* */
/* 	 * Get cptr */
/* 	 *\/ */
/* 	slot = __lcd_cr0(lcd->utcb); */
/* 	__lcd_cap_delete(&lcd->cspace, slot); */

/* 	return 0; */
/* } */

/* static int do_create_sync_endpoint(struct lcd *lcd) */
/* { */
/* 	cptr_t slot; */

/* 	slot = __lcd_cr0(lcd->utcb); */
/* 	return __lcd_create_sync_endpoint(lcd, slot); */
/* } */

/* static int lcd_handle_syscall(struct lcd *lcd, int *lcd_ret) */
/* { */
/* 	int syscall_id; */
/* 	cptr_t endpoint; */
/* 	int ret; */
	
/* 	syscall_id = lcd_arch_get_syscall_num(lcd->lcd_arch); */

/* 	LCD_DEBUG(LCD_DEBUG_VERB,  */
/* 		"got syscall %d from lcd %p", syscall_id, lcd); */

/* 	switch (syscall_id) { */
/* 	case LCD_SYSCALL_EXIT: */
/* 		/\* */
/* 		 * Return value to signal exit */
/* 		 *\/ */
/* 		*lcd_ret = (int)__lcd_r0(lcd->utcb); */
/* 		ret = 1; */
/* 		break; */
/* 	case LCD_SYSCALL_PUTCHAR: */
/* 		/\* */
/* 		 * Put char and possibly print on host */
/* 		 *\/ */
/* 		ret = __lcd_put_char(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_SEND: */
/* 		/\* */
/* 		 * Get endpoint */
/* 		 *\/ */
/* 		endpoint = __lcd_cr0(lcd->utcb); */
/* 		/\* */
/* 		 * Null out that register so that it's not transferred */
/* 		 * during ipc */
/* 		 *\/ */
/* 		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL); */
/* 		/\* */
/* 		 * Do send, and set ret val */
/* 		 *\/ */
/* 		ret = __lcd_send(lcd, endpoint); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_RECV: */
/* 		/\* */
/* 		 * Get endpoint */
/* 		 *\/ */
/* 		endpoint = __lcd_cr0(lcd->utcb); */
/* 		/\* */
/* 		 * Null out that register so that it's not transferred */
/* 		 * during ipc */
/* 		 *\/ */
/* 		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL); */
/* 		/\* */
/* 		 * Do recv */
/* 		 *\/ */
/* 		ret = __lcd_recv(lcd, endpoint); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_CALL: */
/* 		/\* */
/* 		 * Get endpoint */
/* 		 *\/ */
/* 		endpoint = __lcd_cr0(lcd->utcb); */
/* 		/\* */
/* 		 * Null out that register so that it's not transferred */
/* 		 * during ipc */
/* 		 *\/ */
/* 		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL); */
/* 		/\* */
/* 		 * Do call */
/* 		 *\/ */
/* 		ret = __lcd_call(lcd, endpoint); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_REPLY: */
/* 		/\* */
/* 		 * No endpoint arg; just do reply */
/* 		 *\/ */
/* 		ret = __lcd_reply(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_PAGE_ALLOC: */
/* 		/\* */
/* 		 * Do page alloc */
/* 		 *\/ */
/* 		ret = do_page_alloc(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_PAGE_MAP: */
/* 		/\* */
/* 		 * Do page map */
/* 		 *\/ */
/* 		ret = do_page_map(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_PAGE_UNMAP: */
/* 		/\* */
/* 		 * Do page unmap */
/* 		 *\/ */
/* 		ret = do_page_unmap(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_CAP_DELETE: */
/* 		/\* */
/* 		 * Do cap delete */
/* 		 *\/ */
/* 		ret = do_cap_delete(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	case LCD_SYSCALL_SYNC_EP: */
/* 		/\* */
/* 		 * Create synchronous endpoint */
/* 		 *\/ */
/* 		ret = do_create_sync_endpoint(lcd); */
/* 		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret); */
/* 		break; */
/* 	default: */
/* 		LCD_ERR("unimplemented syscall %d", syscall_id); */
/* 		ret = -ENOSYS; */
/* 	} */
/* 	return ret; */
/* } */

/* static int lcd_kthread_run_once(struct lcd *lcd, int *lcd_ret) */
/* { */
/* 	int ret; */

/* 	ret = lcd_arch_run(lcd->lcd_arch); */
/* 	if (ret < 0) { */
/* 		LCD_ERR("running lcd %p", lcd); */
/* 		goto out; */
/* 	} */

/* 	switch(ret) { */
/* 	case LCD_ARCH_STATUS_PAGE_FAULT: */
/* 		LCD_ERR("page fault for lcd %p", lcd); */
/* 		ret = -ENOSYS; */
/* 		goto out; */
/* 		break; */
/* 	case LCD_ARCH_STATUS_EXT_INTR: */
/* 		/\* */
/* 		 * Continue */
/* 		 *\/ */
/* 		ret = 0; */
/* 		goto out; */
/* 	case LCD_ARCH_STATUS_EPT_FAULT: */
/* 		LCD_ERR("ept fault"); */
/* 		ret = -ENOSYS; */
/* 		goto out; */
/* 	case LCD_ARCH_STATUS_CR3_ACCESS: */
/* 		/\* */
/* 		 * Continue */
/* 		 *\/ */
/* 		ret = 0; */
/* 		goto out; */
/* 	case LCD_ARCH_STATUS_SYSCALL: */
/* 		ret = lcd_handle_syscall(lcd, lcd_ret); */
/* 		goto out; */
/* 	} */
	
/* 	/\* */
/* 	 * Sleep if we don't have full preemption turned on, and someone */
/* 	 * else should have a turn. */
/* 	 *\/ */
/* #ifndef CONFIG_PREEMPT */
/* 	cond_resched(); */
/* #endif	 */

/* out: */
/* 	return ret; */
/* } */

/* static int lcd_should_stop(struct lcd *lcd) */
/* { */
/* 	int ret; */

/* 	/\* */
/* 	 * Check our status */
/* 	 *\/ */
/* 	switch(get_lcd_status(lcd)) { */

/* 	case LCD_STATUS_DEAD: */
/* 		/\* */
/* 		 * We're dead; return 1 to signal to caller. */
/* 		 * (kthread_should_stop would also become true at some */
/* 		 * later point) */
/* 		 *\/ */
/* 		ret = 1; */
/* 		goto out; */
/* 	case LCD_STATUS_RUNNING: */
/* 		/\* */
/* 		 * The lcd should start or continue running; return 0 */
/* 		 * to signal that */
/* 		 *\/ */
/* 		ret = 0; */
/* 		goto out; */
/* 	default: */
/* 		BUG(); /\* shouldn't be in any other state *\/ */
/* 		ret = 1; */
/* 		goto out; */
/* 	} */
/* out: */
/* 	return ret; */
/* } */

/* static int lcd_main_for_lcd(struct lcd *lcd) */
/* { */
/* 	int ret; */
/* 	int lcd_ret = 0; */
/* 	/\* */
/* 	 * Enter run loop, check after each iteration if we should stop */
/* 	 * */
/* 	 * XXX: We're not giving the thread a chance to gracefully exit */
/* 	 * for now (e.g., we could use a special upcall message to signal that  */
/* 	 * it should terminate). But maybe not ... it's up to the owners */
/* 	 * of the LCD to notify it when it should tear down? */
/* 	 *\/ */
/* 	for (;;) { */
/* 		ret = lcd_kthread_run_once(lcd, &lcd_ret); */
/* 		if (ret < 0 || lcd_should_stop(lcd)) { */
/* 			/\* ret < 0 means fatal error *\/ */
/* 			/\* lcd_should_stop means our parent told us to die *\/ */
/* 			return ret; */
/* 		} else if (ret == 1) { */
/* 			/\* lcd exited *\/ */
/* 			return lcd_ret; */
/* 		} else { */
/* 			/\* ret = 0; continue *\/ */
/* 			continue; */
/* 		} */
/* 	} */
	
/* 	/\* unreachable *\/ */
/* } */

/* static int lcd_main_for_klcd(struct lcd *lcd) */
/* { */
/* 	return lcd->klcd_main(); */
/* } */

/* static int lcd_kthread_main(void *data) /\* data is NULL *\/ */
/* { */
/* 	struct lcd *current_lcd; */

/* 	current_lcd = current->lcd; */

/* 	/\* */
/* 	 * TODO: If the LCD exits, and it is the only one that has a */
/* 	 * reference to itself, we should tear it down. */
/* 	 *\/ */
/* 	switch (current_lcd->type) { */

/* 	case LCD_TYPE_ISOLATED: */
/* 		return lcd_main_for_lcd(current_lcd); */
/* 	case LCD_TYPE_NONISOLATED: */
/* 		return lcd_main_for_klcd(current_lcd); */
/* 	default: */
/* 		LCD_ERR("unexpected lcd type = %d", */
/* 			current_lcd->type); */
/* 		return -EINVAL; */
/* 	} */
/* } */

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

/* SYSRQ debugging -------------------------------------------------- */

static void sysrq_handle_showlcds(int key)
{
	printk(KERN_ERR "==========BEGIN LCD SYSRQ INFO==========\n");
	printk(KERN_ERR "Is there an LCD in non-root? %s\n",
		lcd_in_non_root ? "yes" : "no");
	if (lcd_in_non_root) {
		printk(KERN_ERR "And it's running on cpu %d\n",
			lcd_on_cpu);
	}
	printk(KERN_ERR "==========END LCD SYSRQ INFO==========\n");
}

static struct sysrq_key_op sysrq_showlcds_op = {
	.handler	= sysrq_handle_showlcds,
	.help_msg	= "show-lcd-info(x)",
	.action_msg	= "Show LCD Info",
};

/* Init / Exit ---------------------------------------- */

static int __init lcd_init(void)
{
	int ret;

	/*
	 * Register sysrq handler before we do *anything*
	 */
	ret = register_sysrq_key('x', &sysrq_showlcds_op);
	if (ret) {
		LCD_ERR("failed to register sysrq key");
		goto failkey;
	}
	ret = lcd_arch_init();
	if (ret) {
		LCD_ERR("failed to init arch-dependent code");
		goto fail0;
	}
	ret = __lcd_cap_init();
	if (ret) {
		LCD_ERR("failed to init capability subsystem");
		goto fail1;
	}
	ret = __lcd_ipc_init();
	if (ret) {
		LCD_ERR("failed to init ipc subsystem");
		goto fail2;
	}
	ret = misc_register(&lcd_dev);
	if (ret) {
		LCD_ERR("misc device register failed, ret = %d", ret);
		goto fail3;
	}
	ret = __kliblcd_init();
	if (ret) {
		LCD_ERR("failed to init kliblcd");
		goto fail4;
	}

	LCD_MSG("lcd microkernel initialized");

	return 0;

fail4:
	misc_deregister(&lcd_dev);
fail3:
	__lcd_ipc_exit();
fail2:
	__lcd_cap_exit();
fail1:
	lcd_arch_exit();
fail0:
	unregister_sysrq_key('x', &sysrq_showlcds_op);
failkey:
	return ret;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	__lcd_cap_exit();
	__lcd_ipc_exit();
	__kliblcd_exit();
	lcd_arch_exit();
	unregister_sysrq_key('x', &sysrq_showlcds_op);

	LCD_MSG("lcd microkernel exited");
}

/* EXPORTS ---------------------------------------- */

module_init(lcd_init);
module_exit(lcd_exit);
