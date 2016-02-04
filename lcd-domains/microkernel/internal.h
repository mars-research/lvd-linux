/*
 * internal.h
 *
 * Internal interfaces used in microkernel and kliblcd.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_INTERNAL_H
#define LCD_DOMAINS_INTERNAL_H

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/lcd-domains.h>
#include <lcd-domains/types.h>
#include <lcd-domains/liblcd/sync_ipc.h>
#include <lcd-domains/liblcd/syscall.h>

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
 * LCD Data Definitions
 * --------------------------------------------------
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
 * LCD CREATION AND CONFIGURATION
 * --------------------------------------------------
 *
 * create.c
 */

/**
 * __lcd_create_no_vm_no_thread -- Create an empty LCD with no underlying VM
 *                                 container, and no kthread
 * 
 * This is intended to be used with lcd_enter for a non-isolated thread
 * (entering capability mode). It sets up the thread's cspace, synchronous
 * IPC buffers, etc.
 */
int __lcd_create_no_vm_no_thread(struct lcd **out);
/**
 * __lcd_create_no_vm -- Create an empty LCD with no underlying VM container
 *
 * For now, you probably want to use __lcd_create_klcd.
 *
 * This is intended to be used for creating non-isolated LCDs (kLCDs) in
 * which we need a new kthread to run the LCD's code, but we don't want
 * the code to run inside an isolated VM.
 */
int __lcd_create_no_vm(struct lcd **out);
/**
 * __lcd_create -- Create an empty isolated LCD
 * @caller: the LCD doing the creating
 * @slot: the slot in the @caller's cspace where the LCD capability should
 *        be stored
 */
int __lcd_create(struct lcd *caller, cptr_t slot);
/**
 * __lcd_create_klcd -- Create an empty non-isolated LCD (kLCD)
 * @caller: the LCD doing the creating
 * @slot: the slot in the @caller's cspace where the LCD capability should
 *        be stored
 */
int __lcd_create_klcd(struct lcd *caller, cptr_t slot);
/**
 * __lcd_get -- Look up an LCD in caller's cspace
 * @caller: the LCD in whose cspace we look for capability
 * @lcd: the cptr to the slot that contains the LCD capability
 * @cnode: out param, the cnode in @caller's cspace that contains the LCD
 * @lcd_struct: out param, the LCD itself
 *
 * Locks cnode in @caller's cspace *and* the @lcd_struct. Call __lcd_put
 * to unlock both.
 */
int __lcd_get(struct lcd *caller, cptr_t lcd, struct cnode **cnode,
	struct lcd **lcd_struct);

/**
 * __lcd_put -- Release locks on LCD and containing cnode
 */
void __lcd_put(struct lcd *caller, struct cnode *cnode, struct lcd *lcd);

/**
 * __lcd_config -- Configure LCD's registers
 * @caller: the LCD doing the configuring
 * @lcd: the cptr to the LCD to configure
 * @pc: instruction pointer / program counter guest virtual address
 * @sp: stack pointer
 * @gva_root: physical address of root of guest virtual page tables
 * @utcb_page: where to map the UTCB in the LCD's address space
 */
int __lcd_config(struct lcd *caller, cptr_t lcd, gva_t pc, gva_t sp, 
		gpa_t gva_root, gpa_t utcb_page);

/**
 * __lcd_memory_grant_and_map -- Grant LCD memory object capability and
 *                               map memory object in LCD's address space
 * @caller: the LCD doing the granting
 * @lcd: cptr to LCD to grant to
 * @mo_cptr: cptr to memory object capability (e.g. pages)
 * @dest_slot: cptr to destination slot in @lcd's cspace
 * @base: base physical address where to map memory object.
 */
int __lcd_memory_grant_and_map(struct lcd *caller, cptr_t lcd, 
			cptr_t mo_cptr, cptr_t dest_slot, gpa_t base);

/**
 * __lcd_cap_grant -- Grant LCD capability directly (rather than via IPC)
 * @caller: LCD doing the granting
 * @lcd: cptr to LCD to grant to
 * @src: cptr to the slot in @caller's cspace that contains the capability to
 *       grant
 * @dest: cptr to slot in @lcd's cspace where we should store capability
 */
int __lcd_cap_grant(struct lcd *caller, cptr_t lcd, cptr_t src, cptr_t dest);

