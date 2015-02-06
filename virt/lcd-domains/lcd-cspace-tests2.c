/*
 * Tests for cap.c
 *
 * Muktesh Khole, University of Utah
 */

#define TEST_PASSED true
#define TEST_FAILED false

int test_cap_count = 0, test_cap_grant_count = 0, test_cap_grant_count2=0;
struct cspace test_cspace, test_cspace_grant, test_cspace_grant2;
int test_max_cap = 4 * MAX_CAP_SLOTS;
capability_t arr[4 * MAX_CAP_SLOTS] = {0};
capability_t arr_grant[4 * MAX_CAP_SLOTS] = {0};
capability_t arr_grant2[4 * MAX_CAP_SLOTS] = {0};

bool test_insert(void) {
	capability_t cap;
	int i = 0;
	int res = 0;
	
	for (; i < test_max_cap; i++) {
		if (lcd_alloc_cap(&(test_cspace.cap_cache), &cap) == 0) {
			res = lcd_cap_insert(&test_cspace, cap, &test_max_cap, LCD_CAP_TYPE_SYNC_EP); 
			if (res != 0) {
				printk(KERN_ERR "lcd_cap_test insert failed for capability # %llu\n", cap);
				break;
			} else {
				arr[test_cap_count++] = cap;
				printk(KERN_INFO "lcd_cap_test insert succeeded for capability # %llu\n", cap);
			}
		} else {
			printk(KERN_ERR "lcd_cap_test insert failed to allocate capability\n");
			return false;
		}		
	}
	return (res==0);
}

bool test_grant(void) {
	int i = 0;
	int res;
	capability_t capsrc, capdst;
	for (; i < test_cap_count; i++) {
		if (lcd_alloc_cap(&(test_cspace_grant.cap_cache), &capdst) == 0) {
			capsrc = arr[i];
			res = lcd_cap_grant(&test_cspace, capsrc, &(test_cspace_grant), capdst);
			if (res == 0) {
				arr_grant[test_cap_grant_count++] = capdst;
				printk(KERN_INFO "lcd_cap_test grant capability # %llu to %llu succeeded\n", capsrc, capdst);  
			} else {
				printk(KERN_ERR "lcd_cap_test grant capability # %llu to %llu failed\n", capsrc, capdst);  
				return false;
			}
		} else {
			printk(KERN_ERR "lcd_cap_test grant failed to allcoate capability\n");
			return false;
		}		
	}

	for (i = 0; i < test_cap_count; i++) {
		if (lcd_alloc_cap(&(test_cspace_grant2.cap_cache), &capdst) == 0) {
			capsrc = arr[i];
			res = lcd_cap_grant(&test_cspace, capsrc, &(test_cspace_grant2), capdst);
			if (res == 0) {
				arr_grant2[test_cap_grant_count2++] = capdst;
				printk(KERN_INFO "lcd_cap_test grant2 capability # %llu to %llu succeeded\n", capsrc, capdst);  
			} else {
				printk(KERN_ERR "lcd_cap_test grant2 capability # %llu to %llu failed\n", capsrc, capdst);  
				return false;
			}
		} else {
			printk(KERN_ERR "lcd_cap_test grant2 failed to allcoate capability\n");
			return false;
		}		
	}
	return true;
}

void test_delete(void) {
	int i = 0;
	for (; i < test_cap_count; i++) {
		lcd_cap_delete(&test_cspace, arr[i]);
		printk(KERN_INFO "lcd_cap_test deleted original capability # %llu\n", arr[i]);  
	}
	test_cap_count = 0;

	for (i = 0; i < test_cap_grant_count; i++) {
		lcd_cap_delete(&test_cspace_grant, arr_grant[i]);
		printk(KERN_INFO "lcd_cap_test deleted granted capability # %llu\n", arr_grant[i]);  
	}
	test_cap_grant_count = 0;

	for (i = 0; i < test_cap_grant_count2; i++) {
		lcd_cap_delete(&test_cspace_grant2, arr_grant2[i]);
		printk(KERN_INFO "lcd_cap_test deleted granted2 capability # %llu\n", arr_grant2[i]);  
	}
	test_cap_grant_count2 = 0;
}

bool test_revoke(void) {
	// this inherently tests delete functionality
	int i = 0;
	int res;
	bool ret = true;
	for (; i < test_cap_count; i++) {
		res = lcd_cap_revoke(&test_cspace, arr[i]);
		ret = ret & (res == 0);
		if (res == 0) {
			printk(KERN_INFO "lcd_cap_test revoked capability # %llu\n", arr[i]);  
		} else {
			printk(KERN_ERR "lcd_cap_test FAILED to revoke capability # %llu\n", arr[i]);  
		}
	}
	test_cap_count = 0;
	test_cap_grant_count = 0;
	test_cap_grant_count2 = 0;
	return ret;
}

bool test_cspace_destroy(void) {
	bool res = false;
	int ret = -1;
	res = test_insert();
	if (res) {
		printk(KERN_INFO "lcd_cap_test insert succeeded");
		ret = lcd_cap_destroy_cspace(&test_cspace);
	} else {
		printk(KERN_ERR "lcd_cap_test insert failed");
	}

	return ret == 0;
}

static int cap_tests(void)
{
	bool res;
	lcd_init_cspace(&test_cspace);
	lcd_init_cspace(&test_cspace_grant);
	lcd_init_cspace(&test_cspace_grant2);
	res = test_insert();
	if (res == TEST_FAILED) {
		printk(KERN_ERR "lcd_cap_test test_insert() failed\n");
		return false;
	} else {
		printk(KERN_INFO "lcd_cap_test test_insert() Succeeded\n");
	}
	res = test_grant();
	if (res == TEST_FAILED) {
		printk(KERN_ERR "lcd_cap_test test_grant() failed\n"); 
		return false;
	} else {
		printk(KERN_INFO "lcd_cap_test test_grant() Succeeded\n"); 
	}
	// test_delete();
	res = test_revoke();
	if (res == TEST_FAILED) {
		printk(KERN_ERR "lcd_cap_test test_revoke() failed\n"); 
		return false;
	} else {
		printk(KERN_INFO "lcd_cap_test test_revoke() Succeeded\n"); 
	}

	res = test_cspace_destroy();
	if (res == TEST_FAILED) {
		printk(KERN_ERR "lcd_cap_test test_cspace_destroy() failed\n"); 
		return false;
	} else {
		printk(KERN_INFO "lcd_cap_test test_cspace_destroy() Succeeded\n"); 
	}

	printk(KERN_INFO "all cap tests passed!");

	return true;
}
