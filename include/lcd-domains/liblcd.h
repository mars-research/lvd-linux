/* 
 * liblcd.h - header for liblcd/
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * This is the minimal environment inside an lcd.
 *
 * This is x86_64 specific.
 */
#ifndef LCD_DOMAINS_LIBLCD_H
#define LCD_DOMAINS_LIBLCD_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>

#include <lcd-domains/types.h>
#include <lcd-domains/syscall.h>
#include <lcd-domains/utcb.h>


/* LCD REG ACCESS -------------------------------------------------- */

static inline struct lcd_utcb * lcd_get_utcb(void)
{
	struct lcd_utcb *out;
	/*
	 * The utcb is in the bottom of the stack page. All we have to
	 * do is mask off the low 12 bits to get the address.
	 */
	asm volatile(
		"movq %%rsp, %0 \n\t"
		"andq $0xFFFFFFFFFFFFF000, %0 \n\t"
		: "=g" (out)
		::);
	return out;
}

#define LCD_MK_REG_ACCESS(idx)						\
static inline u64 lcd_r##idx(void)					\
{									\
	return __lcd_r##idx(lcd_get_utcb());				\
}									\
static inline void lcd_set_r##idx(u64 val)				\
{									\
	__lcd_set_r##idx(lcd_get_utcb(), val);				\
}									\
static inline cptr_t lcd_cr##idx(void)					\
{									\
        return __lcd_cr##idx(lcd_get_utcb());				\
}								        \
static inline void lcd_set_cr##idx(cptr_t val)				\
{									\
	__lcd_set_cr##idx(lcd_get_utcb(), val);				\
}									
LCD_MK_REG_ACCESS(0)
LCD_MK_REG_ACCESS(1)
LCD_MK_REG_ACCESS(2)
LCD_MK_REG_ACCESS(3)
LCD_MK_REG_ACCESS(4)
LCD_MK_REG_ACCESS(5)
LCD_MK_REG_ACCESS(6)
LCD_MK_REG_ACCESS(7)


/* LOW LEVEL SYSCALLS -------------------------------------------------- */

/**
 * Low level syscall routines.
 */

#define __LCD_MK_SYSCALL(num)						\
static inline int lcd_syscall_##num(void)	                        \
{									\
        long ret;							\
	asm volatile(							\
		"movq $" #num ", %%rax \n\t" /* move syscall into rax */\
		"vmcall \n\t"                /* do vmcall (exit vm)   */\
		"movq %%rax, %0 \n\t"        /* get return value      */\
		: "=g" (ret)						\
		:							\
		: "rax");						\
	return (int)ret;						\
}									

#define LCD_MK_SYSCALL(name) __LCD_MK_SYSCALL(name)

LCD_MK_SYSCALL(LCD_SYSCALL_EXIT)
LCD_MK_SYSCALL(LCD_SYSCALL_SEND)
LCD_MK_SYSCALL(LCD_SYSCALL_RECV)
LCD_MK_SYSCALL(LCD_SYSCALL_CALL)
LCD_MK_SYSCALL(LCD_SYSCALL_REPLY)
LCD_MK_SYSCALL(LCD_SYSCALL_PUTCHAR)
LCD_MK_SYSCALL(LCD_SYSCALL_PAGE_ALLOC)
LCD_MK_SYSCALL(LCD_SYSCALL_PAGE_MAP)
LCD_MK_SYSCALL(LCD_SYSCALL_PAGE_UNMAP)
LCD_MK_SYSCALL(LCD_SYSCALL_CAP_DELETE)

#define __LCD_DO_SYSCALL(num) lcd_syscall_##num()
#define LCD_DO_SYSCALL(name) __LCD_DO_SYSCALL(name)


/* ENTER / EXIT FROM LCD -------------------------------------------------- */

/**
 * This should be called right when a thread starts executing inside an lcd.
 */
int lcd_enter(void);

/**
 * Exit from lcd with return value. Doesn't return.
 */
void __noreturn lcd_exit(int retval);


/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */

/**
 * Initialize the memory subsystem.
 *
 * This should be called when the lcd boots.
 */
