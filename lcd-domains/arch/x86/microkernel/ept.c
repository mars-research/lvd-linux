/*
 * ept.c
 *
 * Code for manipulating an LCD's EPT tables.
 *
 * Copyright: University of Utah
 */

#include <linux/mm.h>
#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <asm/lcd_domains/microkernel.h>

/* INVEPT / INVVPID --------------------------------------------------*/

static inline bool cpu_has_vmx_invvpid_single(void)
{
	return lcd_vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invvpid_global(void)
{
	return lcd_vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_context(void)
{
	return lcd_vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_global(void)
{
	return lcd_vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT;
}

static inline bool cpu_has_vmx_ept_ad_bits(void)
{
	return lcd_vmx_capability.ept & VMX_EPT_AD_BIT;
}

static inline void __invept(int ext, u64 eptp)
{
	u8 error;

	struct {
		u64 eptp, gpa;
	} operand = {eptp, 0};

	asm volatile (ASM_VMX_INVEPT "; setna %0"
                /* CF==1 or ZF==1 --> rc = -1 */
                : "=qm"(error) : "a" (&operand), "c" (ext) : "cc", "memory");
	if (error)
		LCD_ERR("ext=%d, eptp=0x%llx\n", ext, eptp);
			
}

/**
 * Invalidates all mappings associated with eptp's.
 */
static inline void invept_global_context(void)
{
	if (cpu_has_vmx_invept_global())
		__invept(VMX_EPT_EXTENT_GLOBAL, 0);
}

/**
 * Invalidates all mappings associated with eptp, and possibly
 * others.
 */
static inline void invept_single_context(u64 eptp)
{
	if (cpu_has_vmx_invept_context())
		__invept(VMX_EPT_EXTENT_CONTEXT, eptp);
	else
		invept_global_context();
}

static inline void __invvpid(int ext, u16 vpid)
{
	u8 error;

	struct {
		u64 vpid : 16;
		u64 rsvd : 48;
		u64 addr;
	} operand = { vpid, 0, 0 };

	asm volatile (ASM_VMX_INVVPID "; setna %0"
                /* CF==1 or ZF==1 --> rc = -1 */
		: "=qm"(error) : "a"(&operand), "c"(ext) : "cc", "memory");
	if (error)
		LCD_ERR("ext=%d, vpid=0x%hx\n", ext, vpid);
}

/**
 * Invalidates all mappings associated with vpid's other than
 * vpid = 0 (the host).
 */
static inline void invvpid_global_context(void)
{
	if (cpu_has_vmx_invvpid_global())
		__invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0);
}

/**
 * Invalidates all mappings associated with vpid.
 */
static inline void invvpid_single_context(u16 vpid)
{
	/*
	 * Don't invalidate host mappings
	 */
	if (vpid == 0)
		return;

	if (cpu_has_vmx_invvpid_single())
		__invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vpid);
	else
		invvpid_global_context();		
}

void lcd_arch_ept_global_invalidate(void)
{
	invvpid_global_context();
	invept_global_context();
}

void lcd_arch_ept_invvpid(u16 vpid)
{
	invvpid_single_context(vpid);
}

void lcd_arch_ept_invept(u64 eptp)
{
	invept_single_context(eptp);
}

/* EPT  -------------------------------------------------- */

/**
 * PAGE_SHIFT is assumed to be 12.
 */
#define VMX_EPTE_ADDR_MASK PAGE_MASK
#define VMX_EPT_ALL_MASK (VMX_EPT_READABLE_MASK | \
                          VMX_EPT_WRITABLE_MASK | \
			  VMX_EPT_EXECUTABLE_MASK)
