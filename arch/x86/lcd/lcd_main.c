/*
 * LCD main source file.
 *
 * Based on KVM and Dune.
 *
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/tboot.h>
#include <linux/types.h>

#include <uapi/asm/kvm.h>

#include <asm/vmx.h>
#include <asm/processor.h>
#include <asm/desc.h>
#include <asm/virtext.h>

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <asm/segment.h>
#include <asm/tsc.h>
#include <asm/uaccess.h>

#include <uapi/linux/limits.h>
static char target_mod_filepath[PATH_MAX];
module_param_string(mod_file, target_mod_filepath,
                    sizeof(target_mod_filepath), 0644);
MODULE_PARM_DESC(mod_file, "Filepath of the target module to"
                 " run inside LCD");

/* #include "lcd.h" */
#include "lcd_defs.h"

MODULE_AUTHOR("Weibin Sun");
MODULE_LICENSE("GPL");

struct vmx_capability vmx_capability;

static atomic_t vmx_enable_failed;

static DECLARE_BITMAP(vmx_vpid_bitmap, VMX_NR_VPIDS);
static DEFINE_SPINLOCK(vmx_vpid_lock);

static DEFINE_PER_CPU(struct vmcs *, vmxarea);
static DEFINE_PER_CPU(struct desc_ptr, host_gdt);
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(lcd_struct *, local_vcpu);

static unsigned long *msr_bitmap;

struct vmcs_config vmcs_config;


static cycles_t g_prepare_start, g_prepare_time;

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

static inline void __invept(int ext, u64 eptp, u64 gpa) {
  struct {
    u64 eptp, gpa;
  } operand = {eptp, gpa};

  asm volatile (ASM_VMX_INVEPT
                /* CF==1 or ZF==1 --> rc = -1 */
                "; ja 1f ; ud2 ; 1:\n"
                : : "a" (&operand), "c" (ext) : "cc", "memory");
}

static inline void ept_sync_global(void) {
  if (cpu_has_vmx_invept_global())
    __invept(VMX_EPT_EXTENT_GLOBAL, 0, 0);
}

static inline void ept_sync_context(u64 eptp) {
  if (cpu_has_vmx_invept_context())
    __invept(VMX_EPT_EXTENT_CONTEXT, eptp, 0);
  else
    ept_sync_global();
}

static inline void ept_sync_individual_addr(u64 eptp, u64 gpa) {
  if (cpu_has_vmx_invept_individual_addr())
    __invept(VMX_EPT_EXTENT_INDIVIDUAL_ADDR,
             eptp, gpa);
  else
    ept_sync_context(eptp);
}

static inline void __invvpid(int ext, u16 vpid, u64 gva) {
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

static bool allow_1_setting(u32 msr, u32 ctl) {
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

static inline bool is_external_interrupt(u32 intr_info) {
  return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VALID_MASK))
      == (INTR_TYPE_EXT_INTR | INTR_INFO_VALID_MASK);
}

/* Memory Management */

static inline bool is_page_fault(u32 intr_info) {
  return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK |
                       INTR_INFO_VALID_MASK)) ==
      (INTR_TYPE_HARD_EXCEPTION | PF_VECTOR | INTR_INFO_VALID_MASK);
}

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

static inline int epte_big(epte_t epte) {
  return (epte & __EPTE_SZ) > 0;
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

static int ept_lookup_gpa(lcd_struct *vcpu,
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

static void __set_epte(lcd_struct *vcpu,
                       epte_t* epte, u64 hpa) {
  epte_t flags = __EPTE_READ | __EPTE_EXEC | __EPTE_WRITE |
      __EPTE_TYPE(EPTE_TYPE_WB) | __EPTE_IPAT;

  if (vcpu->ept_ad_enabled) {
    /* premark A/D to avoid extra memory references */
    flags |= __EPTE_A | __EPTE_D;
  }

  *epte = epte_addr(hpa) | flags;
}

static int ept_set_epte(lcd_struct *vcpu,
                        u64 gpa,
                        u64 hpa,
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
      clear_epte(epte);
    else {
      spin_unlock(&vcpu->ept_lock);
      printk(KERN_ERR "ept: epte exists %p\n", (void*)(*epte));
      return -EINVAL;
    }
  }

  __set_epte(vcpu, epte, hpa);
  
  spin_unlock(&vcpu->ept_lock);

  return 0;
}

static unsigned long alloc_pt_pfn(lcd_struct *vcpu) {
  unsigned long which = bitmap_find_next_zero_area(
      vcpu->bmp_pt_pages,
      LCD_NR_PT_PAGES,
      0, 1, 0);
  if (which >= LCD_NR_PT_PAGES) {
    return 0;
  } else {
    bitmap_set(vcpu->bmp_pt_pages, which, 1);
    return (LCD_PT_PAGES_START >> PAGE_SHIFT) + which;
  }                               
}

/*
static int free_pt_pfn(lcd_struct *vcpu,
                       unsigned long pfn) {
  unsigned long which = pfn - (PT_PAGES_START >> PAGE_SHIFT);
  if (which >= (PT_PAGES_END >> PAGE_SHIFT)) {
    return -EINVAL; 
  } else {
    bitmap_clear(vcpu->bmp_pt_pages, which, 1);
    return 0;
  }
}
*/

static int ept_alloc_pt_item(lcd_struct *vcpu,
                             unsigned long *gpfn,
                             unsigned long *page) {
  int ret = 0;
  *gpfn = alloc_pt_pfn(vcpu);
  if (*gpfn == 0) {
    ret = -ENOMEM;
  } else {
    epte_t *epte;
    ret = ept_lookup_gpa(vcpu, (void*)((*gpfn) << PAGE_SHIFT), 1, &epte);
    if (!ret) {
      if (!epte_present(*epte)) {
        *page = __get_free_page(GFP_ATOMIC);
        if (!(*page)) {
          ret = -ENOMEM;
        } else {
          memset((void*)(*page), 0, PAGE_SIZE);
          __set_epte(vcpu, epte, __pa(*page));
        }
      } else {
        *page = (unsigned long)epte_page_vaddr(*epte);
      }
    }
  }
  return ret;
}

static int ept_gpa_to_hva(lcd_struct* vcpu,
                          u64 gpa,
                          u64* hva) {
  epte_t *epte;
  int ret = ept_lookup_gpa(vcpu, (void*)gpa, 0, &epte);
  if (ret) {
    printk(KERN_ERR "ept: failed to lookup GPA: %p\n",
           (void*)gpa);
  } else {
    if (!epte_present(*epte)) {
      printk(KERN_ERR "ept: epte not present when translating\n");
      ret = -ENOENT;
    } else {
      *hva = (u64)epte_page_vaddr(*epte);
    }
  }
  return ret;
}

// Set gva => gpa. No page allocated for gva/gpa, just
// pte value setting. But page table structures along the
// path can be created by setting 'create' to 1.
static int map_gva_to_gpa(lcd_struct *vcpu,
                          u64 gva,
                          u64 gpa,
                          int create, int overwrite) {
  int ret = 0;
  unsigned long gpfn, page;
  /* epte_t *epte; */
  
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
                         (u64*)(&pud_dir));
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
                         (u64*)(&pmd_dir));
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
                         (u64*)(&pte_dir));
    if (ret)
      return ret;
  }  // !pmd_present

  pte = pte_dir + pte_index(gva);
  if (!pte_present(*pte) || overwrite) {
    set_pte(pte, __pte((gpa & PTE_PFN_MASK)|__PAGE_KERNEL_EXEC));
  } else {
    printk(KERN_ERR "mm: pte conflicts %p %p\n", (void*)gpa,
           (void*)pte_val(*pte));
    ret = -EINVAL;
  }

  return ret;  
}

