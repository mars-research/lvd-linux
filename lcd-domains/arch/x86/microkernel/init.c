/*
 * init.c
 *
 * Code for initializing the VT-x part of the microkernel (turning
 * on VT-x, etc.)
 *
 * Copyright: University of Utah
 */

#include <linux/tboot.h>
#include <asm/vmx.h>
#include <asm/virtext.h>
#include <asm/desc.h>

#include <lcd_domains/types.h>
#include <asm/lcd_domains/microkernel.h>
#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/vmfunc.h>
#include <asm/lcd_domains/bflank.h> 

#if defined(CONFIG_LVD)
DEFINE_PER_CPU(struct page *, vmfunc_eptp_list_page);
DEFINE_PER_CPU(struct page *, vmfunc_state_hpa_page);
struct lcd_vmx_capability lcd_vmx_capability;
extern union vmfunc_state_page vmfunc_state_page;
#else
static atomic_t vmx_enable_failed;
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(struct lcd_arch_vmcs *, vmxon_area);
#endif

#ifndef CONFIG_LVD
/* DEBUGGING --------------------------------------------------*/

/**
 * Prints the vmx controls, lower and upper bounds on the controls,
 * and tries to find the bits that were rejected.
 *
 * Useful for debugging set up of the vmcs.
 */
static void print_vmx_controls(u32 controls, u32 mask, u32 msr)
{
	u32 msr_low;
	u32 msr_high;
	u32 bad_high;
	u32 bad_low;
	int i;

	/*
	 * See doc in adjust_vmx_controls
	 */

	rdmsr(msr, msr_low, msr_high);

	LCD_MSG("  MSR LOW:             0x%08x\n", msr_low);
	LCD_MSG("  ATTEMPTED CONTROLS:  0x%08x\n", controls);
	LCD_MSG("  MSR HIGH:            0x%08x\n", msr_high);
	LCD_MSG("  RESERVED BIT MASK:   0x%08x\n", mask);

	/*
	 * For each bit, if the reserved mask is not set *and* the msr high
	 * bit is not set, then the control bit should not be set.
	 */
	bad_high = ~msr_high & ~mask & controls;
	for (i = 0; i < 32; i++) {
		if (bad_high & 1)
			LCD_MSG("  Control bit %d should be 0.\n", i);
		bad_high >>= 1;
	}

	/*
	 * For each bit, if the reserved mask is not set *and* the msr low
	 * bit is set, then the control bit should be set.
	 */
	bad_low = msr_low & ~mask & ~controls;
	for (i = 0; i < 32; i++) {
		if (bad_low & 1)
			LCD_MSG("  Control bit %d should be 1.\n", i);
		bad_low >>= 1;
	}

	LCD_MSG("See Intel SDM V3 24.{6,7,8,9} and Appendix A\n");
}

/* VMCS CONFIGURATION ---------------------------------------- */

/**
 * Clears the correct bit in the msr bitmap to allow vm access
 * to an msr.
 */
static void vmx_disable_intercept_for_msr(unsigned long *msr_bitmap, u32 msr)
{
	int sz;
	sz = sizeof(unsigned long);

	/*
	 * Intel SDM V3 24.6.9 (MSR-Bitmap Addresses).
	 *
	 * The bitmap is 4KBs:
	 *
	 *  -- bitmap + 0KB (0x000) = read bitmap for low MSRs
	 *  -- bitmap + 1KB (0x400) = read bitmap for high MSRs
	 *  -- bitmap + 2KB (0x800) = write bitmap for low MSRs
	 *  -- bitmap + 3KB (0xc00) = write bitmap for high MSRs
	 *
	 * We have to divide by the size of an unsigned long to get
	 * the correct pointer offset.
	 */
	if (msr <= 0x1fff) {
		/*
		 * Low MSR
		 */
		__clear_bit(msr, msr_bitmap + 0x000 / sz); /* read  */
		__clear_bit(msr, msr_bitmap + 0x800 / sz); /* write */
	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
		/*
		 * High MSR
		 */
		msr &= 0x1fff;
		__clear_bit(msr, msr_bitmap + 0x400 / sz); /* read  */
		__clear_bit(msr, msr_bitmap + 0xc00 / sz); /* write */
	}
}
	
