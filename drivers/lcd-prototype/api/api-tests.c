/* 
 * api-tests.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include "../include/common.h"
#include "defs.h"

#define LCD_FAIL(msg...) do{ LCD_ERR(msg); return -1; } while(0)
#define LCD_PASS() do{ return 0; } while(0)

static inline int test_rm_cspace(struct cspace *cspace, int ret_val)
{
	int ret;
	ret = lcd_cap_lock();
	if (ret) {
		LCD_ERR("lock failed with %d", ret);
		return ret;
	}
	__lcd_rm_cspace(&cspace);
	lcd_cap_unlock();
	return ret_val;	
}

static inline int test_check_cnode(struct cspace *cspace, cptr_t cptr,
				void *object, enum lcd_cap_type t,
				int rights)
{
	struct cnode *cnode;

	if (lcd_cap_lock())
		return -1;
	if (__lcd_cnode_lookup(cspace, cptr, &cnode)) {
		LCD_ERR("looking up object at %p in cspace at %p at cptr %d",
			object, cspace, cptr);
		goto fail;
	}
	if (__lcd_cnode_type(cnode) != t) {
		LCD_ERR("cnode has type %d, expected %d",
			__lcd_cnode_type(cnode), t);
		goto fail;
	}
	if (__lcd_cnode_rights(cnode) != rights) {
		LCD_ERR("cnode has rights %d, expected %d",
			__lcd_cnode_rights(cnode), rights);
		goto fail;
	}
	if (__lcd_cnode_object(cnode) != object) {
		LCD_ERR("cnode has object %p, expected %p",
			__lcd_cnode_object(cnode), object);
		goto fail;
	}

	lcd_cap_unlock();
	return 0;

fail:
	lcd_cap_unlock();
	return -1;
}

static inline struct lcd * test_mk_lcd(void)
{
	struct lcd *lcd;
	int ret;

	lcd = kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd)
		return NULL;

	ret = lcd_mk_cspace(&lcd->cspace);
	if (ret) {
		kfree(lcd);
		return NULL;
	}
	
	return lcd;
}

static inline void test_rm_lcd(struct lcd *lcd)
{
	__lcd_rm_cspace(&lcd->cspace);
	kfree(lcd);
}

static int test01(void)
{
	struct cspace *cspace;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	if (!cspace)
		LCD_FAIL("cspace null");

	if (lcd_cap_lock())
		return -1;
	__lcd_rm_cspace(&cspace);
	lcd_cap_unlock();

	if (cspace)
		LCD_FAIL("cspace non-null");

	LCD_PASS();
}

static int test02(void)
{
	struct cspace *cspace;
	struct cnode *cnode;
	cptr_t cptr;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	if (lcd_cnode_alloc(cspace, &cptr)) {
		LCD_ERR("alloc");
		goto fail;
	}
	if (cptr >= LCD_NUM_CAPS) {
		LCD_ERR("cptr = %d >= %d = LCD_NUM_CAPS", 
			cptr, LCD_NUM_CAPS);
		goto fail;
	}
	/*
	 * LOCK cap
	 */
	if (lcd_cap_lock())
		goto fail;
	if (__lcd_cnode_lookup(cspace, cptr, &cnode)) {
		LCD_ERR("lookup");
		goto fail_unlock;
	}
	if (!__lcd_cnode_is_unformed(cnode)) {
		LCD_ERR("cnode not unformed, type is %d", cnode->type);
		goto fail_unlock;
	}
	if (__lcd_cnode_rights(cnode) != 0) {
		LCD_ERR("cnode rights non-zero");
		goto fail_unlock;
	}
	if (cnode->cspace != cspace) {
		LCD_ERR("cnode bad parent");
		goto fail_unlock;
	}
	if (!list_empty(&cnode->free_list)) {
		LCD_ERR("cnode still in free list?");
		goto fail_unlock;
	}
	if (!list_empty(&cnode->children)) {
		LCD_ERR("cnode children non-empty");
		goto fail_unlock;
	}
	if (!list_empty(&cnode->child_list)) {
		LCD_ERR("cnode child list non-empty");
		goto fail_unlock;
	}
	/*
	 * UNLOCK cap
	 */
	__lcd_rm_cspace(&cspace);
	lcd_cap_unlock();
	return 0;

