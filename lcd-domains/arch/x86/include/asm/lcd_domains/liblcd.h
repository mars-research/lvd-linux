/*
 * liblcd.h
 *
 * Low-level arch-dependent liblcd-internal details.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_LCD_DOMAINS_LIBLCD_H
#define ASM_LCD_DOMAINS_LIBLCD_H

#include <libcap.h>
#include <lcd_domains/types.h>
#include <liblcd/syscall.h>

/* LOW LEVEL SYSCALLS -------------------------------------------------- */

/* Bit of redundancy. But trying not to get macro crazy. */

static inline int lcd_syscall_no_args(int id)
{
        long ret;
	asm volatile(
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id)
		: "rax");
	return (int)ret;
}

static inline int lcd_syscall_one_arg(int id, unsigned long arg0)
{
        long ret;
	asm volatile(
		"movq %2, %%r8 \n\t"
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id), "g" (arg0)
		: "rax", "r8");
	return (int)ret;
}

static inline int lcd_syscall_two_args(int id, 
				unsigned long arg0,
				unsigned long arg1)
{
        long ret;
	asm volatile(
		"movq %3, %%r9 \n\t"
		"movq %2, %%r8 \n\t"
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id), "g" (arg0), "g" (arg1)
		: "rax", "r8", "r9");
	return (int)ret;
}

static inline int lcd_syscall_three_args(int id, 
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2)
{
        long ret;
	asm volatile(
		"movq %4, %%r10 \n\t"
		"movq %3, %%r9 \n\t"
		"movq %2, %%r8 \n\t"
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id), "g" (arg0), "g" (arg1), "g" (arg2)
		: "rax", "r8", "r9", "r10");
	return (int)ret;
}

static inline int lcd_syscall_four_args(int id, 
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2,
					unsigned long arg3)
{
        long ret;
	asm volatile(
		"movq %5, %%r11 \n\t"
		"movq %4, %%r10 \n\t"
		"movq %3, %%r9 \n\t"
		"movq %2, %%r8 \n\t"
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id), "g" (arg0), "g" (arg1), "g" (arg2), "g" (arg3)
		: "rax", "r8", "r9", "r10", "r11");
	return (int)ret;
}

static inline int lcd_syscall_five_args(int id, 
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2,
					unsigned long arg3,
					unsigned long arg4)
{
        long ret;
	asm volatile(
		"movq %6, %%r12 \n\t"
		"movq %5, %%r11 \n\t"
		"movq %4, %%r10 \n\t"
		"movq %3, %%r9 \n\t"
		"movq %2, %%r8 \n\t"
		"movq %1, %%rax \n\t"
		"vmcall \n\t"
		"movq %%rax, %0 \n\t"
		: "=g" (ret)
		: "g" (id), "g" (arg0), "g" (arg1), "g" (arg2), "g" (arg3),
		  "g" (arg4)
		: "rax", "r8", "r9", "r10", "r11", "r12");
	return (int)ret;
}

static inline void lcd_syscall_cap_delete(cptr_t cptr)
{
	lcd_syscall_one_arg(LCD_SYSCALL_CAP_DELETE, cptr_val(cptr));
}

static inline int lcd_syscall_cap_revoke(cptr_t cptr)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_CAP_REVOKE, cptr_val(cptr));
}

static inline int lcd_syscall_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return lcd_syscall_three_args(LCD_SYSCALL_CAP_GRANT, 
				cptr_val(lcd), cptr_val(src), cptr_val(dest));
}

static inline int lcd_syscall_putchar(unsigned char c)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_PUTCHAR, c);
}

static inline int lcd_syscall_create(cptr_t lcd_slot)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_CREATE, cptr_val(lcd_slot));
}

static inline int lcd_syscall_config_registers(cptr_t lcd, gva_t pc,
					gva_t sp, gpa_t gva_root,
					gpa_t utcb_page)
{
	return lcd_syscall_five_args(LCD_SYSCALL_CONFIG_REGISTERS, 
				cptr_val(lcd), gva_val(pc), gva_val(sp),
				gpa_val(gva_root), gpa_val(utcb_page));
}

static inline int lcd_syscall_memory_grant_and_map(cptr_t lcd,
						cptr_t mo_cptr,
						cptr_t dest_slot,
						gpa_t base)
{
	return lcd_syscall_four_args(LCD_SYSCALL_MEMORY_GRANT_AND_MAP, 
				cptr_val(lcd), cptr_val(mo_cptr), 
				cptr_val(dest_slot), gpa_val(base));
}

static inline int lcd_syscall_run(cptr_t lcd)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_RUN, cptr_val(lcd));
}

static inline void __noreturn lcd_syscall_exit(int retval)
{
	for(;;)
		lcd_syscall_one_arg(LCD_SYSCALL_EXIT, retval);
}

static inline int lcd_syscall_sync_send(cptr_t endpoint)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_SYNC_SEND, cptr_val(endpoint));
}

static inline int lcd_syscall_sync_recv(cptr_t endpoint)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_SYNC_RECV, cptr_val(endpoint));
}

static inline int lcd_syscall_sync_call(cptr_t endpoint)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_SYNC_CALL, cptr_val(endpoint));
}

static inline int lcd_syscall_sync_reply(void)
{
	return lcd_syscall_no_args(LCD_SYSCALL_SYNC_REPLY);
}

static inline int lcd_syscall_create_sync_ep(cptr_t slot)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_CREATE_SYNC_EP, cptr_val(slot));
}

static inline int lcd_syscall_alloc_pages_exact_node(cptr_t slot, int nid,
						unsigned int flags,
						unsigned int order)
{
	return lcd_syscall_four_args(LCD_SYSCALL_ALLOC_PAGES_EXACT_NODE, 
				cptr_val(slot), nid, flags, order);
}

static inline int lcd_syscall_alloc_pages(cptr_t slot,
					unsigned int flags,
					unsigned int order)
{
	return lcd_syscall_three_args(LCD_SYSCALL_ALLOC_PAGES, 
				cptr_val(slot), flags, order);
}

static inline int lcd_syscall_vmalloc(cptr_t slot,
				unsigned long nr_pages)
{
	return lcd_syscall_two_args(LCD_SYSCALL_VMALLOC, 
				cptr_val(slot), nr_pages);
}

static inline int lcd_syscall_mmap(cptr_t mo_cptr, gpa_t base)
{
	return lcd_syscall_two_args(LCD_SYSCALL_MMAP, 
				cptr_val(mo_cptr), gpa_val(base));
}

static inline int lcd_syscall_munmap(cptr_t mo_cptr)
{
	return lcd_syscall_one_arg(LCD_SYSCALL_MUNMAP, cptr_val(mo_cptr));
}


#endif /* ASM_LCD_DOMAINS_LIBLCD_H */