/**
 * Checks and sets basic vmcs settings (vmxon region size, etc.)
 */
static int vmcs_config_basic_settings(struct lcd_vmcs_config *vmcs_conf)
{
	u32 msr_low;
	u32 msr_high;

	/*
	 * Read and store basic vmcs settings.
	 *
	 * Intel SDM V3 Appendix A
	 */

	rdmsr(MSR_IA32_VMX_BASIC, msr_low, msr_high);

	/*
	 * VMCS size is never greater than 4KBs
	 */
	if ((msr_high & 0x1fff) > PAGE_SIZE)
		return -EIO;

	/* 
	 * 64-bit CPUs always have VMX_BASIC_MSR[48] == 0. Controls
	 * physical address width.
	 */
	if (msr_high & (1u<<16))
		return -EIO;

	/*
	 * Require Write-Back (WB) memory type for VMCS accesses.
	 */
	if (((msr_high >> 18) & 15) != 6)
		return -EIO;

	vmcs_conf->size  = msr_high & 0x1fff;
	vmcs_conf->order = get_order(lcd_global_vmcs_config.size);
	vmcs_conf->revision_id = msr_low;
	return 0;
}

/**
 * Returns 0 if controls not allowed, non-zero otherwise. If
 * successful, controls is updated with reserved bits properly
 * set. The negation of the reserved mask is used to ignore
 * reserved bits during the `checking' process.
 */
static int adjust_vmx_controls(u32 *controls, u32 reserved_mask, u32 msr)
{
	u32 msr_low;
	u32 msr_high;
	u32 controls_copy;
	
	/*
	 * Make sure the desired controls are possible. In the pin-based
	 * exec, primary and secondary exec, vmentry, and vmexit MSRs:
	 * 
	 * -- The low word contains the *minimum required* bits that must
	 *    be set to 1 (i.e., if the bit in the low msr is 1, the vmx
	 *    control bit must be 1).
	 *
	 * -- The high word contains the *maximum allowed* bits that can
	 *    be set to 1 (i.e., if the bit in the high msr is 0, the vmx
	 *    control must be 0).
	 *
	 * If these conditions aren't met, vmentry fails. Some of these
	 * bits are reserved, so a mask is used to ensure we're only
	 * checking those bits we care about.
	 *
	 * See Intel SDM V3 Appendix A.
	 */

	rdmsr(msr, msr_low, msr_high);

	controls_copy = *controls;

	/*
	 * (msr high bit not set, and not a reserved bit) ==> ctrl bit not set
	 */
	if (~msr_high & ~reserved_mask & controls_copy)
		return -1;

	/*
	 * (msr low bit set, and not a reserved bit) ==> ctrl bit set
	 */
	if (msr_low & ~reserved_mask & ~controls_copy)
		return -1;

	controls_copy &= msr_high;
	controls_copy |= msr_low;

	*controls = controls_copy;
	return 0;
}

/**
 * Populates default settings in vmcs_conf for
 * vm entries, vm exits, vm execution (e.g., interrupt handling),
 * etc. for all lcd types.
 */
