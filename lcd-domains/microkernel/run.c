/*
 * run.c -- Code for running LCDs
 *
 * Copyright: University of Utah
 */
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include "internal.h"

static int __lcd_put_char(struct lcd *lcd)
{
	char c;
	/*
	 * Char is in r0
	 */
	c = __lcd_debug_reg(lcd->utcb);
	/*
	 * Put char in lcd's console buff
	 */
	lcd->console_buff[lcd->console_cursor] = c;
	/*
	 * Bump cursor and check
	 */
	lcd->console_cursor++;
	if (c == 0) {
		/*
		 * End of string; printk to host and reset buff
		 */
		printk(KERN_INFO "message from lcd %p: %s\n",
			lcd, lcd->console_buff);
		lcd->console_cursor = 0;
		return 0;
	}
	if (lcd->console_cursor >= LCD_CONSOLE_BUFF_SIZE - 1) {
		/*
		 * Filled buffer; empty it.
		 */
		lcd->console_buff[LCD_CONSOLE_BUFF_SIZE - 1] = 0;
		printk(KERN_INFO "(incomplete) message from lcd %p: %s\n",
			lcd, lcd->console_buff);
		lcd->console_cursor = 0;
		return 0;
	}
	return 0;
}

static int do_cap_delete(struct lcd *lcd)
{
	cptr_t slot;
	/*
	 * Get cptr
	 */
	slot = __lcd_cr0(lcd->utcb);
	__lcd_cap_delete(&lcd->cspace, slot);

	return 0;
}

static int do_create_sync_endpoint(struct lcd *lcd)
{
	cptr_t slot;

	slot = __lcd_cr0(lcd->utcb);
	return __lcd_create_sync_endpoint(lcd, slot);
}

static int handle_syscall(struct lcd *lcd, int *lcd_ret)
{
	int syscall_id;
	cptr_t endpoint;
	int ret;
	
	syscall_id = lcd_arch_get_syscall_num(lcd->lcd_arch);

	LCD_DEBUG(LCD_DEBUG_VERB, 
		"got syscall %d from lcd %p", syscall_id, lcd);

	switch (syscall_id) {
	case LCD_SYSCALL_EXIT:
		/*
		 * Return value to signal exit
		 */
		*lcd_ret = (int)__lcd_r0(lcd->utcb);
		ret = 1;
		break;
	case LCD_SYSCALL_PUTCHAR:
		/*
		 * Put char and possibly print on host
		 */
		ret = __lcd_put_char(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_SEND:
		/*
		 * Get endpoint
		 */
		endpoint = __lcd_cr0(lcd->utcb);
		/*
		 * Null out that register so that it's not transferred
		 * during ipc
		 */
		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL);
		/*
		 * Do send, and set ret val
		 */
		ret = __lcd_send(lcd, endpoint);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_RECV:
		/*
		 * Get endpoint
		 */
		endpoint = __lcd_cr0(lcd->utcb);
		/*
		 * Null out that register so that it's not transferred
		 * during ipc
		 */
		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL);
		/*
		 * Do recv
		 */
		ret = __lcd_recv(lcd, endpoint);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_CALL:
		/*
		 * Get endpoint
		 */
		endpoint = __lcd_cr0(lcd->utcb);
		/*
		 * Null out that register so that it's not transferred
		 * during ipc
		 */
		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL);
		/*
		 * Do call
		 */
		ret = __lcd_call(lcd, endpoint);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_REPLY:
		/*
		 * No endpoint arg; just do reply
		 */
		ret = __lcd_reply(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_PAGE_ALLOC:
		/*
		 * Do page alloc
		 */
		ret = __lcd_alloc_pages(lcd, __lcd_cr0(lcd->utcb),
					GFP_KERNEL, 0);
		__lcd_set_cr0(lcd->utcb, LCD_CPTR_NULL);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_PAGE_MAP:
		/*
		 * Do page map
		 */
		ret = do_page_map(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_PAGE_UNMAP:
		/*
		 * Do page unmap
		 */
		ret = do_page_unmap(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_CAP_DELETE:
		/*
		 * Do cap delete
		 */
		ret = do_cap_delete(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	case LCD_SYSCALL_SYNC_EP:
		/*
		 * Create synchronous endpoint
		 */
		ret = do_create_sync_endpoint(lcd);
		lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);
		break;
	default:
		LCD_ERR("unimplemented syscall %d", syscall_id);
		ret = -ENOSYS;
	}
	return ret;
}

static int run_once(struct lcd *lcd, int *lcd_ret)
{
	int ret;

	ret = lcd_arch_run(lcd->lcd_arch);
	if (ret < 0) {
		LCD_ERR("running lcd %p", lcd);
		goto out;
	}

	switch(ret) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		LCD_ERR("page fault for lcd %p", lcd);
		ret = -ENOSYS;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		ret = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		LCD_ERR("ept fault");
		ret = -ENOSYS;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		/*
		 * Continue
		 */
		ret = 0;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		ret = lcd_handle_syscall(lcd, lcd_ret);
		goto out;
	}
	
	/*
	 * Sleep if we don't have full preemption turned on, and someone
	 * else should have a turn.
	 */
#ifndef CONFIG_PREEMPT
	cond_resched();
#endif	

out:
	return ret;
}

static int should_stop(struct lcd *lcd)
{
	int ret;

	/*
	 * Check our status
	 */
	switch(get_lcd_status(lcd)) {

	case LCD_STATUS_DEAD:
		/*
		 * We're dead; return 1 to signal to caller.
		 * (kthread_should_stop would also become true at some
		 * later point)
		 */
		ret = 1;
		goto out;
	case LCD_STATUS_RUNNING:
		/*
		 * The lcd should start or continue running; return 0
		 * to signal that
		 */
		ret = 0;
		goto out;
	default:
		BUG(); /* shouldn't be in any other state */
		ret = 1;
		goto out;
	}
out:
	return ret;
}

static int main_for_lcd(struct lcd *lcd)
{
	int ret;
	int lcd_ret = 0;
	/*
	 * Enter run loop, check after each iteration if we should stop
	 *
	 * XXX: We're not giving the thread a chance to gracefully exit
	 * for now (e.g., we could use a special upcall message to signal that 
	 * it should terminate). But maybe not ... it's up to the owners
	 * of the LCD to notify it when it should tear down?
	 */
	for (;;) {
		ret = run_once(lcd, &lcd_ret);
		if (ret < 0 || should_stop(lcd)) {
			/* ret < 0 means fatal error */
			/* lcd_should_stop means our parent told us to die */
			return ret;
		} else if (ret == 1) {
			/* lcd exited */
			return lcd_ret;
		} else {
			/* ret = 0; continue */
			continue;
		}
	}
	
	/* unreachable */
}

static int main_for_klcd(struct lcd *lcd)
{
	return lcd->klcd_main();
}

int __lcd_kthread_main(void *data) /* data is NULL */
{
	struct lcd *current_lcd = current->lcd;
	/*
	 * TODO: If the LCD exits, and it is the only one that has a
	 * reference to itself, we should tear it down.
	 */
	switch (current_lcd->type) {
	case LCD_TYPE_ISOLATED:
		return main_for_lcd(current_lcd);
	case LCD_TYPE_NONISOLATED:
		return main_for_klcd(current_lcd);
	default:
		LCD_ERR("unexpected lcd type = %d",
			current_lcd->type);
		return -EINVAL;
	}
}
