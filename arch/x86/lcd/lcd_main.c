#include "lcd.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/vmx.h>

MODULE_AUTHOR("FluxVirt");
MODULE_LICENSE("GPL");

static DECLARE_BITMAP(vmx_vpid_bitmap, VMX_NR_VPIDS);
static DEFINE_SPINLOCK(vmx_vpid_lock);

static DEFINE_PER_CPU(struct vmcs *, vmxarea);
static DEFINE_PER_CPU(struct desc_ptr, host_gdt);
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(struct vmx_vcpu *, local_vcpu);

static struct vmcs_config vmcs_config;

/* The label in the asm code. */
extern unsigned long vmx_return;


/* CPU Features and Ops */

static inline bool cpu_has_secondary_exec_ctrls(void) {
  return vmcs_config.cpu_based_exec_ctrl &
      CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
}

static inline bool cpu_has_vmx_vpid(void) {
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_VPID;
}

static inline bool cpu_has_vmx_invpcid(void) {
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_INVPCID;
}

static inline bool cpu_has_vmx_invvpid_single(void) {
  return vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invvpid_global(void) {
  return vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_ept(void) {
  return vmcs_config.cpu_based_2nd_exec_ctrl &
      SECONDARY_EXEC_ENABLE_EPT;
}

static inline bool cpu_has_vmx_invept_individual_addr(void) {
  return vmx_capability.ept & VMX_EPT_EXTENT_INDIVIDUAL_BIT;
}

static inline bool cpu_has_vmx_invept_context(void) {
  return vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_global(void) {
  return vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT;
}

static inline bool cpu_has_vmx_ept_ad_bits(void) {
  return vmx_capability.ept & VMX_EPT_AD_BIT;
}

static inline void __invvpid(int ext, u16 vpid, gva_t gva) {
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

static inline void vpid_sync_vcpu_single(u16 vpid) {
  if (vpid == 0)
    return;

  if (cpu_has_vmx_invvpid_single())
    __invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vpid, 0);
}

static inline void vpid_sync_vcpu_global(void) {
  if (cpu_has_vmx_invvpid_global())
    __invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0, 0);
}

static inline void vpid_sync_context(u16 vpid) {
  if (cpu_has_vmx_invvpid_single())
    vpid_sync_vcpu_single(vpid);
  else
    vpid_sync_vcpu_global();
}

static inline u16 vmx_read_ldt(void) {
  u16 ldt;
  asm("sldt %0" : "=g"(ldt));
  return ldt;
}

static __init bool allow_1_setting(u32 msr, u32 ctl) {
  u32 vmx_msr_low, vmx_msr_high;

  rdmsr(msr, vmx_msr_low, vmx_msr_high);
  return vmx_msr_high & ctl;
}

static inline void __vmxon(u64 addr) {
  asm volatile (ASM_VMX_VMXON_RAX
                : : "a"(&addr), "m"(addr)
                : "memory", "cc");
}

static inline void __vmxoff(void) {
  asm volatile (ASM_VMX_VMXOFF : : : "cc");
}

static void vmcs_clear(struct vmcs *vmcs) {
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "kvm: vmclear fail: %p/%llx\n",
           vmcs, phys_addr);
}

static void vmcs_load(struct vmcs *vmcs) {
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "vmx: vmptrld %p/%llx failed\n",
           vmcs, phys_addr);
}

static __always_inline unsigned long vmcs_readl(unsigned long field) {
  unsigned long value;

  asm volatile (ASM_VMX_VMREAD_RDX_RAX
                : "=a"(value) : "d"(field) : "cc");
  return value;
}

static __always_inline u16 vmcs_read16(unsigned long field) {
  return vmcs_readl(field);
}

static __always_inline u32 vmcs_read32(unsigned long field) {
  return vmcs_readl(field);
}

static __always_inline u64 vmcs_read64(unsigned long field) {
  return vmcs_readl(field);
}

static noinline void vmwrite_error(unsigned long field,
                                   unsigned long value) {
  printk(KERN_ERR "vmwrite error: reg %lx value %lx (err %d)\n",
         field, value, vmcs_read32(VM_INSTRUCTION_ERROR));
  dump_stack();
}