static int setup_vmcs_config(struct lcd_vmcs_config *vmcs_conf)
{
	u32 pin_based_exec_controls;
	u32 primary_proc_based_exec_controls;
	u32 secondary_proc_based_exec_controls;
	u32 vmexit_controls;
	u32 vmentry_controls;

	/*
	 * Basic VMX Configuration
	 */
	if (vmcs_config_basic_settings(vmcs_conf) < 0)
		return -EIO;

	/*
	 * VMX Execution Controls (Intel SDM V3 24.6)
	 */
	
	/*
	 * Pin Based Execution Controls (exceptions, nmi's, ...)
	 * 
	 * -- external interrupts and nmi's cause vm exit.
	 */
	pin_based_exec_controls = PIN_BASED_EXT_INTR_MASK | 
		PIN_BASED_NMI_EXITING;
	if (adjust_vmx_controls(&pin_based_exec_controls,
					PIN_BASED_RESERVED_MASK,
					MSR_IA32_VMX_PINBASED_CTLS) < 0) {
		LCD_ERR("pin based exec controls not allowed\n");
		print_vmx_controls(pin_based_exec_controls,
				PIN_BASED_RESERVED_MASK,
				MSR_IA32_VMX_PINBASED_CTLS);
		return -EIO;
	}

	/*
	 * Primary Processor Execution Controls
	 *
	 * -- HLT Exit
	 * -- Invalidate PG Exit
	 * -- MWAIT Exit
	 * -- RDPMC Exit
	 * -- L/S CR8 Exit
	 * -- L/S CR3 Exit   / required by emulab machines :(
	 * -- MOV DR Exit
	 * -- Unconditional I/O Exit (no I/O bitmap)
	 * -- Use MSR Bitmaps
	 * -- MONITOR Exit
	 * -- Activate Secondary Proc Exec Controls
	 *
	 * Note: TSC offsetting and TPR Shadowing are not set. We are
	 * currently not virtualizing access to the TPR.
	 */
	primary_proc_based_exec_controls = CPU_BASED_HLT_EXITING |
		CPU_BASED_INVLPG_EXITING |
		CPU_BASED_MWAIT_EXITING |
		CPU_BASED_RDPMC_EXITING |
		CPU_BASED_CR8_LOAD_EXITING |
		CPU_BASED_CR8_STORE_EXITING |
		CPU_BASED_CR3_LOAD_EXITING |
		CPU_BASED_CR3_STORE_EXITING |
		CPU_BASED_MOV_DR_EXITING |
		CPU_BASED_UNCOND_IO_EXITING |
		CPU_BASED_USE_MSR_BITMAPS |
		CPU_BASED_MONITOR_EXITING |
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
	if (adjust_vmx_controls(&primary_proc_based_exec_controls,
					CPU_BASED_RESERVED_MASK,
					MSR_IA32_VMX_PROCBASED_CTLS)) {
		LCD_ERR("primary proc based exec ctrls not allowed\n");
		print_vmx_controls(primary_proc_based_exec_controls,
				CPU_BASED_RESERVED_MASK,
				MSR_IA32_VMX_PROCBASED_CTLS);
		return -EIO;
	}

	/*
	 * Secondary Processor Execution Controls
	 *
	 * -- Enable EPT
	 * -- Enable RDTSCP
	 * -- Enable VPID
	 * -- Enable VMFUNC
	 * -- WBINVD Exit
	 *
	 * Note: Unrestricted guest and INVPCID not available on
	 * emulab machines.
	 */
	secondary_proc_based_exec_controls = SECONDARY_EXEC_ENABLE_EPT |
		SECONDARY_EXEC_RDTSCP |
		SECONDARY_EXEC_ENABLE_VPID |
		SECONDARY_EXEC_WBINVD_EXITING |
		SECONDARY_EXEC_ENABLE_VMFUNCTIONS;

	if (adjust_vmx_controls(&secondary_proc_based_exec_controls,
					SECONDARY_EXEC_RESERVED_MASK,
					MSR_IA32_VMX_PROCBASED_CTLS2) < 0) {
		LCD_ERR("secondary proc based exec ctls not allowed\n");
		print_vmx_controls(secondary_proc_based_exec_controls,
				SECONDARY_EXEC_RESERVED_MASK,
				MSR_IA32_VMX_PROCBASED_CTLS2);
		return -EIO;
	}

	/*
	 * Remember the EPT and VPID capabilities
	 */
	rdmsr(MSR_IA32_VMX_EPT_VPID_CAP,
		lcd_vmx_capability.ept, lcd_vmx_capability.vpid);


	/*
	 * VM Exit Controls (Intel SDM V3 24.7)
	 *
	 * -- Host Address Space (host in 64-bit mode on vm exit)
	 * -- Save / load IA-32 EFER MSR on exit
	 * -- Save debug controls    / needed for emulab machines
	 */
	vmexit_controls = VM_EXIT_HOST_ADDR_SPACE_SIZE |
		VM_EXIT_SAVE_IA32_EFER |
		VM_EXIT_LOAD_IA32_EFER |
		VM_EXIT_SAVE_DEBUG_CONTROLS;
	if (adjust_vmx_controls(&vmexit_controls, 
					VM_EXIT_RESERVED_MASK,
					MSR_IA32_VMX_EXIT_CTLS) < 0) {
		LCD_ERR("vmexit controls not allowed\n");
		
		print_vmx_controls(vmexit_controls,
				VM_EXIT_RESERVED_MASK,
				MSR_IA32_VMX_EXIT_CTLS);
		return -EIO;
	}

	/*
	 * VM Entry Controls (Intel SDM V3 24.8)
	 *
	 * -- IA-32E Mode inside guest
	 * -- Load IA-32 EFER MSR on entry
	 * -- Load debug controls  / needed on emulab
	 * -- Load IA32 PAT MSR
	 */
	vmentry_controls = VM_ENTRY_IA32E_MODE |
		VM_ENTRY_LOAD_IA32_EFER |
		VM_ENTRY_LOAD_IA32_PAT |
		VM_ENTRY_LOAD_DEBUG_CONTROLS;
	if (adjust_vmx_controls(&vmentry_controls,
					VM_ENTRY_RESERVED_MASK,
					MSR_IA32_VMX_ENTRY_CTLS) < 0) {
		LCD_ERR("vm entry controls not allowed\n");
		
		print_vmx_controls(vmentry_controls,
				VM_ENTRY_RESERVED_MASK,
				MSR_IA32_VMX_ENTRY_CTLS);
		
		return -EIO;
	}


	vmcs_conf->pin_based_exec_controls = pin_based_exec_controls;
	vmcs_conf->primary_proc_based_exec_controls =
		primary_proc_based_exec_controls;
	vmcs_conf->secondary_proc_based_exec_controls = 
		secondary_proc_based_exec_controls;
	vmcs_conf->vmexit_controls = vmexit_controls;
	vmcs_conf->vmentry_controls = vmentry_controls;

	return 0;
}

