/* 
 * liblcd.h - header for liblcd.ko
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * This is the *isolated* code interface to the microkernel. The
 * implementation is in arch/x86/lcd-domains/liblcd.c.
 *
 * This code runs in a very bare environment.
 *
 * We don't use arch headers for asm because some macros aren't 
 * available (for e.g., bitwise and), and this has to run in 64 bit anyway.
 */
#ifndef _ASM_X86_LCD_DOMAINS_LIBLCD_H
#define _ASM_X86_LCD_DOMAINS_LIBLCD_H

#include <lcd-domains/types.h>
#include <lcd-domains/syscall.h>
#include <lcd-domains/utcb.h>
#include <linux/errno.h>

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


#define __LCD_DO_SYSCALL(num) lcd_syscall_##num()
#define LCD_DO_SYSCALL(name) __LCD_DO_SYSCALL(name)


/* ENTER / EXIT FROM LCD -------------------------------------------------- */

/**
 * This should be called right when a thread starts executing inside an lcd.
 * Sets up cptr cache and other odds and ends.
 */
int lcd_enter(void);

/**
 * Exit from lcd with return value. Doesn't return.
 */
void __noreturn lcd_exit(int retval);


/* LOW LEVEL PAGE ALLOC -------------------------------------------------- */


/**
 * Allocate a zero'd out page, and put the capability in slot_out. Map it
 * at gpa.
 */
int lcd_page_alloc(cptr_t *slot_out, gpa_t gpa);
/**
 * Higher level routine to get a free page. Maps it in guest physical
 * and virtual address spaces. Returns slot and addresses.
 */
int lcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out);


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


/* LCD CREATE / SETUP -------------------------------------------------- */


/**
 * Allocates lcd and does minimal initialization of hardware virtual
 * machine and lcd's cspace. Returns non-zero on error. Stack should be
 * the guest physical address where stack/utcb should be mapped (the 
 * microkernel will allocate a page for the stack/utcb - it can't trust
 * the caller, and the microkernel needs safe access to the utcb during
 * ipc).
 */
int lcd_create(cptr_t *slot_out, gpa_t stack);
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
 */
int lcd_config(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root);
/**
 * Runs / resumes an lcd.
 */
int lcd_run(cptr_t lcd);
/**
 * Suspend an lcd.
 *
 * This will not tear it down - this happens when the last capability to the
 * lcd goes away. Use lcd_delete or lcd_revoke as necessary.
 *
 * Blocks until lcd halts. Warning: This could be a while if the lcd is
 * sitting in an endpoint queue.
 */
int lcd_suspend(cptr_t lcd);


/* CAPABILITIES -------------------------------------------------- */


/**
 * Grant a capability to an lcd we have created.
 *
 * Use lcd_cap_page_grant_map for granting caps to pages.
 *
 * Yes, I'm breaking from seL4 here by allowing grant to happen outside
 * of ipc.

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
int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest);
/**
 * Grant a capability to page to lcd, and map page in the lcd's guest physical
 * address space at gpa.
 */
int lcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, gpa_t gpa);
/**
 * Delete the capability in slot from caller's cspace.
 *
 * This may change the state of the caller. (For example, if the caller is
 * a regular lcd, and if the capability is to a page, the page will be unmapped
 * from the caller's address space.)
 *
 * If this is the last capability to the object, the object will be destroyed,
 * unless it is a kernel page.
 */
void lcd_cap_delete(cptr_t slot);
/**
 * Revoke all derived capabilities.
 *
 * Does not delete the caller's capability.
 *
 * This may change the state of the lcd's whose capabilities are revoked (see
 * comment lcd_cap_delete).
 */
int lcd_cap_revoke(cptr_t slot);


/* CPTR CACHE -------------------------------------------------- */


/**
 * Find an unused cptr (a cptr that refers to an unused cnode).
 */
int lcd_alloc_cptr(cptr_t *free_slot);
/**
 * Return a cptr (after deleting a cnode).
 */
void lcd_free_cptr(cptr_t c);


/* EXTRAS -------------------------------------------------- */


/* TODO : not implemented for regular lcd's yet */


