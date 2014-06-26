/*
 * LCD core VTX functions 
 *
 * Based on KVM and Dune.
 *
 * Authors:
 *
 *   Weibin Sun <wbsun@flux.utah.edu>
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

//#include <asm/vmx.h>
#include <asm/processor.h>
#include <asm/desc.h>
#include <asm/virtext.h>

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <asm/segment.h>
#include <asm/tsc.h>
#include <asm/uaccess.h>

#include <uapi/asm/e820.h>
#include <uapi/asm/bootparam.h>
#include <uapi/linux/elf.h>

#include <uapi/linux/limits.h>

/* #include "lcd.h" */
#include <lcd/ipc.h>
#include "lcd_defs.h"



/* struct vmx_capability vmx_capability; */

/* static atomic_t vmx_enable_failed; */

/* static DECLARE_BITMAP(vmx_vpid_bitmap, VMX_NR_VPIDS); */
/* static DEFINE_SPINLOCK(vmx_vpid_lock); */

/* static DEFINE_PER_CPU(struct vmcs *, vmxarea); */
/* static DEFINE_PER_CPU(struct desc_ptr, host_gdt); */
/* static DEFINE_PER_CPU(int, vmx_enabled); */
/* static DEFINE_PER_CPU(struct lcd *, local_vcpu); */

/* static unsigned long *msr_bitmap; */

/* struct vmcs_config vmcs_config; */


/* static cycles_t g_prepare_start, g_prepare_time; */

/* CPU Features and Ops */

/* static inline bool cpu_has_secondary_exec_ctrls(void) { */
/* 	return vmcs_config.cpu_based_exec_ctrl & */
/* 		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS; */
/* } */

/* static inline bool cpu_has_vmx_vpid(void) { */
/* 	return vmcs_config.cpu_based_2nd_exec_ctrl & */
/* 		SECONDARY_EXEC_ENABLE_VPID; */
/* } */

/* static inline bool cpu_has_vmx_invpcid(void) { */
/* 	return vmcs_config.cpu_based_2nd_exec_ctrl & */
/* 		SECONDARY_EXEC_ENABLE_INVPCID; */
/* } */

/* static inline bool cpu_has_vmx_invvpid_single(void) { */
/* 	return vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT; */
/* } */

/* static inline bool cpu_has_vmx_invvpid_global(void) { */
/* 	return vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT; */
/* } */

/* static inline bool cpu_has_vmx_ept(void) { */
/* 	return vmcs_config.cpu_based_2nd_exec_ctrl & */
/* 		SECONDARY_EXEC_ENABLE_EPT; */
/* } */

/* static inline bool cpu_has_vmx_invept_individual_addr(void) { */
/* 	return vmx_capability.ept & VMX_EPT_EXTENT_INDIVIDUAL_BIT; */
/* } */

/* static inline bool cpu_has_vmx_invept_context(void) { */
/* 	return vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT; */
/* } */

/* static inline bool cpu_has_vmx_invept_global(void) { */
/* 	return vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT; */
/* } */

/* static inline bool cpu_has_vmx_ept_ad_bits(void) { */
/* 	return vmx_capability.ept & VMX_EPT_AD_BIT; */
/* } */

/* static inline void __invept(int ext, u64 eptp, u64 gpa) { */
/* 	struct { */
/* 		u64 eptp, gpa; */
/* 	} operand = {eptp, gpa}; */

/* 	asm volatile (ASM_VMX_INVEPT */
/*                 /\* CF==1 or ZF==1 --> rc = -1 *\/ */
/*                 "; ja 1f ; ud2 ; 1:\n" */
/*                 : : "a" (&operand), "c" (ext) : "cc", "memory"); */
/* } */

/* static inline void ept_sync_global(void) { */
/* 	if (cpu_has_vmx_invept_global()) */
/* 		__invept(VMX_EPT_EXTENT_GLOBAL, 0, 0); */
/* } */

/* static inline void ept_sync_context(u64 eptp) { */
/* 	if (cpu_has_vmx_invept_context()) */
/* 		__invept(VMX_EPT_EXTENT_CONTEXT, eptp, 0); */
/* 	else */
/* 		ept_sync_global(); */
/* } */

/* static inline void ept_sync_individual_addr(u64 eptp, u64 gpa) { */
/* 	if (cpu_has_vmx_invept_individual_addr()) */
/* 		__invept(VMX_EPT_EXTENT_INDIVIDUAL_ADDR, */
/* 			eptp, gpa); */
/* 	else */
/* 		ept_sync_context(eptp); */
/* } */

/* static inline void __invvpid(int ext, u16 vpid, u64 gva) { */
/* 	struct { */
/* 		u64 vpid : 16; */
/* 		u64 rsvd : 48; */
/* 		u64 gva; */
/* 	} operand = { vpid, 0, gva }; */

/* 	asm volatile (ASM_VMX_INVVPID */
/*                 /\* CF==1 or ZF==1 --> rc = -1 *\/ */
/* 		"; ja 1f ; ud2 ; 1:" */
/*                 : : "a"(&operand), "c"(ext) : "cc", "memory"); */
/* } */

/* static inline void vpid_sync_vcpu_single(u16 vpid) { */
/* 	if (vpid == 0) */
/* 		return; */

/* 	if (cpu_has_vmx_invvpid_single()) */
/* 		__invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vpid, 0); */
/* } */

/* static inline void vpid_sync_vcpu_global(void) { */
/* 	if (cpu_has_vmx_invvpid_global()) */
/* 		__invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0, 0); */
/* } */

/* static inline void vpid_sync_context(u16 vpid) { */
/* 	if (cpu_has_vmx_invvpid_single()) */
/* 		vpid_sync_vcpu_single(vpid); */
/* 	else */
/* 		vpid_sync_vcpu_global(); */
/* } */

/* static inline u16 vmx_read_ldt(void) { */
/* 	u16 ldt; */
/* 	asm("sldt %0" : "=g"(ldt)); */
/* 	return ldt; */
/* } */

/* static bool allow_1_setting(u32 msr, u32 ctl) { */
/* 	u32 vmx_msr_low, vmx_msr_high; */

/* 	rdmsr(msr, vmx_msr_low, vmx_msr_high); */
/* 	return vmx_msr_high & ctl; */
/* } */

/* static inline void __vmxon(u64 addr) { */
/* 	asm volatile (ASM_VMX_VMXON_RAX */
/*                 : : "a"(&addr), "m"(addr) */
/*                 : "memory", "cc"); */
/* } */

/* static inline void __vmxoff(void) { */
/* 	asm volatile (ASM_VMX_VMXOFF : : : "cc"); */
/* } */

/* static void vmcs_clear(struct vmcs *vmcs) { */
/* 	u64 phys_addr = __pa(vmcs); */
/* 	u8 error; */

/* 	asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0" */
/*                 : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr) */
/*                 : "cc", "memory"); */
/* 	if (error) */
/* 		printk(KERN_ERR "kvm: vmclear fail: %p/%llx\n", */
/* 			vmcs, phys_addr); */
/* } */

/* static void vmcs_load(struct vmcs *vmcs) { */
/* 	u64 phys_addr = __pa(vmcs); */
/* 	u8 error; */

/* 	asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0" */
/*                 : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr) */
/*                 : "cc", "memory"); */
/* 	if (error) */
/* 		printk(KERN_ERR "vmx: vmptrld %p/%llx failed\n", */
/* 			vmcs, phys_addr); */
/* } */

/* static __always_inline unsigned long vmcs_readl(unsigned long field) { */
/* 	unsigned long value; */

/* 	asm volatile (ASM_VMX_VMREAD_RDX_RAX */
/*                 : "=a"(value) : "d"(field) : "cc"); */
/* 	return value; */
/* } */

/* static __always_inline u16 vmcs_read16(unsigned long field) { */
/* 	return vmcs_readl(field); */
/* } */

/* static __always_inline u32 vmcs_read32(unsigned long field) { */
/* 	return vmcs_readl(field); */
/* } */

/* static __always_inline u64 vmcs_read64(unsigned long field) { */
/* 	return vmcs_readl(field); */
/* } */

/* static noinline void vmwrite_error(unsigned long field, */
/* 				unsigned long value) { */
/* 	printk(KERN_ERR "vmwrite error: reg %lx value %lx (err %d)\n", */
/* 		field, value, vmcs_read32(VM_INSTRUCTION_ERROR)); */
/* 	dump_stack(); */
/* } */

/* static void vmcs_writel(unsigned long field, unsigned long value) { */
/* 	u8 error; */

/* 	asm volatile (ASM_VMX_VMWRITE_RAX_RDX "; setna %0" */
/*                 : "=q"(error) : "a"(value), "d"(field) : "cc"); */
/* 	if (unlikely(error)) */
/* 		vmwrite_error(field, value); */
/* } */

/* static void vmcs_write16(unsigned long field, u16 value) { */
/* 	vmcs_writel(field, value); */
/* } */

/* static void vmcs_write32(unsigned long field, u32 value) { */
/* 	vmcs_writel(field, value); */
/* } */

/* static void vmcs_write64(unsigned long field, u64 value) { */
/* 	vmcs_writel(field, value); */
/* } */

/* static inline bool is_external_interrupt(u32 intr_info) { */
/* 	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VALID_MASK)) */
/* 		== (INTR_TYPE_EXT_INTR | INTR_INFO_VALID_MASK); */
/* } */

/* Memory Management */

/* static inline bool is_page_fault(u32 intr_info) { */
/* 	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK | */
/* 				INTR_INFO_VALID_MASK)) == */
/* 		(INTR_TYPE_HARD_EXCEPTION | PF_VECTOR | INTR_INFO_VALID_MASK); */
/* } */

/* static inline uintptr_t epte_addr(epte_t epte) { */
/* 	return (epte & EPTE_ADDR); */
/* } */

/* static inline uintptr_t epte_page_vaddr(epte_t epte) { */
/* 	return (uintptr_t) __va(epte_addr(epte)); */
/* } */

/* static inline epte_t epte_flags(epte_t epte) { */
/* 	return (epte & EPTE_FLAGS); */
/* } */

/* static inline int epte_present(epte_t epte) { */
/* 	return (epte & __EPTE_FULL) > 0; */
/* } */

/* static inline int epte_big(epte_t epte) { */
/* 	return (epte & __EPTE_SZ) > 0; */
/* } */

/* static void free_ept_page(epte_t epte) { */
/* 	struct page *page = pfn_to_page(epte_addr(epte) >> PAGE_SHIFT); */

/* 	if (epte & __EPTE_WRITE) */
/* 		set_page_dirty_lock(page); */
/* 	put_page(page); */
/* } */

/* static int clear_epte(epte_t *epte) { */
/* 	if (*epte == __EPTE_NONE) */
/* 		return 0; */

/* 	free_ept_page(*epte); */
/* 	*epte = __EPTE_NONE; */

/* 	return 1; */
/* } */

/* /\** */
/*  * Look up the ept entry for guest physical */
/*  * address gpa. If create is 1, ept paging structures */
/*  * will be allocated on the fly. */
/*  * */
/*  * The page frame where gpa resides is not allocated. */
/*  * */
/*  * FIXME: Do we need to lock inside here? */
/*  *\/ */
/* static int lcd_ept_walk(struct lcd *vcpu, u64 gpa, int create, */
/* 			epte_t **epte_out) { */
/* 	int i; */
/* 	epte_t *dir = (epte_t *) __va(vcpu->ept_root); */

/* 	for (i = EPT_LEVELS - 1; i > 0; i--) { */
/* 		int idx = ADDR_TO_IDX(gpa, i); */

/* 		if (!epte_present(dir[idx])) { */
/* 			void *page; */

/* 			if (!create) */
/* 				return -ENOENT; */

/* 			page = (void *) __get_free_page(GFP_ATOMIC); */
/* 			if (!page) */
/* 				return -ENOMEM; */

/* 			memset(page, 0, PAGE_SIZE); */
/* 			dir[idx] = epte_addr(virt_to_phys(page)) | */
/* 				__EPTE_FULL; */
/* 		} */

/* 		if (epte_big(dir[idx])) { */
/* 			return -EINVAL; */
/* 		} */

