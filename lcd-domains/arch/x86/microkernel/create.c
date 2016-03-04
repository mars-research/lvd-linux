/*
 * create.c
 *
 * Code for creating an lcd_arch (the arch-specific part
 * of an LCD)
 *
 * Copyright: University of Utah
 */

#include <asm/lcd_domains/create.h>

/* These are initialized by init.c */
struct kmem_cache *lcd_arch_cache;
struct lcd_vmcs_config lcd_global_vmcs_config;
unsigned long *lcd_global_msr_bitmap;
struct lcd_vpids lcd_vpids;

static void lcd_arch_free_vmcs(struct lcd_arch_vmcs *vmcs)
{
	free_pages((unsigned long)vmcs, lcd_global_vmcs_config.order);
}

struct lcd_arch_vmcs *lcd_arch_alloc_vmcs(int cpu)
{
	int node;
	struct page *pages;
	struct lcd_arch_vmcs *vmcs;

	node = cpu_to_node(cpu);
	pages = alloc_pages_exact_node(node, GFP_KERNEL, 
				lcd_global_vmcs_config.order);
	if (!pages)
		return NULL;
	vmcs = page_address(pages);
	memset(vmcs, 0, lcd_global_vmcs_config.size);

	vmcs->revision_id = lcd_global_vmcs_config.revision_id;

	return vmcs;
}

/* HOST INFO -------------------------------------------------- */

/**
 * Returns pointer to current gdt (array of segment descriptors) on 
 * calling cpu.
 */
static struct desc_struct * vmx_host_gdt(void)
{
	struct desc_ptr gdt_ptr;
	/*
	 * I had trouble using get_cpu_gdt_table: Unknown symbol gdt_page
	 * when inserting module. Couldn't figure out why it couldn't find
	 * the gdt_page symbol (gdt_page was listed in /proc/kallsyms).
	 *
	 * Also, it looks like CONFIG_PARAVIRT is always set by default,
	 * due to KVM being default? This means store_gdt in desc.h is
	 * not visible, so need to use native_store_gdt.
	 */
	native_store_gdt(&gdt_ptr);
	return (struct desc_struct *)gdt_ptr.address;
}

/**
 * Returns pointer to current idt (array of gate descriptors) on 
 * calling cpu.
 */
static gate_desc * vmx_host_idt(void)
{
	struct desc_ptr idt_ptr;
	native_store_idt(&idt_ptr);
	return (gate_desc *)idt_ptr.address;
}

static int vmx_host_seg_base(u16 selector, hva_t *hva_out)
{
	u16 ldt_sel;
	hva_t ldt_hva;
	hva_t base;
	struct desc_struct *table;
	struct desc_struct *desc;
	struct ldttss_desc64 *ldttss_desc;

	if (!(selector & ~SEGMENT_RPL_MASK)) {
		/*
		 * null selector
		 */
		return -EIO;
	}
	/*
	 * get desc table (gdt or ldt)
	 */
	if ((selector & SEGMENT_TI_MASK) == SEGMENT_LDT) {
		store_ldt(ldt_sel);
		if (vmx_host_seg_base(ldt_sel, &ldt_hva)) {
			LCD_ERR("host seg sel points to ldt, which is not present\n");
			return -EIO;
		}
		table = hva2va(ldt_hva);		
	} else {
		table = vmx_host_gdt();
	}
	/*
	 * get seg descriptor
	 */
	desc = &table[selector >> 3];
	if (desc->s == 0) {
		/*
		 * system desc (tss, call gate, ...)
		 */
		if (desc->type == 2 || desc->type == 9 || desc->type == 11) {
			/*
			 * ldt or tss desc
			 */
			base = __hva(get_desc_base(desc));
			ldttss_desc = (struct ldttss_desc64 *)desc;
			base = __hva(hva_val(base) | 
				(((unsigned long)ldttss_desc->base3) << 32));
			*hva_out = base;
			return 0;
		} else {
			/*
			 * call gate, or interrupt / trap gate; no base
			 */
			return -EIO;
		}
	}
				
	/*
	 * otherwise, non-system segment desc
	 */
	*hva_out = __hva(get_desc_base(desc));
	return 0;
}

/**
 * Returns hva of current tss.
 */