fail_unlock:
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();			
fail:
	return test_rm_cspace(cspace, -1);
}

static int test03(void)
{
	struct cspace *cspace;
	struct cnode *cnode;
	cptr_t cptr;
	int x;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	if (lcd_cnode_alloc(cspace, &cptr)) {
		LCD_ERR("alloc");
		goto fail;
	}
	/*
	 * Insert object
	 */
	x = 517;
	if (lcd_cnode_insert(cspace, cptr, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_WRITE)) {
		LCD_ERR("insert");
		goto fail;
	}

	/*
	 * LOCK cap
	 */
	if (lcd_cap_lock())
		goto fail;
	if (__lcd_cnode_lookup(cspace, cptr, &cnode)) {
		LCD_ERR("lookup");
		goto fail_unlock;
	}
	if (!__lcd_cnode_is_sync_ep(cnode)) {
		LCD_ERR("cnode not sync ep");
		goto fail_unlock;
	}
	if (__lcd_cnode_rights(cnode) != LCD_CAP_RIGHT_WRITE) {
		LCD_ERR("cnode rights not write");
		goto fail_unlock;
	}
	if (__lcd_cnode_object(cnode) != &x) {
		LCD_ERR("cnode bad object");
		goto fail_unlock;
	}

	__lcd_rm_cspace(&cspace);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return 0;

fail_unlock:
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();			
fail:
	return test_rm_cspace(cspace, -1);
}

static int test04(void)
{
	struct cspace *cspace;
	struct cnode *cnode;
	cptr_t cptr;
	int x;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	if (lcd_cnode_alloc(cspace, &cptr)) {
		LCD_ERR("alloc");
		goto fail;
	}
	/*
	 * Insert object
	 */
	x = 517;
	if (lcd_cnode_insert(cspace, cptr, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_WRITE)) {
		LCD_ERR("insert");
		goto fail;
	}

	/*
	 * LOCK cap
	 */
	if (lcd_cap_lock())
		goto fail;
	if (__lcd_cnode_lookup(cspace, cptr, &cnode)) {
		LCD_ERR("lookup");
		goto fail_unlock;
	}
	if (!__lcd_cnode_is_sync_ep(cnode)) {
		LCD_ERR("cnode not sync ep");
		goto fail_unlock;
	}
	if (__lcd_cnode_rights(cnode) != LCD_CAP_RIGHT_WRITE) {
		LCD_ERR("cnode rights not write");
		goto fail_unlock;
	}
	if (__lcd_cnode_object(cnode) != &x) {
		LCD_ERR("cnode bad object");
		goto fail_unlock;
	}

	__lcd_rm_cspace(&cspace);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return 0;

fail_unlock:
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();			
fail:
	return test_rm_cspace(cspace, -1);
}