static void vmx_free_ept(u64 ept_root) {
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
        } // PTE loop

        free_page((unsigned long) pte);
      } // PMD loop

      free_page((unsigned long) pmd);
    } // PUD loop

    free_page((unsigned long) pud);
  } // PGD loop

  free_page((unsigned long) pgd);
}


int vmx_init_ept(lcd_struct *vcpu) {
  void *page = (void *) __get_free_page(GFP_KERNEL);

  if (!page)
    return -ENOMEM;

  memset(page, 0, PAGE_SIZE);
  vcpu->ept_root =  __pa(page);

  return 0;
}

static u64 construct_eptp(u64 root_hpa) {
  u64 eptp;

  eptp = VMX_EPT_DEFAULT_MT |
      VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT;
  if (cpu_has_vmx_ept_ad_bits())
    eptp |= VMX_EPT_AD_ENABLE_BIT;
  eptp |= (root_hpa & PAGE_MASK);

  return eptp;
}

static int vmx_setup_initial_page_table(lcd_struct *vcpu) {
  int ret = 0;
  int i;

  u64 gpa, hpa, gva;
  /* Map guest page table structure's pages in ept  */
  for (i = 0, gpa = LCD_PT_PAGES_START;
       i < LCD_NR_PT_PAGES; gpa+=PAGE_SIZE, ++i) {
    hpa = __get_free_page(GFP_KERNEL);
    if (!hpa)
      return -ENOMEM;
    memset((void*)hpa, 0, PAGE_SIZE);
    hpa = __pa(hpa);

    /* No overwriting, should not exist at all. */
    ret = ept_set_epte(vcpu, gpa, hpa, 0);
    if (ret) {
      printk(KERN_ERR "ept: map pt pages failed at %d\n", i);
      return ret;
    }
  }

  gpa = gva = LCD_PT_PAGES_START;
  /* Populate at least a page table path, 4 pages. */
  for (i = 0; i < LCD_NR_PT_PAGES; ++i) {
    ret = map_gva_to_gpa(vcpu, gva, gpa, 1, 0);
    if (ret) {
      printk(KERN_ERR "ept: populate pt failed at %d\n", i);
      return ret;
    }
    gva += PAGE_SIZE;
    gpa += PAGE_SIZE;
  }

  /* Test code page */
  gpa = gva = LCD_TEST_CODE_ADDR;
  ret = map_gva_to_gpa(vcpu, gva, gpa, 1, 0);
  if (ret) {
    printk(KERN_ERR "ept: populate code page failed\n");
    return ret;
  }
  hpa = __get_free_page(GFP_KERNEL);
  if (!hpa)
    return -ENOMEM;
  memset((void*)hpa, 0, PAGE_SIZE);
  {
    u8* bincode = (u8*)hpa;
    /* 0: b8 e0 0f 00 01       mov    $0x1000fe0,%eax*/
    /* 5: c7 00 ef be ad de    movl   $0xdeadbeef,(%rax) */
    /* ((u8*)hpa)[0] = 0xb8; ((u8*)hpa)[1] = 0xe0; ((u8*)hpa)[2] = 0xf; ((u8*)hpa)[3] = 0x10; */
    /* ((u8*)hpa)[5] = 0xc7; ((u8*)hpa)[7] = 0xef; ((u8*)hpa)[8] = 0xbe; ((u8*)hpa)[9] = 0xad; */
    /* ((u8*)hpa)[10] = 0xde; */
    /* ((u8*)hpa)[11] = 0xf4; ((u8*)hpa)[12] = 0xf4; */
    
    bincode[0] = 0xf;  bincode[1] = 0x1; bincode[2] = 0xc1;
    bincode[3] = 0xb8; bincode[4] = 0x34; bincode[5] = 0x12;
    bincode[8] = 0x48; bincode[9] = 0x89; bincode[10] = 0xc1;
    bincode[11] = 0xeb; bincode[12] = 0xf6;
    bincode[13] = 0x90;
    bincode[14] = 0xf4;
  }
  
  hpa = __pa(hpa);
  
  ret = ept_set_epte(vcpu, gpa, hpa, 0);
  if (ret) {
    printk(KERN_ERR "ept: map code page failed\n");
    return ret;
  }

  /* Map stack PT */
  gpa = gva = LCD_STACK_BOTTOM;
  for (i = 0; i < (LCD_STACK_SIZE >> PAGE_SHIFT); ++i) {
    ret = map_gva_to_gpa(vcpu, gva, gpa, 1, 0);
    if (ret) {
      printk(KERN_ERR "ept: populate stack pt failed at %d\n", i);
      return ret;
    }
    hpa = __get_free_page(GFP_KERNEL);
    if (!hpa)
      return -ENOMEM;
    memset((void*)hpa, 0, PAGE_SIZE);
    hpa = __pa(hpa);

    ret = ept_set_epte(vcpu, gpa, hpa, 0);
    if (ret) {
      printk(KERN_ERR "ept: map stack pages failed at %d\n", i);
      return ret;
    }

    gva += PAGE_SIZE;
    gpa += PAGE_SIZE;
  }
  
  /* Map descriptors and tables in EPT */
  ret = ept_set_epte(vcpu, LCD_GDT_ADDR, __pa(vcpu->gdt), 0);
  if (ret) {
    printk(KERN_ERR "ept: GDT phy-addr occupied in EPT\n");
    return ret;
  }

  ret = ept_set_epte(vcpu, LCD_IDT_ADDR, __pa(vcpu->idt), 0);
  if (ret) {
    printk(KERN_ERR "ept: IDT phy-addr occupied in EPT\n");
    return ret;
  }

  ret = ept_set_epte(vcpu, LCD_TSS_ADDR, __pa(vcpu->tss), 0);
  if (ret) {
    printk(KERN_ERR "ept: TSS phy-addr occupied in EPT\n");
    return ret;
  }

  ret = ept_set_epte(vcpu, LCD_COMM_ISR_ADDR, __pa(vcpu->isr_page), 0);
  if (ret) {
    printk(KERN_ERR "ept: common ISR phy-addr occupied in EPT\n");
    return ret;
  }

  ret = map_gva_to_gpa(vcpu, LCD_GDT_ADDR, LCD_GDT_ADDR, 1, 0);
  if (ret) {
    printk(KERN_ERR "ept: GDT virt-addr occupied in guest PT\n");
    return ret;
  }

  ret = map_gva_to_gpa(vcpu, LCD_IDT_ADDR, LCD_IDT_ADDR, 1, 0);
  if (ret) {
    printk(KERN_ERR "ept: IDT virt-addr occupied in guest PT\n");
    return ret;
  }

  ret = map_gva_to_gpa(vcpu, LCD_TSS_ADDR, LCD_TSS_ADDR, 1, 0);
  if (ret) {
    printk(KERN_ERR "ept: TSS virt-addr occupied in guest PT\n");
    return ret;
  }

  ret = map_gva_to_gpa(vcpu, LCD_COMM_ISR_ADDR, LCD_COMM_ISR_ADDR, 1, 0);
  if (ret) {
    printk(KERN_ERR "ept: common ISR virt-addr occupied in guest PT\n");
    return ret;
  }

  return 0;
}