static int vmx_host_tss(hva_t *hva_out)
{
	u16 tr;
	int ret;
	store_tr(tr);
	ret = vmx_host_seg_base(tr, hva_out);
	if (ret)
		LCD_ERR("error looking up host tss\n");
	return ret;
}	

/* VMCS INITIALIZATION -------------------------------------------------- */

/**
 * Stores *most* of the expected host state in VMCS.
 *
 * Host state is initialized / set in three primary places:
 *
 * vmx_setup_vmcs_host
 *   - the majority of the host state
 *   - %cr0, %cr4, and %cr3
 *   - sysenter %cs and %eip
 *   - segment registers -- %cs, %ss, %ds, %es, %fs, %gs
 *   - idt base (assuming it is not per cpu!)
 *
 * __vmx_setup_cpu
 *   - this is executed after a vmcs is loaded on a cpu
 *   - for per-cpu info (tr base, gdt base, sysenter %esp, 
 *     %fs and %gs bases)
 *
 * vmx_enter
 *   - host %rsp: not known until we're about to enter
 */
static void vmx_setup_vmcs_host(struct lcd_arch *lcd_arch)
{
	unsigned long tmpl;
	gate_desc *idt;
	u32 low32;
	u32 high32;
	u16 tmps;

	/*
	 * Host %cr0, %cr4, %cr3
	 *
	 * -- ensure TS (Task Switched) in %cr0 is 0
	 *
	 * Intel SDM V3 2.5
	 */
	vmcs_writel(HOST_CR0, read_cr0() & ~X86_CR0_TS);
	vmcs_writel(HOST_CR4, read_cr4());
	vmcs_writel(HOST_CR3, read_cr3());

	/*
	 * Host segment selectors
	 *
	 * Even though %es, %ds, and %ss are ignored in 64-bit
	 * mode, we still set them. See x86/include/asm/segment.h and
	 * Intel SDM V3 3.4.4.
	 *
	 * Most segment register info is set by the processor
	 * on vm exit, so no need to set it here. See Intel
	 * SDM V3 27.5.2.
	 */
	savesegment(cs, tmps);
	vmcs_write16(HOST_CS_SELECTOR, tmps);
	savesegment(ds, tmps);
	vmcs_write16(HOST_DS_SELECTOR, tmps);
	savesegment(es, tmps);
	vmcs_write16(HOST_ES_SELECTOR, tmps);
	savesegment(ss, tmps);
	vmcs_write16(HOST_SS_SELECTOR, tmps);
	savesegment(fs, tmps);
	vmcs_write16(HOST_FS_SELECTOR, tmps);
	savesegment(gs, tmps);
	vmcs_write16(HOST_GS_SELECTOR, tmps);
	store_tr(tmps);
	vmcs_write16(HOST_TR_SELECTOR, tmps);

	/*
	 * Host tr, %fs, and %gs bases are per cpu, and done
	 * in __vmx_setup_cpu.
	 */

	/*
	 * Host MSR EFER
	 *
	 * (will be loaded on vm exit)
	 */
	rdmsrl(MSR_EFER, tmpl);
	vmcs_writel(HOST_IA32_EFER, tmpl);

	/*
	 * Host %rip
	 *
	 * Points to the return label after vmexit.
	 * Note! Host %rsp set just before vmentry inside
	 * vmx_entry.
	 *
	 * Also note: vmx_return will not get a value until
	 * this module is linked and loaded.
	 */
	vmcs_writel(HOST_RIP, vmx_return);

	/*
	 * Host idtr
	 */
	idt = vmx_host_idt();
	vmcs_writel(HOST_IDTR_BASE, (unsigned long)idt);

	/*
	 * Host sysenter info (%cs, %eip, and %esp)
	 *
	 * Even though the guest cannot access the sysenter msr,
	 * the processor loads the values in these fields on exit,
	 * so we need to have the correct values there.
	 *
	 * %esp is set when the vmcs is loaded on a cpu (since
	 * each cpu has its own sysenter stack? following dune and
	 * kvm here ...). This happens in __vmx_setup_cpu.
	 *
	 * See Intel SDM V3 27.5.1
	 */
	rdmsr(MSR_IA32_SYSENTER_CS, low32, high32);
	vmcs_write32(HOST_IA32_SYSENTER_CS, low32);
	rdmsrl(MSR_IA32_SYSENTER_EIP, tmpl);
	vmcs_writel(HOST_IA32_SYSENTER_EIP, tmpl);
}

