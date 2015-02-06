#ifndef LCD_DOMAINS_INTERNAL_H
#define LCD_DOMAINS_INTERNAL_H

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/lcd-domains-arch.h>
#include <lcd-domains/types.h>

/* --------------------------------------------------
 * DEBUG
 * --------------------------------------------------
 */

#define LCD_DEBUG 0

#define LCD_ERR(msg...) __lcd_err(__FILE__, __LINE__, msg)
static inline void __lcd_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_MSG(msg...) __lcd_msg(__FILE__, __LINE__, msg)
static inline void __lcd_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_WARN(msg...) __lcd_warn(__FILE__, __LINE__, msg)
static inline void __lcd_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

/*
 * --------------------------------------------------
 * CAPABILITIES
 * --------------------------------------------------
 *
 * See Documentation/lcd-domains/cap.txt.
 */

#define LCD_MAX_CAPS 32
#define LCD_CNODE_TABLE_NUM_SLOTS 8 /* should be a power of 2 */

/* 
 * --------------------------------------------------
 *
 * CNODES
 * 
 */


enum lcd_cap_type {
	LCD_CAP_TYPE_INVALID,
	LCD_CAP_TYPE_FREE,
	LCD_CAP_TYPE_SYNC_EP,
	LCD_CAP_TYPE_PAGE,
	LCD_CAP_TYPE_KPAGE,
	LCD_CAP_TYPE_LCD,
	LCD_CAP_TYPE_CNODE,
};

enum allocation_state {
	ALLOCATION_INVALID,
	ALLOCATION_VALID,
	ALLOCATION_MARKED_FOR_DELETE,
	ALLOCATION_REMOVED,
};

struct cnode;
struct cdt_root_node {
	struct mutex lock;
	struct cnode *cnode;
	enum allocation_state state;
};

struct cspace;
struct cnode {
	struct mutex lock;
	/*
	 * cnode data
	 */
	enum lcd_cap_type type;
	void *object;
	struct cspace *cspace;
	/*
	 * cdt data
	 */
	struct cdt_root_node *cdt_root;
	struct list_head children;
	struct list_head siblings;
	/*
	 * Page-specific cnodes (temporary!)
	 * =================================
	 *
	 * We need to keep track of if and where the page is mapped so we
	 * know how to unmap it when the lcd deletes/loses this capability.
	 *
	 * We impose the constraint that pages can only be mapped *once* in an 
	 * lcd's ept, so we only need one gpa. 
	 */
	int is_mapped;
	gpa_t where_mapped;
};

struct cnode_table {
	struct cnode cnode[LCD_CNODE_TABLE_NUM_SLOTS];
	uint8_t table_level;
	struct list_head table_list;
};

struct cspace {
	struct mutex lock;
	enum allocation_state state;
	struct cnode_table *cnode_table;
	struct kmem_cache *cnode_table_cache;
	struct list_head table_list;
};

/**
 * Initializes caches, etc. in capability subsystem. Called when microkernel
 * intializes.
 */
int __lcd_cap_init(void);
/**
 * Tears down caches, etc. in capability subsystem. Called when microkernel
 * is exiting.
 */
void __lcd_cap_exit(void);
/**
 * Sets up cspace - initializes lock, root cnode table, etc.
 */
int __lcd_cap_init_cspace(struct cspace *cspace);
/**
 * Inserts object data into cspace at cnode pointed at by c.
 */
int __lcd_cap_insert(struct cspace *cspace, cptr_t c, void *object, 
		enum lcd_cap_type type);
/**
 * Deletes object data from cspace at cnode pointed at by c.
 *
 * Updates the state of the microkernel to reflect rights change (e.g., if
 * a cnode for a page is deleted, and the page is mapped, the page will be
 * unmapped).
 *
 * If this is the last cnode that refers to the object, the object is
 * destroyed.
 */
void __lcd_cap_delete(struct cspace *cspace, cptr_t c);
/**
 * Copies cnode data in src cnode at c_src to dest cnode at c_dst. The dest
 * cnode will be a child of the src cnode in the cdt containing src cnode.
 */
