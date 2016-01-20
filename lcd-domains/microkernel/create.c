/*
 * create.c -- Code for creating/destroying LCDs
 *
 * Copyright: University of Utah
 */
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include "internal.h"

/* CREATE -------------------------------------------------- */

int __lcd_create_no_vm_no_thread(struct lcd **out)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Alloc lcd data structure
	 *
	 * (Because we're doing a zalloc, this will set the type, status,
	 * and so on to "defaults".)
	 */
	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		LCD_ERR("error alloc'ing lcd");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Alloc and init LCD's cspace. Use the microkernel's
	 * type system.
	 */
	lcd->cspace = cap_alloc_cspace();
	if (!lcd->cspace) {
		LCD_ERR("cspace alloc failed");
		ret = -ENOMEM;
		goto fail2;
	}
	ret = cap_init_cspace_with_type_system(lcd->cspace,
					lcd_libcap_type_system);
	if (ret) {
		LCD_ERR("cspace init failed");
		goto fail3;
	}
	/*
	 * Set up synchronous ipc utcb
	 */
	lcd->utcb = hva2va(__hva(get_zeroed_page(GFP_KERNEL)));
	if (!lcd->utcb) {
		LCD_ERR("utcb alloc");
		goto fail4;
	}
	/*
	 * Init mutex
	 */
	mutex_init(&lcd->lock);
	/*
	 * Initialize send/recv queue list element
	 */
	INIT_LIST_HEAD(&lcd->endpoint_queue);
	/*
	 * Initialize console cursor
	 */
	lcd->console_cursor = 0;

	*out = lcd;

	return 0;

fail4:
	cap_destroy_cspace(lcd->cspace);
fail3:
	cap_free_cspace(lcd->cspace);
fail2:
	kfree(lcd);
fail1:
	return ret;
}

int __lcd_create_no_vm(struct lcd **out)
{
	struct lcd *lcd;
	int ret;
	/*
	 * More basic init
	 */
	ret = __lcd_create_no_vm_no_thread(&lcd);
	if (ret) {
		LCD_ERR("basic lcd create failed");
		goto fail1;
	}
	/*
	 * Create a kernel thread (won't run till we wake it up)
	 */
	lcd->kthread = kthread_create(__lcd_kthread_main, NULL, "lcd");
	if (!lcd->kthread) {
		LCD_ERR("failed to create kthread");
		goto fail2;
	}
	/*
	 * Bump reference count on kthread
	 */
	get_task_struct(lcd->kthread);
	/*
	 * Store back reference to lcd
	 */
	lcd->kthread->lcd = lcd;

	*out = lcd;

	return 0;

fail2:
	__lcd_destroy_no_vm_no_thread(lcd);
fail1:
	return ret;
}

int __lcd_create(struct lcd *caller, cptr_t slot)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Basic init of lcd
	 */
	ret = __lcd_create_no_vm(&lcd);
	if (ret) {
		LCD_ERR("lcd create");
		goto fail1;
	}
	/*
	 * Alloc vm / arch-dependent part
	 */
	ret = lcd_arch_create(&lcd->lcd_arch);
	if(ret) {
		LCD_ERR("error creating lcd_arch");
		goto fail2;
	}
	/*
	 * Put in caller's cspace
	 */
	ret = cap_insert(&caller->cspace, slot, 
			lcd,
			__lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_LCD));
	if (ret) {
		LCD_ERR("cap insert failed");
		goto fail3;
	}
	/*
	 * Done
	 */
	return 0;

fail3:
	lcd_arch_destroy(lcd->lcd_arch);
fail2:
	__lcd_destroy_no_vm(lcd);
fail1:
	return ret;
}

int __klcd_create_klcd(struct lcd *caller, cptr_t slot)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Basic init of lcd
	 */
	ret = __lcd_create_no_vm(&lcd);
	if (ret) {
		LCD_ERR("lcd create");
		goto fail1;
	}
	/*
	 * Set type as non-isolated
	 */
	lcd->type = LCD_TYPE_NONISOLATED;
	/*
	 * Put in caller's cspace
	 */
	ret = __lcd_cap_insert(&caller->cspace, slot, lcd, 
			LCD_CAP_TYPE_KLCD);
	if (ret) {
		LCD_ERR("insert");
		goto fail2;
	}
	/*
	 * Done
	 */
	return 0;

