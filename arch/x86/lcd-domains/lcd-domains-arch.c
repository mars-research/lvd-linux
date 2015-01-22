/*
 * LCD core VMX functions 
 *
 * Based on KVM and Dune.
 *
 * Authors:
 *
 *   Weibin Sun <wbsun@flux.utah.edu>
 *   Charlie Jacobsen <charlesj@cs.utah.edu>
 *
 */

#include <asm/virtext.h>
#include <asm/vmx.h>
#include <uapi/asm/vmx.h>
#include <asm/desc.h>
#include <asm/lcd-domains-arch.h>

#include <linux/bitmap.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/tboot.h>
#include <linux/slab.h>
#include <linux/kmsg_dump.h>

/* DEBUGGING -------------------------------------------------- */

#define LCD_ARCH_ERR(msg...) __lcd_arch_err(__FILE__, __LINE__, msg)
static inline void __lcd_arch_err(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: error: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_ARCH_MSG(msg...) __lcd_arch_msg(__FILE__, __LINE__, msg)
static inline void __lcd_arch_msg(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: note: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
#define LCD_ARCH_WARN(msg...) __lcd_arch_warn(__FILE__, __LINE__, msg)
static inline void __lcd_arch_warn(char *file, int lineno, char *fmt, ...)
{
	va_list args;
	printk(KERN_ERR "lcd-vmx: %s:%d: warning: ", file, lineno);
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}

/* VMX DATA STRUCTURES -------------------------------------------------- */

struct vmx_vmcs_config {
	int size;
	int order;
	u32 revision_id;
	u32 pin_based_exec_controls;
	u32 primary_proc_based_exec_controls;
	u32 secondary_proc_based_exec_controls;
	u32 vmexit_controls;
	u32 vmentry_controls;
};

struct vmx_capability {
	u32 ept;
	u32 vpid;
};

/*
 * Declared in inline assembly in vmx_enter
 */
extern const unsigned long vmx_return;

/* SHARED / PERCPU VARS -------------------------------------------------- */

static struct kmem_cache *lcd_arch_thread_cache;

static struct vmx_vmcs_config vmcs_config;
static struct vmx_capability vmx_capability;

static atomic_t vmx_enable_failed;
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(struct lcd_arch_vmcs *, vmxon_area);

static struct {
	DECLARE_BITMAP(bitmap, VMX_NR_VPIDS);
	spinlock_t lock;
} vpids;

static DEFINE_PER_CPU(struct lcd_arch_thread *, local_lcd_arch_thread);

static unsigned long *msr_bitmap;

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

	LCD_ARCH_MSG("  MSR LOW:             0x%08x\n", msr_low);
	LCD_ARCH_MSG("  ATTEMPTED CONTROLS:  0x%08x\n", controls);
	LCD_ARCH_MSG("  MSR HIGH:            0x%08x\n", msr_high);
	LCD_ARCH_MSG("  RESERVED BIT MASK:   0x%08x\n", mask);

	/*
	 * For each bit, if the reserved mask is not set *and* the msr high
	 * bit is not set, then the control bit should not be set.
	 */
	bad_high = ~msr_high & ~mask & controls;
	for (i = 0; i < 32; i++) {
		if (bad_high & 1)
			LCD_ARCH_MSG("  Control bit %d should be 0.\n", i);
		bad_high >>= 1;
	}

	/*
	 * For each bit, if the reserved mask is not set *and* the msr low
	 * bit is set, then the control bit should be set.
	 */
	bad_low = msr_low & ~mask & ~controls;
	for (i = 0; i < 32; i++) {
		if (bad_low & 1)
			LCD_ARCH_MSG("  Control bit %d should be 1.\n", i);
		bad_low >>= 1;
	}

	LCD_ARCH_MSG("See Intel SDM V3 24.{6,7,8,9} and Appendix A\n");
}

/* INVEPT / INVVPID --------------------------------------------------*/

static inline bool cpu_has_vmx_invvpid_single(void)
{
	return vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invvpid_global(void)
{
	return vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_context(void)
{
	return vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_global(void)
{
	return vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT;
}

static inline bool cpu_has_vmx_ept_ad_bits(void)
{
	return vmx_capability.ept & VMX_EPT_AD_BIT;
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
		LCD_ARCH_ERR("ext=%d, eptp=0x%llx\n", ext, eptp);
			
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
		LCD_ARCH_ERR("ext=%d, vpid=0x%hx\n", ext, vpid);
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

/* VMCS READ / WRITE --------------------------------------------------*/

/**
 * Takes vmcs from any state to {inactive, clear, not current}
 */
static void vmcs_clear(struct lcd_arch_vmcs *vmcs)
{
	u64 hpa = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&hpa), "m"(hpa)
                : "cc", "memory");
	if (error)
		LCD_ARCH_ERR("vmclear fail: %p/%llx\n",	vmcs, hpa);
}

/**
 * Takes vmcs to {active, current} on cpu. Any vmcs reads and writes
 * will affect this vmcs.
 */
static void vmcs_load(struct lcd_arch_vmcs *vmcs)
{
	u64 hpa = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&hpa), "m"(hpa)
                : "cc", "memory");
	if (error)
		LCD_ARCH_ERR("vmptrld %p/%llx failed\n", vmcs, hpa);
}

static __always_inline unsigned long vmcs_readl(unsigned long field)
{
	unsigned long value;

	asm volatile (ASM_VMX_VMREAD_RDX_RAX
                : "=a"(value) : "d"(field) : "cc");
	return value;
}

static __always_inline u16 vmcs_read16(unsigned long field)
{
	return vmcs_readl(field);
}

static __always_inline u32 vmcs_read32(unsigned long field)
{
	return vmcs_readl(field);
}

static __always_inline u64 vmcs_read64(unsigned long field)
{
	return vmcs_readl(field);
}

static noinline void vmwrite_error(unsigned long field, unsigned long value)
{
	LCD_ARCH_ERR("reg %lx value %lx (err %d)\n",
		field, value, vmcs_read32(VM_INSTRUCTION_ERROR));
	dump_stack();
}

static void vmcs_writel(unsigned long field, unsigned long value)
{
	u8 error;

	asm volatile (ASM_VMX_VMWRITE_RAX_RDX "; setna %0"
                : "=q"(error) : "a"(value), "d"(field) : "cc");
	if (unlikely(error))
		vmwrite_error(field, value);
}

static void vmcs_write16(unsigned long field, u16 value)
{
	vmcs_writel(field, value);
}

static void vmcs_write32(unsigned long field, u32 value)
{
	vmcs_writel(field, value);
}

static void vmcs_write64(unsigned long field, u64 value)
{
	vmcs_writel(field, value);
}

/* VMCS SETUP --------------------------------------------------*/

/**
 * Frees vmcs memory.
 */
static void vmx_free_vmcs(struct lcd_arch_vmcs *vmcs)
{
	free_pages((unsigned long)vmcs, vmcs_config.order);
}

/**
 * Allocates memory for a vmcs on cpu, and sets the
 * revision id.
 */
static struct lcd_arch_vmcs *vmx_alloc_vmcs(int cpu)
{
	int node;
	struct page *pages;
	struct lcd_arch_vmcs *vmcs;

	node = cpu_to_node(cpu);
	pages = alloc_pages_exact_node(node, GFP_KERNEL, vmcs_config.order);
	if (!pages)
		return NULL;
	vmcs = page_address(pages);
	memset(vmcs, 0, vmcs_config.size);

	vmcs->revision_id = vmcs_config.revision_id;

	return vmcs;
}

/* VMX ON/OFF --------------------------------------------------*/

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
	if (read_cr4() & X86_CR4_VMXE)
		return -EBUSY;
	write_cr4(read_cr4() | X86_CR4_VMXE);

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

	vmxon_buf = __get_cpu_var(vmxon_area);
	
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
	invvpid_global_context();
	invept_global_context();

	__get_cpu_var(vmx_enabled) = 1;

	LCD_ARCH_MSG("VMX enabled on CPU %d\n",	raw_smp_processor_id());
	return;

failed:
	atomic_inc(&vmx_enable_failed);
	LCD_ARCH_ERR("failed to enable VMX, err = %d\n", ret);
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
	if (__get_cpu_var(vmx_enabled)) {
		__vmxoff();
		write_cr4(read_cr4() & ~X86_CR4_VMXE);
		__get_cpu_var(vmx_enabled) = 0;
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
			vmx_free_vmcs(per_cpu(vmxon_area, cpu));
			per_cpu(vmxon_area, cpu) = NULL;
		}
	}
}