int __lcd_cap_grant(struct cspace *cspacesrc, cptr_t c_src, 
		struct cspace *cspacedst, cptr_t c_dst);
int __lcd_cap_grant_page(struct cspace *cspacesrc, cptr_t c_src, 
			struct cspace *cspacedst, cptr_t c_dst, gpa_t gpa);
/**
 * Equivalent to calling lcd_cap_delete on all of the cnode's children. 
 *
 * ** Does not delete the cnode itself. **
 */
int __lcd_cap_revoke(struct cspace *cspace, cptr_t c);
/**
 * Equivalent to calling lcd_cap_delete on all cnodes in cspace. Frees up
 * all cnode tables, etc.
 */
void __lcd_cap_destroy_cspace(struct cspace *cspace);
/**
 * Looks up and locks cnode at c in cspace.
 *
 * ** Must match with lcd_cnode_put. **
 *
 * ** Interrupts and preemption *are not* disabled. **
 *    (so we can easily get out of deadlocks while debugging)
 */
int __lcd_cnode_get(struct cspace *cspace, cptr_t cap, struct cnode **cnode);
/**
 * Unlocks cnode.
 */
void __lcd_cnode_put(struct cnode *c);


/*
 * --------------------------------------------------
 *
 * UTCB - user thread control block
 *
 * This is the message buffer mapped inside the LCD.
 *
 *   -- mr are general registers and are just copied during a send/recv
 *   -- cr are capability pointer registers, and the corresponding 
 *      capabilities are granted during a send/recv
 *
 * NOTE: This needs to fit inside a page (4 KBs). It goes at the bottom
 * of the lcd's stack (so it should be reasonably small).
 */

#define LCD_NUM_REGS 8

struct lcd_utcb {
	u64 mr[LCD_NUM_REGS];
	cptr_t cr[LCD_NUM_REGS];
};


/* 
 * --------------------------------------------------
 *
 * LCDs - lightweight capability domain
 *
 */


#define	LCD_STATUS_EMBRYO     0
#define	LCD_STATUS_SUSPENDED  1
#define	LCD_STATUS_RUNNING    2
#define	LCD_STATUS_DEAD       3 /* an lcd can never come back to life */


enum lcd_xmit_status {
	LCD_XMIT_INVALID = 0, /* when lcd is not in queue */
	LCD_XMIT_SUCCESS = 1, /* when send/recv succeeded */
	LCD_XMIT_FAILED  = 2, /* when send/recv failed    */
};

struct lcd {
	/*
	 * Lock
	 *
	 * Protects all fields except the cspace and endpoint_queue.
	 *
	 * (Pray for me that I will not get deadlocks)
	 */
	struct mutex lock;
	/*
	 * Status
	 */
	atomic_t status;
	/*
	 * My corresponding kthread
	 */
	struct task_struct *kthread;
	/*
	 * Completions so other lcd's can wait till an lcd is fully
	 * suspended. (Similar technique used for kthreads - 
	 * see kernel/kthread.c.)
	 */
	struct completion suspended;
	/*
	 * The arch-dependent parts of the lcd (e.g., the ept). (This needs
	 * to be aligned properly, so we use a pointer instead of embedding
	 * it.)
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * The LCD's cspace
	 */
	struct cspace cspace;
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
	atomic_t xmit_flag;
	/*
	 * Non-zero if we are making an ipc call (so microkernel knows
	 * it needs to do a grant on the sender's call endpoint)
	 */
	int making_call;
	/*
	 * Non-zero if we are doing a reply (so microkernel knows it
	 * needs to delete capability for reply endpoint)
	 */
	int doing_reply;
	/*
	 * Send/recv queue we are on. 
	 *
	 * This is protected by the endpoint's lock. The only time we need
	 * to access this field from the lcd side (rather than the endpoint
	 * side) is when we're tearing down the lcd. The proper way to do that
	 * is to first look up the capability for the endpoint, lock it, and
	 * then check if the lcd is in the queue.
	 */
	struct list_head endpoint_queue;
};

/* similar to task structs */
#define set_lcd_status(lcd, status_value)		\
	do {						\
	atomic_set(&(lcd)->status, (status_value));	\
	smp_mb();					\
	} while(0);					