/**
 * Sets up MSR autloading for MSRs listed in autload_msrs (local var).
 */
static void vmx_setup_vmcs_msr(struct lcd_arch *lcd_arch)
{
	int i;
	u64 val;
	struct vmx_msr_entry *e;
	int autoload_msrs[] = {
		MSR_LSTAR
	};
	
	/*
	 * MSR Bitmap (bit = 0 ==> access allowed in non-root mode)
	 *
	 * Intel SDM V3 24.6.9, 31.10.1
	 */
	vmcs_write64(MSR_BITMAP, __pa(msr_bitmap));
	
	/*
	 * MSR Load / Store areas
	 *
	 * ENTRY MSR Load Area : MSR values guest expects
	 * EXIT MSR Store Area : MSR values guest had on exit
	 * EXIT MSR Load Area  : MSR values host expects
	 *
	 * Entry MSR Load = Exit MSR Store means guest will see
	 * the values they wrote to the MSR when we enter.
	 *
	 * Intel SDM V3 24.7.2, 24.8.2, 31.10.2, 31.10.3
	 */
	vmcs_write32(VM_EXIT_MSR_STORE_COUNT, LCD_ARCH_NUM_AUTOLOAD_MSRS);
	vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, LCD_ARCH_NUM_AUTOLOAD_MSRS);
	vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, LCD_ARCH_NUM_AUTOLOAD_MSRS);

	vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(lcd_arch->msr_autoload.host));
	vmcs_write64(VM_EXIT_MSR_STORE_ADDR, __pa(
			lcd_arch->msr_autoload.guest));
	vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(
			lcd_arch->msr_autoload.guest));

	for (i = 0; i < LCD_ARCH_NUM_AUTOLOAD_MSRS; i++) {
	
		e = &lcd_arch->msr_autoload.host[i];
		e->index = autoload_msrs[i];
		rdmsrl(e->index, val);
		e->value = val;

		e = &lcd_arch->msr_autoload.guest[i];
		e->index = autoload_msrs[i];
	}
}

/**
 * Sets up initial guest register values in VMCS.
 *
 * Most of the guest state is set here and in 
 * vmx_setup_vmcs_guest_settings. The processor
 * is picky about what goes into the guest state; if
 * it doesn't like it, vmentry will fail. See Intel
 * SDM V3 26.3.1.
 *
 * vmx_setup_vmcs_guest_regs
 *   - %cr0, %cr4
 *   - EFER MSR (part of setting guest to use 64-bit mode)
 *   - %rsp (for now! should be accessible by arch-indep
 *     code through interface)
 *   - %rflags
 *   - segment registers -- %cs, %ds, %ss, %es, %fs, %gs
 *     - we have to do more setup here since the processor
 *       doesn't set defaults (access rights, limits, etc.)
 *   - misc fields -- activity state, debug controls, etc.
 *
 * vmx_setup_vmcs_guest_settings
 *   - vpid
 *   - execution, exit, and entry controls
 *   - ept pointer (so you must init ept before!)
 *   - exception handling
 *   - %cr0, %cr4 access masks
 *
 * lcd_arch_set_pc
 *   - %rip (to be set by arch-indep code)

 * lcd_arch_set_gva_root
 *   - %cr3 (to be set by arch-indep code)
 */