/* VMX SETTINGS --------------------------------------------------*/

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
static int vmcs_config_basic_settings(struct vmx_vmcs_config *vmcs_conf)
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
	vmcs_conf->order = get_order(vmcs_config.size);
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
static int setup_vmcs_config(struct vmx_vmcs_config *vmcs_conf)
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
		LCD_ARCH_ERR("pin based exec controls not allowed\n");
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
		LCD_ARCH_ERR("primary proc based exec ctrls not allowed\n");
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
	 * -- WBINVD Exit
	 *
	 * Note: Unrestricted guest and INVPCID not available on
	 * emulab machines.
	 */
	secondary_proc_based_exec_controls = SECONDARY_EXEC_ENABLE_EPT |
		SECONDARY_EXEC_RDTSCP |
		SECONDARY_EXEC_ENABLE_VPID |
		SECONDARY_EXEC_WBINVD_EXITING;
	if (adjust_vmx_controls(&secondary_proc_based_exec_controls,
					SECONDARY_EXEC_RESERVED_MASK,
					MSR_IA32_VMX_PROCBASED_CTLS2) < 0) {
		LCD_ARCH_ERR("secondary proc based exec ctls not allowed\n");
		print_vmx_controls(secondary_proc_based_exec_controls,
				SECONDARY_EXEC_RESERVED_MASK,
				MSR_IA32_VMX_PROCBASED_CTLS2);
		return -EIO;
	}

	/*
	 * Remember the EPT and VPID capabilities
	 */
	rdmsr(MSR_IA32_VMX_EPT_VPID_CAP,
		vmx_capability.ept, vmx_capability.vpid);


	/*
	 * VM Exit Controls (Intel SDM V3 24.7)
	 *
	 * -- Host Address Space (host in 64-bit mode on vm exit)
	 * -- Acknowledge interrupts on vm exit
	 * -- Save / load IA-32 EFER MSR on exit
	 * -- Save debug controls    / needed for emulab machines
	 */
	vmexit_controls = VM_EXIT_HOST_ADDR_SPACE_SIZE |
		VM_EXIT_ACK_INTR_ON_EXIT |
		VM_EXIT_SAVE_IA32_EFER |
		VM_EXIT_LOAD_IA32_EFER |
		VM_EXIT_SAVE_DEBUG_CONTROLS;
	if (adjust_vmx_controls(&vmexit_controls, 
					VM_EXIT_RESERVED_MASK,
					MSR_IA32_VMX_EXIT_CTLS) < 0) {
		LCD_ARCH_ERR("vmexit controls not allowed\n");
		
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
	 */
	vmentry_controls = VM_ENTRY_IA32E_MODE |
		VM_ENTRY_LOAD_IA32_EFER |
		VM_ENTRY_LOAD_DEBUG_CONTROLS;
	if (adjust_vmx_controls(&vmentry_controls,
					VM_ENTRY_RESERVED_MASK,
					MSR_IA32_VMX_ENTRY_CTLS) < 0) {
		LCD_ARCH_ERR("vm entry controls not allowed\n");
		
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

/* VMX INIT / EXIT -------------------------------------------------- */

static void lcd_arch_tests(void);

int lcd_arch_init(void)
{
	int ret;
	int cpu;

	/*
	 * Check For VMX Features
	 */

	if (!cpu_has_vmx()) {
		LCD_ARCH_ERR("CPU does not support VMX\n");
		return -EIO;
	}

	if (setup_vmcs_config(&vmcs_config) < 0)
		return -EIO;

	/*
	 * Set up default MSR bitmap
	 */

	msr_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!msr_bitmap) {
		ret = -ENOMEM;
		goto failed1;
	}	

	memset(msr_bitmap, 0xff, PAGE_SIZE);
	vmx_disable_intercept_for_msr(msr_bitmap, MSR_FS_BASE);
	vmx_disable_intercept_for_msr(msr_bitmap, MSR_GS_BASE);

	/*
	 * Initialize VPID bitmap spinlock
	 */
	spin_lock_init(&vpids.lock);

	/*
	 * VPID 0 is reserved for host. See INVVPID instruction.
	 */
	set_bit(0, vpids.bitmap); 

	/*
	 * Allocate vmxon buffers for each cpu. A vmxon buffer is
	 * (currently) the same size as a vmcs, so we can re-use
	 * the vmx_alloc_vmcs routine.
	 */

	for_each_possible_cpu(cpu) {
		struct lcd_arch_vmcs *vmxon_buf;

		vmxon_buf = vmx_alloc_vmcs(cpu);
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
		LCD_ARCH_ERR("timeout waiting for VMX mode enable.\n");
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
	lcd_arch_thread_cache = kmem_cache_create("lcd_arch_thread", 
						sizeof(struct lcd_arch_thread),
						__alignof__(struct lcd_arch_thread),
						0, NULL);
	if (!lcd_arch_thread_cache) {
		LCD_ARCH_ERR("failed to set up kmem cache\n");
		ret = -ENOMEM;
		goto failed3;
	}

	/*
	 * Run tests
	 */
	lcd_arch_tests();

	return 0;

failed3:
failed2:
	on_each_cpu(vmx_disable, NULL, 1);
failed1:
	vmx_free_vmxon_areas();
	free_page((unsigned long)msr_bitmap);
	return ret;
}

void lcd_arch_exit(void)
{
	on_each_cpu(vmx_disable, NULL, 1);
	vmx_free_vmxon_areas();
	free_page((unsigned long)msr_bitmap);
	kmem_cache_destroy(lcd_arch_thread_cache);
}

/* VMX EPT -------------------------------------------------- */

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
		 * Page table entry. Set EPT memory type to write back
		 * and ignore page attribute table.
		 */
		*epte |= (VMX_EPT_IPAT_BIT |
			(VMX_EPTE_MT_WB << VMX_EPT_MT_EPTE_SHIFT));
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
			
			if (!create) {
				LCD_ARCH_ERR("attempted lookup for unmapped gpa %lx, create was not allowed\n",
					gpa_val(a));
				return -ENOENT;
			}
			/*
			 * Get host virtual addr of fresh page, and
			 * set the epte's addr to the host physical addr
			 */
			page = __hva(__get_free_page(GFP_KERNEL));
			if (!hva_val(page)) {
				LCD_ARCH_ERR("alloc failed\n");
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
		LCD_ARCH_ERR("would overwrite hpa %lx with hpa %lx\n",
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
			LCD_ARCH_ERR("error mapping gpa %lx to hpa %lx\n",
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

int lcd_arch_ept_unmap_range(struct lcd_arch *lcd, gpa_t ga_start, 
			unsigned long npages)
{
	unsigned long off;
	unsigned long len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_arch_ept_unmap(lcd, gpa_add(ga_start, off))) {
			LCD_ARCH_ERR("error unmapping gpa %lx\n",
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
		LCD_ARCH_ERR("gpa %lx is not mapped\n",
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
 * the page containing the dir.
 *
 * 0 = pml4
 * 1 = pdpt
 * 2 = page dir
 * 3 = page table
 */
static void vmx_free_ept_dir_level(lcd_arch_epte_t *dir, int level)
{
	int idx;
	
	if (level == 3) {
		/*
		 * Base case of recursion
		 *
		 * Just make sure none of the pte's are present, to
		 * ensure there are no memory leaks.
		 */
		for (idx = 0; idx < LCD_ARCH_PTRS_PER_EPTE; idx++) {
			if (vmx_epte_present(dir[idx])) {
				LCD_ARCH_ERR("potential memory leak at hva %lx (hpa %lx, pt idx %d)\n",
					hva_val(vmx_epte_hva(dir[idx])),
					hpa_val(vmx_epte_hpa(dir[idx])));
		}
	} else {
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

/**
 * Frees all memory associated with ept structures
 * (but not the mapped memory itself! -- this can
 * lead to memory leaks, but is better than potential
 * double frees that crash the machine or scrog the disk!).
 */
static void vmx_free_ept(struct lcd_arch *lcd)
{
	lcd_arch_epte_t *dir;
	/*
	 * Get pml4 table
	 */
	dir = lcd->ept.root;
	vmx_free_ept_dir_level(dir, 0);
}

/**
 * Initializes the EPT's root global page directory page, the
 * VMCS pointer, and the spinlock.
 */
int vmx_init_ept(struct lcd_arch *lcd)
{
	hva_t page;
	u64 eptp;

	/*
	 * Alloc the root global page directory page
	 */

	page = __hva(get_zeroed_page(GFP_KERNEL));
	if (!hva_val(page)) {
		LCD_ARCH_ERR("failed to alloc page\n");
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
	lcd->ept.vmcs_ptr = eptp;

	/*
	 * Init the mutex
	 */
	mutex_init(&lcd_arch->ept.lock);

	return 0;
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
			LCD_ARCH_ERR("host seg sel points to ldt, which is not present\n");
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
		LCD_ARCH_ERR("error looking up host tss\n");
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
static void vmx_setup_vmcs_host(struct lcd_arch_thread *t)
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
static void vmx_setup_vmcs_msr(struct lcd_arch_thread *t)
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

	vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(t->msr_autoload.host));
	vmcs_write64(VM_EXIT_MSR_STORE_ADDR, __pa(t->msr_autoload.guest));
	vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(t->msr_autoload.guest));

	for (i = 0; i < LCD_ARCH_NUM_AUTOLOAD_MSRS; i++) {
	
		e = &t->msr_autoload.host[i];
		e->index = autoload_msrs[i];
		rdmsrl(e->index, val);
		e->value = val;

		e = &t->msr_autoload.guest[i];
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
static void vmx_setup_vmcs_guest_regs(struct lcd_arch_thread *t)
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
static void vmx_setup_vmcs_guest_settings(struct lcd_arch_thread *t,
					struct lcd_arch *lcd_arch)
{
	/*
	 * VPID
	 */
	vmcs_write16(VIRTUAL_PROCESSOR_ID, vcpu->vpid);
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
	 * XXX: We aren't locking here ...
	 */
	vmcs_write64(EPT_POINTER, t->lcd_arch->ept.vmcs_ptr);
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
static void vmx_setup_vmcs(struct lcd_arch_thread *t, struct lcd_arch *lcd_arch)
{
	/*
	 * Set up guest part of vmcs, and guest exec
	 */
	vmx_setup_vmcs_guest_settings(t, lcd_arch);
	vmx_setup_vmcs_guest_regs(t);
	/*
	 * Set up MSR bitmap and autoloading
	 */
	vmx_setup_vmcs_msr(t);
	/*
	 * Set up host part of vmcs
	 */
	vmx_setup_vmcs_host(t);
}


/* VMCS LOADING -------------------------------------------------- */

/**
 * Updates an lcd_arch_thread's VMCS when it is moved to a different
 * cpu. (Linux uses per-cpu data that needs to be updated in
 * the lcd's VMCS.)
 */
static void __vmx_setup_cpu(struct lcd_arch_thread *t, int cur_cpu)
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
	struct lcd_arch_thread *t;
	t = ptr;
	BUG_ON(raw_smp_processor_id() != t->cpu);
	vmcs_clear(t->vmcs);
	if (__get_cpu_var(local_lcd_arch_thread) == t)
		__get_cpu_var(local_lcd_arch_thread) = NULL;
}

/**
 * Loads t on the calling cpu.
 *
 * Disables preemption. Call vmx_put_cpu() when finished.
 */
static void vmx_get_cpu(struct lcd_arch_thread *t)
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
	if (__get_cpu_var(local_lcd_arch_thread) != t) {

		__get_cpu_var(local_lcd_arch_thread) = t;

		if (t->cpu != cur_cpu) {

			/*
			 * t not active on this cpu
			 */
			if (t->cpu >= 0)
				/*
				 * t active on a different cpu;
				 * clear it there (active -> inactive)
				 */
				smp_call_function_single(t->cpu,
							__vmx_get_cpu_helper, 
							(void *) t, 1);
			else
				/*
				 * t inactive; clear it to get to
				 * initial vmcs state
				 *
				 * Intel SDM V3 24.11.3
				 */
				vmcs_clear(t->vmcs);

			/*
			 * Invalidate any vpid or ept cache lines
			 */
			invvpid_single_context(t->vpid);
			invept_single_context(t->lcd_arch->ept.vmcs_ptr);

			/*
			 * t is not in launched state
			 */
			t->launched = 0;

			/*
			 * Load vmcs pointer on this cpu
			 */
			vmcs_load(t->vmcs);

			/*
			 * Update cpu-specific data in vmcs
			 */
			__vmx_setup_cpu(t, cur_cpu);

			/*
			 * Remember which cpu we are active on
			 */
			t->cpu = cur_cpu;
		} else {
			vmcs_load(t->vmcs);
		}
	}
}

/**
 * Match with vmx_get_cpu.
 *
 * Enables preemption.
 */
static void vmx_put_cpu(struct lcd_arch_thread *t)
{
	put_cpu();
}

/* LCD_ARCH CREATE / DESTROY ---------------------------------------- */

struct lcd_arch* lcd_arch_create(void)
{
	struct lcd_arch *lcd_arch;

	/*
	 * Alloc lcd_arch
	 */
	lcd_arch = kzalloc(sizeof(*lcd_arch), GFP_KERNEL);
	if (!lcd_arch) {
		LCD_ARCH_ERR("failed to alloc lcd_arch");
		goto fail_alloc;
	}
	
	/*
	 * Set up ept
	 */
	if (vmx_init_ept(lcd_arch)) {
		LCD_ARCH_ERR("setting up etp");
		goto fail_ept;
	}

	/*
	 * Set up list
	 */
	mutex_init(&lcd_arch->lcd_arch_threads.lock);
	list_init(&lcd_arch->lcd_arch_threads.list);

	return lcd_arch;

fail_ept:
	kfree(lcd_arch);
fail_alloc:
	return NULL;
}

void lcd_arch_destroy(struct lcd_arch *lcd_arch)
{
	/*
	 * Assumes all lcd_arch_thread's are destroyed ...
	 */
	if (mutex_lock_interruptible(lcd_arch->lcd_arch_threads.lock)) {
		LCD_ARCH_ERR("interrupted, skipping checks and freeing");
		goto free_junk;
	}
	/*
	 * Warn
	 */
	if (!list_empty(&lcd_arch->lcd_arch_threads.list))
		LCD_ARCH_ERR("lcd_arch still contains some threads...");
	mutex_unlock(lcd_arch->lcd_arch_threads.lock);

free_junk:

	invept_single_context(lcd_arch->ept.vmcs_ptr);
	vmx_free_ept(lcd_arch);
	kfree(lcd_arch);
}


/* LCD_ARCH_THREAD CREATE / DESTROY ---------------------------------------- */

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

/**
 * Reserves a vpid and sets it in the vcpu.
 */
static int vmx_allocate_vpid(struct lcd_arch_thread *t)
{
	int vpid;

	t->vpid = 0;

	spin_lock(&vpids.lock);
	vpid = find_first_zero_bit(vpids.bitmap, VMX_NR_VPIDS);
	if (vpid < VMX_NR_VPIDS) {
		t->vpid = vpid;
		__set_bit(vpid, vpids.bitmap);
	}
	spin_unlock(&vpids.lock);

	return vpid >= VMX_NR_VPIDS;
}

/**
 * Frees a vpid.
 */
static void vmx_free_vpid(struct lcd_arch_thread *t)
{
	spin_lock(&vpids.lock);
	if (t->vpid != 0)
		__clear_bit(vmx->vpid, vpids.bitmap);
	spin_unlock(&vpids.lock);
}

struct lcd_arch_thread* lcd_arch_add_thread(struct lcd_arch *lcd_arch)
{
	struct lcd_arch_thread* t;

	/*
	 * Alloc lcd_arch_thread
	 */
	t = kmem_cache_zalloc(lcd_arch_thread_cache, GFP_KERNEL);
	if (!t) {
		LCD_ARCH_ERR("failed to alloc lcd_arch_thread\n");
		goto fail_vcpu;
	}

	/*
	 * Alloc vmcs
	 */
	t->vmcs = vmx_alloc_vmcs(raw_smp_processor_id());
	if (!t->vmcs) {
		LCD_ARCH_ERR("failed to alloc vmcs\n");
		goto fail_vmcs;
	}

	if (vmx_allocate_vpid(t)) {
		LCD_ARCH_ERR("failed to alloc vpid\n");
		goto fail_vpid;
	}

	/*
	 * Not loaded on a cpu right now
	 */
	t->cpu = -1;

	/*
	 * Initialize VMCS register values and settings
	 * 
	 * Preemption disabled while doing so ...
	 */
	vmx_get_cpu(t);
	vmx_setup_vmcs(t, lcd_arch);
	vmx_put_cpu(t);
	
	/*
	 * Add t to lcd_arch's list
	 */
	t->lcd_arch = lcd_arch;
	list_init(&t.lcd_arch_threads);
	if (mutex_lock_interruptible(lcd_arch->lcd_arch_threads.lock))
		goto fail_list;

	list_add(&t->lcd_arch_threads, &lcd_arch->lcd_arch_threads.list);

	mutex_unlock(lcd_arch->lcd_arch_threads.lock);

	return t;

fail_list:
	vmx_free_vpid(t);
fail_vpid:
	vmx_free_vmcs(t->vmcs);
fail_vmcs:
	kmem_cache_free(lcd_arch_thread_cache, t);
fail_vcpu:
	return NULL;
}

void lcd_arch_destroy_thread(struct lcd_arch_thread *t)
{
	/*
	 * Premption Disabled
	 *
	 * The call to vmx_get_cpu is done because if t is
	 * active on a different cpu, it needs to be
	 * vmclear'd there (and vmx_get_cpu will do
	 * that, as a side effect).
	 *
	 * There might be alternative ways, but this works ...
	 */
	vmx_get_cpu(t);
	/*
	 * Invalidate any cached vpid mappings in tlb.
	 */
	invvpid_single_context(t->vpid);
	/*
	 * VM clear on this cpu
	 */
	vmcs_clear(t->vmcs);
	__get_cpu_var(local_lcd_arch_thread) = NULL;
	/*
	 * Preemption enabled
	 */
	vmx_put_cpu(t);
	/*
	 * Remove t from containing lcd_arch
	 */
	if (mutex_lock_interruptible(t->lcd_arch->lcd_arch_threads.lock)) {
		LCD_ARCH_ERR("interrupted, still try to free ...");
		goto free_rest;
	}

	list_del(&t->lcd_arch_threads);	

	mutex_unlock(t->lcd_arch->lcd_arch_threads.lock);

free_rest:
	/*
	 * Free remaining junk and t
	 */
	vmx_free_vpid(t);
	vmx_free_vmcs(t->vmcs);
	kmem_cache_free(lcd_arch_thread_cache, t);
}

/* VMX EXIT HANDLING -------------------------------------------------- */

/**
 * Skips to next instruction in lcd. This should only be
 * called when a vm exit occurs that sets the exit
 * instruction length. See Intel SDM V3 27.2.4.
 */
static void vmx_step_instruction(void)
{
	vmcs_writel(GUEST_RIP, vmcs_readl(GUEST_RIP) +
		vmcs_read32(VM_EXIT_INSTRUCTION_LEN));
}

/**
 * Processes a vmcall. For now, the only reason the lcd
 * would execute a vmcall is for a syscall to the
 * hypervisor.
 */
static int vmx_handle_vmcall(struct lcd_arch *vcpu)
{
	return LCD_ARCH_STATUS_SYSCALL;
}

/**
 * Processes an external interrupt (e.g. timer interrupt) by
 * emulating 64-bit interrupt handling. Kernel preemption should
 * be disabled, otherwise the interrupt handler could switch to
 * another conflicting task. Unlike kvm, interrupts are assumed
 * to be *enabled*, but will be disabled when the interrupt
 * handler is called, per the ia32 spec (Intel SDM V3 6.8.1).
 */
static int vmx_handle_external_intr(struct lcd_arch *vcpu)
{
	unsigned int vector;
	unsigned long entry;
	unsigned long sp_tmp;
	gate_desc *idt;
	gate_desc *gate;
	/*
	 * Load interrupt entry address
	 *
	 * Intel SDM V3 24.9.2, 27.2.2 (for vmx intr info)
	 * Intel SDM V3 6.14.1 (for idt layout)
	 */
	vector =  vcpu->exit_intr_info & INTR_INFO_VECTOR_MASK;
	idt = vmx_host_idt();
	gate = idt + vector;
	entry = gate_offset(*gate);

	/*
	 * Disable interrupts, per what the interrupt handler expects.
	 *
	 * Intel SDM V3 6.8.1
	 */
	local_irq_disable();

	/*
	 * Emulate 64-bit interrupt handling
	 *
	 * Intel SDM V3 6.14
	 */
	asm volatile(
		/*
		 * Save current stack pointer (notice & in asm flags)
		 */
		"mov %%" _ASM_SP ", %[sp]\n\t"
		/*
		 * Align sp to 16 bits (this should be OK under normal
		 * conditions since stack frames are 16-bit aligned and
		 * any junk in this stack frame is no longer needed -- we
		 * just needed to calc handler_addr, which is now in a
		 * register due to the r constraint).
		 */
		"and $0xfffffffffffffff0, %%" _ASM_SP "\n\t"
		/*
		 * %ss and sp are always pushed
		 */
		"push $%c[ss]\n\t"
		"push %[sp]\n\t"
		/*
		 * Push %rflags. Ensure `interrupts enabled' bit is
		 * set so that interrupts will be enabled on return
		 * (and maybe for some other reason?).
		 */
		"pushf\n\t"
		"orl $0x200, (%%" _ASM_SP ")\n\t"
		/*
		 * Push %cs
		 */
		__ASM_SIZE(push) " $%c[cs]\n\t"
		/*
		 * Call will push %rip, and jump to handler. Error code
		 * not needed for external interrupts.
		 *
		 * Interrupt service routine will return here.
		 */
		"call *%[entry]\n\t"
		:
		[sp]"=&r"(sp_tmp)
		:
		[entry]"r"(entry),
		[ss]"i"(__KERNEL_DS),
		[cs]"i"(__KERNEL_CS)
		);
	return LCD_ARCH_STATUS_EXT_INTR;
}

/**
 * Processes hardware exceptions -- page faults, general protection
 * exceptions, etc. For now, we have the host handle all except page
 * faults.
 */
static int vmx_handle_hard_exception(struct lcd_arch *vcpu)
{
	unsigned int vector;
	/*
	 * Intel SDM V3 24.9.2, 27.2.2
	 */
	vector = vcpu->exit_intr_info & INTR_INFO_VECTOR_MASK;
	switch (vector) {
	case 14:
		/*
		 * Guest virtual page fault
		 *
		 * Set page fault address, and return status code.
		 */
		vcpu->run_info.gv_fault_addr = __gva(vcpu->exit_qualification);
		return LCD_ARCH_STATUS_PAGE_FAULT;
	default:
		printk(KERN_ERR "lcd vmx: hw exception:\n");
		printk(KERN_ERR "         vector: %x, info: %x\n",
			vector, vcpu->exit_intr_info);
		printk(KERN_ERR "         letting host handle it\n");
		vmx_handle_external_intr(vcpu);
		return -EIO;
	}
}

/**
 * Processes software / hardware exceptions and nmi's generated
 * while lcd was running. For now, the host handles all exceptions/nmi's.
 */
static int vmx_handle_exception_nmi(struct lcd_arch *vcpu)
{
	int type;
	type = vcpu->exit_intr_info & INTR_INFO_INTR_TYPE_MASK;
	switch (type) {
	case INTR_TYPE_HARD_EXCEPTION:
		/*
		 * Page fault, trap, machine check, gp ...
		 */
		return vmx_handle_hard_exception(vcpu);
	default:
		/*
		 * NMI, div by zero, overflow, ...
		 */
		printk(KERN_ERR "lcd vmx: exception or nmi:\n");
		printk(KERN_ERR "         interrupt info: %x\n",
			vcpu->exit_intr_info);
		printk(KERN_ERR "         letting host handle it\n");
		return vmx_handle_external_intr(vcpu);
		//return -EIO;
	}
}

/**
 * Simple processing of ept violation (e.g., violation due to
 * an access to unmapped memory).
 */
static int vmx_handle_ept(struct lcd_arch *vcpu)
{
	/*
	 * Intel SDM V3 27.2.1
	 */
	vcpu->run_info.gv_fault_addr = __gva(vmcs_readl(GUEST_LINEAR_ADDRESS));
	vcpu->run_info.gp_fault_addr = 
		__gpa(vmcs_readl(GUEST_PHYSICAL_ADDRESS));
	return LCD_ARCH_STATUS_EPT_FAULT;
}

/**
 * Checking and emulation of control register accessing.
 * All accesses to %cr0, %cr4, and %cr8 lead to vm exits, and are
 * not currently allowed. Loads / stores to %cr3 lead to
 * vm exits (required for emulab machines) and are emulated.
 */
static int vmx_handle_control_reg(struct lcd_arch *vcpu)
{
	int control_reg;
	int access_type;
	int general_reg;
	/*
	 * Intel SDM V3 27.2.1
	 */
	
	/*
	 * Determine the control and general purpose registers involved, 
	 * and the access type.
	 */
	control_reg = vcpu->exit_qualification & CONTROL_REG_ACCESS_NUM;
	access_type = vcpu->exit_qualification & CONTROL_REG_ACCESS_TYPE;
	general_reg = vcpu->exit_qualification & CONTROL_REG_ACCESS_REG;

	switch (control_reg) {
	case 3:
		/*
		 * %cr3
		 */
		if (access_type == 0) {
			/*
			 * Move to
			 */
			vmcs_writel(GUEST_CR3, vcpu->regs[general_reg]);
		} else {
			/*
			 * Move from
			 */
			vcpu->regs[general_reg] = vmcs_readl(GUEST_CR3);
		}
		/*
		 * Step past instruction that caused exit
		 */
		vmx_step_instruction();
		return LCD_ARCH_STATUS_CR3_ACCESS;
	default:
		printk(KERN_ERR "lcd vmx: attempted access to protected cr\n");
		return -EIO;
	}
	

}

/* VMX RUN -------------------------------------------------- */

/**
 * Low-level vmx launch / resume to enter non-root mode on cpu with
 * the current vmcs.
 */
static int __noclone vmx_enter(struct lcd_arch *vcpu)
{
	asm(
		/* 
		 * Store host registers on stack (all other regs are
		 * clobbered)
		 */
		"push %%rdx \n\t"
		"push %%rbp \n\t"
		"push %%rcx \n\t" /* placeholder for guest %rcx (see below) */
		"push %%rcx \n\t" /* copy of lcd_arch vcpu pointer */
		/*
		 * Remember the current %rsp *and* store it
		 * in the vmcs.
		 * (je 1f jumps forward to numeric label 1)
		 */
		"cmp %%rsp, %c[host_rsp](%0) \n\t"
		"je 1f \n\t"
		"mov %%rsp, %c[host_rsp](%0) \n\t"
		"mov $%c[host_rsp_field], %%rdx \n\t"
		ASM_VMX_VMWRITE_RSP_RDX "\n\t"
		"1: \n\t"
		/*
		 * Re-load %cr2 (page fault addr) if changed
		 */
		"mov %c[cr2](%0), %%rax \n\t"
		"mov %%cr2, %%rdx \n\t"
		"cmp %%rax, %%rdx \n\t"
		"je 2f \n\t"
		"mov %%rax, %%cr2 \n\t"
		"2: \n\t"
		/*
		 * Check if vmlaunch or vmresume is needed
		 */
		"cmpl $0, %c[launched](%0) \n\t"
		/*
		 * Load guest registers.  Don't clobber flags.
		 */
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
		"mov %c[rcx](%0), %%rcx \n\t" /* kills %0 (host %rcx) */
		/*
		 * Enter guest mode
		 */
		"jne 3f \n\t"
		ASM_VMX_VMLAUNCH "\n\t"
		"jmp vmx_return_lbl \n\t"
		"3: " ASM_VMX_VMRESUME "\n\t"
		"vmx_return_lbl: "

		/*
		 * Save guest registers, load host registers, keep flags
		 */

		/*
		 * Store guest %rcx at rsp + wordsize (placeholder)
		 */
		"mov %0, %c[wordsize](%%rsp) \n\t"
		/*
		 * Pop lcd_arch vcpu pointer into %rcx (%0) 
		 * (host %rcx is at bottom of stack)
		 */
		"pop %0 \n\t"
		"mov %%rax, %c[rax](%0) \n\t"
		"mov %%rbx, %c[rbx](%0) \n\t"
		
		/*
		 * Pop guest %rcx that was stored above
		 */
		"popq %c[rcx](%0) \n\t"

		/*
		 * Store remaining regs
		 */
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

		"pop  %%rbp \n\t"
		"pop  %%rdx \n\t"
		
		/*
		 * Read flags
		 */
		"setbe %c[fail](%0) \n\t"

		/*
		 * Return %rip info (needed to store in HOST_RIP
		 * field in vmcs)
		 */
		".pushsection .rodata \n\t"
		".global vmx_return \n\t"
		"vmx_return: " _ASM_PTR " vmx_return_lbl \n\t"
		".popsection"
		: : "c"(vcpu),
		  [host_rsp_field]"i"(HOST_RSP),
		  [launched]"i"(offsetof(struct lcd_arch, launched)),
		  [fail]"i"(offsetof(struct lcd_arch, fail)),
		  [host_rsp]"i"(offsetof(struct lcd_arch, host_rsp)),
		  [rax]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RAX])),
		  [rbx]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RBX])),
		  [rcx]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RCX])),
		  [rdx]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RDX])),
		  [rsi]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RSI])),
		  [rdi]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RDI])),
		  [rbp]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_RBP])),
		  [r8]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R8])),
		  [r9]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R9])),
		  [r10]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R10])),
		  [r11]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R11])),
		  [r12]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R12])),
		  [r13]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R13])),
		  [r14]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R14])),
		  [r15]"i"(offsetof(struct lcd_arch, regs[LCD_ARCH_REGS_R15])),
		  [cr2]"i"(offsetof(struct lcd_arch, cr2)),
		  [wordsize]"i"(sizeof(ulong))
		: "cc", "memory"
		  , "rax", "rdx", "rbx", "rdi", "rsi"
		  , "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
		);

	vcpu->launched = 1;

	if (unlikely(vcpu->fail)) {
		/*
		 * See Intel SDM V3 30.4 for error codes
		 */
		printk(KERN_ERR "lcd vmx: failure detected (err %x)\n",
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

int lcd_arch_run(struct lcd_arch *vcpu)
{
	int exit_reason;
	int ret;

	/*
	 * Load the lcd and invalidate any cached mappings.
	 *
	 * *preemption disabled*
	 */
	vmx_get_cpu(vcpu);

	/*
	 * Enter lcd
	 */
	exit_reason = vmx_enter(vcpu);

	/*
	 * Handle exit reason
	 *
	 * Intel SDM V3 Appendix C
	 */
	switch (exit_reason) {
	case EXIT_REASON_EXCEPTION_NMI:
		ret = vmx_handle_exception_nmi(vcpu);
		break;
	case EXIT_REASON_EXTERNAL_INTERRUPT:
		ret = LCD_ARCH_STATUS_EXT_INTR;
		break;
		ret = vmx_handle_external_intr(vcpu);
		break;
	case EXIT_REASON_VMCALL:
		ret = vmx_handle_vmcall(vcpu);
		break;
	case EXIT_REASON_EPT_VIOLATION:
		ret = vmx_handle_ept(vcpu);
		break;
	case EXIT_REASON_CR_ACCESS:
		ret = vmx_handle_control_reg(vcpu);
		break;
	default:
		printk(KERN_ERR "lcd vmx: unhandled exit reason: %x\n",
			exit_reason);
		ret = -EIO;
		break;
	}

	/*
	 * Preemption enabled
	 */
	vmx_put_cpu(vcpu);	

	return ret;
}

/* LCD RUNTIME ENV -------------------------------------------------- */

int lcd_arch_set_pc(struct lcd_arch *vcpu, gva_t a)
{
	vcpu->regs[LCD_ARCH_REGS_RIP] = gva_val(a);
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(vcpu);
	vmcs_writel(GUEST_RIP, gva_val(a));
	vmx_put_cpu(vcpu);
	return 0;
}

int lcd_arch_set_sp(struct lcd_arch_thread *t, gva_t a)
{
	vcpu->regs[LCD_ARCH_REGS_RSP] = gva_val(a);
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(vcpu);
	vmcs_writel(GUEST_RSP, gva_val(a));
	vmx_put_cpu(vcpu);
	return 0;
}

int lcd_arch_set_gva_root(struct lcd_arch *vcpu, gpa_t a)
{
	u64 cr3_ptr;

	cr3_ptr = gpa_val(a); /* no page write through, etc. ... */
	/*
	 * Must load vmcs to modify it
	 */
	vmx_get_cpu(vcpu);
	vmcs_writel(GUEST_CR3, cr3_ptr);
	vmx_put_cpu(vcpu);
	return 0;
}

/* CHECKING ---------------------------------------- */

static inline u16 vmx_get16(struct lcd_arch *vcpu, u64 field)
{
	u16 out;
	vmx_get_cpu(vcpu);
	out = vmcs_read16(field);
	vmx_put_cpu(vcpu);
	return out;
}

static inline u32 vmx_get32(struct lcd_arch *vcpu, u64 field)
{
	u32 out;
	vmx_get_cpu(vcpu);
	out = vmcs_read32(field);
	vmx_put_cpu(vcpu);
	return out;
}

static inline u64 vmx_getl(struct lcd_arch *vcpu, u64 field)
{
	u64 out;
	vmx_get_cpu(vcpu);
	out = vmcs_readl(field);
	vmx_put_cpu(vcpu);
	return out;
}

static inline int vmx_bad_phys_addr(u64 phys_addr)
{
	u32 phys_addr_width;
	u64 mask;

	/*
	 * Note: 64-bit processors are never limited to 32 bits by
	 * vmx basic capability msr. See Intel SDM V3 Appendix A.
	 */
	phys_addr_width = cpuid_eax(0x80000008) & 0xff;
	mask = ~((1UL << phys_addr_width) - 1);
	
	return (phys_addr & mask) != 0;
}

static inline u32 vmx_get_pin(struct lcd_arch *vcpu)
{
	return vmx_get32(vcpu, PIN_BASED_VM_EXEC_CONTROL);
}

static inline int vmx_pin_has(struct lcd_arch *vcpu, u32 field)
{
	return (vmx_get_pin(vcpu) & field) != 0;
} 

static inline u32 vmx_get_prim_exec(struct lcd_arch *vcpu)
{
	return vmx_get32(vcpu, CPU_BASED_VM_EXEC_CONTROL);
}

static inline int vmx_prim_exec_has(struct lcd_arch *vcpu, u32 field)
{
	return (vmx_get_prim_exec(vcpu) & field) != 0;
} 

static inline int vmx_has_sec_exec(struct lcd_arch *vcpu)
{
	return (vmx_get_prim_exec(vcpu) & 
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) != 0;
}

static inline u32 vmx_get_sec_exec(struct lcd_arch *vcpu)
{
	if (!vmx_has_sec_exec(vcpu)) {
		printk(KERN_ERR "lcd vmx get sec exec: bad check, something wrong; vcpu doesn't have secondary exec ctrls enabled\n");
	}
	BUG_ON(!vmx_has_sec_exec(vcpu));
	return vmx_get32(vcpu, SECONDARY_VM_EXEC_CONTROL);
}

static inline int vmx_sec_exec_has(struct lcd_arch *vcpu, u32 field)
{
	if (!vmx_has_sec_exec(vcpu)) {
		printk(KERN_ERR "lcd vmx get sec exec: bad check, something wrong; vcpu doesn't have secondary exec ctrls enabled\n");
	}
	BUG_ON(!vmx_has_sec_exec(vcpu));
	return (vmx_get_sec_exec(vcpu) & field) != 0;
} 

static inline u32 vmx_get_exit(struct lcd_arch *vcpu)
{
	return vmx_get32(vcpu, VM_EXIT_CONTROLS);
}

static inline int vmx_exit_has(struct lcd_arch *vcpu, u32 field)
{
	return (vmx_get_exit(vcpu) & field) != 0;
} 

static inline u32 vmx_get_entry(struct lcd_arch *vcpu)
{
	return vmx_get32(vcpu, VM_ENTRY_CONTROLS);
}

static inline int vmx_entry_has(struct lcd_arch *vcpu, u32 field)
{
	return (vmx_get_entry(vcpu) & field) != 0;
}

static inline int vmx_addr_is_canonical(u64 addr)
{
	signed long saddr;
	saddr = (signed long)addr;

	/*
	 * I couldn't find the precise defn for canonical in the manual,
	 * but from the web it appears to mean "the 47th bit is sign
	 * extended to bit 63".
	 */
	return ((saddr >> 47) == 0 ||
		(saddr >> 47) == -1);		
}

static inline int vmx_bool_eq(int b1, int b2)
{
	return (b1 == 0 && b2 == 0) || (b1 != 0 && b2 != 0);
}

static int vmx_check_exec_ctrls(struct lcd_arch *vcpu)
{
	u32 low32;
	u32 high32;
	u32 act32;
	u64 act64;

	/*
	 * Pin based controls
	 */
	rdmsr(MSR_IA32_VMX_PINBASED_CTLS, low32, high32);
	act32 = vmx_get_pin(vcpu);
	if (low32 & ~act32) {
		printk(KERN_ERR "lcd vmx: min pin based ctrls not set\n");
		return -1;
	}
	if (~high32 & act32) {
		printk(KERN_ERR "lcd vmx: above max pin based ctrls\n");
		return -1;
	}

	/*
	 * Primary exec controls
	 */
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS, low32, high32);
	act32 = vmx_get_prim_exec(vcpu);
	if (low32 & ~act32) {
		printk(KERN_ERR "lcd vmx: min prim exec ctrls not set\n");
		return -1;
	}
	if (~high32 & act32) {
		printk(KERN_ERR "lcd vmx: above max prim exec ctrls\n");
		return -1;
	}

	/*
	 * Secondary exec controls
	 */
	if (vmx_has_sec_exec(vcpu)) {
		rdmsr(MSR_IA32_VMX_PROCBASED_CTLS2, low32, high32);
		act32 = vmx_get_sec_exec(vcpu);
		if (low32 & ~act32) {
			printk(KERN_ERR "lcd vmx: min second exec ctrls not set\n");
			return -1;
		}
		if (~high32 & act32) {
			printk(KERN_ERR "lcd vmx: above max second exec ctrls\n");
			return -1;
		}
	}

	/*
	 * cr3 target count
	 */
	act32 = vmx_get32(vcpu, CR3_TARGET_COUNT);
	if (act32 > 4) {
		printk(KERN_ERR "lcd vmx: too many cr3 targets (%u > 4)\n",
			act32);
		return -1;
	}

	/*
	 * i/o bitmap addresses
	 */
	if (vmx_prim_exec_has(vcpu, CPU_BASED_USE_IO_BITMAPS)) {
		act64 = vmx_getl(vcpu, IO_BITMAP_A);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			printk(KERN_ERR "lcd vmx: bad io bitmap A addr\n");
			return -1;
		}
		act64 = vmx_getl(vcpu, IO_BITMAP_B);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			printk(KERN_ERR "lcd vmx: bad io bitmap B addr\n");
			return -1;
		}
	}

	/*
	 * msr bitmap address
	 */
	if (vmx_prim_exec_has(vcpu, CPU_BASED_USE_MSR_BITMAPS)) {
		act64 = vmx_getl(vcpu, MSR_BITMAP);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			printk(KERN_ERR "lcd vmx: bad msr bitmap addr %llx\n",
				act64);
			return -1;
		}
	}

	/*
	 * tpr shadow, check 1
	 */
	if (vmx_prim_exec_has(vcpu, CPU_BASED_TPR_SHADOW)) {
		printk(KERN_ERR "lcd vmx: tpr shadow checks unimplemented\n");
		return -1;
	}

	/*
	 * tpr shadow, check 2
	 */
	if (!vmx_prim_exec_has(vcpu, CPU_BASED_TPR_SHADOW) &&
		vmx_has_sec_exec(vcpu) &&
		(vmx_sec_exec_has(vcpu, SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE)
			||
			vmx_sec_exec_has(vcpu, SECONDARY_EXEC_APIC_REGISTER_VIRT)
			||
			vmx_sec_exec_has(vcpu, SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY))) {
		printk(KERN_ERR "lcd vmx: tpr shadow unset, but one of x2apic, apic reg virt, or virt intr deliv set\n");
		return -1;
	}

	/*
	 * nmi's
	 */
	if (!vmx_pin_has(vcpu, PIN_BASED_NMI_EXITING) && 
		vmx_pin_has(vcpu, PIN_BASED_VIRTUAL_NMIS)) {
		printk(KERN_ERR "lcd vmx: nmi exit unset but virt nmis set\n");
		return -1;
	}
	if (!vmx_pin_has(vcpu, PIN_BASED_NMI_EXITING) &&
		vmx_prim_exec_has(vcpu, CPU_BASED_VIRTUAL_NMI_PENDING)) {
			printk(KERN_ERR "lcd vmx: nmi exit unset but virt nmi pending set\n");
			return -1;
	}

	/*
	 * virtual apic page address
	 */
	if (vmx_has_sec_exec(vcpu) && vmx_sec_exec_has(vcpu, SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES)) {
		printk(KERN_ERR "lcd vmx: virt apic access checks not implemented\n");
		return -1;
	}

	/*
	 * virt x2apic
	 */
	if (vmx_has_sec_exec(vcpu) && vmx_sec_exec_has(vcpu, SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE)) {
		printk(KERN_ERR "lcd vmx: virt x2apic checks not implemented\n");
		return -1;
	}

	/*
	 * virt interrupt delivery
	 */
	if (vmx_has_sec_exec(vcpu) && vmx_sec_exec_has(vcpu, SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY)) {
		printk(KERN_ERR "lcd vmx: virt int deliv checks not implemented\n");
		return -1;
	}

	/*
	 * process posted interrupts
	 */
	if (vmx_pin_has(vcpu, PIN_BASED_POSTED_INTR)) {
		printk(KERN_ERR "lcd vmx: posted intr checks not implemented\n");
		return -1;
	}

	/*
	 * vpid
	 */
	if (vmx_has_sec_exec(vcpu) && 
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_ENABLE_VPID)) {
		if (vmx_get16(vcpu, VIRTUAL_PROCESSOR_ID) == 0) {
			printk(KERN_ERR "lcd vmx: vpid cannot be 0\n");
			return -1;
		}
	}

	/*
	 * eptp
	 */
	if (vmx_has_sec_exec(vcpu) && 
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_ENABLE_EPT)) {
		act64 = vmx_getl(vcpu, EPT_POINTER);
		if (!(vmx_capability.ept & VMX_EPTP_UC_BIT) &&
			(((act64 & ((1UL << VMX_EPT_MT_EPTE_SHIFT) - 1)) == 0))) {
			printk(KERN_ERR "lcd vmx: ept uncacheable not supported\n");
			return -1;
		}
		if (!(vmx_capability.ept & VMX_EPTP_WB_BIT) &&
			(((act64 & ((1UL << VMX_EPT_MT_EPTE_SHIFT) - 1)) == 6))) {
			printk(KERN_ERR "lcd vmx: ept write-back not supported\n");
			return -1;
		}
		if (((act64 >> VMX_EPT_MT_EPTE_SHIFT) & 0x7) != 3) {
			printk(KERN_ERR "lcd vmx: ept walk length must be 4\n");
			return -1;
		}
		if ((act64 & VMX_EPT_AD_ENABLE_BIT) &
			!(vmx_capability.ept & VMX_EPT_AD_BIT)) {
			printk(KERN_ERR "lcd vmx: ept access/dirty bit not supported\n");
			return -1;
		}
		if (act64 & 0xf80UL) {
			printk(KERN_ERR "lcd vmx: ept has rsrv bits set\n");
			return -1;
		}
		if (vmx_bad_phys_addr(act64 & 0xfff)) {
			printk(KERN_ERR "lcd vmx: ept bad addr %llx\n",
				act64);
			return -1;
		}
	}

	/*
	 * ept & unrestricted guest
	 */
	if (vmx_has_sec_exec(vcpu) && 
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_UNRESTRICTED_GUEST)) {
		printk(KERN_ERR "lcd vmx: unrestricted guest checks not implemented\n");
		return -1;
	}

	/*
	 * vm functions
	 */
	if (vmx_has_sec_exec(vcpu) &&
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_ENABLE_VMFUNCTIONS)) {
		printk(KERN_ERR "lcd vmx: vmfunction checking not implemented\n");
		return -1;
	}

	/*
	 * vmcs shadowing
	 */
	if (vmx_has_sec_exec(vcpu) &&
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_SHADOW_VMCS)) {
		printk(KERN_ERR "lcd vmx: vmcs shadow checking not implemented\n");
		return -1;
	}

	/*
	 * ept violation
	 */
	if (vmx_has_sec_exec(vcpu) &&
		vmx_sec_exec_has(vcpu, SECONDARY_EXEC_EPT_VIOLATION_VE)) {
		printk(KERN_ERR "lcd vmx: ept violation ve checking not implemented\n");
		return -1;
	}


	return 0;	
}

