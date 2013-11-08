/*
 * _lcd_vmx_ops.c - LCD related VMX operations
 *
 * To be included by LCD source files.
 */

#ifndef LCD__LCD_VMX_OPS_C
#define LCD__LCD_VMX_OPS_C

// We disable CR4.PCIDE
#define LCD_PHY_MEM_START (0x1ULL << 30)
#define LCD_CR3_VAL (LCD_PHY_MEM_START << PAGE_SHIFT)
// Fix: LCD_CR3_VAL & with PUD_MASK
#define LCD_PML4_0_VAL (_KERNPG_TABLE | (LCD_CR3_VAL))


/* The label in the asm code. */
extern unsigned long vmx_return;

static struct vmcs_config {
  int size;
  int order;
  u32 revision_id;
  u32 pin_based_exec_ctrl;
  u32 cpu_based_exec_ctrl;
  u32 cpu_based_2nd_exec_ctrl;
  u32 vmexit_ctrl;
  u32 vmentry_ctrl;
} vmcs_config;

#include "vmx_ops.c"

static __init int setup_vmcs_config(struct vmcs_config *vmcs_conf)
{
  u32 vmx_msr_low, vmx_msr_high;
  u32 min, opt, min2, opt2;
  u32 _pin_based_exec_control = 0;
  u32 _cpu_based_exec_control = 0;
  u32 _cpu_based_2nd_exec_control = 0;
  u32 _vmexit_control = 0;
  u32 _vmentry_control = 0;

  /* Primrary Processor-based VME controls:
     - No TSC offsetting (not set)
     - HLT exit
     - Invalidate PG exit
     - MWAIT exit
     - RDPMC exit
     - L/S CR8 exit
     - Unconditional I/O exit (no I/O bitmap)
     - No MSR bitmap
   */  
  min = CPU_BASED_HLT_EXITING |
      CPU_BASED_MWAIT_EXITING |
      CPU_BASED_RDPMC_EXITING |
      CPU_BASED_CR8_LOAD_EXITING |
      CPU_BASED_CR8_STORE_EXITING |
      CPU_BASED_MOV_DR_EXITING |
      CPU_BASED_UNCOND_IO_EXITING | 
      CPU_BASED_MONITOR_EXITING;

  opt = CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
  if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PROCBASED_CTLS,
                          &_cpu_based_exec_control) < 0)
    return -EIO;
  if ((_cpu_based_exec_control & CPU_BASED_TPR_SHADOW))
    _cpu_based_exec_control &= ~CPU_BASED_CR8_LOAD_EXITING &
        ~CPU_BASED_CR8_STORE_EXITING;

  // We need the secondary VME controls.
  if (_cpu_based_exec_control & CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) {
    min2 = 0;
    /* Secondary VME control:
       - No virtual APIC access
     */
    opt2 = SECONDARY_EXEC_ENABLE_EPT |
        SECONDARY_EXEC_RDTSCP |
        SECONDARY_EXEC_ENABLE_VPID |
        SECONDARY_EXEC_WBINVD_EXITING |
        SECONDARY_EXEC_ENABLE_INVPCID;
    if (adjust_vmx_controls(min2, opt2,
                            MSR_IA32_VMX_PROCBASED_CTLS2,
                            &_cpu_based_2nd_exec_control) < 0)
      return -EIO;
  } else {
    printk(KERN_ERR "lcd: secondary VME controls not supported\n")
    return -EIO;
  }

  // We need EPT.
  if (_cpu_based_2nd_exec_control & SECONDARY_EXEC_ENABLE_EPT) {
    rdmsr(MSR_IA32_VMX_EPT_VPID_CAP,
          vmx_capability.ept, vmx_capability.vpid);
  } else {
    printk(KERN_ERR "lcd: EPT not supported\n")
    return -EIO;
  }

  // Pin-based control:
  min = PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING;
  opt = 0;
  if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PINBASED_CTLS,
                          &_pin_based_exec_control) < 0)
    return -EIO;


  // Exit control:
  min = VM_EXIT_HOST_ADDR_SPACE_SIZE;
  opt = 0;
  if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_EXIT_CTLS,
                          &_vmexit_control) < 0)
    return -EIO;

  // Entry control:
  min = VM_ENTRY_IA32E_MODE;
  opt = 0;
  if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_ENTRY_CTLS,
                          &_vmentry_control) < 0)
    return -EIO;

  rdmsr(MSR_IA32_VMX_BASIC, vmx_msr_low, vmx_msr_high);

  /* IA-32 SDM Vol 3B: VMCS size is never greater than 4kB. */
  if ((vmx_msr_high & 0x1fff) > PAGE_SIZE)
    return -EIO;

  /* IA-32 SDM Vol 3B: 64-bit CPUs always have VMX_BASIC_MSR[48]==0. */
  if (vmx_msr_high & (1u<<16))
    return -EIO;

  /* Require Write-Back (WB) memory type for VMCS accesses. */
  if (((vmx_msr_high >> 18) & 15) != 6)
    return -EIO;

  vmcs_conf->size = vmx_msr_high & 0x1fff;
  vmcs_conf->order = get_order(vmcs_config.size);
  vmcs_conf->revision_id = vmx_msr_low;

  vmcs_conf->pin_based_exec_ctrl = _pin_based_exec_control;
  vmcs_conf->cpu_based_exec_ctrl = _cpu_based_exec_control;
  vmcs_conf->cpu_based_2nd_exec_ctrl = _cpu_based_2nd_exec_control;
  vmcs_conf->vmexit_ctrl         = _vmexit_control;
  vmcs_conf->vmentry_ctrl        = _vmentry_control;

  vmx_capability.has_load_efer =
      allow_1_setting(MSR_IA32_VMX_ENTRY_CTLS,
                      VM_ENTRY_LOAD_IA32_EFER)
      && allow_1_setting(MSR_IA32_VMX_EXIT_CTLS,
                         VM_EXIT_LOAD_IA32_EFER);

  return 0;
}