static int vmx_create_ept(lcd_struct *vcpu) {
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
  rdmsrl(MSR_IA32_SYSENTER_ESP, sysenter_esp);
  vmcs_writel(HOST_IA32_SYSENTER_ESP, sysenter_esp); /* 22.2.3 */

  rdmsrl(MSR_FS_BASE, tmpl);
  vmcs_writel(HOST_FS_BASE, tmpl); /* 22.2.4 */
  rdmsrl(MSR_GS_BASE, tmpl);
  vmcs_writel(HOST_GS_BASE, tmpl); /* 22.2.4 */
}

static void __vmx_get_cpu_helper(void *ptr) {
  lcd_struct *vcpu = ptr;

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
static void vmx_get_cpu(lcd_struct *vcpu) {
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
static void vmx_put_cpu(lcd_struct *vcpu) {
  put_cpu();
}


static int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt,
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

static void vmx_dump_cpu(lcd_struct *vcpu) {
  unsigned long flags;
  u32 inst_len;

  vmx_get_cpu(vcpu);
  vcpu->regs[VCPU_REGS_RIP] = vmcs_readl(GUEST_RIP);
  vcpu->regs[VCPU_REGS_RSP] = vmcs_readl(GUEST_RSP);
  flags = vmcs_readl(GUEST_RFLAGS);
  inst_len = vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
  vmx_put_cpu(vcpu);

  printk(KERN_INFO "vmx: --- Begin VCPU Dump ---\n");
  printk(KERN_INFO "vmx: CPU %d VPID %d\n", vcpu->cpu, vcpu->vpid);
  printk(KERN_INFO "vmx: RIP 0x%016llx Instruction length %u\n",
         vcpu->regs[VCPU_REGS_RIP], inst_len);
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
  printk(KERN_INFO "vmx: RFLAGS 0x%08lx CR2 0x%016llx\n",
         flags, vcpu->cr2);
  printk(KERN_INFO "vmx: eq: 0x%016llx, intr_info: 0x%08x\n",
         vcpu->exit_qualification, vcpu->exit_intr_info);
  printk(KERN_INFO "vmx: idt: 0x%08x\n",
         vcpu->idt_vectoring_info);
  printk(KERN_INFO "vmx: --- End VCPU Dump ---\n");
}

static int setup_vmcs_config(struct vmcs_config *vmcs_conf) {
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
     ### - (No) MSR bitmap
   */  
  min = CPU_BASED_HLT_EXITING |
      CPU_BASED_INVLPG_EXITING |
      CPU_BASED_MWAIT_EXITING |
      CPU_BASED_RDPMC_EXITING |
      CPU_BASED_CR8_LOAD_EXITING |
      CPU_BASED_CR8_STORE_EXITING |
      CPU_BASED_MOV_DR_EXITING |
      CPU_BASED_UNCOND_IO_EXITING |
      CPU_BASED_MONITOR_EXITING;

  opt = CPU_BASED_ACTIVATE_SECONDARY_CONTROLS |
      CPU_BASED_USE_MSR_BITMAPS;
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
    printk(KERN_ERR "lcd: secondary VME controls not supported\n");
    return -EIO;
  }

  // We need EPT.
  if (_cpu_based_2nd_exec_control & SECONDARY_EXEC_ENABLE_EPT) {
    rdmsr(MSR_IA32_VMX_EPT_VPID_CAP,
          vmx_capability.ept, vmx_capability.vpid);
  } else {
    printk(KERN_ERR "lcd: EPT not supported\n");
    return -EIO;
  }

  // Pin-based control:
  min = PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING;
  opt = 0;
  if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PINBASED_CTLS,
                          &_pin_based_exec_control) < 0)
    return -EIO;


  // Exit control:
  min = VM_EXIT_HOST_ADDR_SPACE_SIZE//;
      | VM_EXIT_ACK_INTR_ON_EXIT;
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

