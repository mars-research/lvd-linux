/*
 * vmx.c - LCD related VMX operations
 *
 * To be included by LCD source files.
 */

#ifndef LCD_VMX_C
#define LCD_VMX_C

/*
 * LCD initializes its memory space as follows:
 *
 * Physical memory starts at 1GB (0x4000_0000), ends at
 * 1GB+2MB-1 (0x41ff_ffff), totally 2MB, 512 pages.
 *
 * Virtual memory is mapped to physical memory identically.
 *
 * The first several (four) pages are for page table structures.
 */

// 1GB Physical memory start
#define LCD_PHY_MEM_START (0x1ULL << 30)
// L4 table address
#define LCD_PGD_TBL_ADDR (LCD_PHY_MEM_START)                  
// first L3 table address
#define LCD_PUD_TBL_ADDR (LCD_PGD_TBL_ADDR + PAGE_SIZE)     
// second L2 table address (in the first L3 table)
#define LCD_PMD_TBL_ADDR (LCD_PUD_TBL_ADDR + PAGE_SIZE)
// first L1 table address (in the above L2 table)
#define LCD_PTE_TBL_ADDR (LCD_PMD_TBL_ADDR + PAGE_SIZE)

// A test page for code.
#define LCD_TEST_CODE_PAGE_ADDR (LCD_PTE_TBL_ADDR + PAGE_SIZE)
// A test page for data.
#define LCD_TEST_DATA_PAGE_ADDR (LCD_TEST_CODE_PAGE_ADDR + PAGE_SIZE)

// We disable CR4.PCIDE
// LCD domain CR3 value, fixed.
#define LCD_CR3_VAL (LCD_PGD_TBLE_ADDR & PAGE_MASK)
#define LCD_PGD_VAL __pgd(_KERNPG_TABLE | (LCD_PUD_TBL_ADDR & PAGE_MASK))
#define LCD_PUD_VAL __pud(_KERNPG_TABLE | (LCD_PMD_TBL_ADDR & PAGE_MASK))
#define LCD_PMD_VAL __pmd(_KERNPG_TABLE | (LCD_PTE_TBL_ADDR & PAGE_MASK))

#define MAKE_VADDR (pgd_ofs, pud_ofs, pmd_ofs, pte_ofs, pg_ofs)         \
  ((unsigned long)((pgd_ofs << PGD_SHITF) | (pud_ofs << PUD_SHIFT) |    \
                   (pmd_ofs << PMD_SHIFT) | (pte_ofs << PTE_SHIFT) |    \
                   pg_ofs))

#define LCD_PTE_PGD_TBL __pte(__PAGE_KERNEL_EXEC | (LCD_PGD_TBL_ADDR & PAGE_MASK))
#define LCD_PTE_PUD_TBL __pte(__PAGE_KERNEL_EXEC | (LCD_PUD_TBL_ADDR & PAGE_MASK))
#define LCD_PTE_PMD_TBL __pte(__PAGE_KERNEL_EXEC | (LCD_PMD_TBL_ADDR & PAGE_MASK))
#define LCD_PTE_PTE_TBL __pte(__PAGE_KERNEL_EXEC | (LCD_PTE_TBL_ADDR & PAGE_MASK))
#define LCD_PTE_TEST_CODE_PAGE __pte(__PAGE_KERNEL_EXEC | (LCD_TEST_CODE_PAGE_ADDR & PAGE_MASK))
#define LCD_PTE_TEST_DATA_PAGE __pte(__PAGE_KERNEL_EXEC | (LCD_TEST_DATA_PAGE_ADDR & PAGE_MASK))

/*
 * EPT settings
 */

#define EPT_LEVELS 4

#define VMX_EPT_FAULT_READ 0x01
#define VMX_EPT_FAULT_WRITE 0x02
#define VMX_EPT_FAULT_INS 0x04

typedef unsigned long epte_t;

