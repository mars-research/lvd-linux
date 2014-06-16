#ifndef LCD_VMX_H
#define LCD_VMX_H

#include <asm/vmx.h>
#include <linux/spinlock.h>

struct lcd_vmx_vmcs_config {
	int size;
	int order;
	u32 revision_id;
	u32 pin_based_exec_controls;
	u32 primary_proc_based_exec_controls;
	u32 secondary_proc_based_exec_controls;
	u32 vmexit_controls;
	u32 vmentry_controls;
};

struct lcd_vmx_vmcs {
	u32 revision_id;
	u32 abort;
	char data[0];
};

struct lcd_vmx_capability {
	u32 ept;
	u32 vpid;
};

int lcd_vmx_autoload_msrs[] = {
	/* NONE */
};
#define LCD_VMX_NUM_AUTOLOAD_MSRS 0

enum lcd_vmx_reg {
	LCD_VMX_REGS_RAX = 0,
	LCD_VMX_REGS_RCX = 1,
	LCD_VMX_REGS_RDX = 2,
	LCD_VMX_REGS_RBX = 3,
	LCD_VMX_REGS_RSP = 4,
	LCD_VMX_REGS_RBP = 5,
	LCD_VMX_REGS_RSI = 6,
	LCD_VMX_REGS_RDI = 7,
	LCD_VMX_REGS_R8 = 8,
	LCD_VMX_REGS_R9 = 9,
	LCD_VMX_REGS_R10 = 10,
	LCD_VMX_REGS_R11 = 11,
	LCD_VMX_REGS_R12 = 12,
	LCD_VMX_REGS_R13 = 13,
	LCD_VMX_REGS_R14 = 14,
	LCD_VMX_REGS_R15 = 15,
	LCD_VMX_REGS_RIP,
	LCD_VMX_NUM_REGS
};

#define LCD_VMX_CS_SELECTOR 1
#define LCD_VMX_FS_SELECTOR 2
#define LCD_VMX_GS_SELECTOR 3

struct lcd_vmx_ept {
	spinlock_t lock;
	unsigned long root_hpa;
	unsigned long vmcs_ptr;
	bool access_dirty_enabled;
};

struct lcd_vmx {
	int cpu;
	int launched;
	int vpid;
	struct lcd_vmx_vmcs *vmcs;
	struct lcd_vmx_ept ept;
	struct desc_struct *gdt;

	u8  fail;
	u64 exit_reason;
	u64 exit_qualification;
	u32 idt_vectoring_info;
	u32 exit_intr_info;
	u32 error_code;
	u32 vec_no;
	u64 host_rsp;
	u64 regs[LCD_VMX_NUM_REGS];
	u64 cr2;
	int shutdown;
	int ret_code;

	struct msr_autoload {
#if LCD_VMX_NUM_AUTOLOAD_MSRS > 0
		struct vmx_msr_entry guest[LCD_VMX_NUM_AUTOLOAD_MSRS];
		struct vmx_msr_entry host[LCD_VMX_NUM_AUTOLOAD_MSRS];
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
int lcd_vmx_init(void);
/**
 * Turns off VMX on *all* cpu's and tears down arch-dependent code.
 * 
 * Important: All LCDs should be destroyed before calling this
 * routine (otherwise, memory will leak).
 */
void lcd_vmx_exit(void);
/**
 * Creates the arch-dependent part of an LCD, and initializes 
 * the settings and most register values.
 */
struct lcd_vmx *lcd_vmx_create(void);
/**
 * Tears down arch-dep part of LCD. (If LCD is launched on
 * some cpu, it will become inactive.)
 */
void lcd_vmx_destroy(struct lcd_vmx *vcpu);

#endif  /* LCD_VMX_H */