/**
 * __lcd_destroy_no_vm_no_thread -- Destroy an LCD that has no underlying
 *                                  VM, nor a kthread
 * @lcd: the LCD to destroy
 *
 * This is called during lcd_exit for non-isolated / kLCDs. It's for
 * non-isolated LCDs (no VM) who entered LCD mode via lcd_enter.
 *
 * @lcd pointer is no longer valid after this call.
 */
void __lcd_destroy_no_vm_no_thread(struct lcd *lcd);

/**
 * __lcd_destroy_no_vm -- Destroy an LCD with no underlying VM
 * @lcd: the LCD to destroy
 *
 * This is for destroying a non-isolated LCD *that was spawned by another
 * non-isolated LCD*. During this spawn process, we created a kthread to
 * run this LCD's module init. We need to destroy that kthread (as well
 * as the cspace, utcb, etc.).
 *
 * @lcd pointer is no longer valid after this call.
 */
void __lcd_destroy_no_vm(struct lcd *lcd);

/**
 * __lcd_destroy -- Destroy an isolated LCD
 * @lcd: the LCD to destroy
 *
 * This tears down everything -- kthread, VM, cspace, utcb, etc.
 *
 * @lcd pointer is no longer valid after this call.
 */
void __lcd_destroy(struct lcd *lcd);


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
 *
 * mem.c
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

/**
 * __lcd_alloc_pages_exact_node -- Alloc host pages and insert into caller's
 *                                 cspace
 * @caller: the LCD whose cspace we should insert into
 * @slot: the slot in the LCD's cspace to install the pages capability
 * @nid: the node to allocate from
 * @flags: gfp flags
 * @order: alloc 2^order pages
 *
 * This uses Linux's alloc_pages_exact_node internally.
 */
int __lcd_alloc_pages_exact_node(struct lcd *caller, cptr_t slot, int nid,
				unsigned int flags, unsigned int order);

/**
 * __lcd_alloc_pages -- Similar to __lcd_alloc_pages_exact_node
 */
int __lcd_alloc_pages(struct lcd *caller, cptr_t slot,
		unsigned int flags, unsigned int order);

/**
 * __lcd_get_memory_object -- Look up memory object in LCD's cspace
 * @caller: LCD whose cspace we should look in
 * @mo_cptr: cptr to slot in LCD's cspace
 * @cnode: out param, the cnode that contains the memory object
 * @mo_out: out param, the memory object itself
 *
 * This locks the cnode. Call __lcd_put_memory_object to release it.
 */
int __lcd_get_memory_object(struct lcd *caller, cptr_t mo_cptr, 
			struct cnode **cnode, 
			struct lcd_memory_object **mo_out);

/**
 * __lcd_put_memory_object -- Release lock on cnode from prior lookup
 */
void __lcd_put_memory_object(struct lcd *caller, struct cnode *cnode, 
			struct lcd_memory_object *mo);

/**
 * __lcd_do_map_memory_object -- Map memory object in LCD's physical
 *                               address space
 * @lcd: the LCD whose address space we should map into
 * @mo: the memory object to map
 * @meta: the per-LCD mapping metadata associated with the memory object
 * @base: the base physical address to map the memory object at
 *
 * Maps the memory object in a contiguous chunk of the LCD's physical
 * address space. Note: For non-isolated LCD's, this function is a no-op (all
 * memory objects / host physical addresses are accessible to non-isolated 
 * code).
 *
 * For isolated LCDs, if the memory object is already mapped, fails.
 */
int __lcd_do_map_memory_object(struct lcd *lcd, 
			struct lcd_memory_object *mo,
			struct lcd_mapping_metadata *meta,
			gpa_t base);

/**
 * __lcd_map_memory_object -- See __lcd_do_map_memory_object.
 *
 * This function just does the cspace lookup for the memory object, and
 * passes to __lcd_do_map_memory_object.
 */
int __lcd_map_memory_object(struct lcd *caller, cptr_t mo_cptr, gpa_t base);

/**
 * __lcd_unmap_memory_object -- Unmap memory object from LCD's physical
 *                              address space
 * @lcd: the LCD whose address space we should unmap from
 * @mo: the memory object to unmap
 * @meta: the per-LCD mapping metadata associated with the memory object
 *
 * For non-isolated LCDs, this function is a no-op (all memory objects
 * are always mapped in non-isolated code's physical address space).
 *
 * For isolated LCDs, this function silently does nothing if the
 * memory object is not mapped.
 */
