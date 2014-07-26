#ifndef _ASM_X86_LCD_DOMAINS_ARCH_H
#define _ASM_X86_LCD_DOMAINS_ARCH_H

#include <asm/vmx.h>
#include <linux/spinlock.h>
#include <lcd-domains/utcb.h>

/* ADDRESS SPACE TYPES ---------------------------------------- */

/* XXX: Assuming host and guest run in 64-bit mode */

typedef struct { unsigned long gva; } gva_t;
typedef struct { unsigned long hva; } hva_t;
typedef struct { unsigned long gpa; } gpa_t;
typedef struct { unsigned long hpa; } hpa_t;

static inline gva_t __gva(unsigned long gva)
{
	return (gva_t){ gva };
}
static inline unsigned long gva_val(gva_t gva)
{
	return gva.gva;
}
static inline unsigned long * gva_ptr(gva_t * gva)
{
	return &(gva->gva);
}
static inline gva_t gva_add(gva_t gva, unsigned long off)
{
	return __gva(gva_val(gva) + off);
}
static inline hva_t __hva(unsigned long hva)
{
	return (hva_t){ hva };
}
static inline unsigned long hva_val(hva_t hva)
{
	return hva.hva;
}
static inline unsigned long * hva_ptr(hva_t * hva)
{
	return &(hva->hva);
}
static inline hva_t hva_add(hva_t hva, unsigned long off)
{
	return __hva(hva_val(hva) + off);
}
static inline gpa_t __gpa(unsigned long gpa)
{
	return (gpa_t){ gpa };
}
static inline unsigned long gpa_val(gpa_t gpa)
{
	return gpa.gpa;
}
static inline unsigned long * gpa_ptr(gpa_t * gpa)
{
	return &(gpa->gpa);
}
static inline gpa_t gpa_add(gpa_t gpa, unsigned long off)
{
	return __gpa(gpa_val(gpa) + off);
}
static inline hpa_t __hpa(unsigned long hpa)
{
	return (hpa_t){ hpa };
}
static inline unsigned long hpa_val(hpa_t hpa)
{
	return hpa.hpa;
}
static inline unsigned long * hpa_ptr(hpa_t * hpa)
{
	return &(hpa->hpa);
}
static inline hpa_t hpa_add(hpa_t hpa, unsigned long off)
{
	return __hpa(hpa_val(hpa) + off);
}
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
#define LCD_ARCH_PTRS_PER_EPTE   (1 << 9)

typedef unsigned long lcd_arch_epte_t;

struct lcd_arch_ept {
	spinlock_t lock;
	lcd_arch_epte_t *root;
	u64 vmcs_ptr;
	bool access_dirty_enabled;
};

struct lcd_arch_tss {
	/*
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

struct lcd_arch {
	/*
	 * Public Data
	 */
	struct {
		gva_t gv_fault_addr;
		gpa_t gp_fault_addr;
	} run_info;

	/*
	 * Private Data
	 */
	int cpu;
	int launched;
	int vpid;
	struct lcd_arch_vmcs *vmcs;

