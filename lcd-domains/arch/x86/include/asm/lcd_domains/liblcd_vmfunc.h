/*
 * liblcd.h
 *
 * Low-level arch-dependent liblcd-internal details.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_LCD_DOMAINS_LIBLCD_VMFUNC_H
#define ASM_LCD_DOMAINS_LIBLCD_VMFUNC_H

#include <libcap.h>
#include <lcd_domains/types.h>
#include <libfipc.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <liblcd/syscall.h>

/* LOW LEVEL SYSCALLS -------------------------------------------------- */

#define PREP_SYSCALL_MSG(msg, id)	do {		\
		msg->syscall_nr = id;			\
		msg->vmfunc_id = VMFUNC_LCD_SYSCALL;	\
	} while(0)

extern int noinline vmfunc_wrapper(struct fipc_message *m);

static inline void lcd_syscall_no_args(struct fipc_message *m, int id)
{
	PREP_SYSCALL_MSG(m, id);
}

static inline void lcd_syscall_one_arg(struct fipc_message *m, int id, unsigned long arg0)
{
	PREP_SYSCALL_MSG(m, id);
	fipc_set_reg0(m, arg0);
}

static inline void lcd_syscall_two_args(struct fipc_message *m, int id,
				unsigned long arg0,
				unsigned long arg1)
{
	PREP_SYSCALL_MSG(m, id);
	fipc_set_reg0(m, arg0);
	fipc_set_reg1(m, arg1);
}

static inline void lcd_syscall_three_args(struct fipc_message *m, int id,
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2)
{
	PREP_SYSCALL_MSG(m, id);
	fipc_set_reg0(m, arg0);
	fipc_set_reg1(m, arg1);
	fipc_set_reg2(m, arg2);
}

static inline void lcd_syscall_four_args(struct fipc_message *m, int id,
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2,
					unsigned long arg3)
{
	PREP_SYSCALL_MSG(m, id);
	fipc_set_reg0(m, arg0);
	fipc_set_reg1(m, arg1);
	fipc_set_reg2(m, arg2);
	fipc_set_reg3(m, arg3);
}

static inline void lcd_syscall_five_args(struct fipc_message *m, int id,
					unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2,
					unsigned long arg3,
					unsigned long arg4)
{
	PREP_SYSCALL_MSG(m, id);
	fipc_set_reg0(m, arg0);
	fipc_set_reg1(m, arg1);
	fipc_set_reg2(m, arg2);
	fipc_set_reg3(m, arg3);
	fipc_set_reg4(m, arg4);
}

static inline void lcd_syscall_cap_delete(cptr_t cptr)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_CAP_DELETE, cptr_val(cptr));
	vmfunc_wrapper(&msg);
}

