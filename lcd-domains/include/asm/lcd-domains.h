#ifndef _ASM_X86_LCD_DOMAINS_LCD_DOMAINS_H
#define _ASM_X86_LCD_DOMAINS_LCD_DOMAINS_H

#include <asm/vmx.h>
#include <linux/spinlock.h>
#include <linux/bitmap.h>
#include <lcd-domains/types.h>
#include <linux/mutex.h>

extern int lcd_on_cpu;
extern int lcd_in_non_root;

/* DEBUGGING -------------------------------------------------- */

#define LCD_ARCH_DEBUG 0

#define LCD_ARCH_ERR(msg...) __lcd_arch_err(__FILE__, __LINE__, msg)
static inline void __lcd_arch_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_ARCH_MSG(msg...) __lcd_arch_msg(__FILE__, __LINE__, msg)
static inline void __lcd_arch_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_ARCH_WARN(msg...) __lcd_arch_warn(__FILE__, __LINE__, msg)
static inline void __lcd_arch_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}


/* LCD ARCH DATA STRUCTURES ---------------------------------------- */

struct lcd_arch_vmcs {
	u32 revision_id;
	u32 abort;
	char data[0];
};

#define LCD_ARCH_NUM_AUTOLOAD_MSRS 1

enum lcd_arch_reg {
	LCD_ARCH_REGS_RAX = 0,
	LCD_ARCH_REGS_RCX = 1,
	LCD_ARCH_REGS_RDX = 2,
	LCD_ARCH_REGS_RBX = 3,
	LCD_ARCH_REGS_RSP = 4,
	LCD_ARCH_REGS_RBP = 5,
	LCD_ARCH_REGS_RSI = 6,
	LCD_ARCH_REGS_RDI = 7,
	LCD_ARCH_REGS_R8 = 8,
	LCD_ARCH_REGS_R9 = 9,
	LCD_ARCH_REGS_R10 = 10,
	LCD_ARCH_REGS_R11 = 11,
	LCD_ARCH_REGS_R12 = 12,
	LCD_ARCH_REGS_R13 = 13,
	LCD_ARCH_REGS_R14 = 14,
	LCD_ARCH_REGS_R15 = 15,
	LCD_ARCH_REGS_RIP,
	LCD_ARCH_NUM_REGS
};

#define LCD_ARCH_EPT_WALK_LENGTH 4
#define LCD_ARCH_EPTP_WALK_SHIFT 3
#define LCD_ARCH_PTRS_PER_EPTE   (1 << 9) /* 512 table entries */

typedef unsigned long lcd_arch_epte_t; /* table entry */

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

/*
 * Guest Physical Memory Layout & Segment Registers
 * ================================================
 *
 * No gdt/tss/idt for now (easier), but perhaps in the future (see
 * Documentation/lcd-domains/vmx.txt). We can get away with this since
 * we set all of the hidden fields in the segment registers - like %fs, %gs,
 * etc.
 *
 * See Intel SDM V3 26.3.1.2, 26.3.1.3 for register requirements
 * See Intel SDM V3 3.4.2, 3.4.3 for segment register layout
 * See Intel SDM V3 2.4.1 - 2.4.4 for gdtr, ldtr, idtr, tr
 *
 *                         +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                         |                           |
 *                         :                           :
 *                         :      Free / Unmapped      :
 *                         :                           :
 *                         |                           |
 * LCD_ARCH_TOP,---------> +---------------------------+ 0x0000 0000 0000 1000
 * LCD_ARCH_BOTTOM         |         Reserved          |
 *                         | (not mapped, catch NULLs) | (4 KBs)
 *                         +---------------------------+ 0x0000 0000 0000 0000
 */
#define LCD_ARCH_BOTTOM (1 << 12)
#define LCD_ARCH_TOP LCD_ARCH_BOTTOM

#define LCD_ARCH_FS_BASE     __gpa(0UL)
#define LCD_ARCH_FS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GS_BASE     __gpa(0UL)
#define LCD_ARCH_GS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GDTR_BASE   __gpa(0UL)
#define LCD_ARCH_GDTR_LIMIT  0x0 /* no gdt */
#define LCD_ARCH_TSS_BASE    __gpa(0UL)
#define LCD_ARCH_TSS_LIMIT   0x0 /* no tss */
#define LCD_ARCH_IDTR_BASE   __gpa(0UL)
#define LCD_ARCH_IDTR_LIMIT  0x0 /* no idt right now */

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
	u64 regs[LCD_ARCH_NUM_REGS];
	u64 cr2;

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
};

/**
 * Initializes the arch-dependent code for LCD (detects required
 * features, turns on VMX on *all* cpu's).
 */
int lcd_arch_init(void);
/**
 * Turns off VMX on *all* cpu's and tears down arch-dependent code.
 * 
 * Important: All LCDs should be destroyed before calling this
 * routine (otherwise, memory will leak).
 */
void lcd_arch_exit(void);
/**
 * Creates the arch-dependent part of an LCD (e.g., the ept).
 */
int lcd_arch_create(struct lcd_arch **out);
/**
 * Tears down arch-dep part of LCD.
 *
 * IMPORTANT: When the ept is torn down, any host memory that is still mapped
 * will be freed. This is for convenience. But beware.
 */
