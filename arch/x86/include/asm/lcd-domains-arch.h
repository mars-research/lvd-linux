#ifndef LCD_DOMAINS_ARCH_H
#define LCD_DOMAINS_ARCH_H

#include <asm/vmx.h>
#include <linux/spinlock.h>

struct lcd_arch_vmcs {
	u32 revision_id;
	u32 abort;
	char data[0];
};

int lcd_arch_autoload_msrs[] = {
	/* NONE */
};
#define LCD_ARCH_NUM_AUTOLOAD_MSRS 0

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

#define LCD_ARCH_CS_SELECTOR 1
#define LCD_ARCH_FS_SELECTOR 2
#define LCD_ARCH_GS_SELECTOR 3

struct lcd_arch_ept {
	spinlock_t lock;
	unsigned long root_hpa;
	unsigned long vmcs_ptr;
	bool access_dirty_enabled;
};

struct lcd_arch {
	/*
	 * Public Data
	 */
	struct {
		u64 gva;
		u64 gpa;
	} run_info;

	/*
	 * Private Data
	 */
	int cpu;
	int launched;
	int vpid;
	struct lcd_arch_vmcs *vmcs;
	struct lcd_arch_ept ept;
	struct desc_struct *gdt;

	u8  fail;
	u64 exit_reason;
	u64 exit_qualification;
	u32 idt_vectoring_info;
	u32 exit_intr_info;
	u32 error_code;
	u32 vec_no;
	u64 host_rsp;
	u64 regs[LCD_ARCH_NUM_REGS];
	u64 cr2;
	int shutdown;
	int ret_code;

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
 * Creates the arch-dependent part of an LCD, and initializes 
 * the settings and most register values.
 */
struct lcd_arch *lcd_arch_create(void);
/**
 * Tears down arch-dep part of LCD. (If LCD is launched on
 * some cpu, it will become inactive.)
 */
void lcd_arch_destroy(struct lcd_arch *vcpu);
/**
 * Runs the LCD on the calling cpu. (If the LCD is active on
 * a different cpu, it will become inactive there.) Kernel
 * preemption is disabled while the LCD is launched, but
 * external interrupts are not disabled and will be handled.
 *
 * Unless the caller does otherwise, kernel preemption is
 * enabled before returning.
 */
int lcd_arch_run(struct lcd_arch *vcpu);

/**
 * Status codes for running LCDs.
 */
enum lcd_arch_status {
	LCD_ARCH_STATUS_PAGE_FAULT = 0,
	LCD_ARCH_STATUS_EXT_INTR = 1,
	LCD_ARCH_STATUS_EPT_FAULT = 2,
};

#endif  /* LCD_DOMAINS_ARCH_H */