static struct vmcs *vmx_alloc_vmcs(void)
{
  int cpu = raw_smp_processor_id();
  int node = cpu_to_node(cpu);
  struct page *pages;
  struct vmcs *vmcs;

  pages = alloc_pages_exact_node(node, GFP_KERNEL, vmcs_config.order);
  if (!pages)
    return NULL;
  vmcs = page_address(pages);
  memset(vmcs, 0, vmcs_config.size);
  vmcs->revision_id = vmcs_config.revision_id; /* vmcs revision id */
  return vmcs;
}

static void vmx_free_vmcs(struct vmcs *vmcs)
{
  free_pages((unsigned long)vmcs, vmcs_config.order);
}

static void vmx_setup_constant_host_state(void)
{
  u32 low32, high32;
  unsigned long tmpl;
  struct desc_ptr dt;

  vmcs_writel(HOST_CR0, read_cr0() & ~X86_CR0_TS);  /* 22.2.3 */
  vmcs_writel(HOST_CR4, read_cr4());  /* 22.2.3, 22.2.5 */
  vmcs_writel(HOST_CR3, read_cr3());  /* 22.2.3 */

  vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);  /* 22.2.4 */
  vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);  /* 22.2.4 */

  native_store_idt(&dt);
  vmcs_writel(HOST_IDTR_BASE, dt.address);   /* 22.2.4 */

  /* TODO: change the label for vmx_return  */
  asm("mov $.Lvmx_return, %0" : "=r"(tmpl));
  vmcs_writel(HOST_RIP, tmpl); /* 22.2.5 */

  rdmsr(MSR_IA32_SYSENTER_CS, low32, high32);
  vmcs_write32(HOST_IA32_SYSENTER_CS, low32);
  rdmsrl(MSR_IA32_SYSENTER_EIP, tmpl);
  vmcs_writel(HOST_IA32_SYSENTER_EIP, tmpl);   /* 22.2.3 */

  rdmsr(MSR_EFER, low32, high32);
  vmcs_write32(HOST_IA32_EFER, low32);

  if (vmcs_config.vmexit_ctrl & VM_EXIT_LOAD_IA32_PAT) {
    printk(KERN_INFO "lcd: we need to load PAT on exit?\n");
    rdmsr(MSR_IA32_CR_PAT, low32, high32);
    vmcs_write64(HOST_IA32_PAT, low32 | ((u64) high32 << 32));
  }

  vmcs_write16(HOST_FS_SELECTOR, 0);            /* 22.2.4 */
  vmcs_write16(HOST_GS_SELECTOR, 0);            /* 22.2.4 */

  rdmsrl(MSR_FS_BASE, tmpl);
  vmcs_writel(HOST_FS_BASE, tmpl); /* 22.2.4 */
  rdmsrl(MSR_GS_BASE, tmpl);
  vmcs_writel(HOST_GS_BASE, tmpl); /* 22.2.4 */
}