void __lcd_do_unmap_memory_object(struct lcd *lcd, 
				struct lcd_memory_object *mo,
				struct lcd_mapping_metadata *meta);

/**
 * __lcd_unmap_memory_object -- Similar to __lcd_do_unmap_memory_object.
 *
 * This just does the cspace lookup, and calls __lcd_do_unmap_memory_object.
 */
void __lcd_unmap_memory_object(struct lcd *caller, cptr_t mo_cptr);

/**
 * __lcd_grant_memory_object -- Does the capability grant for memory objects
 * @src: the LCD to grant from
 * @dest: the LCD to grant to
 * @c_src: the cptr to the memory object capability in the src LCD's cspace
 * @c_dest: the cptr to the destination slot in the dest LCD's cspace
 *
 * This does a memory object grant and initializes the mapping metadata
 * for the destination LCD.
 */
int __lcd_grant_memory_object(struct lcd *src, struct lcd *dest,
			cptr_t c_src, cptr_t c_dest);
/**
 * __lcd_mem_init -- Call this when the microkernel initializes
 *
 * Initializes global data structures etc. used in the memory management
 * code.
 */
int __lcd_mem_init(void)

/**
 * __lcd_mem_exit -- Call this when the microkernel is exiting
 *
 * Tears down memory management internals.
 */
void __lcd_mem_exit(void)

/* --------------------------------------------------
 * GLOBAL MEMORY INTERVAL TREE
 * --------------------------------------------------
 *
 * mem_itree.c
 *
 * See more doc in that file.
 */

/**
 * struct lcd_mem_itree
 *
 * Global (across all LCDs) interval tree for tracking the
 * host memory that is under the microkernel's capability
 * system.
 */
struct lcd_mem_itree {
	struct rb_root root;
};
/**
 * struct lcd_mem_itree_node
 *
 * Node in the lcd_mem_itree. Contains pointer to the memory object
 * itself (the physical pages, device memory address, vmalloc address,
 * etc.).
 */
struct lcd_mem_itree_node {
	struct interval_tree_node it_node;
	struct lcd_memory_object *mo;
	unsigned int flags;
};
/**
 * __lcd_mem_itree_insert -- Insert a memory object into the appropriate
 *                           interval tree
 * @mo: the memory object to insert
 * @flags: the flags to store in the tree node
 *
 * If the memory object already exists in the tree, you will get duplicates.
 * This is an internal function. It's up to you to make sure you did the
 * lookup check first. (We don't do this check for every insert or else this
 * would be slow.)
 */
int __lcd_mem_itree_insert(struct lcd_memory_object *mo, unsigned int flags);
/**
 * __lcd_mem_itree_lookup -- Search for a node in the appropriate tree
 *                           for a memory object that contains address
 * @addr: address to search for
 * @type: the memory object type to search for
 * @node_out: out param, the tree node, if found
 *
 * If searching for physical memory (pages, device memory), @addr is
 * a physical address. If searching for vmalloc memory, @addr is a host
 * virtual address.
 */
int __lcd_mem_itree_lookup(unsigned long addr,
			enum lcd_microkernel_type_id type,
			struct lcd_mem_itree_node **node_out);
/**
 * __lcd_mem_itree_delete -- Delete a node from its containing memory interval
 *                           tree
 * @node: the node to delete
 */
int __lcd_mem_itree_delete(struct lcd_mem_itree_node *node);
/**
 * __lcd_mem_itree_init -- Call this when the microkernel initializes
 *
 * Initializes global memory interval tree.
 */
int __lcd_mem_itree_init(void);

/**
 * __lcd_mem_itree_exit -- Call this when the microkernel is exiting
 *
 * Tears down global memory interval tree.
 */
void __lcd_mem_itree_exit(void);

/* --------------------------------------------------
 * RUNNING LCDs
 * --------------------------------------------------
 *
 * run.c
 */

/**
 * __lcd_kthread_main -- This is what the dedicated per-LCD kthread runs
 *                       when it initializes
 *
 * @data is not used right now (NULL).
 */
int __lcd_kthread_main(void *data);

/**
 * __lcd_run -- Start an LCD
 * @caller: the LCD kicking off the run
 * @lcd: cptr to LCD capability in @caller's cspace
 *
 * This wakes up the per-LCD kthread (which will start running
 * __lcd_kthread_main).
 */
int __lcd_run(struct lcd *caller, cptr_t lcd);

/**
 * __lcd_run_init -- Invoke this when microkernel initializes
 */