/* VMXON/VMXOFF -------------------------------------------------- */

static inline void __vmxon(hpa_t addr)
{
	u64 paddr;
	
	paddr = hpa_val(addr);
	asm volatile (ASM_VMX_VMXON_RAX
                : : "a"(&paddr), "m"(paddr)
                : "memory", "cc");
}

static inline void __vmxoff(void)
{
	asm volatile (ASM_VMX_VMXOFF : : : "cc");
}

/**
 * Helper for vmx_enable. A few more low-level checks and
 * settings, and then turns on vmx.
 */
static int __vmx_enable(struct lcd_arch_vmcs *vmxon_buf)
{
	hpa_t a;
	u64 old;
	u64 test_bits;

	a = va2hpa(vmxon_buf);

	/*
	 * Intel SDM V3 23.7
	 */

	/*
	 * We can't use vmx if someone else is
	 */
	if (cr4_read_shadow() & X86_CR4_VMXE)
		return -EBUSY;

	/*
	 * Set MSR_IA32_FEATURE_CONTROL
	 */

	rdmsrl(MSR_IA32_FEATURE_CONTROL, old);
	test_bits = FEATURE_CONTROL_LOCKED;
	test_bits |= FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX;
	if (tboot_enabled())
		test_bits |= FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX;

	if ((old & test_bits) != test_bits) {
		/* enable and lock */
		wrmsrl(MSR_IA32_FEATURE_CONTROL, old | test_bits);
	}

	/*
	 * set VMXE bit using this as opposed to directly writing cr4
	 */
	cr4_set_bits(X86_CR4_VMXE);

	/*
	 * Turn on vmx
	 */
	__vmxon(a);


	return 0;
}

/**
 * Turn on vmx on calling cpu, using per cpu vmxon_area.
 *
 * unused is ignored (NULL is passed in lcd_arch_init).
 *
 * Important: Assumes preemption is disabled (it will be
 * if called via on_each_cpu).
 */
static void vmx_enable(void *unused)
{
	int ret;
	struct lcd_arch_vmcs *vmxon_buf;

	vmxon_buf = __this_cpu_read(vmxon_area);
	
	/*
	 * Turn on vmx
	 */
	ret = __vmx_enable(vmxon_buf);
	if (ret)
		goto failed;

	/*
	 * Flush TLB and caches of any old VPID and EPT
	 * mappings.
	 */
	lcd_arch_ept_global_invalidate();

	this_cpu_write(vmx_enabled, 1);

	printk(KERN_INFO "VMX enabled on CPU %d\n",	
		raw_smp_processor_id());
	return;

failed:
	atomic_inc(&vmx_enable_failed);
	LCD_ERR("failed to enable VMX, err = %d\n", ret);
	return;
}