#define __EPTE_READ 0x01
#define __EPTE_WRITE 0x02
#define __EPTE_EXEC 0x04
#define __EPTE_IPAT 0x40
#define __EPTE_SZ 0x80
#define __EPTE_A 0x100
#define __EPTE_D 0x200
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

#define EPTE_ADDR (~(PAGE_SIZE - 1))
#define EPTE_FLAGS (PAGE_SIZE - 1)

static inline uintptr_t epte_addr(epte_t epte)
{
  return (epte & EPTE_ADDR);
}

static inline uintptr_t epte_page_vaddr(epte_t epte)
{
  return (uintptr_t) __va(epte_addr(epte));
}

static inline epte_t epte_flags(epte_t epte)
{
  return (epte & EPTE_FLAGS);
}

static inline int epte_present(epte_t epte)
{
  return (epte & __EPTE_FULL) > 0;
}

static void free_ept_page(epte_t epte)
{
  struct page *page = pfn_to_page(epte_addr(epte) >> PAGE_SHIFT);

  if (epte & __EPTE_WRITE)
    set_page_dirty_lock(page);
  put_page(page);
}

static int ept_clear_epte(epte_t *epte)
{
  if (*epte == __EPTE_NONE)
    return 0;

  free_ept_page(*epte);
  *epte = __EPTE_NONE;

  return 1;
}

static int ept_clear_l1_epte(epte_t *epte)
{
  int i;
  epte_t *pte = (epte_t *) epte_page_vaddr(*epte);

  if (*epte == __EPTE_NONE)
    return 0;

  for (i = 0; i < PTRS_PER_PTE; i++) {
    if (!epte_present(pte[i]))
      continue;

    free_ept_page(pte[i]);
  }

  free_page((uintptr_t) pte);
  *epte = __EPTE_NONE;

  return 1;
}

#define ADDR_TO_IDX(la, n)                                      \
  ((((unsigned long) (la)) >> (12 + 9 * (n))) & ((1 << 9) - 1))


static int ept_lookup_gpa(struct vmx_vcpu *vcpu,
                          void *gpa,
                          int create,
                          epte_t **epte_out)
{
  int i;
  epte_t *dir = (epte_t *) __va(vcpu->ept_root);

  for (i = EPT_LEVELS - 1; i > 0; i--) {
    int idx = ADDR_TO_IDX(gpa, i);

    if (!epte_present(dir[idx])) {
      void *page;

      if (!create)
        return -ENOENT;

      page = (void *) __get_free_page(GFP_ATOMIC);
      if (!page)
        return -ENOMEM;

      memset(page, 0, PAGE_SIZE);
      dir[idx] = epte_addr(virt_to_phys(page)) |
          __EPTE_FULL;
    }

    if (epte_big(dir[idx])) {
      return -EINVAL;
    }

    dir = (epte_t *) epte_page_vaddr(dir[idx]);
  }

  *epte_out = &dir[ADDR_TO_IDX(gpa, 0)];
  return 0;
}


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

static void vmx_free_ept(unsigned long ept_root)
{
  epte_t *pgd = (epte_t *) __va(ept_root);
  int i, j, k, l;

  for (i = 0; i < PTRS_PER_PGD; i++) {
    epte_t *pud = (epte_t *) epte_page_vaddr(pgd[i]);
    if (!epte_present(pgd[i]))
      continue;

    for (j = 0; j < PTRS_PER_PUD; j++) {
      epte_t *pmd = (epte_t *) epte_page_vaddr(pud[j]);
      if (!epte_present(pud[j]))
        continue;
      if (epte_flags(pud[j]) & __EPTE_SZ)
        continue;

      for (k = 0; k < PTRS_PER_PMD; k++) {
        epte_t *pte = (epte_t *) epte_page_vaddr(pmd[k]);
        if (!epte_present(pmd[k]))
          continue;
        if (epte_flags(pmd[k]) & __EPTE_SZ) {
          free_ept_page(pmd[k]);
          continue;
        }

        for (l = 0; l < PTRS_PER_PTE; l++) {
          if (!epte_present(pte[l]))
            continue;

          free_ept_page(pte[l]);
        }

        free_page((unsigned long) pte);
      }

      free_page((unsigned long) pmd);
    }

    free_page((unsigned long) pud);
  }

  free_page((unsigned long) pgd);
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

  eptp = VMX_EPT_DEFAULT_MT |
      VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT;
  if (cpu_has_vmx_ept_ad_bits())
    eptp |= VMX_EPT_AD_ENABLE_BIT;
  eptp |= (root_hpa & PAGE_MASK);

  return eptp;
}