void lcd_arch_destroy(struct lcd_arch *lcd_arch);
/**
 * Does logical consistency checks (e.g., runs through checks
 * listed in Intel SDM V3 26.1, 26.2, and 26.3).
 */
int lcd_arch_check(struct lcd_arch *lcd_arch);
/**
 * Runs lcd_arch on the calling cpu. (If the LCD is active on
 * a different cpu, it will become inactive there.) Kernel
 * preemption is disabled while t is launched, but
 * external interrupts are not disabled and will be handled.
 *
 * Unless the caller does otherwise, kernel preemption is
 * enabled before returning.
 *
 * Returns status code (e.g., LCD_ARCH_STATUS_PAGE_FAULT)
 * so that caller knows why lcd_arch exited and can respond.
 */
int lcd_arch_run(struct lcd_arch *lcd_arch);

/**
 * Status codes for running LCDs.
 */
enum lcd_arch_status {
	LCD_ARCH_STATUS_PAGE_FAULT = 0,
	LCD_ARCH_STATUS_EXT_INTR   = 1,
	LCD_ARCH_STATUS_EPT_FAULT  = 2,
	LCD_ARCH_STATUS_CR3_ACCESS = 3,
	LCD_ARCH_STATUS_SYSCALL    = 4,
};

/**
 * Lookup ept entry for guest physical address a.
 *
 * Set create = 1 to allocate ept page table data structures
 * along the path as needed.
 */
int lcd_arch_ept_walk(struct lcd_arch *lcd, gpa_t a, int create,
		lcd_arch_epte_t **epte_out);
/**
 * Set the guest physical => host physical mapping in the ept entry.
 */
void lcd_arch_ept_set(lcd_arch_epte_t *epte, hpa_t a);
/**
 * Read the host physical address stored in epte.
 */
hpa_t lcd_arch_ept_hpa(lcd_arch_epte_t *epte);
/**
 * Clears guest physical => host physical mapping in the ept.
 *
 * (This is not going to free up potentially empty paging structures
 * higher up in the ept hierarchy; but, for now, unset is used when
 * we're tearing down the lcd -- due to an error -- so the paging
 * structures will be freed up when the ept is torn down.)
 */
int lcd_arch_ept_unset(lcd_arch_epte_t *epte);
/**
 * Simple routine combining ept walk and set.
 *
 * overwrite = 0  => do not overwrite if ept entry is already present
 * overwrite = 1  => overwrite any existing ept entry
 */
int lcd_arch_ept_map(struct lcd_arch *lcd, gpa_t ga, hpa_t ha,
		int create, int overwrite);
/**
 * Maps 
 *
 *    ga_start --> ga_start + npages * PAGE_SIZE
 *
 * to
 *
 *    ha_start --> ha_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_arch_ept_map_range(struct lcd_arch *lcd, gpa_t ga_start, 
			hpa_t ha_start, unsigned long npages);
/**
 * Simple routine combining ept walk and unset.
 */
int lcd_arch_ept_unmap(struct lcd_arch *lcd, gpa_t a);
/**
 * Like unmpa, but returns old hpa in hpa_out if it is non-NULL.
 */
int lcd_arch_ept_unmap2(struct lcd_arch *lcd, gpa_t a, hpa_t *hpa_out);
/**
 * Unmaps 
 *
 *    ga_start --> ga_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_arch_ept_unmap_range(struct lcd_arch *lcd, gpa_t ga_start, 
			unsigned long npages);
/**
 * Simple routine combinding ept walk and get.
 */
int lcd_arch_ept_gpa_to_hpa(struct lcd_arch *lcd, gpa_t ga, hpa_t *ha_out);
/**
 * Set the lcd's program counter to the guest virtual address
 * a.
 */
int lcd_arch_set_pc(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Set the lcd's stack pointer to the guest virtual address
 * a.
 */
int lcd_arch_set_sp(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Set the lcd's gva root pointer (for x86, %cr3) to the
 * guest physical address a.
 */
int lcd_arch_set_gva_root(struct lcd_arch *lcd_arch, gpa_t a);

/*
 * Accessor Macro for syscalls
 * ===========================
 */
static inline u64 lcd_arch_get_syscall_num(struct lcd_arch *lcd)
{
	return lcd->regs[LCD_ARCH_REGS_RAX];
}
static inline u64 lcd_arch_get_syscall_arg0(struct lcd_arch *lcd)
{
	return lcd->regs[LCD_ARCH_REGS_R8];
}
static inline u64 lcd_arch_get_syscall_arg1(struct lcd_arch *lcd)
{
	return lcd->regs[LCD_ARCH_REGS_R9];
}
static inline u64 lcd_arch_get_syscall_arg2(struct lcd_arch *lcd)
{
	return lcd->regs[LCD_ARCH_REGS_R10];
}
static inline u64 lcd_arch_get_syscall_arg3(struct lcd_arch *lcd)
{
	return lcd->regs[LCD_ARCH_REGS_R11];
}
static inline void lcd_arch_set_syscall_ret(struct lcd_arch *lcd, u64 val)
{
	lcd->regs[LCD_ARCH_REGS_RAX] = val;
}

#endif  /* _ASM_X86_LCD_DOMAINS_ARCH_H */
