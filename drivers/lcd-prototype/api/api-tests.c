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

static inline void test_check_cnode(struct cspace *cspace, cptr_t cptr,
				void *object, enum lcd_cap_type t,
				int rights)
{
	struct cnode *cnode;
	lcd_cap_lock();
	if (lcd_cnode_lookup(cspace, cptr, &cnode)) {
		LCD_ERR("looking up object at %p in cspace at %p at cptr %d",
			object, cspace, cptr);
		goto unlock;
	}
	if (__lcd_cnode_type(cnode) != t) {
		LCD_ERR("cnode has type %d, expected %d",
			__lcd_cnode_type(cnode), t);
		goto unlock;
	}
	if (__lcd_cnode_rights(cnode) != rights) {
		LCD_ERR("cnode has rights %d, expected %d",
			__lcd_cnode_rights(cnode), rights);
		goto unlock;
	}
	if (__lcd_cnode_object(cnode) != object) {
		LCD_ERR("cnode has object %p, expected %p",
			__lcd_cnode_object(cnode), object);
		goto unlock;
	}
unlock:
	lcd_cap_unlock();
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

static int test04(void)
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
	if (lcd_cap_alloc(cspace1, &cptr1)) {
		LCD_ERR("alloc cptr1");
		goto fail3;
	}
	if (lcd_cap_alloc(cspace2, &cptr2)) {
		LCD_ERR("alloc cptr2");
		goto fail3;
	}
	if (lcd_cap_alloc(cspace3, &cptr3)) {
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
	 * Grant write and read to cspace 2
	 */
	if (lcd_cnode_grant(cspace1, cspace2, cptr1, cptr2,
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
	test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_ALL);
	test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_WRITE | LCD_CAP_RIGHT_READ);
	test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_READ);
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
	test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_ALL);
	test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_WRITE);
	test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_SYNC_EP, 0);

	/*
	 * Free cap
	 */
	lcd_cap_lock();
	if (lcd_cnode_lookup(cspace1, cptr1, &cnode)) {
		LCD_ERR("cspace1 cnode lookup");
		goto fail3_unlock;
	}
	__lcd_cnode_free(cnode);
	lcd_cap_unlock();

	/*
	 * Check
	 */
	test_check_cnode(cspace1, cptr1, &x, LCD_CAP_TYPE_FREE, 0);
	test_check_cnode(cspace2, cptr2, &x, LCD_CAP_TYPE_FREE, 0);
	test_check_cnode(cspace3, cptr3, &x, LCD_CAP_TYPE_FREE, 0);

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
	LCD_MSG("all api tests passed!");
	return 0;
}