static inline int vmx_bad_msr_addr(u64 msr_addr, u32 msr_count)
{
	if (msr_count > 0) {
		return (msr_addr & 0xf) || 
			vmx_bad_phys_addr(msr_addr) ||
			vmx_bad_phys_addr(msr_addr + msr_count * 16 - 1);
	} else {
		return 0;
	}
}

static int vmx_check_exit_ctrls(struct lcd_arch *vcpu)
{
	u32 low32;
	u32 high32;
	u32 act32;

	/*
	 * min / max settings
	 */
	rdmsr(MSR_IA32_VMX_EXIT_CTLS, low32, high32);
	act32 = vmx_get_exit(vcpu);
	if (low32 & ~act32) {
		printk(KERN_ERR "lcd vmx: min exit ctrls not set\n");
		return -1;
	}
	if (~high32 & act32) {
		printk(KERN_ERR "lcd vmx: exit ctrls above max\n");
		return -1;
	}

	/*
	 * timer
	 */
	if (!vmx_pin_has(vcpu, PIN_BASED_VMX_PREEMPTION_TIMER) &&
		vmx_exit_has(vcpu, VM_EXIT_SAVE_VMX_PREEMPTION_TIMER)) {
		printk(KERN_ERR "lcd vmx: preempt timer unset, but save on exit set\n");
		return -1;
	}

	/*
	 * MSR load / store
	 */
	if (vmx_bad_msr_addr(vmx_getl(vcpu, VM_EXIT_MSR_STORE_ADDR),
				vmx_get32(vcpu, VM_EXIT_MSR_STORE_COUNT))) {
		printk(KERN_ERR "lcd vmx: bad exit msr store addr\n");
		return -1;
	}
	if (vmx_bad_msr_addr(vmx_getl(vcpu, VM_EXIT_MSR_LOAD_ADDR),
				vmx_get32(vcpu, VM_EXIT_MSR_LOAD_COUNT))) {
		printk(KERN_ERR "lcd vmx: bad exit msr load addr\n");
		return -1;
	}

	return 0;
}

