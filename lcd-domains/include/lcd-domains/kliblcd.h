/* 
 * kliblcd.h - header for kliblcd.ko
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * This is the non-isolated code interface to the microkernel. The
 * implementation is in virt/lcd-domains/kliblcd.c.
 *
 * An LCD that runs in non-isolated code is called a klcd.
 */
#ifndef LCD_DOMAINS_KLIBLCD_H
#define LCD_DOMAINS_KLIBLCD_H

#include <lcd-domains/types.h>
#include <lcd-domains/utcb.h>
#include <linux/sched.h>

/* KLIBLCD INTERNALS -------------------------------------------------- */

struct lcd_info;

int __klcd_alloc_cptr(struct cptr_cache *cptr_cache, cptr_t *free_cptr);
void __klcd_free_cptr(struct cptr_cache *cptr_cache, cptr_t c);
int klcd_init_cptr(struct cptr_cache **c_out);
void klcd_destroy_cptr(struct cptr_cache *c);
int klcd_alloc_cptr(cptr_t *free_slot);
void klcd_free_cptr(cptr_t c);
int klcd_add_page(struct page *p, cptr_t *slot_out);
void klcd_rm_page(cptr_t slot);
int klcd_enter(void);
void klcd_exit(int retval);
int klcd_page_alloc(cptr_t *slot_out, gpa_t gpa);
int klcd_pages_alloc(cptr_t *slots_out, hpa_t *hp_base_out, 
		hva_t *hv_base_out, unsigned order);
int klcd_phys_addr_to_page_cptr(unsigned long data, 
				unsigned long len, 
				cptr_t *page_cptr_out, 
				unsigned long *offset_out);
int klcd_virt_addr_to_page_cptr(char *data, unsigned long len, 
				cptr_t *page_cptr_out, 
				unsigned long *offset_out);
int klcd_addr_to_page_cptr_cleanup(cptr_t page_cptr);
int klcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out);
int klcd_free_page(cptr_t page);
int klcd_map_pages_phys(cptr_t *pages, gpa_t *base_out, unsigned order);
int klcd_map_pages_both(cptr_t *pages, gva_t *base_out, unsigned order);
void klcd_unmap_pages_both(cptr_t *pages, gva_t base, unsigned order);
int klcd_create_sync_endpoint(cptr_t *slot_out);
int klcd_send(cptr_t endpoint);
int klcd_recv(cptr_t endpoint);
int klcd_call(cptr_t endpoint);
int klcd_reply(void);
int klcd_create(cptr_t *slot_out);
int klcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
		gpa_t utcb_page);
int klcd_run(cptr_t lcd);
int klcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest);
int klcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, gpa_t gpa);
void klcd_cap_delete(cptr_t slot);
int klcd_cap_revoke(cptr_t slot);
int klcd_load_module(char *mdir, char *mname, cptr_t mloader_endpoint, 
		struct lcd_info **mi);
void klcd_unload_module(struct lcd_info *mi, cptr_t mloader_endpoint);
int klcd_create_module_lcd(cptr_t *slot_out, char *mdir, char *mname, 
			cptr_t mloader_endpoint, struct lcd_info **mi);
void klcd_destroy_module_lcd(cptr_t lcd, struct lcd_info *mi,
			cptr_t mloader_endpoint);
int klcd_dump_boot_info(struct lcd_info *mi);
struct lcd_utcb * klcd_get_utcb(void);

/* DEBUG ------------------------------------------------------------ */

#define LIBLCD_ERR(msg...) __kliblcd_err(__FILE__, __LINE__, msg)
static inline void __kliblcd_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "error in klcd (kthread 0x%p): %s:%d: error: ", 
		current, file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LIBLCD_MSG(msg...) __kliblcd_msg(__FILE__, __LINE__, msg)
static inline void __kliblcd_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "msg in klcd (kthread 0x%p): %s:%d: note: ", 
		current, file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LIBLCD_WARN(msg...) __kliblcd_warn(__FILE__, __LINE__, msg)
static inline void __kliblcd_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "warn in klcd (kthread 0x%p): %s:%d: warning: ", 
		current, file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

/* KLCD-SPECIFIC STUFF -------------------------------------------------- */

struct lcd;