/* 		dir = (epte_t *) epte_page_vaddr(dir[idx]); */
/* 	} */

/* 	*epte_out = &dir[ADDR_TO_IDX(gpa, 0)]; */
/* 	return 0; */
/* } */

/* /\** */
/*  * Store host physical address hpa, along with */
/*  * default flags, to the epte. */
/*  * */
/*  * FIXME: Do we want to lock inside here? */
/*  *\/ */
/* static int lcd_ept_set_epte(struct lcd *vcpu, epte_t *epte_entry, u64 hpa) { */
/* 	int ret; */
/* 	epte_t flags; */

/* 	spin_lock(&vcpu->ept_lock); */

/* 	flags = __EPTE_READ | __EPTE_EXEC | __EPTE_WRITE | */
/* 		__EPTE_TYPE(EPTE_TYPE_WB) | __EPTE_IPAT; */

/* 	if (vcpu->ept_ad_enabled) { */
/* 		/\* premark A/D to avoid extra memory references *\/ */
/* 		flags |= __EPTE_A | __EPTE_D; */
/* 	} */

/* 	*epte = epte_addr(hpa) | flags; */
  
/* 	spin_unlock(&vcpu->ept_lock); */

/* 	return 0; */
/* } */

/* /\** */
/*  * Maps gpa to hpa in the lcd's ept. Simple wrapper */
/*  * around lookup and set. */
/*  *\/ */
/* static int lcd_ept_map_gpa_to_hpa(struct lcd *vcpu, u64 gpa, u64 hpa, */
/* 				int create, int overwrite) { */
/* 	int ret; */
/* 	epte_t *ept_entry; */

/* 	/\* */
/* 	 * Walk ept */
/* 	 *\/ */
/* 	ret = lcd_ept_walk(vcpu, gpa, create, &ept_entry); */
/* 	if (ret) */
/* 		return ret; */

/* 	/\* */
/* 	 * Check if guest physical address already mapped */
/* 	 *\/ */
/* 	if (!overwrite && epte_present(ept_entry)) */
/* 		return -E; */

/* 	/\* */
/* 	 * Map the guest physical addr to the host physical addr. */
/* 	 *\/ */
/* 	lcd_ept_set_epte(vcpu, ept_entry, gpa, hpa); */

/* 	return 0; */
/* } */

/* /\** */
/*  * Returns host virtual address associated with */
/*  * guest physical address, using lcd's ept. */
/*  * */
/*  * Needed for manipulating guest (inner lcd) data structures */
/*  * that store guest physical addresses, like guest virtual */
/*  * paging structures. */
/*  *\/ */
/* static int lcd_ept_gpa_to_hva(struct lcd* vcpu, u64 gpa, u64 *hva) { */
/* 	epte_t *epte; */
/* 	int ret; */

/* 	ret = lcd_ept_walk(vcpu, gpa, 0, &epte); */
/* 	if (ret) { */
/* 		printk(KERN_ERR "ept gpa to hva: failed to lookup gpa: %x\n", */
/* 			gpa); */
/* 		return ret; */
/* 	} */

/* 	if (!epte_present(*epte)) { */
/* 		printk(KERN_ERR "ept gpa to hva: no mapping for gpa: %x\n", */
/* 			gpa); */
/* 		return -ENOENT; */
/* 	} */

/* 	*hva = (u64)epte_page_vaddr(*epte); */

/* 	return 0; */
/* } */

/* static void lcd_free_ept(u64 ept_root) { */
/* 	epte_t *pgd; */
/* 	int i, j, k, l; */

/* 	pgd = (epte_t *) __va(ept_root); */

/* 	for (i = 0; i < PTRS_PER_PGD; i++) { */
/* 		epte_t *pud = (epte_t *) epte_page_vaddr(pgd[i]); */
/* 		if (!epte_present(pgd[i])) */
/* 			continue; */

/* 		for (j = 0; j < PTRS_PER_PUD; j++) { */
/* 			epte_t *pmd = (epte_t *) epte_page_vaddr(pud[j]); */
/* 			if (!epte_present(pud[j])) */
/* 				continue; */
/* 			if (epte_flags(pud[j]) & __EPTE_SZ) */
/* 				continue; */

/* 			for (k = 0; k < PTRS_PER_PMD; k++) { */
/* 				epte_t *pte = (epte_t *) epte_page_vaddr(pmd[k]); */
/* 				if (!epte_present(pmd[k])) */
/* 					continue; */
/* 				if (epte_flags(pmd[k]) & __EPTE_SZ) { */
/* 					free_ept_page(pmd[k]); */
/* 					continue; */
/* 				} */

/* 				for (l = 0; l < PTRS_PER_PTE; l++) { */
/* 					if (!epte_present(pte[l])) */
/* 						continue; */

/* 					free_ept_page(pte[l]); */
/* 				} // PTE loop */

/* 				free_page((unsigned long) pte); */
/* 			} // PMD loop */

/* 			free_page((unsigned long) pmd); */
/* 		} // PUD loop */

/* 		free_page((unsigned long) pud); */
/* 	} // PGD loop */

/* 	free_page((unsigned long) pgd); */
/* } */


/* int vmx_init_ept(struct lcd *vcpu) { */
/* 	void *page = (void *) __get_free_page(GFP_KERNEL); */

/* 	if (!page) */
/* 		return -ENOMEM; */

/* 	memset(page, 0, PAGE_SIZE); */
/* 	vcpu->ept_root =  __pa(page); */

/* 	return 0; */
/* } */

/* static u64 construct_eptp(u64 root_hpa) { */
/* 	u64 eptp; */

/* 	eptp = VMX_EPT_DEFAULT_MT | */
/* 		VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT; */
/* 	if (cpu_has_vmx_ept_ad_bits()) */
/* 		eptp |= VMX_EPT_AD_ENABLE_BIT; */
/* 	eptp |= (root_hpa & PAGE_MASK); */

/* 	return eptp; */
/* } */

/* END EPT ======================================== */

/**
 * Allocates a host physical page and guest physical
 * page (using the bitmap) for storing a page table.
 */
static int lcd_gv_alloc_paging_mem_page(unsigned long *paging_mem_bitmap,
				u64 *gpa, u64 *hva) {
	unsigned long which;
	u64 page;
	
	/*
	 * Allocate a host physical page for the paging memory
	 */
	page = __get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;
	memset((void *)page, 0, PAGE_SIZE);
	*hva = page;

	/*
	 * Allocate guest physical address space for paging mem page.
	 */
	which = bitmap_find_next_zero_area(
		paging_mem_bitmap,
		LCD_PAGING_MEM_NUM_PAGES,
		0, 1, 0);

	if (which >= LCD_PAGING_MEM_NUM_PAGES) {
		return -ENOMEM;
	} else {
		bitmap_set(paging_mem_bitmap, which, 1);
		*gpa = LCD_PAGING_MEM_START + (which << PAGE_SHIFT);
		return 0;
	}
}

/**
 * Look up the page table (level 3) for guest virtual
 * address gva, using the middle page directory (level 2)
 * pmd_hva (host virtual address of the pmd).
 */
static int lcd_gv_lookup_ptb(struct lcd *vcpu, pmd_t *pmd_hva, u64 gva,
			unsigned long *paging_mem_bitmap, 
			pte_t **ptb_out) {
	int ret;
	pmd_t *pmd_entry;
	u64 ptb_gpa;
	u64 ptb_hva;

	pmd_entry = pmd_hva + pmd_index(gva);
	if (!pmd_present(*pmd_entry)) {

		/*
		 * Alloc and map a page table
		 */
		ret = lcd_gv_alloc_paging_mem_page(paging_mem_bitmap,
						&ptb_gpa, &ptb_hva);
		if (ret)
			return ret;

		/*
		 * Map *guest physical* address into pmd entry
		 */
		set_pmd(pmd_entry, __pmd(ptb_gpa | _KERNPG_TABLE));

	} else {

		ret = lcd_ept_gpa_to_hva(vcpu, pmd_val(*pmd_entry),
					&ptb_hva);
		if (ret)
			return ret;
	}

	*ptb_out = ptb_hva;

	return 0;
}

/**
 * Look up the middle page directory (level 2)
 * for guest virtual address gva, using the upper
 * page directory (level 1) pud_hva (host virtual
 * address of upper directory).
 */
static int lcd_gv_lookup_pmd(struct lcd *vcpu, pud_t *pud_hva, u64 gva,
			unsigned long *paging_mem_bitmap, 
			pmd_t **pmd_out) {
	int ret;
	pud_t *pud_entry;
	u64 pmd_gpa;
	u64 pmd_hva;

	pud_entry = pud_hva + pud_index(gva);
	if (!pud_present(*pud_entry)) {

		/*
		 * Alloc and map a pmd
		 */
		ret = lcd_gv_alloc_paging_mem_page(paging_mem_bitmap,
						&pmd_gpa, &pmd_hva);
		if (ret)
			return ret;

		/*
		 * Map *guest physical* address into pud entry
		 */
		set_pud(pud_entry, __pud(pmd_gpa | _KERNPG_TABLE));

	} else {

		ret = lcd_ept_gpa_to_hva(vcpu, pud_val(*pud_entry),
					&pmd_hva);
		if (ret)
			return ret;
	}

	*pmd_out = pmd_hva;

	return 0;
}

/**
 * Look up the upper page directory (level 1)
 * for guest virtual address gva, using the
 * global page directory (level 0?) root_hva
 * (host virtual address of the root / global
 * page directory).
 */
static int lcd_gv_lookup_pud(struct lcd *vcpu, pgd_t *root_hva, u64 gva,
			unsigned long *paging_mem_bitmap, 
			pud_t **pud_out) {
	int ret;
	pgd_t *pgd_entry;
	u64 pud_gpa;
	u64 pud_hva;

	pgd_entry = root_hva + pgd_index(gva);
	if (!pgd_present(*pgd_entry)) {

		/*
		 * Alloc and map a pud
		 */
		ret = lcd_gv_alloc_paging_mem_page(paging_mem_bitmap,
						&pud_gpa, &pud_hva);
		if (ret)
			return ret;

		/*
		 * Map *guest physical* address into pgd entry
		 */
		set_pgd(pgd_entry, mk_kernel_pgd(pud_gpa));

	} else {

		ret = lcd_ept_gpa_to_hva(vcpu, pgd_val(*pgd_entry),
					&pud_hva);
		if (ret)
			return ret;
	}

	*pud_out = pud_hva;

	return 0;
}

/**
 * Look up the page table entry for guest virtual
 * address gva, using the root / global page
 * directory root_hva (host virtual address of the
 * global page directory).
 *
 * Paging data structures are allocated along the
 * way.
 */
static int lcd_gv_walk(struct lcd *vcpu, pgd_t *root_hva, u64 gva,
			unsigned long *paging_mem_bitmap, 
			pte_t *pte_out) {
	int ret;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptb;

	/*
	 * 0th level --> 1st level
	 */
	ret = lcd_gv_lookup_pud(vcpu, root_hva, gva, paging_mem_bitmap,
				&pud);
	if (ret)
		return ret;

	/*
	 * 1st level --> 2nd level
	 */
	ret = lcd_gv_lookup_pmd(vcpu, pud, gva, paging_mem_bitmap, &pmd);
	if (ret)
		return ret;

	/*
	 * 2nd level --> 3rd level
	 */
	ret = lcd_gv_lookup_ptb(vcpu, pmd, gva, paging_mem_bitmap, &ptb);
	if (ret)
		return ret;

	/*
	 * Lookup pte
	 */
	*pte_out = ptb + pte_index(gva);

	return 0;
}

/**
 * Maps gva to gpa in the paging hierarchy
 * rooted at root_hva (the host virtual
 * address of the global page directory).
 *
 * Simple wrapper for lookup and set.
 */
static int lcd_gv_map_gva_to_gpa(struct lcd *vcpu, pgd_t *root_hva, u64 gva,
				u64 gpa, unsigned long *paging_mem_bitmap) {
	int ret;
	pte_t *pte_entry;

	ret = lcd_gv_walk(vcpu, root_hva, gva, gpa, paging_mem_bitmap,
			&pte_entry);
	if (ret)
		return ret;

	/*
	 * Set pte, making memory executable
	 */
	set_pte(pte_entry, __pte(gpa | __PAGE_KERNEL_EXEC));

	return 0;
}

