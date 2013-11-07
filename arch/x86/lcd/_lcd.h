#ifndef LCD_INTERNAL_H
#define LCD_INTERNAL_H

struct vmcs {
  u32 revision_id;
  u32 abort;
  char data[0];
};

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

#endif