static void vmx_setup_vmcs_guest_regs(struct lcd_arch *lcd_arch)
{
	unsigned long cr0;
	unsigned long cr4;

	/*
	 * %cr0 (and its shadow)
	 *
	 * -- (PG) paging (required for 64-bit)
	 * -- (PE) protected mode (required for 64-bit)
	 * -- (WP) supervisor-mode write protect
	 * -- (MP) monitor coprocessor (fpu related)
	 * -- (ET) extension type (fpu related)
	 * -- (NE) numeric error reporting (fpu related)
	 *
	 * Intel SDM V3 2.5
	 */
	cr0 = X86_CR0_PG | 
		X86_CR0_PE | 
		X86_CR0_WP |
		X86_CR0_MP |
		X86_CR0_ET |
		X86_CR0_NE;
	vmcs_writel(GUEST_CR0, cr0);
	vmcs_writel(CR0_READ_SHADOW, cr0);

	/*
	 * %cr4 (and its shadow)
	 *
	 * -- (PAE)        physical address extension
	 * -- (PGE)        page global enable
	 * -- (VMXE)       vmx enabled
	 * -- (OSXMMEXCPT) os support for unmasked fpu exceptions
	 * -- (OSFXSR)     os support for fast fpu save
	 * -- (PCIDE)      process context identifiers, if available
	 * -- (RDWRGSFS)   allow RDFSBASE, WRFSBASE, etc., if available
	 *
	 * Nothing for floating point, since it is turned off.
	 *
	 * Intel SDM V3 2.5
	 */
	cr4 = X86_CR4_PAE | 
		X86_CR4_PGE |
		X86_CR4_VMXE |
		X86_CR4_OSXMMEXCPT |
		X86_CR4_OSFXSR;
	if (boot_cpu_has(X86_FEATURE_PCID))
		cr4 |= X86_CR4_PCIDE;
	if (boot_cpu_has(X86_FEATURE_FSGSBASE))
		cr4 |= X86_CR4_RDWRGSFS;
	vmcs_writel(GUEST_CR4, cr4);
	vmcs_writel(CR4_READ_SHADOW, cr4);

	/*
	 * %cr3 -- to be set when guest address space set up
	 */

	/*
	 * MSR EFER (extended feature enable register)
	 *
	 * -- 64-bit mode (long mode enabled and active)
	 */
	vmcs_writel(GUEST_IA32_EFER, EFER_LME | EFER_LMA);

	/*
 	 * IA32 MSR - setup PAT entry	 
 	 */
	vmx_setup_pat_msr(0, PAT_WB);
	vmx_setup_pat_msr(1, PAT_UC);

	/*
	 * Sysenter info
	 *
	 * All set to 0.
	 */
	vmcs_write32(GUEST_SYSENTER_CS, 0);
	vmcs_writel(GUEST_SYSENTER_ESP, 0);
	vmcs_writel(GUEST_SYSENTER_EIP, 0);

	/*
	 * %rsp, %rip -- to be set by arch-independent code when guest address 
	 * space set up (see lcd_arch_set_sp and lcd_arch_set_pc).
	 */

	/*
	 * %rflags
	 *
	 * 2nd reserved bit should be 1 (Intel SDM V3 2.3)
	 */
	vmcs_writel(GUEST_RFLAGS, 0x02);

	/*
	 *===--- Segment and descriptor table registers ---===
	 *
	 * See Intel SDM V3 26.3.1.2, 26.3.1.3 for register requirements
	 */

	/* 
	 * Bases for segment and desc table registers.
	 *
	 * Note: MSR's for %fs and %gs will be loaded with
	 * the values in %fs.base and %gs.base; see Intel SDM V3 26.3.2.1.
	 */
	vmcs_writel(GUEST_CS_BASE, 0);
	vmcs_writel(GUEST_DS_BASE, 0);
	vmcs_writel(GUEST_ES_BASE, 0);
	vmcs_writel(GUEST_SS_BASE, 0);
	vmcs_writel(GUEST_FS_BASE, gpa_val(LCD_ARCH_FS_BASE));
	vmcs_writel(GUEST_GS_BASE, gpa_val(LCD_ARCH_GS_BASE));
 	vmcs_writel(GUEST_GDTR_BASE, gpa_val(LCD_ARCH_GDTR_BASE));
 	vmcs_writel(GUEST_IDTR_BASE, gpa_val(LCD_ARCH_IDTR_BASE));
 	vmcs_writel(GUEST_TR_BASE, gpa_val(LCD_ARCH_TSS_BASE));

	/*
	 * Access rights
	 *
	 * -- %cs:
	 *    -- code segment type, execute/read/accessed
	 *    -- code or data segment desc type
	 *    -- dpl = 0
	 *    -- present
	 *    -- 64-bit mode
	 *    -- d/b default operation size (32 bit code)
	 *    -- granularity = 1
	 * -- %ds, %es, %fs, %gs, %ss
	 *    -- data segment type, read/write/accessed
	 *    -- code or data segment desc type
	 *    -- dpl = 0
	 *    -- present
	 *    -- granularity = 1
	 * -- ldtr
	 *    -- unusable (bit 16 = 1)
	 * -- tr
	 *    -- 64-bit busy tss
	 *    -- present
	 *    -- granularity = 0
	 *
	 * Intel SDM V3 24.4.1, 3.4.5, 26.3.1.2
	 */
	vmcs_writel(GUEST_CS_AR_BYTES,   0xA09B);
	vmcs_writel(GUEST_DS_AR_BYTES,   0x8093);
	vmcs_writel(GUEST_ES_AR_BYTES,   0x8093);
	vmcs_writel(GUEST_FS_AR_BYTES,   0x8093);
	vmcs_writel(GUEST_GS_AR_BYTES,   0x8093);
	vmcs_writel(GUEST_SS_AR_BYTES,   0x8093);
 	vmcs_writel(GUEST_LDTR_AR_BYTES, (1 << 16));
 	vmcs_writel(GUEST_TR_AR_BYTES,   0x008B);

	/*
	 * Limits 
	 *
	 * -- Limits are always in bytes
	 * -- Granularity (see above) seems to have different
	 *    semantics? See Intel SDM V3 26.3.1.2.
	 * -- For gdtr and idtr, upper 16 bits are set to 0 on vm enter.
	 */
	vmcs_write32(GUEST_CS_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_DS_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_ES_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_FS_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_GS_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_SS_LIMIT, 0xFFFFFFFF);
	vmcs_write32(GUEST_TR_LIMIT,   LCD_ARCH_TSS_LIMIT);
	vmcs_write32(GUEST_GDTR_LIMIT, LCD_ARCH_GDTR_LIMIT);
	vmcs_write32(GUEST_IDTR_LIMIT, LCD_ARCH_IDTR_LIMIT);

	/* 
	 * Guest segment selectors
	 *
	 * Since we aren't using a gdt, these are all set to 0.
	 * This is ok since we set all of the hidden fields in the
	 * segment registers.
	 *
	 * Intel SDM V3 3.4.4
	 */
	vmcs_write16(GUEST_CS_SELECTOR, 0);
	vmcs_write16(GUEST_DS_SELECTOR, 0);
	vmcs_write16(GUEST_ES_SELECTOR, 0);
	vmcs_write16(GUEST_FS_SELECTOR, 0);
	vmcs_write16(GUEST_GS_SELECTOR, 0);
	vmcs_write16(GUEST_SS_SELECTOR, 0);
	vmcs_write16(GUEST_TR_SELECTOR, 0);

	/*
	 * Guest activity state = active
	 *
	 * Intel SDM V3 24.4.2
	 */
	vmcs_write32(GUEST_ACTIVITY_STATE, GUEST_ACTIVITY_ACTIVE);

	/*
	 * Guest interruptibility state = 0 (interruptible)
	 *
	 * Intel SDM V3 24.4.2
	 */
	vmcs_write32(GUEST_INTERRUPTIBILITY_INFO, 0);

	/*
	 * Clear the interrupt event injection field (valid bit is 0)
	 *
	 * Intel SDM V3 24.8.3
	 */
	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, 0);

	/*
	 * No pending debug exceptions
	 *
	 * Intel SDM V3 24.4.2
	 */
	vmcs_write32(GUEST_PENDING_DBG_EXCEPTIONS, 0);

	/*
	 * This might not be needed in 64-bit mode
	 *
	 * Intel SDM V3 26.3.1.5
	 */
	vmcs_write64(GUEST_IA32_DEBUGCTL, 0);
}

