#include "test.h"

int test_cap_count = 0;
struct cspace test_cspace;
int test_max_cap = 4 * MAX_CAP_SLOTS;
capability_t arr[4 * MAX_CAP_SLOTS] = {0};

bool test_insert(void) {
	capability_t cap;
	int i = 0;
	int res = 0;

	lcd_init_cspace(&test_cspace);
	for (; i < test_max_cap; i++) {
		if (lcd_alloc_cap(&(test_cspace.cap_cache), &cap) == 0) {
			printk(KERN_ERR "lcd_cap_test got capability %llu", cap);
			arr[test_cap_count++] = cap;
			res = lcd_cap_insert(&test_cspace, cap, &test_max_cap, LCD_CAP_TYPE_SYNC_EP); 
			if (res != 0) {
				break;
			}
		}		
	}
	return (res==0);
}

void test_delete(void) {
	int i = 0;
	for (; i < test_cap_count; i++) {
		lcd_cap_delete(&test_cspace, arr[i]);
	}
	test_cap_count = 0;
}