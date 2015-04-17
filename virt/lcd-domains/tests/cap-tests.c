/**
 * Regression tests for cap.c.
 */

static int test01(void)
{
	struct cspace c;
	int ret;
	ret = __lcd_cap_init_cspace(&c);
	if (ret) {
		LCD_ERR("init cspace");
		return ret;
	}
	__lcd_cap_destroy_cspace(&c);

	return 0;
}

static int test02(void)
{
	struct cspace c;
	int ret;
	struct cnode *cnode1, *cnode2;
	cptr_t cp = __cptr((1 << LCD_CPTR_SLOT_BITS) - 1);
	ret = __lcd_cap_init_cspace(&c);
	if (ret) {
		LCD_ERR("init cspace");
		goto fail1;
	}
	ret = __lcd_cnode_lookup(&c, cp, true, &cnode1);
	if (ret) {
		LCD_ERR("lookup");
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&c, cp, true, &cnode2);
	if (ret) {
		LCD_ERR("lookup");
		goto fail3;
	}
	if (cnode1 != cnode2) {
		LCD_ERR("got two different cnodes");
		goto fail4;
	}
	if (cnode1->type != LCD_CAP_TYPE_FREE) {
		LCD_ERR("not free");
		goto fail5;
	}
	
	__lcd_cap_destroy_cspace(&c);

	return 0;
fail5:
fail4:
fail3:
fail2:
	__lcd_cap_destroy_cspace(&c);
fail1:
	return ret;
}

static int test03(void)
{
	struct cnode cnode;
	struct cdt_root_node root;
	int ret;
	/*
	 * Tree:
	 *
	 *   0  (single node)
	 */
	INIT_LIST_HEAD(&cnode.children);
	INIT_LIST_HEAD(&cnode.siblings);
	root.cnode = &cnode;
	ret = do_delete_from_cdt(&cnode, &root);
	if (ret != 1) {
		LCD_ERR("didn't say last node");
		ret = -1;
		goto fail1;
	}
	if (root.cnode) {
		LCD_ERR("didn't delete cnode");
		ret = -1;
		goto fail1;
	}

	return 0;

fail1:
	return ret;
}

static int test04(void)
{
	struct cnode *cnodes;
	struct cdt_root_node root;
	int ret;
	int i;
	int num_nodes = 3;
	/*
	 * Tree:
	 *
	 *   0 ----> 1
	 *    \
	 *     `---> 2
	 *
	 * Removing zero leaves 1 and 2 in the tree.
	 */
	cnodes = kzalloc(sizeof(*cnodes) * num_nodes, GFP_KERNEL);
	if (!cnodes) {
		LCD_ERR("cnodes alloc");
		ret = -1;
		goto fail1;
	}
	for (i = 0; i < num_nodes; i++) {
		INIT_LIST_HEAD(&cnodes[i].children);
		INIT_LIST_HEAD(&cnodes[i].siblings);
	}
	/*
	 * Set up tree
	 */
	list_add(&cnodes[1].siblings, &cnodes[0].children);
	list_add(&cnodes[2].siblings, &cnodes[0].children);
	root.cnode = &cnodes[0];
	/*
	 * Delete cnode 0 from tree
	 */
	ret = do_delete_from_cdt(&cnodes[0], &root);
	if (ret) {
		LCD_ERR("non zero ret = %d", ret);
		goto fail2;
	}
	/*
	 * Checks
	 */
	if (root.cnode != &cnodes[1] && root.cnode != &cnodes[2]) {
		LCD_ERR("incorrect root cnode");
		ret = -1;
		goto fail2;
	}
	if (!list_empty(&cnodes[0].children)) {
		LCD_ERR("cnode 0 still has children");
		ret = -1;
		goto fail2;
	}
	if (!list_is_singular(&cnodes[1].siblings) ||
		list_first_entry(&cnodes[1].siblings, struct cnode, siblings) !=
		&cnodes[2]) {
		LCD_ERR("bad sibling list");
		ret = -1;
		goto fail2;
	}
	if (!list_is_singular(&cnodes[2].siblings) ||
		list_first_entry(&cnodes[2].siblings, struct cnode, siblings) !=
		&cnodes[1]) {
		LCD_ERR("bad sibling list");
		ret = -1;
		goto fail2;
	}

	kfree(cnodes);

	return 0;

fail2:
	kfree(cnodes);
fail1:
	return ret;
}