/**
 * Sets up VMCS settings--execution control, control register
 * access, exception handling.
 *
 * We need the lcd_arch so we can set up t's ept.
 */
static void vmx_setup_vmcs_guest_settings(struct lcd_arch *lcd_arch)
{
	/*
	 * VPID
	 */
	vmcs_write16(VIRTUAL_PROCESSOR_ID, lcd_arch->vpid);
	/*
	 * No VMCS Shadow (Intel SDM V3 24.4.2)
	 */
	vmcs_write64(VMCS_LINK_POINTER, -1ull);
	/*
	 * Execution controls
	 */
	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL,
		vmcs_config.pin_based_exec_controls);
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL,
		vmcs_config.primary_proc_based_exec_controls);
	vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
		vmcs_config.secondary_proc_based_exec_controls);
	/*
	 * Entry / Exit controls
	 */
	vmcs_write32(VM_ENTRY_CONTROLS, vmcs_config.vmentry_controls);
	vmcs_write32(VM_EXIT_CONTROLS, vmcs_config.vmexit_controls);
	/*
	 * EPT
	 *
	 * XXX: We aren't locking here, probably don't need to since we're
	 * setting up the lcd.
	 */
	vmcs_write64(EPT_POINTER, lcd_arch->ept.vmcs_ptr);
	/*
	 * Exception handling (vm exit on any exception)
	 *
	 * Exit on any kind of page fault (Intel SDM V3 25.2)
	 */
	vmcs_write32(EXCEPTION_BITMAP, 0xffffffff);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK, 0);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH, 0);
	/*
	 * No %cr3 targets (Intel SDM V3 24.6.7)
	 *
	 * (Loads / stores to %cr3 are allowed in non-root anyway.)
	 */
	vmcs_write32(CR3_TARGET_COUNT, 0);
	/*
	 * %cr0 and %cr4 guest accesses always cause vm exit
	 *
	 * Intel SDM V3 24.6.7
	 */
	vmcs_writel(CR0_GUEST_HOST_MASK, ~0ul);
	vmcs_writel(CR4_GUEST_HOST_MASK, ~0ul);
}