static struct vmcs *vmx_alloc_vmcs(int cpu) {
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

static void vmx_free_vmcs(struct vmcs *vmcs) {
  free_pages((unsigned long)vmcs, vmcs_config.order);
}

static void vmx_setup_constant_host_state(lcd_struct *lcd) {
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
  lcd->host_idt_base = dt.address;

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

static void vmx_disable_intercept_for_msr(
    unsigned long *msr_bitmap, u32 msr) {
  int f = sizeof(unsigned long);
  /*
   * See Intel PRM Vol. 3, 20.6.9 (MSR-Bitmap Address). Early manuals
   * have the write-low and read-high bitmap offsets the wrong way round.
   * We can control MSRs 0x00000000-0x00001fff and 0xc0000000-0xc0001fff.
   */
  if (msr <= 0x1fff) {
    __clear_bit(msr, msr_bitmap + 0x000 / f); /* read-low */
    __clear_bit(msr, msr_bitmap + 0x800 / f); /* write-low */
  } else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
    msr &= 0x1fff;
    __clear_bit(msr, msr_bitmap + 0x400 / f); /* read-high */
    __clear_bit(msr, msr_bitmap + 0xc00 / f); /* write-high */
  }
}

static void setup_msr(lcd_struct *vcpu) {
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

static void vmx_setup_vmcs(lcd_struct *vcpu)
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

  vmcs_write32(EXCEPTION_BITMAP, 0xffffffff);
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

  vmx_setup_constant_host_state(vcpu);
}

/**
 * vmx_allocate_vpid - reserves a vpid and sets it in the VCPU
 * @vmx: the VCPU
 */
static int vmx_allocate_vpid(lcd_struct *vmx)
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
static void vmx_free_vpid(lcd_struct *vmx)
{
  spin_lock(&vmx_vpid_lock);
  if (vmx->vpid != 0)
    __clear_bit(vmx->vpid, vmx_vpid_bitmap);
  spin_unlock(&vmx_vpid_lock);
}

static void vmx_setup_initial_guest_state(lcd_struct *vcpu)
{
  /* unsigned long tmpl; */
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
  vmcs_writel(GUEST_CR3, vcpu->pt_gpa);
  vmcs_writel(GUEST_CR4, cr4);
  vmcs_writel(CR4_READ_SHADOW, cr4);
  vmcs_writel(GUEST_IA32_EFER, EFER_LME | EFER_LMA |
              EFER_SCE | EFER_FFXSR);
  vmcs_writel(GUEST_GDTR_BASE, LCD_GDT_ADDR);
  vmcs_writel(GUEST_GDTR_LIMIT, GDT_SIZE);
  vmcs_writel(GUEST_IDTR_BASE, LCD_IDT_ADDR);
  vmcs_writel(GUEST_IDTR_LIMIT, IDT_ENTRIES*16);
  vmcs_writel(GUEST_RIP, LCD_TEST_CODE_ADDR);
  vmcs_writel(GUEST_RSP, LCD_STACK_ADDR);
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
  vmcs_write16(GUEST_CS_SELECTOR, __KERNEL_CS); // code
  vmcs_write16(GUEST_DS_SELECTOR, __KERNEL_DS); // data
  vmcs_write16(GUEST_ES_SELECTOR, 0);
  vmcs_write16(GUEST_FS_SELECTOR, 0);
  vmcs_write16(GUEST_GS_SELECTOR, 0);
  vmcs_write16(GUEST_SS_SELECTOR, 0); // data
  vmcs_write16(GUEST_TR_SELECTOR, 0);

  /* guest LDTR */
  vmcs_write16(GUEST_LDTR_SELECTOR, 0);
  vmcs_writel(GUEST_LDTR_AR_BYTES, 0x0082);
  vmcs_writel(GUEST_LDTR_BASE, 0);
  vmcs_writel(GUEST_LDTR_LIMIT, 0);

  /* guest TSS */
  vmcs_writel(GUEST_TR_BASE, LCD_TSS_ADDR);
  vmcs_writel(GUEST_TR_AR_BYTES, 0x0080 | AR_TYPE_BUSY_64_TSS);
  vmcs_writel(GUEST_TR_LIMIT, LCD_TSS_SIZE);

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

static int setup_gdt(lcd_struct* vcpu) {
  struct desc_struct *desc;
  tss_desc* tssd;
  struct x86_hw_tss* tss;

  memset(vcpu->gdt, 0, GDT_SIZE);

  desc = vcpu->gdt + GDT_ENTRY_KERNEL_CS;
  /* ignored fields according to APM Vol.3 Ch4.8 */
  /* code seg desc */
  desc->type = SEG_TYPE_CODE | SEG_TYPE_EXEC_READ;
  desc->s = DESC_TYPE_CODE_DATA;
  desc->dpl = 0;
  desc->p = 1;
  desc->l = 1;
  desc->d = 0;

  /* data seg desc */
  desc = vcpu->gdt + GDT_ENTRY_KERNEL_DS;
  desc->type = SEG_TYPE_DATA | SEG_TYPE_READ_WRITE;
  desc->s = DESC_TYPE_CODE_DATA;
  desc->p = 1;

  /* task segment desc value */
  tssd = (tss_desc*)(vcpu->gdt + GDT_ENTRY_TSS);
  set_tssldt_descriptor(tssd, LCD_TSS_ADDR, DESC_TSS, LCD_TSS_SIZE);

  /* TSS segment */
  memset(vcpu->tss, 0, PAGE_SIZE);
  tss = &vcpu->tss->tss;
  tss->sp0 = LCD_STACK_ADDR;
  tss->io_bitmap_base = offsetof(struct lcd_tss_struct, io_bitmap);
  tss->ist[0] = LCD_TSS_ADDR + (PAGE_SIZE>>1);
  vcpu->tss->io_bitmap[0] = 0xff;

  return 0;
}

static void setup_idt(lcd_struct* vcpu) {
  int i;
  memset(vcpu->idt, 0, PAGE_SIZE);
  /* Just fill the IDT  */
  for (i = 0; i < IDT_ENTRIES; ++i) {
    gate_desc* gate = vcpu->idt + i;
    pack_gate(gate, GATE_INTERRUPT, LCD_COMM_ISR_ADDR, 0, 1, __KERNEL_CS);
  }
}

static lcd_struct * vmx_create_vcpu(void) {
  lcd_struct *vcpu = kmalloc(sizeof(lcd_struct), GFP_KERNEL);
  if (!vcpu)
    return NULL;

  memset(vcpu, 0, sizeof(*vcpu));

  vcpu->bmp_pt_pages = kmalloc(
      sizeof(long) * BITS_TO_LONGS(LCD_NR_PT_PAGES),
      GFP_KERNEL);
  if (!vcpu->bmp_pt_pages)
    goto fail_bmp;
  bitmap_zero(vcpu->bmp_pt_pages, LCD_NR_PT_PAGES);

  vcpu->gdt = (struct desc_struct*)__get_free_page(GFP_KERNEL);
  if (!vcpu->gdt)
    goto fail_gdt;

  vcpu->idt = (gate_desc*)__get_free_page(GFP_KERNEL);
  if (!vcpu->idt)
    goto fail_idt;

  vcpu->tss = (struct lcd_tss_struct*)__get_free_page(GFP_KERNEL);
  if (!vcpu->tss)
    goto fail_tss;

  vcpu->isr_page = __get_free_page(GFP_KERNEL);
  if (!vcpu->isr_page)
    goto fail_isr;
  memset((void*)vcpu->isr_page, 0, PAGE_SIZE);
  *((u8*)vcpu->isr_page) = 0xf4; //HLT

  vcpu->vmcs = vmx_alloc_vmcs(raw_smp_processor_id());
  if (!vcpu->vmcs)
    goto fail_vmcs;

  if (vmx_allocate_vpid(vcpu))
    goto fail_vpid;

  vcpu->cpu = -1;

  spin_lock_init(&vcpu->ept_lock);
  if (vmx_init_ept(vcpu))
    goto fail_ept;
  vcpu->eptp = construct_eptp(vcpu->ept_root);

  if (cpu_has_vmx_ept_ad_bits()) {
    vcpu->ept_ad_enabled = true;
    printk(KERN_INFO "vmx: enabled EPT A/D bits");
  }
  if (vmx_create_ept(vcpu)) {
    printk(KERN_ERR "vmx: creating EPT failed.");
    goto fail_ept;
  }

  vmx_get_cpu(vcpu);
  vmx_setup_vmcs(vcpu);
  vmx_setup_initial_guest_state(vcpu);
  setup_gdt(vcpu);
  setup_idt(vcpu);
  vmx_put_cpu(vcpu);

  return vcpu;

fail_ept:
  vmx_free_vpid(vcpu);
fail_vpid:
  vmx_free_vmcs(vcpu->vmcs);
fail_vmcs:
  kfree((void*)vcpu->isr_page);
fail_isr:
  kfree(vcpu->tss);
fail_tss:
  kfree(vcpu->idt);
fail_idt:
  kfree(vcpu->gdt);
fail_gdt:
  kfree(vcpu->bmp_pt_pages);
fail_bmp:
  kfree(vcpu);

  return NULL;
}

static void vmx_destroy_vcpu(lcd_struct *vcpu)
{
  vmx_free_ept(vcpu->ept_root);
  vmx_get_cpu(vcpu);
  ept_sync_context(vcpu->eptp);
  vmcs_clear(vcpu->vmcs);
  __get_cpu_var(local_vcpu) = NULL;
  vmx_put_cpu(vcpu);
  vmx_free_vpid(vcpu);
  vmx_free_vmcs(vcpu->vmcs);
  kfree(vcpu->bmp_pt_pages);
  kfree(vcpu);
}

static int __noclone vmx_run_vcpu(lcd_struct *vcpu) {
  asm(
      /* Store host registers */
      "push %%rdx; push %%rbp;"
      "push %%rcx \n\t" /* placeholder for guest rcx */
      "push %%rcx \n\t"
      "cmp %%rsp, %c[host_rsp](%0) \n\t"
      "je 1f \n\t"
      "mov %%rsp, %c[host_rsp](%0) \n\t"
      ASM_VMX_VMWRITE_RSP_RDX "\n\t"
      "1: \n\t"
      /* Reload cr2 if changed */
      "mov %c[cr2](%0), %%rax \n\t"
      "mov %%cr2, %%rdx \n\t"
      "cmp %%rax, %%rdx \n\t"
      "je 2f \n\t"
      "mov %%rax, %%cr2 \n\t"
      "2: \n\t"
      /* Check if vmlaunch of vmresume is needed */
      "cmpl $0, %c[launched](%0) \n\t"
      /* Load guest registers.  Don't clobber flags. */
      "mov %c[rax](%0), %%rax \n\t"
      "mov %c[rbx](%0), %%rbx \n\t"
      "mov %c[rdx](%0), %%rdx \n\t"
      "mov %c[rsi](%0), %%rsi \n\t"
      "mov %c[rdi](%0), %%rdi \n\t"
      "mov %c[rbp](%0), %%rbp \n\t"
      "mov %c[r8](%0),  %%r8  \n\t"
      "mov %c[r9](%0),  %%r9  \n\t"
      "mov %c[r10](%0), %%r10 \n\t"
      "mov %c[r11](%0), %%r11 \n\t"
      "mov %c[r12](%0), %%r12 \n\t"
      "mov %c[r13](%0), %%r13 \n\t"
      "mov %c[r14](%0), %%r14 \n\t"
      "mov %c[r15](%0), %%r15 \n\t"
      "mov %c[rcx](%0), %%rcx \n\t" /* kills %0 (ecx) */

      /* Enter guest mode */
      "jne .Llaunched \n\t"
      ASM_VMX_VMLAUNCH "\n\t"
      "jmp .Lvmx_return \n\t"
      ".Llaunched: " ASM_VMX_VMRESUME "\n\t"
      ".Lvmx_return: "
      /* Save guest registers, load host registers, keep flags */
      "mov %0, %c[wordsize](%%rsp) \n\t"
      "pop %0 \n\t"
      "mov %%rax, %c[rax](%0) \n\t"
      "mov %%rbx, %c[rbx](%0) \n\t"
      "popq %c[rcx](%0) \n\t"
      "mov %%rdx, %c[rdx](%0) \n\t"
      "mov %%rsi, %c[rsi](%0) \n\t"
      "mov %%rdi, %c[rdi](%0) \n\t"
      "mov %%rbp, %c[rbp](%0) \n\t"
      "mov %%r8,  %c[r8](%0) \n\t"
      "mov %%r9,  %c[r9](%0) \n\t"
      "mov %%r10, %c[r10](%0) \n\t"
      "mov %%r11, %c[r11](%0) \n\t"
      "mov %%r12, %c[r12](%0) \n\t"
      "mov %%r13, %c[r13](%0) \n\t"
      "mov %%r14, %c[r14](%0) \n\t"
      "mov %%r15, %c[r15](%0) \n\t"
      "mov %%rax, %%r10 \n\t"
      "mov %%rdx, %%r11 \n\t"

      "mov %%cr2, %%rax   \n\t"
      "mov %%rax, %c[cr2](%0) \n\t"

      "pop  %%rbp; pop  %%rdx \n\t"
      "setbe %c[fail](%0) \n\t"

      "mov $" __stringify(__USER_DS) ", %%rax \n\t"
      "mov %%rax, %%ds \n\t"
      "mov %%rax, %%es \n\t"
      : : "c"(vcpu), "d"((unsigned long)HOST_RSP),
        [launched]"i"(offsetof(lcd_struct, launched)),
        [fail]"i"(offsetof(lcd_struct, fail)),
        [host_rsp]"i"(offsetof(lcd_struct, host_rsp)),
        [rax]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RAX])),
        [rbx]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RBX])),
        [rcx]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RCX])),
        [rdx]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RDX])),
        [rsi]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RSI])),
        [rdi]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RDI])),
        [rbp]"i"(offsetof(lcd_struct, regs[VCPU_REGS_RBP])),
        [r8]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R8])),
        [r9]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R9])),
        [r10]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R10])),
        [r11]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R11])),
        [r12]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R12])),
        [r13]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R13])),
        [r14]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R14])),
        [r15]"i"(offsetof(lcd_struct, regs[VCPU_REGS_R15])),
        [cr2]"i"(offsetof(lcd_struct, cr2)),
        [wordsize]"i"(sizeof(ulong))
      : "cc", "memory"
        , "rax", "rbx", "rdi", "rsi"
        , "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
      );

  vcpu->launched = 1;

  if (unlikely(vcpu->fail)) {
    printk(KERN_ERR "vmx: failure detected (err %x)\n",
           vmcs_read32(VM_INSTRUCTION_ERROR));
    return VMX_EXIT_REASONS_FAILED_VMENTRY;
  }

  vcpu->exit_reason = vmcs_read32(VM_EXIT_REASON);
  vcpu->exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
  vcpu->idt_vectoring_info = vmcs_read32(IDT_VECTORING_INFO_FIELD);
  vcpu->error_code = vmcs_read32(IDT_VECTORING_ERROR_CODE);
  vcpu->exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
  vcpu->vec_no = vcpu->exit_intr_info & INTR_INFO_VECTOR_MASK;

  return vcpu->exit_reason;
}