/**
 * Turns off vmx on calling cpu.
 *
 * unused is ignored (NULL is passed in lcd_arch_init).
 *
 * Important: Assumes preemption is disabled. (It will
 * be if called from on_each_cpu.)
 */
static void vmx_disable(void *unused)
{
	if (__this_cpu_read(vmx_enabled)) {
		__vmxoff();
		cr4_clear_bits(X86_CR4_VMXE);
		if (cr4_read_shadow() & X86_CR4_VMXE)
			LCD_ERR("VMX disabling failed on cpu %d\n",
				raw_smp_processor_id());
		else
			LCD_MSG("VMX disabling Successful on cpu %d\n",
				raw_smp_processor_id());

		this_cpu_write(vmx_enabled, 0);
	}
}

/**
 * Frees any vmxon areas allocated for cpu's.
 */
static void vmx_free_vmxon_areas(void)
{
	int cpu;
	for_each_possible_cpu(cpu) {
		if (per_cpu(vmxon_area, cpu)) {
			lcd_arch_free_vmcs(per_cpu(vmxon_area, cpu));
			per_cpu(vmxon_area, cpu) = NULL;
		}
	}
}
#endif

#ifdef CONFIG_LVD
static int vmx_alloc_vmfunc_ept_switching_page(void)
{
	int cpu;

	LCD_MSG("Initializing VMFUNC EPT list page\n"); 

	for_each_possible_cpu(cpu) {
		struct page *eptp_list_page;  	
		eptp_list_page = alloc_page(GFP_KERNEL | __GFP_ZERO);
		
		per_cpu(vmfunc_eptp_list_page, cpu) = eptp_list_page;  
		if (!eptp_list_page)
			return -ENOMEM;

	}

	/* Install LCDs EPT on each CPU */
	on_each_cpu(on_cpu_install_vmfunc_ept_page, NULL, 1);

	bfcall_dump_stack();

	return 0;
}

static int vmx_map_vmfunc_state_page(void)
{
	int cpu;
	struct gpa_hpa_pair gpa_hpa_pair; 

	LCD_MSG("Mapping VMFUNC state page on all CPUs\n"); 

	gpa_hpa_pair.gpa = virt_to_page(&vmfunc_state_page); 

	for_each_possible_cpu(cpu) {
		struct page *page;
		union vmfunc_state_page *this_cpu_vmfunc_page; 
		int ret; 	

		page = alloc_page(GFP_KERNEL | __GFP_ZERO);
		
		per_cpu(vmfunc_state_hpa_page, cpu) = page;  
		if (!page)
			return -ENOMEM;

		gpa_hpa_pair.hpa = page;

		//memcpy(this_cpu_vmfunc_page, &vmfunc_state_page, sizeof(struct vmfunc_state_page)); 

		/* We have to set up general params, e.g, cpuid and in_kernel
		 * before mapping this page, however we need to setup gs and 
		 * only then setup in_kernel = 0 on a specific CPU.
		 * 
		 * We do this in the on_cpu_setup_vmfunc_page() function
		 *
		 * */

		this_cpu_vmfunc_page = page_to_virt(page); 
		this_cpu_vmfunc_page->vmfunc_state.cpuid = cpu;
		this_cpu_vmfunc_page->vmfunc_state.in_kernel = 1; 

		this_cpu_vmfunc_page->vmfunc_state.debug_stack = per_cpu(lvd_debug_stack_addr, cpu);  

		ret = smp_call_function_single(cpu, 
				on_cpu_ept_map_page, 
				&gpa_hpa_pair, 1);
		if(ret) 
			return -EIO; 

		ret = smp_call_function_single(cpu, 
				on_cpu_setup_vmfunc_page, 
				this_cpu_vmfunc_page, 1);
		if(ret) 
			return -EIO; 

		LCD_MSG("vmfunc state page: gs_base:0x%lx, cpu:%d, debug_stack:%lx\n", 
			this_cpu_vmfunc_page->vmfunc_state.kernel_gs_base, 
			this_cpu_vmfunc_page->vmfunc_state.cpuid,
			this_cpu_vmfunc_page->vmfunc_state.debug_stack); 

	}

	return 0;
}