static inline int lcd_syscall_cap_revoke(cptr_t cptr)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_CAP_REVOKE, cptr_val(cptr));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	struct fipc_message msg = {0};
	lcd_syscall_three_args(&msg, LCD_SYSCALL_CAP_GRANT,
				cptr_val(lcd), cptr_val(src), cptr_val(dest));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_putchar(unsigned char c)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_PUTCHAR, c);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_create(cptr_t lcd_slot)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_CREATE, cptr_val(lcd_slot));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_config_registers(cptr_t lcd, gva_t pc,
					gva_t sp, gpa_t gva_root,
					gpa_t utcb_page)
{
	struct fipc_message msg = {0};
	lcd_syscall_five_args(&msg, LCD_SYSCALL_CONFIG_REGISTERS,
				cptr_val(lcd), gva_val(pc), gva_val(sp),
				gpa_val(gva_root), gpa_val(utcb_page));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_memory_grant_and_map(cptr_t lcd,
						cptr_t mo_cptr,
						cptr_t dest_slot,
						gpa_t base)
{
	struct fipc_message msg = {0};
	lcd_syscall_four_args(&msg, LCD_SYSCALL_MEMORY_GRANT_AND_MAP,
				cptr_val(lcd), cptr_val(mo_cptr),
				cptr_val(dest_slot), gpa_val(base));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_run(cptr_t lcd)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_RUN, cptr_val(lcd));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline void __noreturn lcd_syscall_exit(int retval)
{
	struct fipc_message msg = {0};
	for(;;)
		lcd_syscall_one_arg(&msg, LCD_SYSCALL_EXIT, retval);
	vmfunc_wrapper(&msg);
}

static inline int lcd_syscall_sync_send(cptr_t endpoint)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_SYNC_SEND, cptr_val(endpoint));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_sync_recv(cptr_t endpoint)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_SYNC_RECV, cptr_val(endpoint));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_sync_poll_recv(cptr_t endpoint)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_SYNC_POLL_RECV,
				cptr_val(endpoint));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_sync_call(cptr_t endpoint)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_SYNC_CALL, cptr_val(endpoint));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_sync_reply(void)
{
	struct fipc_message msg = {0};
	lcd_syscall_no_args(&msg, LCD_SYSCALL_SYNC_REPLY);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_assign_device(int domain, int bus, int devfn)
{
	struct fipc_message msg = {0};
	lcd_syscall_three_args(&msg, LCD_SYSCALL_ASSIGN_DEVICE, domain, bus, devfn);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_deassign_device(int domain, int bus, int devfn)
{
	struct fipc_message msg = {0};
	lcd_syscall_three_args(&msg, LCD_SYSCALL_DEASSIGN_DEVICE, domain, bus, devfn);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_iommu_map_page(gpa_t gpa, unsigned int order, bool force)
{
	struct fipc_message msg = {0};
	lcd_syscall_three_args(&msg, LCD_SYSCALL_IOMMU_MAP_PAGE, gpa_val(gpa), order, force);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_iommu_unmap_page(gpa_t gpa, unsigned int order)
{
	struct fipc_message msg = {0};
	lcd_syscall_two_args(&msg, LCD_SYSCALL_IOMMU_UNMAP_PAGE, gpa_val(gpa), order);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_create_sync_ep(cptr_t slot)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_CREATE_SYNC_EP, cptr_val(slot));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_alloc_pages_exact_node(cptr_t slot, int nid,
						unsigned int flags,
						unsigned int order)
{
	struct fipc_message msg = {0};
	lcd_syscall_four_args(&msg, LCD_SYSCALL_ALLOC_PAGES_EXACT_NODE,
				cptr_val(slot), nid, flags, order);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_alloc_pages(cptr_t slot,
					unsigned int flags,
					unsigned int order)
{
	struct fipc_message msg = {0};
	lcd_syscall_three_args(&msg, LCD_SYSCALL_ALLOC_PAGES,
				cptr_val(slot), flags, order);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_free_pages(struct page *base,
					unsigned int order)
{
	struct fipc_message msg = {0};
	lcd_syscall_two_args(&msg, LCD_SYSCALL_FREE_PAGES, (unsigned long)base, order);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_vmalloc(cptr_t slot,
				unsigned long nr_pages)
{
	struct fipc_message msg = {0};
	lcd_syscall_two_args(&msg, LCD_SYSCALL_VMALLOC,
				cptr_val(slot), nr_pages);
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_mmap(cptr_t mo_cptr, gpa_t base)
{
	struct fipc_message msg = {0};
	lcd_syscall_two_args(&msg, LCD_SYSCALL_MMAP,
				cptr_val(mo_cptr), gpa_val(base));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline int lcd_syscall_munmap(cptr_t mo_cptr)
{
	struct fipc_message msg = {0};
	lcd_syscall_one_arg(&msg, LCD_SYSCALL_MUNMAP, cptr_val(mo_cptr));
	vmfunc_wrapper(&msg);
	return 0;
}

static inline void lcd_syscall_dump_stack(void)
{
	struct fipc_message msg = {0};
	lcd_syscall_no_args(&msg, LCD_SYSCALL_DUMP_STACK);
	vmfunc_wrapper(&msg);
}

static inline void lcd_syscall_irq_disable(void)
{
	struct fipc_message msg = {0};
	lcd_syscall_no_args(&msg, LCD_SYSCALL_IRQ_DISABLE);
	vmfunc_wrapper(&msg);
}

static inline void lcd_syscall_irq_enable(void)
{
	struct fipc_message msg = {0};
	lcd_syscall_no_args(&msg, LCD_SYSCALL_IRQ_ENABLE);
	vmfunc_wrapper(&msg);
}

#endif /* ASM_LCD_DOMAINS_LIBLCD_VMFUNC_H */
