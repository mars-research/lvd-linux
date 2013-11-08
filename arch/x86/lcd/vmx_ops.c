/*
 * vmx_ops.c - VMX primitive operations
 *
 * To be included by other higher level VMX setup source files.
 */

#ifndef LCD_VMX_OPS_C
#define LCD_VMX_OPS_C

static inline bool cpu_has_secondary_exec_ctrls(void)
{
  return vmcs_config.cpu_based_exec_ctrl &
      CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
}

static inline bool cpu_has_vmx_vpid(void)
{
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_VPID;
}

static inline bool cpu_has_vmx_invpcid(void)
{
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_INVPCID;
}

static inline bool cpu_has_vmx_invvpid_single(void)
{
  return vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invvpid_global(void)
{
  return vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_ept(void)
{
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_EPT;
}

static inline bool cpu_has_vmx_invept_individual_addr(void)
{
  return vmx_capability.ept & VMX_EPT_EXTENT_INDIVIDUAL_BIT;
}

static inline bool cpu_has_vmx_invept_context(void)
{
  return vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_global(void)
{
  return vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT;
}

static inline bool cpu_has_vmx_ept_ad_bits(void)
{
  return vmx_capability.ept & VMX_EPT_AD_BIT;
}

static inline void __invvpid(int ext, u16 vpid, gva_t gva)
{
  struct {
    u64 vpid : 16;
    u64 rsvd : 48;
    u64 gva;
  } operand = { vpid, 0, gva };

  asm volatile (ASM_VMX_INVVPID
                /* CF==1 or ZF==1 --> rc = -1 */
                  "; ja 1f ; ud2 ; 1:"
                : : "a"(&operand), "c"(ext) : "cc", "memory");
}

static inline void vpid_sync_vcpu_single(u16 vpid)
{
  if (vpid == 0)
    return;

  if (cpu_has_vmx_invvpid_single())
    __invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vpid, 0);
}

static inline void vpid_sync_vcpu_global(void)
{
  if (cpu_has_vmx_invvpid_global())
    __invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0, 0);
}

static inline void vpid_sync_context(u16 vpid)
{
  if (cpu_has_vmx_invvpid_single())
    vpid_sync_vcpu_single(vpid);
  else
    vpid_sync_vcpu_global();
}


static inline u16 vmx_read_ldt(void)
{
  u16 ldt;
  asm("sldt %0" : "=g"(ldt));
  return ldt;
}

static unsigned long segment_base(u16 selector)
{
  struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
  struct desc_struct *d;
  unsigned long table_base;
  unsigned long v;

  if (!(selector & ~3))
    return 0;

  table_base = gdt->address;

  if (selector & 4) {           /* from ldt */
    u16 ldt_selector = vmx_read_ldt();

    if (!(ldt_selector & ~3))
      return 0;

    table_base = segment_base(ldt_selector);
  }
  d = (struct desc_struct *)(table_base + (selector & ~7));
  v = get_desc_base(d);
  #ifdef CONFIG_X86_64
  if (d->s == 0 && (d->type == 2 || d->type == 9 || d->type == 11))
    v |= ((unsigned long)((struct ldttss_desc64 *)d)->base3) << 32;
  #endif
  return v;
}

static inline unsigned long vmx_read_tr_base(void)
{
  u16 tr;
  asm("str %0" : "=g"(tr));
  return segment_base(tr);
}


static void __vmx_setup_cpu(void)
{
  struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
  unsigned long sysenter_esp;
  unsigned long tmpl;

  /*
   * Linux uses per-cpu TSS and GDT, so set these when switching
   * processors.
   */
  vmcs_writel(HOST_TR_BASE, vmx_read_tr_base()); /* 22.2.4 */
  vmcs_writel(HOST_GDTR_BASE, gdt->address);   /* 22.2.4 */

  rdmsrl(MSR_IA32_SYSENTER_ESP, sysenter_esp);
  vmcs_writel(HOST_IA32_SYSENTER_ESP, sysenter_esp); /* 22.2.3 */

  rdmsrl(MSR_FS_BASE, tmpl);
  vmcs_writel(HOST_FS_BASE, tmpl); /* 22.2.4 */
  rdmsrl(MSR_GS_BASE, tmpl);
  vmcs_writel(HOST_GS_BASE, tmpl); /* 22.2.4 */
}

static void __vmx_get_cpu_helper(void *ptr)
{
  struct vmx_vcpu *vcpu = ptr;

  BUG_ON(raw_smp_processor_id() != vcpu->cpu);
  vmcs_clear(vcpu->vmcs);
  if (__get_cpu_var(local_vcpu) == vcpu)
    __get_cpu_var(local_vcpu) = NULL;
}

/**
 * vmx_get_cpu - called before using a cpu
 * @vcpu: VCPU that will be loaded.
 *
 * Disables preemption. Call vmx_put_cpu() when finished.
 */
static void vmx_get_cpu(struct vmx_vcpu *vcpu)
{
  int cur_cpu = get_cpu();

  if (__get_cpu_var(local_vcpu) != vcpu) {
    __get_cpu_var(local_vcpu) = vcpu;

    if (vcpu->cpu != cur_cpu) {
      if (vcpu->cpu >= 0)
        smp_call_function_single(
            vcpu->cpu,
            __vmx_get_cpu_helper, (void *) vcpu, 1);
      else
        vmcs_clear(vcpu->vmcs);

      vpid_sync_context(vcpu->vpid);
      ept_sync_context(vcpu->eptp);

      vcpu->launched = 0;
      vmcs_load(vcpu->vmcs);
      __vmx_setup_cpu();
      vcpu->cpu = cur_cpu;
    } else {
      vmcs_load(vcpu->vmcs);
    }
  }
}

/**
 * vmx_put_cpu - called after using a cpu
 * @vcpu: VCPU that was loaded.
 */
static void vmx_put_cpu(struct vmx_vcpu *vcpu)
{
  put_cpu();
}


static __init int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt,
                                      u32 msr, u32 *result)
{
  u32 vmx_msr_low, vmx_msr_high;
  u32 ctl = ctl_min | ctl_opt;

  rdmsr(msr, vmx_msr_low, vmx_msr_high);

  ctl &= vmx_msr_high; /* bit == 0 in high word ==> must be zero */
  ctl |= vmx_msr_low;  /* bit == 1 in low word  ==> must be one  */

  /* Ensure minimum (required) set of control bits are supported. */
  if (ctl_min & ~ctl)
    return -EIO;

  *result = ctl;
  return 0;
}