#define KLCD_MK_REG_ACCESS(idx)						\
extern u64 __klcd_r##idx(struct lcd *lcd);			        \
extern void __klcd_set_r##idx(struct lcd *lcd, u64 val);		\
extern cptr_t __klcd_cr##idx(struct lcd *lcd);				\
extern void __klcd_set_cr##idx(struct lcd *lcd, cptr_t val);		\
static inline u64 lcd_r##idx(void)					\
{									\
        return __klcd_r##idx(current->lcd);				\
}									\
static inline void lcd_set_r##idx(u64 val)				\
{									\
	__klcd_set_r##idx(current->lcd, val);				\
}									\
static inline cptr_t lcd_cr##idx(void)					\
{									\
        return __klcd_cr##idx(current->lcd);				\
}								        \
static inline void lcd_set_cr##idx(cptr_t val)				\
{									\
	__klcd_set_cr##idx(current->lcd, val);				\
}									
KLCD_MK_REG_ACCESS(0)
KLCD_MK_REG_ACCESS(1)
KLCD_MK_REG_ACCESS(2)
KLCD_MK_REG_ACCESS(3)
KLCD_MK_REG_ACCESS(4)
KLCD_MK_REG_ACCESS(5)
KLCD_MK_REG_ACCESS(6)
KLCD_MK_REG_ACCESS(7)

/* GET UTCB -------------------------------------------------- */

static inline struct lcd_utcb * lcd_get_utcb(void)
{
	return klcd_get_utcb();
}

/* KLCD SPECIFICS -------------------------------------------------- */

/**
 * Put a kernel page in the caller's cspace at slot_out. The microkernel will 
 * not free such pages when the last capability to them goes away - the caller
 * is responsible for freeing them.
 *
 * (This is used for adding module pages.)
 */
int klcd_add_page(struct page *p, cptr_t *slot_out);
/**
 * Remove a kernel page from the caller's cspace at slot. This will
 * automatically revoke any capabilities that were derived. Doesn't free page.
 */
void klcd_rm_page(cptr_t slot);


/* LCD ENTER / EXIT -------------------------------------------------- */


/**
 * Thread enter lcd mode. This is required before invoking anything.
 */
static inline int lcd_enter(void)
{
	return klcd_enter();
}
/**
 * Thread exit lcd mode. This will tear down the thread's cspace, etc.
 *
 * For klcd's, the kernel thread won't die, and retval is ignored (for now).
 */
static inline void lcd_exit(int retval)
{
	return klcd_exit(retval);
}


/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */


/**
 * Allocate a zero'd out page, and put the capability in slot_out. Map it
 * at gpa in the guest physical address space.
 */
static inline int lcd_page_alloc(cptr_t *slot_out, gpa_t gpa)
{
	return klcd_page_alloc(slot_out, gpa);
}
/**
 * Allocate 2**order zero'd out pages, and put capabilities in slots_out.
 * slots_out should be an array with at least 2**order slots. Returns
 * guest physical and virtual addresses of first page. (For KLCDs, the
 * guest physical and virtual will be == to the host physical and virtual.)
 */
static inline int lcd_pages_alloc(cptr_t *slots_out, gpa_t *gp_base_out, 
				gva_t *gv_base_out, unsigned order)
{
	hpa_t hp_base_out;
	hva_t hv_base_out;
	int ret;

	ret = klcd_pages_alloc(slots_out, &hp_base_out, &hv_base_out, order);
	if (ret)
		return ret;
	/*
	 * For KLCDs, gpa = hpa, gva = hva.
	 */
	*gp_base_out = __gpa(hpa_val(hp_base_out));
	*gv_base_out = __gva(hva_val(hv_base_out));

	return ret;
}

/**
 * Higher level routine to get a free page. Maps it in guest physical
 * and virtual address spaces. Returns slot and addresses.
 */
static inline int lcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out)
{
	return klcd_gfp(slot_out, gpa_out, gva_out);
}

/**
 * Free a page allocated with lcd_gfp.
 */
static inline int lcd_free_page(cptr_t page)
{
	return klcd_free_page(page);
}

/* ADDR -> CPTR TRANSLATION ------------------------------ */