static inline hpa_t vmx_epte_hpa(lcd_arch_epte_t epte)
{
	return __hpa(((u64)epte) & VMX_EPTE_ADDR_MASK);
}
static inline hva_t vmx_epte_hva(lcd_arch_epte_t epte)
{
	return hpa2hva(vmx_epte_hpa(epte));
}
static inline lcd_arch_epte_t * vmx_epte_dir_hva(lcd_arch_epte_t epte)
{
	return (lcd_arch_epte_t *)hva_val(vmx_epte_hva(epte));
}
static inline int vmx_epte_present(lcd_arch_epte_t epte)
{
	return (int)(((u64)epte) & VMX_EPT_ALL_MASK);
}
/*
 * level 0 (PML4) = bits 47:39 (9 bits)
 * level 1 (PDPT) = bits 38:30 (9 bits)
 * level 2 (PD)   = bits 29:21 (9 bits)
 * level 3 (PT)   = bits 20:12 (9 bits)
 */
static inline int vmx_ept_idx(gpa_t a, int lvl)
{
	/* we right shift by the correct amount, then mask off 9 bits */
	return (int)(((gpa_val(a)) >> (12 + 9 * (3 - lvl))) & ((1 << 9) - 1));
}
static inline u64 vmx_ept_offset(gpa_t a)
{
	return gpa_val(a) & ~(PAGE_MASK);
}

enum vmx_epte_mts {
	VMX_EPTE_MT_UC = 0, /* uncachable */
	VMX_EPTE_MT_WC = 1, /* write combining */
	VMX_EPTE_MT_WT = 4, /* write through */
	VMX_EPTE_MT_WP = 5, /* write protected */
	VMX_EPTE_MT_WB = 6, /* write back */
};

/**
 * Sets address in epte along with default access settings. Since
 * we are using a page walk length of 4, epte's at all levels have
 * the `size' bit (bit 7) set to 0. Page table entries (entries at the final
 * level) have the IPAT (ignore page attribute table) and EPT MT (memory
 * type) bits set. Paging levels are zero-indexed:
 *
 *  0 = PML4 entry
 *  1 = PDPTE entry
 *  2 = Page Directory entry
 *  3 = Page Table entry
 *
 *  See Intel SDM V3 Figure 28-1 and 28.2.2.
 */
static void vmx_epte_set(lcd_arch_epte_t *epte, hpa_t a, int level)
{
	/*
	 * zero out epte, and set
	 */
	*epte = 0;
	*epte = (hpa_val(a) & VMX_EPTE_ADDR_MASK) | VMX_EPT_ALL_MASK;
	if (level == 3) {
		
		/* 
 		 * To support ioremap, effective memory type used for 
 		 * guest-physical address will be a combination of 
 		 * EPT and PAT. Since MTRRs have no effect on memory
 		 * type used for an access to a guest-physical address,
 		 * we use the EPT memory type and allow PAT memory type
 		 * to be set explictly. See table 11-7 in section 11.5.2.2
 		 * & section 28.2.5.2 of the Intel Software Developer 
 		 * Manual Vol 3 for effective memory type.
 		 */
		*epte |= VMX_EPTE_MT_WB << VMX_EPT_MT_EPTE_SHIFT;
		*epte &= ~VMX_EPT_IPAT_BIT;
	}
}

int lcd_arch_ept_walk(struct lcd_arch *lcd, gpa_t a, int create,
		lcd_arch_epte_t **epte_out)
{
	int i;
	lcd_arch_epte_t *dir;
	u64 idx;
	hva_t page;

	dir = lcd->ept.root;

	/*
	 * Walk plm4 -> pdpt -> pd. Each step uses 9 bits
	 * of the gpa.
	 */
	for (i = 0; i < LCD_ARCH_EPT_WALK_LENGTH - 1; i++) {

		idx = vmx_ept_idx(a, i);

		if (!vmx_epte_present(dir[idx])) {
			
			if (!create)
				return -ENOENT;
			/*
			 * Get host virtual addr of fresh page, and
			 * set the epte's addr to the host physical addr
			 */
			page = __hva(__get_free_page(GFP_KERNEL));
			if (!hva_val(page)) {
				LCD_ERR("alloc failed\n");
				return -ENOMEM;
			}
			memset(hva2va(page), 0, PAGE_SIZE);
			vmx_epte_set(&dir[idx], hva2hpa(page), i);
		}

		dir = (lcd_arch_epte_t *) hva2va(vmx_epte_hva(dir[idx]));
	}

	/*
	 * dir points to page table (level 3)
	 */
	*epte_out = &dir[vmx_ept_idx(a, 3)];
	return 0;
}

