/*
 * types.h
 *
 * x86 VT-x types used throughout.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_TYPES_H
#define ASM_X86_LCD_DOMAINS_TYPES_H

#include <asm/vmx.h>
#include <linux/mutex.h>
#include <linux/module.h>

struct lcd_arch_vmcs {
	u32 revision_id;
	u32 abort;
	char data[0];
};

/* We can get fancy with unions so you can access lower bits and such,
 * but until we need that, lets just stick with a vanilla 64-bit value. */
#define LCD_DECL_REG(name) \
	u64 r ## name

#define LCD_DECL_64_REG(name) \
	u64 r ## name

struct lcd_arch_regs {

	LCD_DECL_REG(ax);
	LCD_DECL_REG(bx);
	LCD_DECL_REG(cx);
	LCD_DECL_REG(dx);
	LCD_DECL_REG(si);
	LCD_DECL_REG(di);
	LCD_DECL_REG(ip);
	LCD_DECL_REG(bp);
	LCD_DECL_REG(sp);
	LCD_DECL_REG(flags);
	LCD_DECL_64_REG(8);
	LCD_DECL_64_REG(9);
	LCD_DECL_64_REG(10);
	LCD_DECL_64_REG(11);
	LCD_DECL_64_REG(12);
	LCD_DECL_64_REG(13);
	LCD_DECL_64_REG(14);
	LCD_DECL_64_REG(15);

	u16 cs;
	u16 ss;
	u16 es;
	u16 ds;
	u16 fs;
	u16 gs;

	u64 cr0;
	u64 cr2;
	u64 cr3;
	u64 cr4;
	u64 cr8;
};

#define LCD_ARCH_NUM_AUTOLOAD_MSRS 1

/* This struct is not used right now. */
struct lcd_arch_tss {
	/*
	 * ** Currently not used. Keeping it here in case we add tss's back
	 * in later. See Documentation/lcd-domains/vmx.txt. **
	 *
	 * Intel SDM V3 7.7
	 *
	 * Base TSS before I/O bitmap, etc.
	 */
	struct x86_hw_tss base_tss;
	/*
	 * I/O bitmap must be at least 8 bits to contain
	 * required 8 bits that are set.
	 *
	 * Intel SDM V1 16.5.2
	 */
	u8 io_bitmap[1];
} __attribute__((packed));

#define LCD_ARCH_EPT_WALK_LENGTH 4
#define LCD_ARCH_EPTP_WALK_SHIFT 3
#define LCD_ARCH_PTRS_PER_EPTE   (1 << 9) /* 512 table entries */

typedef unsigned long lcd_arch_epte_t; /* table entry */

struct lcd_arch {
	/*
	 * CPU we're running on / vmloaded on
	 */
	int cpu;
	/*
	 * = 1 if we've launched (need to use vm resume instead of launch
	 * if we have already launched; intel makes us track this)
	 */
	int launched;
	/*
	 * VPID of the vm, used by the tlb for vpid-specific invalidations
	 */
	int vpid;
	/*
	 * vmcs data structure; *must* be accessed using vmread / vmwrite
	 */
	struct lcd_arch_vmcs *vmcs;
	/*
	 * Guest physical address space
	 */
	struct {
		struct mutex lock;
		lcd_arch_epte_t *root;
		u64 vmcs_ptr; /* to be loaded in vmcs EPT_POINTER field */
		bool access_dirty_enabled;
	} ept;

	/*
	 * Exit info
	 */
	u8  fail;
	u64 exit_reason;
	u64 exit_qualification;
	u32 idt_vectoring_info;
	u32 exit_intr_info;
	u32 error_code;
	u32 vec_no;

	/*
	 * Stuff we need to save explicitly
	 */
	u64 host_rsp;
	struct lcd_arch_regs regs;

	int shutdown;
	int ret_code;

	/*
	 * MSR autload area (used during vm enters and exits). Fields must
	 * be word aligned!
	 */
	struct msr_autoload {
#if LCD_ARCH_NUM_AUTOLOAD_MSRS > 0
		struct vmx_msr_entry guest[LCD_ARCH_NUM_AUTOLOAD_MSRS];
		struct vmx_msr_entry host[LCD_ARCH_NUM_AUTOLOAD_MSRS];
#else
		struct vmx_msr_entry *guest;
		struct vmx_msr_entry *host;
#endif
	} msr_autoload;

	/*
	 * Pointer to struct module inside LCD container. We need this
	 * for resolving addresses to symbol names for stack traces.
	 */
	struct module *kernel_module;
};

struct lcd_vmx_capability {
	u32 ept;
	u32 vpid;
};

struct lcd_vmcs_config {
	int size;
	int order;
	u32 revision_id;
	u32 pin_based_exec_controls;
	u32 primary_proc_based_exec_controls;
	u32 secondary_proc_based_exec_controls;
	u32 vmexit_controls;
	u32 vmentry_controls;
};

struct lcd_vpids {
	DECLARE_BITMAP(bitmap, VMX_NR_VPIDS);
	spinlock_t lock;
};

#endif /* ASM_X86_LCD_DOMAINS_TYPES_H */
