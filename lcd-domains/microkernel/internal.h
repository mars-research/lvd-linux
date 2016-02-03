#ifndef LCD_DOMAINS_INTERNAL_H
#define LCD_DOMAINS_INTERNAL_H

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/lcd-domains.h>
#include <lcd-domains/types.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/syscall.h>

/* --------------------------------------------------
 * DEBUG
 * --------------------------------------------------
 */

#define LCD_DEBUG_LVL 1

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

#define LCD_DEBUG_ERR  1
#define LCD_DEBUG_WARN 2
#define LCD_DEBUG_MSG  3
#define LCD_DEBUG_VERB 4

#define LCD_DEBUG(lvl, msg...) {					\
	if (lvl <= LCD_DEBUG_LVL)					\
		__lcd_debug(__FILE__, __LINE__, msg);			\
	}
		
static inline void __lcd_debug(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-domains: %s:%d: debug: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

/* --------------------------------------------------
 * INTERNAL ADDRESS CONVERSIONS
 * --------------------------------------------------
 *
 * This used to be in the types.h header, but it only
 * makes sense for internal microkernel calculations.
 */

static inline hpa_t pa2hpa(unsigned long pa)
{
	return (hpa_t){ pa };
}
static inline hpa_t va2hpa(void *va)
{
	return (hpa_t){ __pa(va) };
}
static inline void * hpa2va(hpa_t hpa)
{
	return __va(hpa_val(hpa));
}
static inline hva_t hpa2hva(hpa_t hpa)
{
	return (hva_t){ (unsigned long)__va(hpa.hpa) };
}
static inline void * hva2va(hva_t hva)
{
	return (void *)hva_val(hva);
}
static inline hva_t va2hva(void *va)
{
	return __hva((unsigned long)va);
}
static inline hpa_t hva2hpa(hva_t hva)
{
	return (hpa_t){ (unsigned long)__pa(hva2va(hva)) };
}

/* --------------------------------------------------
 * LCD MICROKERNEL CAPABILITY TYPES
 * --------------------------------------------------
 *
 * cap_types.c
 */
enum lcd_microkernel_type_id {
	LCD_MICROKERNEL_TYPE_ID_PAGE,
	LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE,
	LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM,
	LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM,
	LCD_MICROKERNEL_TYPE_ID_SYNC_EP,
	LCD_MICROKERNEL_TYPE_ID_LCD,
	LCD_MICROKERNEL_TYPE_ID_KLCD,

	LCD_MICROKERNEL_NUM_CAP_TYPES,
};

extern struct cap_type_system *lcd_libcap_type_system;

/**
 * __lcd_init_cap_types -- Initialize data structures for libcap type system
 *
 * Initializes the cap_type_system that is shared by all LCD cspaces. Should
 * be invoked when the microkernel initializes. Returns non-zero if init
 * fails.
 */
int __lcd_init_cap_types(void);
/**
 * __lcd_exit_cap_types -- Destroy/tear down data structures for type system
 *
 * Should be invoked before microkernel module is uninstalled from host.
 */
void __lcd_exit_cap_types(void);
/**
 * __lcd_get_libcap_type -- Translate our type id to libcap's type id
 * @t_id: one of the type id enum values
 *
 * Motivation: You need to know the libcap type id in order to insert
 * an object in a cspace.
 */
cap_type_t __lcd_get_libcap_type(enum lcd_microkernel_type_id t_id);

/* --------------------------------------------------
 * MEMORY
 * --------------------------------------------------
 */

/**
 * struct lcd_mapping_metadata
 *
 * This is used as cnode metadata in cspaces to record whether
 * a page capability has been mapped in an LCD's address
 * space, and if so, where. This information is needed when
 * the capability is revoked or deleted, so that we can
 * unmap the pages from the LCD's guest physical address space.
 */
struct lcd_mapping_metadata {
	int is_mapped;
	gpa_t where_mapped;
};
/**
 * struct lcd_memory_object
 *
 * This is used as a wrapper around struct page and arbitrary
 * host physical addresses, so that we can track the size of the
 * memory region. This is the object that is inserted into an LCD's
 * cspace (rather than a struct page, say).
 */
struct lcd_memory_object {
	enum lcd_microkernel_type_id sub_type;
	void *object;
	unsigned int order;
};

/* 
 * --------------------------------------------------
 *
 * LCDs - lightweight capability domain
 */


#define	LCD_STATUS_EMBRYO     0
#define	LCD_STATUS_CONFIGED   1
#define	LCD_STATUS_RUNNING    2
#define	LCD_STATUS_DEAD       4

enum lcd_type {
	LCD_TYPE_ISOLATED,
	LCD_TYPE_NONISOLATED,
	LCD_TYPE_TOP,
};

enum lcd_xmit_status {
	LCD_XMIT_INVALID = 0, /* when lcd is not in queue */
	LCD_XMIT_SUCCESS = 1, /* when send/recv succeeded */
	LCD_XMIT_FAILED  = 2, /* when send/recv failed    */
};

#define LCD_CONSOLE_BUFF_SIZE 512

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
	 * Type (isolated, non-isolated (klcd))
	 */
	enum lcd_type type;
	/*
	 * Status
	 */
	atomic_t status;
	/*
	 * My corresponding kthread
	 */
	struct task_struct *kthread;
	/*
	 * The arch-dependent parts of the lcd (e.g., the ept). (This needs
	 * to be aligned properly, so we use a pointer instead of embedding
	 * it.)
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * The LCD's cspace
	 */
	struct cspace *cspace;
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
	/*
	 * Console
	 */
	char console_buff[LCD_CONSOLE_BUFF_SIZE];
	unsigned console_cursor;
	/*
	 * KLCD specifics
	 * ==============
	 */
	int (*klcd_main)(void);
};