static void setup_msr(struct vmx_vcpu *vcpu)
{
  int set[] = { MSR_LSTAR };
  struct vmx_msr_entry *e;
  int sz = sizeof(set) / sizeof(*set);
  int i;

  sz = 0;

  BUILD_BUG_ON(sz > NR_AUTOLOAD_MSRS);

  vcpu->msr_autoload.nr = sz;

  /* XXX enable only MSRs in set */
  vmcs_write64(MSR_BITMAP, __pa(msr_bitmap));

  vmcs_write32(VM_EXIT_MSR_STORE_COUNT, vcpu->msr_autoload.nr);
  vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, vcpu->msr_autoload.nr);
  vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, vcpu->msr_autoload.nr);

  vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.host));
  vmcs_write64(VM_EXIT_MSR_STORE_ADDR, __pa(vcpu->msr_autoload.guest));
  vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.guest));

  for (i = 0; i < sz; i++) {
    uint64_t val;

    e = &vcpu->msr_autoload.host[i];
    e->index = set[i];
    rdmsrl(e->index, val);
    e->value = val;

    e = &vcpu->msr_autoload.guest[i];
    e->index = set[i];
  }
}

static void vmx_setup_vmcs(struct vmx_vcpu *vcpu)
{
  vmcs_write16(VIRTUAL_PROCESSOR_ID, vcpu->vpid);
  vmcs_write64(VMCS_LINK_POINTER, -1ull); /* 22.3.1.5 */

  /* Control */
  vmcs_write32(PIN_BASED_VM_EXEC_CONTROL,
               vmcs_config.pin_based_exec_ctrl);

  vmcs_write32(CPU_BASED_VM_EXEC_CONTROL,
               vmcs_config.cpu_based_exec_ctrl);

  if (cpu_has_secondary_exec_ctrls()) {
    vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
                 vmcs_config.cpu_based_2nd_exec_ctrl);
  }

  vmcs_write64(EPT_POINTER, vcpu->eptp);

  vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK, 0);
  vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH, 0);
  vmcs_write32(CR3_TARGET_COUNT, 0);           /* 22.2.1 */

  setup_msr(vcpu);
  
#if 0
  // We don't need any MSR and PAT>
  if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
    u32 msr_low, msr_high;
    u64 host_pat;
    rdmsr(MSR_IA32_CR_PAT, msr_low, msr_high);
    host_pat = msr_low | ((u64) msr_high << 32);
    /* Write the default value follow host pat */
    vmcs_write64(GUEST_IA32_PAT, host_pat);
    /* Keep arch.pat sync with GUEST_IA32_PAT */
    vmx->vcpu.arch.pat = host_pat;
  }

  for (i = 0; i < NR_VMX_MSR; ++i) {
    u32 index = vmx_msr_index[i];
    u32 data_low, data_high;
    int j = vmx->nmsrs;

    if (rdmsr_safe(index, &data_low, &data_high) < 0)
      continue;
    if (wrmsr_safe(index, data_low, data_high) < 0)
      continue;
    vmx->guest_msrs[j].index = i;
    vmx->guest_msrs[j].data = 0;
    vmx->guest_msrs[j].mask = -1ull;
    ++vmx->nmsrs;
  }
#endif

  //vmcs_config.vmentry_ctrl |= VM_ENTRY_IA32E_MODE;

  vmcs_write32(VM_EXIT_CONTROLS, vmcs_config.vmexit_ctrl);
  vmcs_write32(VM_ENTRY_CONTROLS, vmcs_config.vmentry_ctrl);

  vmcs_writel(CR0_GUEST_HOST_MASK, ~0ul);
  vmcs_writel(CR4_GUEST_HOST_MASK, ~0ul);

  vmcs_writel(TSC_OFFSET, 0);

  vmx_setup_constant_host_state();
}