static int test05(void)
{
	struct cspace *cspace1;
	struct cspace *cspace2;
	struct cspace *cspace3;
	cptr_t cptr1;
	cptr_t cptr2;
	cptr_t cptr3;
	int x;
	struct cnode *cnode;

	/*
	 * MAKE CSPACES
	 */
	if (lcd_mk_cspace(&cspace1))
		LCD_FAIL("mk cspace1");
	if (lcd_mk_cspace(&cspace2)) {
		LCD_ERR("mk cspace2");
		goto fail1;
	}
	if (lcd_mk_cspace(&cspace3)) {
		LCD_ERR("mk cspace3");
		goto fail2;
	}

	/*
	 * ALLOC SLOTS IN EACH FOR CAP
	 */
	if (lcd_cnode_alloc(cspace1, &cptr1)) {
		LCD_ERR("alloc cptr1");
		goto fail3;
	}
	if (lcd_cnode_alloc(cspace2, &cptr2)) {
		LCD_ERR("alloc cptr2");
		goto fail3;
	}
	if (lcd_cnode_alloc(cspace3, &cptr3)) {
		LCD_ERR("alloc cptr3");
		goto fail3;
	}
	
	/*
	 * INSERT OBJECT INTO CSPACE 1
	 */
	x = 519;
	if (lcd_cnode_insert(cspace1, cptr1, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_ALL)) {
		LCD_ERR("insert cspace1");
		goto fail3;
	}

	/*
	 * GRANT RIGHTS
	 */

	/*
	 * Grant grant, write and read to cspace 2
	 */
	if (lcd_cnode_grant(cspace1, cspace2, cptr1, cptr2,
				LCD_CAP_RIGHT_GRANT | 
				LCD_CAP_RIGHT_WRITE | LCD_CAP_RIGHT_READ)) {
		LCD_ERR("grant 1 to 2");
		goto fail3;
	}
	/*
	 * Grant read to cspace 3
	 */
	if (lcd_cnode_grant(cspace2, cspace3, cptr2, cptr3,
				LCD_CAP_RIGHT_READ)) {
		LCD_ERR("grant 2 to 3");
		goto fail3;
	}

	/*
	 * Check
	 */
	if (test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_ALL))
		goto fail3;
	if (test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_GRANT | 
				LCD_CAP_RIGHT_WRITE | LCD_CAP_RIGHT_READ))
		goto fail3;
	if (test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_READ))
		goto fail3;
	/*
	 * Revoke read
	 */
	if (lcd_cnode_revoke(cspace1, cptr1, LCD_CAP_RIGHT_READ)) {
		LCD_ERR("revoke read");
		goto fail3;
	}
	/*
	 * Check
	 */
	if (test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_ALL))
		goto fail3;
	if (test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_SYNC_EP,
				LCD_CAP_RIGHT_GRANT | LCD_CAP_RIGHT_WRITE))
		goto fail3;
	if (test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_SYNC_EP, 0))
		goto fail3;

	/*
	 * Free cap
	 */
	if (lcd_cap_lock())
		goto fail3;
	if (__lcd_cnode_lookup(cspace1, cptr1, &cnode)) {
		LCD_ERR("cspace1 cnode lookup");
		goto fail3_unlock;
	}
	__lcd_cnode_free(cnode);
	lcd_cap_unlock();

	/*
	 * Check
	 */
	if (test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_FREE, 0))
		goto fail3;
	if (test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_FREE, 0))
		goto fail3;
	if (test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_FREE, 0))
		goto fail3;

	/*
	 * rm cspaces
	 */
	
	test_rm_cspace(cspace1, 0);
	test_rm_cspace(cspace2, 0);
	test_rm_cspace(cspace3, 0);
	
	LCD_PASS();


fail3_unlock:
	lcd_cap_unlock();
fail3:
	test_rm_cspace(cspace3, -1);
fail2:
	test_rm_cspace(cspace2, -1);
fail1:
	return test_rm_cspace(cspace1, -1);
}

struct test06_info {
	struct lcd *lcd;
	cptr_t ep_cptr;
	struct completion c;
	int ret_val;
};

static int test06_thread1(void *__info)
{
	struct test06_info *info;

	info = (struct test06_info *)__info;
	
	info->ret_val = __lcd_recv(info->lcd, info->ep_cptr);

	complete(&info->c);
	
	return 0;
}

static int test06_thread2(void *__info)
{
	struct test06_info *info;

	info = (struct test06_info *)__info;

	info->ret_val = __lcd_send(info->lcd, info->ep_cptr);

	complete(&info->c);

	return 0;
}

