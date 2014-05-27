#ifndef LCD_LCD_H
#define LCD_LCD_H

#include <asm/vmx.h>

#include <lcd/ipc.h>

/* XXX: arch dependent, move to ./arch/x86/lcd */ 
#define NR_AUTOLOAD_MSRS 8

/* XXX: arch dependent, move to ./arch/x86/lcd */ 
enum vmx_reg {
	VCPU_REGS_RAX = 0,
	VCPU_REGS_RCX = 1,
	VCPU_REGS_RDX = 2,
	VCPU_REGS_RBX = 3,
	VCPU_REGS_RSP = 4,
	VCPU_REGS_RBP = 5,
	VCPU_REGS_RSI = 6,
	VCPU_REGS_RDI = 7,
	VCPU_REGS_R8 = 8,
	VCPU_REGS_R9 = 9,
	VCPU_REGS_R10 = 10,
	VCPU_REGS_R11 = 11,
	VCPU_REGS_R12 = 12,
	VCPU_REGS_R13 = 13,
	VCPU_REGS_R14 = 14,
	VCPU_REGS_R15 = 15,
	VCPU_REGS_RIP,
	NR_VCPU_REGS
};

/* XXX: split and move arch dependent parts to ./arch/x86/lcd */ 
typedef struct lcd {
	int cpu;
	int vpid;
	int launched;

	spinlock_t ept_lock;
	unsigned long ept_root;
	unsigned long eptp;
	bool ept_ad_enabled;

	pgd_t* pt;
	unsigned long pt_gpa;

	unsigned long *bmp_pt_pages;

	/* GDT_ENTRIES * desc_struct */
	struct desc_struct* gdt;
	/* IDT_ENTRIES * gate_desc */
	gate_desc* idt;
	struct lcd_tss_struct* tss;

	unsigned long isr_page;

	unsigned long host_idt_base;

	u8  fail;
	u64 exit_reason;
	u64 exit_qualification;
	u32 idt_vectoring_info;
	u32 exit_intr_info;
	u32 error_code;
	u32 vec_no;
	u64 host_rsp;
	u64 regs[NR_VCPU_REGS];
	u64 cr2;

	int shutdown;
	int ret_code;

	struct msr_autoload {
		unsigned nr;
		struct vmx_msr_entry guest[NR_AUTOLOAD_MSRS];
		struct vmx_msr_entry host[NR_AUTOLOAD_MSRS];
	} msr_autoload;

	sync_ipc_t sync_ipc;
	struct vmcs *vmcs;
	void *shared;

	struct boot_params *bp;
	struct start_info *si;
  
	struct module *mod;
} lcd_t;

int lcd_vmx_init(void); 
void lcd_vmx_exit(void);


int lcd_init_module(void __user * umod, unsigned long len, const char __user * uargs);
//int lcd_load_module(struct load_info *info, const char __user *uargs, int flags);

int lcd_load_vmlinux(const char* kfile, struct lcd *lcd, u64 *elf_entry);

/* Exported functions */
struct lcd* lcd_create(void);
int lcd_destroy(struct lcd *lcd);

int lcd_move_module(struct lcd *lcd, struct module *mod);

int lcd_map_gpa_to_hpa(struct lcd *lcd, u64 gpa, u64 hpa, int overwrite);
int lcd_map_gva_to_gpa(struct lcd *lcd, u64 gva, u64 gpa, int create, int overwrite);
int lcd_find_hva_by_gpa(struct lcd *lcd, u64 gpa, u64 *hva);

int lcd_run(struct lcd *lcd);
const char* lcd_exit_reason(int exit_code);

#endif /* LCD_LCD_H */
