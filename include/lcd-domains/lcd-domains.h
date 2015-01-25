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
	 * My corresponding kthread
	 */
	struct task_struct *kthread;
	/*
	 * The containing lcd
	 */
	struct lcd *lcd;
	/*
	 * List of threads in containing lcd. Protected by lock in containing
	 * lcd.
	 */
	struct list_head lcd_threads;
	/*
	 * Thread control block, accessible by lcd_thread while running inside
	 * lcd (mapped in the bottom of its stack). Contains message 
	 * registers, etc.
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
 * layout. This means guest physical and virtual addresses have identical
 * values. (See the arch-dependent code header for the arch-dependent
 * part of the address space.)
 *
 * Stack/utcb pages: Except for the first thread, these are set up on demand.
 * The microkernel will set up and map the first thread's stack/utcb in the
 * stack 0 / utcb 0 page. For subsequent threads, the LCD requests that a
 * thread be created and provides the guest physical and virtual addresses for 
 * its stack/utcb page; the microkernel will allocate a host physical page
 * and map it in the guest physical/virtual address space of the LCD. This 
 * makes the microkernel logic simpler and puts the tricky/vulnerable
 * guest address space allocation logic inside the LCD.
 *
 * The module is mapped to the same guest physical / guest virtual
 * address space as the host, to avoid relocating symbols.
 * 
 * The LCD is free to modify its guest virtual -> guest physical mappings after
 * it starts. The guest virtual paging memory is for the microkernel's
 * initial set up of the guest virtual address space (mapping the arch
 * dependent chunks, the module, and the stack for the first thread).
 *
 *                   +---------------------------+
 *   module mapped   |                           |
 *   somewhere in    :                           :
 *    here ------->  :                           :
 *   at a higher     |        FREE SPACE         |
 *   address         |                           |
 *                   |                           |
 *                   +---------------------------+
 *                   |         Stack 0           |
 *                   :                           : (4 KBs)
 *                   |          utcb 0           |
 *                   +---------------------------+
 *                   |       Guest Virtual       | (4 MBs)
 *                   |       Paging Memory       |
 * LCD_ARCH_TOP----> +---------------------------+
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
	 * The module mapped in the lcd
	 */
	struct module *module;
	/*
	 * List of contained lcd_threads
	 */
	struct {
		struct list_head list;
		struct mutex lock;
	} lcd_threads;
	/*
	 * Guest virtual paging
	 *
	 * This is only needed while we're setting up the LCD. The LCD will
	 * be responsible for managing its guest virtual address space after
	 * it starts up (hence no lock, for now ...).
	 *
	 * We use a simple counter for allocating guest virtual memory used for 
	 * page tables for the lcd's guest virtual address space, to ensure
	 * we don't go past 4 MBs. (See the address space diagram above.)
	 *
	 * root is the host virtual address that points to the root of
	 * the lcd's guest virtual paging hierarchy.
	 */
	struct {
		pgd_t *root;
		gpa_t root_gpa;
		int counter;
	} gv_paging;
	/*
	 * The first thread to enter the lcd and run the module's init. Pass
	 * this to lcd_thread_start to start running it in the lcd.
	 */
	struct lcd_thread *init_thread;
};

/**
 * -- Loads module_name into host kernel. (Note: The module loading code
 *    expects underscores, _, rather than hyphens. If the module's name
 *    in the file system is some-module.ko, use the name some_module.)
 * -- Sets up the address space with the module mapped inside it in the lcd
 * -- Sets up an initial thread for running inside the lcd (stored in
 *    the struct lcd's init_thread)
 *
 * Call lcd_thread_start on the lcd's init thread to start executing the
 * module's init inside the lcd.
 *
 * Call lcd_destroy after killing the lcd's init thread to tear down the
 * lcd and remove the module from the host.
 *
 * lcd_thread_kill will block until the underlying kernel thread exits (the
 * kthread checks whether it should stop each time the lcd thread exits from
 * the lcd -- due to an interrupt, etc.).
 */
int lcd_create(char *module_name, struct lcd **out);
void lcd_destroy(struct lcd *lcd);
int lcd_thread_start(struct lcd_thread *t);
int lcd_thread_kill(struct lcd_thread *t);

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