static int test05(void)
{
	int ret;
	struct cnode *cnode;
	struct page *p;
	cptr_t cp = __cptr((1 << LCD_CPTR_SLOT_BITS) - 1);

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail0;
	}
	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("enter klcd");
		__free_pages(p, 0);
		goto fail0;
	}
	ret = __lcd_cap_insert(&current->lcd->cspace, 
			cp, p, LCD_CAP_TYPE_PAGE);
	if (ret) {
		LCD_ERR("insert");
		__free_pages(p, 0);
		goto fail1;
	}
	ret = __lcd_cnode_lookup(&current->lcd->cspace, 
				cp, false, &cnode);
	if (ret) {
		LCD_ERR("lookup");
		goto fail1; /* page should be tracked and will be freed when
			     * klcd exits */
	}
	if (cnode->type != LCD_CAP_TYPE_PAGE) {
		LCD_ERR("not free");
		goto fail1;
	}
	if (cnode->object != p) {
		LCD_ERR("wrong object");
		goto fail1;
	}
	
	__klcd_exit();

	return 0;

fail1:
	__klcd_exit();
fail0:
	return ret;
}

static int test06(void)
{
	struct cnode *cnodes;
	struct cdt_root_node root;
	struct list_head *cursor;
	int ret;
	int i;
	int num_nodes = 4;
	/*
	 * Tree:
	 *
	 *  0 is root, 1 is 0's sibling. 2 is 0's only child. 3 and 4 are 1's
	 *  children.
	 *
	 *   0 -------> 1 ----> 2
	 *               \
	 *                `---> 3
	 *
	 * Removing 1 makes 2 and 3 the children of 0.
	 */
	cnodes = kzalloc(sizeof(*cnodes) * num_nodes, GFP_KERNEL);
	if (!cnodes) {
		LCD_ERR("cnodes alloc");
		ret = -1;
		goto fail1;
	}
	for (i = 0; i < num_nodes; i++) {
		INIT_LIST_HEAD(&cnodes[i].children);
		INIT_LIST_HEAD(&cnodes[i].siblings);
	}
	/*
	 * Set up tree
	 */
	list_add(&cnodes[1].siblings, &cnodes[0].children);
	list_add(&cnodes[2].siblings, &cnodes[1].children);
	list_add(&cnodes[3].siblings, &cnodes[1].children);
	root.cnode = &cnodes[0];
	/*
	 * Delete cnode 1 from tree
	 */
	ret = do_delete_from_cdt(&cnodes[1], &root);
	if (ret) {
		LCD_ERR("non zero ret = %d", ret);
		goto fail2;
	}
	/*
	 * Checks
	 */
	if (root.cnode != &cnodes[0]) {
		LCD_ERR("incorrect root cnode");
		ret = -1;
		goto fail2;
	}
	if (list_empty(&cnodes[0].children)) {
		LCD_ERR("cnode 0 has no children");
		ret = -1;
		goto fail2;
	}
	list_for_each(cursor, &cnodes[0].children) {
		if (cursor != &cnodes[2].siblings &&
			cursor != &cnodes[3].siblings) {
			LCD_ERR("cnode 0 has unexpected children");
			ret = -1;
			goto fail2;
		}
	}

	kfree(cnodes);

	return 0;

fail2:
	kfree(cnodes);
fail1:
	return ret;
}