static void vmx_step_instruction(void) {
  vmcs_writel(GUEST_RIP, vmcs_readl(GUEST_RIP) +
              vmcs_read32(VM_EXIT_INSTRUCTION_LEN));
}

static int vmx_handle_ept_violation(lcd_struct *vcpu) {
  unsigned long gva, gpa;
  int ret;

  vmx_get_cpu(vcpu);
  gva = vmcs_readl(GUEST_LINEAR_ADDRESS);
  gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS);
  vmx_put_cpu(vcpu);

  if (vcpu->exit_qualification & (1 << 6)) {
    printk(KERN_ERR "EPT: GPA 0x%lx exceeds GAW!\n", gpa);
    return -EINVAL;
  }

  if (!(vcpu->exit_qualification & (1 << 7))) {
    printk(KERN_ERR "EPT: linear address is not valid, GPA: 0x%lx!\n", gpa);
    return -EINVAL;
  }

  /* TODO: fix this. Now we fail on EPT fault unconditionally.  */
  /* ret = vmx_do_ept_fault(vcpu, gpa, gva, exit_qual); */
  printk(KERN_INFO "EPT: get violation GPA 0x%lx, GVA: 0x%lx, fail now!\n",
         gpa, gva);
  ret = -EINVAL;

  if (ret) {
    printk(KERN_ERR "vmx: EPT violation failure "
           "GPA: 0x%lx, GVA: 0x%lx\n",
           gpa, gva);
    vcpu->ret_code = ((EFAULT) << 8);
    vmx_dump_cpu(vcpu);
  }

  return ret;
}