static void vmcs_writel(unsigned long field, unsigned long value) {
  u8 error;

  asm volatile (ASM_VMX_VMWRITE_RAX_RDX "; setna %0"
                : "=q"(error) : "a"(value), "d"(field) : "cc");
  if (unlikely(error))
    vmwrite_error(field, value);
}

static void vmcs_write16(unsigned long field, u16 value) {
  vmcs_writel(field, value);
}

static void vmcs_write32(unsigned long field, u32 value) {
  vmcs_writel(field, value);
}

static void vmcs_write64(unsigned long field, u64 value) {
  vmcs_writel(field, value);
}


/* Memory Management */

static inline uintptr_t epte_addr(epte_t epte) {
  return (epte & EPTE_ADDR);
}

static inline uintptr_t epte_page_vaddr(epte_t epte) {
  return (uintptr_t) __va(epte_addr(epte));
}

static inline epte_t epte_flags(epte_t epte) {
  return (epte & EPTE_FLAGS);
}

static inline int epte_present(epte_t epte) {
  return (epte & __EPTE_FULL) > 0;
}

static void free_ept_page(epte_t epte) {
  struct page *page = pfn_to_page(epte_addr(epte) >> PAGE_SHIFT);

  if (epte & __EPTE_WRITE)
    set_page_dirty_lock(page);
  put_page(page);
}

static int clear_epte(epte_t *epte) {
  if (*epte == __EPTE_NONE)
    return 0;

  free_ept_page(*epte);
  *epte = __EPTE_NONE;

  return 1;
}