/**
 * Get the cptr for the page that contains data, and the offset
 * into the page. If data + len goes beyond the page, returns
 * EINVAL. _virt_ takes a guest virtual address, _phys_ takes a 
 * guest physical address.
 *
 * For non-isolated code, these functions will determine the
 * page that contains the data and "volunteer" the page to the
 * microkernel for cspace management (i.e., will insert the page
 * into the caller's cspace).
 *
 * This is why it is IMPORTANT that you call lcd_addr_to_page_cptr_cleanup
 * when you are finished sharing the page (perhaps you have revoked
 * access to it). This will "unvolunteer" the page, removing it from
 * the caller's cspace. (It's not the end of the world if you don't
 * call lcd_addr_to_page_cptr_cleanup, you'll just have pages floating around
 * in your cspace that you may not have expected to see there.)
 *
 * (Further NOTE: if you call this function multiple times with the
 * same pointer, or multiple pointers inside the same page, the page
 * will be inserted multiple times inside the caller's cspace. Yes, we
 * could keep track of whether a page has been inserted already, but it
 * leads to a bit of memory and compute overhead that I'm avoiding for now.)
 */
static inline int lcd_phys_addr_to_page_cptr(unsigned long data, 
					unsigned long len, 
					cptr_t *page_cptr_out, 
					unsigned long *offset_out)
{
	return klcd_phys_addr_to_page_cptr(data, len, page_cptr_out, 
					offset_out);
}
static inline int lcd_virt_addr_to_page_cptr(char *data, unsigned long len, 
					cptr_t *page_cptr_out, 
					unsigned long *offset_out)
{
	return klcd_virt_addr_to_page_cptr(data, len, 
					page_cptr_out, offset_out);
}

/**
 * This function should be called when you are no longer using the
 * page returned from the lookup functions above (phys/virt addr to page cptr).
 *
 * For the motivation, see the doc for the address lookup functions above.
 */
static inline int lcd_addr_to_page_cptr_cleanup(cptr_t page_cptr)
{
	return klcd_addr_to_page_cptr_cleanup(page_cptr);
}

/* PAGE MAPPING ---------------------------------------- */

/**
 * "Map" 2^order pages in caller's physical address space. For
 * non-isolated code, the pages will already be available in the
 * physical address space (non-isolated code has access to host
 * physical). This function just locates where the pages are, and
 * returns the address in base_out. (Note that gpa == hpa for non-isolated
 * code.)
 *
 * This function assumes the pages are contiguous in host physical.
 */
static inline int lcd_map_pages_phys(cptr_t *pages, gpa_t *base_out, 
				unsigned order)
{
	return klcd_map_pages_phys(pages, base_out, order);
}
/**
 * "Unmap" 2^order pages that were previously mapped via lcd_map_pages_phys.
 * Note that this is a no-op for non-isolated code, and is only here to
 * make non-isolated and isolated code symmetric (for isolated code,
 * this function is meaningful, as it will update the isolated code's
 * guest physical address space).
 */
static inline void lcd_unmap_pages_phys(cptr_t *pages, gpa_t base, 
					unsigned order)
{
	return;
}
/**
 * Map 2^order pages in the guest physical *and* virtual address spaces
 * of the caller. Returns the virtual address of the starting point of the 
 * pages.
 *
 * Note: For 64-bit x86 (current implementation), this is basically
 * just a lookup. No mapping is done. (The pages are already mapped. See
 * Documentation/x86/x86_64/mm.txt.)
 */
static inline int lcd_map_pages_both(cptr_t *pages, gva_t *base_out, 
				unsigned order)
{
	return klcd_map_pages_both(pages, base_out, order);
}
/**
 * Unmap 2^order pages in both address spaces.
 *
 * For 64-bit x86 (current implementation), this is a no-op. (The pages
 * are always mapped. See note for lcd_map_pages_both.)
 */
static inline void lcd_unmap_pages_both(cptr_t *pages, gva_t base, 
					unsigned order)
{
	return;
}

/**
 * The following just call the above functions
 */
static inline int lcd_map_page_phys(cptr_t page_cptr, gpa_t *gpa_out)
{
	return lcd_map_pages_phys(&page_cptr, gpa_out, 0);
}
static inline int lcd_map_page_both(cptr_t page_cptr, gva_t *gva_out)
{
	return lcd_map_pages_both(&page_cptr, gva_out, 0);
}
static inline void lcd_unmap_page_phys(cptr_t page_cptr, gpa_t gpa)
{
	lcd_unmap_pages_phys(&page_cptr, gpa, 0);
}
static inline void lcd_unmap_page_both(cptr_t page_cptr, gva_t gva)
{
	lcd_unmap_pages_both(&page_cptr, gva, 0);
}


/* IPC -------------------------------------------------- */

/**
 * Create a synchronous endpoint, capability stored in slot_out.
 */