static int ept_set_epte(struct vmx_vcpu *vcpu,
                        unsigned long gpa,
                        unsigned long hpa,
                        int overwrite)
{
  int ret;
  epte_t *epte, flags;

  spin_lock(&vcpu->ept_lock);

  ret = ept_lookup_gpa(vcpu, (void *) gpa, 1, &epte);
  if (ret) {
    spin_unlock(&vcpu->ept_lock);
    printk(KERN_ERR "ept: failed to lookup EPT entry\n");
    return ret;
  }

  if (epte_present(*epte)) {
    if (overwrite)
      ept_clear_epte(epte);
    else {
      spin_unlock(&vcpu->ept_lock);
      printk(KERN_ERR "ept: epte exists %p\n", (void*)(*epte));
      return -EINVL;
    }
  }

  flags = __EPTE_READ | __EPTE_EXEC | __EPTE_WRITE |
      __EPTE_TYPE(EPTE_TYPE_WB) | __EPTE_IPAT;

  if (vcpu->ept_ad_enabled) {
    /* premark A/D to avoid extra memory references */
    flags |= __EPTE_A | __EPTE_D;
  }

  *epte = epte_addr(hpa) | flags;

  spin_unlock(&vcpu->ept_lock);

  return 0;
}

static int vmx_setup_initial_page_table(struct vmx_vcpu *vcpu) {
  pgd_t *pgd_dir = (pgd_t*)(page_to_pfn(vcpu->pgd_table) << PAGE_SHIFT);
  pgd_t *pgd = pgd_dir + pgd_index(LCD_PGD_TBL_ADDR);
  set_pgd(pgd, LCD_PGD_VAL);

  pud_t *pud_dir = (pud_t*)(page_to_pfn(vcpu->pud_table) << PAGE_SHIFT);
  pud_t *pud = pud_dir + pud_index(LCD_PGD_TBL_ADDR);
  set_pud(pud, LCD_PUD_VAL);

  pmd_t *pmd_dir = (pmd_t*)(page_to_pfn(vcpu->pmd_table) << PAGE_SHIFT);
  pmd_t *pmd = pmd_dir + pmd_index(LCD_PGD_TBL_ADDR);
  set_pmd(pmd, LCD_PMD_VAL);

  pte_t *pte_dir = (pte_t*)(page_to_pfn(vcpu->pte_table) << PAGE_SHIFT);
  pte_t *pte = pte_dir + pte_index(LCD_PGD_TBL_ADDR);
  set_pte(pte, LCD_PTE_PGD_TBL);
  ++pte;
  set_pte(pte, LCD_PTE_PUD_TBL);
  ++pte;
  set_pte(pte, LCD_PTE_PMD_TBL);
  ++pte;
  set_pte(pte, LCD_PTE_PTE_TBL);
  ++pte;
  set_pte(pte, LCD_PTE_TEST_CODE_PAGE);
  ++pte;
  set_pte(pte, LCD_PTE_TEST_DATA_PAGE);  
}

