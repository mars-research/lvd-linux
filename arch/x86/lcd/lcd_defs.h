#ifndef LCD_LCD_H
#define LCD_LCD_H

#include <linux/bitmap.h>

/* Memory management */

#define NR_PT_PAGES    (1 << 12)       /* #pages for page table */
#define PT_PAGES_START (0x1ULL << 24)  /* above 16MB */
#define PT_PAGES_END   (PT_PAGES_START + (NR_PT_PAGES << PAGE_SHIFT))

// #define NORMAL_MEM_START (0x1ULL << 30)

#define EPT_LEVELS 4

#define VMX_EPT_FAULT_READ  0x01
#define VMX_EPT_FAULT_WRITE 0x02
#define VMX_EPT_FAULT_INS   0x04

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

struct vmx_vcpu {
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

  u8  fail;
  u64 exit_reason;
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
};

struct lcd_tss_struct {
  struct x86_hw_tss tss;
  u8 io_bitmap[1];
} __attribute__((packed));

#define LCD_GDT_ADDR (0x1ULL << 30)
#define LCD_IDT_ADDR (LCD_GDT_ADDR + PAGE_SIZE)
#define LCD_TSS_ADDR (LCD_IDT_ADDR + PAGE_SIZE)
#define LCD_TSS_SIZE (sizeof(struct lcd_tss_struct))

#define LCD_STACK_ADDR (0x1ULL << 31)
#define LCD_STACK_SIZE (PAGE_SIZE * 4)

#define LCD_ISR_ADDR LCD_STACK_ADDR

#endif
