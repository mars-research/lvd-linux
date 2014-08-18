/* 
 * api-tests.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>
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

static int test01(void)
{
	struct cspace *cspace;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	if (!cspace)
		LCD_FAIL("cspace null");

	lcd_cap_lock();
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
	lcd_cap_lock();
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
	lcd_cap_lock();
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

#if 0
static int test03(void)
{
	struct cspace *cspace;
	cptr_t cptr;
	int i;

	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");

	for (i = 0; i < 5; i++) {
		if (lcd_cap_alloc(cspace, &cptr)) {
			lcd_rm_cspace(cspace);
			LCD_FAIL("cap alloc");
		}
		if (cptr != i) {
			lcd_rm_cspace(cspace);
			LCD_FAIL("i = %d, cptr = %lld", i, cptr);
		}
	}
	if (cspace->free_slot != 5) {
		lcd_rm_cspace(cspace);
		LCD_FAIL("free slot is %d", cspace->free_slot);
	}
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

static int test04(void)
{
	struct cspace *cspace;
	cptr_t cptr;
	int i;
	int x;
	struct cnode *cnode;
	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	for (i = 0; i < 5; i++) {
		if (lcd_cap_alloc(cspace, &cptr)) {
			lcd_rm_cspace(cspace);
			LCD_FAIL("cap alloc");
		}
	}
	/* cptr = 4 */
	x = 517;
	if (lcd_cap_insert(cspace, cptr, &x, LCD_CAP_TYPE_SYNC_EP)) {
		lcd_rm_cspace(cspace);
		LCD_FAIL("insert");
	}
	if (lcd_cnode_lookup(cspace, cptr, &cnode)) {
		lcd_rm_cspace(cspace);
		LCD_FAIL("lookup");
	}
	if (cnode->type != LCD_CAP_TYPE_SYNC_EP ||
		cnode->object != &x) {
		lcd_rm_cspace(cspace);
		LCD_FAIL("cnode type = %d, object addr = %p",
			cnode->type, cnode->object);
	}
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

static int test05(void)
{
	struct cspace *src_cspace;
	struct cspace *dest_cspace;
	cptr_t src_cptr, dest_cptr;
	int i;
	int x;
	struct cnode *cnode;

	if (lcd_mk_cspace(&src_cspace))
		LCD_FAIL("mk src cspace");
	if (lcd_mk_cspace(&dest_cspace))
		LCD_FAIL("mk dest cspace");
	for (i = 0; i < 5; i++) {
		if (lcd_cap_alloc(src_cspace, &src_cptr)) {
			lcd_rm_cspace(src_cspace);
			lcd_rm_cspace(dest_cspace);
			LCD_FAIL("src cap alloc");
		}
	}
	for (i = 0; i < 3; i++) {
		if (lcd_cap_alloc(dest_cspace, &dest_cptr)) {
			lcd_rm_cspace(src_cspace);
			lcd_rm_cspace(dest_cspace);
			LCD_FAIL("dest cap alloc");
		}
	}
	/* src_cptr = 4, dest_ctpr = 2 */
	x = 517;
	if (lcd_cap_insert(src_cspace, src_cptr, &x, LCD_CAP_TYPE_SYNC_EP)) {
		lcd_rm_cspace(src_cspace);
		lcd_rm_cspace(dest_cspace);
		LCD_FAIL("insert");
	}
	if (lcd_cap_grant(src_cspace, dest_cspace, src_cptr, dest_cptr)) {
		lcd_rm_cspace(src_cspace);
		lcd_rm_cspace(dest_cspace);
		LCD_FAIL("grant");
	}
	if (lcd_cnode_lookup(dest_cspace, dest_cptr, &cnode)) {
		lcd_rm_cspace(src_cspace);
		lcd_rm_cspace(dest_cspace);
		LCD_FAIL("lookup");
	}
	if (cnode->type != LCD_CAP_TYPE_SYNC_EP ||
		cnode->object != &x) {
		lcd_rm_cspace(src_cspace);
		lcd_rm_cspace(dest_cspace);
		LCD_FAIL("cnode type = %d, object addr = %p",
			cnode->type, cnode->object);
	}
	lcd_rm_cspace(src_cspace);
	lcd_rm_cspace(dest_cspace);
	LCD_PASS();
}

#endif

int api_tests(void)
{
	if (test01())
		return -1;
	if (test02())
		return -1;
	if (test03())
		return -1;
	/* if (test04()) */
	/* 	return -1; */
	/* if (test05()) */
	/* 	return -1; */
	LCD_MSG("all api tests passed!");
	return 0;
}
