#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <linux/module.h>
#include <asm/lcd-domains-arch.h>
#include <linux/sched.h>

/*
 * lcd_thread_status
 * =================
 *
 * Similar to status used for task_struct's.
 *
 * LCD_THREAD_UNFORMED   = still setting up, or status not set yet
 * LCD_THREAD_SUSPENDED  = lcd_thread is going to sleep / is asleep
 * LCD_THREAD_RUNNABLE   = set status to this to wake up lcd_thread
 * LCD_THREAD_RUNNING    = lcd_thread is running, or is about to run
 * LCD_THREAD_KILL       = set status to this to kill lcd_thread (as soon as
 *                         possible)
 * LCD_THREAD_DEAD       = lcd_thread is not running, most parts have been 
 *                         destroyed (only hanging around to provide 
 *                         status info)
 */
enum lcd_thread_status {
	LCD_THREAD_UNFORMED  = 0,
	LCD_THREAD_SUSPENDED = 1,
	LCD_THREAD_RUNNABLE  = 2,
	LCD_THREAD_RUNNING   = 3,
	LCD_THREAD_KILL      = 4,
	LCD_THREAD_DEAD      = 5,
};

struct lcd;

struct lcd_thread {
	/*
	 * The containing lcd
	 */
	struct lcd *lcd;
	/*
	 * List of threads in containing lcd
	 */
	struct list_head lcd_threads;
	/*
	 * Thread control block, accessible by lcd_thread while running inside
	 * lcd. Contains message registers, etc.
	 */
	struct lcd_utcb *utcb;
	/*
	 * Status (see above)
	 */
	int status;
	/*
	 * Arch-dependent state of lcd_thread
	 */
	struct lcd_arch_thread *lcd_arch_thread;
};

/*
 * LCD Memory Layout
 * =================
 *
 * The layout below reflects the guest physical *and* virtual memory
 * layout. Guest virtual paging memory is filled on on demand
 * during lcd initialization (so parts may not be backed by host physical
 * memory), and stacks/utcb's are also filled in on demand as threads are
 * created and added to the lcd. The lcd starts with one lcd_thread, so
 * Stack 0 / utcb 0 will be mapped after the lcd is initialized.
 *
 * Guest physical addresses are mapped one-to-one to the same guest 
 * virtual addresses.
 *
 * The module is mapped to the same guest physical / guest virtual
 * address space as the host, to avoid relocating symbols.
 *
 *                   +---------------------------+
 *   module mapped   |                           |
 *   somewhere in    :                           :
 *    here ------->  :                           :
 *   at a higher     |                           |
 *   address         |                           |
 *                   |                           |
 *                   +---------------------------+
 *                   |       Stack 1023          |
 *                   :                           : (4 KBs)
 *                   |        utcb 1023          |
 *                   +---------------------------+
 *                   |           ...             |
 *                   :                           :
 *                   |           ...             |
 *                   +---------------------------+
 *                   |         Stack 1           |
 *                   :                           : (4 KBs)
 *                   |          utcb 1           |
 *                   +---------------------------+
 *                   |         Stack 0           |
 *                   :                           : (4 KBs)
 *                   |          utcb 0           |
 *                   +---------------------------+
 *                   |       Guest Virtual       | (4 MBs)
 *                   |       Paging Memory       |
 * LCD_ARCH_FREE---> +---------------------------+
 *                   |                           |
 *                   :   Reserved Arch Memory    :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 */

#define LCD_PAGING_MEM_SIZE (4 << 20)

struct lcd {
	/*
	 * Display name
	 */
	char name[MODULE_NAME_LEN];
	/*
	 * Guest virtual paging:
	 *
	 * root is the host virtual address that points to the root of
	 * the lcd's guest virtual paging hierarchy.
	 *
	 * We use a simple bitmap for allocating memory used for page tables
	 * for the lcd's guest virtual address space. This is only needed when
	 * the lcd is being set up - mapping the arch bits, module code, the
	 * first lcd_thread's tcb, etc.
	 */
	pgd_t *root;
	DECLARE_BITMAP(gv_paging_bmap, (LCD_PAGING_MEM_SIZE >> PAGE_SIZE));
};

/**
 * -- Loads module_name into host kernel. (Note: The module loading code
 *    expects underscores, _, rather than hyphens. If the module's name
 *    in the file system is some-module.ko, use the name some_module.)
 * -- Spawns a kernel thread that will host the lcd.
 * -- The kernel thread will create the lcd and map the module into
 *    the lcd. The kernel thread will then wait with the lcd's status
 *    set to LCD_THREAD_SUSPENDED.
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