void lcd_mem_init(void);
/**
 * Allocate a zero'd out page, and put the capability in slot_out.
 */
int lcd_page_alloc(cptr_t *slot_out);
/**
 * Map page at guest physical address gpa.
 */
int lcd_page_map(cptr_t page, gpa_t gpa);
/**
 * Unmap page at guest physical address gpa.
 */
int lcd_page_unmap(cptr_t page, gpa_t gpa);
/**
 * Maps pages in guest virtual *and* physical address spaces.
 */
int lcd_map_pages(cptr_t *pages, gva_t *base_out, unsigned order);
/**
 * Maps page in gv and gp address spaces.
 */
int lcd_map_page(cptr_t page_cptr, gva_t *gva_out);
/**
 * Unmaps pages in gv and gp address spaces.
 */
void lcd_unmap_pages(cptr_t *pages, gva_t base, unsigned order);
/**
 * Unmaps page in gv and gp address spaces.
 */
void lcd_unmap_page(cptr_t page_cptr, gva_t page);
/**
 * Allocates 2^order pages. These will be properly mapped in gv and gp
 * address spaces (so you're done). Returns gv address of first page
 * in base_out.
 */
int lcd_alloc_pages(unsigned order, gva_t *base_out);
/**
 * One page alloc.
 */
int lcd_alloc_page(gva_t *page_out);
/**
 * Frees 2^order pages. Undoes what lcd_alloc_pages did.
 */
void lcd_free_pages(gva_t base, unsigned order);
/**
 * Frees one page.
 */
void lcd_free_page(gva_t page);
/**
 * Return struct page associated with addr. Similar to Linux's
 * virt_to_head_page (but we don't ever use compound pages).
 *
 * Necessary to satisfy slab dependency.
 */
struct page * lcd_virt_to_head_page(const void *addr);
/**
 * Return guest virtual address (as void *) associated with
 * struct page. Similar to Linux's virt_to_page.
 *
 * Necessary to satisfy slab dependency.
 */
void *lcd_page_address(const struct page *);
/**
 * Wrapper around lcd_alloc_pages.
 *
 * Necessary to satisfy slab dependency.
 */
struct page * lcd_alloc_pages_exact_node(int nid, gfp_t gfp_mask,
					unsigned order);
/**
 * Wrapper around lcd_free_pages.
 *
 * Necessary to satisfy slab dependency.
 */
void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order);

/* IPC -------------------------------------------------- */

/**
 * Create a synchronous endpoint, capability stored in slot_out.
 */
int lcd_create_sync_endpoint(cptr_t *slot_out);
/**
 * Synchronous send. Set message registers using lcd_set_r0(), lcd_set_r1(), 
 * etc. before calling.
 */
int lcd_send(cptr_t endpoint);
/**
 * Synchronous recv. Set cptr registers (for receiving granted caps) using 
 * lcd_set_cr0(), lcd_set_cr1(), etc. before calling.
 */
int lcd_recv(cptr_t endpoint);
/**
 * Synchronous call. Blocks until callee replies on lcd's reply
 * endpoint.
 *
 * (Internally, this is a send followed by a receive.)
 */
int lcd_call(cptr_t endpoint);
/**
 * Reply to a synchronous call.
 */
int lcd_reply(void);

/* CAPABILITIES -------------------------------------------------- */

/**
 * Delete the capability in slot from caller's cspace.
 */
void lcd_cap_delete(cptr_t slot);

/* CPTR CACHE -------------------------------------------------- */

/**
 * Initialize the cptr cache.
 *
 * This should be called when an lcd boots.
 */
void lcd_init_cptr(void);
/**
 * Find an unused cptr in the lcd's cptr cache (a cptr that refers to an 
 * unused cnode).
 */
int lcd_alloc_cptr(cptr_t *free_slot);
/**
 * Return a cptr (after deleting a capability).
 */
void lcd_free_cptr(cptr_t c);

/* MISC -------------------------------------------------- */

/**
 * Print a message to the host kernel log. Almost all printk escapes are
 * supported.
 */
void lcd_printk(char *fmt, ...);

#endif  /* LCD_DOMAINS_LIBLCD_H */