#define lcd_status_embryo(lcd) \
	(atomic_read(&(lcd)->status) == LCD_STATUS_EMBRYO)
#define lcd_status_suspended(lcd) \
	(atomic_read(&(lcd)->status) == LCD_STATUS_SUSPENDED)
#define lcd_status_running(lcd) \
	(atomic_read(&(lcd)->status) == LCD_STATUS_RUNNING)
#define lcd_status_dead(lcd) \
	(atomic_read(&(lcd)->status) == LCD_STATUS_DEAD)

/*
 * --------------------------------------------------
 *
 * MESSAGE PASSING
 *
 * Synchronous endpoints contain a send/receive queue for lcd's 
 * (similar to an seL4 endpoint). Also known as `rendezvous point'.
 */
struct lcd_sync_endpoint {
	struct list_head senders;
	struct list_head receivers;
        struct mutex lock;
};

/* --------------------------------------------------
 *
 * SYSTEM CALLS
 *
 */

enum lcd_syscall {
	LCD_SYSCALL_SEND       = 0,
	LCD_SYSCALL_NBSEND     = 1,
	LCD_SYSCALL_CALL       = 2,
	LCD_SYSCALL_WAIT       = 3,
	LCD_SYSCALL_REPLY      = 4,
	LCD_SYSCALL_REPLYWAIT  = 5,
	LCD_SYSCALL_YIELD      = 6,
};

/* KLCD STUFF -------------------------------------------------- */

int __kliblcd_init(void);
void __kliblcd_exit(void);

int __klcd_enter(void);
void __klcd_exit(void);
int __klcd_page_zalloc(struct lcd *klcd, cptr_t c, hpa_t *hpa_out,
		hva_t *hva_out);
int __lcd_create(struct lcd *caller, cptr_t slot, gpa_t stack);
int __lcd_create__(struct lcd **out);
int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp, 
		gpa_t gva_root);
int __lcd_run(struct lcd *caller, cptr_t lcd);
int __lcd_suspend(struct lcd *caller, cptr_t lcd);
int __lcd_cap_grant_cheat(struct lcd *caller, cptr_t lcd, cptr_t src, 
			cptr_t dest);
int __lcd_cap_page_grant_map_cheat(struct lcd *caller, cptr_t lcd, 
				cptr_t page, cptr_t dest, gpa_t gpa);

/* IPC -------------------------------------------------- */

int __lcd_ipc_init(void);
void __lcd_ipc_exit(void);
int __lcd_create_sync_endpoint(struct lcd *caller, cptr_t slot);
int __lcd_send(struct lcd *caller, cptr_t endpoint);
int __lcd_recv(struct lcd *caller, cptr_t endpoint);
int __lcd_call(struct lcd *caller, cptr_t endpoint);
int __lcd_reply(struct lcd *caller);


/* CHECK & DESTROY  -------------------------------------------------- */

/*
 * These are executed by code in cap.c when a capability to an object
 * is revoked. It gives the microkernel a chance to update any state (like
 * removing an lcd from an endpoint queue, unmapping a page, etc.). If the
 * final capability is revoked, these are called, followed by the appropriate
 * destroy routine below.
 */
void __lcd_sync_endpoint_check(struct lcd *lcd, struct lcd_sync_endpoint *e);
void __lcd_page_check(struct lcd *lcd, struct page *p, int is_mapped, 
		gpa_t where_mapped);
void __lcd_kpage_check(struct lcd *lcd, struct page *p, int is_mapped, 
		gpa_t where_mapped);
void __lcd_check(struct lcd *owning_lcd, struct lcd *owned_lcd);

/*
 * These are called by code in cap.c when the last capability goes
 * away.
 */
void __lcd_sync_endpoint_destroy(struct lcd_sync_endpoint *e);
void __lcd_page_destroy(struct page *p);
void __lcd_kpage_destroy(struct page *p);
void __lcd_destroy(struct lcd *owned_lcd);

void __lcd_destroy__(struct lcd *owned_lcd);

#endif /* LCD_DOMAINS_INTERNAL_H */
