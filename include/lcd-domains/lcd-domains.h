#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <linux/module.h>
#include <asm/lcd-domains-arch.h>
#include <linux/sched.h>
#include <linux/waitqueue.h>

emum lcd_thread_status {
	LCD_THREAD_DEAD,
	LCD_THREAD_ALIVE,
};

emum lcd_xmit_status {
	LCD_XMIT_INVALID = 0,
	LCD_XMIT_SUCCESS = 1,
	LCD_XMIT_FAILED  = 2,
};

struct lcd;
struct lcd_thread_proxy;

struct lcd_thread {
	/*
	 * Reference counting
	 */
	atomic_t usage;
	/*
	 * Status
	 */
	int status;
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
	 * Message Passing
	 * ===============
	 *
	 * Thread control block, accessible by lcd_thread while running inside
	 * lcd (mapped in the bottom of its stack). Contains message 
	 * registers, etc.
	 */
	struct lcd_utcb *utcb;
	/*
	 * Transmission status. When a send/recv is completed, set to
	 * success/fail.
	 */
	atomic_t xmit_status;
	/*
	 * We use a `proxy' object for rendezvous point recv/send queues.
	 *
	 * For now, a thread is only allowed to do one blocking send or
	 * one blocking receive (hence only one proxy).
	 */
	struct lcd_thread_proxy proxy;
	/*
	 *
	 * Arch-dependent state of lcd_thread
	 * ==================================
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
	 * Reference counting
	 */
	atomic_t usage;
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
 * Message Passing
 * ===============
 *
 * Synchronous endpoints contain a send/receive queue for lcd_thread's 
 * (similar to an seL4 endpoint). Also known as `rendezvous point'.
 */
struct lcd_sync_endpoint {
	atomic_t usage;
	struct list_head senders;
	struct list_head receivers;
        struct mutex lock;
};
/**
 * An lcd_thread_proxy represents an lcd_thread in an endpoint's queue.
 *
 * This is similar to a wait_queue_t that represents a task in a wait queue.
 *
 * We do this so that an lcd_thread can receive on multiple endpoints. It
 * also conveniently wraps a back pointer to the endpoint so we can safely
 * remove the lcd_thread_proxy (using the endpoint's lock) from the queue.
 */
struct lcd_thread_proxy {
	struct lcd_thread *lcd_thread;
	struct lcd_sync_endpoint *endpoint;
	struct list_head proxies;
};

/* REFERENCE COUNTING -------------------------------------------------- */

/**
 * TODO: This will be shifted into cspaces soon.
 *
 * The put inlines are built so that the ptr is automatically set to NULL.
 * This prevents use after free.
 */

extern void __put_lcd_thread(struct lcd_thread *t);
static inline void __get_lcd_thread(struct lcd_thread *t)
{
	atomic_inc(&t->usage);
}
static inline void get_lcd_thread(struct lcd_thread **ptr, struct lcd_thread *t)
{
	__get_lcd_thread(t);
	*ptr = t;
}
static inline void put_lcd_thread(struct lcd_thread **ptr)
{
	BUG_ON(!atomic_read(&(*ptr)->usage));
	if (atomic_dec_and_test(&(*ptr)->usage))
		__put_lcd_thread(*ptr);
	*ptr = NULL;
}

extern void __put_lcd(struct lcd *lcd);
static inline void __get_lcd(struct lcd *lcd)
{
	atomic_inc(&lcd->usage);
}
static inline void get_lcd(struct lcd **ptr, struct lcd *lcd)
{
	__get_lcd(lcd);
	*ptr = lcd;
}
static inline void put_lcd(struct lcd **ptr)
{
	BUG_ON(!atomic_read(&(*ptr)->usage));
	if (atomic_dec_and_test(&(*ptr)->usage))
		__put_lcd(*ptr);
	*ptr = NULL;
}

extern void __put_lcd_sync_endpoint(struct lcd_sync_endpoint *e);
static inline void __get_lcd_sync_endpoint(struct lcd_sync_endpoint *e)
{
	atomic_inc(&e->usage);
}
static inline void get_lcd_sync_endpoint(struct lcd_sync_endpoint **ptr,
					struct lcd_sync_endpoint *e)
{
	__get_lcd_sync_endpoint(e);
	*ptr = e;
}
static inline void put_lcd_sync_endpoint(struct lcd_sync_endpoint **ptr);
{
	BUG_ON(!atomic_read(&(*ptr)->usage));
	if (atomic_dec_and_test(&(*ptr)->usage))
		__put_lcd_sync_endpoint(*ptr);
	*ptr = NULL;
}

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
