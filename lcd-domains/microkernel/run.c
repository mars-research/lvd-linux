/*
 * run.c -- Code for running LCDs
 *
 * Copyright: University of Utah
 */
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/run.h>
#include <asm/lcd_domains/create.h>

/* SYSCALL HANDLERS -------------------------------------------------- */

static int handle_syscall_create_sync_ep(struct lcd *lcd)
{
	cptr_t slot;
	/*
	 * Get slot where to store new sync endpoint
	 */
	slot = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Create sync ipc endpoint
	 */
	return __lcd_create_sync_endpoint(lcd, slot);
}

static int handle_syscall_create(struct lcd *lcd)
{
	cptr_t lcd_slot;
	/*
	 * Args
	 */
	lcd_slot = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do create
	 */
	return __lcd_create(lcd, lcd_slot);
}

static int handle_syscall_cap_grant(struct lcd *lcd)
{
	cptr_t dest_lcd, src, dest;
	/*
	 * Args
	 */
	dest_lcd = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	src = __cptr(lcd_arch_get_syscall_arg1(lcd->lcd_arch));
	dest = __cptr(lcd_arch_get_syscall_arg2(lcd->lcd_arch));
	/*
	 * Do grant
	 */
	return __lcd_cap_grant(lcd, dest_lcd, src, dest);
}

static int handle_syscall_cap_revoke(struct lcd *lcd)
{
	cptr_t object;
	/*
	 * Get object cptr
	 */
	object = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do revoke
	 */
	return cap_revoke(lcd->cspace, object);
}

static int handle_syscall_cap_delete(struct lcd *lcd)
{
	cptr_t object;
	/*
	 * Get object cptr
	 */
	object = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Delete from lcd's cspace
	 */
	cap_delete(lcd->cspace, object);

	return 0;
}

static int handle_syscall_config_registers(struct lcd *lcd)
{
	cptr_t dest_lcd;
	gva_t pc, sp;
	gpa_t gva_root, utcb_page;
	/*
	 * Args
	 */
	dest_lcd = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	pc = __gva(lcd_arch_get_syscall_arg1(lcd->lcd_arch));
	sp = __gva(lcd_arch_get_syscall_arg2(lcd->lcd_arch));
	gva_root = __gpa(lcd_arch_get_syscall_arg3(lcd->lcd_arch));
	utcb_page = __gpa(lcd_arch_get_syscall_arg4(lcd->lcd_arch));
	/*
	 * Do config
	 */
	return __lcd_config(lcd, dest_lcd, pc, sp, gva_root,
			utcb_page);
}

static int handle_syscall_memory_grant_and_map(struct lcd *lcd)
{
	cptr_t dest_lcd, mo_cptr, dest_slot;
	gpa_t base;
	/*
	 * Args
	 */
	dest_lcd = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	mo_cptr = __cptr(lcd_arch_get_syscall_arg1(lcd->lcd_arch));
	dest_slot = __cptr(lcd_arch_get_syscall_arg2(lcd->lcd_arch));
	base = __gpa(lcd_arch_get_syscall_arg3(lcd->lcd_arch));
	/*
	 * Do grant map
	 */
	return __lcd_memory_grant_and_map(lcd, dest_lcd, mo_cptr,
					dest_slot, base);
}

static int handle_syscall_munmap(struct lcd *lcd)
{
	cptr_t mem_object;
	/*
	 * Get memory object cptr
	 */
	mem_object = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do unmap
	 */
	__lcd_unmap_memory_object(lcd, mem_object);
	
	return 0;
}

static int handle_syscall_mmap(struct lcd *lcd)
{
	cptr_t mem_object;
	gpa_t base;
	/*
	 * Get memory object cptr and the base gpa where the memory
	 * object should be mapped.
	 */
	mem_object = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	base = __gpa(lcd_arch_get_syscall_arg1(lcd->lcd_arch));
	/*
	 * Do map
	 */
	return __lcd_map_memory_object(lcd, mem_object, base);
}