static int vmx_create_ept(struct vmx_vcpu *vcpu)
{
  int ret;
  
  vcpu->pgd_table = alloc_page(GFP_KERNEL);
  if (!vcpu->pgd_table) {
    ret = -ENOMEM;
    goto fail;
  }

  vcpu->pud_table = alloc_page(GFP_KERNEL);
  if (!vcpu->pud_table) {
    ret = -ENOMEM;
    goto out1;
  }

  vcpu->pmd_table = alloc_page(GFP_KERNEL);
  if (!vcpu->pmd_table) {
    ret = -ENOMEM;
    goto out2;
  }

  vcpu->pte_table = alloc_page(GFP_KERNEL);
  if (!vcpu->pte_table) {
    ret = -ENOMEM;
    goto out3;
  }

  vcpu->code_page = alloc_page(GFP_KERNEL);
  if (!vcpu->code_page) {
    ret = -ENOMEM;
    goto out4;
  }

  vcpu->data_page = alloc_page(GFP_KERNEL);
  if (!vcpu->data_page) {
    ret = -ENOMEM;
    goto out5;
  }

  ret = ept_set_epte(vcpu, LCD_PGD_TBL_ADDR,
                     page_to_pfn(vcpu->pgd_table) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }
  ret = ept_set_epte(vcpu, LCD_PUD_TBL_ADDR,
                     page_to_pfn(vcpu->pud_table) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }
  ret = ept_set_epte(vcpu, LCD_PMD_TBL_ADDR,
                     page_to_pfn(vcpu->pmd_table) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }
  ret = ept_set_epte(vcpu, LCD_PTE_TBL_ADDR,
                     page_to_pfn(vcpu->pte_table) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }
  ret = ept_set_epte(vcpu, LCD_TEST_CODE_PAGE_ADDR,
                     page_to_pfn(vcpu->code_page) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }
  ret = ept_set_epte(vcpu, LCD_TEST_DATA_PAGE_ADDR,
                     page_to_pfn(vcpu->data_page) << PAGE_SHIFT);
  if (ret < 0) {
    goto out6;
  }

  vcpu->page_table_pool = LCD_TEST_DATA_PAGE_ADDR + PAGE_SIZE;

  ret = vmx_setup_initial_page_table(vcpu);
  if (ret == 0) {
    return ret;
  }

out6:
  __free_page(vcpu->data_page);
out5:
  __free_page(vcpu->code_page);  
out4:
  __free_page(vcpu->pte_table);
out3:
  __free_page(vcpu->pmd_table);
out2:
  __free_page(vcpu->pud_table);
out1:
  __free_page(vcpu->pgd_table);
fail:
  vmx_free_ept(vcpu->ept_root);

  return ret;
}