static int test06(void)
{
	struct lcd *lcd1;
	struct lcd *lcd2;
	struct task_struct *lcd1_task;
	struct task_struct *lcd2_task;
	struct test06_info info1;
	struct test06_info info2;
	cptr_t cptr1;
	cptr_t cptr2;
	int ret;

	lcd1 = test_mk_lcd();
	if (!lcd1) {
		ret = -1;
		goto clean1;
	}
	lcd2 = test_mk_lcd();
	if (!lcd2) {
		ret = -1;
		goto clean2;
	}

	/*
	 * Allocate a cnode and endpoint
	 */
	ret = lcd_cnode_alloc(lcd1->cspace, &cptr1);
	if (ret)
		goto clean3;
	ret = __lcd_mk_sync_endpoint(lcd1, cptr1);
	if (ret) {
		LCD_ERR("mk sync endpoint");
		goto clean3;
	}
	/*
	 * Grant send rights to lcd2
	 */
	ret = lcd_cnode_alloc(lcd2->cspace, &cptr2);
	if (ret)
		goto clean4;
	ret = lcd_cnode_grant(lcd1->cspace, lcd2->cspace, cptr1, cptr2,
			LCD_CAP_RIGHT_WRITE);
	if (ret)
		goto clean4;
	/*
	 * Set up message for lcd2 --> lcd1
	 */
	lcd2->utcb.regs[0] = 12345;
	lcd2->utcb.max_valid_reg_idx = 1;
	lcd2->utcb.max_valid_out_cap_reg_idx = 0;
	lcd2->utcb.max_valid_in_cap_reg_idx = 0;

	info1.lcd = lcd1;
	info1.ep_cptr = cptr1;
	init_completion(&info1.c);

	lcd1_task = kthread_create(test06_thread1, &info1, "test06_thread1");
	if (!lcd1_task) {
		LCD_ERR("spawning lcd1 task");
		goto clean4;
	}

	info2.lcd = lcd2;
	info2.ep_cptr = cptr2;
	init_completion(&info2.c);

	lcd2_task = kthread_create(test06_thread2, &info2, "test06_thread2");
	if (!lcd2_task) {
		LCD_ERR("spawning lcd2 task");

		/* cancel thread1 (before it even starts) */
		kthread_stop(lcd1_task);
		goto clean4;
	}

	/* set tasks' lcds */
	lcd1->parent = lcd1_task;
	lcd2->parent = lcd2_task;

	/* coast clear, wake up both tasks */
	wake_up_process(lcd1_task);
	wake_up_process(lcd2_task);

	/* wait for kthreads to complete, and get ret vals */
	/* (using completions rather than kthread_stop to avoid hangs) */
	wait_for_completion_interruptible(&info1.c);
	wait_for_completion_interruptible(&info2.c);

	/* send / recv should be done; check result. */
	if (info1.ret_val != 0) {
		LCD_ERR("lcd1 non-zero ret_val = %d", info1.ret_val);
		ret = -1;
		goto clean4;
	}
	if (info2.ret_val != 0) {
		LCD_ERR("lcd2 non-zero ret_val = %d", info2.ret_val);
		ret = -1;
		goto clean4;
	}
	if (lcd1->utcb.regs[0] != 12345) {
		LCD_ERR("lcd1 reg 0 is %u", lcd1->utcb.regs[0]);
		ret = -1;
		goto clean4;
	}

	ret = 0;
	goto clean4;

clean4:
	__lcd_rm_sync_endpoint(lcd1, cptr1);
clean3:
	test_rm_lcd(lcd2);
clean2:
	test_rm_lcd(lcd1);
clean1:
	return ret;
}

struct test07_info {
	struct lcd *lcd;
	cptr_t ep_cptr;
	struct completion c;
	int ret_val;
};

static int test07_thread1(void *__info)
{
	struct test07_info *info;
	int ret;

	info = (struct test07_info *)__info;
	
	ret = __lcd_recv(info->lcd, info->ep_cptr);
	if (ret) {
		info->ret_val = ret;		
		complete(&info->c);
		return 0;
	}

	info->lcd->utcb.regs[0] += 100;
	info->lcd->utcb.max_valid_reg_idx = 1;
	info->lcd->utcb.max_valid_out_cap_reg_idx = 0;
	info->lcd->utcb.max_valid_in_cap_reg_idx = 0;

	info->ret_val = __lcd_reply(info->lcd);

	complete(&info->c);
	
	return 0;
}

static int test07_thread2(void *__info)
{
	struct test07_info *info;

	info = (struct test07_info *)__info;

	/*
	 * do call
	 */
	info->ret_val = __lcd_call(info->lcd, info->ep_cptr);

	complete(&info->c);

	return 0;
}

