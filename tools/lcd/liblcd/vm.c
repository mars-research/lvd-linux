/*
 * Taken from lcd guest virtual mapping
 * code. Pasted here for now.
 */


static int map_gva_to_gpa(struct lcd *vcpu,
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
