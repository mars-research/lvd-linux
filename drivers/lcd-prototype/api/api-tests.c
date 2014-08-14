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

static int test01(void)
{
	struct cspace *cspace;
	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

static int test02(void)
{
	struct cspace *cspace;
	int ret;
	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	ret = lcd_lock_cspace(cspace);
	if (ret) {
		lcd_rm_cspace(cspace);
		LCD_FAIL("lock cspace returned %d", ret);
	}
	lcd_unlock_cspace(cspace);
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

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
	LCD_MSG("all api tests passed!");
	return 0;
}