/**
 * Front-end for setting up VMCS. Calls helper routines
 * to set up guest and host states of VMCS.
 */
static void vmx_setup_vmcs(struct lcd_arch *lcd_arch)
{
	/*
	 * Set up guest part of vmcs, and guest exec
	 */
	vmx_setup_vmcs_guest_settings(lcd_arch);
	vmx_setup_vmcs_guest_regs(lcd_arch);
	/*
	 * Set up MSR bitmap and autoloading
	 */
	vmx_setup_vmcs_msr(lcd_arch);
	/*
	 * Set up host part of vmcs
	 */
	vmx_setup_vmcs_host(lcd_arch);
}


/* VMCS LOADING -------------------------------------------------- */

/**
 * Updates an lcd_arch's VMCS when it is moved to a different
 * cpu. (Linux uses per-cpu data that needs to be updated in
 * the lcd's VMCS.)
 */
static void __vmx_setup_cpu(struct lcd_arch *lcd_arch, int cur_cpu)
{
	struct desc_struct *gdt;
	hva_t host_tss;
	unsigned long tmpl;

	/*
	 * Linux uses per-cpu TSS and GDT, so we need to set these
	 * in the host part of the vmcs when switching cpu's.
	 */
	gdt = vmx_host_gdt();
	vmx_host_tss(&host_tss);
	vmcs_writel(HOST_TR_BASE, hva_val(host_tss));
	vmcs_writel(HOST_GDTR_BASE, (unsigned long)gdt);

	/*
	 * Sysenter %esp (per cpu? so has to go in here? following
	 * dune and kvm...)
	 */
	rdmsrl(MSR_IA32_SYSENTER_ESP, tmpl);
	vmcs_writel(HOST_IA32_SYSENTER_ESP, tmpl);

	/*
	 * %fs and %gs are also per-cpu
	 *
	 * (MSRs are used to load / store %fs and %gs in 64-bit mode.
	 * See Intel SDM V3 3.2.4 and 3.4.4.)
	 */
	rdmsrl(MSR_FS_BASE, tmpl);
	vmcs_writel(HOST_FS_BASE, tmpl);
	rdmsrl(MSR_GS_BASE, tmpl);
	vmcs_writel(HOST_GS_BASE, tmpl);
}

/**
 * Clears vcpu (active -> inactive) on a cpu.
 */
static void __vmx_get_cpu_helper(void *ptr)
{
	struct lcd_arch *lcd_arch;
	lcd_arch = ptr;
	BUG_ON(raw_smp_processor_id() != lcd_arch->cpu);
	vmcs_clear(lcd_arch->vmcs);
	if (__get_cpu_var(local_lcd_arch) == lcd_arch)
		__get_cpu_var(local_lcd_arch) = NULL;
}