/**
 * When provided with an endpoint connected to a module loader, this routine
 * will communicate with the module loader and load the module with name and
 * get capabilities to the pages that contain the module. It returns a
 * list of lcd_module_pages inside the doubly-linked list head.
 *
 * The caller is responsible for freeing the list of lcd_module_page structs
 * (e.g., via lcd_unload_module).
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
#define LCD_MODULE_NAME_MAX (64 - sizeof(unsigned long))
struct lcd_module_info {
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
};

int lcd_load_module(char *mname, cptr_t mloader_endpoint, 
		struct lcd_module_info **mi);
/**
 * Deletes capabilities to pages that contain module (passed as doubly-linked
 * list of lcd_module_page's). (If the caller is a klcd, this will never
 * lead to freeing the module pages.)
 *
 * Tells module loader we are done with the module. (If the caller is a klcd,
 * this is a call to the regular module loading code to delete the module.)
 */
void lcd_unload_module(struct lcd_module_info *mi, cptr_t mloader_endpoint);
/**
 * Big routine to automatically create an lcd, load a module inside it,
 * and configure it per the address space layout below. Uses lcd_load_module
 * to do so (so a klcd can pass a null cptr for the module loader endpoint),
 * and lcd_config to set up program counter, stack pointer, etc.
 *
 * Puts the new lcd in slot.
 *
 * Returns an lcd_module_info that contains the module's name and the list of 
 * lcd_module_pages. Call lcd_destroy_module_lcd to tear it down. So long as 
 * you don't pass the capability to any other lcd, etc., this will stop and 
 * destroy the lcd.
 *
 * Guest Physical Memory Layout
 * ============================
 *
 * No gdt/tss/idt for now (easier). See Documentation/lcd-domains/vmx.txt.
 *
 * From bottom to top,
 *
 *   -- The bottom 1 MB is unmapped / reserved in case the module is expecting 
 *      the standard physical memory layout of a PC. (Of course, it or its 
 *      creator would need to map something there to emulate that memory.)
 *
 *   -- Guest virtual page tables come next, 4 MBs. This puts a (big) upper 
 *      limit on the size of the module that can be mapped. The page tables
 *      in the hierarchy are allocated on demand as the module is mapped.
 *
 *   -- The stack/UTCB used by the initial thread when the lcd boots. (The
 *      microkernel manages this page.)
 *
 *   -- The module itself.
 *
 *   -- A huge chunk of free/unmapped guest physical memory available to the
 *      module.
 *
 *   -- The upper part is unusable (see Intel SDM V3 28.2.2). The last
 *      usable byte is at 0x0000 FFFF FFFF FFFF.
 *
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                   |         Unusable          |
 *                   +---------------------------+ 0x0000 FFFF FFFF FFFF
 *                   |                           |
 *                   :           Free            :
 *                   |                           |
 *                   +---------------------------+ (variable)
 *                   |                           |
 *                   :          Module           :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 1000
 *                   |        Stack/UTCB         |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 *
 * Guest Virtual Memory Layout
 * ===========================
 *
 * The lower part has the same layout as the guest physical.
 *
 * The module is mapped per the guest virtual addresses in the lcd_module_page
 * list returned from the module loader, so that relinking is unnecessary.
 * 
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *  The module       |                           |
 *  gets mapped      |        Upper Part         |
 *  somewhere in     :       (mostly free)       :
 *  here  -------->  |                           |
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 1000
 *                   |        Stack/UTCB         |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 *
 * Initial CSPACE
 * ==============
 *
 * The lcd's cspace will contain capabilities to the module pages and
 * guest virtual paging pages, with the root of the guest virtual paging
 * hierarchy in cnode @ cptr = 1 (all others in no systematic order).
 */

#define LCD_GV_PAGING_MEM_GPA __gpa(1 << 20)
#define LCD_GV_PAGING_MEM_SIZE (4 << 20)
#define LCD_STACK_GPA gpa_add(LCD_GV_PAGING_MEM_GPA, LCD_GV_PAGING_MEM_SIZE)
#define LCD_STACK_SIZE (4 << 10)
#define LCD_MODULE_GPA gpa_add(LCD_STACK_GPA, LCD_STACK_SIZE)
#define LCD_GV_PAGING_MEM_GVA __gva(gpa_val(LCD_GV_PAGING_MEM_GPA))
#define LCD_STACK_GVA __gva(gpa_val(LCD_STACK_GPA))


int lcd_create_module_lcd(cptr_t *slot_out, char *mname, 
			cptr_t mloader_endpoint, 
			struct lcd_module_info **mi);

void lcd_destroy_module_lcd(cptr_t lcd, struct lcd_module_info *mi,
			cptr_t mloader_endpoint);


#endif  /* _ASM_X86_LCD_DOMAINS_LIBLCD_H */
