#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <linux/module.h>
#include <asm/lcd-domains-arch.h>

/*
 * lcd_status = status of kthread / lcd
 * ====================================
 *
 * LCD_STATUS_UNFORMED   = still setting up, or status not set yet
 * LCD_STATUS_SUSPENDED  = lcd is paused, and kthread is going to asleep / is 
 *                         asleep
 * LCD_STATUS_RUNNABLE   = lcd is paused, and kthread is awake, or should awaken
 *                         (the status should be set to this if the lcd / 
 *                         kthread are suspended, and you want it to wake up)
 * LCD_STATUS_RUNNING    = lcd is running, or is about to run
 * LCD_STATUS_KILL       = lcd and kthread should die
 * LCD_STATUS_DEAD       = lcd is not running, most parts have been destroyed
 *                         (lcd_struct is only hanging around to provide status 
 *                         info); the kthread is ready to die and be reaped
 */
enum lcd_status {
	LCD_STATUS_UNFORMED  = 0,
	LCD_STATUS_SUSPENDED = 1,
	LCD_STATUS_RUNNABLE  = 2,
	LCD_STATUS_RUNNING   = 3,
	LCD_STATUS_KILL      = 4,
	LCD_STATUS_DEAD      = 5,
};

struct lcd {
	/*
	 * Display name
	 */
	char name[MODULE_NAME_LEN];
	/*
	 * Status (enum lcd_status)
	 */
	int status;
	/*
	 * Arch-dependent state of lcd
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * Guest virtual paging.
	 */
	struct {
		/*
		 * = 0 if root_hva invalid
		 * = 1 if root_hva valid
		 */
		int present;
		/*
		 * Host virtual address of the root of the lcd's
		 * (initial) guest virtual paging hierarchy.
		 */
		pgd_t *root;
		/*
		 * Pointer to start of guest physical address space 
		 * used for paging.
		 */
		gpa_t paging_mem_bot;
		/*
		 * Pointer to next free page in guest physical
		 * address space that can be used for a page table.
		 */
		gpa_t paging_mem_brk;
		/*
		 * Top of region in guest physical address space
		 * for page tables.
		 */
		gpa_t paging_mem_top;
	} gv;
};

/**
 * -- Loads module_name into host kernel. (Note: The module loading code
 *    expects underscores, _, rather than hyphens. If the module's name
 *    in the file system is some-module.ko, use the name some_module.)
 * -- Spawns a kernel thread that will host the lcd.
 * -- The kernel thread will create the lcd and map the module into
 *    the lcd. The kernel thread will then wait with the lcd's status
 *    set to LCD_STATUS_SUSPENDED.
 * -- Call lcd_run_as_module to start running the lcd.
 * -- Returns NULL if we fail to create the kernel thread, or if the
 *    kernel thread failed to initialize the lcd, etc.
 *
 * Call lcd_destroy_as_module after a successful return from 
 * lcd_create_as_module to stop the kthread and remove the module
 * from the host kernel.
 */
struct task_struct * lcd_create_as_module(char *module_name);
/**
 * Wakes up kthread to start running lcd. Call this after a successful
 * return from lcd_create_as_module. Call lcd_destroy_as_module when
 * the kthread/lcd are no longer needed.
 */
int lcd_run_as_module(struct task_struct *t);
/**
 * Stops the kernel thread (which in turn, destroys the lcd) and removes
 * the module from the host kernel. 
 *
 * Note: The kthread checks if it should stop each time the lcd exits in
 * the main run loop.
 */
void lcd_destroy_as_module(struct task_struct *t, char *module_name);

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
