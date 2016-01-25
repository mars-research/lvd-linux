
#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

static int cptr_test(void)
{
	cptr_t cptrs[10];
	int i, j;
	int ret;

	for (i = 0; i < 10; i++) {
		ret = lcd_alloc_cptr(&cptrs[i]);
		if (ret) {
			LIBLCD_ERR("failed at i = %d", i);
			goto fail;
		}
		if (cptr_is_null(cptrs[i])) {
			LIBLCD_ERR("got null cptr");
			i++;
			goto fail;
		}
	}

	ret = 0;
	goto out;

out:
fail:
	for (j = 0; j < i; j++)
		lcd_free_cptr(cptrs[j]);

	return ret;
}

static int page_alloc_test(void)
{
	int ret;
	gva_t pages[10];
	int i;
	int j;
	int *loc;
	/*
	 * Allocate ten pages
	 */
	for (i = 0; i < 10; i++) {
		ret = lcd_alloc_page(&pages[i]);
		if (ret) {
			LIBLCD_ERR("failed to alloc page");
			goto fail1;
		}
	}
	/*
	 * Write and read each one, make sure we don't fault.
	 */
	for (i = 0; i < 10; i++) {
		loc = (int *)gva_val(pages[i]);
		*loc = i;
	}
	for (i = 0; i < 10; i++) {
		loc = (int *)gva_val(pages[i]);
		if (*loc != i) {
			LIBLCD_ERR("unexpected val %d != %d",
				*loc, i);
			i = 10;
			ret = -1;
			goto fail1;
		}
	}

	/*
	 * Free the pages
	 */
	i = 10;
	ret = 0;
	goto out;

out:
fail1:
	for (j = 0; j < i; j++)
		lcd_free_page(pages[j]);

	return ret;
}

struct a {
	int x, y, z;
	char buff[512];
};

struct bstruct {
	int x, y, z;
	char buff[512];
};

static int kmalloc_test(void)
{
	struct a *x;
	struct kmem_cache *cache;
	int i;
	struct bstruct *b;
	int ret;
	/*
	 * Kmalloc test
	 */
	x = kmalloc(sizeof(struct a) * 10, GFP_KERNEL);
	if (!x) {
		LIBLCD_ERR("kmalloc failed");
		ret = -1;
		goto fail1;
	}
	for (i = 0; i < 10; i++) {
		x[i].x = 2 * i + 1;
		x[i].y = 3 * i + 1;
		x[i].z = 4 * i + 1;
		memset(&x[i].buff, 0, 512);
	}
	kfree(x);
	/*
	 * Kmem cache test
	 */
	cache = KMEM_CACHE(bstruct, 0);
	if (!cache) {
		LIBLCD_ERR("kmem cache failed");
		ret = -1;
		goto fail2;
	}
	b = kmem_cache_zalloc(cache, GFP_KERNEL);
	if (!b) {
		LIBLCD_ERR("kmem cache alloc failed");
		ret = -1;
		goto fail3;
	}
	kmem_cache_free(cache, b);
	kmem_cache_destroy(cache);

	return 0;

fail3:
	kmem_cache_destroy(cache);
fail2:
fail1:
	return ret;
}

static int kmalloc_big_test(void)
{
	void *x;

	/* Alloc 24 pages */
	x = kmalloc(24 << PAGE_SHIFT, GFP_KERNEL);
	if (!x) {
		LIBLCD_ERR("kmalloc failed");
		return -1;
	}
	kfree(x);
	return 0;
}

static int kmalloc_bigbig_test(void)
{
	void *x;

	/* This alloc should fail. */
	x = kmalloc(KMALLOC_MAX_SIZE + 1, GFP_KERNEL);
	if (x) {
		LIBLCD_ERR("kmalloc should have failed");
		kfree(x);
		return -1;
	}
	return 0;
}