// Obsoleted, use lcd_run instead.
int vmx_launch(int64_t *ret_code) {
  int ret, done = 0;
  lcd_struct *vcpu = vmx_create_vcpu();
  if (!vcpu)
    return -ENOMEM;

  printk(KERN_ERR "vmx: created VCPU (VPID %d)\n",
         vcpu->vpid);

  while (1) {
    vmx_get_cpu(vcpu);

#if 0
    /* TODO: fix this.
     * We assume that a Dune process will always use
     * the FPU whenever it is entered, and thus we go
     * ahead and load FPU state here. The reason is
     * that we don't monitor or trap FPU usage inside
     * a Dune process.
     */
    if (!__thread_has_fpu(current))
      math_state_restore();
#endif
    
    local_irq_disable();

    if (need_resched()) {
      local_irq_enable();
      vmx_put_cpu(vcpu);
      cond_resched();
      continue;
    }

    ret = vmx_run_vcpu(vcpu);

    local_irq_enable();

    if (ret == EXIT_REASON_VMCALL ||
        ret == EXIT_REASON_CPUID) {
      vmx_step_instruction();
    }

    vmx_put_cpu(vcpu);

    if (ret == EXIT_REASON_EPT_VIOLATION) {
      done = vmx_handle_ept_violation(vcpu);
    } else {
      u64 eq = vmcs_readl(EXIT_QUALIFICATION);
      if (ret == EXIT_REASON_EXTERNAL_INTERRUPT ||
          ret == EXIT_REASON_EXCEPTION_NMI) {
        u32 intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
        printk(KERN_INFO "exception: exit reason %d: %s, intr info %x,"
               " vector %u, vetoring info %x, error code %x\n",
               ret, lcd_exit_reason(ret),
               intr_info, intr_info&INTR_INFO_VECTOR_MASK,
               vmcs_read32(IDT_VECTORING_INFO_FIELD),
               vmcs_read32(IDT_VECTORING_ERROR_CODE));
        if (is_page_fault(intr_info)) {
          printk(KERN_INFO "got page fault gva: %p %p\n",
                 (void*)vcpu->cr2, (void*)eq);
        }
      } else {
        printk(KERN_INFO "unhandled exit: reason %d: %s, "
               "exit qualification %lx\n",
               ret, lcd_exit_reason(ret), (unsigned long)eq);
      }

      vmx_dump_cpu(vcpu);
      done = 1;
    }

    if (done || vcpu->shutdown)
      break;
  }

  printk(KERN_ERR "vmx: destroying VCPU (VPID %d)\n",
         vcpu->vpid);

  {
    u32* ptr = (u32*)((u8*)vcpu->pt + 0xfe0);
    printk(KERN_INFO "LCD: verify data 0x%x\n", *ptr);
  }

  *ret_code = vcpu->ret_code;
  vmx_destroy_vcpu(vcpu);

  return 0;
}

static int __vmx_enable(struct vmcs *vmxon_buf) {
  u64 phys_addr = __pa(vmxon_buf);
  u64 old, test_bits;

  if (read_cr4() & X86_CR4_VMXE)
    return -EBUSY;

  rdmsrl(MSR_IA32_FEATURE_CONTROL, old);

  test_bits = FEATURE_CONTROL_LOCKED;
  test_bits |= FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX;
  if (tboot_enabled())
    test_bits |= FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX;

  if ((old & test_bits) != test_bits) {
    /* enable and lock */
    wrmsrl(MSR_IA32_FEATURE_CONTROL, old | test_bits);
  }
  write_cr4(read_cr4() | X86_CR4_VMXE);

  __vmxon(phys_addr);
  vpid_sync_vcpu_global();
  ept_sync_global();

  return 0;
}

#define store_gdt(g) native_store_gdt(g)