static inline int lcd_create_sync_endpoint(cptr_t *slot_out)
{
	return klcd_create_sync_endpoint(slot_out);
}
/**
 * Synchronous send. Set message registers using lcd_set_r0(), lcd_set_r1(), 
 * etc. before calling.
 */
static inline int lcd_send(cptr_t endpoint)
{
	return klcd_send(endpoint);
}
/**
 * Synchronous recv. Set cptr registers (for receiving granted caps) using 
 * lcd_set_cr0(), lcd_set_cr1(), etc. before calling.
 */
static inline int lcd_recv(cptr_t endpoint)
{
	return klcd_recv(endpoint);
}
/**
 * Synchronous call. Blocks until callee replies on lcd's reply
 * endpoint.
 *
 * (Internally, this is a send followed by a receive.)
 */
static inline int lcd_call(cptr_t endpoint)
{
	return klcd_call(endpoint);
}
/**
 * Reply to a synchronous call.
 */
static inline int lcd_reply(void)
{
	return klcd_reply();
}

/* LCD CREATE / SETUP -------------------------------------------------- */


/**
 * Allocates lcd and does minimal initialization of hardware virtual
 * machine and lcd's cspace. Returns non-zero on error. 
 *
 * (The microkernel will allocate a page for the stack/utcb - it can't 
 * trust the caller, and the microkernel needs safe access to the utcb during
 * ipc.)
 */
static inline int lcd_create(cptr_t *slot_out)
{
	return klcd_create(slot_out);
}
/**
 * Configure lcd environment.
 *
 * For now, we assume lcd will boot with a guest virtual address space.
 *
 * Set program counter, stack pointer, and root of guest virtual page table
 * hierarchy.
 *
 * The lcd's guest physical address space is set up using 
 * lcd_cap_page_grant_map.
 *
 * The lcd's cspace is configured using lcd_cap_grant.
 *
 * utcb_page should be the guest physical address where the utcb should be 
 * mapped.
 */
static inline int lcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
			gpa_t utcb_page)
{
	return klcd_config(lcd, pc, sp, gva_root, utcb_page);
}
/**
 * Runs / resumes an lcd.
 */
static inline int lcd_run(cptr_t lcd)
{
	return klcd_run(lcd);
}


/* CAPABILITIES -------------------------------------------------- */


/**
 * Grant a capability to an lcd we have created.
 *
 * Use lcd_cap_page_grant_map for granting caps to pages.
 *
 * Yes, I'm breaking from seL4 here by allowing grant to happen outside
 * of ipc.
 *
 * It's for one special case: When the caller is
 * setting up an lcd and needs to map pages inside it, we need to put
 * capabilities to those pages in the lcd's cspace. Why? Because the microkernel
 * needs to know how to unmap those pages when they are destroyed, or rights
 * are revoked somewhere. If we didn't add capabilities to the pages when
 * setting up the new lcd, the pages could be freed, but still mapped in the 
 * lcd's guest physical address space without the microkernel being aware.
 *
 * The alternative (discussed with Anton) is to have the creator pass the
 * capabilities when the lcd is booted. This works for endpoint capabilities,
 * etc., but the microkernel can't rely on the creator for pages.
 */
static inline int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return klcd_cap_grant(lcd, src, dest);
}
/**
 * Grant a capability to page to lcd, and map page in the lcd's guest physical
 * address space at gpa.
 */
static inline int lcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, 
					gpa_t gpa)
{
	return klcd_cap_page_grant_map(lcd, page, dest, gpa);
}
/**
 * Delete the capability in slot from caller's cspace.
 *
 * This may change the state of the caller. (For example, if the caller is
 * a regular lcd, and if the capability is to a page, the page will be unmapped
 * from the caller's address space.)
 *
 * If this is the last capability to the object, the object will be destroyed,
 * unless it is a kernel page. See klcd_add_page and klcd_rm_page.
 */
static inline void lcd_cap_delete(cptr_t slot)
{
	return klcd_cap_delete(slot);
}
/**
 * Revoke all derived capabilities.
 *
 * Does not delete the caller's capability.
 *
 * This may change the state of the lcd's whose capabilities are revoked (see
 * comment lcd_cap_delete).
 */
static inline int lcd_cap_revoke(cptr_t slot)
{
	return klcd_cap_revoke(slot);
}

/* CPTR CACHE -------------------------------------------------- */

