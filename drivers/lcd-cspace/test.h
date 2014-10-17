#include <lcd/cap.h>

extern int lcd_init_cspace(struct cspace *cspace);
extern int lcd_cap_insert(struct cspace *cspace, capability_t cap, 
		void *object, enum lcd_cap_type type);
extern void lcd_cap_delete(struct cspace *cspace, capability_t cap);
bool test_insert(void);
bool test_grant(void);
void test_delete(void);

bool begintests(void);