/**
 * Loads t on the calling cpu.
 *
 * Disables preemption. Call vmx_put_cpu() when finished.
 */
static void vmx_get_cpu(struct lcd_arch *lcd_arch)
{
	int cur_cpu;

	/*
	 * Preemption disabled
	 */
	cur_cpu = get_cpu();

	/*
	 * See Intel SDM V3 24.1 and Figure 31.1
	 *
	 * If vcpu is the cpu's local vcpu, vcpu is
	 * active and current on the current cpu, and
	 * there's nothing to be done.
	 */

	/*
	 * Otherwise, we need to make t active
	 * and current on this cpu.
	 */
	if (__get_cpu_var(local_lcd_arch) != lcd_arch) {

		__get_cpu_var(local_lcd_arch) = lcd_arch;

		if (lcd_arch->cpu != cur_cpu) {

			/*
			 * lcd_arch not active on this cpu
			 */
			if (lcd_arch->cpu >= 0)
				/*
				 * lcd_arch active on a different cpu;
				 * clear it there (active -> inactive)
				 */
				smp_call_function_single(lcd_arch->cpu,
							__vmx_get_cpu_helper, 
							(void *) lcd_arch, 1);
			else
				/*
				 * lcd_arch inactive; clear it to get to
				 * initial vmcs state
				 *
				 * Intel SDM V3 24.11.3
				 */
				vmcs_clear(lcd_arch->vmcs);

			/*
			 * Invalidate any vpid or ept cache lines
			 */
			invvpid_single_context(lcd_arch->vpid);
			invept_single_context(lcd_arch->ept.vmcs_ptr);

			/*
			 * lcd_arch is not in launched state
			 */
			lcd_arch->launched = 0;

			/*
			 * Load vmcs pointer on this cpu
			 */
			vmcs_load(lcd_arch->vmcs);

			/*
			 * Update cpu-specific data in vmcs
			 */
			__vmx_setup_cpu(lcd_arch, cur_cpu);

			/*
			 * Remember which cpu we are active on
			 */
			lcd_arch->cpu = cur_cpu;
		} else {
			vmcs_load(lcd_arch->vmcs);
		}
	}
}

/**
 * Match with vmx_get_cpu.
 *
 * Enables preemption.
 */
static void vmx_put_cpu(struct lcd_arch *lcd_arch)
{
	put_cpu();
}

/* LCD_ARCH CREATE / DESTROY ---------------------------------------- */

/**
 * Reserves a vpid and sets it in the vcpu.
 */
static int vmx_allocate_vpid(struct lcd_arch *lcd_arch)
{
	int vpid;

	lcd_arch->vpid = 0;
	
	spin_lock(&vpids.lock);
	vpid = find_first_zero_bit(vpids.bitmap, VMX_NR_VPIDS);
	if (vpid < VMX_NR_VPIDS) {
		lcd_arch->vpid = vpid;
		__set_bit(vpid, vpids.bitmap);
	}
	spin_unlock(&vpids.lock);

	return vpid >= VMX_NR_VPIDS;
}

/**
 * Frees a vpid.
 */
static void vmx_free_vpid(struct lcd_arch *lcd_arch)
{
	spin_lock(&vpids.lock);
	if (lcd_arch->vpid != 0)
		__clear_bit(lcd_arch->vpid, vpids.bitmap);
	spin_unlock(&vpids.lock);
}