static int vmx_check_entry_ctrls(struct lcd_arch *vcpu)
{
	u32 low32;
	u32 high32;
	u32 act32;

	/*
	 * min / max settings
	 */
	rdmsr(MSR_IA32_VMX_ENTRY_CTLS, low32, high32);
	act32 = vmx_get_entry(vcpu);
	if (low32 & ~act32) {
		printk(KERN_ERR "lcd vmx: min entry ctrls not set\n");
		return -1;
	}
	if (~high32 & act32) {
		printk(KERN_ERR "lcd vmx: entry ctrls above max\n");
		return -1;
	}

	/*
	 * interrupt info
	 */
	if (vmx_get32(vcpu, VM_ENTRY_INTR_INFO_FIELD) > 0) {
		printk(KERN_ERR "lcd vmx: entry interr info check not implemented\n");
		return -1;
	}

	/*
	 * MSR load
	 */
	if (vmx_bad_msr_addr(vmx_getl(vcpu, VM_ENTRY_MSR_LOAD_ADDR),
				vmx_get32(vcpu, VM_ENTRY_MSR_LOAD_COUNT))) {
		printk(KERN_ERR "lcd vmx: bad entry msr load addr\n");
		return -1;
	}

	/*
	 * smm
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_SMM) ||
		vmx_entry_has(vcpu, VM_ENTRY_DEACT_DUAL_MONITOR)) {
		printk(KERN_ERR "lcd vmx: entry smm / deact dual monitor checks not implemented\n");
		return -1;
	}
	
	return 0;
}

static inline int vmx_bad_host_sel(u16 sel)
{
	return (sel & SEGMENT_RPL_MASK) != 0 || (sel & SEGMENT_TI_MASK) != 0;
}

static int vmx_check_host_seg(struct lcd_arch *vcpu)
{
	u16 act16;
	u64 act64;

	/*
	 * cs
	 */
	act16 = vmx_get16(vcpu, HOST_CS_SELECTOR);
	if (vmx_bad_host_sel(act16) || act16 == 0) {
		printk(KERN_ERR "lcd vmx: bad host cs selector\n");
		return -1;
	}

	/*
	 * tr
	 */
	act16 = vmx_get16(vcpu, HOST_TR_SELECTOR);
	if (vmx_bad_host_sel(act16) || act16 == 0) {
		printk(KERN_ERR "lcd vmx: bad host tr selector\n");
		return -1;
	}

	/*
	 * ss
	 */
	act16 = vmx_get16(vcpu, HOST_SS_SELECTOR);
	if (vmx_bad_host_sel(act16) ||
		(!vmx_exit_has(vcpu, VM_EXIT_HOST_ADDR_SPACE_SIZE) &&
			act16 == 0)) {
		printk(KERN_ERR "lcd vmx: bad host ss selector\n");
		return -1;
	}

	/*
	 * ds, es, fs, gs
	 */
	if (vmx_bad_host_sel(vmx_get16(vcpu, HOST_DS_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(vcpu, HOST_ES_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(vcpu, HOST_FS_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(vcpu, HOST_GS_SELECTOR))) {
		printk(KERN_ERR "lcd vmx: bad ds, es, fs, or gs selector\n");
		return -1;
	}
	
	/*
	 * fs, gs, gdtr, idtr, tr base addresses
	 */
	act64 = vmx_getl(vcpu, HOST_FS_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		printk(KERN_ERR "lcd vmx: bad fs base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(vcpu, HOST_GS_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		printk(KERN_ERR "lcd vmx: bad gs base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(vcpu, HOST_GDTR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		printk(KERN_ERR "lcd vmx: bad gdtr base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(vcpu, HOST_IDTR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		printk(KERN_ERR "lcd vmx: bad idtr base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(vcpu, HOST_TR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		printk(KERN_ERR "lcd vmx: bad tr base addr %llx\n",
			act64);
		return -1;
	}

	return 0;
}

static int vmx_check_host_ctrl_regs(struct lcd_arch *vcpu)
{
	u64 act64;
	u64 min;
	u64 max;

	/*
	 * cr0 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR0_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR0_FIXED1, max);
	act64 = vmx_getl(vcpu, HOST_CR0);
	if (min & ~act64) {
		printk(KERN_ERR "lcd vmx: host cr0 missing min settings\n");
		return -1;
	}
	if (~max & act64) {
		printk(KERN_ERR "lcd vmx: host cr0 exceeds max settings\n");
		return -1;
	}
	/*
	 * cr4 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR4_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR4_FIXED1, max);
	act64 = vmx_getl(vcpu, HOST_CR4);
	if (min & ~act64) {
		printk(KERN_ERR "lcd vmx: host cr4 missing min settings\n");
		return -1;
	}
	if (~max & act64) {
		printk(KERN_ERR "lcd vmx: host cr4 exceeds max settings\n");
		return -1;
	}

	/*
	 * cr3
	 */
	if (vmx_bad_phys_addr(vmx_getl(vcpu, HOST_CR3) & PAGE_MASK)) {
		printk(KERN_ERR "lcd vmx: bad host cr3 address\n");
		return -1;
	}

	/*
	 * sysenter_{eip,esp}
	 */
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, HOST_IA32_SYSENTER_EIP)) ||
		!vmx_addr_is_canonical(vmx_getl(vcpu, HOST_IA32_SYSENTER_ESP))) {
		printk(KERN_ERR "lcd vmx: bad host sysenter eip or esp\n");
		return -1;
	}

	/*
	 * perf global msr
	 */
	if (vmx_exit_has(vcpu, VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL)) {
		printk(KERN_ERR "lcd vmx: vmexit perf global load check not implemented\n");
		return -1;
	}

	/*
	 * pat msr
	 */
	if (vmx_exit_has(vcpu, VM_EXIT_LOAD_IA32_PAT)) {
		printk(KERN_ERR "lcd vmx: vmexit pat msr load check not implemented\n");
		return -1;
	}

	/*
	 * efer msr
	 */
	if (vmx_exit_has(vcpu, VM_EXIT_LOAD_IA32_EFER)) {
		act64 = vmx_getl(vcpu, HOST_IA32_EFER);
		if (0x000000feUL & act64 ||
			(1UL << 9) & act64 ||
			act64 & ~((1UL << 12) - 1)) {
			printk(KERN_ERR "lcd vmx: host efer has some reserved bits set\n");
			return -1;
		}
		if (!vmx_bool_eq(vmx_exit_has(vcpu, VM_EXIT_HOST_ADDR_SPACE_SIZE), (act64 & EFER_LME) && (act64 & EFER_LMA))) {
			printk(KERN_ERR "lcd vmx: vmexit host addr space size mismatch with lme or lma in efer\n");
			return -1;
		}
	}

	return 0;
}

static int vmx_check_addr_size(struct lcd_arch *vcpu)
{
	u64 templ;

	rdmsrl(MSR_EFER, templ);
	if (!(templ & EFER_LMA)) {
		printk(KERN_ERR "lcd vmx: not in 64 bit mode\n");
		return -1;
	}

	if (!vmx_exit_has(vcpu, VM_EXIT_HOST_ADDR_SPACE_SIZE)) {
		printk(KERN_ERR "lcd vmx: host addr space size unset, needed to run in 64-bit mode\n");
		return -1;
	}

	if (!(vmx_getl(vcpu, HOST_CR4) & X86_CR4_PAE)) {
		printk(KERN_ERR "lcd vmx: pae bit in host cr4 field not set\n");
		return -1;
	}

	if (!vmx_addr_is_canonical(vmx_getl(vcpu, HOST_RIP))) {
		printk(KERN_ERR "lcd vmx: bad host rip\n");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_ctrl_regs(struct lcd_arch *vcpu)
{
	u64 act64;
	u64 min;
	u64 max;

	/*
	 * cr0 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR0_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR0_FIXED1, max);
	act64 = vmx_getl(vcpu, GUEST_CR0);
	if (min & ~act64) {
		printk(KERN_ERR "lcd vmx: guest cr0 missing min settings\n");
		return -1;
	}
	if (~max & act64) {
		printk(KERN_ERR "lcd vmx: guest cr0 exceeds max settings\n");
		return -1;
	}
	if ((act64 & X86_CR0_PG) && !(act64 & X86_CR0_PE)) {
		printk(KERN_ERR "lcd vmx: guest cr0 PG set but PE unset\n");
		return -1;
	}

	/*
	 * cr4 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR4_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR4_FIXED1, max);
	act64 = vmx_getl(vcpu, GUEST_CR4);
	if (min & ~act64) {
		printk(KERN_ERR "lcd vmx: guest cr4 missing min settings\n");
		return -1;
	}
	if (~max & act64) {
		printk(KERN_ERR "lcd vmx: guest cr4 exceeds max settings\n");
		return -1;
	}

	/*
	 * debug ctrls
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_LOAD_DEBUG_CONTROLS)) {
		act64 = vmx_getl(vcpu, GUEST_IA32_DEBUGCTL);
		if ((act64 & 0x3c) || (act64 >> 15)) {
			printk(KERN_ERR "lcd vmx: bad guest debug ctrl msr\n");
			return -1;
		}
	}

	/*
	 * ia-32e mode
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE)) {
		act64 = vmx_getl(vcpu, GUEST_CR0);
		if (!(vmx_getl(vcpu, GUEST_CR0) & X86_CR0_PG) || 
			!(vmx_getl(vcpu, GUEST_CR4) & X86_CR4_PAE)) {
			printk(KERN_ERR "lcd vmx: guest entry for ia32e mode requries cr0 PG bit and cr4 PAE bit\n");
			return -1;
		}
	}
	
	/*
	 * no ia-32e mode
	 */
	if (!vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) &&
		(vmx_getl(vcpu, GUEST_CR4) & X86_CR4_PCIDE)) {
		printk(KERN_ERR "lcd vmx: guest entry not ia32e, but cr4 PCIDE bit is set\n");
		return -1;
	}
		

	/*
	 * cr3
	 */
	if (vmx_bad_phys_addr(vmx_getl(vcpu, GUEST_CR3) & PAGE_MASK)) {
		printk(KERN_ERR "lcd vmx: bad guest cr3 address\n");
		return -1;
	}

	/*
	 * debug ctrls, check 2
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_LOAD_DEBUG_CONTROLS) &&
		(vmx_getl(vcpu, GUEST_DR7) >> 32)) {
		printk(KERN_ERR "lcd vmx: bad guest dr7 has high bits set\n");
		return -1;
	}

	/*
	 * sysenter_{eip,esp}
	 */
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_SYSENTER_EIP)) ||
		!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_SYSENTER_ESP))) {
		printk(KERN_ERR "lcd vmx: bad guest sysenter eip or esp\n");
		return -1;
	}

	/*
	 * perf global msr
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL)) {
		printk(KERN_ERR "lcd vmx: vmentry perf global load check not implemented\n");
		return -1;
	}

	/*
	 * pat msr
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_LOAD_IA32_PAT)) {
		printk(KERN_ERR "lcd vmx: vmentry pat msr load check not implemented\n");
		return -1;
	}

	/*
	 * efer msr
	 */
	if (vmx_entry_has(vcpu, VM_ENTRY_LOAD_IA32_EFER)) {
		act64 = vmx_getl(vcpu, GUEST_IA32_EFER);
		if (0x000000feUL & act64 ||
			(1UL << 9) & act64 ||
			act64 & ~((1UL << 12) - 1)) {
			printk(KERN_ERR "lcd vmx: guest efer has some reserved bits set\n");
			return -1;
		}
		if (!vmx_bool_eq(act64 & EFER_LME,
					vmx_entry_has(vcpu, 
						VM_ENTRY_IA32E_MODE))) {
			printk(KERN_ERR "lcd vmx: guest efer.lme != entry ia32e mode setting\n");
			return -1;
		}
		if ((vmx_getl(vcpu, GUEST_CR0) & X86_CR0_PG) &&
			!vmx_bool_eq(act64 & EFER_LME, act64 & EFER_LMA)) {
			printk(KERN_ERR "lcd vmx: guest cr0.pg is set, but guest efer.lma != efer.lme\n");
			return -1;
		}
	}

	return 0;
}