fail2:
	__lcd_destroy_no_vm(lcd);
fail1:
	return ret;
}

/* LOOKUP -------------------------------------------------- */

static int lookup_lcd(struct cspace *cspace, cptr_t slot, struct cnode **cnode)
{
	int ret;
	cap_type_t t;
	/*
	 * Look up
	 */
	ret = cap_cnode_get(cspace, slot, cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm it's an lcd or klcd
	 */
	t = cap_cnode_type(*cnode);
	if (t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_LCD) &&
		t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_KLCD)) {
		LCD_ERR("not an lcd");
		goto fail2;
	}

	return 0;

fail2:
	cap_cnode_put(*cnode);
fail1:
	return ret;
}

int __lcd_get(struct lcd *caller, cptr_t lcd, struct cnode **cnode,
	struct lcd **lcd_struct)
{
	int ret;
	/*
	 * Look up and lock cnode containing lcd
	 */
	ret = lookup_lcd(caller->cspace, lcd, cnode);
	if (ret)
		goto fail1;
	*lcd_struct = cap_cnode_object(*cnode);
	/*
	 * Lock lcd_struct
	 */
	ret = mutex_lock_interruptible(&(*lcd_struct)->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail2;
	}
	
	return 0; /* caller should match with __lcd_put */

fail2:
	cap_cnode_put(*cnode);
fail1:
	return ret;
}

void __lcd_put(struct lcd *caller, struct cnode *cnode, struct lcd *lcd)
{
	/*
	 * Unlock lcd
	 */
	mutex_unlock(&lcd->lock);
	/*
	 * Release cnode containing lcd
	 */
	cap_cnode_put(cnode);
}

/* CONFIGURE LCD -------------------------------------------------- */

static int config_lcd(struct lcd *caller, struct lcd *lcd_struct, 
		gva_t pc, gva_t sp, 
		gpa_t gva_root, gpa_t utcb_page)
{
	int ret;
	hva_t utcb_page_addr;
	/*
	 * If lcd is not an embryo, fail
	 */
	if (!lcd_status_embryo(lcd_struct)) {
		LCD_ERR("cannot config: lcd not an embryo");
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Set pc, sp, gva_root
	 */
	lcd_arch_set_pc(lcd_struct->lcd_arch, pc);
	lcd_arch_set_sp(lcd_struct->lcd_arch, sp);
	lcd_arch_set_gva_root(lcd_struct->lcd_arch, gva_root);
	/*
	 * Map utcb page in guest physical
	 */
	utcb_page_addr = va2hva(lcd_struct->utcb);
	ret = __lcd_map(lcd_struct, utcb_page, hva2hpa(utcb_page_addr));
	if (ret) {
		LCD_ERR("map");
		goto fail2;
	}
	/*
	 * Make sure lcd_arch has valid state
	 */
	ret = lcd_arch_check(lcd_struct->lcd_arch);
	if (ret) {
		LCD_ERR("bad lcd_arch state");
		goto fail3;
	}

	return 0;

fail3:
	__lcd_unmap(lcd_struct, utcb_page);
fail2:
fail1:
	return ret;
}

static int config_klcd(struct lcd *caller, struct lcd *lcd_struct, 
		gva_t pc, gva_t sp, 
		gpa_t gva_root, gpa_t utcb_page)
{
	/*
	 * For now, we ignore everything except the program counter.
	 *
	 * The program counter is assumed to be a host virtual
	 * address to a kernel module's init.
	 *
	 * (Recall that for klcd's, guest addresses = host addresses.)
	 */
	lcd_struct->klcd_main = hva2va(__hva(gva_val(pc)));

	return 0;
}

int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp, 
		gpa_t gva_root, gpa_t utcb_page)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = __lcd_get(caller->cspace, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * Switch on the type of the lcd (a regular lcd, klcd, ...)
	 */
	switch (lcd_struct->type) {

	case LCD_TYPE_ISOLATED:
		ret = __lcd_config_lcd(caller, lcd_struct, pc, sp, gva_root, 
				utcb_page);
		break;
	case LCD_TYPE_NONISOLATED:
		ret = __lcd_config_klcd(caller, lcd_struct, pc, sp, gva_root, 
					utcb_page);
		break;
	default:
		/* shouldn't happen */
		LCD_ERR("unexpected lcd type: %d",
			lcd_struct->type);
		goto fail2;		
	}
	if (ret) {
		LCD_ERR("error config'ing lcd, ret = %d", ret);
		goto fail2;
	}
	/*
	 * Set status to configed
	 */
	set_lcd_status(lcd_struct, LCD_STATUS_CONFIGED);

	ret = 0;
	goto out;

out:
fail2:
	/*
	 * Unlock
	 */
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}