static int dstore_test(void)
{
	struct dstore *d1, *d2;
	struct dstore_node *n;
	int ret;
	int i, j;
	int objs_d1[10];
	dptr_t dptrs_d1[10];
	int objs_d2[10];
	dptr_t dptrs_d2[10];
	void *obj;
	/*
	 * Initialize them
	 */
	ret = lcd_dstore_init_dstore(&d1);
	if (ret) {
		LIBLCD_ERR("init dstore d1");
		goto fail1;
	}
	ret = lcd_dstore_init_dstore(&d2);
	if (ret) {
		LIBLCD_ERR("init dstore d2");
		goto fail2;
	}
	/*
	 * Insert objects
	 */
	for (i = 0; i < 10; i++) {
		objs_d1[i] = 2 * i;
		ret = lcd_dstore_insert(d1, &objs_d1[i], 3, &dptrs_d1[i]);
		if (ret) {
			LIBLCD_ERR("insert into d1 at i = %d", i);
			goto fail3;
		}
	}
	for (j = 0; j < 10; j++) {
		objs_d2[j] = 2 * j + 1;
		ret = lcd_dstore_insert(d2, &objs_d2[j], 7, &dptrs_d2[j]);
		if (ret) {
			LIBLCD_ERR("insert into d2 at j = %d", j);
			goto fail4;
		}
	}
	/*
	 * Look up and check
	 */
	for (i = 0; i < 10; i++) {
		ret = lcd_dstore_get(d1, dptrs_d1[i], 3, &n);
		obj = lcd_dstore_node_object(n);
		if (ret) {
			LIBLCD_ERR("lookup object i = %d in d1", i);
			goto fail5;
		}
		if (obj != (void *)&objs_d1[i]) {
			LIBLCD_ERR("diff object at i = %d in d1", i);
			goto fail5;
		}
		if (*((int *)obj) != objs_d1[i]) {
			LIBLCD_ERR("wrong value at i = %d in d1", i);
			goto fail5;
		}
		lcd_dstore_put(n);
	}
	for (j = 0; j < 10; j++) {
		ret = lcd_dstore_get(d2, dptrs_d2[j], 7, &n);
		obj = lcd_dstore_node_object(n);
		if (ret) {
			LIBLCD_ERR("lookup object j = %d in d2", j);
			goto fail6;
		}
		if (obj != (void *)&objs_d2[j]) {
			LIBLCD_ERR("diff object at j = %d in d2", j);
			goto fail6;
		}
		if (*((int *)obj) != objs_d2[j]) {
			LIBLCD_ERR("wrong value at j = %d in d2", j);
			goto fail6;
		}
		lcd_dstore_put(n);
	}
	/*
	 * Tear it all down
	 */
	ret = 0;
	goto out;
	
out:
fail6:
fail5:	
fail4:
fail3:
	lcd_dstore_destroy(d2);
fail2:
	lcd_dstore_destroy(d1);
fail1:
	return ret;
}

static char *cptr_lookup_str = "foo";

static int cptr_lookup_test(void)
{
	int ret;
	cptr_t pg_cptr = { 0 };
	unsigned long off = 0;

	ret = lcd_virt_addr_to_page_cptr(cptr_lookup_str,
					strlen(cptr_lookup_str),
					&pg_cptr,
					&off);
	if (ret) {
		LIBLCD_ERR("failed lookup");
		goto fail1;
	}
	if (cptr_is_null(pg_cptr)) {
		LIBLCD_ERR("bad cptr");
		ret = -1;
		goto fail2;
	}

	return 0;

fail2:
fail1:
	return ret;
}

int lcd_run_tests(void)
{
	int ret;
	ret = cptr_test();
	if (ret) {
		LIBLCD_ERR("cptr test failed");
		return ret;
	}
	LIBLCD_MSG("cptr test passed!");
	ret = page_alloc_test();
	if (ret) {
		LIBLCD_ERR("page alloc test failed");
		return ret;
	}
	LIBLCD_MSG("page alloc test passed!");
	ret = kmalloc_test();
	if (ret) {
		LIBLCD_ERR("kmalloc test failed");
		return ret;
	}
	LIBLCD_MSG("kmalloc test passed!");
	ret = kmalloc_big_test();
	if (ret) {
		LIBLCD_ERR("kmalloc big test failed");
		return ret;
	}
	LIBLCD_MSG("kmalloc big test passed!");
	ret = kmalloc_bigbig_test();
	if (ret) {
		LIBLCD_ERR("kmalloc bigbig test failed");
		return ret;
	}
	LIBLCD_MSG("kmalloc bigbig test passed!");
	ret = dstore_test();
	if (ret) {
		LIBLCD_ERR("dstore test failed");
		return ret;
	}
	LIBLCD_MSG("dstore test passed!");
	ret = cptr_lookup_test();
	if (ret) {
		LIBLCD_ERR("cptr lookup test failed");
		return ret;
	}
	LIBLCD_MSG("cptr lookup test passed!");

	LIBLCD_MSG("ALL LIBLCD TESTS PASSED");

	return 0;
}