void lcd_arch_ept_set(lcd_arch_epte_t *epte, hpa_t a)
{
	vmx_epte_set(epte, a, 3);
}

int lcd_arch_ept_unset(lcd_arch_epte_t *epte)
{
	*epte = 0;
	return 0;
}

hpa_t lcd_arch_ept_hpa(lcd_arch_epte_t *epte)
{
	return vmx_epte_hpa(*epte);
}

int lcd_arch_ept_map(struct lcd_arch *lcd, gpa_t ga, hpa_t ha,
				int create, int overwrite)
{
	int ret;
	lcd_arch_epte_t *ept_entry;

	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(lcd, ga, create, &ept_entry);
	if (ret)
		return ret;

	/*
	 * Check if guest physical address already mapped
	 */
	if (!overwrite && vmx_epte_present(*ept_entry)) {
		LCD_ERR("would overwrite hpa %lx with hpa %lx\n",
			hpa_val(lcd_arch_ept_hpa(ept_entry)), 
			hpa_val(ha));
		return -EINVAL;
	}

	/*
	 * Map the guest physical addr to the host physical addr.
	 */
	lcd_arch_ept_set(ept_entry, ha);

	return 0;
}

int lcd_arch_ept_map_range(struct lcd_arch *lcd, gpa_t ga_start, 
			hpa_t ha_start, unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_arch_ept_map(lcd,
					/* gpa */
					gpa_add(ga_start, off),
					/* hpa */
					hpa_add(ha_start, off),
					/* create */
					1,
					/* no overwrite */
					0)) {
			LCD_ERR("error mapping gpa %lx to hpa %lx\n",
				gpa_val(gpa_add(ga_start, off)),
				hpa_val(hpa_add(ha_start, off)));
			return -EIO;
		}
	}

	return 0;
}

int lcd_arch_ept_unmap(struct lcd_arch *lcd, gpa_t a)
{
	int ret;
	lcd_arch_epte_t *ept_entry;
	
	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(lcd, a, 0, &ept_entry);
	if (ret)
		return ret;

	/*
	 * Unset
	 */
	lcd_arch_ept_unset(ept_entry);

	return 0;
}

int lcd_arch_ept_unmap2(struct lcd_arch *lcd, gpa_t a, hpa_t *hpa_out)
{
	int ret;
	lcd_arch_epte_t *ept_entry;

	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(lcd, a, 0, &ept_entry);
	if (ret)
		return ret;
	/*
	 * Extract hpa
	 */
	if (hpa_out)
		*hpa_out = lcd_arch_ept_hpa(ept_entry);
	/*
	 * Unset
	 */
	lcd_arch_ept_unset(ept_entry);

	return 0;
}

int lcd_arch_ept_unmap_range(struct lcd_arch *lcd, gpa_t ga_start, 
			unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_arch_ept_unmap(lcd, gpa_add(ga_start, off))) {
			LCD_ERR("error unmapping gpa %lx\n",
				gpa_val(gpa_add(ga_start, off)));
			return -EIO;
		}
	}

	return 0;
}

int lcd_arch_ept_gpa_to_hpa(struct lcd_arch *lcd, gpa_t ga, hpa_t *ha_out)
{
	int ret;
	lcd_arch_epte_t *ept_entry;
	hpa_t hpa;

	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(lcd, ga, 0, &ept_entry);
	if (ret)
		return ret;

	/*
	 * Confirm the entry is present
	 */
	if (!vmx_epte_present(*ept_entry)) {
		LCD_ERR("gpa %lx is not mapped\n",
			gpa_val(ga));
		return -EINVAL;
	}	

	/*
	 * Get the base host physical address, and add the offset.
	 */
	hpa = lcd_arch_ept_hpa(ept_entry);
	hpa = hpa_add(hpa, vmx_ept_offset(ga));
	*ha_out = hpa;

	return 0;
}

