#ifndef LCD_LCD_DEFS_H
#define LCD_LCD_DEFS_H

#include <linux/bitmap.h>

#if !defined(VMX_EPT_AD_BIT)
#define VMX_EPT_AD_BIT          (1ull << 21)
#define VMX_EPT_AD_ENABLE_BIT   (1ull << 6)
#endif

#ifndef VMX_EPT_EXTENT_INDIVIDUAL_BIT
#define VMX_EPT_EXTENT_INDIVIDUAL_BIT           (1ull << 24)
#endif

#ifndef X86_CR4_PCIDE
#define X86_CR4_PCIDE 0x00020000 /* enable PCID support */
#endif

#ifndef SECONDARY_EXEC_ENABLE_INVPCID
#define SECONDARY_EXEC_ENABLE_INVPCID 0x00001000
#endif

#ifndef SEG_TYPE_DATA
#define SEG_TYPE_DATA		(0 << 3)
#endif

#ifndef SEG_TYPE_READ_WRITE
#define SEG_TYPE_READ_WRITE	(1 << 1)
#endif

#ifndef SEG_TYPE_CODE
#define SEG_TYPE_CODE		(1 << 3)
#endif

#ifndef SEG_TYPE_EXEC_READ
#define SEG_TYPE_EXEC_READ	(1 << 1)
#endif

#ifndef SEG_TYPE_TSS
#define SEG_TYPE_TSS		((1 << 3) | (1 << 0))
#endif

#ifndef SEG_OP_SIZE_32BIT
#define SEG_OP_SIZE_32BIT	(1 << 0)
#endif

#ifndef SEG_GRANULARITY_4KB
#define SEG_GRANULARITY_4KB	(1 << 0)
#endif

#ifndef DESC_TYPE_CODE_DATA
#define DESC_TYPE_CODE_DATA	(1 << 0)
#endif

/* Memory management */

#define EPT_LEVELS 4

#define VMX_EPT_FAULT_READ  0x01

typedef unsigned long epte_t;

#define __EPTE_READ    0x01
#define __EPTE_WRITE   0x02
#define __EPTE_EXEC    0x04
#define __EPTE_IPAT    0x40
#define __EPTE_SZ      0x80
#define __EPTE_A       0x100
#define __EPTE_D       0x200
#define __EPTE_TYPE(n) (((n) & 0x7) << 3)

enum {
  EPTE_TYPE_UC = 0, /* uncachable */
  EPTE_TYPE_WC = 1, /* write combining */
  EPTE_TYPE_WT = 4, /* write through */
  EPTE_TYPE_WP = 5, /* write protected */
  EPTE_TYPE_WB = 6, /* write back */
};

#define __EPTE_NONE 0
#define __EPTE_FULL (__EPTE_READ | __EPTE_WRITE | __EPTE_EXEC)

#define EPTE_ADDR  (~(PAGE_SIZE - 1))
#define EPTE_FLAGS (PAGE_SIZE - 1)

#define ADDR_TO_IDX(la, n)                                      \
  ((((unsigned long) (la)) >> (12 + 9 * (n))) & ((1 << 9) - 1))


/* VMCS related */

struct vmcs_config {
  int size;
  int order;
  u32 revision_id;
  u32 pin_based_exec_ctrl;
  u32 cpu_based_exec_ctrl;
  u32 cpu_based_2nd_exec_ctrl;
  u32 vmexit_ctrl;
  u32 vmentry_ctrl;
};

struct vmcs {
  u32 revision_id;
  u32 abort;
  char data[0];
};

struct vmx_capability {
  u32 ept;
  u32 vpid;
  int has_load_efer:1;
};

extern struct vmx_capability vmx_capability;
extern struct vmcs_config vmcs_config;

#define NR_AUTOLOAD_MSRS 8

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

struct lcd_tss_struct {
  struct x86_hw_tss tss;
  u8 io_bitmap[1];
} __attribute__((packed));

typedef struct {
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

  struct vmcs *vmcs;
  
  struct module *mod;
} lcd_struct;