/* similar to task structs */
#define set_lcd_status(lcd, status_value)			\
	do {							\
		atomic_set(&(lcd)->status, (status_value));	\
		smp_mb();					\
	} while(0);					
#define get_lcd_status(lcd) (atomic_read(&(lcd)->status))
#define lcd_status_embryo(lcd) \
	(get_lcd_status(lcd) == LCD_STATUS_EMBRYO)
#define lcd_status_configed(lcd) \
	(get_lcd_status(lcd) == LCD_STATUS_CONFIGED)
#define lcd_status_running(lcd) \
	(get_lcd_status(lcd) == LCD_STATUS_RUNNING)
#define lcd_status_dead(lcd) \
	(get_lcd_status(lcd) == LCD_STATUS_DEAD)

#define set_lcd_xmit(lcd, xmit_val)				\
	do {							\
		atomic_set(&(lcd)->xmit_flag, (xmit_val));	\
		smp_mb();					\
	} while(0);
#define get_lcd_xmit(lcd) (atomic_read(&(lcd)->xmit_flag))

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

/* KLCD STUFF -------------------------------------------------- */

int __kliblcd_init(void);
void __kliblcd_exit(void);

int __klcd_enter(void);
void __klcd_exit(void);
int __klcd_page_zalloc(struct lcd *klcd, cptr_t c, hpa_t *hpa_out,
		hva_t *hva_out);
int __klcd_pages_zalloc(struct lcd *klcd, cptr_t *slots, 
			hpa_t *hp_base_out, hva_t *hv_base_out,
			unsigned order);

int __klcd_get_module(char *mdir, char *module_name, struct module **m);
void __klcd_put_module(char *module_name);
int __klcd_do_call_endpoint(cptr_t lcd);

int __klcd_create_klcd(struct lcd *caller, cptr_t slot);
int __lcd_create(struct lcd *caller, cptr_t slot);
int __lcd_create__(struct lcd **out);
int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp, 
		gpa_t gva_root, gpa_t stack_page);
int __lcd_run(struct lcd *caller, cptr_t lcd);
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


#endif /* LCD_DOMAINS_INTERNAL_H */