int __lcd_run_init(void);

/**
 * __lcd_run_exit -- Invoke this before microkernel exits
 */
void __lcd_run_exit(void);

/* --------------------------------------------------
 * CONSOLE PRINTING FROM LCD
 * --------------------------------------------------
 *
 * console.c
 */

/**
 * __lcd_put_char -- Print character to LCD's buffer
 *
 * If the LCD's buffer fills up, or if @c = '\0', the LCD's buffer
 * is dumped to the host kernel logs.
 */
int __lcd_put_char(struct lcd *lcd, char c)

/**
 * __lcd_console_init -- Invoke when microkernel initializes
 */
int __lcd_console_init(void);

/**
 * __lcd_console_exit -- Invoke when microkernel exits
 */
void __lcd_console_exit(void)

/*
 * --------------------------------------------------
 * SYNCHRONOUS IPC
 * --------------------------------------------------
 *
 * ipc.c
 *
 * Synchronous endpoints contain a send/receive queue for lcd's 
 * (similar to an seL4 endpoint). Also known as `rendezvous point'.
 */
struct lcd_sync_endpoint {
	struct list_head senders;
	struct list_head receivers;
        struct mutex lock;
};

/**
 * __lcd_create_sync_endpoint__ -- Create synchronous IPC endpoint
 * @ep_out: out param, the created synchronous endpoint
 */
int __lcd_create_sync_endpoint__(struct lcd_sync_endpoint **ep_out);
/**
 * __lcd_destroy_sync_endpoint -- Destroy synchronous IPC endpoint
 */
void __lcd_destroy_sync_endpoint__(struct lcd_sync_endpoint *ep);
/**
 * __lcd_create_sync_endpoint -- Create synchronous IPC endpoint and
 *                               install it in @caller's cspace
 * @caller: LCD doing the creating
 * @slot: cptr to slot in @caller's cspace where to store sync endpoint
 */
int __lcd_create_sync_endpoint(struct lcd *caller, cptr_t slot);
/**
 * __lcd_get_sync_endpoint -- Look up synchronous IPC endpoint in LCD's
 *                            cspace, lock it
 * @caller: LCD in whose cspace we are looking
 * @sync_ep_cptr: cptr to slot in LCD's cspace that contains endpoint
 * @cnode: out param, the cnode containing the endpoint
 * @e: the endpoint
 *
 * This locks the @cnode and endpoint @e. Call __lcd_put_sync_endpoint
 * to release locks.
 */
int __lcd_get_sync_endpoint(struct lcd *caller, cptr_t sync_ep_cptr,
			struct cnode **cnode, 
			struct lcd_sync_endpoint **e);
/**
 * __lcd_put_sync_endpoint -- Release sync endpoint locks
 */
void __lcd_put_sync_endpoint(struct lcd *caller, struct cnode *cnode, 
			struct lcd_sync_endpoint *e);
/**
 * __lcd_send -- Sync IPC send
 * @caller: LCD doing the sending
 * @endpoint: cptr to sync IPC endpoint
 */
int __lcd_send(struct lcd *caller, cptr_t endpoint);
/**
 * __lcd_recv -- Sync IPC receive
 * @caller: LCD doing the receiving
 * @endpoint: cptr to sync IPC endpoint
 */
int __lcd_recv(struct lcd *caller, cptr_t endpoint);
/**
 * __lcd_call -- Sync IPC call
 * @caller: LCD doing the calling
 * @endpoint: cptr to sync IPC endpoint
 *
 * This is equivalent to an __lcd_send followed by an __lcd_recv
 * on @caller's private endpoint (LCD_CPTR_CALL_ENDPOINT).
 */
int __lcd_call(struct lcd *caller, cptr_t endpoint);
/**
 * __lcd_reply -- Sync IPC reply
 * @caller: LCD doing the replying
 *
 * The endpoint is implicit: it is stored in a dedicated slot in the
 * @caller's cspace (LCD_CPTR_REPLY_ENDPOINT).
 */
int __lcd_reply(struct lcd *caller);
/**
 * __lcd_ipc_init -- Invoke when microkernel initiailizes
 *
 * Initialize synchronous IPC code, data structures.
 */
int __lcd_ipc_init(void);
/**
 * __lcd_ipc_exit -- Invoke before microkernel exits
 */
void __lcd_ipc_exit(void);

#endif /* LCD_DOMAINS_INTERNAL_H */