/**
 * Initialize the cptr cache.
 *
 * This should be called when an lcd boots.
 */
static inline int lcd_init_cptr(void)
{
	return klcd_init_cptr(&current->cptr_cache);
}
/**
 * This should be called before an lcd exits.
 */
static inline void lcd_destroy_cptr(void)
{
	klcd_destroy_cptr(current->cptr_cache);
	current->cptr_cache = NULL;
}
/**
 * Find an unused cptr (a cptr that refers to an unused cnode).
 */
static inline int lcd_alloc_cptr(cptr_t *free_slot)
{
	return klcd_alloc_cptr(free_slot);
}
/**
 * Return a cptr (after deleting a cnode).
 */
static inline void lcd_free_cptr(cptr_t c)
{
	return klcd_free_cptr(c);
}
/**
 * This is needed when an lcd is creating another lcd (it needs to set up
 * the other lcd's cptr cache).
 */
static inline int __lcd_alloc_cptr(struct cptr_cache *cache, 
				cptr_t *free_slot)
{
	return __klcd_alloc_cptr(cache, free_slot);
}
/**
 * Same comment as __lcd_alloc_cptr.
 */
static inline void __lcd_free_cptr(struct cptr_cache *cache, cptr_t c)
{
	return __klcd_free_cptr(cache, c);
}

/* DATA STORE -------------------------------------------------- */

/* 
 * Adapted from cptrs / capabilities.
 */
struct dstore;
struct dstore_node;

/**
 * These tags are reserved.
 */
#define LCD_DSTORE_TAG_NULL 0
#define LCD_DSTORE_TAG_DSTORE_NODE 1

/**
 * Initializes caches, etc. in data store subsystem. Should be called when
 * LCD boots, but after the slab allocator is initialized.
 */
int lcd_dstore_init(void);
/**
 * Tears down caches, etc. in data store subsystem. Should be called before
 * LCD exits (not critical right now).
 */
void lcd_dstore_exit(void);
/**
 * Sets up a data store.
 */
int lcd_dstore_init_dstore(struct dstore **out);
/**
 * Inserts object into data store. The caller can associate
 * an arbitrary integer tag with the object so that it can do basic
 * type checking. Tags 0 and 1 are reserved.
 *
 * Returns dptr where object is stored in out.
 */
int lcd_dstore_insert(struct dstore *dstore, void *object, int tag, 
		dptr_t *out);
/**
 * Removes object from data store. Unlike capabilities, does not free object
 * or do anything else; the caller is responsible for tracking that.
 *
 * Silently fails if no object is stored at d, or if d is invalid.
 */
void lcd_dstore_delete(struct dstore *dstore, dptr_t d);
/**
 * Tears down data store.
 */
void lcd_dstore_destroy(struct dstore *dstore);
/**
 * Look up object in dstore at d. Ensure it has tag. Returns dstore node
 * containing object in out.
 *
 * ** THIS LOCKS THE DSTORE NODE. CALL A MATCHING lcd_dstore_put TO RELEASE **
 *
 * This can be used to synchronize multiple threads that are trying to
 * look up the object and modify it.
 *
 * Returns non-zero if object not found, or if object found but has wrong
 * tag.
 */
int lcd_dstore_get(struct dstore *dstore, dptr_t d, int tag, 
		struct dstore_node **out);
/**
 * Extracts object from dstore node.
 */
void *lcd_dstore_node_object(struct dstore_node *n);
/**
 * Release the lock on the dstore object stored at d.
 */
void lcd_dstore_put(struct dstore_node *n);


/* EXTRAS -------------------------------------------------- */

/**
 * Create a non-isolated `LCD'. This essentially initializes everything
 * except the low-level hardware VM container.
 *
 * The klcd can be configured etc. just like a regular lcd (program counter,
 * stack - though you should probably use the same stack that was set up
 * by the kernel when the kthread was created, and so on). You can grant
 * the klcd caps too.
 */
int klcd_create_klcd(cptr_t *klcd_out);

/**
 * Create a non-isolated LCD. This basically loads a kernel module (with
 * name mname) and creates a kernel thread to run its module init. This
 * allows the caller to grant capabilities to a non-isolated thread.
 */
int klcd_create_module_klcd(cptr_t *slot_out, char *mdir, char *mname);

/**
 * Destroys non-isolated LCD / thread (it will have to wait until that
 * thread exits the module's init, so this may block!). Unloads the
 * module with name module_name. It's up to you to make sure you pass the
 * same module name that you used in klcd_create_module_klcd.
 */