static int handle_syscall_vmalloc(struct lcd *lcd)
{
	cptr_t slot;
	unsigned long nr_pages;
	/*
	 * Get slot where to store alloc'd pages and order
	 */
	slot = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	nr_pages = lcd_arch_get_syscall_arg1(lcd->lcd_arch);
	/*
	 * Do vmalloc
	 */
	return __lcd_vmalloc(lcd, slot, nr_pages);
}

static int handle_syscall_pages_alloc(struct lcd *lcd)
{
	cptr_t slot;
	unsigned int flags;
	unsigned int order;
	/*
	 * Get slot where to store alloc'd pages, flags,
	 * and order
	 */
	slot = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	flags = (unsigned int)lcd_arch_get_syscall_arg1(lcd->lcd_arch);
	order = (unsigned int)lcd_arch_get_syscall_arg2(lcd->lcd_arch);
	/*
	 * Do page alloc
	 */
	return __lcd_alloc_pages(lcd, slot, flags, order);
}

static int handle_syscall_pages_alloc_exact_node(struct lcd *lcd)
{
	cptr_t slot;
	int nid;
	unsigned int flags;
	unsigned int order;
	/*
	 * Get slot where to store alloc'd pages, node id, flags,
	 * and order
	 */
	slot = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	nid = (int)lcd_arch_get_syscall_arg1(lcd->lcd_arch);
	flags = (unsigned int)lcd_arch_get_syscall_arg2(lcd->lcd_arch);
	order = (unsigned int)lcd_arch_get_syscall_arg3(lcd->lcd_arch);
	/*
	 * Do page alloc
	 */
	return __lcd_alloc_pages_exact_node(lcd, slot, nid, flags, order);
}

static int handle_syscall_sync_reply(struct lcd *lcd)
{
	/*
	 * No endpoint arg; just do reply
	 */
	return __lcd_reply(lcd);
}

static int handle_syscall_sync_call(struct lcd *lcd)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do synchronous ipc call
	 */
	return __lcd_call(lcd, endpoint);
}

static int handle_syscall_sync_recv(struct lcd *lcd)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do synchronous ipc recv
	 */
	return __lcd_recv(lcd, endpoint);
}

static int handle_syscall_sync_send(struct lcd *lcd)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Do synchronous ipc send
	 */
	return __lcd_send(lcd, endpoint);
}

static int handle_syscall_run(struct lcd *lcd)
{
	cptr_t dest_lcd;
	/*
	 * Args
	 */
	dest_lcd = __cptr(lcd_arch_get_syscall_arg0(lcd->lcd_arch));
	/*
	 * Run it
	 */
	return __lcd_run(lcd, dest_lcd);
}

static int handle_syscall_putchar(struct lcd *lcd)
{
	char c;
	/*
	 * Put char and possibly print on host
	 */
	c = (char)lcd_arch_get_syscall_arg0(lcd->lcd_arch);
	return __lcd_put_char(lcd, c);
}

static int handle_syscall_exit(struct lcd *lcd, int *lcd_ret)
{
	/*
	 * LCD's exit value in arg0
	 */
	*lcd_ret = (int)lcd_arch_get_syscall_arg0(lcd->lcd_arch);
	return 1; /* signal LCD has exited */
}