	struct lcd_arch_ept ept;
	struct desc_struct  *gdt;
	struct lcd_arch_tss *tss;
	struct lcd_utcb *utcb;

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
 * Does logical consistency checks (e.g., runs through checks
 * listed in Intel SDM V3 26.1, 26.2, and 26.3).
 */
int lcd_arch_check(struct lcd_arch *vcpu);
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
 *
 * Returns status code (e.g., LCD_ARCH_STATUS_PAGE_FAULT)
 * so that caller knows why lcd exited and can respond.
 */
int lcd_arch_run(struct lcd_arch *vcpu);

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
int lcd_arch_ept_walk(struct lcd_arch *vcpu, gpa_t a, int create,
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
int lcd_arch_ept_map(struct lcd_arch *vcpu, gpa_t ga, hpa_t ha,
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
int lcd_arch_ept_unmap(struct lcd_arch *vcpu, gpa_t a);
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
int lcd_arch_ept_gpa_to_hpa(struct lcd_arch *vcpu, gpa_t ga, hpa_t *ha_out);
/**
 * Set the lcd's program counter to the guest virtual address
 * a.
 */
int lcd_arch_set_pc(struct lcd_arch *vcpu, gva_t a);
/**
 * Set the lcd's gva root pointer (for x86, %cr3) to the
 * guest physical address a.
 */
int lcd_arch_set_gva_root(struct lcd_arch *vcpu, gpa_t a);

/*
 * GDT Layout
 * ==========
 * 0 = NULL
 * 1 = Code segment
 * 2 = Data segment  (%fs, default not present)
 * 3 = Data segment  (%gs, default not present)
 * 4 = Task segment
 *
 * See Intel SDM V3 26.3.1.2, 26.3.1.3 for register requirements.
 * See Intel SDM V3 3.4.2, 3.4.3 for segment register layout
 * See Intel SDM V3 2.4.1 - 2.4.4 for gdtr, ldtr, idtr, tr
 */
#define LCD_ARCH_FS_BASE     __gpa(0UL)
#define LCD_ARCH_FS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GS_BASE     __gpa(0UL)
#define LCD_ARCH_GS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GDTR_BASE   __gpa(1UL << PAGE_SHIFT)
#define LCD_ARCH_GDTR_LIMIT  0xFFFUL
#define LCD_ARCH_TSS_BASE    __gpa(2UL << PAGE_SHIFT)
/* tss base + limit = address of last byte in tss, hence -1 */
#define LCD_ARCH_TSS_LIMIT   (sizeof(struct lcd_arch_tss) - 1)
#define LCD_ARCH_IDTR_BASE   __gpa(0UL)
#define LCD_ARCH_IDTR_LIMIT  0x0 /* no idt right now */

#define LCD_ARCH_CS_SELECTOR   (1 << 3)
#define LCD_ARCH_FS_SELECTOR   (2 << 3)
#define LCD_ARCH_GS_SELECTOR   (3 << 3)
#define LCD_ARCH_TR_SELECTOR   (4 << 3) /* TI must be 0 */
#define LCD_ARCH_LDTR_SELECTOR (0 << 3) /* unusable */

/*
 * Guest Physical Memory Layout
 * ============================
 *
 *                         +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                         |                           |
 *                         :                           :
 *                         :      Free / Unmapped      :
 *                         :                           :
 *                         |                           |
 * LCD_ARCH_STACK_TOP,---> +---------------------------+ 0x0000 0000 0000 4000
 * LCD_ARCH_FREE           |                           |
 *                         |          Stack            |
 *                         :       (grows down)        : (4 KBs)
 *                         :                           :
 *                         |                           |
 *                         |   User Thread Ctrl Block  |
 * LCD_ARCH_UTCB---------> +---------------------------+ 0x0000 0000 0000 3000
 *                         |           TSS             |
 *                         |    only sizeof(tss) is    | (4 KBs)
 *                         |           used            |
 * LCD_ARCH_TSS_BASE-----> +---------------------------+ 0x0000 0000 0000 2000
 *                         |           GDT             | (4 KBs)
 * LCD_ARCH_GDTR_BASE----> +---------------------------+ 0x0000 0000 0000 1000
 *                         |         Reserved          |
 *                         |       (not mapped)        | (4 KBs)
 *                         +---------------------------+ 0x0000 0000 0000 0000
 */
#define LCD_ARCH_UTCB        __gpa(3UL << PAGE_SHIFT)
#define LCD_ARCH_STACK_TOP   __gpa(4UL << PAGE_SHIFT)
#define LCD_ARCH_FREE        LCD_ARCH_STACK_TOP

/*
 * Accessor Macro for syscalls
 * ===========================
 */
#define LCD_ARCH_GET_SYSCALL_NUM(vcpu) (vcpu->regs[LCD_ARCH_REGS_RAX])

/*
 * Accessor Macro for utcb
 * =======================
 */
#define LCD_ARCH_GET_UTCB(vcpu) (vcpu->utcb);

/*
 * Accessor Macros for IPC
 * =======================
 *
 * Based on x86 seL4 message register design.
 *
 * See seL4 manual, 4.1.
 */
#define LCD_ARCH_GET_CAP_REG(vcpu) (vcpu->regs[LCD_ARCH_REGS_RBX])
#define LCD_ARCH_GET_BDG_REG(vcpu) (vcpu->regs[LCD_ARCH_REGS_RBX])
#define LCD_ARCH_GET_TAG_REG(vcpu) (vcpu->regs[LCD_ARCH_REGS_RSI])
#define LCD_ARCH_GET_MSG_REG(vcpu, idx) (__lcd_arch_get_msg_reg(vcpu, idx))
static inline u64 __lcd_arch_get_msg_reg(struct lcd_arch *vcpu, 
					unsigned int idx)
{
	/*
	 * Message regs 0 and 1 are fast (use machine registers)
	 *
	 * Message regs 2, ... always use the mr's in struct lcd_ipc_regs.
	 *
	 * (The first two mr's in utcb are reserved for
	 * mr's 0 and 1. If the caller wishes to explicitly use those mr's,
	 * they should do so by manually accessing the mr's in utcb.)
	 */
	if (idx == 0)
		return vcpu->regs[LCD_ARCH_REGS_RDI];
	else if (idx == 1)
		return vcpu->regs[LCD_ARCH_REGS_RBP];
	else
		return vcpu->utcb->ipc.mr[idx];
}

#define LCD_ARCH_SET_CAP_REG(vcpu, val) ({                    \
			vcpu->regs[LCD_ARCH_REGS_RBX] = val;  \
		})
#define LCD_ARCH_SET_BDG_REG(vcpu, val) ({                    \
			vcpu->regs[LCD_ARCH_REGS_RBX] = val;  \
		})
#define LCD_ARCH_SET_TAG_REG(vcpu, val) ({                    \
			vcpu->regs[LCD_ARCH_REGS_RSI] = val;  \
		})
#define LCD_ARCH_SET_MSG_REG(vcpu, idx, val) ({                 \
			__lcd_arch_set_msg_reg(vcpu, val, idx);	\
		})
static inline void __lcd_arch_set_msg_reg(struct lcd_arch *vcpu, 
					unsigned int idx, u64 val)
{
	/*
	 * Message regs 0 and 1 are fast (use machine registers)
	 *
	 * Message regs 2, ... always use the mr's in struct lcd_ipc_regs.
	 *
	 * (The first two mr's in utcb are reserved for
	 * mr's 0 and 1. If the caller wishes to explicitly use those mr's,
	 * they should do so by manually accessing the mr's in utcb.)
	 */
	if (idx == 0)
		vcpu->regs[LCD_ARCH_REGS_RDI] = val;
	else if (idx == 1)
		vcpu->regs[LCD_ARCH_REGS_RBP] = val;
	else
		vcpu->utcb->ipc.mr[idx] = val;
}

#endif  /* _ASM_X86_LCD_DOMAINS_ARCH_H */
