/*
 * LCD MM Rules:
 * - No huge/large page
 * - Dedicaed page table structure pages
 */

#include <linux/bitmap.h>

#define NR_PT_PAGES (1 << 15)  // Pages for page table
#define PT_PAGES_START (0x1ULL << 24)  // Above 16MB
#define PT_PAGES_END (PT_PAGES_START + (NR_PT_PAGES << PAGE_SHIFT))

void init_mm(void) {
}


unsigned long alloc_pt_pfn(struct vmx_vcpu *vcpu) {
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

int free_pt_pfn(struct vmx_vcpu *vcpu, unsigned long pfn) {
  unsigned long which = pfn - (PT_PAGES_START >> PAGE_SHIFT);
  if (which >= (PT_PAGES_END >> PAGE_SHIFT)) {
    return -EINVL; 
  } else {
    bitmap_clear(vcpu->bmp_pt_pages, which, 1);
    return 0;
  }
}

int ept_alloc_pt_item(struct vmx_vcpu *vcpu, unsigned long *gpfn,
                      unsigned long *page) {
  int ret = 0;
  *gpfn = alloc_pt_pfn(vcpu);
  if (*gpfn == 0) {
    ret = -ENOMEM;
  } else {
    // FIXME: Which mem flag do we need?
    *page = __get_free_page(GFP_ATOMIC);
    if (!(*page)) {
      ret = -ENOMEM;
    } else {
      memset((void*)(*page), 0, PAGE_SIZE);
      ret = ept_set_epte(vcpu, ((*gpfn) << PAGE_SHIFT), *page, 0);
    }
  }
  return ret;
}

int ept_gpa_to_hva(struct vmx_vcpu* vcpu, unsigned long gpa,
                   unsigned long* hva) {
  epte_t *epte;
  int ret = ept_lookup_gpa(vcpu, gpa, 0, &epte);
  if (ret) {
    printk(KERN_ERR "ept: failed to lookup GPA: %p\n",
           (void*)gpa);
  } else {
    if (!epte_present(*epte)) {
      printk(KERN_ERR "ept: epte not present when translating\n");
      ret = -EINVL;
    } else {
      *hva = (unsigned long)epte_page_vaddr(*epte);
    }
  }
  return ret;
}

// Set gva => gpa. No page allocated for gva/gpa, just
// pte value setting. But page table structures along the
// path can be created by setting 'create' to 1.
int map_gva_to_gpa(struct vmx_vcpu *vcpu,
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
