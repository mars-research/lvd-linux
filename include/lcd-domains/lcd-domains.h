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
 * Guest Physical
 * --------------
 *
 * The low addresses are occupied by the arch-dependent chunk of memory.
 * We use a 16 MB part of the guest physical address space for dynamically
 * mapping pages as they are allocated for the LCD, and a simple bitmap
 * for tracking which pages in the address space are allocated.
 *
 * The rest of guest physical is unused, and the top part is unusable (see
 * Intel SDM V3 28.2.2). Some of the guest virtual address space code uses
 * host code for paging - and this depends on the host's physical address
 * configuration (e.g., extracting a guest physical address from a page
 * table entry will use the physical address width of the host). Beware!
 *
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                   |                           |
 *                   |        Unusable           |
 *                   |                           |
 *                   +---------------------------+ 0x0000 FFFF FFFF FFFF
 *                   |                           |
 *                   |                           |
 *                   :    Free Guest Physical    :
 *                   :         Space             :
 *                   |                           |
 *                   |                           |
 *                   +---------------------------+
 *                   |                           |
 *                   |      Guest Physical       | (16 MBs)
 *                   |       "Page Heap"         |
 *                   |                           |
 * LCD_ARCH_TOP----> +---------------------------+
 *                   |                           |
 *                   :   Reserved Arch Memory    :
 *                   |                           |
 * LCD_ARCH_BOTTOM-> +---------------------------+
 *                   :        Not mapped         :
 *                   +---------------------------+ 0x0000 0000 0000 0000
 *
 * Guest Virtual
 * -------------
 *
 * Since we will be running modules (for now), and these are mapped in the
 * host at high addresses, we have set up the address space to accomodate
 * that. 
 *
 *   -- Arch-dependent memory is mapped "one-to-one" to guest physical
 *      addresses. 
 *   -- A 4 MB part is used for a temporary boot guest virtual address
 *      space.
 *   -- A 4 KB page is for the initial thread's stack/utcb.
 *   -- The module is mapped in the guest virtual at the same place that it is
 *      in the host virtual - so we can avoid relocating symbols, etc.
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
 * The LCD is free to modify its guest virtual -> guest physical mappings after
 * it starts.
 *
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                   |                           |
 *                   |                           |
 *                   :    Free Guest Virtual     :
 *                   :         Space             :
 *                   |                           |
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
 * LCD_ARCH_BOTTOM-> +---------------------------+
 *                   :        Not mapped         :
 *                   +---------------------------+ 0x0000 0000 0000 0000
 */

#define LCD_GP_MEM_SIZE (16 << 20)
#define LCD_GP_BMAP_NBITS (LCD_GP_MEM_SIZE >> PAGE_SHIFT)
#define LCD_GP_MEM_START LCD_ARCH_TOP
#define LCD_GV_MEM_SIZE (4 << 20)
#define LCD_GV_MEM_START LCD_ARCH_TOP

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
	 * The arch-dependent parts of the lcd (e.g., the ept)
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * List of contained lcd_threads
	 */
	struct {
		struct list_head list;
		struct mutex lock;
		unsigned int count;
	} lcd_threads;
	/*
	 * Guest physical address space
	 *
	 * We use a simple bitmap to track allocation of pages in the
	 * guest physical address space.
	 */
	struct {
		DECLARE_BITMAP(bmap, LCD_GP_BMAP_NBITS);
	} gp_paging;
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