int lcd_arch_vmfunc_init(void)
{
	int ret = 0;

	vmx_alloc_vmfunc_ept_switching_page();

	vmx_map_vmfunc_state_page(); 

	/*
	 * Init lcd_arch_thread cache (using instead of kmalloc since
	 * these structs need to be aligned properly)
	 */
	lcd_arch_cache = KMEM_CACHE(lcd_arch, 0);
	if (!lcd_arch_cache) {
		LCD_ERR("failed to set up kmem cache\n");
		ret = -ENOMEM;
	}

	return ret;
}

void lcd_arch_vmfunc_exit(void)
{
	kmem_cache_destroy(lcd_arch_cache);
}

#else

int lcd_arch_init(void)
{
	int ret;
	int cpu;

	/*
	 * Check For VMX Features
	 */

	if (!cpu_has_vmx()) {
		LCD_ERR("CPU does not support VMX\n");
		return -EIO;
	}

	if (setup_vmcs_config(&lcd_global_vmcs_config) < 0)
		return -EIO;

	/*
	 * Set up default MSR bitmap
	 */

	lcd_global_msr_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!lcd_global_msr_bitmap) {
		ret = -ENOMEM;
		goto failed1;
	}	

	/* Allow access to %fs and %gs */
	memset(lcd_global_msr_bitmap, 0xff, PAGE_SIZE);
	vmx_disable_intercept_for_msr(lcd_global_msr_bitmap, MSR_FS_BASE);
	vmx_disable_intercept_for_msr(lcd_global_msr_bitmap, MSR_GS_BASE);

	/*
	 * Initialize VPID bitmap spinlock
	 */
	spin_lock_init(&lcd_vpids.lock);

	/*
	 * VPID 0 is reserved for host. See INVVPID instruction.
	 */
	set_bit(0, lcd_vpids.bitmap); 

	/*
	 * Allocate vmxon buffers for each cpu. A vmxon buffer is
	 * (currently) the same size as a vmcs, so we can re-use
	 * the vmx_alloc_vmcs routine.
	 */

	for_each_possible_cpu(cpu) {
		struct lcd_arch_vmcs *vmxon_buf;

		vmxon_buf = lcd_arch_alloc_vmcs(cpu);
		if (!vmxon_buf) {
			vmx_free_vmxon_areas();
			return -ENOMEM;
		}

		per_cpu(vmxon_area, cpu) = vmxon_buf;
	}

	/*
	 * Turn on vmx on each cpu
	 *
	 * Note: on_each_cpu disables preemption
	 */

	atomic_set(&vmx_enable_failed, 0);
	if (on_each_cpu(vmx_enable, NULL, 1)) {
		LCD_ERR("timeout waiting for VMX mode enable.\n");
		ret = -EIO;
		goto failed1; /* sadly we can't totally recover */
	}

	if (atomic_read(&vmx_enable_failed)) {
		ret = -EBUSY;
		goto failed2;
	}

	/*
	 * Init lcd_arch_thread cache (using instead of kmalloc since
	 * these structs need to be aligned properly)
	 */
	lcd_arch_cache = KMEM_CACHE(lcd_arch, 0);
	if (!lcd_arch_cache) {
		LCD_ERR("failed to set up kmem cache\n");
		ret = -ENOMEM;
		goto failed3;
	}

	return 0;

failed3:
failed2:
	on_each_cpu(vmx_disable, NULL, 1);
failed1:
	vmx_free_vmxon_areas();
	free_page((unsigned long)lcd_global_msr_bitmap);
	return ret;
}

void lcd_arch_exit(void)
{
	on_each_cpu(vmx_disable, NULL, 1);
	vmx_free_vmxon_areas();
	free_page((unsigned long)lcd_global_msr_bitmap);
	kmem_cache_destroy(lcd_arch_cache);
}
#endif
