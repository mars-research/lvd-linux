/*
 * run.c -- Code for running LCDs
 *
 * Copyright: University of Utah
 */
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/pci.h>
#include <lcd_domains/microkernel.h>
#include <lcd_domains/lcd_iommu.h>
#include <asm/lcd_domains/run.h>
#include <asm/lcd_domains/create.h>
/* KLCD MAIN -------------------------------------------------- */

static int main_for_klcd(struct lcd *lcd)
{
	return lcd->klcd_main();
}

/* KTHREAD MAIN -------------------------------------------------- */

/* This is what the dedicated per-LCD kernel thread runs as its main. */

int __lcd_kthread_main(void *data) /* data is NULL */
{
	struct lcd *current_lcd = current->lcd;
	/*
	 * TODO: If the LCD exits, and it is the only one that has a
	 * reference to itself, we should tear it down.
	 */
	switch (current_lcd->type) {
	case LCD_TYPE_ISOLATED:
		return 0;
	case LCD_TYPE_NONISOLATED:
		return main_for_klcd(current_lcd);
	default:
		LCD_ERR("unexpected lcd type = %d",
			current_lcd->type);
		return -EINVAL;
	}
}

/* RUN -------------------------------------------------- */

int __lcd_run(struct lcd *caller, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = __lcd_get(caller, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is not in config state, fail
	 */
	if (!lcd_status_configed(lcd_struct)) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"cannot run: lcd is in state %d",
			get_lcd_status(lcd_struct));
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * This will run the kthread for the first time
	 */
	set_lcd_status(lcd_struct, LCD_STATUS_RUNNING);
#ifdef CONFIG_LVD
	if (lcd_struct->type == LCD_TYPE_NONISOLATED)
		if (lcd_struct->m)
			lcd_struct->m->init();
#else
	wake_up_process(lcd_struct->kthread);
#endif
	/*
	 * Unlock
	 */
	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}

int __lcd_stop(struct lcd *caller, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock
	 */
	ret = __lcd_get(caller, lcd, &cnode, &lcd_struct);
	if (ret)
		goto fail1;
	/*
	 * If lcd is not in running state, fail
	 */
	if (!lcd_status_running(lcd_struct)) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"cannot run: lcd is in state %d",
			get_lcd_status(lcd_struct));
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * This will run the kthread for the first time
	 */
	if (lcd_struct->type == LCD_TYPE_NONISOLATED)
		if (lcd_struct->m)
			lcd_struct->m->exit();
	/*
	 * Unlock
	 */
	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}


/* INIT/EXIT -------------------------------------------------- */

int __lcd_run_init(void)
{
	/*
	 * Nothing for now
	 */
	return 0;
}

void __lcd_run_exit(void)
{
	/*
	 * Nothing for now
	 */
	return;
}