static int clear_l1_epte(epte_t *epte) {
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

static int ept_lookup_gpa(struct vmx_vcpu *vcpu,
                          void *gpa,
                          int create,
                          epte_t **epte_out) {
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

static void __set_epte(struct vmx_vcpu *vcpu,
                       epte_t* epte, unsigned long hpa) {
  epte_t flags = __EPTE_READ | __EPTE_EXEC | __EPTE_WRITE |
      __EPTE_TYPE(EPTE_TYPE_WB) | __EPTE_IPAT;

  if (vcpu->ept_ad_enabled) {
    /* premark A/D to avoid extra memory references */
    flags |= __EPTE_A | __EPTE_D;
  }

  *epte = epte_addr(hpa) | flags;
}

static int ept_set_epte(struct vmx_vcpu *vcpu,
                        unsigned long gpa,
                        unsigned long hpa,
                        int overwrite) {
  int ret;
  epte_t *epte;

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

  __set_epte(vcpu, epte, hpa);
  
  spin_unlock(&vcpu->ept_lock);

  return 0;
}

static unsigned long alloc_pt_pfn(struct vmx_vcpu *vcpu) {
  unsigned long which = bitmap_find_next_zero_area(vcpu->bmp_pt_pages,
                                                   NR_PT_PAGES,
                                                   0, 1, 0);
  if (which >= NR_PT_PAGES) {
    return 0;
  } else {
    bitmap_set(vcpu->bmp_pt_pages, which, 1);
    return (PT_PAGES_START >> PAGE_SHIFT) + which;
  }                               
}

static int free_pt_pfn(struct vmx_vcpu *vcpu,
                       unsigned long pfn) {
  unsigned long which = pfn - (PT_PAGES_START >> PAGE_SHIFT);
  if (which >= (PT_PAGES_END >> PAGE_SHIFT)) {
    return -EINVL; 
  } else {
    bitmap_clear(vcpu->bmp_pt_pages, which, 1);
    return 0;
  }
}

static int ept_alloc_pt_item(struct vmx_vcpu *vcpu,
                             unsigned long *gpfn,
                             unsigned long *page) {
  int ret = 0;
  *gpfn = alloc_pt_pfn(vcpu);
  if (*gpfn == 0) {
    ret = -ENOMEM;
  } else {
    epte_t *epte;
    ret = ept_lookup_gpa(vcpu, ((*gpfn) << PAGE_SHIFT), 1, &epte);
    if (!ret) {
      if (!epte_present(*epte)) {
        *page = __get_free_page(GFP_ATOMIC);
        if (!(*page)) {
          ret = -ENOMEM;
        } else {
          memset((void*)(*page), 0, PAGE_SIZE);
          __set_epte(vcpu, epte, *page);
        }
      } else {
        *page = (unsigned long)epte_page_vaddr(*epte);
      }
    }
  }
  return ret;
}

static int ept_gpa_to_hva(struct vmx_vcpu* vcpu,
                          unsigned long gpa,
                          unsigned long* hva) {
  epte_t *epte;
  int ret = ept_lookup_gpa(vcpu, gpa, 0, &epte);
  if (ret) {
    printk(KERN_ERR "ept: failed to lookup GPA: %p\n",
           (void*)gpa);
  } else {
    if (!epte_present(*epte)) {
      printk(KERN_ERR "ept: epte not present when translating\n");
      ret = -ENOENT;
    } else {
      *hva = (unsigned long)epte_page_vaddr(*epte);
    }
  }
  return ret;
}

// Set gva => gpa. No page allocated for gva/gpa, just
// pte value setting. But page table structures along the
// path can be created by setting 'create' to 1.
static int map_gva_to_gpa(struct vmx_vcpu *vcpu,
                          unsigned long gva,
                          unsigned long gpa,
                          int create, int overwrite) {
  int ret = 0;
  unsigned long gpfn, page;
  epte_t *epte;
  
  pud_t *pud_dir, *pud;
  pmd_t *pmd_dir, *pmd;
  pte_t *pte_dir, *pte;
  
  pgd_t *pgd;
  if (!vcpu->pt) {
    if (!create)
      return -ENOENT;
    else {
      ret = ept_alloc_pt_item(vcpu, &gpfn, &page);
      if (ret)
        return ret;
      else {
        vcpu->pt = (pgd_t*)page;
        vcpu->pt_gpa = (gpfn << PAGE_SHIFT);
      }
    }
  }
  
  pgd = vcpu->pt + pgd_index(gva);
  if (!pgd_present(*pgd)) {
    if (!create) {
      return -ENOENT;
    } else {
      ret = ept_alloc_pt_item(vcpu, &gpfn, &page);
      if (ret) {
        return ret;
      } else {
        set_pgd(pgd, mk_kernel_pgd((gpfn << PAGE_SHIFT)));
        pud_dir = (pud_t*)page;
      }  // ept_alloc_pt_item
    }  // !create
  } else {
    ret = ept_gpa_to_hva(vcpu, pgd_val(*pgd)&PTE_PFN_MASK,
                         (unsigned long*)(&pud_dir));
    if (ret)
      return ret;
  }  // !pgd_present

  pud = pud_dir + pud_index(gva);
  if (!pud_present(*pud)) {
    if (!create) {
      return -ENOENT;
    } else {
      ret = ept_alloc_pt_item(vcpu, &gpfn, &page);
      if (ret) {
        return ret;
      } else {
        set_pud(pud, __pud((gpfn << PAGE_SHIFT)|_KERNPG_TABLE));
        pmd_dir = (pmd_t*)page;
      }  // ept_alloc_pt_item
    }  // !create
  } else {
    ret = ept_gpa_to_hva(vcpu, pud_val(*pud)&PTE_PFN_MASK,
                         (unsigned long*)(&pmd_dir));
    if (ret)
      return ret;
  }  // !pud_present

  pmd = pmd_dir + pmd_index(gva);
  if (!pmd_present(*pmd)) {
    if (!create) {
      return -ENOENT;
    } else {
      ret = ept_alloc_pt_item(vcpu, &gpfn, &page);
      if (ret) {
        return ret;
      } else {
        set_pmd(pmd, __pmd((gpfn << PAGE_SHIFT)|_KERNPG_TABLE));
        pte_dir = (pte_t*)page;
      }  // ept_alloc_pt_item
    }  // !create
  } else {
    ret = ept_gpa_to_hva(vcpu, pmd_val(*pmd)&PTE_PFN_MASK,
                         (unsigned long*)(&pte_dir));
    if (ret)
      return ret;
  }  // !pmd_present

  pte = pte_dir + pte_index(gva);
  if (!pte_present(*pte) || overwrite) {
    set_pte(pte, __pte((gpa & PTE_PFN_MASK)|__PAGE_KERNEL_EXEC));
  } else {
    printk(KERN_ERR "mm: pte conflicts %p %p\n", (void*)gpa, (void*)(*pte));
    ret = -EINVL;
  }

  return ret;  
}

static void vmx_free_ept(unsigned long ept_root) {
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


int vmx_init_ept(struct vmx_vcpu *vcpu) {
  void *page = (void *) __get_free_page(GFP_KERNEL);

  if (!page)
    return -ENOMEM;

  memset(page, 0, PAGE_SIZE);
  vcpu->ept_root =  __pa(page);

  return 0;
}

static u64 construct_eptp(unsigned long root_hpa) {
  u64 eptp;

  eptp = VMX_EPT_DEFAULT_MT |
      VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT;
  if (cpu_has_vmx_ept_ad_bits())
    eptp |= VMX_EPT_AD_ENABLE_BIT;
  eptp |= (root_hpa & PAGE_MASK);

  return eptp;
}

static int vmx_setup_initial_page_table(struct vmx_vcpu *vcpu) {
  int ret = 0;
  int i;

  unsigned long gpa, hpa, gva;
  /* Map guest page table structure's pages in ept  */
  for (i = 0, gpa = PT_PAGES_START;
       i < NR_PT_PAGES, gpa+=PAGE_SIZE; ++i) {
    hpa = __get_free_page(GFP_KERNEL);
    if (!hpa)
      return -ENOMEM;

    /* No overwriting, should not exist at all. */
    ret = ept_set_epte(vcpu, gpa, hpa, 0);
    if (ret) {
      printk(KERN_ERR "ept: map pt pages failed at %d\n", i);
      return ret;
    }
  }

  gpa = gva = PT_PAGES_START;
  /* Populate at least a page table path, 4 pages. */
  for (i = 0; i < 4; ++i) {
    ret = map_gva_to_gpa(vcpu, gva, gpa, 1, 0);
    if (ret) {
      printk(KERN_ERR "ept: populate pt failed at %d\n", i);
      return ret;
    }
    gva += PAGE_SIZE;
    gpa += PAGE_SIZE;
  }
}

static int vmx_create_ept(struct vmx_vcpu *vcpu)
{
  int ret;
  ret = vmx_setup_initial_page_table(vcpu);
  return ret;
}


/* VMCS Related */

static unsigned long segment_base(u16 selector) {
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

  if (d->s == 0 && (d->type == 2 || d->type == 9 || d->type == 11))
    v |= ((unsigned long)((struct ldttss_desc64 *)d)->base3) << 32;

  return v;
}

static inline unsigned long vmx_read_tr_base(void) {
  u16 tr;
  asm("str %0" : "=g"(tr));
  return segment_base(tr);
}

static void __vmx_setup_cpu(void) {
  struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
  unsigned long sysenter_esp;
  unsigned long tmpl;

  /*
   * Linux uses per-cpu TSS and GDT, so set these when switching
   * processors.
   */
  vmcs_writel(HOST_TR_BASE, vmx_read_tr_base()); /* 22.2.4 */
  vmcs_writel(HOST_GDTR_BASE, gdt->address);   /* 22.2.4 */

  /* No sysenter should be called */
  rdmsrl(MSR_IA32_SYSENTER_ESP, 0);
  vmcs_writel(HOST_IA32_SYSENTER_ESP, 0); /* 22.2.3 */

  rdmsrl(MSR_FS_BASE, tmpl);
  vmcs_writel(HOST_FS_BASE, tmpl); /* 22.2.4 */
  rdmsrl(MSR_GS_BASE, tmpl);
  vmcs_writel(HOST_GS_BASE, tmpl); /* 22.2.4 */
}

static void __vmx_get_cpu_helper(void *ptr) {
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
static void vmx_get_cpu(struct vmx_vcpu *vcpu) {
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
static void vmx_put_cpu(struct vmx_vcpu *vcpu) {
  put_cpu();
}


static __init int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt,
                                      u32 msr, u32 *result) {
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

static void vmx_dump_cpu(struct vmx_vcpu *vcpu) {
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

static __init int setup_vmcs_config(struct vmcs_config *vmcs_conf) {
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
  vmcs_writel(GUEST_GDTR_BASE, LCD_GDT_ADDR);
  vmcs_writel(GUEST_GDTR_LIMIT, GDT_SIZE);
  vmcs_writel(GUEST_IDTR_BASE, LCD_IDT_ADDR);
  vmcs_writel(GUEST_IDTR_LIMIT, IDT_ENTRIES*16);
  vmcs_writel(GUEST_RIP, LCD_CODE_START);
  vmcs_writel(GUEST_RSP, LCD_STACK_START);
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
  vmcs_write16(GUEST_CS_SELECTOR, 2); // code
  vmcs_write16(GUEST_DS_SELECTOR, 3); // data
  vmcs_write16(GUEST_ES_SELECTOR, 0);
  vmcs_write16(GUEST_FS_SELECTOR, 0);
  vmcs_write16(GUEST_GS_SELECTOR, 0);
  vmcs_write16(GUEST_SS_SELECTOR, 3); // data
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

static void set_gdt_entry(struct desc_struct* desc,
                          u8 type, u8 s) {
  memset(desc, 0, sizeof (struct desc_struct));
}

static int setup_gdt(struct vmx_vcpu* vcpu) {
  memset(vcpu->gdt, 0, GDT_SIZE);
  struct desc_struct *desc = vcpu->gdt + LCD_CS;

  /* ignored fields according to APM Vol.3 Ch4.8 */
  /* code seg */
  desc->type = SEG_TYPE_CODE | SEG_TYPE_EXEC_READ;
  desc->s = DESC_TYPE_CODE_DATA;
  desc->dpl = 0;
  desc->p = 1;
  desc->l = 1;
  desc->d = 0;

  /* data seg */
  desc = vcpu->gdt + LCD_DS;
  desc->type = SEG_TYPE_DATA | SEG_TYPE_READ_WRITE;
  desc->s = DESC_TYPE_CODE_DATA;
  desc->p = 1;

  /* task segment value */
  tss_desc* tss = (tss_desc*)(vcpu->gdt + LCD_TSS);

  desc->limit0 = 0x0000;
  desc->base0 = 0x0000;
  desc->base1 = 0x0000;
  desc->type = SEG_TYPE_TSS;
  desc->s = 0;
  desc->dpl = 0;
  desc->p = 1;
  desc->limit = 0x0;
  desc->avl = 0;
  desc->l = 0;
  desc->d = 0;
  desc->g = SEG_GRANULARITY_4KB;
  desc->base2 = 0x00;
}

static struct vmx_vcpu * vmx_create_vcpu()
{
  struct vmx_vcpu *vcpu = kmalloc(sizeof(struct vmx_vcpu), GFP_KERNEL);
  if (!vcpu)
    return NULL;

  memset(vcpu, 0, sizeof(*vcpu));

  vcpu->bmp_pt_pages = kmalloc(sizeof(long) * BITS_TO_LONGS(NR_PT_PAGES));
  if (!vcpu->bmp_at_page)
    goto fail_bmp;
  bitmap_zero(vcpu->bmp_pt_pages, NR_PT_PAGES);

  vcpu->gdt = (struct desc_struct*)__get_free_page(GFP_KERNEL);
  if (!vcpu->gdt)
    goto fail_gdt;

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
  kfree(vcpu->gdt);
fail_gdt:
  kfree(vcpu->bmp_pt_pages);
fail_gmp:
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

static void __init lcd_init(void) {
}

static void __exit lcd_exit(void) {
}

module_init(lcd_init);
module_exit(lcd_exit);