/**
 * vmx_allocate_vpid - reserves a vpid and sets it in the VCPU
 * @vmx: the VCPU
 */
static int vmx_allocate_vpid(struct vmx_vcpu *vmx)
{
  int vpid;

  vmx->vpid = 0;

  spin_lock(&vmx_vpid_lock);
  vpid = find_first_zero_bit(vmx_vpid_bitmap, VMX_NR_VPIDS);
  if (vpid < VMX_NR_VPIDS) {
    vmx->vpid = vpid;
    __set_bit(vpid, vmx_vpid_bitmap);
  }
  spin_unlock(&vmx_vpid_lock);

  return vpid >= VMX_NR_VPIDS;
}

/**
 * vmx_free_vpid - frees a vpid
 * @vmx: the VCPU
 */
static void vmx_free_vpid(struct vmx_vcpu *vmx)
{
  spin_lock(&vmx_vpid_lock);
  if (vmx->vpid != 0)
    __clear_bit(vmx->vpid, vmx_vpid_bitmap);
  spin_unlock(&vmx_vpid_lock);
}

int vmx_init_ept(struct vmx_vcpu *vcpu)
{
  void *page = (void *) __get_free_page(GFP_KERNEL);

  if (!page)
    return -ENOMEM;

  memset(page, 0, PAGE_SIZE);
  vcpu->ept_root =  __pa(page);

  return 0;
}

static u64 construct_eptp(unsigned long root_hpa)
{
  u64 eptp;

  /* TODO write the value reading from MSR */
  eptp = VMX_EPT_DEFAULT_MT |
      VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT;
  if (cpu_has_vmx_ept_ad_bits())
    eptp |= VMX_EPT_AD_ENABLE_BIT;
  eptp |= (root_hpa & PAGE_MASK);

  return eptp;
}

/**
 * vmx_create_vcpu - allocates and initializes a new virtual cpu
 *
 * Returns: A new VCPU structure
 */
static struct vmx_vcpu * vmx_create_vcpu()
{
  struct vmx_vcpu *vcpu = kmalloc(sizeof(struct vmx_vcpu), GFP_KERNEL);
  if (!vcpu)
    return NULL;

  memset(vcpu, 0, sizeof(*vcpu));

  vcpu->vmcs = vmx_alloc_vmcs();
  if (!vcpu->vmcs)
    goto fail_vmcs;

  if (vmx_allocate_vpid(vcpu))
    goto fail_vpid;

  vcpu->cpu = -1;

  spin_lock_init(&vcpu->ept_lock);
  if (vmx_init_ept(vcpu))
    goto fail_ept;
  vcpu->eptp = construct_eptp(vcpu->ept_root);

  vmx_get_cpu(vcpu);
  vmx_setup_vmcs(vcpu);
  vmx_setup_initial_guest_state();
  vmx_put_cpu(vcpu);

  if (cpu_has_vmx_ept_ad_bits()) {
    vcpu->ept_ad_enabled = true;
    printk(KERN_INFO "vmx: enabled EPT A/D bits");
  }
  if (vmx_create_ept(vcpu))
    goto fail_ept;

  return vcpu;

fail_ept:
  vmx_free_vpid(vcpu);
fail_vpid:
  vmx_free_vmcs(vcpu->vmcs);
fail_vmcs:
  kfree(vcpu);
  return NULL;
}

/**
 * vmx_destroy_vcpu - destroys and frees an existing virtual cpu
 * @vcpu: the VCPU to destroy
 */
static void vmx_destroy_vcpu(struct vmx_vcpu *vcpu)
{
  vmx_destroy_ept(vcpu);
  vmx_get_cpu(vcpu);
  ept_sync_context(vcpu->eptp);
  vmcs_clear(vcpu->vmcs);
  __get_cpu_var(local_vcpu) = NULL;
  vmx_put_cpu(vcpu);
  vmx_free_vpid(vcpu);
  vmx_free_vmcs(vcpu->vmcs);
  kfree(vcpu);
}

#endif  // LCD__LCD_VMX_OPS_C