static inline int vmx_seg_type(u64 ar_bytes)
{
	return ar_bytes & AR_TYPE_MASK;
}

static inline int vmx_desc_type(u64 ar_bytes)
{
	return (ar_bytes & AR_S_MASK) >> 3;
}

static inline int vmx_seg_dpl(u64 ar_bytes)
{
	return AR_DPL(ar_bytes);
}

static inline int vmx_seg_pres(u64 ar_bytes)
{
	return (ar_bytes & AR_P_MASK) != 0;
}

static inline int vmx_seg_l_mode(u64 ar_bytes)
{
	return (ar_bytes & AR_L_MASK) != 0;
}

static inline int vmx_seg_db(u64 ar_bytes)
{
	return (ar_bytes & AR_DB_MASK) != 0;
}

static inline int vmx_seg_gran(u64 ar_bytes)
{
	return (ar_bytes & AR_G_MASK) != 0;
}

static inline int vmx_seg_usable(u64 ar_bytes)
{
	return (ar_bytes & AR_UNUSABLE_MASK) == 0;
}

static inline int vmx_seg_bad_limit(u64 ar_bytes, u32 limit)
{
	if ((limit & ((1 << 12) - 1)) != 0xFFF) {
		if (vmx_seg_gran(ar_bytes)) {
			return -1;
		}
	}
	if (limit >> 19) {
		if (!vmx_seg_gran(ar_bytes)) {
			return -1;
		}
	}
	return 0;
}