static __init bool allow_1_setting(u32 msr, u32 ctl)
{
  u32 vmx_msr_low, vmx_msr_high;

  rdmsr(msr, vmx_msr_low, vmx_msr_high);
  return vmx_msr_high & ctl;
}


static inline void __vmxon(u64 addr)
{
  asm volatile (ASM_VMX_VMXON_RAX
                : : "a"(&addr), "m"(addr)
                : "memory", "cc");
}

static inline void __vmxoff(void)
{
  asm volatile (ASM_VMX_VMXOFF : : : "cc");
}

static void vmcs_clear(struct vmcs *vmcs)
{
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "kvm: vmclear fail: %p/%llx\n",
           vmcs, phys_addr);
}

static void vmcs_load(struct vmcs *vmcs)
{
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "vmx: vmptrld %p/%llx failed\n",
           vmcs, phys_addr);
}

static __always_inline unsigned long vmcs_readl(unsigned long field)
{
  unsigned long value;

  asm volatile (ASM_VMX_VMREAD_RDX_RAX
                : "=a"(value) : "d"(field) : "cc");
  return value;
}

static __always_inline u16 vmcs_read16(unsigned long field)
{
  return vmcs_readl(field);
}

static __always_inline u32 vmcs_read32(unsigned long field)
{
  return vmcs_readl(field);
}

static __always_inline u64 vmcs_read64(unsigned long field)
{
  return vmcs_readl(field);
}

static noinline void vmwrite_error(unsigned long field, unsigned long value)
{
  printk(KERN_ERR "vmwrite error: reg %lx value %lx (err %d)\n",
         field, value, vmcs_read32(VM_INSTRUCTION_ERROR));
  dump_stack();
}

static void vmcs_writel(unsigned long field, unsigned long value)
{
  u8 error;

  asm volatile (ASM_VMX_VMWRITE_RAX_RDX "; setna %0"
                : "=q"(error) : "a"(value), "d"(field) : "cc");
  if (unlikely(error))
    vmwrite_error(field, value);
}

static void vmcs_write16(unsigned long field, u16 value)
{
  vmcs_writel(field, value);
}

static void vmcs_write32(unsigned long field, u32 value)
{
  vmcs_writel(field, value);
}

static void vmcs_write64(unsigned long field, u64 value)
{
  vmcs_writel(field, value);
}

static void vmx_dump_cpu(struct vmx_vcpu *vcpu)
{
  unsigned long flags;

  vmx_get_cpu(vcpu);
  vcpu->regs[VCPU_REGS_RIP] = vmcs_readl(GUEST_RIP);
  vcpu->regs[VCPU_REGS_RSP] = vmcs_readl(GUEST_RSP);
  flags = vmcs_readl(GUEST_RFLAGS);
  vmx_put_cpu(vcpu);

  printk(KERN_INFO "vmx: --- Begin VCPU Dump ---\n");
  printk(KERN_INFO "vmx: CPU %d VPID %d\n", vcpu->cpu, vcpu->vpid);
  printk(KERN_INFO "vmx: RIP 0x%016llx RFLAGS 0x%08lx\n",
         vcpu->regs[VCPU_REGS_RIP], flags);
  printk(KERN_INFO "vmx: RAX 0x%016llx RCX 0x%016llx\n",
         vcpu->regs[VCPU_REGS_RAX], vcpu->regs[VCPU_REGS_RCX]);
  printk(KERN_INFO "vmx: RDX 0x%016llx RBX 0x%016llx\n",
         vcpu->regs[VCPU_REGS_RDX], vcpu->regs[VCPU_REGS_RBX]);
  printk(KERN_INFO "vmx: RSP 0x%016llx RBP 0x%016llx\n",
         vcpu->regs[VCPU_REGS_RSP], vcpu->regs[VCPU_REGS_RBP]);
  printk(KERN_INFO "vmx: RSI 0x%016llx RDI 0x%016llx\n",
         vcpu->regs[VCPU_REGS_RSI], vcpu->regs[VCPU_REGS_RDI]);
  printk(KERN_INFO "vmx: R8  0x%016llx R9  0x%016llx\n",
         vcpu->regs[VCPU_REGS_R8], vcpu->regs[VCPU_REGS_R9]);
  printk(KERN_INFO "vmx: R10 0x%016llx R11 0x%016llx\n",
         vcpu->regs[VCPU_REGS_R10], vcpu->regs[VCPU_REGS_R11]);
  printk(KERN_INFO "vmx: R12 0x%016llx R13 0x%016llx\n",
         vcpu->regs[VCPU_REGS_R12], vcpu->regs[VCPU_REGS_R13]);
  printk(KERN_INFO "vmx: R14 0x%016llx R15 0x%016llx\n",
         vcpu->regs[VCPU_REGS_R14], vcpu->regs[VCPU_REGS_R15]);
  printk(KERN_INFO "vmx: --- End VCPU Dump ---\n");
}

#endif // LCD_VMX_OPS_C