static void vmx_enable(void *unused) {
  int ret;
  struct vmcs *vmxon_buf = __get_cpu_var(vmxarea);

  if ((ret = __vmx_enable(vmxon_buf)))
    goto failed;

  __get_cpu_var(vmx_enabled) = 1;
  store_gdt(&__get_cpu_var(host_gdt));

  printk(KERN_INFO "vmx: VMX enabled on CPU %d\n",
         raw_smp_processor_id());
  return;

failed:
  atomic_inc(&vmx_enable_failed);
  printk(KERN_ERR "vmx: failed to enable VMX, err = %d\n", ret);
}

static void vmx_disable(void *unused) {
  if (__get_cpu_var(vmx_enabled)) {
    __vmxoff();
    write_cr4(read_cr4() & ~X86_CR4_VMXE);
    __get_cpu_var(vmx_enabled) = 0;
  }
}

static void vmx_free_vmxon_areas(void) {
  int cpu;

  for_each_possible_cpu(cpu) {
    if (per_cpu(vmxarea, cpu)) {
      vmx_free_vmcs(per_cpu(vmxarea, cpu));
      per_cpu(vmxarea, cpu) = NULL;
    }
  }
}

int vmx_init(void) {
  int r, cpu;

  if (!cpu_has_vmx()) {
    printk(KERN_ERR "vmx: CPU does not support VT-x\n");
    return -EIO;
  }

  if (setup_vmcs_config(&vmcs_config) < 0)
    return -EIO;

  if (!cpu_has_vmx_vpid()) {
    printk(KERN_ERR "vmx: CPU is missing required feature 'VPID'\n");
    return -EIO;
  }

  if (!cpu_has_vmx_ept()) {
    printk(KERN_ERR "vmx: CPU is missing required feature 'EPT'\n");
    return -EIO;
  }

  if (!vmx_capability.has_load_efer) {
    printk(KERN_ERR "vmx: ability to load EFER register is required\n");
    return -EIO;
  }

  msr_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
  if (!msr_bitmap) {
    return -ENOMEM;
  }

  memset(msr_bitmap, 0xff, PAGE_SIZE);
  vmx_disable_intercept_for_msr(msr_bitmap, MSR_FS_BASE);
  vmx_disable_intercept_for_msr(msr_bitmap, MSR_GS_BASE);

  set_bit(0, vmx_vpid_bitmap); /* 0 is reserved for host */

  for_each_possible_cpu(cpu) {
    struct vmcs *vmxon_buf;

    vmxon_buf = vmx_alloc_vmcs(cpu);
    if (!vmxon_buf) {
      vmx_free_vmxon_areas();
      return -ENOMEM;
    }

    per_cpu(vmxarea, cpu) = vmxon_buf;
  }

  atomic_set(&vmx_enable_failed, 0);
  if (on_each_cpu(vmx_enable, NULL, 1)) {
    printk(KERN_ERR "vmx: timeout waiting for VMX mode enable.\n");
    r = -EIO;
    goto failed1; /* sadly we can't totally recover */
  }

  if (atomic_read(&vmx_enable_failed)) {
    r = -EBUSY;
    goto failed2;
  }

  return 0;

failed2:
  on_each_cpu(vmx_disable, NULL, 1);
failed1:
  vmx_free_vmxon_areas();
  return r;
}

void vmx_exit(void)
{
  on_each_cpu(vmx_disable, NULL, 1);
  vmx_free_vmxon_areas();
  free_page((unsigned long)msr_bitmap);
}

static struct task_struct *test_thread;

static int lcd_test(void) {
  int ret;
  lcd_struct *lcd;

  printk("LCD: start launching vmx\n");
  /* ret = vmx_launch(&ret_code); */
  g_prepare_start = get_cycles();
  lcd = lcd_create();
  if (!lcd)
    return -ENOMEM;
  /* printk("LCD: create lcd VPID %d\n", lcd->vpid); */
  ret = lcd_run(lcd);
  if (ret) {
    printk(KERN_ERR "LCD: launch failure %d\n", ret);
  } else {
    printk(KERN_INFO "LCD: launch OK\n");
  }

  printk(KERN_INFO "LCD: prepare time %llu, ret code: %d\n",
         g_prepare_time, lcd->ret_code);
  lcd_destroy(lcd);
  return ret;
}

static int lcd_thread(void* d) {
  int ret = lcd_test();
  do_exit(ret);
  return 0; // avoid compiler warning
}

lcd_struct* lcd_create(void) {
  lcd_struct* lcd = vmx_create_vcpu();
  return lcd;
}
EXPORT_SYMBOL(lcd_create);

int lcd_destroy(lcd_struct* lcd) {
  vmx_destroy_vcpu(lcd);
  return 0;
}
EXPORT_SYMBOL(lcd_destroy);

static int lcd_va_to_pa(void* va, void** pa) {
  *(unsigned long*)pa = vmalloc_to_pfn(va)<<PAGE_SHIFT;
  return 0;
}

static int __move_host_mapping(lcd_struct *lcd, void* hva,
                               unsigned int size) {
  unsigned int mapped = 0;
  void *pa;
  void *va = (void*)round_down(((unsigned long)hva), PAGE_SIZE);
  int ret = 0;
  while (mapped < size) {
    ret = lcd_va_to_pa(va, &pa);
    if (ret != 0) {
      return ret;
    }

    ret = lcd_map_gpa_to_hpa(lcd, (u64)pa, (u64)pa, 0);
    if (ret != 0) {
      printk(KERN_ERR "lcd: move PA mapping conflicts\n");
      return ret;
    }
    ret = lcd_map_gva_to_gpa(lcd, (u64)va, (u64)pa, 1, 0);
    if (ret != 0) {
      printk(KERN_ERR "lcd: move PT mapping conflicts\n");
      return ret;
    }
    mapped += PAGE_SIZE;
    va += PAGE_SIZE;
  }
  return 0;
}

int lcd_move_module(lcd_struct *lcd, struct module *mod) {
  int ret;
  lcd->mod = mod;
  // lcd_va_to_pa(va, pa) // 4KB page assumption

  ret = __move_host_mapping(lcd, mod->module_init, mod->init_size);
  if (!ret) {
    ret = __move_host_mapping(lcd, mod->module_core, mod->core_size);
    if (!ret) {
      vmcs_writel(GUEST_RIP, (unsigned long)mod->init);
    }
  }

  printk(KERN_ERR "lcd: failed to move module %d\n", ret);

  return ret;
}
EXPORT_SYMBOL(lcd_move_module);

int lcd_map_gpa_to_hpa(lcd_struct *lcd, u64 gpa, u64 hpa, int overwrite) {
  return ept_set_epte(lcd, gpa, hpa, overwrite);
}
EXPORT_SYMBOL(lcd_map_gpa_to_hpa);

