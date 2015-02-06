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

#include <asm/lcd-domains-arch.h>
#include <lcd-domains/types.h>
#include "internal.h"
#include <uapi/linux/lcd-domains.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* QUICK LCD LOOKUP / LOCK -------------------------------------------------- */

static int lookup_lcd(struct cspace *cspace, cptr_t slot, struct cnode **cnode)
{
	int ret;
	/*
	 * Look up
	 */
	ret = __lcd_cnode_get(cspace, slot, cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm it's an lcd
	 */
	if ((*cnode)->type != LCD_CAP_TYPE_LCD) {
		LCD_ERR("not an lcd");
		goto fail2;
	}

	return 0;

fail2:
	__lcd_cnode_put(*cnode);
fail1:
	return ret;
}

static int get_lcd(struct cspace *cspace, cptr_t lcd, struct cnode **cnode,
		struct lcd **lcd_struct)
{
	int ret;
	/*
	 * Look up and lock cnode containing lcd
	 */
	ret = lookup_lcd(cspace, lcd, cnode);
	if (ret)
		goto fail1;
	*lcd_struct = (*cnode)->object;
	/*
	 * Lock lcd_struct
	 */
	ret = mutex_lock_interruptible(&(*lcd_struct)->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail2;
	}
	
	return 0; /* caller should match with put_lcd */

fail2:
	__lcd_cnode_put(*cnode);
fail1:
	return ret;
}

static void put_lcd(struct cnode *cnode, struct lcd *lcd)
{
	/*
	 * Unlock lcd
	 */
	mutex_unlock(&lcd->lock);
	/*
	 * Release cnode
	 */
	__lcd_cnode_put(cnode);
}

static int lookup_page(struct cspace *cspace, cptr_t slot, struct cnode **cnode)
{
	int ret;
	/*
	 * Look up
	 */
	ret = __lcd_cnode_get(cspace, slot, cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm it's an lcd
	 */
	if ((*cnode)->type != LCD_CAP_TYPE_PAGE &&
		(*cnode)->type != LCD_CAP_TYPE_KPAGE) {
		LCD_ERR("not an lcd");
		goto fail2;
	}

	return 0;

fail2:
	__lcd_cnode_put(*cnode);
fail1:
	return ret;
}

static int lcd_get_page(struct cspace *cspace, cptr_t page, 
			struct cnode **cnode, struct page **page_struct)
{
	int ret;
	/*
	 * Look up and lock cnode containing page
	 */
	ret = lookup_page(cspace, page, cnode);
	if (ret)
		goto fail1;
	*page_struct = (*cnode)->object;
	/*
	 * XXX: for now, we assume we won't be touching any of the page
	 * fields - we just look up for mapping / unmapping etc.
	 */
	
	return 0; /* caller should match with put_page */

fail1:
	return ret;
}

static void lcd_put_page(struct cnode *cnode, struct page *page)
{
	/*
	 * (no unlock on page - see get_page)
	 *
	 * Release cnode
	 */
	__lcd_cnode_put(cnode);
}


/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */


int __klcd_page_zalloc(struct lcd *klcd, cptr_t c, hpa_t *hpa_out,
			       hva_t *hva_out)
{
	int ret;
	struct page *p;
	unsigned long addr;
	/*
	 * Allocate a page
	 */
	addr = get_zeroed_page(GFP_KERNEL);
	if (!addr) {
		LCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail1;
	}
	p = virt_to_page(addr);
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
	*hva_out = __hva(addr);
	*hpa_out = hva2hpa(*hva_out);

	return 0;

fail2:
	free_page(addr);
fail1:
	return ret;
}


/* IPC -------------------------------------------------- */

/* in ipc.c */


/* LCD CREATE / CONFIG -------------------------------------------------- */


static int gp_map(struct lcd *lcd, gpa_t gpa, hpa_t hpa)
{
	/* 
	 * Create, and do not overwrite
	 */
	return lcd_arch_ept_map(lcd->lcd_arch, gpa, hpa, 1, 0);
}

static void gp_unmap(struct lcd *lcd, gpa_t gpa)
{
	int ret;
	ret = lcd_arch_ept_unmap(lcd->lcd_arch, gpa);
	if (ret)
		LCD_ERR("some error unmapping");
}

int __lcd_create__(struct lcd **out)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Alloc lcd data structure
	 */
	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		LCD_ERR("error alloc'ing lcd");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Init cspace
	 */
	ret = __lcd_cap_init_cspace(&lcd->cspace);
	if (ret) {
		LCD_ERR("set up cspace");
		goto fail2;
	}
	/*
	 * Init mutex and completion
	 */
	mutex_init(&lcd->lock);
	init_completion(&lcd->suspended);
	/*
	 * Initialize send/recv queue list element
	 */
	INIT_LIST_HEAD(&lcd->endpoint_queue);

	*out = lcd;

	return 0;

fail2:
	kfree(lcd);
fail1:
	return ret;
}

int __lcd_create(struct lcd *caller, cptr_t slot, gpa_t stack)
{
	struct lcd *lcd;
	hva_t addr;
	int ret;
	/*
	 * Basic init of lcd
	 */
	ret = __lcd_create__(&lcd);
	if (ret) {
		LCD_ERR("lcd create");
		goto fail1;
	}
	/*
	 * Alloc arch-dependent part
	 */
	ret = lcd_arch_create(&lcd->lcd_arch);
	if(ret) {
		LCD_ERR("error creating lcd_arch");
		goto fail2;
	}
	/*
	 * Allocate a page for the stack/utcb
	 */
	addr = __hva(get_zeroed_page(GFP_KERNEL));
	if (!hva_val(addr)) {
		LCD_ERR("alloc page");
		goto fail3;
	}
	/*
	 * Map in lcd
	 */
	ret = gp_map(lcd, stack, hva2hpa(addr));
	if (ret) {
		LCD_ERR("map");
		goto fail4;
	}
	/*
	 * Store ref in lcd struct's utcb
	 */
	lcd->utcb = hva2va(addr);
	/*
	 * Put in caller's cspace
	 */
	ret = __lcd_cap_insert(&caller->cspace, slot, lcd, LCD_CAP_TYPE_LCD);
	if (ret) {
		LCD_ERR("insert");
		goto fail5;
	}
	/*
	 * Done
	 */
	return 0;

fail5:
	gp_unmap(lcd, stack);
fail4:
	free_page(hva_val(addr));
fail3:
	lcd_arch_destroy(lcd->lcd_arch);
fail2:
	__lcd_destroy__(lcd);
fail1:
	return ret;
}

static int lcd_kthread_main(void *data);

int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp, 
		gpa_t gva_root)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * Set pc, sp, gva_root
	 */
	lcd_arch_set_pc(lcd_struct->lcd_arch, pc);
	lcd_arch_set_sp(lcd_struct->lcd_arch, sp);
	lcd_arch_set_gva_root(lcd_struct->lcd_arch, gva_root);
	/*
	 * Make sure lcd_arch has valid state
	 */
	ret = lcd_arch_check(lcd_struct->lcd_arch);
	if (ret) {
		LCD_ERR("bad lcd_arch state");
		goto fail2;
	}
	/*
	 * Create a kernel thread (won't run till we wake it up)
	 */
	lcd_struct->kthread = kthread_create(lcd_kthread_main, NULL, "lcd");
	if (!lcd_struct->kthread) {
		LCD_ERR("failed to create kthread");
		goto fail3;
	}
	/*
	 * Bump reference count on kthread
	 */
	get_task_struct(lcd_struct->kthread);
	/*
	 * Store back reference to lcd
	 */
	lcd_struct->kthread->lcd = lcd_struct;

	/*
	 * Unlock
	 */
	put_lcd(cnode, lcd_struct);

	return 0;