static int test07(void)
{
	struct lcd *lcds[7];
	int i, j;
	int ret;
	struct page *p;
	struct cnode *cnode;
	/*
	 * Assume certain cptr layout for simplicity
	 */
	cptr_t cp0 = __cptr(1);
	cptr_t cp1 = __cptr(4 | (1 << LCD_CPTR_LEVEL_SHIFT));
	cptr_t cp2 = __cptr(0xb | (1 << LCD_CPTR_LEVEL_SHIFT));
	cptr_t cp3 = __cptr(0x30 | 0x3 | (2 << LCD_CPTR_LEVEL_SHIFT));
	cptr_t cp4 = __cptr(0x30 | 0x1 | (2 << LCD_CPTR_LEVEL_SHIFT));
	cptr_t cp5 = __cptr(0xFF | (3 << LCD_CPTR_LEVEL_SHIFT));
	cptr_t cp6 = __cptr(3);

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("page alloc");
		ret = -ENOMEM;
		goto fail0;
	}

	for (i = 0; i < 7; i++) {
		ret = __lcd_create__(&lcds[i]);
		if (ret) {
			LCD_ERR("lcd create");
			__free_pages(p, 0);
			goto fail1;
		}
	}
	/*
	 * Insert into 0's cspace
	 */
	ret = __lcd_cap_insert(&lcds[0]->cspace, cp0, p,
			LCD_CAP_TYPE_PAGE);
	if (ret) {
		LCD_ERR("insert");
		__free_pages(p, 0);
		goto fail2;
	}
	/*
	 * Grant to 1 and 2 (so they are children of 0)
	 */
	ret = __lcd_cap_grant(&lcds[0]->cspace, cp0,
			&lcds[1]->cspace, cp1);
	if (ret) {
		LCD_ERR("grant 0 to 1");
		goto fail2; /* page is tracked since we inserted it, so will
			     * be freed when we destroy all lcds */
	}
	ret = __lcd_cap_grant(&lcds[0]->cspace, cp0,
			&lcds[2]->cspace, cp2);
	if (ret) {
		LCD_ERR("grant 0 to 2");
		goto fail2;
	}
	/*
	 * Grant 1 -> 3 & 4, 2 -> 5 & 6
	 */
	ret = __lcd_cap_grant(&lcds[1]->cspace, cp1,
			&lcds[3]->cspace, cp3);
	if (ret) {
		LCD_ERR("grant 1 to 3");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[1]->cspace, cp1,
			&lcds[4]->cspace, cp4);
	if (ret) {
		LCD_ERR("grant 1 to 4");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[2]->cspace, cp2,
			&lcds[5]->cspace, cp5);
	if (ret) {
		LCD_ERR("grant 2 to 5");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[2]->cspace, cp2,
			&lcds[6]->cspace, cp6);
	if (ret) {
		LCD_ERR("grant 2 to 6");
		goto fail2;
	}
	/*
	 * 0 does a revoke
	 */
	ret = __lcd_cap_revoke(&lcds[0]->cspace, cp0);
	if (ret) {
		LCD_ERR("revoke");
		goto fail2;
	}
	/*
	 * Make sure 0 still has access
	 */
	ret = __lcd_cnode_lookup(&lcds[0]->cspace, cp0, false, &cnode);
	if (ret) {
		LCD_ERR("cnode lookup");
		goto fail2;
	}
	if (cnode->type != LCD_CAP_TYPE_PAGE ||
		cnode->object != p) {
		LCD_ERR("bad cnode in 0");
		ret = -1;
		goto fail2;
	}
	/*
	 * Make sure none of 1 - 6 have access
	 *
	 * XXX: this relies on us not cleaning up unused cnode tables, 
	 * may change in the future ...
	 */
	ret = __lcd_cnode_lookup(&lcds[1]->cspace, cp1, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[2]->cspace, cp2, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[3]->cspace, cp3, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[4]->cspace, cp4, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[5]->cspace, cp5, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[6]->cspace, cp6, false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = 0;
	goto out;

out:	
fail2:
fail1:
	for (j = 0; j < i; j++)
		__lcd_destroy__(lcds[j]); /* tears down everything */
fail0:
	return ret;
}

static int test08(void)
{
	int ret;
	struct page *p;
	cptr_t cp = LCD_CPTR_NULL;

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail0;
	}
	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("enter klcd");
		__free_pages(p, 0);
		goto fail0;
	}
	ret = __lcd_cap_insert(&current->lcd->cspace, 
			cp, p, LCD_CAP_TYPE_PAGE);
	if (!ret) {
		LCD_ERR("insert didn't fail");
		goto fail1;
	}
	
	__klcd_exit();

	__free_pages(p, 0); /* page shouldn't have been inserted, so won't
			     * be freed when we exit */

	return 0;

fail1:
	__klcd_exit(); /* page should be freed since it was (incorrectly)
			* inserted */
fail0:
	return ret;
}

int cap_tests(void)
{
	if (test01())
		return -1;
	LCD_MSG("test01 passed");
	if (test02())
		return -1;
	LCD_MSG("test02 passed");
	if (test03())
		return -1;
	LCD_MSG("test03 passed");
	if (test04())
		return -1;
	LCD_MSG("test04 passed");
	if (test05())
		return -1;
	LCD_MSG("test05 passed");
	if (test06())
		return -1;
	LCD_MSG("test06 passed");
	if (test07())
		return -1;
	LCD_MSG("test07 passed");
	if (test08())
		return -1;
	LCD_MSG("all cap.c tests passed!");
	return 0;
}