/* Memory layout */
// Range format: [begin, end)
// 0x0000 0000 0000 0000 ~ 0x0000 0000 4000 0000 : 1GB  : Physical Mem
// 4K gap
// 0x0000 0000 4000 1000 ~ 0x0000 0000 4040 1000 : 4MB : Page table structures
// 0x0000 0000 4040 1000 ~ 0x0000 0000 4040 2000 : 4KB  : GDT
// 0x0000 0000 4040 2000 ~ 0x0000 0000 4040 3000 : 4KB  : IDT
// 0x0000 0000 4040 3000 ~ 0x0000 0000 4040 4000 : 4KB  : TSS page (sizeof(lcd_tss_struct))
// 4K page gap as memory guard
// 0x0000 0000 4040 5000 ~ 0x0000 0000 4040 6000 : 4KB  : Common ISR code page
// 4K memory guard
// 0x0000 0000 4040 7000 ~ 0x0000 0000 4040 F000 : 32KB : stack
// 4K memory guard
// 0x0000 0000 4041 0000 ~ 0x0000 0000 4051 0000 : 1MB  : 256 ISRs, 4KB code page per ISR



// Bootup structure:
#define LCD_PHY_MEM_SIZE (1 << 30)  /* 1GB physical mem */

#define LCD_NR_PT_PAGES    (1 << 10)       /* #pages for page table */
#define LCD_PT_PAGES_START (LCD_PHY_MEM_SIZE + PAGE_SIZE) /* 1GB + 4KB */
#define LCD_PT_PAGES_END   (LCD_PT_PAGES_START + (LCD_NR_PT_PAGES << PAGE_SHIFT))

#define LCD_GDT_ADDR (LCD_PT_PAGES_END)  /* start from 1G + 4M + 4K */
#define LCD_IDT_ADDR (LCD_GDT_ADDR + PAGE_SIZE)
#define LCD_TSS_ADDR (LCD_IDT_ADDR + PAGE_SIZE)
#define LCD_TSS_SIZE (sizeof(struct lcd_tss_struct))

#define LCD_COMM_ISR_ADDR (LCD_TSS_ADDR + 2*PAGE_SIZE)
#define LCD_COMM_ISR_END  (LCD_COMM_ISR_ADDR + PAGE_SIZE)

#define LCD_STACK_BOTTOM (LCD_COMM_ISR_END + PAGE_SIZE)
#define LCD_STACK_SIZE   (PAGE_SIZE * 8)
#define LCD_STACK_TOP    (LCD_STACK_BOTTOM + LCD_STACK_SIZE)
#define LCD_STACK_ADDR   LCD_STACK_TOP

#define LCD_NR_ISRS      256
#define LCD_ISR_START    (LCD_STACK_TOP + PAGE_SIZE)
#define LCD_ISR_END      (LCD_ISR_START + LCD_NR_ISRS*PAGE_SIZE)
#define LCD_ISR_ADDR(n)  (LCD_ISR_START + (n)*PAGE_SIZE)

#define LCD_PHY_MEM_LIMIT LCD_ISR_END

#define LCD_FREE_MEM_START (LCD_ISR_END + PAGE_SIZE)
#define LCD_TEST_CODE_ADDR LCD_FREE_MEM_START

/* Exported functions */
lcd_struct* lcd_create(void);
int lcd_destroy(lcd_struct *lcd);

int lcd_move_module(lcd_struct *lcd, struct module *mod);

int lcd_map_gpa_to_hpa(lcd_struct *lcd, u64 gpa, u64 hpa, int overwrite);
int lcd_map_gva_to_gpa(lcd_struct *lcd, u64 gva, u64 gpa, int create, int overwrite);
int lcd_find_hva_by_gpa(lcd_struct *lcd, u64 gpa, u64 *hva);

int lcd_run(lcd_struct *lcd);
const char* lcd_exit_reason(int exit_code);

// Inside LCD:
int lcd_read_mod_file(const char* filepath, void** content, long* size);

int lcd_load_vmlinux(const char* kfile, lcd_struct *lcd, u64 *elf_entry);



#endif