static int test07(void)
{
	struct lcd *lcd1;
	struct lcd *lcd2;
	struct task_struct *lcd1_task;
	struct task_struct *lcd2_task;
	struct test07_info info1;
	struct test07_info info2;
	cptr_t cptr1a;
	cptr_t cptr2a;
	int ret;

	lcd1 = test_mk_lcd();
	if (!lcd1) {
		ret = -1;
		goto clean1;
	}
	lcd2 = test_mk_lcd();
	if (!lcd2) {
		ret = -1;
		goto clean2;
	}

	/*
	 * Allocate a cnode and endpoint
	 */
	ret = lcd_cnode_alloc(lcd1->cspace, &cptr1a);
	if (ret)
		goto clean3;
	ret = __lcd_mk_sync_endpoint(lcd1, cptr1a);
	if (ret) {
		LCD_ERR("mk sync endpoint");
		goto clean3;
	}
	/*
	 * Grant send rights to lcd2
	 */
	ret = lcd_cnode_alloc(lcd2->cspace, &cptr2a);
	if (ret)
		goto clean4;
	ret = lcd_cnode_grant(lcd1->cspace, lcd2->cspace, cptr1a, cptr2a,
			LCD_CAP_RIGHT_WRITE);
	if (ret)
		goto clean4;
	/*
	 * Set up message for lcd2 --> lcd1
	 */
	lcd2->utcb.regs[0] = 5678;
	lcd2->utcb.max_valid_reg_idx = 1;
	lcd2->utcb.max_valid_out_cap_reg_idx = 0;
	lcd2->utcb.max_valid_in_cap_reg_idx = 0;

	/*
	 * Spawn threads ...
	 */

	info1.lcd = lcd1;
	info1.ep_cptr = cptr1a;
	init_completion(&info1.c);

	lcd1_task = kthread_create(test07_thread1, &info1, "test07_thread1");
	if (!lcd1_task) {
		LCD_ERR("spawning lcd1 task");
		goto clean4;
	}

	info2.lcd = lcd2;
	info2.ep_cptr = cptr2a;
	init_completion(&info2.c);

	lcd2_task = kthread_create(test07_thread2, &info2, "test07_thread2");
	if (!lcd2_task) {
		LCD_ERR("spawning lcd2 task");

		/* cancel thread1 (before it even starts) */
		kthread_stop(lcd1_task);
		goto clean4;
	}

	/* set tasks' lcds */
	lcd1->parent = lcd1_task;
	lcd2->parent = lcd2_task;

	/* coast clear, wake up both tasks */
	wake_up_process(lcd1_task);
	wake_up_process(lcd2_task);

	/* wait for kthreads to complete, and get ret vals */
	/* (using completions rather than kthread_stop to avoid hangs) */
	wait_for_completion_interruptible(&info1.c);
	wait_for_completion_interruptible(&info2.c);

	/* send / recv should be done; check result. */
	if (info1.ret_val != 0) {
		LCD_ERR("lcd1 non-zero ret_val = %d", info1.ret_val);
		ret = -1;
		goto clean4;
	}
	if (info2.ret_val != 0) {
		LCD_ERR("lcd2 non-zero ret_val = %d", info2.ret_val);
		ret = -1;
		goto clean4;
	}
	if (lcd1->utcb.regs[0] != 5778) {
		LCD_ERR("lcd1 reg 0 is %u", lcd1->utcb.regs[0]);
		ret = -1;
		goto clean4;
	}
	if (lcd2->utcb.regs[0] != 5778) {
		LCD_ERR("lcd2 reg 0 is %u", lcd2->utcb.regs[0]);
		ret = -1;
		goto clean4;
	}

	ret = 0;
	goto clean4;

clean4:
	__lcd_rm_sync_endpoint(lcd1, cptr1a);
clean3:
	test_rm_lcd(lcd2);
clean2:
	test_rm_lcd(lcd1);
clean1:
	return ret;
}

int api_tests(void)
{
	if (test01())
		return -1;
	if (test02())
		return -1;
	if (test03())
		return -1;
	if (test04())
		return -1;
	if (test05())
		return -1;
	if (test06())
		return -1;
	if (test07())
		return -1;
	LCD_MSG("all api tests passed!");
	return 0;
}