/**
 * Maps all critical parts of the guest virtual
 * address space so that code can run upon entry.
 *
 * The guest physical address of the root / global 
 * page directory is stored in root_gpa_out.
 *
 * If mapping fails, caller is responsible for
 * freeing memory allocated for guest virtual
 * paging (using the mappings in the ept).
 */
static int lcd_setup_initial_gva(struct lcd *vcpu, u64 *root_gpa_out) {
	unsigned long *paging_mem_bitmap;
	pte_t *pte;
	u64 root_gpa;
	u64 root_hva;

	/*
	 * Use a bitmap to track allocation of memory for
	 * guest virtual paging.
	 */
	paging_mem_bitmap = kmalloc(sizeof(long) * 
				BITS_TO_LONGS(LCD_PAGING_MEM_NUM_PAGES),
				GFP_KERNEL);
	if (!paging_mem_bitmap) {
		ret = -ENOMEM;
		goto fail1;
	}
	bitmap_zero(paging_mem_bitmap, LCD_PAGING_MEM_NUM_PAGES);

	/*
	 * Allocate and map root page directory
	 */
	ret = alloc_paging_mem_page(paging_mem_bitmap, &root_gpa, &root_hva);
	if (ret) {
		printk(KERN_ERR "guest virt setup: root pgd fail\n");
		goto fail2;
	}
	ret = lcd_ept_map_gpa_to_hpa(vcpu, root_gpa, __pa(root_hva), 1, 0);
	if (ret) {
		printk(KERN_ERR "guest virt setup: root pgd fail\n");
		goto fail2;
	}

	/*
	 * Map all of the virtual address space, up to LCD_HEAP_START.
	 * Guest physical and guest virtual addresses are mapped one-to-one.
	 */
	for (gpa = gva = LCD_BOTTOM; gva < LCD_HEAP_START; 
	     gpa += PAGE_SIZE, gva += PAGE_SIZE) {

		ret = lcd_gv_map_gva_to_gpa(vcpu, pgd_t *root_hva, gva, gpa,
					paging_mem_bitmap);
		if (ret) {
			printk(KERN_ERR "map gva to gpa: map failed at %x\n",
				gva);
			goto fail2;
		}
	}

	*root_gpa_out = root_gpa;
	
	return 0;

fail2:
	kfree(paging_mem_bmp);
fail1:
	return ret;
}

/* /\** */
/*  * Maps all critical parts of the guest physical */
/*  * address space that are needed by the lcd upon */
/*  * entry. */
/*  * */
/*  * If mapping fails, caller is responsible for */
/*  * freeing host physical memory allocated */
/*  * (using the entries in the ept). */
/*  *\/ */
/* static int lcd_setup_initial_ept(struct lcd *vcpu) { */
/* 	int ret; */
/* 	int i; */
/* 	u64 hva; */
/* 	u64 gpa; */
/* 	epte_t *ept_entry; */

/* 	/\* */
/* 	 * Map guest physical from LCD_BOTTOM to LCD_PAGING_MEM_START */
/* 	 *\/ */
/* 	for (gpa = LCD_BOTTOM; gpa < LCD_PAGING_MEM_START; gpa += PAGE_SIZE) { */

/* 		hva = __get_free_page(GFP_KERNEL); */
/* 		if (!hva) { */
/* 			return -ENOMEM; */
/* 		} */
/* 		memset((void*)hva, 0, PAGE_SIZE); */

/* 		/\* */
/* 		 * Map guest physical to host physical, allocating */
/* 		 * ept paging structures along the way, but do not */
/* 		 * overwrite. */
/* 		 *\/ */
/* 		ret = lcd_ept_map_gpa_to_hpa(vcpu, gpa, __pa(hva), 1, 0); */
/* 		if (ret) { */
/* 			printk(KERN_ERR "setup initial ept: failed at %x\n", */
/* 				gpa); */
/* 			return ret; */
/* 		} */
/* 	} */
/* } */

/* /\** */
/*  * Allocates memory and maps all parts of the */
/*  * guest physical and guest virtual address */
/*  * space that are needed upon initial entry */
/*  * to the lcd. */
/*  * */
/*  * On error, deallocates ept. */
/*  *\/ */
/* static int lcd_setup_addr_space(struct lcd *vcpu) { */
/* 	int ret; */
/* 	epte_t ept_entry; */
/* 	u64 guest_paging_pgd_gpa; */

/* 	ret = lcd_setup_initial_ept(vcpu); */
/* 	if (ret) */
/* 		goto fail; */

/* 	ret = lcd_setup_initial_gva(vcpu, &guest_paging_pgd_gpa); */
/* 	if (ret) */
/* 		goto fail; */


/* 	return 0; */

/* fail: */
/* 	lcd_free_ept(vcpu); */
/* 	return ret; */
/* } */

/* /\* VMCS Related *\/ */

/* static unsigned long segment_base(u16 selector) { */
/* 	struct desc_ptr *gdt = &__get_cpu_var(host_gdt); */
/* 	struct desc_struct *d; */
/* 	unsigned long table_base; */
/* 	unsigned long v; */

/* 	if (!(selector & ~3)) */
/* 		return 0; */

/* 	table_base = gdt->address; */

/* 	if (selector & 4) {           /\* from ldt *\/ */
/* 		u16 ldt_selector = vmx_read_ldt(); */

/* 		if (!(ldt_selector & ~3)) */
/* 			return 0; */

/* 		table_base = segment_base(ldt_selector); */
/* 	} */
/* 	d = (struct desc_struct *)(table_base + (selector & ~7)); */
/* 	v = get_desc_base(d); */

/* 	if (d->s == 0 && (d->type == 2 || d->type == 9 || d->type == 11)) */
/* 		v |= ((unsigned long)((struct ldttss_desc64 *)d)->base3) << 32; */

/* 	return v; */
/* } */

/* static inline unsigned long vmx_read_tr_base(void) { */
/* 	u16 tr; */
/* 	asm("str %0" : "=g"(tr)); */
/* 	return segment_base(tr); */
/* } */

/* static void __vmx_setup_cpu(void) { */
/* 	struct desc_ptr *gdt = &__get_cpu_var(host_gdt); */
/* 	unsigned long sysenter_esp; */
/* 	unsigned long tmpl; */

/* 	/\* */
/* 	 * Linux uses per-cpu TSS and GDT, so set these when switching */
/* 	 * processors. */
/* 	 *\/ */
/* 	vmcs_writel(HOST_TR_BASE, vmx_read_tr_base()); /\* 22.2.4 *\/ */
/* 	vmcs_writel(HOST_GDTR_BASE, gdt->address);   /\* 22.2.4 *\/ */

/* 	/\* No sysenter should be called *\/ */
/* 	rdmsrl(MSR_IA32_SYSENTER_ESP, sysenter_esp); */
/* 	vmcs_writel(HOST_IA32_SYSENTER_ESP, sysenter_esp); /\* 22.2.3 *\/ */

/* 	rdmsrl(MSR_FS_BASE, tmpl); */
/* 	vmcs_writel(HOST_FS_BASE, tmpl); /\* 22.2.4 *\/ */
/* 	rdmsrl(MSR_GS_BASE, tmpl); */
/* 	vmcs_writel(HOST_GS_BASE, tmpl); /\* 22.2.4 *\/ */
/* } */

/* static void __vmx_get_cpu_helper(void *ptr) { */
/* 	struct lcd *vcpu = ptr; */

/* 	BUG_ON(raw_smp_processor_id() != vcpu->cpu); */
/* 	vmcs_clear(vcpu->vmcs); */
/* 	if (__get_cpu_var(local_vcpu) == vcpu) */
/* 		__get_cpu_var(local_vcpu) = NULL; */
/* } */

/* /\** */
/*  * vmx_get_cpu - called before using a cpu */
/*  * @vcpu: VCPU that will be loaded. */
/*  * */
/*  * Disables preemption. Call vmx_put_cpu() when finished. */
/*  *\/ */
/* static void vmx_get_cpu(struct lcd *vcpu) { */
/* 	int cur_cpu = get_cpu(); */

/* 	if (__get_cpu_var(local_vcpu) != vcpu) { */
/* 		__get_cpu_var(local_vcpu) = vcpu; */

/* 		if (vcpu->cpu != cur_cpu) { */
/* 			if (vcpu->cpu >= 0) */
/* 				smp_call_function_single( */
/* 					vcpu->cpu, */
/* 					__vmx_get_cpu_helper, (void *) vcpu, 1); */
/* 			else */
/* 				vmcs_clear(vcpu->vmcs); */

/* 			vpid_sync_context(vcpu->vpid); */
/* 			ept_sync_context(vcpu->eptp); */

/* 			vcpu->launched = 0; */
/* 			vmcs_load(vcpu->vmcs); */
/* 			__vmx_setup_cpu(); */
/* 			vcpu->cpu = cur_cpu; */
/* 		} else { */
/* 			vmcs_load(vcpu->vmcs); */
/* 		} */
/* 	} */
/* } */

/* /\** */
/*  * vmx_put_cpu - called after using a cpu */
/*  * @vcpu: VCPU that was loaded. */
/*  *\/ */
/* static void vmx_put_cpu(struct lcd *vcpu) { */
/* 	put_cpu(); */
/* } */


/* static int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt, */
/* 			u32 msr, u32 *result) { */
/* 	u32 vmx_msr_low, vmx_msr_high; */
/* 	u32 ctl = ctl_min | ctl_opt; */

/* 	rdmsr(msr, vmx_msr_low, vmx_msr_high); */

/* 	ctl &= vmx_msr_high; /\* bit == 0 in high word ==> must be zero *\/ */
/* 	ctl |= vmx_msr_low;  /\* bit == 1 in low word  ==> must be one  *\/ */

/* 	/\* Ensure minimum (required) set of control bits are supported. *\/ */
/* 	if (ctl_min & ~ctl) */
/* 		return -EIO; */

/* 	*result = ctl; */
/* 	return 0; */
/* } */