int lcd_arch_create(struct lcd_arch **out)
{
	struct lcd_arch *lcd_arch;
	int ret;
	/*
	 * Alloc lcd_arch
	 */
	lcd_arch = kmem_cache_zalloc(lcd_arch_cache, GFP_KERNEL);
	if (!lcd_arch) {
		LCD_ERR("failed to alloc lcd_arch");
		ret = -ENOMEM;
		goto fail_alloc;
	}
	/*
	 * Set up ept
	 */
	ret = vmx_init_ept(lcd_arch);
	if (ret) {
		LCD_ERR("setting up etp");
		goto fail_ept;
	}
	/*
	 * Alloc vmcs
	 */
	lcd_arch->vmcs = vmx_alloc_vmcs(raw_smp_processor_id());
	if (!lcd_arch->vmcs) {
		LCD_ERR("failed to alloc vmcs\n");
		ret = -ENOMEM;
		goto fail_vmcs;
	}

	ret = vmx_allocate_vpid(lcd_arch);
	if (ret) {
		LCD_ERR("failed to alloc vpid\n");
		goto fail_vpid;
	}

	/*
	 * Not loaded on a cpu right now
	 */
	lcd_arch->cpu = -1;
	/*
	 * Initialize VMCS register values and settings
	 * 
	 * Preemption disabled while doing so ...
	 */
	vmx_get_cpu(lcd_arch);
	vmx_setup_vmcs(lcd_arch);
	vmx_put_cpu(lcd_arch);

	*out = lcd_arch;
	
	return 0;

fail_vpid:
	vmx_free_vmcs(lcd_arch->vmcs);
fail_vmcs:
	vmx_free_ept(lcd_arch);
fail_ept:
	kmem_cache_free(lcd_arch_cache, lcd_arch);
fail_alloc:
	return ret;
}

void lcd_arch_destroy(struct lcd_arch *lcd_arch)
{
	/*
	 * Premption Disabled
	 *
	 * The call to vmx_get_cpu is done because if lcd_arch is
	 * active on a different cpu, it needs to be
	 * vmclear'd there (and vmx_get_cpu will do
	 * that, as a side effect).
	 *
	 * There might be alternative ways, but this works ...
	 */
	vmx_get_cpu(lcd_arch);
	/*
	 * Invalidate any cached mappings in tlb.
	 *
	 * XXX: No lock on ept
	 */
	invvpid_single_context(lcd_arch->vpid);
	invept_single_context(lcd_arch->ept.vmcs_ptr);
	/*
	 * VM clear on this cpu
	 */
	vmcs_clear(lcd_arch->vmcs);
	__get_cpu_var(local_lcd_arch) = NULL;
	/*
	 * Preemption enabled
	 */
	vmx_put_cpu(lcd_arch);
	/*
	 * Free remaining junk
	 */
	vmx_free_vpid(lcd_arch);
	vmx_free_vmcs(lcd_arch->vmcs);
	vmx_free_ept(lcd_arch);
	kmem_cache_free(lcd_arch_cache, lcd_arch);
}

#if 0
/**
 * Pack base, limit, and flags into a segment descriptor.
 *
 * This is not used since we removed the gdt init code. But we're keeping
 * it in case we have gdt's in lcd's in the future. See
 * Documentation/lcd-domains/vmx.txt.
 *
 * See Intel SDM V3 3.4.5
 */
static void vmx_pack_desc(struct desc_struct *desc, u64 base, u64 limit,
			unsigned char type, unsigned char s,
			unsigned char dpl, unsigned char p,
			unsigned char avl, unsigned char l,
			unsigned char d, unsigned char g)
{
	set_desc_base(desc, base);
	set_desc_limit(desc, limit);
	desc->type = type;
	desc->s    = s;
	desc->dpl  = dpl;
	desc->p    = p;
	desc->avl  = avl;
	desc->l    = l;
	desc->d    = d;
	desc->g    = g;
}
#endif

int lcd_arch_set_pc(struct lcd_arch *lcd_arch, gva_t a)
{
	lcd_arch->regs.rip = gva_val(a);
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(lcd_arch);
	vmcs_writel(GUEST_RIP, gva_val(a));
	vmx_put_cpu(lcd_arch);
	return 0;
}

int lcd_arch_set_sp(struct lcd_arch *lcd_arch, gva_t a)
{
	lcd_arch->regs.rsp = gva_val(a);
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(lcd_arch);
	vmcs_writel(GUEST_RSP, gva_val(a));
	vmx_put_cpu(lcd_arch);
	return 0;
}

int lcd_arch_set_gva_root(struct lcd_arch *lcd_arch, gpa_t a)
{
	u64 cr3_ptr;

	cr3_ptr = gpa_val(a); /* no page write through, etc. ... */
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(lcd_arch);
	vmcs_writel(GUEST_CR3, cr3_ptr);
	vmx_put_cpu(lcd_arch);
	return 0;
}
