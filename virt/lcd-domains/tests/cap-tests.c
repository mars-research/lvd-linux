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
	ret = __lcd_cap_init_cspace(&c);
	if (ret) {
		LCD_ERR("init cspace");
		goto fail1;
	}
	ret = __lcd_cnode_lookup(&c, __cptr(15), true, &cnode1);
	if (ret) {
		LCD_ERR("lookup");
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&c, __cptr(15), true, &cnode2);
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

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail0;
	}
	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("enter klcd");
		goto fail1;
	}
	ret = __lcd_cap_insert(&current->lcd->cspace, 
			__cptr(15), p, LCD_CAP_TYPE_PAGE);
	if (ret) {
		LCD_ERR("insert");
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&current->lcd->cspace, 
				__cptr(15), false, &cnode);
	if (ret) {
		LCD_ERR("lookup");
		goto fail3;
	}
	if (cnode->type != LCD_CAP_TYPE_PAGE) {
		LCD_ERR("not free");
		goto fail4;
	}
	if (cnode->object != p) {
		LCD_ERR("wrong object");
		goto fail5;
	}
	
	__klcd_exit();

	return 0;
fail5:
fail4:
fail3:
fail2:
	__klcd_exit();
fail1:
	__free_pages(p, 0);
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
			goto fail1;
		}
	}
	/*
	 * Insert into 0's cspace
	 */
	ret = __lcd_cap_insert(&lcds[0]->cspace, __cptr(23), p,
			LCD_CAP_TYPE_PAGE);
	if (ret) {
		LCD_ERR("insert");
		goto fail2;
	}
	/*
	 * Grant to 1 and 2 (so they are children of 0)
	 */
	ret = __lcd_cap_grant(&lcds[0]->cspace, __cptr(23),
			&lcds[1]->cspace, __cptr(13));
	if (ret) {
		LCD_ERR("grant 0 to 1");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[0]->cspace, __cptr(23),
			&lcds[2]->cspace, __cptr(15));
	if (ret) {
		LCD_ERR("grant 0 to 2");
		goto fail2;
	}
	/*
	 * Grant 1 -> 3 & 4, 2 -> 5 & 6
	 */
	ret = __lcd_cap_grant(&lcds[1]->cspace, __cptr(13),
			&lcds[3]->cspace, __cptr(4));
	if (ret) {
		LCD_ERR("grant 1 to 3");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[1]->cspace, __cptr(13),
			&lcds[4]->cspace, __cptr(6));
	if (ret) {
		LCD_ERR("grant 1 to 4");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[2]->cspace, __cptr(15),
			&lcds[5]->cspace, __cptr(9));
	if (ret) {
		LCD_ERR("grant 2 to 5");
		goto fail2;
	}
	ret = __lcd_cap_grant(&lcds[2]->cspace, __cptr(15),
			&lcds[6]->cspace, __cptr(10));
	if (ret) {
		LCD_ERR("grant 2 to 6");
		goto fail2;
	}
	/*
	 * 0 does a revoke
	 */
	ret = __lcd_cap_revoke(&lcds[0]->cspace, __cptr(23));
	if (ret) {
		LCD_ERR("revoke");
		goto fail2;
	}
	/*
	 * Make sure 0 still has access
	 */
	ret = __lcd_cnode_lookup(&lcds[0]->cspace, __cptr(23), false, &cnode);
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
	ret = __lcd_cnode_lookup(&lcds[1]->cspace, __cptr(13), false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[2]->cspace, __cptr(15), false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[3]->cspace, __cptr(4), false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[4]->cspace, __cptr(6), false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[5]->cspace, __cptr(9), false, &cnode);
	if (!ret) {
		LCD_ERR("cnode lookup unexpectedly found something");
		ret = -1;
		goto fail2;
	}
	ret = __lcd_cnode_lookup(&lcds[6]->cspace, __cptr(10), false, &cnode);
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


int cap_tests(void)
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
	LCD_MSG("all cap.c tests passed!");
	return 0;
}