/* DESTROY -------------------------------------------------- */

static void mark_lcd_as_dead(struct lcd *lcd)
{
	BUG_ON(lcd_status_dead(lcd)); /* lcd shouldn't be dead already */
	/*
	 * We need to synchronize with e.g. the synchronous ipc code - it
	 * checks to see if an LCD is dead while it is sitting in an
	 * ipc queue.
	 */ 
	if (mutex_lock_interruptible(&lcd->lock)) {
		/*
		 * This is probably not ideal
		 */
		LCD_ERR("interrupted, continuing with lcd stop (gulp) ...");
		set_lcd_status(lcd, LCD_STATUS_DEAD);
		return;
	}
	set_lcd_status(lcd, LCD_STATUS_DEAD);
	
	mutex_unlock(&lcd->lock);
}

static void destroy_cspace_and_utcb(struct lcd *lcd)
{
	/*
	 * Tear down and free cspace.
	 */
	cap_destroy_cspace(lcd->cspace);
	cap_free_cspace(lcd->cspace);
	lcd->cspace = NULL;
	/*
	 * Free synchronous ipc utcb page
	 */
	free_page((unsigned long)lcd->utcb);
	lcd->utcb = NULL;
}

static void destroy_kthread(struct lcd *lcd)
{
	int ret;
	/*
	 * XXX: it would be nice to pass the return value to someone; for
	 * now, we just print it out if it's non-zero, and then drop it
	 */
	ret = kthread_stop(lcd->kthread);
	if (ret)
		LCD_ERR("got non-zero exit status %d from lcd %p",
			ret, lcd);
	/*
	 * Put task struct so that host will free it
	 */
	put_task_struct(lcd->kthread);
	lcd->kthread = NULL;
}

void __lcd_destroy_no_vm_no_thread(struct lcd *lcd)
{
	mark_lcd_as_dead(lcd);
	destroy_cspace_and_utcb(lcd);
	kfree(lcd);
}

void __lcd_destroy_no_vm(struct lcd *lcd)
{
	int ret;
	/*
	 * ORDER IS IMPORTANT:
	 *
	 * (0) Mark the LCD as dead. This prevents e.g. synchronous endpoints 
	 *     from trying to wake up the kthread.
	 *
	 *     (Alternative: Just start tearing down the cspace and let
	 *     ipc's just fail in the lcd. After that, stop the lcd, since
	 *     it can't be in any endpoint queues.)
	 *
	 * (1) Stop the kthread. This will tell the kthread to exit.
	 *
	 * (2) Tear down the lcd's cspace. This will free any objects
	 *     for which the lcd has the last capability (like pages).
	 */
	mark_lcd_as_dead(lcd);
	destroy_kthread(lcd);
	destroy_cspace_and_utcb(lcd);
	kfree(lcd);
}

void __lcd_destroy(struct lcd *lcd)
{
	/*
	 * ORDER IS IMPORTANT:
	 *
	 * (0) Mark the LCD as dead. This prevents e.g. synchronous endpoints 
	 *     from trying to wake up the kthread.
	 *
	 *     (Alternative: Just start tearing down the cspace and let
	 *     ipc's just fail in the lcd. After that, stop the lcd, since
	 *     it can't be in any endpoint queues.)
	 *
	 * (1) Stop the kthread. This will tell the kthread to exit.
	 *
	 * (2) Tear down the lcd's cspace. This will free any objects
	 *     for which the lcd has the last capability (like pages).
	 *
	 * (3) Tear down the lcd arch. (This *must* come after (2), because
	 *     the cspace tear down may lead to unmapping pages in the lcd's
	 *     ept, which requires the lcd arch.)
	 */
	mark_lcd_as_dead(lcd);
	destroy_kthread(lcd);
	destroy_cspace_and_utcb(lcd);
	lcd_arch_destroy(lcd->lcd_arch);
	lcd->lcd_arch = NULL;
	kfree(lcd);
}