fail3:
fail2:
	put_lcd(cnode, lcd_struct);
fail1:
	return ret;
}

int __lcd_run(struct lcd *caller, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is dead, fail
	 */
	if (lcd_status_dead(lcd_struct)) {
		LCD_ERR("lcd is dead");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * If lcd is an embryo, this will run the kthread for the first
	 * time
	 */
	set_lcd_status(lcd_struct, LCD_STATUS_RUNNING);
	wake_up_process(lcd_struct->kthread);
	/*
	 * Unlock
	 */
	put_lcd(cnode, lcd_struct);

	return 0;

fail2:
	put_lcd(cnode, lcd_struct);
fail1:
	return ret;
}

int __lcd_suspend(struct lcd *caller, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is dead, fail
	 */
	if (lcd_status_dead(lcd_struct)) {
		LCD_ERR("lcd is dead");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * If lcd is running, suspend it; otherwise (embryo or already
	 * suspended), just skip
	 */
	if (lcd_status_running(lcd_struct)) {

		set_lcd_status(lcd_struct, LCD_STATUS_SUSPENDED);
		/*
		 * Wait till it acknowledges
		 */
		wait_for_completion(&lcd_struct->suspended);

	}

	/*
	 * Unlock
	 */
	put_lcd(cnode, lcd_struct);

	return 0;

fail2:
	put_lcd(cnode, lcd_struct);
fail1:
	return ret;
}

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
	 * Mark lcd as running
	 */
	set_lcd_status(lcd, LCD_STATUS_RUNNING);

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
	/*
	 * Like lcd destroy, but simpler.
	 *
	 * Since the caller is the thread itself, we can assume it is
	 * not in an endpoint queue.
	 *
	 * We still use some care and mark the lcd as dead (even though
	 * the thread itself isn't going away).
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
}

/* CAPABILITIES -------------------------------------------------- */

int __lcd_cap_grant_cheat(struct lcd *caller, cptr_t lcd, cptr_t src, 
			cptr_t dest)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = get_lcd(&caller->cspace, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is not an embryo, fail - we only allow direct grants when
	 * the lcd is being set up
	 */
	if (!lcd_status_embryo(lcd_struct)) {
		LCD_ERR("lcd is not an embryo");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * Do regular grant
	 */
	ret = __lcd_cap_grant(&caller->cspace, src,
			&lcd_struct->cspace, dest);
	if (ret)
		goto fail3;
	/*
	 * Put
	 */
	put_lcd(cnode, lcd_struct);

	return 0;

fail3:
fail2:
	put_lcd(cnode, lcd_struct);
fail1:
	return ret;
}

int __lcd_cap_page_grant_map_cheat(struct lcd *caller, cptr_t lcd, 
				cptr_t page, cptr_t dest, gpa_t gpa)
{
	struct lcd *lcd_struct;
	struct page *page_struct;
	struct cnode *lcd_cnode;
	struct cnode *page_cnode;
	int ret;
	/*
	 * Look up and lock lcd and page, in that order (gulp)
	 */
	ret = get_lcd(&caller->cspace, lcd, &lcd_cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is not an embryo, fail - we only allow direct grants when
	 * the lcd is being set up
	 */
	if (!lcd_status_embryo(lcd_struct)) {
		LCD_ERR("lcd is not an embryo");
		ret = -EINVAL;
		goto fail2;
	}
	ret = lcd_get_page(&caller->cspace, page, &page_cnode, &page_struct);
	if (ret)
		goto fail3;
	/*
	 * Map page
	 */
	ret = gp_map(lcd_struct, gpa, va2hpa(page_address(page_struct)));
	if (ret) {
		LCD_ERR("map");
		lcd_put_page(page_cnode, page_struct);
		goto fail4;
	}
	/*
	 * Release page cnode, so that grant can go through
	 */
	__lcd_cnode_put(page_cnode);
	/*
	 * Do regular grant
	 */
	ret = __lcd_cap_grant_page(&caller->cspace, page,
				&lcd_struct->cspace, dest, gpa);
	if (ret)
		goto fail5;
	/*
	 * Put lcd
	 */
	put_lcd(lcd_cnode, lcd_struct);

	return 0;

fail5:
	gp_unmap(lcd_struct, gpa);
fail4:
fail3:
fail2:
	put_lcd(lcd_cnode, lcd_struct);
fail1:
	return ret;
}

/* rest is in cap.c */


/* END OF INTERFACE STUFF -------------------------------------------------- */


/* CHECKS -------------------------------------------------- */


void __lcd_page_check(struct lcd *lcd, struct page *p, int is_mapped, 
		gpa_t where_mapped)
{
	/*
	 * If it's not mapped, nothing to do
	 */
	if (!is_mapped)
		return;
	/*
	 * Unmap it
	 */
	gp_unmap(lcd, where_mapped);
}

void __lcd_kpage_check(struct lcd *lcd, struct page *p, int is_mapped, 
		gpa_t where_mapped)
{
	__lcd_page_check(lcd, p, is_mapped, where_mapped);
}

void __lcd_check(struct lcd *owning_lcd, struct lcd *owned_lcd)
{
	/* no-op for now */
	return;
}

/* endpoint check in ipc.c */


/* DESTROY -------------------------------------------------- */


void __lcd_page_destroy(struct page *p)
{
	/* just free the page - it's unmapped everywhere */
	free_page((unsigned long)page_address(p));
}

void __lcd_kpage_destroy(struct page *p)
{
	/* do nothing - klcd is responsible for freeing it */
	return;
}

void __lcd_destroy__(struct lcd *lcd)
{
	/*
	 * Tear down cspace.
	 */
	__lcd_cap_destroy_cspace(&lcd->cspace);
	kfree(lcd);
}

void __lcd_destroy(struct lcd *lcd)
{
	int ret;
	/*
	 * ORDER IS IMPORTANT:
	 *
	 * (0) Stop the kthread and set the LCD's status. This stops the
	 *     lcd and prevents endpoints from trying to wake up the kthread.
	 *
	 *     (Alternative: Just start tearing down the cspace and let
	 *     ipc's just fail in the lcd. After that, stop the lcd, since
	 *     it can't be in any endpoint queues.)
	 *
	 * (1) Tear down the lcd's cspace. This will free any objects
	 *     for which the lcd has the last capability (like pages). Pages
	 *     will automatically be unmapped as their caps are deleted (via
	 *     __lcd_page_check).
	 *
	 * (2) Tear down the lcd arch
	 *
	 * Mark the lcd as dead
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
	 * Now any endpoints that still have the lcd in the queue will
	 * check and see that it is dead before trying to wake up the
	 * kthread.
	 *
	 * We now stop the kthread.
	 *
	 * XXX: it would be nice to pass the return value to someone; for
	 * now, we just drop it
	 */
	ret = kthread_stop(lcd->kthread);
	/*
	 * Decrement the kthread's reference count. Host kernel will clean
	 * up the rest.
	 */
	put_task_struct(lcd->kthread);
	/*
	 * Free the stack page
	 */
	free_page((unsigned long)lcd->utcb);
	/*
	 * Tear down lcd_arch (ept, ...)
	 */
	lcd_arch_destroy(lcd->lcd_arch);
	/*
	 * Finish - cspace, etc.
	 */
	__lcd_destroy__(lcd);
}


/* LCD EXECUTION -------------------------------------------------- */

#if 0
static int lcd_handle_syscall(struct lcd *lcd)
{
	int syscall_id;
	
	syscall_id = LCD_ARCH_GET_SYSCALL_NUM(lcd->lcd_arch);
	LCD_MSG("got syscall %d", syscall_id);

	switch (syscall_id) {
	case LCD_SYSCALL_YIELD:
		return 1;
	case LCD_SYSCALL_SEND:
	case LCD_SYSCALL_RECV:
	default:
		LCD_ERR("unimplemented syscall %d", syscall_id);
		return -ENOSYS;
	}
}

static int lcd_kthread_run_once(struct lcd *lcd)
{
	int ret;

	ret = lcd_arch_run(lcd->lcd_arch);
	if (ret < 0) {
		LCD_ERR("running lcd");
		goto out;
	}

	switch(ret) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		LCD_ERR("page fault");
		ret = -ENOSYS;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		LCD_MSG("got external intr");
		ret = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		LCD_ERR("ept fault");
		ret = -ENOSYS;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		LCD_ERR("cr3 access");
		ret = -ENOSYS;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		LCD_MSG("syscall");
		ret = lcd_handle_syscall(t);
		goto out;
	}

out:
	return ret;
}
#endif

static int lcd_kthread_main(void *data) { return 0; }

#if 0
static int lcd_kthread_main(void *data) /* data is NULL */
{
	int ret;
	struct lcd *current_lcd;

	current_lcd = current->lcd;
	/*
	 * Enter run loop, check after each iteration if we should stop
	 *
	 * XXX: We're not giving the thread a chance to gracefully exit
	 * for now (e.g., we could use a special upcall message to signal that 
	 * it should terminate).
	 */
	for (;;) {
		ret = lcd_kthread_run_once(current_lcd);
		if (ret || kthread_should_stop())
			return ret; /* to microkernel via kthread_stop */
	}
	
	/* unreachable */
}

int lcd_start(struct lcd *lcd)
{
	/*
	 * Wake up the kthread (will start running lcd_kthread_main).
	 */
	wake_up_process(lcd->kthread);

	return 0;
}

#endif

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

/* Init / Exit ---------------------------------------- */

static int main_tests(void);

static int __init lcd_init(void)
{
	int ret;

	printk(KERN_ERR "LCD module loaded\n");

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
	ret = __kliblcd_init();
	if (ret) {
		LCD_ERR("failed to init kliblcd");
		goto fail3;
	}
	ret = misc_register(&lcd_dev);
	if (ret) {
		LCD_ERR("misc device register failed");
		goto fail4;		
	}

	/*
	 * Run main's tests
	 */
	ret = main_tests();
	if (ret)
		goto fail5;

	return 0;

fail5:
	misc_deregister(&lcd_dev);
fail4:
	__kliblcd_exit();
fail3:
	__lcd_ipc_exit();
fail2:
	__lcd_cap_exit();
fail1:
	return ret;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	__lcd_cap_exit();
	__lcd_ipc_exit();
}

/* EXPORTS ---------------------------------------- */

module_init(lcd_init);
module_exit(lcd_exit);

/* DEBUGGING ---------------------------------------- */

#include "tests/main-tests.c"