static inline int vmx_seg_usable_bad_lim(u64 ar_bytes, u32 limit)
{
	return vmx_seg_usable(ar_bytes) &&
		vmx_seg_bad_limit(ar_bytes, limit);
}

static inline int vmx_seg_resrv(u64 ar_bytes)
{
	return ar_bytes & AR_RESERVD_MASK;
}

static int vmx_check_guest_seg(struct lcd_arch *vcpu)
{
	u64 act64;
	u64 act64b;
	int rslt;

	/*
	 * Disallow virtual 8086 and unrestricted guest for now, to 
	 * minimize number of checks.
	 */
	if ((vmx_getl(vcpu, GUEST_RFLAGS) & X86_EFLAGS_VM) ||
		(vmx_has_sec_exec(vcpu) && 
			vmx_sec_exec_has(vcpu, SECONDARY_EXEC_UNRESTRICTED_GUEST))) {
		printk(KERN_ERR "lcd vmx: guest virtual 8086 mode / unrestricted guest segment checks not implemented\n");
		return -1;
	}

	/*
	 * assume guest is not going to be in virtual 8086 or unrest. guest
	 */

	/*
	 * tr visible
	 */
	if ((vmx_get16(vcpu, GUEST_TR_SELECTOR) & SEGMENT_TI_MASK) != 0) {
		printk(KERN_ERR "lcd vmx: bad visible guest tr (wrong TI)\n");
		return -1;
	}
	
	/*
	 * ldtr visible
	 */
	if (vmx_seg_usable(vmx_getl(vcpu, GUEST_LDTR_AR_BYTES)) &&
		((vmx_get16(vcpu, GUEST_LDTR_SELECTOR) & SEGMENT_TI_MASK) != 0)) {
		printk(KERN_ERR "lcd vmx: bad visible guest ldtr (wrong TI)\n");
		return -1;
	}

	/*
	 * ss rpl == cs rpl
	 *
	 * (Note! Assumes guest will not be in virtual 8086 or unrest guest.)
	 */
	if ((vmx_get16(vcpu, GUEST_SS_SELECTOR) & SEGMENT_RPL_MASK) !=
		(vmx_get16(vcpu, GUEST_CS_SELECTOR) & SEGMENT_RPL_MASK)) {
		printk(KERN_ERR "lcd vmx: guest ss rpl != guest cs rpl\n");
		return -1;
	}

	/*
	 * base addresses
	 *
	 * (Note! Assumes guest will not be in virtual 8086, and host
	 * is in 64-bit mode.)
	 */
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_TR_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest tr base\n");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_FS_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest fs base\n");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_GS_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest gs base\n");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(vcpu, GUEST_LDTR_AR_BYTES)) &&
		!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_LDTR_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest ldtr base\n");
		return -1;
	}
	if (vmx_getl(vcpu, GUEST_CS_BASE) >> 32) {
		printk(KERN_ERR "lcd vmx: bad guest cs base\n");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(vcpu, GUEST_SS_AR_BYTES)) &&
		(vmx_getl(vcpu, GUEST_SS_BASE) >> 32)) {
		printk(KERN_ERR "lcd vmx: bad guest ss base\n");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(vcpu, GUEST_DS_AR_BYTES)) &&
		(vmx_getl(vcpu, GUEST_DS_BASE) >> 32)) {
		printk(KERN_ERR "lcd vmx: bad guest ds base\n");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(vcpu, GUEST_ES_AR_BYTES)) &&
		(vmx_getl(vcpu, GUEST_ES_BASE) >> 32)) {
		printk(KERN_ERR "lcd vmx: bad guest es base\n");
		return -1;
	}

	/*
	 * ar bytes (this is the worst of it ...)
	 *
	 * (Note! Assumes guest not in virtual 8086 / unrest guest.)
	 *
	 * Segment type
	 */
	rslt = vmx_seg_type(vmx_getl(vcpu, GUEST_CS_AR_BYTES));
	if (rslt != 3 && rslt != 9 && rslt != 11 && rslt != 13 && rslt != 15) {
		printk(KERN_ERR "lcd vmx: bad guest cs seg type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) != 3 &&
		vmx_seg_type(act64) != 7) {
		printk(KERN_ERR "lcd vmx: bad guest ss seg type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		printk(KERN_ERR "lcd vmx: bad guest ds seg type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		printk(KERN_ERR "lcd vmx: bad guest es seg type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		printk(KERN_ERR "lcd vmx: bad guest fs seg type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		printk(KERN_ERR "lcd vmx: bad guest gs seg type\n");
		return -1;
	}

	/*
	 * Desc type
	 */
	act64 = vmx_getl(vcpu, GUEST_CS_AR_BYTES);
	if (!vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest cs desc type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest ss desc type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest fs desc type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest gs desc type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest es desc type\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest ds desc type\n");
		return -1;
	}

	/*
	 * cs dpl
	 */
	act64 = vmx_getl(vcpu, GUEST_CS_AR_BYTES);
	act64b = vmx_getl(vcpu, GUEST_SS_AR_BYTES);
	if (vmx_seg_type(act64) == 3) {
		printk(KERN_ERR "lcd vmx: guest cs seg type of 3 not allowed when not using unrest guest\n");
		return -1;
	}
	if ((vmx_seg_type(act64) == 9 || vmx_seg_type(act64) == 11) &&
		(vmx_seg_dpl(act64) != vmx_seg_dpl(act64b))) {
		printk(KERN_ERR "lcd vmx: guest cs dpl != guest ss dpl\n");
		return -1;
	}
	if ((vmx_seg_type(act64) == 13 || vmx_seg_type(act64) == 15) &&
		(vmx_seg_dpl(act64) > vmx_seg_dpl(act64b))) {
		printk(KERN_ERR "lcd vmx: guest cs dpl > guest ss dpl\n");
		return -1;
	}

	/*
	 * ss dpl
	 */
	act64 = vmx_getl(vcpu, GUEST_SS_AR_BYTES);
	act64b = vmx_getl(vcpu, GUEST_CS_AR_BYTES);
	if (vmx_seg_dpl(act64) != 
		(vmx_getl(vcpu, GUEST_SS_SELECTOR) & SEGMENT_RPL_MASK)) {
		printk(KERN_ERR "lcd vmx: guest ss dpl != ss rpl\n");
		return -1;
	}
	if ((vmx_seg_type(act64b) == 3 ||
			vmx_getl(vcpu, GUEST_CR0) & X86_CR0_PE) &&
		vmx_seg_dpl(act64) != 0) {
		printk(KERN_ERR "lcd vmx: guest ss dpl != 0\n");
		return -1;
	}

	/*
	 * ds, es, fs, gs dpl
	 */
	act64 = vmx_getl(vcpu, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(vcpu, GUEST_DS_SELECTOR) & SEGMENT_RPL_MASK)) {
			printk(KERN_ERR "lcd vmx: guest ds dpl < ds rpl\n");
			return -1;
		}
	}
	act64 = vmx_getl(vcpu, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(vcpu, GUEST_ES_SELECTOR) & SEGMENT_RPL_MASK)) {
			printk(KERN_ERR "lcd vmx: guest es dpl < es rpl\n");
			return -1;
		}
	}
	act64 = vmx_getl(vcpu, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(vcpu, GUEST_FS_SELECTOR) & SEGMENT_RPL_MASK)) {
			printk(KERN_ERR "lcd vmx: guest fs dpl < fs rpl\n");
			return -1;
		}
	}
	act64 = vmx_getl(vcpu, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(vcpu, GUEST_GS_SELECTOR) & SEGMENT_RPL_MASK)) {
			printk(KERN_ERR "lcd vmx: guest gs dpl < gs rpl\n");
			return -1;
		}
	}

	/*
	 * present bit
	 */
	if (!vmx_seg_pres(vmx_getl(vcpu, GUEST_CS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest cs not present\n");
		return -1;
	}
	act64 = vmx_getl(vcpu, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest ss not present\n");
		return -1;
	}	
	act64 = vmx_getl(vcpu, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest ds not present\n");
		return -1;
	}	
	act64 = vmx_getl(vcpu, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest es not present\n");
		return -1;
	}	
	act64 = vmx_getl(vcpu, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest fs not present\n");
		return -1;
	}	
	act64 = vmx_getl(vcpu, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest gs not present\n");
		return -1;
	}

	/*
	 * d/b and ia32e long mode bits
	 */
	act64 = vmx_getl(vcpu, GUEST_CS_AR_BYTES);
	if (vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_l_mode(act64) && vmx_seg_db(act64)) {
		printk(KERN_ERR "lcd vmx: guest cs improper db/l-mode bits\n");
		return -1;
	}

	/*
	 * granularity
	 */
	if (vmx_seg_bad_limit(vmx_getl(vcpu, GUEST_CS_AR_BYTES),
				vmx_get32(vcpu, GUEST_CS_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest cs limit\n");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(vcpu, GUEST_SS_AR_BYTES),
					vmx_get32(vcpu, GUEST_SS_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest ss limit\n");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(vcpu, GUEST_DS_AR_BYTES),
					vmx_get32(vcpu, GUEST_DS_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest ds limit\n");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(vcpu, GUEST_ES_AR_BYTES),
					vmx_get32(vcpu, GUEST_ES_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest es limit\n");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(vcpu, GUEST_FS_AR_BYTES),
					vmx_get32(vcpu, GUEST_FS_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest fs limit\n");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(vcpu, GUEST_GS_AR_BYTES),
					vmx_get32(vcpu, GUEST_GS_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest gs limit\n");
		return -1;
	}

	/*
	 * reserved bits
	 */
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_CS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest cs resrv bits set\n");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_SS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest ss resrv bits set\n");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_DS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest ds resrv bits set\n");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_ES_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest es resrv bits set\n");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_FS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest fs resrv bits set\n");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(vcpu, GUEST_GS_AR_BYTES))) {
		printk(KERN_ERR "lcd vmx: guest gs resrv bits set\n");
		return -1;
	}
	

	/*
	 * tss descriptor
	 */
	act64 = vmx_getl(vcpu, GUEST_TR_AR_BYTES);
	if (!vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_type(act64) != 3 && vmx_seg_type(act64) != 11) {
		printk(KERN_ERR "lcd vmx: wrong guest tr type (outside ia32e)\n");
		return -1;
	}
	if (vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_type(act64) != 11) {
		printk(KERN_ERR "lcd vmx: wrong guest tr type (inside ia32e)\n");
		return -1;
	}
	if (vmx_desc_type(act64)) {
		printk(KERN_ERR "lcd vmx: wrong guest tr desc type\n");
		return -1;
	}
	if (!vmx_seg_pres(act64)) {
		printk(KERN_ERR "lcd vmx: guest tr not present\n");
		return -1;
	}
	if (vmx_seg_bad_limit(act64, vmx_get32(vcpu, GUEST_TR_LIMIT))) {
		printk(KERN_ERR "lcd vmx: bad guest tr limit\n");
		return -1;
	}
	if (!vmx_seg_usable(act64)) {
		printk(KERN_ERR "lcd vmx: guest tr marked as unusable\n");
		return -1;
	}
	if (vmx_seg_resrv(act64)) {
		printk(KERN_ERR "lcd vmx: guest tr resrv bits set\n");
		return -1;
	}

	/*
	 * ldtr
	 */
	act64 = vmx_getl(vcpu, GUEST_LDTR_AR_BYTES);
	if (vmx_seg_usable(act64)) {
		if (vmx_seg_type(act64) != 2) {
			printk(KERN_ERR "lcd vmx: bad guest ldtr type\n");
			return -1;
		}
		if (vmx_desc_type(act64)) {
			printk(KERN_ERR "lcd vmx: wrong guest ldtr desc type\n");
			return -1;
		}
		if (!vmx_seg_pres(act64)) {
			printk(KERN_ERR "lcd vmx: guest ldtr not present\n");
			return -1;
		}
		if (vmx_seg_bad_limit(act64, vmx_get32(vcpu, GUEST_LDTR_LIMIT))) {
			printk(KERN_ERR "lcd vmx: bad guest ldtr limit\n");
			return -1;
		}
		if (vmx_seg_resrv(act64)) {
			printk(KERN_ERR "lcd vmx: guest ldtr resrv bits set\n");
			return -1;
		}
	}

	/*
	 * gdtr and idtr
	 */
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_GDTR_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest gdtr base\n");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(vcpu, GUEST_IDTR_BASE))) {
		printk(KERN_ERR "lcd vmx: bad guest idtr base\n");
		return -1;
	}
	if (vmx_getl(vcpu, GUEST_GDTR_LIMIT) >> 15) {
		printk(KERN_ERR "lcd vmx: bad guest gdtr limit\n");
		return -1;
	}
	if (vmx_getl(vcpu, GUEST_IDTR_LIMIT) >> 15) {
		printk(KERN_ERR "lcd vmx: bad guest idtr limit\n");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_rip_rflags(struct lcd_arch *vcpu)
{
	u64 act64;
	u32 act32;
	signed long sact64;
	u32 lin_addr_width;

	/*
	 * rip
	 */
	act64 = vmx_getl(vcpu, GUEST_RIP);
	if (!vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) ||
		!vmx_seg_l_mode(vmx_getl(vcpu, GUEST_CS_AR_BYTES))) {
		if (act64 >> 31) {
			printk(KERN_ERR "lcd vmx: guest rip has bits in 63:32 set\n");
			return -1;
		}
	} else {
		lin_addr_width = (cpuid_eax(0x80000008) >> 8) & 0xff;
		sact64 = (signed long)act64;
		if ((sact64 >> lin_addr_width) != 0 &&
			(sact64 >> lin_addr_width) != -1) {
			printk(KERN_ERR "lcd vmx: guest rip 0x%llx exceeds max linear addr\n", act64);
			return -1;
		}
	}

	/*
	 * rflags
	 */
	act64 = vmx_getl(vcpu, GUEST_RFLAGS);
	if (act64 >> 21 ||
		(act64 & (1 << 15)) ||
		(act64 & (1 << 5)) ||
		(act64 & (1 << 3))) {
		printk(KERN_ERR "lcd vmx: guest rflags has resrv bits set\n");
		return -1;
	}
	if (!(act64 & (1 << 1))) {
		printk(KERN_ERR "lcd vmx: guest rflags bit 1 not set\n");
		return -1;
	}
	if ((vmx_entry_has(vcpu, VM_ENTRY_IA32E_MODE) ||
		!(vmx_getl(vcpu, GUEST_CR0) & X86_CR0_PE)) &&
		(act64 & X86_EFLAGS_VM)) {
		printk(KERN_ERR "lcd vmx: guest rflags vm bit set mistakenly\n");
		return -1;
	}
	act32 = vmx_get32(vcpu, VM_ENTRY_INTR_INFO_FIELD);
	if ((act32 & INTR_INFO_VALID_MASK) &&
		(act32 & INTR_INFO_INTR_TYPE_MASK) &&
		!(act64 & X86_EFLAGS_IF)) {
		printk(KERN_ERR "lcd vmx: valid ext intr info, but rflags.if not set\n");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_non_reg(struct lcd_arch *vcpu)
{
	if (vmx_get32(vcpu, GUEST_ACTIVITY_STATE) !=
		GUEST_ACTIVITY_ACTIVE) {
		printk(KERN_ERR "lcd vmx: checks for non-active guest activity states not implemented\n");
		return -1;
	}

	if (vmx_get32(vcpu, GUEST_INTERRUPTIBILITY_INFO) != 0) {
		printk(KERN_ERR "lcd vmx: checks for non-zero guest intr info not implemented\n");
		return -1;
	}

	if (vmx_entry_has(vcpu, VM_ENTRY_SMM)) {
		printk(KERN_ERR "lcd vmx: guest intr info check for entry to smm not implemented\n");
		return -1;
	}

	if (vmx_get32(vcpu, GUEST_PENDING_DBG_EXCEPTIONS) != 0) {
		printk(KERN_ERR "lcd vmx: nonzero guest pending dbg exc checks not implemented\n");
		return -1;
	}

	if (vmx_getl(vcpu, VMCS_LINK_POINTER) != -1ull) {
		printk(KERN_ERR "lcd vmx: checks when vmcs link pointer != -1 not implemented\n");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_pdpte(struct lcd_arch *vcpu)
{
	
	if (vmx_getl(vcpu, GUEST_CR0) & X86_CR0_PG &&
		vmx_getl(vcpu, GUEST_CR4) & X86_CR4_PAE &&
		!(vmx_getl(vcpu, GUEST_IA32_EFER) & EFER_LMA)) {
		printk(KERN_ERR "lcd vmx: guest pdpte checks for PAE paging not implemented\n");
		return -1;
	}

	return 0;
}

static int vmx_check_guest(struct lcd_arch *vcpu)
{
	if (vmx_check_guest_ctrl_regs(vcpu))
		return -1;
	if (vmx_check_guest_seg(vcpu))
		return -1;
	if (vmx_check_guest_rip_rflags(vcpu))
		return -1;
	if (vmx_check_guest_non_reg(vcpu))
		return -1;
	if (vmx_check_guest_pdpte(vcpu))
		return -1;
	return 0;
}

static int vmx_check_host(struct lcd_arch *vcpu)
{
	if (vmx_check_host_ctrl_regs(vcpu))
		return -1;
	if (vmx_check_host_seg(vcpu))
		return -1;
	return 0;
}

static int vmx_check_ctrls(struct lcd_arch *vcpu)
{
	if (vmx_check_exec_ctrls(vcpu))
		return -1;
	if (vmx_check_exit_ctrls(vcpu))
		return -1;
	if (vmx_check_entry_ctrls(vcpu))
		return -1;
	return 0;
}

static int vmx_check_ctrls_and_host(struct lcd_arch *vcpu)
{
	if (vmx_check_ctrls(vcpu))
		return -1;
	if (vmx_check_host(vcpu))
		return -1;
	if (vmx_check_addr_size(vcpu))
		return -1;
	return 0;
}

int lcd_arch_check(struct lcd_arch *vcpu)
{
	if (vmx_check_ctrls_and_host(vcpu))
		return -1;
	if (vmx_check_guest(vcpu))
		return -1;
	return 0;
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_arch_init);
EXPORT_SYMBOL(lcd_arch_exit);
EXPORT_SYMBOL(lcd_arch_create);
EXPORT_SYMBOL(lcd_arch_destroy);
EXPORT_SYMBOL(lcd_arch_run);
EXPORT_SYMBOL(lcd_arch_ept_walk);
EXPORT_SYMBOL(lcd_arch_ept_set);
EXPORT_SYMBOL(lcd_arch_ept_unset);
EXPORT_SYMBOL(lcd_arch_ept_unmap);
EXPORT_SYMBOL(lcd_arch_ept_unmap_range);
EXPORT_SYMBOL(lcd_arch_ept_hpa);
EXPORT_SYMBOL(lcd_arch_ept_map);
EXPORT_SYMBOL(lcd_arch_ept_map_range);
EXPORT_SYMBOL(lcd_arch_ept_gpa_to_hpa);
EXPORT_SYMBOL(lcd_arch_set_pc);
EXPORT_SYMBOL(lcd_arch_set_gva_root);
EXPORT_SYMBOL(lcd_arch_check);

/* DEBUGGING -------------------------------------------------- */

#include "lcd-domains-arch-tests.c"