static void vmx_setup_initial_guest_state(struct vmx_vcpu *vcpu)
{
  unsigned long tmpl;
  unsigned long cr4 = X86_CR4_PAE | X86_CR4_VMXE | X86_CR4_OSXMMEXCPT |
      X86_CR4_PGE | X86_CR4_OSFXSR;
  /* if (boot_cpu_has(X86_FEATURE_PCID)) */
    /* cr4 |= X86_CR4_PCIDE; */
  if (boot_cpu_has(X86_FEATURE_OSXSAVE))
    cr4 |= X86_CR4_OSXSAVE;
  if (boot_cpu_has(X86_FEATURE_FSGSBASE))
    cr4 |= X86_CR4_RDWRGSFS;

  /* configure control and data registers */
  vmcs_writel(GUEST_CR0, X86_CR0_PG | X86_CR0_PE | X86_CR0_WP |
              X86_CR0_MP | X86_CR0_ET | X86_CR0_NE);
  vmcs_writel(CR0_READ_SHADOW, X86_CR0_PG | X86_CR0_PE | X86_CR0_WP |
              X86_CR0_MP | X86_CR0_ET | X86_CR0_NE);
  vmcs_writel(GUEST_CR3, LCD_CR3_VAL);
  vmcs_writel(GUEST_CR4, cr4);
  vmcs_writel(CR4_READ_SHADOW, cr4);
  vmcs_writel(GUEST_IA32_EFER, EFER_LME | EFER_LMA |
              EFER_SCE | EFER_FFXSR);
  vmcs_writel(GUEST_GDTR_BASE, 0);
  vmcs_writel(GUEST_GDTR_LIMIT, 0);
  vmcs_writel(GUEST_IDTR_BASE, 0);
  vmcs_writel(GUEST_IDTR_LIMIT, 0);
  vmcs_writel(GUEST_RIP, LCD_TEST_CODE_PAGE_ADDR);
  vmcs_writel(GUEST_RSP, LCD_TEST_DATA_PAGE_ADDR+PAGE_SIZE-sizeof(unsigned long));
  vmcs_writel(GUEST_RFLAGS, 0x02);
  vmcs_writel(GUEST_DR7, 0);

  /* guest segment bases */
  vmcs_writel(GUEST_CS_BASE, 0);
  vmcs_writel(GUEST_DS_BASE, 0);
  vmcs_writel(GUEST_ES_BASE, 0);
  vmcs_writel(GUEST_GS_BASE, 0);
  vmcs_writel(GUEST_SS_BASE, 0);
  /* rdmsrl(MSR_FS_BASE, tmpl); */
  vmcs_writel(GUEST_FS_BASE, 0);

  /* guest segment access rights */
  vmcs_writel(GUEST_CS_AR_BYTES, 0xA09B);
  vmcs_writel(GUEST_DS_AR_BYTES, 0xA093);
  vmcs_writel(GUEST_ES_AR_BYTES, 0xA093);
  vmcs_writel(GUEST_FS_AR_BYTES, 0xA093);
  vmcs_writel(GUEST_GS_AR_BYTES, 0xA093);
  vmcs_writel(GUEST_SS_AR_BYTES, 0xA093);

  /* guest segment limits */
  vmcs_write32(GUEST_CS_LIMIT, 0xFFFFFFFF);
  vmcs_write32(GUEST_DS_LIMIT, 0xFFFFFFFF);
  vmcs_write32(GUEST_ES_LIMIT, 0xFFFFFFFF);
  vmcs_write32(GUEST_FS_LIMIT, 0xFFFFFFFF);
  vmcs_write32(GUEST_GS_LIMIT, 0xFFFFFFFF);
  vmcs_write32(GUEST_SS_LIMIT, 0xFFFFFFFF);

  /* configure segment selectors */
  vmcs_write16(GUEST_CS_SELECTOR, 0);
  vmcs_write16(GUEST_DS_SELECTOR, 0);
  vmcs_write16(GUEST_ES_SELECTOR, 0);
  vmcs_write16(GUEST_FS_SELECTOR, 0);
  vmcs_write16(GUEST_GS_SELECTOR, 0);
  vmcs_write16(GUEST_SS_SELECTOR, 0);
  vmcs_write16(GUEST_TR_SELECTOR, 0);

  /* guest LDTR */
  vmcs_write16(GUEST_LDTR_SELECTOR, 0);
  vmcs_writel(GUEST_LDTR_AR_BYTES, 0x0082);
  vmcs_writel(GUEST_LDTR_BASE, 0);
  vmcs_writel(GUEST_LDTR_LIMIT, 0);

  /* guest TSS */
  vmcs_writel(GUEST_TR_BASE, 0);
  vmcs_writel(GUEST_TR_AR_BYTES, 0x0080 | AR_TYPE_BUSY_64_TSS);
  vmcs_writel(GUEST_TR_LIMIT, 0xff);

  /* initialize sysenter */
  vmcs_write32(GUEST_SYSENTER_CS, 0);
  vmcs_writel(GUEST_SYSENTER_ESP, 0);
  vmcs_writel(GUEST_SYSENTER_EIP, 0);

  /* other random initialization */
  vmcs_write32(GUEST_ACTIVITY_STATE, GUEST_ACTIVITY_ACTIVE);
  vmcs_write32(GUEST_INTERRUPTIBILITY_INFO, 0);
  vmcs_write32(GUEST_PENDING_DBG_EXCEPTIONS, 0);
  vmcs_write64(GUEST_IA32_DEBUGCTL, 0);
  vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, 0);  /* 22.2.1 */
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
  if (vmx_create_ept(vcpu)) {
    printk(KERN_ERRO "vmx: creating EPT failed.");
    goto fail_ept;
  }

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

#endif  // LCD_VMX_C