static void vmx_dump_cpu(struct lcd *vcpu) {
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

/* static int setup_vmcs_config(struct vmcs_config *vmcs_conf) { */
/* 	u32 vmx_msr_low, vmx_msr_high; */
/* 	u32 min, opt, min2, opt2; */
/* 	u32 _pin_based_exec_control = 0; */
/* 	u32 _cpu_based_exec_control = 0; */
/* 	u32 _cpu_based_2nd_exec_control = 0; */
/* 	u32 _vmexit_control = 0; */
/* 	u32 _vmentry_control = 0; */

/* 	/\* Primrary Processor-based VME controls: */
/* 	   - No TSC offsetting (not set) */
/* 	   - HLT exit */
/* 	   - Invalidate PG exit */
/* 	   - MWAIT exit */
/* 	   - RDPMC exit */
/* 	   - L/S CR8 exit */
/* 	   - Unconditional I/O exit (no I/O bitmap) */
/* 	   ### - (No) MSR bitmap */
/* 	*\/   */
/* 	min = CPU_BASED_HLT_EXITING | */
/* 		CPU_BASED_INVLPG_EXITING | */
/* 		CPU_BASED_MWAIT_EXITING | */
/* 		CPU_BASED_RDPMC_EXITING | */
/* 		CPU_BASED_CR8_LOAD_EXITING | */
/* 		CPU_BASED_CR8_STORE_EXITING | */
/* 		CPU_BASED_MOV_DR_EXITING | */
/* 		CPU_BASED_UNCOND_IO_EXITING | */
/* 		CPU_BASED_MONITOR_EXITING; */

/* 	opt = CPU_BASED_ACTIVATE_SECONDARY_CONTROLS | */
/* 		CPU_BASED_USE_MSR_BITMAPS; */
/* 	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PROCBASED_CTLS, */
/* 					&_cpu_based_exec_control) < 0) */
/* 		return -EIO; */
/* 	if ((_cpu_based_exec_control & CPU_BASED_TPR_SHADOW)) */
/* 		_cpu_based_exec_control &= ~CPU_BASED_CR8_LOAD_EXITING & */
/* 			~CPU_BASED_CR8_STORE_EXITING; */

/* 	// We need the secondary VME controls. */
/* 	if (_cpu_based_exec_control & CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) { */
/* 		min2 = 0; */
/* 		/\* Secondary VME control: */
/* 		   - No virtual APIC access */
/* 		*\/ */
/* 		opt2 = SECONDARY_EXEC_ENABLE_EPT | */
/* 			SECONDARY_EXEC_RDTSCP | */
/* 			SECONDARY_EXEC_ENABLE_VPID | */
/* 			SECONDARY_EXEC_WBINVD_EXITING | */
/* 			SECONDARY_EXEC_UNRESTRICTED_GUEST | // SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY | */
/* 			SECONDARY_EXEC_ENABLE_INVPCID; */
/* 		if (adjust_vmx_controls(min2, opt2, */
/* 						MSR_IA32_VMX_PROCBASED_CTLS2, */
/* 						&_cpu_based_2nd_exec_control) < 0) */
/* 			return -EIO; */
/* 	} else { */
/* 		printk(KERN_ERR "lcd: secondary VME controls not supported\n"); */
/* 		return -EIO; */
/* 	} */

/* 	// We need EPT. */
/* 	if (_cpu_based_2nd_exec_control & SECONDARY_EXEC_ENABLE_EPT) { */
/* 		rdmsr(MSR_IA32_VMX_EPT_VPID_CAP, */
/* 			vmx_capability.ept, vmx_capability.vpid); */
/* 	} else { */
/* 		printk(KERN_ERR "lcd: EPT not supported\n"); */
/* 		return -EIO; */
/* 	} */

/* 	// Pin-based control: */
/* 	min = PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING; */
/* 	opt = 0; */
/* 	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PINBASED_CTLS, */
/* 					&_pin_based_exec_control) < 0) */
/* 		return -EIO; */


/* 	// Exit control: */
/* 	min = VM_EXIT_HOST_ADDR_SPACE_SIZE//; */
/* 		| VM_EXIT_ACK_INTR_ON_EXIT; */
/* 	opt = 0; */
/* 	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_EXIT_CTLS, */
/* 					&_vmexit_control) < 0) */
/* 		return -EIO; */

/* 	// Entry control: */
/* 	min = VM_ENTRY_IA32E_MODE; */
/* 	opt = 0; */
/* 	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_ENTRY_CTLS, */
/* 					&_vmentry_control) < 0) */
/* 		return -EIO; */

/* 	rdmsr(MSR_IA32_VMX_BASIC, vmx_msr_low, vmx_msr_high); */

/* 	/\* IA-32 SDM Vol 3B: VMCS size is never greater than 4kB. *\/ */
/* 	if ((vmx_msr_high & 0x1fff) > PAGE_SIZE) */
/* 		return -EIO; */

/* 	/\* IA-32 SDM Vol 3B: 64-bit CPUs always have VMX_BASIC_MSR[48]==0. *\/ */
/* 	if (vmx_msr_high & (1u<<16)) */
/* 		return -EIO; */

/* 	/\* Require Write-Back (WB) memory type for VMCS accesses. *\/ */
/* 	if (((vmx_msr_high >> 18) & 15) != 6) */
/* 		return -EIO; */

/* 	vmcs_conf->size = vmx_msr_high & 0x1fff; */
/* 	vmcs_conf->order = get_order(vmcs_config.size); */
/* 	vmcs_conf->revision_id = vmx_msr_low; */

/* 	vmcs_conf->pin_based_exec_ctrl = _pin_based_exec_control; */
/* 	vmcs_conf->cpu_based_exec_ctrl = _cpu_based_exec_control; */
/* 	vmcs_conf->cpu_based_2nd_exec_ctrl = _cpu_based_2nd_exec_control; */
/* 	vmcs_conf->vmexit_ctrl         = _vmexit_control; */
/* 	vmcs_conf->vmentry_ctrl        = _vmentry_control; */

/* 	vmx_capability.has_load_efer = */
/* 		allow_1_setting(MSR_IA32_VMX_ENTRY_CTLS, */
/* 				VM_ENTRY_LOAD_IA32_EFER) */
/* 		&& allow_1_setting(MSR_IA32_VMX_EXIT_CTLS, */
/* 				VM_EXIT_LOAD_IA32_EFER); */

/* 	return 0; */
/* } */

/* static struct vmcs *vmx_alloc_vmcs(int cpu) { */
/* 	int node = cpu_to_node(cpu); */
/* 	struct page *pages; */
/* 	struct vmcs *vmcs; */

/* 	pages = alloc_pages_exact_node(node, GFP_KERNEL, vmcs_config.order); */
/* 	if (!pages) */
/* 		return NULL; */
/* 	vmcs = page_address(pages); */
/* 	memset(vmcs, 0, vmcs_config.size); */
/* 	vmcs->revision_id = vmcs_config.revision_id; /\* vmcs revision id *\/ */
/* 	return vmcs; */
/* } */

/* static void vmx_free_vmcs(struct vmcs *vmcs) { */
/* 	free_pages((unsigned long)vmcs, vmcs_config.order); */
/* } */

/* static void vmx_setup_constant_host_state(struct lcd *lcd) { */
/* 	u32 low32, high32; */
/* 	unsigned long tmpl; */
/* 	struct desc_ptr dt; */

/* 	vmcs_writel(HOST_CR0, read_cr0() & ~X86_CR0_TS);  /\* 22.2.3 *\/ */
/* 	vmcs_writel(HOST_CR4, read_cr4());  /\* 22.2.3, 22.2.5 *\/ */
/* 	vmcs_writel(HOST_CR3, read_cr3());  /\* 22.2.3 *\/ */

/* 	vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);  /\* 22.2.4 *\/ */
/* 	vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /\* 22.2.4 *\/ */
/* 	vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /\* 22.2.4 *\/ */
/* 	vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);  /\* 22.2.4 *\/ */
/* 	vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);  /\* 22.2.4 *\/ */

/* 	native_store_idt(&dt); */
/* 	vmcs_writel(HOST_IDTR_BASE, dt.address);   /\* 22.2.4 *\/ */
/* 	lcd->host_idt_base = dt.address; */

/* 	asm("mov $.Llcd_vmx_return, %0" : "=r"(tmpl)); */
/* 	vmcs_writel(HOST_RIP, tmpl); /\* 22.2.5 *\/ */

/* 	rdmsr(MSR_IA32_SYSENTER_CS, low32, high32); */
/* 	vmcs_write32(HOST_IA32_SYSENTER_CS, low32); */
/* 	rdmsrl(MSR_IA32_SYSENTER_EIP, tmpl); */
/* 	vmcs_writel(HOST_IA32_SYSENTER_EIP, tmpl);   /\* 22.2.3 *\/ */

/* 	rdmsr(MSR_EFER, low32, high32); */
/* 	vmcs_write32(HOST_IA32_EFER, low32); */

/* 	if (vmcs_config.vmexit_ctrl & VM_EXIT_LOAD_IA32_PAT) { */
/* 		printk(KERN_INFO "lcd: we need to load PAT on exit?\n"); */
/* 		rdmsr(MSR_IA32_CR_PAT, low32, high32); */
/* 		vmcs_write64(HOST_IA32_PAT, low32 | ((u64) high32 << 32)); */
/* 	} */

/* 	vmcs_write16(HOST_FS_SELECTOR, 0);            /\* 22.2.4 *\/ */
/* 	vmcs_write16(HOST_GS_SELECTOR, 0);            /\* 22.2.4 *\/ */

/* 	rdmsrl(MSR_FS_BASE, tmpl); */
/* 	vmcs_writel(HOST_FS_BASE, tmpl); /\* 22.2.4 *\/ */
/* 	rdmsrl(MSR_GS_BASE, tmpl); */
/* 	vmcs_writel(HOST_GS_BASE, tmpl); /\* 22.2.4 *\/ */
/* } */

/* static void vmx_disable_intercept_for_msr( */
/* 	unsigned long *msr_bitmap, u32 msr) { */
/* 	int f = sizeof(unsigned long); */
/* 	/\* */
/* 	 * See Intel PRM Vol. 3, 20.6.9 (MSR-Bitmap Address). Early manuals */
/* 	 * have the write-low and read-high bitmap offsets the wrong way round. */
/* 	 * We can control MSRs 0x00000000-0x00001fff and 0xc0000000-0xc0001fff. */
/* 	 *\/ */
/* 	if (msr <= 0x1fff) { */
/* 		__clear_bit(msr, msr_bitmap + 0x000 / f); /\* read-low *\/ */
/* 		__clear_bit(msr, msr_bitmap + 0x800 / f); /\* write-low *\/ */
/* 	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) { */
/* 		msr &= 0x1fff; */
/* 		__clear_bit(msr, msr_bitmap + 0x400 / f); /\* read-high *\/ */
/* 		__clear_bit(msr, msr_bitmap + 0xc00 / f); /\* write-high *\/ */
/* 	} */
/* } */

/* static void setup_msr(struct lcd *vcpu) { */
/* 	int set[] = { MSR_LSTAR }; */
/* 	struct vmx_msr_entry *e; */
/* 	int sz = sizeof(set) / sizeof(*set); */
/* 	int i; */

/* 	sz = 0; */

/* 	BUILD_BUG_ON(sz > NR_AUTOLOAD_MSRS); */

/* 	vcpu->msr_autoload.nr = sz; */

/* 	/\* XXX enable only MSRs in set *\/ */
/* 	vmcs_write64(MSR_BITMAP, __pa(msr_bitmap)); */

/* 	vmcs_write32(VM_EXIT_MSR_STORE_COUNT, vcpu->msr_autoload.nr); */
/* 	vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, vcpu->msr_autoload.nr); */
/* 	vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, vcpu->msr_autoload.nr); */

/* 	vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.host)); */
/* 	vmcs_write64(VM_EXIT_MSR_STORE_ADDR, __pa(vcpu->msr_autoload.guest)); */
/* 	vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.guest)); */

/* 	for (i = 0; i < sz; i++) { */
/* 		uint64_t val; */

/* 		e = &vcpu->msr_autoload.host[i]; */
/* 		e->index = set[i]; */
/* 		rdmsrl(e->index, val); */
/* 		e->value = val; */

/* 		e = &vcpu->msr_autoload.guest[i]; */
/* 		e->index = set[i]; */
/* 	} */
/* } */

/* static void vmx_setup_vmcs(struct lcd *vcpu) */
/* { */
/* 	vmcs_write16(VIRTUAL_PROCESSOR_ID, vcpu->vpid); */
/* 	vmcs_write64(VMCS_LINK_POINTER, -1ull); /\* 22.3.1.5 *\/ */

/* 	/\* Control *\/ */
/* 	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL, */
/* 		vmcs_config.pin_based_exec_ctrl); */

/* 	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, */
/* 		vmcs_config.cpu_based_exec_ctrl); */

/* 	if (cpu_has_secondary_exec_ctrls()) { */
/* 		vmcs_write32(SECONDARY_VM_EXEC_CONTROL, */
/* 			vmcs_config.cpu_based_2nd_exec_ctrl); */
/* 	} */

/* 	vmcs_write64(EPT_POINTER, vcpu->eptp); */

/* 	vmcs_write32(EXCEPTION_BITMAP, 0xffffffff); */
/* 	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK, 0); */
/* 	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH, 0); */
/* 	vmcs_write32(CR3_TARGET_COUNT, 0);           /\* 22.2.1 *\/ */

/* 	setup_msr(vcpu); */
  
/* #if 0 */
/* 	// We don't need any MSR and PAT> */
/* 	if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) { */
/* 		u32 msr_low, msr_high; */
/* 		u64 host_pat; */
/* 		rdmsr(MSR_IA32_CR_PAT, msr_low, msr_high); */
/* 		host_pat = msr_low | ((u64) msr_high << 32); */
/* 		/\* Write the default value follow host pat *\/ */
/* 		vmcs_write64(GUEST_IA32_PAT, host_pat); */
/* 		/\* Keep arch.pat sync with GUEST_IA32_PAT *\/ */
/* 		vmx->vcpu.arch.pat = host_pat; */
/* 	} */

/* 	for (i = 0; i < NR_VMX_MSR; ++i) { */
/* 		u32 index = vmx_msr_index[i]; */
/* 		u32 data_low, data_high; */
/* 		int j = vmx->nmsrs; */

/* 		if (rdmsr_safe(index, &data_low, &data_high) < 0) */
/* 			continue; */
/* 		if (wrmsr_safe(index, data_low, data_high) < 0) */
/* 			continue; */
/* 		vmx->guest_msrs[j].index = i; */
/* 		vmx->guest_msrs[j].data = 0; */
/* 		vmx->guest_msrs[j].mask = -1ull; */
/* 		++vmx->nmsrs; */
/* 	} */
/* #endif */

/* 	//vmcs_config.vmentry_ctrl |= VM_ENTRY_IA32E_MODE; */

/* 	vmcs_write32(VM_EXIT_CONTROLS, vmcs_config.vmexit_ctrl); */
/* 	vmcs_write32(VM_ENTRY_CONTROLS, vmcs_config.vmentry_ctrl); */

/* 	vmcs_writel(CR0_GUEST_HOST_MASK, ~0ul); */
/* 	vmcs_writel(CR4_GUEST_HOST_MASK, ~0ul); */

/* 	vmcs_writel(TSC_OFFSET, 0); */

/* 	vmx_setup_constant_host_state(vcpu); */
/* } */

/* /\** */
/*  * vmx_allocate_vpid - reserves a vpid and sets it in the vcpu */
/*  *\/ */
/* static int vmx_allocate_vpid(struct lcd *vmx) */
/* { */
/* 	int vpid; */

/* 	vmx->vpid = 0; */

/* 	spin_lock(&vpids.lock); */
/* 	vpid = find_first_zero_bit(vpids.bitmap, VMX_NR_VPIDS); */
/* 	if (vpid < VMX_NR_VPIDS) { */
/* 		vmx->vpid = vpid; */
/* 		__set_bit(vpid, vpids.bitmap); */
/* 	} */
/* 	spin_unlock(&vpids.lock); */

/* 	return vpid >= VMX_NR_VPIDS; */
/* } */

/* /\** */
/*  * vmx_free_vpid - frees a vpid */
/*  * @vmx: the VCPU */
/*  *\/ */
/* static void vmx_free_vpid(struct lcd *vmx) */
/* { */
/* 	spin_lock(&vmx_vpid_lock); */
/* 	if (vmx->vpid != 0) */
/* 		__clear_bit(vmx->vpid, vmx_vpid_bitmap); */
/* 	spin_unlock(&vmx_vpid_lock); */
/* } */

/* static void vmx_setup_initial_guest_state(struct lcd *vcpu,  */
/* 					u64 guest_paging_cr3) */
/* { */
/* 	/\* unsigned long tmpl; *\/ */
/* 	unsigned long cr4 = X86_CR4_PAE | X86_CR4_VMXE | X86_CR4_OSXMMEXCPT | */
/* 		X86_CR4_PGE | X86_CR4_OSFXSR; */
/* 	/\* if (boot_cpu_has(X86_FEATURE_PCID)) *\/ */
/* 	/\* cr4 |= X86_CR4_PCIDE; *\/ */
/* 	if (boot_cpu_has(X86_FEATURE_OSXSAVE)) */
/* 		cr4 |= X86_CR4_OSXSAVE; */
/* 	if (boot_cpu_has(X86_FEATURE_FSGSBASE)) */
/* 		cr4 |= X86_CR4_RDWRGSFS; */

/* 	/\* configure control and data registers *\/ */
/* 	vmcs_writel(GUEST_CR0, X86_CR0_PG | X86_CR0_PE | X86_CR0_WP | */
/* 		X86_CR0_MP | X86_CR0_ET | X86_CR0_NE); */
/* 	vmcs_writel(CR0_READ_SHADOW, X86_CR0_PG | X86_CR0_PE | X86_CR0_WP | */
/* 		X86_CR0_MP | X86_CR0_ET | X86_CR0_NE); */
/* 	vmcs_writel(GUEST_CR3, guest_paging_cr3); */
/* 	vmcs_writel(GUEST_CR4, cr4); */
/* 	vmcs_writel(CR4_READ_SHADOW, cr4); */
/* 	vmcs_writel(GUEST_IA32_EFER, EFER_LME | EFER_LMA | */
/* 		EFER_SCE | EFER_FFXSR); */
/* 	vmcs_writel(GUEST_GDTR_BASE, LCD_GDT_ADDR); */
/* 	vmcs_writel(GUEST_GDTR_LIMIT, GDT_SIZE); */
/* 	vmcs_writel(GUEST_IDTR_BASE, LCD_IDT_ADDR); */
/* 	vmcs_writel(GUEST_IDTR_LIMIT, IDT_ENTRIES*16); */
/* 	vmcs_writel(GUEST_RIP, LCD_TEST_CODE_ADDR); */
/* 	// commented out to test the dynamically allocated stack */
/* 	// vmcs_writel(GUEST_RSP, LCD_STACK_ADDR); */
/* 	vmcs_writel(GUEST_RFLAGS, 0x02); */
/* 	vmcs_writel(GUEST_DR7, 0); */

/* 	/\* guest segment bases *\/ */
/* 	vmcs_writel(GUEST_CS_BASE, 0); */
/* 	vmcs_writel(GUEST_DS_BASE, 0); */
/* 	vmcs_writel(GUEST_ES_BASE, 0); */
/* 	vmcs_writel(GUEST_GS_BASE, 0); */
/* 	vmcs_writel(GUEST_SS_BASE, 0); */
/* 	/\* rdmsrl(MSR_FS_BASE, tmpl); *\/ */
/* 	vmcs_writel(GUEST_FS_BASE, 0); */

/* 	/\* guest segment access rights *\/ */
/* 	vmcs_writel(GUEST_CS_AR_BYTES, 0xA09B); */
/* 	vmcs_writel(GUEST_DS_AR_BYTES, 0xA093); */
/* 	vmcs_writel(GUEST_ES_AR_BYTES, 0xA093); */
/* 	vmcs_writel(GUEST_FS_AR_BYTES, 0xA093); */
/* 	vmcs_writel(GUEST_GS_AR_BYTES, 0xA093); */
/* 	vmcs_writel(GUEST_SS_AR_BYTES, 0xA093); */

/* 	/\* guest segment limits *\/ */
/* 	vmcs_write32(GUEST_CS_LIMIT, 0xFFFFFFFF); */
/* 	vmcs_write32(GUEST_DS_LIMIT, 0xFFFFFFFF); */
/* 	vmcs_write32(GUEST_ES_LIMIT, 0xFFFFFFFF); */
/* 	vmcs_write32(GUEST_FS_LIMIT, 0xFFFFFFFF); */
/* 	vmcs_write32(GUEST_GS_LIMIT, 0xFFFFFFFF); */
/* 	vmcs_write32(GUEST_SS_LIMIT, 0xFFFFFFFF); */

/* 	/\* configure segment selectors *\/ */
/* 	vmcs_write16(GUEST_CS_SELECTOR, __KERNEL_CS); // code */
/* 	vmcs_write16(GUEST_DS_SELECTOR, __KERNEL_DS); // data */
/* 	vmcs_write16(GUEST_ES_SELECTOR, 0); */
/* 	vmcs_write16(GUEST_FS_SELECTOR, 0); */
/* 	vmcs_write16(GUEST_GS_SELECTOR, 0); */
/* 	vmcs_write16(GUEST_SS_SELECTOR, 0); // data */
/* 	vmcs_write16(GUEST_TR_SELECTOR, 0); */

/* 	/\* guest LDTR *\/ */
/* 	vmcs_write16(GUEST_LDTR_SELECTOR, 0); */
/* 	vmcs_writel(GUEST_LDTR_AR_BYTES, 0x0082); */
/* 	vmcs_writel(GUEST_LDTR_BASE, 0); */
/* 	vmcs_writel(GUEST_LDTR_LIMIT, 0); */

/* 	/\* guest TSS *\/ */
/* 	vmcs_writel(GUEST_TR_BASE, LCD_TSS_ADDR); */
/* 	vmcs_writel(GUEST_TR_AR_BYTES, 0x0080 | AR_TYPE_BUSY_64_TSS); */
/* 	vmcs_writel(GUEST_TR_LIMIT, LCD_TSS_SIZE); */

/* 	/\* initialize sysenter *\/ */
/* 	vmcs_write32(GUEST_SYSENTER_CS, 0); */
/* 	vmcs_writel(GUEST_SYSENTER_ESP, 0); */
/* 	vmcs_writel(GUEST_SYSENTER_EIP, 0); */

/* 	/\* other random initialization *\/ */
/* 	vmcs_write32(GUEST_ACTIVITY_STATE, GUEST_ACTIVITY_ACTIVE); */
/* 	vmcs_write32(GUEST_INTERRUPTIBILITY_INFO, 0); */
/* 	vmcs_write32(GUEST_PENDING_DBG_EXCEPTIONS, 0); */
/* 	vmcs_write64(GUEST_IA32_DEBUGCTL, 0); */
/* 	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, 0);  /\* 22.2.1 *\/ */
/* } */

/* static int setup_gdt(struct lcd* vcpu) { */
/* 	struct desc_struct *desc; */
/* 	tss_desc* tssd; */
/* 	struct x86_hw_tss* tss; */

/* 	memset(vcpu->gdt, 0, GDT_SIZE); */

/* 	desc = vcpu->gdt + GDT_ENTRY_KERNEL_CS; */
/* 	/\* ignored fields according to APM Vol.3 Ch4.8 *\/ */
/* 	/\* code seg desc *\/ */
/* 	desc->type = SEG_TYPE_CODE | SEG_TYPE_EXEC_READ; */
/* 	desc->s = DESC_TYPE_CODE_DATA; */
/* 	desc->dpl = 0; */
/* 	desc->p = 1; */
/* 	desc->l = 1; */
/* 	desc->d = 0; */

/* 	/\* data seg desc *\/ */
/* 	desc = vcpu->gdt + GDT_ENTRY_KERNEL_DS; */
/* 	desc->type = SEG_TYPE_DATA | SEG_TYPE_READ_WRITE; */
/* 	desc->s = DESC_TYPE_CODE_DATA; */
/* 	desc->p = 1; */

/* 	/\* task segment desc value *\/ */
/* 	tssd = (tss_desc*)(vcpu->gdt + GDT_ENTRY_TSS); */
/* 	set_tssldt_descriptor(tssd, LCD_TSS_ADDR, DESC_TSS, LCD_TSS_SIZE); */

/* 	/\* TSS segment *\/ */
/* 	memset(vcpu->tss, 0, PAGE_SIZE); */
/* 	tss = &vcpu->tss->tss; */
/* 	tss->sp0 = LCD_STACK_ADDR; */
/* 	tss->io_bitmap_base = offsetof(struct lcd_tss_struct, io_bitmap); */
/* 	tss->ist[0] = LCD_TSS_ADDR + (PAGE_SIZE>>1); */
/* 	vcpu->tss->io_bitmap[0] = 0xff; */

/* 	return 0; */
/* } */

/* static void setup_idt(struct lcd* vcpu) { */
/* 	int i; */
/* 	memset(vcpu->idt, 0, PAGE_SIZE); */
/* 	/\* Just fill the IDT  *\/ */
/* 	for (i = 0; i < IDT_ENTRIES; ++i) { */
/* 		gate_desc* gate = vcpu->idt + i; */
/* 		pack_gate(gate, GATE_INTERRUPT, LCD_COMM_ISR_ADDR, 0, 1, __KERNEL_CS); */
/* 	} */
/* } */

// Some temporary capability functions for IPC testing
// To be replaced while interfacing with capability module
/* static u32 curr_cap = 1; */
/* void * cap_arr[10]= {0}; */

/* void * get_cap_obj(u32 cap_id){ */
/* 	return cap_arr[cap_id]; */
/* } */

/* u32 set_cap(void *obj){ */
/* 	cap_arr[curr_cap] = obj; */
/* 	return curr_cap++; */
/* } */

/* static struct lcd * vmx_create_vcpu(void) { */
/* 	struct lcd *vcpu = kmalloc(sizeof(struct lcd), GFP_KERNEL); */
/* 	u64 guest_paging_cr3; */

/* 	if (!vcpu) */
/* 		return NULL; */

/* 	memset(vcpu, 0, sizeof(*vcpu)); */

/* 	vcpu->bmp_pt_pages = kmalloc( */
/* 		sizeof(long) * BITS_TO_LONGS(LCD_NR_PT_PAGES), */
/* 		GFP_KERNEL); */
/* 	if (!vcpu->bmp_pt_pages) */
/* 		goto fail_bmp; */
/* 	bitmap_zero(vcpu->bmp_pt_pages, LCD_NR_PT_PAGES); */

/* 	vcpu->gdt = (struct desc_struct*)__get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->gdt) */
/* 		goto fail_gdt; */

/* 	vcpu->idt = (gate_desc*)__get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->idt) */
/* 		goto fail_idt; */

/* 	vcpu->tss = (struct lcd_tss_struct*)__get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->tss) */
/* 		goto fail_tss; */

/* 	vcpu->isr_page = __get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->isr_page) */
/* 		goto fail_isr; */
/* 	memset((void*)vcpu->isr_page, 0, PAGE_SIZE); */
/* 	*((u8*)vcpu->isr_page) = 0xf4; //HLT */

/* 	vcpu->vmcs = vmx_alloc_vmcs(raw_smp_processor_id()); */
/* 	if (!vcpu->vmcs) */
/* 		goto fail_vmcs; */

/* 	if (vmx_allocate_vpid(vcpu)) */
/* 		goto fail_vpid; */

/* 	vcpu->cpu = -1; */

/* 	spin_lock_init(&vcpu->ept_lock); */
/* 	if (vmx_init_ept(vcpu)) */
/* 		goto fail_ept; */
/* 	vcpu->eptp = construct_eptp(vcpu->ept_root); */

/* 	if (cpu_has_vmx_ept_ad_bits()) { */
/* 		vcpu->ept_ad_enabled = true; */
/* 		printk(KERN_INFO "vmx: enabled EPT A/D bits\n"); */
/* 	} */
/* 	if (lcd_setup_addr_space(vcpu, &guest_paging_cr3)) { */
/* 		printk(KERN_ERR "vmx: creating addr space failed.\n"); */
/* 		goto fail_ept; */
/* 	} */

/* 	vcpu->bp = (struct boot_params*)__get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->bp) { */
/* 		printk(KERN_ERR "vmx: out of mem for boot params\n"); */
/* 		goto fail_ept; */
/* 	} */
/* 	memset(vcpu->bp, 0, sizeof(struct boot_params)); */

/* 	vcpu->si = (struct start_info*)__get_free_page(GFP_KERNEL); */
/* 	if (!vcpu->si) { */
/* 		printk(KERN_ERR "vmx: out of mem for start info\n"); */
/* 		goto fail_si; */
/* 	} */
/* 	memset(vcpu->si, 0, sizeof(struct start_info)); */
  

/* 	vmx_get_cpu(vcpu); */
/* 	vmx_setup_vmcs(vcpu); */
/* 	vmx_setup_initial_guest_state(vcpu, guest_paging_cr3); */
/* 	setup_gdt(vcpu); */
/* 	setup_idt(vcpu); */
/* 	vmx_put_cpu(vcpu); */

/* 	//IPC queues  */
/* 	INIT_LIST_HEAD(&vcpu->sync_ipc.snd_q); */
/* 	vcpu->sync_ipc.task = current; */
/* 	vcpu->sync_ipc.state = IPC_DONT_CARE; */
/* 	vcpu->sync_ipc.expected_sender = 0; */
/* 	vcpu->sync_ipc.my_capid =  set_cap(vcpu); */
/* 	// printk(KERN_ERR "=========%d======\n", vcpu->sync_ipc.my_capid); */


/* 	return vcpu; */

/* fail_si: */
/* 	free_page((unsigned long) vcpu->bp); */
/* fail_ept: */
/* 	vmx_free_vpid(vcpu); */
/* fail_vpid: */
/* 	vmx_free_vmcs(vcpu->vmcs); */
/* fail_vmcs: */
/* 	free_page((unsigned long)vcpu->isr_page); */
/* fail_isr: */
/* 	free_page((unsigned long)vcpu->tss); */
/* fail_tss: */
/* 	free_page((unsigned long)vcpu->idt); */
/* fail_idt: */
/* 	free_page((unsigned long)vcpu->gdt); */
/* fail_gdt: */
/* 	kfree(vcpu->bmp_pt_pages); */
/* fail_bmp: */
/* 	kfree(vcpu); */

/* 	return NULL; */
/* } */

/* static void vmx_destroy_vcpu(struct lcd *vcpu) */
/* { */
/* 	vmx_free_ept(vcpu->ept_root); */
/* 	vmx_get_cpu(vcpu); */
/* 	ept_sync_context(vcpu->eptp); */
/* 	vmcs_clear(vcpu->vmcs); */
/* 	__get_cpu_var(local_vcpu) = NULL; */
/* 	vmx_put_cpu(vcpu); */
/* 	vmx_free_vpid(vcpu); */
/* 	vmx_free_vmcs(vcpu->vmcs); */
/* 	kfree(vcpu->bmp_pt_pages); */
/* 	free_page((unsigned long) vcpu->bp); */
/* 	free_page((unsigned long) vcpu->si); */
/* 	kfree(vcpu); */
/* } */

/* static int __noclone vmx_run_vcpu(struct lcd *vcpu) { */
/* 	asm( */
/* 		/\* Store host registers *\/ */
/* 		"push %%rdx; push %%rbp;" */
/* 		"push %%rcx \n\t" /\* placeholder for guest rcx *\/ */
/* 		"push %%rcx \n\t" */
/* 		"cmp %%rsp, %c[host_rsp](%0) \n\t" */
/* 		"je 1f \n\t" */
/* 		"mov %%rsp, %c[host_rsp](%0) \n\t" */
/* 		ASM_VMX_VMWRITE_RSP_RDX "\n\t" */
/* 		"1: \n\t" */
/* 		/\* Reload cr2 if changed *\/ */
/* 		"mov %c[cr2](%0), %%rax \n\t" */
/* 		"mov %%cr2, %%rdx \n\t" */
/* 		"cmp %%rax, %%rdx \n\t" */
/* 		"je 2f \n\t" */
/* 		"mov %%rax, %%cr2 \n\t" */
/* 		"2: \n\t" */
/* 		/\* Check if vmlaunch of vmresume is needed *\/ */
/* 		"cmpl $0, %c[launched](%0) \n\t" */
/* 		/\* Load guest registers.  Don't clobber flags. *\/ */
/* 		"mov %c[rax](%0), %%rax \n\t" */
/* 		"mov %c[rbx](%0), %%rbx \n\t" */
/* 		"mov %c[rdx](%0), %%rdx \n\t" */
/* 		"mov %c[rsi](%0), %%rsi \n\t" */
/* 		"mov %c[rdi](%0), %%rdi \n\t" */
/* 		"mov %c[rbp](%0), %%rbp \n\t" */
/* 		"mov %c[r8](%0),  %%r8  \n\t" */
/* 		"mov %c[r9](%0),  %%r9  \n\t" */
/* 		"mov %c[r10](%0), %%r10 \n\t" */
/* 		"mov %c[r11](%0), %%r11 \n\t" */
/* 		"mov %c[r12](%0), %%r12 \n\t" */
/* 		"mov %c[r13](%0), %%r13 \n\t" */
/* 		"mov %c[r14](%0), %%r14 \n\t" */
/* 		"mov %c[r15](%0), %%r15 \n\t" */
/* 		"mov %c[rcx](%0), %%rcx \n\t" /\* kills %0 (ecx) *\/ */

/* 		/\* Enter guest mode *\/ */
/* 		"jne .Llaunched \n\t" */
/* 		ASM_VMX_VMLAUNCH "\n\t" */
/* 		"jmp .Llcd_vmx_return \n\t" */
/* 		".Llaunched: " ASM_VMX_VMRESUME "\n\t" */
/* 		".Llcd_vmx_return: " */
/* 		/\* Save guest registers, load host registers, keep flags *\/ */
/* 		"mov %0, %c[wordsize](%%rsp) \n\t" */
/* 		"pop %0 \n\t" */
/* 		"mov %%rax, %c[rax](%0) \n\t" */
/* 		"mov %%rbx, %c[rbx](%0) \n\t" */
/* 		"popq %c[rcx](%0) \n\t" */
/* 		"mov %%rdx, %c[rdx](%0) \n\t" */
/* 		"mov %%rsi, %c[rsi](%0) \n\t" */
/* 		"mov %%rdi, %c[rdi](%0) \n\t" */
/* 		"mov %%rbp, %c[rbp](%0) \n\t" */
/* 		"mov %%r8,  %c[r8](%0) \n\t" */
/* 		"mov %%r9,  %c[r9](%0) \n\t" */
/* 		"mov %%r10, %c[r10](%0) \n\t" */
/* 		"mov %%r11, %c[r11](%0) \n\t" */
/* 		"mov %%r12, %c[r12](%0) \n\t" */
/* 		"mov %%r13, %c[r13](%0) \n\t" */
/* 		"mov %%r14, %c[r14](%0) \n\t" */
/* 		"mov %%r15, %c[r15](%0) \n\t" */
/* 		"mov %%rax, %%r10 \n\t" */
/* 		"mov %%rdx, %%r11 \n\t" */

/* 		"mov %%cr2, %%rax   \n\t" */
/* 		"mov %%rax, %c[cr2](%0) \n\t" */

/* 		"pop  %%rbp; pop  %%rdx \n\t" */
/* 		"setbe %c[fail](%0) \n\t" */

/* 		"mov $" __stringify(__USER_DS) ", %%rax \n\t" */
/* 		"mov %%rax, %%ds \n\t" */
/* 		"mov %%rax, %%es \n\t" */
/* 		: : "c"(vcpu), "d"((unsigned long)HOST_RSP), */
/* 		  [launched]"i"(offsetof(struct lcd, launched)), */
/* 		  [fail]"i"(offsetof(struct lcd, fail)), */
/* 		  [host_rsp]"i"(offsetof(struct lcd, host_rsp)), */
/* 		  [rax]"i"(offsetof(struct lcd, regs[VCPU_REGS_RAX])), */
/* 		  [rbx]"i"(offsetof(struct lcd, regs[VCPU_REGS_RBX])), */
/* 		  [rcx]"i"(offsetof(struct lcd, regs[VCPU_REGS_RCX])), */
/* 		  [rdx]"i"(offsetof(struct lcd, regs[VCPU_REGS_RDX])), */
/* 		  [rsi]"i"(offsetof(struct lcd, regs[VCPU_REGS_RSI])), */
/* 		  [rdi]"i"(offsetof(struct lcd, regs[VCPU_REGS_RDI])), */
/* 		  [rbp]"i"(offsetof(struct lcd, regs[VCPU_REGS_RBP])), */
/* 		  [r8]"i"(offsetof(struct lcd, regs[VCPU_REGS_R8])), */
/* 		  [r9]"i"(offsetof(struct lcd, regs[VCPU_REGS_R9])), */
/* 		  [r10]"i"(offsetof(struct lcd, regs[VCPU_REGS_R10])), */
/* 		  [r11]"i"(offsetof(struct lcd, regs[VCPU_REGS_R11])), */
/* 		  [r12]"i"(offsetof(struct lcd, regs[VCPU_REGS_R12])), */
/* 		  [r13]"i"(offsetof(struct lcd, regs[VCPU_REGS_R13])), */
/* 		  [r14]"i"(offsetof(struct lcd, regs[VCPU_REGS_R14])), */
/* 		  [r15]"i"(offsetof(struct lcd, regs[VCPU_REGS_R15])), */
/* 		  [cr2]"i"(offsetof(struct lcd, cr2)), */
/* 		  [wordsize]"i"(sizeof(ulong)) */
/* 		: "cc", "memory" */
/* 		  , "rax", "rbx", "rdi", "rsi" */
/* 		  , "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" */
/* 		); */

/* 	vcpu->launched = 1; */

/* 	if (unlikely(vcpu->fail)) { */
/* 		printk(KERN_ERR "vmx: failure detected (err %x)\n", */
/* 			vmcs_read32(VM_INSTRUCTION_ERROR)); */
/* 		return VMX_EXIT_REASONS_FAILED_VMENTRY; */
/* 	} */

/* 	vcpu->exit_reason = vmcs_read32(VM_EXIT_REASON); */
/* 	vcpu->exit_qualification = vmcs_readl(EXIT_QUALIFICATION); */
/* 	vcpu->idt_vectoring_info = vmcs_read32(IDT_VECTORING_INFO_FIELD); */
/* 	vcpu->error_code = vmcs_read32(IDT_VECTORING_ERROR_CODE); */
/* 	vcpu->exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO); */
/* 	vcpu->vec_no = vcpu->exit_intr_info & INTR_INFO_VECTOR_MASK; */

/* 	return vcpu->exit_reason; */
/* } */

/* static void vmx_step_instruction(void) { */
/* 	vmcs_writel(GUEST_RIP, vmcs_readl(GUEST_RIP) + */
/* 		vmcs_read32(VM_EXIT_INSTRUCTION_LEN)); */
/* } */

/* static int vmx_handle_ept_violation(struct lcd *vcpu) { */
/* 	unsigned long gva, gpa; */
/* 	int ret; */

/* 	vmx_get_cpu(vcpu); */
/* 	gva = vmcs_readl(GUEST_LINEAR_ADDRESS); */
/* 	gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS); */
/* 	vmx_put_cpu(vcpu); */

/* 	if (vcpu->exit_qualification & (1 << 6)) { */
/* 		printk(KERN_ERR "EPT: GPA 0x%lx exceeds GAW!\n", gpa); */
/* 		return -EINVAL; */
/* 	} */

/* 	if (!(vcpu->exit_qualification & (1 << 7))) { */
/* 		printk(KERN_ERR "EPT: linear address is not valid, GPA: 0x%lx!\n", gpa); */
/* 		return -EINVAL; */
/* 	} */

/* 	/\* */
/* 	 * EPT Fault. */
/* 	 * TODO: lock page table? */
/* 	 *\/ */
/* 	if (gpa < LCD_PHY_MEM_SIZE) { */
/* 		u64 gpa_pg; */
/* 		u64 pg = __get_free_page(GFP_KERNEL); */
/* 		if (!pg) */
/* 			ret = -ENOMEM; */
/* 		else { */
/* 			gpa_pg = round_down(gpa, PAGE_SIZE); */
/* 			ret = lcd_map_gpa_to_hpa(vcpu, gpa_pg, __pa(pg), 0); */
/* 			if (ret) { */
/* 				printk(KERN_ERR "EPT: map page %p for %p failed\n", */
/* 					(void*)pg, (void*)gpa_pg); */
/* 				free_page(pg); */
/* 			} */
/* 		} */
/* 	} else */
/* 		ret = -EINVAL; */

/* 	if (ret) { */
/* 		printk(KERN_ERR "vmx: EPT violation failure " */
/* 			"GPA: 0x%lx, GVA: 0x%lx\n", */
/* 			gpa, gva); */
/* 		vcpu->ret_code = ((EFAULT) << 8); */
/* 		vmx_dump_cpu(vcpu); */
/* 	} */

/* 	return ret; */
/* } */

/* static int __vmx_enable(struct vmcs *vmxon_buf) { */
/* 	u64 phys_addr = __pa(vmxon_buf); */
/* 	u64 old, test_bits; */

/* 	if (read_cr4() & X86_CR4_VMXE) */
/* 		return -EBUSY; */

/* 	rdmsrl(MSR_IA32_FEATURE_CONTROL, old); */

/* 	test_bits = FEATURE_CONTROL_LOCKED; */
/* 	test_bits |= FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX; */
/* 	if (tboot_enabled()) */
/* 		test_bits |= FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX; */

/* 	if ((old & test_bits) != test_bits) { */
/* 		/\* enable and lock *\/ */
/* 		wrmsrl(MSR_IA32_FEATURE_CONTROL, old | test_bits); */
/* 	} */
/* 	write_cr4(read_cr4() | X86_CR4_VMXE); */

/* 	__vmxon(phys_addr); */
/* 	vpid_sync_vcpu_global(); */
/* 	ept_sync_global(); */

/* 	return 0; */
/* } */

/* #define store_gdt(g) native_store_gdt(g) */

/* static void vmx_enable(void *unused) { */
/* 	int ret; */
/* 	struct vmcs *vmxon_buf = __get_cpu_var(vmxarea); */

/* 	if ((ret = __vmx_enable(vmxon_buf))) */
/* 		goto failed; */

/* 	__get_cpu_var(vmx_enabled) = 1; */
/* 	store_gdt(&__get_cpu_var(host_gdt)); */

/* 	printk(KERN_INFO "vmx: VMX enabled on CPU %d\n", */
/* 		raw_smp_processor_id()); */
/* 	return; */

/* failed: */
/* 	atomic_inc(&vmx_enable_failed); */
/* 	printk(KERN_ERR "vmx: failed to enable VMX, err = %d\n", ret); */
/* } */

/* static void vmx_disable(void *unused) { */
/* 	if (__get_cpu_var(vmx_enabled)) { */
/* 		__vmxoff(); */
/* 		write_cr4(read_cr4() & ~X86_CR4_VMXE); */
/* 		__get_cpu_var(vmx_enabled) = 0; */
/* 	} */
/* } */

/* static void vmx_free_vmxon_areas(void) { */
/* 	int cpu; */

/* 	for_each_possible_cpu(cpu) { */
/* 		if (per_cpu(vmxarea, cpu)) { */
/* 			vmx_free_vmcs(per_cpu(vmxarea, cpu)); */
/* 			per_cpu(vmxarea, cpu) = NULL; */
/* 		} */
/* 	} */
/* } */

/* int lcd_vmx_init(void) { */
/* 	int r, cpu; */

/* 	if (!cpu_has_vmx()) { */
/* 		printk(KERN_ERR "vmx: CPU does not support VT-x\n"); */
/* 		return -EIO; */
/* 	} */

/* 	if (setup_vmcs_config(&vmcs_config) < 0) */
/* 		return -EIO; */

/* 	if (!cpu_has_vmx_vpid()) { */
/* 		printk(KERN_ERR "vmx: CPU is missing required feature 'VPID'\n"); */
/* 		return -EIO; */
/* 	} */

/* 	if (!cpu_has_vmx_ept()) { */
/* 		printk(KERN_ERR "vmx: CPU is missing required feature 'EPT'\n"); */
/* 		return -EIO; */
/* 	} */

/* 	if (!vmx_capability.has_load_efer) { */
/* 		printk(KERN_ERR "vmx: ability to load EFER register is required\n"); */
/* 		return -EIO; */
/* 	} */

/* 	msr_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL); */
/* 	if (!msr_bitmap) { */
/* 		return -ENOMEM; */
/* 	} */

/* 	memset(msr_bitmap, 0xff, PAGE_SIZE); */
/* 	vmx_disable_intercept_for_msr(msr_bitmap, MSR_FS_BASE); */
/* 	vmx_disable_intercept_for_msr(msr_bitmap, MSR_GS_BASE); */

/* 	set_bit(0, vmx_vpid_bitmap); /\* 0 is reserved for host *\/ */

/* 	for_each_possible_cpu(cpu) { */
/* 		struct vmcs *vmxon_buf; */

/* 		vmxon_buf = vmx_alloc_vmcs(cpu); */
/* 		if (!vmxon_buf) { */
/* 			vmx_free_vmxon_areas(); */
/* 			return -ENOMEM; */
/* 		} */

/* 		per_cpu(vmxarea, cpu) = vmxon_buf; */
/* 	} */

/* 	atomic_set(&vmx_enable_failed, 0); */
/* 	if (on_each_cpu(vmx_enable, NULL, 1)) { */
/* 		printk(KERN_ERR "vmx: timeout waiting for VMX mode enable.\n"); */
/* 		r = -EIO; */
/* 		goto failed1; /\* sadly we can't totally recover *\/ */
/* 	} */

/* 	if (atomic_read(&vmx_enable_failed)) { */
/* 		r = -EBUSY; */
/* 		goto failed2; */
/* 	} */

/* 	return 0; */

/* failed2: */
/* 	on_each_cpu(vmx_disable, NULL, 1); */
/* failed1: */
/* 	vmx_free_vmxon_areas(); */
/* 	return r; */
/* } */
/* EXPORT_SYMBOL(lcd_vmx_init); */

/* void lcd_vmx_exit(void) */
/* { */
/* 	on_each_cpu(vmx_disable, NULL, 1); */
/* 	vmx_free_vmxon_areas(); */
/* 	free_page((unsigned long)msr_bitmap); */
/* } */
/* EXPORT_SYMBOL(lcd_vmx_exit); */


/* struct lcd* lcd_create(void) { */
/* 	struct lcd* lcd = vmx_create_vcpu(); */
/* 	return lcd; */
/* } */
/* EXPORT_SYMBOL(lcd_create); */

/* int lcd_destroy(struct lcd* lcd) { */
/* 	vmx_destroy_vcpu(lcd); */
/* 	return 0; */
/* } */
/* EXPORT_SYMBOL(lcd_destroy); */

static int lcd_va_to_pa(void* va, void** pa, int vmallocd) {
	if (vmallocd == 1) {
		*(unsigned long*)pa = vmalloc_to_pfn(va)<<PAGE_SHIFT;
	} else {
		*(unsigned long*)pa = virt_to_phys(va);
	}
	return 0;
}

static int __move_host_mapping(struct lcd *lcd, void* hva,
			unsigned int size, int vmallocd) {
	unsigned int mapped = 0;
	void *pa;
	void *va = (void*)round_down(((unsigned long)hva), PAGE_SIZE);
	int ret = 0;
  
	printk(KERN_ERR "mapping base - 0x%p (phys 0x%lx)(phys2 0x%lx) size %d\n", 
		hva, (vmalloc_to_pfn(va)<<PAGE_SHIFT), (unsigned long)virt_to_phys(va),size);
    
	while (mapped < size) {
		ret = lcd_va_to_pa(va, &pa, vmallocd);
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

static int map_host_page_at_guest_va(struct lcd *lcd, void* hva,
				void *gva, int vmallocd ) {
	void *pa;
	void *va = (void*)round_down(((unsigned long)hva), PAGE_SIZE);
	int ret = 0;
    
	ret = lcd_va_to_pa(va, &pa, vmallocd);
	if (ret != 0) {
		return ret;
	}
    
	ret = lcd_map_gpa_to_hpa(lcd, (u64)pa, (u64)pa, 0);
	if (ret != 0) {
		printk(KERN_ERR "lcd: move PA mapping conflicts canary\n");
		return ret;
	}
    
	ret = lcd_map_gva_to_gpa(lcd, (u64)gva, (u64)pa, 1, 0);
	if (ret != 0) {
		printk(KERN_ERR "lcd: move PT mapping conflicts canary\n");
		return ret;
	}
    
	return 0;

}

/* static int lcd_setup_stack(struct lcd *lcd) { */
/* 	char *sp = NULL; */
/* 	char *stack_top = NULL; */
/* 	int ret = 0; */
    
/* 	printk (KERN_ERR "lcd : Entered lcd_setup_stack\n"); */
/* 	// allocate a few pages for LCD stack from */
/* 	// Linux kernel memory allocator */
/* 	sp = (char*)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 2); */
/* 	if (!sp) { */
/* 		return -ENOMEM; */
/* 	} */
    
/* 	//We use low addr page of this block for passing message registers */
/* 	lcd->shared = sp; */
/* 	stack_top = (sp + (PAGE_SIZE * 4) - 1); */
/* 	printk (KERN_ERR "lcd : stack bootm %p , stack top %p and myshare %p", sp, stack_top, sp); */
    
/* 	// map the stack in the LCD address space */
/* 	ret = __move_host_mapping(lcd, (void *)sp , (PAGE_SIZE * 4), 0); */
/* 	if (ret != 0) { */
/* 		printk(KERN_ERR "lcd: Unable to map the dynamically allocate stack into LCD space\n"); */
/* 		return ret; */
/* 	} */
    
/* 	//setup the stack */
/* 	vmcs_writel(GUEST_RSP, (unsigned long) stack_top); */
    
/* 	//setup the stack canary page referenced by %gs:28 */
/* 	sp = (char*)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 0); */
/* 	if (!sp) { */
/* 		return -ENOMEM; */
/* 	} */
    
/* 	// Map a valid page into first guest virtual page */
/* 	// The aim is to not fault due to stack canary added */
/* 	// against buffer overflow  stack protection. */
/* 	// The gcc generated code expects %fs:0x28 to be */
/* 	// valid. Since we have zero base for all segment */
/* 	// registers - mapping the some page into virtual */
/* 	// address 0 should suffice. */
/* 	// refer to linux/arch/x86/include/asm/stackprotector.h for */
/* 	// details.[http://stackoverflow.com/a/22476070/2950979] */
/* 	map_host_page_at_guest_va(lcd, (void *)sp , 0, 0); */
/* 	if (ret != 0) { */
/* 		printk(KERN_ERR "lcd: Unable to map the canary\n"); */
/* 		return ret; */
/* 	} */

    
/* 	return 0; */
/* } */

int lcd_move_module(struct lcd *lcd, struct module *mod) {
	int ret;
	lcd->mod = mod;
	// lcd_va_to_pa(va, pa) // 4KB page assumption

	ret = __move_host_mapping(lcd, mod->module_init, mod->init_size, 1);
	if (!ret) {
		ret = __move_host_mapping(lcd, mod->module_core, mod->core_size, 1);
		if (!ret) {
			vmcs_writel(GUEST_RIP, (unsigned long)mod->init);
			lcd_setup_stack(lcd);
			return ret;
		}
	}

	printk(KERN_ERR "lcd: failed to move module %d\n", ret);

	return ret;
}
EXPORT_SYMBOL(lcd_move_module);

int lcd_map_gpa_to_hpa(struct lcd *lcd, u64 gpa, u64 hpa, int overwrite) {
	return ept_set_epte(lcd, gpa, hpa, overwrite);
}
EXPORT_SYMBOL(lcd_map_gpa_to_hpa);

int lcd_map_gva_to_gpa(struct lcd *lcd, u64 gva, u64 gpa,
		int create, int overwrite) {
	return map_gva_to_gpa(lcd, gva, gpa, create, overwrite);
}
EXPORT_SYMBOL(lcd_map_gva_to_gpa);

int lcd_find_hva_by_gpa(struct lcd *lcd, u64 gpa, u64 *hva) {
	return ept_gpa_to_hva(lcd, gpa, hva);
}
EXPORT_SYMBOL(lcd_find_hva_by_gpa);

/* static void vmx_handle_external_interrupt(struct lcd *lcd) {   */
/* 	if ((lcd->exit_intr_info & */
/* 			(INTR_INFO_VALID_MASK | INTR_INFO_INTR_TYPE_MASK)) */
/* 		== (INTR_INFO_VALID_MASK | INTR_TYPE_EXT_INTR)) { */
/* 		unsigned int vector; */
/* 		unsigned long entry; */
/* 		gate_desc *desc; */
/* 		unsigned long tmp; */
              
/* 		vector = lcd->vec_no; */
/* 		desc = (gate_desc *)lcd->host_idt_base + vector; */
/* 		entry = gate_offset(*desc); */
/* 		asm volatile( */
/* 			"mov %%" _ASM_SP ", %[sp]\n\t" */
/* 			"and $0xfffffffffffffff0, %%" _ASM_SP "\n\t" */
/* 			"push $%c[ss]\n\t" */
/* 			"push %[sp]\n\t" */
/* 			"pushf\n\t" */
/* 			"orl $0x200, (%%" _ASM_SP ")\n\t" */
/* 			__ASM_SIZE(push) " $%c[cs]\n\t" */
/* 			"call *%[entry]\n\t" */
/* 			: */
/* 			[sp]"=&r"(tmp) */
/* 			: */
/* 			[entry]"r"(entry), */
/* 			[ss]"i"(__KERNEL_DS), */
/* 			[cs]"i"(__KERNEL_CS) */
/* 			); */
/* 		/\* printk("lcd: handling external %u\n", vector); *\/ */
/* 		if (irqs_disabled()) */
/* 			local_irq_enable(); */
/* 	} else { */
/* 		local_irq_enable(); */
/* 		/\* printk("lcd: not handling external %u\n", lcd->exit_intr_info); *\/ */
/* 	} */
/* } */


/* static void vmx_handle_vmcall(struct lcd *lcd) { */
/* 	u32 ipc_dir, ipc_peer; */
    
/* 	//printk(KERN_ERR "%c", lcd->regs[VCPU_REGS_RAX]); */
/* 	printk(KERN_ERR "%016llx", lcd->regs[VCPU_REGS_RAX]); */
/* 	ipc_dir = LCD_IPC_DIR(lcd->regs[VCPU_REGS_RAX]); */
/* 	ipc_peer = LCD_IPC_PEER(lcd->regs[VCPU_REGS_RAX]); */

/* 	printk(KERN_ERR "vmx_handle_vmcall  - dir %d peer %d\n", ipc_dir, ipc_peer); */

/* 	switch(ipc_dir) { */
/* 	case IPC_SEND: */
/* 		ipc_send(lcd->sync_ipc.my_capid, ipc_peer); */
/* 		break; */
/* 	case IPC_RECV: */
/* 		ipc_recv(lcd->sync_ipc.my_capid, ipc_peer); */
/* 		break; */
/* 	} */
/* #if 0 */
/* 	//printk(KERN_ERR "lcd_run: got vmcall %llu and %c\n", lcd->regs[VCPU_REGS_RAX], lcd->regs[VCPU_REGS_RAX]); */
/* 	if (lcd->regs[VCPU_REGS_RAX] == 0xdeadbeef) { */
/* 		display_mr((utcb_t *)lcd->shared); */
/* 		return; */
/* 	} */
/* #endif     */
   
/* } */

/* static void vmx_handle_page_fault(struct lcd *lcd) { */
/* 	printk(KERN_INFO "lcd: page fault VA %p\n", */
/* 		(void*)lcd->exit_qualification); */
/* } */

/* static int vmx_handle_nmi_exception(struct lcd *vcpu) */
/* { */
/* 	printk(KERN_INFO "lcd: got an exception\n"); */
/* 	if ((vcpu->exit_intr_info & INTR_INFO_INTR_TYPE_MASK) */
/* 		== INTR_TYPE_NMI_INTR) */
/* 		return 0; */
/* 	else if (is_page_fault(vcpu->exit_intr_info)) { */
/* 		vmx_handle_page_fault(vcpu); */
/* 		return 1; */
/* 	} */

/* 	printk(KERN_ERR "lcd: unhandled nmi, intr_info %x\n", */
/* 		vcpu->exit_intr_info); */
/* 	vcpu->ret_code = ((EFAULT) << 8); */
/* 	return -EIO; */
/* } */

/* int lcd_run(struct lcd *lcd) { */
/* 	int done = 0; */
/* 	int ret = 0; */
/* 	int countdown = 1000; */

/* 	cycles_t begin, end; */
/* 	begin = get_cycles(); */
/* 	g_prepare_time = begin - g_prepare_start; */
/* 	while (1) { */
/* 		vmx_get_cpu(lcd); */

/* 		local_irq_disable(); */

/* 		/\*if (need_resched()) { */
/* 		  local_irq_enable(); */
/* 		  vmx_put_cpu(lcd); */
/* 		  cond_resched(); */
/* 		  continue; */
/* 		  }*\/ */

/* 		// In case we run from insmod. */
/* 		if (signal_pending(current)) { */
/* 			int signr; */
/* 			siginfo_t info; */
/* 			uint32_t x; */

/* 			local_irq_enable(); */
/* 			vmx_put_cpu(lcd); */

/* 			spin_lock_irq(&current->sighand->siglock); */
/* 			signr = dequeue_signal(current, &current->blocked, */
/* 					&info); */
/* 			spin_unlock_irq(&current->sighand->siglock); */
/* 			if (!signr) */
/* 				continue; */

/* 			if (signr == SIGKILL) { */
/* 				printk(KERN_INFO "vmx: got sigkill, dying"); */
/* 				lcd->ret_code = ((ENOSYS) << 8); */
/* 				break; */
/* 			} */

/* 			x  = signr; // + base? */
/* 			x |= INTR_INFO_VALID_MASK; */

/* 			vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, x); */
/* 			continue; */
/* 		} */

/* 		/\* local_irq_enable(); *\/ */

/* 		ret = vmx_run_vcpu(lcd); */
/* 		if (ret == EXIT_REASON_VMCALL || */
/* 			ret == EXIT_REASON_CPUID) { */
/* 			vmx_step_instruction(); */
/* 		} */

/* 		if (ret == EXIT_REASON_EXTERNAL_INTERRUPT) { */
/* 			vmx_handle_external_interrupt(lcd); */
/* 			/\* --countdown; *\/ */
/* 		} */
    
/* 		vmx_put_cpu(lcd); */

/* 		if (ret == EXIT_REASON_VMCALL) { */
/* 			vmx_handle_vmcall(lcd); */
/* 		} */
/* 		else if (ret == EXIT_REASON_EPT_VIOLATION) { */
/* 			done = vmx_handle_ept_violation(lcd); */
/* 		} */
/* 		else if (ret == EXIT_REASON_EXCEPTION_NMI) { */
/* 			if (vmx_handle_nmi_exception(lcd)) */
/* 				done = 1; */
/* 		} else if (ret != EXIT_REASON_EXTERNAL_INTERRUPT) { */
/* 			printk(KERN_INFO "unhandled exit: reason %d: %s, " */
/* 				"exit qualification %llx\n", */
/* 				ret, lcd_exit_reason(ret), lcd->exit_qualification); */
/* 			vmx_dump_cpu(lcd); */
/* 			done = 1; */
/* 		} */

/* 		--countdown; */

/* 		if (countdown <=0 || done || lcd->shutdown) { */
/* 			end = get_cycles(); */
/* 			printk("Cycles: %llu, counter %d\n", end-begin, countdown); */
/* 			printk(KERN_INFO "unhandled exit: reason %d: %s, " */
/* 				"exit qualification %llx, intr_info %x\n", */
/* 				ret, lcd_exit_reason(ret), lcd->exit_qualification, */
/* 				lcd->exit_intr_info); */
/* 			/\* vmx_dump_cpu(lcd); *\/ */
/* 			break; */
/* 		} */
/* 	} */

/* 	if (irqs_disabled()) */
/* 		local_irq_enable(); */

/* 	return 0; */
/* } */
/* EXPORT_SYMBOL(lcd_run); */


/* int setup_vmlinux(struct lcd *lcd, char *file) { */
/* 	int ret; */
/* 	u64 elf_entry; */

/* 	struct boot_params *bp = lcd->bp; */

/* 	ret = lcd_load_vmlinux(file, lcd, &elf_entry); */
/* 	if (!ret) { */
/* 		printk(KERN_ERR "Error when loading vmlinux %d\n", ret); */
/* 		goto err_out; */
/* 	} */

/* 	bp->e820_entries = 2; */
  
/* 	bp->e820_map[0].addr = 0; */
/* 	bp->e820_map[0].size = 0x9fc00; */
/* 	bp->e820_map[0].type = E820_RAM; */

/* 	bp->e820_map[0].addr = 0x9fc00; */
/* 	bp->e820_map[0].size = 0x400; */
/* 	bp->e820_map[0].type = E820_RESERVED; */

/* 	bp->e820_map[0].addr = 0xe8000; */
/* 	bp->e820_map[0].size = 0x18000; */
/* 	bp->e820_map[0].type = E820_RESERVED; */

/* 	bp->e820_map[0].addr = 0x100000; */
/* 	bp->e820_map[0].size = (LCD_PHY_MEM_SIZE - 0x100000); */
/* 	bp->e820_map[0].type = E820_RAM; */

/* 	bp->e820_map[1].addr = LCD_PHY_MEM_SIZE; */
/* 	bp->e820_map[1].size = LCD_PHY_MEM_LIMIT - LCD_PHY_MEM_SIZE; */
/* 	bp->e820_map[1].type = E820_RESERVED_KERN; */

/* 	// TODO: setup correct boot_params */


/* err_out: */
/* 	return ret; */
/* } */


