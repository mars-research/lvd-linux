/* 
 * boot.c
 *
 * Authors: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include "../include/common.h"
#include "../api/defs.h"

static inline struct lcd * test_mk_lcd(void)
{
	struct lcd *lcd;
	int ret;

	lcd = kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd)
		return NULL;
	memset(lcd, 0, sizeof(*lcd));
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

struct lcd_boot_info {
	struct completion c;
	int ret_val;
};

int manufacturer_start(void);
int dealer_start(void);
int customer_start(void);

static int lcd_boot_manufacturer(void *unused)
{
	struct lcd_boot_info *i;

	i = (struct lcd_boot_info *)unused;
	i->ret_val =  manufacturer_start();
	complete(&i->c);
	return 0;
}

static int lcd_boot_dealer(void *unused)
{
	struct lcd_boot_info *i;

	i = (struct lcd_boot_info *)unused;
	i->ret_val =  dealer_start();
	complete(&i->c);
	return 0;
}

static int lcd_boot_customer(void *unused)
{

	struct lcd_boot_info *i;

	i = (struct lcd_boot_info *)unused;
	i->ret_val = customer_start();
	complete(&i->c);
	return 0;;
}

static int lcd_boot(void)
{
	int ret = -1;
	struct lcd *manufacturer_lcd;
	struct lcd *dealer_lcd;
	struct lcd *customer_lcd;
	struct task_struct *manufacturer_task;
	struct task_struct *dealer_task;
	struct task_struct *customer_task;
	struct lcd_boot_info mi;
	struct lcd_boot_info di;
	struct lcd_boot_info ci;
	cptr_t mcptr;
	cptr_t dcptr;
	cptr_t dcptr2;
	cptr_t ccptr;

	/*
	 * Build lcd's
	 */
	manufacturer_lcd = test_mk_lcd();
	if (!manufacturer_lcd) {
		ret = -1;
		goto clean1;
	}
	dealer_lcd = test_mk_lcd();
	if (!dealer_lcd) {
		ret = -1;
		goto clean2;
	}
	customer_lcd = test_mk_lcd();
	if (!customer_lcd) {
		ret = -1;
		goto clean3;
	}

	/*
	 * Create and install boot endpoint
	 */
	ret = lcd_cnode_alloc(dealer_lcd->cspace, &dcptr);
	if (ret)
		goto clean4;
	if (dcptr != 31) {
		ret = -1;
		LCD_ERR("dealer cptr = %d, so need to change macro first",
			dcptr);
		goto clean4;
	}
	ret = __lcd_mk_sync_endpoint(dealer_lcd, dcptr);
	if (ret)
		goto clean4;

	ret = lcd_cnode_alloc(manufacturer_lcd->cspace, &mcptr);
	if (ret)
		goto clean5;
	if (mcptr != 31) {
		ret = -1;
		LCD_ERR("mfter cptr = %d, so need to change macro first",
			mcptr);
		goto clean5;
	}
	ret = lcd_cnode_grant(dealer_lcd->cspace, manufacturer_lcd->cspace,
			dcptr, mcptr, LCD_CAP_RIGHT_WRITE);
	if (ret)
		goto clean5;

	ret = lcd_cnode_alloc(customer_lcd->cspace, &ccptr);
	if (ret)
		goto clean5;
	if (ccptr != 31) {
		ret = -1;
		LCD_ERR("customer cptr = %d, so need to change macro first",
			ccptr);
		goto clean5;
	}
	ret = lcd_cnode_grant(dealer_lcd->cspace, customer_lcd->cspace,
			dcptr, ccptr, LCD_CAP_RIGHT_WRITE);
	if (ret)
		goto clean5;

	/*
	 * Alloc cnode slot for dealer
	 */
	ret = lcd_cnode_alloc(dealer_lcd->cspace, &dcptr2);
	if (ret)
		goto clean5;
	if (dcptr2 != 30) {
		ret = -1;
		LCD_ERR("dealer cptr = %d, so need to change macro first",
			dcptr2);
		goto clean5;
	}

	/*
	 * Init completions
	 */
	init_completion(&mi.c);
	init_completion(&di.c);
	init_completion(&ci.c);
	/*
	 * Spawn kthreads
	 */
	manufacturer_task = kthread_create(lcd_boot_manufacturer, &mi, 
					"lcd_boot_manufacturer");
	if (!manufacturer_task) {
		LCD_ERR("spawning manufacturer task");
		goto clean5;
	}
	dealer_task = kthread_create(lcd_boot_dealer, &di, 
					"lcd_boot_dealer");
	if (!dealer_task) {
		LCD_ERR("spawning dealer task");
		goto clean6;
	}
	customer_task = kthread_create(lcd_boot_customer, &ci, 
					"lcd_boot_customer");
	if (!customer_task) {
		LCD_ERR("spawning customer task");
		goto clean7;
	}

	/*
	 * Install lcd's
	 */
	manufacturer_task->lcd = manufacturer_lcd;
	manufacturer_lcd->parent = manufacturer_task;
	dealer_task->lcd = dealer_lcd;
	dealer_lcd->parent = dealer_task;
	customer_task->lcd = customer_lcd;
	customer_lcd->parent = customer_task;

	/*
	 * Wake up threads
	 */
	wake_up_process(manufacturer_task);
	wake_up_process(dealer_task);
	wake_up_process(customer_task);
	/*
	 * Wait for them to finish (using completions to avoid hang)
	 */
	wait_for_completion_interruptible(&mi.c);
	wait_for_completion_interruptible(&di.c);
	wait_for_completion_interruptible(&ci.c);

	if (mi.ret_val) {
		LCD_ERR("manufacturer non zero ret val %d",
			mi.ret_val);
		ret = -1;
		goto clean5;
	}
	if (di.ret_val) {
		LCD_ERR("dealer non zero ret val %d",
			di.ret_val);
		ret = -1;
		goto clean5;
	}
	if (ci.ret_val) {
		LCD_ERR("customer non zero ret val %d",
			ci.ret_val);
		ret = -1;
		goto clean5;
	}

	ret = 0;
	goto clean5;
	
clean7:
	kthread_stop(dealer_task);
clean6:
	kthread_stop(manufacturer_task);
clean5:
	__lcd_rm_sync_endpoint(dealer_lcd, dcptr);
clean4:
	test_rm_lcd(customer_lcd);
clean3:
	test_rm_lcd(dealer_lcd);
clean2:
	test_rm_lcd(manufacturer_lcd);
clean1:
	return ret;
}


static int __init lcd_boot_init(void)
{
	return lcd_boot();
}

static void __exit lcd_boot_exit(void)
{
	return;
}

module_init(lcd_boot_init);
module_exit(lcd_boot_exit);