/**
 * Recursively frees all present entries in dir at level, and
 * the page containing the dir. The recursion depth is limited to 3 - 4 stack
 * frames, so it's reasonable to use.
 *
 * 0 = pml4
 * 1 = pdpt
 * 2 = page dir
 * 3 = page table
 */
static void vmx_free_ept_dir_level(lcd_arch_epte_t *dir, int level)
{
	int idx;
	
	/*
	 * Base case of recursion is when level = 3.
	 *
	 * In that case - don't do anything - don't try to free any 
	 * pages that are still mapped. The higher level layers 
	 * should've done that already (but may not have bothered 
	 * unmapping). If we try to free pages that are still mapped,
	 * we may get bad double free's.
	 */
	if (level != 3) {
		/*
		 * pml4, pdpt, or page directory
		 *
		 * Recur on present entries
		 */
		for (idx = 0; idx < LCD_ARCH_PTRS_PER_EPTE; idx++) {
			if (vmx_epte_present(dir[idx]))
				vmx_free_ept_dir_level(
					vmx_epte_dir_hva(dir[idx]),
					level + 1);
		}
	}
	/*
	 * Free page containing dir
	 */
	free_page((unsigned long)dir);
}

void lcd_arch_ept_free(struct lcd_arch *lcd)
{
	lcd_arch_epte_t *dir;
	/*
	 * Get pml4 table
	 */
	dir = lcd->ept.root;
	vmx_free_ept_dir_level(dir, 0);
}

int lcd_arch_ept_init(struct lcd_arch *lcd_arch)
{
	hva_t page;
	u64 eptp;
	/*
	 * Alloc the root global page directory page
	 */
	page = __hva(get_zeroed_page(GFP_KERNEL));
	if (!hva_val(page)) {
		LCD_ERR("failed to alloc page\n");
		return -ENOMEM;
	}
	lcd_arch->ept.root = (lcd_arch_epte_t *)hva2va(page);

	/*
	 * Init the VMCS EPT pointer
	 *
	 * -- default memory type (write-back)
	 * -- default ept page walk length (4, pointer stores
	 *    length - 1)
	 * -- use access/dirty bits, if available
	 *
	 * See Intel SDM V3 24.6.11 and Figure 28-1.
	 */

	eptp = VMX_EPT_DEFAULT_MT |
		(LCD_ARCH_EPT_WALK_LENGTH - 1) << LCD_ARCH_EPTP_WALK_SHIFT;
	if (cpu_has_vmx_ept_ad_bits()) {
		lcd_arch->ept.access_dirty_enabled = true;
		eptp |= VMX_EPT_AD_ENABLE_BIT;
	}
	eptp |= hpa_val(va2hpa(lcd_arch->ept.root)) & PAGE_MASK;
	lcd_arch->ept.vmcs_ptr = eptp;
	/*
	 * Init the mutex
	 */
	mutex_init(&lcd_arch->ept.lock);

	return 0;
}

static void debug_ept_lvl(lcd_arch_epte_t *dir, int lvl, int idx)
{
	int i;
	struct page *p;
	lcd_arch_epte_t *entry;
	/*
	 * Print ept dir info
	 */
	printk(KERN_INFO "ept dir lvl %d idx %d\n-----------------------\n",
		lvl, idx);
	/*
	 * Print level entries
	 *
	 * XXX: assumes 512 entires per dir
	 */
	for (i = 0; i < 512; i++) {
		entry = &dir[i];
		if (vmx_epte_present(*entry)) {
			p = virt_to_page(hva2va(vmx_epte_hva(*entry)));
			printk(KERN_INFO "%03d: page = %p\n", i, p);
		}
	}
	/*
	 * Print next level, if lvl < 3
	 */
	if (lvl >= 3)
		return;

	for (i = 0; i < 512; i++) {
		entry = &dir[i];
		if (vmx_epte_present(*entry))
			debug_ept_lvl(hva2va(vmx_epte_hva(*entry)), 
				lvl + 1, i);
	}

	return;
}

void lcd_arch_ept_dump(struct lcd_arch *lcd)
{
	debug_ept_lvl(lcd->ept.root, 0, 0);
}