int lcd_map_gva_to_gpa(lcd_struct *lcd, u64 gva, u64 gpa,
                       int create, int overwrite) {
  return map_gva_to_gpa(lcd, gva, gpa, create, overwrite);
}
EXPORT_SYMBOL(lcd_map_gva_to_gpa);

int lcd_find_hva_by_gpa(lcd_struct *lcd, u64 gpa, u64 *hva) {
  return ept_gpa_to_hva(lcd, gpa, hva);
}
EXPORT_SYMBOL(lcd_find_hva_by_gpa);

static void vmx_handle_external_interrupt(lcd_struct *lcd) {  
  if ((lcd->exit_intr_info &
       (INTR_INFO_VALID_MASK | INTR_INFO_INTR_TYPE_MASK))
      == (INTR_INFO_VALID_MASK | INTR_TYPE_EXT_INTR)) {
    unsigned int vector;
    unsigned long entry;
    gate_desc *desc;
    unsigned long tmp;
              
    vector = lcd->vec_no;
    desc = (gate_desc *)lcd->host_idt_base + vector;
    entry = gate_offset(*desc);
    asm volatile(
        "mov %%" _ASM_SP ", %[sp]\n\t"
        "and $0xfffffffffffffff0, %%" _ASM_SP "\n\t"
        "push $%c[ss]\n\t"
        "push %[sp]\n\t"
        "pushf\n\t"
        "orl $0x200, (%%" _ASM_SP ")\n\t"
        __ASM_SIZE(push) " $%c[cs]\n\t"
        "call *%[entry]\n\t"
        :
        [sp]"=&r"(tmp)
        :
        [entry]"r"(entry),
        [ss]"i"(__KERNEL_DS),
        [cs]"i"(__KERNEL_CS)
                 );
    /* printk("lcd: handling external %u\n", vector); */
    if (irqs_disabled())
      local_irq_enable();
  } else {
    local_irq_enable();
    /* printk("lcd: not handling external %u\n", lcd->exit_intr_info); */
  }
}

static void vmx_handle_vmcall(lcd_struct *lcd) {
  /* printk(KERN_INFO "lcd: got vmcall %llu\n", lcd->regs[VCPU_REGS_RAX]); */
}

static void vmx_handle_page_fault(lcd_struct *lcd) {
  printk(KERN_INFO "lcd: page fault VA %p\n",
         (void*)lcd->exit_qualification);
}

static int vmx_handle_nmi_exception(lcd_struct *vcpu)
{
  printk(KERN_INFO "lcd: got an exception\n");
  if ((vcpu->exit_intr_info & INTR_INFO_INTR_TYPE_MASK)
      == INTR_TYPE_NMI_INTR)
    return 0;
  else if (is_page_fault(vcpu->exit_intr_info)) {
    vmx_handle_page_fault(vcpu);
    return 1;
  }

  printk(KERN_ERR "lcd: unhandled nmi, intr_info %x\n",
         vcpu->exit_intr_info);
  vcpu->ret_code = ((EFAULT) << 8);
  return -EIO;
}

int lcd_run(lcd_struct *lcd) {
  int done = 0;
  int ret = 0;
  int countdown = 1000;

  cycles_t begin, end;
  begin = get_cycles();
  g_prepare_time = begin - g_prepare_start;
  while (1) {
    vmx_get_cpu(lcd);

    local_irq_disable();

    /*if (need_resched()) {
      local_irq_enable();
      vmx_put_cpu(lcd);
      cond_resched();
      continue;
    }*/

    // In case we run from insmod.
    if (signal_pending(current)) {
      int signr;
      siginfo_t info;
      uint32_t x;

      local_irq_enable();
      vmx_put_cpu(lcd);

      spin_lock_irq(&current->sighand->siglock);
      signr = dequeue_signal(current, &current->blocked,
                             &info);
      spin_unlock_irq(&current->sighand->siglock);
      if (!signr)
        continue;

      if (signr == SIGKILL) {
        printk(KERN_INFO "vmx: got sigkill, dying");
        lcd->ret_code = ((ENOSYS) << 8);
        break;
      }

      x  = signr; // + base?
      x |= INTR_INFO_VALID_MASK;

      vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, x);
      continue;
    }

    /* local_irq_enable(); */

    ret = vmx_run_vcpu(lcd);
    if (ret == EXIT_REASON_VMCALL ||
        ret == EXIT_REASON_CPUID) {
      /* vmx_step_instruction(); */
    }

    if (ret == EXIT_REASON_EXTERNAL_INTERRUPT) {
      vmx_handle_external_interrupt(lcd);
      /* --countdown; */
    }
    
    vmx_put_cpu(lcd);

    if (ret == EXIT_REASON_VMCALL)
      vmx_handle_vmcall(lcd);
    else if (ret == EXIT_REASON_EPT_VIOLATION)
      done = vmx_handle_ept_violation(lcd);
    else if (ret == EXIT_REASON_EXCEPTION_NMI) {
      if (vmx_handle_nmi_exception(lcd))
        done = 1;
    } else if (ret != EXIT_REASON_EXTERNAL_INTERRUPT) {
      printk(KERN_INFO "unhandled exit: reason %d: %s, "
             "exit qualification %llx\n",
             ret, lcd_exit_reason(ret), lcd->exit_qualification);
      vmx_dump_cpu(lcd);
      done = 1;
    }

    --countdown;

    if (countdown <=0 || done || lcd->shutdown) {
      end = get_cycles();
      printk("Cycles: %llu, counter %d\n", end-begin, countdown);
      printk(KERN_INFO "unhandled exit: reason %d: %s, "
             "exit qualification %llx, intr_info %x\n",
             ret, lcd_exit_reason(ret), lcd->exit_qualification,
             lcd->exit_intr_info);
      vmx_dump_cpu(lcd);
      break;
    }
  }

  if (irqs_disabled())
    local_irq_enable();

  return 0;
}
EXPORT_SYMBOL(lcd_run);



static int __init lcd_init(void) {
  int r;
  printk(KERN_INFO "LCD module loaded\n");
  if ((r = vmx_init())) {
    printk(KERN_ERR "LCD: failed to init VMX\n");
  } else {
    ;
    /* r = lcd_test(); */
    /*
    test_thread = kthread_create(&lcd_thread, NULL, "LCD");
    if (!test_thread) {
      printk(KERN_ERR "LCD: test thread failed to create\n");
    }
    */
  } 

  return r;
}

static void __exit lcd_exit(void) {
  printk(KERN_INFO "LCD module unloading...\n");
  vmx_exit();
  printk(KERN_INFO "LCD module unloaded\n");
}

module_init(lcd_init);
module_exit(lcd_exit);