static int handle_syscall(struct lcd *lcd, int *lcd_ret)
{
	int syscall_id;
	int ret;
	
	syscall_id = lcd_arch_get_syscall_num(lcd->lcd_arch);

	LCD_DEBUG(LCD_DEBUG_VERB, 
		"got syscall %d from lcd %p", syscall_id, lcd);

	switch (syscall_id) {
	case LCD_SYSCALL_CAP_DELETE:
		ret = handle_syscall_cap_delete(lcd);
		break;
	case LCD_SYSCALL_CAP_REVOKE:
		ret = handle_syscall_cap_revoke(lcd);
		break;
	case LCD_SYSCALL_CAP_GRANT:
		ret = handle_syscall_cap_grant(lcd);
		break;
	case LCD_SYSCALL_PUTCHAR:
		ret = handle_syscall_putchar(lcd);
		break;
	case LCD_SYSCALL_CREATE:
		ret = handle_syscall_create(lcd);
		break;
	case LCD_SYSCALL_CONFIG_REGISTERS:
		ret = handle_syscall_config_registers(lcd);
		break;
	case LCD_SYSCALL_MEMORY_GRANT_AND_MAP:
		ret = handle_syscall_memory_grant_and_map(lcd);
		break;
	case LCD_SYSCALL_RUN:
		ret = handle_syscall_run(lcd);
		break;
	case LCD_SYSCALL_EXIT:
		ret = handle_syscall_exit(lcd, lcd_ret);
		break;
	case LCD_SYSCALL_SYNC_SEND:
		ret = handle_syscall_sync_send(lcd);
		break;
	case LCD_SYSCALL_SYNC_RECV:
		ret = handle_syscall_sync_recv(lcd);
		break;
	case LCD_SYSCALL_SYNC_CALL:
		ret = handle_syscall_sync_call(lcd);
		break;
	case LCD_SYSCALL_SYNC_REPLY:
		ret = handle_syscall_sync_reply(lcd);
		break;
	case LCD_SYSCALL_CREATE_SYNC_EP:
		ret = handle_syscall_create_sync_ep(lcd);
		break;
	case LCD_SYSCALL_ALLOC_PAGES_EXACT_NODE:
		ret = handle_syscall_pages_alloc_exact_node(lcd);
		break;
	case LCD_SYSCALL_ALLOC_PAGES:
		ret = handle_syscall_pages_alloc(lcd);
		break;
	case LCD_SYSCALL_VMALLOC:
		ret = handle_syscall_vmalloc(lcd);
		break;
	case LCD_SYSCALL_MMAP:
		ret = handle_syscall_mmap(lcd);
		break;
	case LCD_SYSCALL_MUNMAP:
		ret = handle_syscall_munmap(lcd);
		break;
	default:
		LCD_ERR("unimplemented syscall %d", syscall_id);
		ret = -ENOSYS;
	}
	/*
	 * Store return value so LCD can get it
	 */
	lcd_arch_set_syscall_ret(lcd->lcd_arch, ret);

	return ret;
}

/* RUN LOOP -------------------------------------------------- */

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
		ret = handle_syscall(lcd, lcd_ret);
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
		if (ret < 0) {
			/* ret < 0 means fatal error */
			LCD_MSG("LCD 0x%p exited with error %d; program counter at exit was: 0x%llx\n",
				lcd, lcd_arch_get_pc(lcd->lcd_arch));
			return ret;
		} else if (should_stop(lcd)) {
			/* ret < 0 means fatal error */
			LCD_MSG("LCD 0x%p stopped; program counter at exit was: 0x%llx\n",
				lcd, lcd_arch_get_pc(lcd->lcd_arch));
			return 0;
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

/* KTHREAD MAIN -------------------------------------------------- */

/* This is what the dedicated per-LCD kernel thread runs as its main. */

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

/* RUN -------------------------------------------------- */

int __lcd_run(struct lcd *caller, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = __lcd_get(caller, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is not in config state, fail
	 */
	if (!lcd_status_configed(lcd_struct)) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"cannot run: lcd is in state %d",
			get_lcd_status(lcd_struct));
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * This will run the kthread for the first time
	 */
	set_lcd_status(lcd_struct, LCD_STATUS_RUNNING);
	wake_up_process(lcd_struct->kthread);
	/*
	 * Unlock
	 */
	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}

/* INIT/EXIT -------------------------------------------------- */

int __lcd_run_init(void)
{
	/*
	 * Nothing for now
	 */
	return 0;
}

void __lcd_run_exit(void)
{
	/*
	 * Nothing for now
	 */
	return;
}