void klcd_destroy_module_klcd(cptr_t klcd, char *module_name);

/**
 * When provided with an endpoint connected to a module loader, this routine
 * will communicate with the module loader and load the module with name and
 * get capabilities to the pages that contain the module. It returns a
 * list of lcd_module_pages inside the doubly-linked list head. (The pages
 * are in this order: init pages followed by core pages.)
 *
 * The caller is responsible for freeing the list of lcd_module_page structs
 * (e.g., via lcd_unload_module).
 *
 * (For klcd's, for now anyway, the endpoint is ignored, so the null cptr
 * can be passed. The non-isolated module loading code is used.)
 */
struct lcd_module_page {
	/*
	 * cptr to page
	 */
	cptr_t cptr;
	/*
	 * guest virtual address where module page should be mapped inside
	 * lcd in order for module to work correctly without relinking
	 */
	gva_t gva;
	/*
	 * linked list of module pages
	 */
	struct list_head list;
};

struct lcd_page_info_list_elem {
	gpa_t page_gpa;
	cptr_t my_cptr;
	struct list_head list;
};

struct lcd_info {
	/*
	 * Module name
	 */
	char mname[LCD_MODULE_NAME_MAX];
	/*
	 * Where to point the program counter to run init
	 */
	gva_t init;
	/*
	 * List of lcd_module_pages
	 */
	struct list_head mpages_list;
	/*
	 * cptr cache
	 */
	struct cptr_cache *cache;
	/*
	 * Boot page
	 */
	char *boot_page_base;
	/*
	 * Boot mem page infos
	 */
	struct list_head boot_mem_list;
	/*
	 * Stack page infos
	 */
	struct list_head stack_mem_list;
	/*
	 * Paging mem page infos
	 */
	struct list_head paging_mem_list;
	/*
	 * Free mem page infos
	 */
	struct list_head free_mem_list;
};

static inline struct lcd_boot_info * to_boot_info(struct lcd_info *mi)
{
	return (struct lcd_boot_info *)mi->boot_page_base;
}

static inline int lcd_load_module(char *mdir, char *mname, 
				cptr_t mloader_endpoint, 
				struct lcd_info **mi)
{
	return klcd_load_module(mdir, mname, mloader_endpoint, mi);
}
/**
 * Deletes capabilities to pages that contain module (passed as doubly-linked
 * list of lcd_module_page's). (If the caller is a klcd, this will never
 * lead to freeing the module pages.)
 *
 * Tells module loader we are done with the module. (If the caller is a klcd,
 * this is a call to the regular module loading code to delete the module.)
 */
static inline void lcd_unload_module(struct lcd_info *mi, 
				cptr_t mloader_endpoint)
{
	return klcd_unload_module(mi, mloader_endpoint);
}
/**
 * Big routine to automatically create an lcd, load a module inside it,
 * and configure it per the address space layout below. Uses lcd_load_module
 * to do so (so a klcd can pass a null cptr for the module loader endpoint),
 * and lcd_config to set up program counter, stack pointer, etc.
 *
 * Puts the new lcd in slot.
 *
 * Returns an lcd_info that contains the module's name and the list of 
 * lcd_module_pages. Call lcd_destroy_module_lcd to tear it down. So long as 
 * you don't pass the capability to any other lcd, etc., this will stop and 
 * destroy the lcd.
 *
 * Address Spaces
 * ==============
 *
 * See types.h.
 *
 * Initial CSPACE
 * ==============
 *
 * The lcd's cspace will contain capabilities to the module pages and
 * guest virtual paging pages.
 */

/* Moved macros to types.h to share with liblcd code. */


static inline int lcd_create_module_lcd(cptr_t *slot_out, 
					char *mdir,
					char *mname,
					cptr_t mloader_endpoint, 
					struct lcd_info **mi)
{
	return klcd_create_module_lcd(slot_out, mdir, mname, mloader_endpoint,
				mi);
}

static inline void lcd_destroy_module_lcd(cptr_t lcd, struct lcd_info *mi,
					cptr_t mloader_endpoint)
{
	return klcd_destroy_module_lcd(lcd, mi, mloader_endpoint);
}

static inline int lcd_dump_boot_info(struct lcd_info *mi)
{
	return klcd_dump_boot_info(mi);
}

#endif  /* LCD_DOMAINS_KLIBLCD_H */
