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

/* SHARED / PERCPU VARS -------------------------------------------------- */

static struct vmx_vmcs_config vmcs_config;
static struct vmx_capability vmx_capability;

static atomic_t vmx_enable_failed;
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(struct lcd_arch_vmcs *, vmxon_area);

static struct {
	DECLARE_BITMAP(bitmap, VMX_NR_VPIDS);
	spinlock_t lock;
} vpids;

static DEFINE_PER_CPU(struct lcd_arch *, local_vcpu);

static unsigned long *msr_bitmap;

/* DEBUG --------------------------------------------------*/

/**
 * Prints the vmx controls, lower and upper bounds on the controls,
 * and tries to find the bits that were rejected.
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

	printk(KERN_ERR "  MSR LOW:             0x%08x\n", msr_low);
	printk(KERN_ERR "  ATTEMPTED CONTROLS:  0x%08x\n", controls);
	printk(KERN_ERR "  MSR HIGH:            0x%08x\n", msr_high);
	printk(KERN_ERR "  RESERVED BIT MASK:   0x%08x\n", mask);

	/*
	 * For each bit, if the reserved mask is not set *and* the msr high
	 * bit is not set, then the control bit should not be set.
	 */
	bad_high = ~msr_high & ~mask & controls;
	for (i = 0; i < 32; i++) {
		if (bad_high & 1)
			printk(KERN_ERR "  Control bit %d should be 0.\n",
				i);
		bad_high >>= 1;
	}

	/*
	 * For each bit, if the reserved mask is not set *and* the msr low
	 * bit is set, then the control bit should be set.
	 */
	bad_low = msr_low & ~mask & ~controls;
	for (i = 0; i < 32; i++) {
		if (bad_low & 1)
			printk(KERN_ERR "  Control bit %d should be 1.\n",
				i);
		bad_low >>= 1;
	}

	printk(KERN_ERR "See Intel SDM V3 24.{6,7,8,9} and Appendix A\n");
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

static inline void __invept(int ext, u64 eptp, u64 gpa)
{
	struct {
		u64 eptp, gpa;
	} operand = {eptp, gpa};

	asm volatile (ASM_VMX_INVEPT
                /* CF==1 or ZF==1 --> rc = -1 */
                "; ja 1f ; ud2 ; 1:\n"
                : : "a" (&operand), "c" (ext) : "cc", "memory");
}

/**
 * Invalidates all mappings associated with eptp's.
 */
static inline void invept_global_context(void)
{
	if (cpu_has_vmx_invept_global())
		__invept(VMX_EPT_EXTENT_GLOBAL, 0, 0);
}

/**
 * Invalidates all mappings associated with eptp, and possibly
 * others.
 */
static inline void invept_single_context(u64 eptp)
{
	if (cpu_has_vmx_invept_context())
		__invept(VMX_EPT_EXTENT_CONTEXT, eptp, 0);
	else
		invept_global_context();
}

static inline void __invvpid(int ext, u16 vpid, u64 gva)
{
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

/**
 * Invalidates all mappings associated with vpid's other than
 * vpid = 0 (the host).
 */
static inline void invvpid_global_context(void)
{
	if (cpu_has_vmx_invvpid_global())
		__invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0, 0);
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
		__invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vpid, 0);
	else
		invvpid_global_context();		
}

/* VMCS READ / WRITE --------------------------------------------------*/

/**
 * Takes vmcs from any state to {inactive, clear, not current}
 */
static void vmcs_clear(struct lcd_arch_vmcs *vmcs)
{
	u64 phys_addr = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
	if (error)
		printk(KERN_ERR "lcd vmx: vmclear fail: %p/%llx\n",
			vmcs, phys_addr);
}

/**
 * Takes vmcs to {active, current} on cpu. Any vmcs reads and writes
 * will affect this vmcs.
 */
static void vmcs_load(struct lcd_arch_vmcs *vmcs)
{
	u64 phys_addr = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
	if (error)
		printk(KERN_ERR "lcd vmx: vmptrld %p/%llx failed\n",
			vmcs, phys_addr);
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
	printk(KERN_ERR "lcd vmx: vmwrite error: reg %lx value %lx (err %d)\n",
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

static inline void __vmxon(u64 addr)
{
	asm volatile (ASM_VMX_VMXON_RAX
                : : "a"(&addr), "m"(addr)
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
	u64 phys_addr;
	u64 old;
	u64 test_bits;

	phys_addr = __pa(vmxon_buf);

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
	__vmxon(phys_addr);


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

	printk(KERN_INFO "lcd vmx: VMX enabled on CPU %d\n",
		raw_smp_processor_id());
	return;

failed:
	atomic_inc(&vmx_enable_failed);
	printk(KERN_ERR "lcd vmx: failed to enable VMX, err = %d\n", ret);
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
		printk(KERN_ERR "lcd vmx: pin based exec controls not allowed\n");
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
		printk(KERN_ERR "lcd vmx: primary proc based exec ctrls not allowed\n");
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
		printk(KERN_ERR "lcd vmx: secondary proc based exec ctls not allowed\n");
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
		printk(KERN_ERR "lcd vmx: vmexit controls not allowed\n");
		
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
		printk(KERN_ERR "lcd vmx: vm entry controls not allowed\n");
		
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
		printk(KERN_ERR "lcd vmx: CPU does not support VMX\n");
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
		printk(KERN_ERR "lcd vmx: timeout waiting for VMX mode enable.\n");
		ret = -EIO;
		goto failed1; /* sadly we can't totally recover */
	}

	if (atomic_read(&vmx_enable_failed)) {
		ret = -EBUSY;
		goto failed2;
	}

	/*
	 * Run tests
	 */
	lcd_arch_tests();

	return 0;

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
}

/* VMX EPT -------------------------------------------------- */

/**
 * PAGE_SHIFT is assumed to be 12.
 */
#define VMX_EPTE_ADDR_MASK PAGE_MASK
#define VMX_EPT_ALL_MASK (VMX_EPT_READABLE_MASK | \
                          VMX_EPT_WRITABLE_MASK | \
			  VMX_EPT_EXECUTABLE_MASK)
static inline u64 vmx_epte_hpa(lcd_arch_epte_t epte)
{
	return ((u64)epte) & VMX_EPTE_ADDR_MASK;
}
static inline u64 vmx_epte_hva(lcd_arch_epte_t epte)
{
	return (u64)__va(vmx_epte_hpa(epte));
}
static inline lcd_arch_epte_t * vmx_epte_dir_hva(lcd_arch_epte_t epte)
{
	return (lcd_arch_epte_t *)vmx_epte_hva(epte);
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
static inline int vmx_ept_idx(u64 gpa, int lvl)
{
	return (int)(((gpa) >> (12 + 9 * (3 - lvl))) & ((1 << 9) - 1));
}
static inline u64 vmx_ept_offset(u64 gpa)
{
	return gpa & ~(PAGE_MASK);
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
static void vmx_epte_set(lcd_arch_epte_t *epte, u64 hpa, int level)
{
	/*
	 * zero out epte, and set
	 */
	*epte = 0;
	*epte = (hpa & VMX_EPTE_ADDR_MASK) | VMX_EPT_ALL_MASK;
	if (level == 3) {
		/*
		 * Page table entry. Set EPT memory type to write back
		 * and ignore page attribute table.
		 */
		*epte |= (VMX_EPT_IPAT_BIT |
			(VMX_EPTE_MT_WB << VMX_EPT_MT_EPTE_SHIFT));
	}
}

int lcd_arch_ept_walk(struct lcd_arch *vcpu, u64 gpa, int create,
		lcd_arch_epte_t **epte_out)
{
	int i;
	lcd_arch_epte_t *dir;
	u64 idx;
	u64 page;

	dir = (lcd_arch_epte_t *) __va(vcpu->ept.root_hpa);

	/*
	 * Walk plm4 -> pdpt -> pd. Each step uses 9 bits
	 * of the gpa.
	 */
	for (i = 0; i < LCD_ARCH_EPT_WALK_LENGTH - 1; i++) {

		idx = vmx_ept_idx(gpa, i);

		if (!vmx_epte_present(dir[idx])) {
			
			if (!create) {
				printk(KERN_ERR "lcd_arch_ept_walk: attempted lookup for unmapped gpa %lx, create was not allowed\n",
					(unsigned long)gpa);
				return -ENOENT;
			}
			/*
			 * Get host virtual addr of fresh page, and
			 * set the epte's addr to the host physical addr
			 */
			page = __get_free_page(GFP_KERNEL);
			if (!page) {
				printk(KERN_ERR "lcd_arch_ept_walk: alloc failed\n");
				return -ENOMEM;
			}
			memset((void *)page, 0, PAGE_SIZE);
			vmx_epte_set(&dir[idx], __pa(page), i);
		}

		dir = (lcd_arch_epte_t *) vmx_epte_hva(dir[idx]);
	}

	/*
	 * dir points to page table (level 3)
	 */
	*epte_out = &dir[vmx_ept_idx(gpa, 3)];
	return 0;
}

void lcd_arch_ept_set(lcd_arch_epte_t *epte, u64 hpa)
{
	vmx_epte_set(epte, hpa, 3);
}

u64 lcd_arch_ept_hpa(lcd_arch_epte_t *epte)
{
	return vmx_epte_hpa(*epte);
}

int lcd_arch_ept_map_gpa_to_hpa(struct lcd_arch *vcpu, u64 gpa, u64 hpa,
				int create, int overwrite)
{
	int ret;
	lcd_arch_epte_t *ept_entry;

	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(vcpu, gpa, create, &ept_entry);
	if (ret)
		return ret;

	/*
	 * Check if guest physical address already mapped
	 */
	if (!overwrite && vmx_epte_present(*ept_entry)) {
		printk(KERN_ERR "lcd_arch_map_gpa_to_hpa: would overwrite hpa %lx with hpa %lx\n",
			(unsigned long)lcd_arch_ept_hpa(ept_entry), 
			(unsigned long)hpa);
		return -EINVAL;
	}

	/*
	 * Map the guest physical addr to the host physical addr.
	 */
	lcd_arch_ept_set(ept_entry, hpa);

	return 0;
}

int lcd_arch_ept_gpa_to_hpa(struct lcd_arch *vcpu, u64 gpa, u64 *hpa_out)
{
	int ret;
	lcd_arch_epte_t *ept_entry;

	/*
	 * Walk ept
	 */
	ret = lcd_arch_ept_walk(vcpu, gpa, 0, &ept_entry);
	if (ret)
		return ret;

	/*
	 * Confirm the entry is present
	 */
	if (!vmx_epte_present(*ept_entry)) {
		printk(KERN_ERR "lcd_arch_ept_gpa_to_hpa: gpa %lx is not mapped\n",
			(unsigned long)gpa);
		return -EINVAL;
	}	

	/*
	 * Get the base host physical address, and add the offset.
	 */
	*hpa_out = lcd_arch_ept_hpa(ept_entry);
	*hpa_out += vmx_ept_offset(gpa);

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
		 * Free present pages in page table
		 */
		for (idx = 0; idx < LCD_ARCH_PTRS_PER_EPTE; idx++) {
			if (vmx_epte_present(dir[idx]))
				free_page(vmx_epte_hva(dir[idx]));
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
	free_page((u64)dir);
}

/**
 * Frees all memory associated with ept (ept paging
 * structures and mapped physical mem).
 */
static void vmx_free_ept(struct lcd_arch *vcpu)
{
	lcd_arch_epte_t *dir;
	/*
	 * Get pml4 table
	 */
	dir = (lcd_arch_epte_t *) __va(vcpu->ept.root_hpa);
	vmx_free_ept_dir_level(dir, 0);
}

/**
 * Initializes the EPT's root global page directory page, the
 * VMCS pointer, and the spinlock.
 */
int vmx_init_ept(struct lcd_arch *vcpu)
{
	void *page;
	u64 eptp;

	/*
	 * Alloc the root global page directory page
	 */

	page = (void *)__get_free_page(GFP_KERNEL);
	if (!page) {
		printk(KERN_ERR "vmx init ept: failed to alloc page\n");
		return -ENOMEM;
	}
	memset(page, 0, PAGE_SIZE);

	vcpu->ept.root_hpa =  __pa(page);

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
		vcpu->ept.access_dirty_enabled = true;
		eptp |= VMX_EPT_AD_ENABLE_BIT;
	}
	eptp |= (vcpu->ept.root_hpa & PAGE_MASK);
	vcpu->ept.vmcs_ptr = eptp;

	/*
	 * Init the spinlock
	 */
	spin_lock_init(&vcpu->ept.lock);

	return 0;
}

/* HOST INFO -------------------------------------------------- */

/**
 * Returns pointer to current gdt (array of segment descriptors) on 
 * calling cpu.
 */
static struct desc_struct * vmx_per_cpu_gdt(void)
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
static gate_desc * vmx_per_cpu_idt(void)
{
	struct desc_ptr idt_ptr;
	native_store_idt(&idt_ptr);
	return (gate_desc *)idt_ptr.address;
}

/**
 * Returns pointer to tss segment descriptor in cpu's gdt.
 */
static struct desc_struct * vmx_per_cpu_tss_desc(void)
{
	struct desc_struct *gdt;
	u16 tr;
	gdt = vmx_per_cpu_gdt();
	store_tr(tr);
	return &gdt[tr];
}	

/* VMCS INITIALIZATION -------------------------------------------------- */

/**
 * Stores expected host state in VMCS.
 */
static void vmx_setup_vmcs_host(struct lcd_arch *vcpu)
{
	unsigned long tmpl;

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
	 * The visible part (selector part) of %fs and %gs
	 * is ignored, so we set it to 0.
	 *
	 * See x86/include/asm/segment.h and
	 * Intel SDM V3 3.4.4.
	 */
	vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);
	vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);
	vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);
	vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);
	vmcs_write16(HOST_FS_SELECTOR, 0);
	vmcs_write16(HOST_GS_SELECTOR, 0);
	vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);

	/*
	 * Host %fs and %gs bases
	 *
	 * Intel SDM V3 3.4.4
	 */
	rdmsrl(MSR_FS_BASE, tmpl);
	vmcs_writel(HOST_FS_BASE, tmpl);
	rdmsrl(MSR_GS_BASE, tmpl);
	vmcs_writel(HOST_GS_BASE, tmpl);

	/*
	 * Host MSR EFER
	 *
	 * (will be loaded on vm exit)
	 */
	rdmsrl(MSR_EFER, tmpl);
	vmcs_writel(HOST_IA32_EFER, tmpl);

	/* asm("mov $.Llcd_arch_return, %0" : "=r"(tmpl)); */
	/* vmcs_writel(HOST_RIP, tmpl); /\* 22.2.5 *\/ */
}

/**
 * Sets up MSR autloading for MSRs listed in lcd_arch_autoload_msrs.
 */
static void vmx_setup_vmcs_msr(struct lcd_arch *vcpu)
{
	int i;
	u64 val;
	struct vmx_msr_entry *e;
	int autoload_msrs[] = {
		/* None for now */
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

	vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.host));
	vmcs_write64(VM_EXIT_MSR_STORE_ADDR, __pa(vcpu->msr_autoload.guest));
	vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(vcpu->msr_autoload.guest));

	for (i = 0; i < LCD_ARCH_NUM_AUTOLOAD_MSRS; i++) {
	
		e = &vcpu->msr_autoload.host[i];
		e->index = autoload_msrs[i];
		rdmsrl(e->index, val);
		e->value = val;

		e = &vcpu->msr_autoload.guest[i];
		e->index = autoload_msrs[i];
	}
}

/**
 * Sets up initial guest register values in VMCS.
 */
static void vmx_setup_vmcs_guest_regs(struct lcd_arch *vcpu)
{
	unsigned long cr0;
	unsigned long cr4;

	/*
	 * %cr0 (and its shadow)
	 *
	 * -- (PG) paging
	 * -- (PE) protected mode
	 * -- (EM) emulation (tells guest there is no fpu)
	 * -- (ET) extension type (esoteric math coproc bit)
	 *
	 * Intel SDM V3 2.5
	 */
	cr0 = X86_CR0_PG | X86_CR0_PE | X86_CR0_EM | X86_CR0_ET;
	vmcs_writel(GUEST_CR0, cr0);
	vmcs_writel(CR0_READ_SHADOW, cr0);

	/*
	 * %cr4 (and its shadow)
	 *
	 * -- (PAE) physical address extension
	 * -- (PGE) page global enable
	 * -- (PCIDE) process context identifiers, if available
	 * -- (FSGSBASE) allow RDFSBASE, WRFSBASE, etc., if available
	 *
	 * Nothing for floating point, since it is turned off.
	 *
	 * Intel SDM V3 2.5
	 */
	cr4 = X86_CR4_PAE | X86_CR4_PGE;
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
	 * %rip, %rsp -- to be set when guest address space set up
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
	 * Bases
	 */
	vmcs_writel(GUEST_CS_BASE, 0);
	vmcs_writel(GUEST_DS_BASE, 0);
	vmcs_writel(GUEST_ES_BASE, 0);
	vmcs_writel(GUEST_SS_BASE, 0);
	vmcs_writel(GUEST_FS_BASE, LCD_ARCH_FS_BASE);
	vmcs_writel(GUEST_GS_BASE, LCD_ARCH_GS_BASE);
 	vmcs_writel(GUEST_GDTR_BASE, LCD_ARCH_GDTR_BASE);
 	vmcs_writel(GUEST_IDTR_BASE, LCD_ARCH_IDTR_BASE);
 	vmcs_writel(GUEST_TR_BASE, LCD_ARCH_TSS_BASE);

	/*
	 * Access rights
	 *
	 * -- %cs:
	 *    -- code segment type, execute/read/accessed
	 *    -- code or data segment desc type
	 *    -- dpl = 0
	 *    -- present
	 *    -- 64-bit mode
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
 	vmcs_writel(GUEST_TR_AR_BYTES,   0x0083);

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
	 * -- In IA-32e mode, %ds, %es, and %ss are ignored
	 *
	 * Intel SDM V3 3.4.4
	 */
	vmcs_write16(GUEST_CS_SELECTOR, LCD_ARCH_CS_SELECTOR); /* code */
	vmcs_write16(GUEST_DS_SELECTOR, 0); /* ignored */
	vmcs_write16(GUEST_ES_SELECTOR, 0); /* ignored */
	vmcs_write16(GUEST_FS_SELECTOR, LCD_ARCH_FS_SELECTOR); /* data */ 
	vmcs_write16(GUEST_GS_SELECTOR, LCD_ARCH_GS_SELECTOR); /* data */
	vmcs_write16(GUEST_SS_SELECTOR, 0); /* ignored */
	vmcs_write16(GUEST_TR_SELECTOR, LCD_ARCH_TR_SELECTOR);

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
 */
static void vmx_setup_vmcs_guest_settings(struct lcd_arch *vcpu)
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
	 */
	vmcs_write64(EPT_POINTER, vcpu->ept.vmcs_ptr);
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
static void vmx_setup_vmcs(struct lcd_arch *vcpu)
{
	/*
	 * Set up guest part of vmcs, and guest exec
	 */
	vmx_setup_vmcs_guest_settings(vcpu);
	vmx_setup_vmcs_guest_regs(vcpu);
	/*
	 * Set up MSR bitmap and autoloading
	 */
	vmx_setup_vmcs_msr(vcpu);
	/*
	 * Set up host part of vmcs
	 */
	vmx_setup_vmcs_host(vcpu);
}


/* VMCS LOADING -------------------------------------------------- */

/**
 * Updates an lcd's VMCS when the lcd is moved to a different
 * cpu. (Linux uses per-cpu data that needs to be updated in
 * the lcd's VMCS.)
 */
static void __vmx_setup_cpu(struct lcd_arch *vcpu, int cur_cpu)
{
	struct desc_struct *gdt;
	struct desc_struct *tss_desc;
	unsigned long tmpl;

	/*
	 * Linux uses per-cpu TSS and GDT, so we need to set these
	 * in the host part of the vmcs when switching cpu's.
	 */
	gdt = vmx_per_cpu_gdt();
	tss_desc = vmx_per_cpu_tss_desc();
	vmcs_writel(HOST_TR_BASE, get_desc_base(tss_desc));
	vmcs_writel(HOST_GDTR_BASE, (unsigned long)gdt);

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
	struct lcd_arch *vcpu;
	vcpu = ptr;
	BUG_ON(raw_smp_processor_id() != vcpu->cpu);
	vmcs_clear(vcpu->vmcs);
	if (__get_cpu_var(local_vcpu) == vcpu)
		__get_cpu_var(local_vcpu) = NULL;
}

/**
 * Loads VCPU on the calling cpu.
 *
 * Disables preemption. Call vmx_put_cpu() when finished.
 */
static void vmx_get_cpu(struct lcd_arch *vcpu)
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
	 * Otherwise, we need to make the vcpu active
	 * and current on this cpu.
	 */
	if (__get_cpu_var(local_vcpu) != vcpu) {

		__get_cpu_var(local_vcpu) = vcpu;

		if (vcpu->cpu != cur_cpu) {

			/*
			 * vcpu not active on this cpu
			 */
			if (vcpu->cpu >= 0)
				/*
				 * vcpu active on a different cpu;
				 * clear it there (active -> inactive)
				 */
				smp_call_function_single(vcpu->cpu,
							__vmx_get_cpu_helper, 
							(void *) vcpu, 1);
			else
				/*
				 * vcpu inactive; clear it to get to
				 * initial vmcs state
				 *
				 * Intel SDM V3 24.11.3
				 */
				vmcs_clear(vcpu->vmcs);

			/*
			 * Invalidate any vpid or ept cache lines
			 */
			invvpid_single_context(vcpu->vpid);
			invept_single_context(vcpu->ept.vmcs_ptr);

			/*
			 * vcpu is not in launched state
			 */
			vcpu->launched = 0;

			/*
			 * Load vmcs pointer on this cpu
			 */
			vmcs_load(vcpu->vmcs);

			/*
			 * Update cpu-specific data in vmcs
			 */
			__vmx_setup_cpu(vcpu, cur_cpu);

			/*
			 * Remember which cpu we are active on
			 */
			vcpu->cpu = cur_cpu;
		} else {
			vmcs_load(vcpu->vmcs);
		}
	}
}

/**
 * Match with vmx_get_cpu.
 *
 * Enables preemption.
 */
static void vmx_put_cpu(struct lcd_arch *vcpu)
{
	put_cpu();
}


/* VMX CREATE / DESTROY -------------------------------------------------- */

/**
 * Pack base, limit, and flags into a segment descriptor.
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
 * Allocates gdt and populates descriptor entries using layout
 * in lcd-domains-arch.h.
 *
 * Maps GDT in guest physical address space.
 *
 * FIXME: Is a gdt necessary if an lcd never touches it? The base,
 * limit, etc. are loaded in the hidden fields of the segment registers.
 */
static int vmx_init_gdt(struct lcd_arch *vcpu)
{
	struct desc_struct *desc;
	struct tss_desc *tssd;
	int ret;

	/*
	 * Alloc zero'd page for gdt
	 */
	vcpu->gdt = (struct desc_struct *)get_zeroed_page(GFP_KERNEL);
	if (!vcpu->gdt) {
		printk(KERN_ERR "vmx init gdt: failed to alloc gdt\n");
		ret = -ENOMEM;
		goto fail;
	}

	/*
	 *===--- Populate gdt; see layout in lcd-domains-arch.h. ---===
	 */

	/*
	 * Code Segment
	 */
	desc = vcpu->gdt + (LCD_ARCH_CS_SELECTOR >> 3); /* div by 8 */
	vmx_pack_desc(desc,
		0,        /* base */
		0xFFFFF,  /* limit (granularity = 1) */
		0xB,      /* code seg type, exec/read/accessed */
		0x1,      /* code/data segment desc type */
		0x0,      /* dpl = 0 */
		0x1,      /* present */
		0x0,      /* avl (not used) */
		0x1,      /* 64-bit code */
		0x0,      /* d must be cleared for 64-bit code */
		0x1);     /* 4KB granularity */

	/*
	 * Data Segment (for %fs)
	 */
	desc = vcpu->gdt + (LCD_ARCH_FS_SELECTOR >> 3); /* div by 8 */
	vmx_pack_desc(desc,
		0,        /* base */
		0xFFFFF,  /* limit (granularity = 1) */
		0x3,      /* data seg type, exec/read/accessed */
		0x1,      /* code/data segment desc type */
		0x0,      /* dpl = 0 */
		0x1,      /* present */
		0x0,      /* avl (not used) */
		0x0,      /* l (not 64-bit code) */
		0x1,      /* d (linux uses 1 for d ...) */
		0x1);     /* 4KB granularity */

	/*
	 * Data Segment (for %gs)
	 */
	desc = vcpu->gdt + (LCD_ARCH_GS_SELECTOR >> 3); /* div by 8 */
	vmx_pack_desc(desc,
		0,        /* base */
		0xFFFFF,  /* limit (granularity = 1) */
		0x3,      /* data seg type, exec/read/accessed */
		0x1,      /* code/data segment desc type */
		0x0,      /* dpl = 0 */
		0x1,      /* present */
		0x0,      /* avl (not used) */
		0x0,      /* l (not 64-bit code) */
		0x1,      /* d (linux uses 1 for d ...) */
		0x1);     /* 4KB granularity */

	/*
	 * Task Segment (descriptor)
	 */
	tssd = (struct tss_desc *)(vcpu->gdt + (LCD_ARCH_TR_SELECTOR >> 3));
	set_tssldt_descriptor(tssd, 
			LCD_ARCH_TSS_BASE,   /* base */
			0xB,                 /* type = 64-bit busy tss */
			LCD_ARCH_TSS_LIMIT); /* limit */

	/*
	 *===--- Map GDT in guest physical address space ---===
	 */
	ret = lcd_arch_ept_map_gpa_to_hpa(vcpu, 
					/* gpa */
					LCD_ARCH_GDTR_BASE, 
					/* hpa */
					__pa(vcpu->gdt),
					/* create paging structs as needed */
					1,
					/* no overwrite */
					0);
	if (ret) {
		printk(KERN_ERR "vmx_gdt_init: failed to map gdt\n");
		goto fail_map;
	}

	return 0;

fail_map:
	free_page((u64)vcpu->gdt);
fail:
	return ret;
}

/**
 * Allocates tss and sets minimal number of fields needed.
 *
 * Maps TSS in guest physical address space.
 *
 * FIXME: Is a TSS necessary if stack switching / interrupts are
 * not handled in the lcd?
 */
static int vmx_init_tss(struct lcd_arch *vcpu)
{
	struct x86_hw_tss *base_tss;
	int ret;

	/*
	 * Alloc zero'd page for tss.
	 *
	 * Only the first part of the page will be filled by the tss. This is
	 * done for now to make the address space layout simpler, but
	 * could perhaps be improved later.
	 */
	vcpu->tss = (struct lcd_arch_tss *)get_zeroed_page(GFP_KERNEL);
	if (!vcpu->tss) {
		printk(KERN_ERR "vmx_init_tss: failed to alloc tss\n");
		ret = -ENOMEM;
		goto fail;
	}
	base_tss = &(vcpu->tss->base_tss);
	/*
	 * Set up 64-bit TSS (See Intel SDM V3 7.7)
	 *
	 * No interrupt stack tables are used (since the lcd won't be
	 * handling interrupts anyway).
	 *
	 * Privilege Level 0 Stack
	 */
	base_tss->sp0 = LCD_ARCH_STACK_TOP;
	/*
	 * The TSS must have a minimal I/O bitmap with one byte of 1's
	 *
	 * Intel SDM V1 16.5.2
	 */
	base_tss->io_bitmap_base = offsetof(struct lcd_arch_tss, io_bitmap);
	vcpu->tss->io_bitmap[0] = 0xff;

	/*
	 *===--- Map TSS in guest physical address space ---===
	 */
	ret = lcd_arch_ept_map_gpa_to_hpa(vcpu, 
					/* gpa */
					LCD_ARCH_TSS_BASE, 
					/* hpa */
					__pa(vcpu->tss),
					/* create paging structs as needed */
					1,
					/* no overwrite */
					0);
	if (ret) {
		printk(KERN_ERR "vmx_init_tss: failed to map tss\n");
		goto fail_map;
	}

	return 0;

fail_map:
	free_page((u64)vcpu->tss);
fail:
	return ret;
}

/**
 * Allocates and maps stack / utcb.
 *
 * %rsp not set until guest virtual address space initialized.
 */
static int vmx_init_stack(struct lcd_arch *vcpu)
{
	int ret;

	/*
	 * Alloc zero'd page for stack.
	 *
	 * Bottom of stack will contain utcb.
	 */
	vcpu->utcb = (struct lcd_utcb *)get_zeroed_page(GFP_KERNEL);
	if (!vcpu->utcb) {
		printk(KERN_ERR "vmx_init_stack: failed to alloc stack\n");
		ret = -ENOMEM;
		goto fail;
	}

	/*
	 *===--- Map stack in guest physical address space ---===
	 */
	ret = lcd_arch_ept_map_gpa_to_hpa(vcpu, 
					/* gpa */
					LCD_ARCH_UTCB,
					/* hpa */
					__pa(vcpu->utcb),
					/* create paging structs as needed */
					1,
					/* no overwrite */
					0);
	if (ret) {
		printk(KERN_ERR "vmx_init_stack: failed to map stack\n");
		goto fail_map;
	}

	return 0;

fail_map:
	free_page((u64)vcpu->utcb);
fail:
	return ret;
}

/**
 * Reserves a vpid and sets it in the vcpu.
 */
static int vmx_allocate_vpid(struct lcd_arch *vmx)
{
	int vpid;

	vmx->vpid = 0;

	spin_lock(&vpids.lock);
	vpid = find_first_zero_bit(vpids.bitmap, VMX_NR_VPIDS);
	if (vpid < VMX_NR_VPIDS) {
		vmx->vpid = vpid;
		__set_bit(vpid, vpids.bitmap);
	}
	spin_unlock(&vpids.lock);

	return vpid >= VMX_NR_VPIDS;
}

/**
 * Frees a vpid.
 */
static void vmx_free_vpid(struct lcd_arch *vmx)
{
	spin_lock(&vpids.lock);
	if (vmx->vpid != 0)
		__clear_bit(vmx->vpid, vpids.bitmap);
	spin_unlock(&vpids.lock);
}

struct lcd_arch* lcd_arch_create(void)
{
	struct lcd_arch* vcpu;

	/*
	 * Alloc lcd_arch
	 */
	vcpu = kmalloc(sizeof(*vcpu), GFP_KERNEL);
	if (!vcpu) {
		printk(KERN_ERR "lcd arch create: failed to alloc lcd\n");
		goto fail_vcpu;
	}
	memset(vcpu, 0, sizeof(*vcpu));

	/*
	 * Alloc vmcs
	 */
	vcpu->vmcs = vmx_alloc_vmcs(raw_smp_processor_id());
	if (!vcpu->vmcs) {
		printk(KERN_ERR "lcd arch create: failed to alloc vmcs\n");
		goto fail_vmcs;
	}

	if (vmx_allocate_vpid(vcpu)) {
		printk(KERN_ERR "lcd arch create: failed to alloc vpid\n");
		goto fail_vpid;
	}

	/*
	 * Not loaded on a cpu right now
	 */
	vcpu->cpu = -1;

	/*
	 * Initialize EPT, GDT, and TSS.
	 *
	 * The EPT must be initialized before GDT, TSS, and stack,
	 * so that they can be mapped in guest physical.
	 */
	if (vmx_init_ept(vcpu)) {
		printk(KERN_ERR "lcd_arch_create: failed to init ept\n");
		goto fail_ept;
	}
	if (vmx_init_gdt(vcpu)) {
		printk(KERN_ERR "lcd_arch_create: failed to init gdt\n");
		goto fail_gdt;
	}
	if (vmx_init_tss(vcpu)) {
		printk(KERN_ERR "lcd_arch_create: failed to init tss\n");
		goto fail_tss;
	}

	/*
	 * Initialize stack / utcb
	 */
	if (vmx_init_stack(vcpu)) {
		printk(KERN_ERR "lcd_arch_create: failed to init stack\n");
		goto fail_stack;
	}

	/*
	 * Initialize VMCS register values and settings
	 * 
	 * Preemption disabled while doing so ...
	 */
	vmx_get_cpu(vcpu);
	vmx_setup_vmcs(vcpu);
	vmx_put_cpu(vcpu);

	return vcpu;

fail_stack:
fail_tss:
fail_gdt:
	/*
	 * free ept will free gdt, tss, and stack, since they
	 * are mapped in ept
	 */
	vmx_free_ept(vcpu);
fail_ept:
	vmx_free_vpid(vcpu);
fail_vpid:
	vmx_free_vmcs(vcpu->vmcs);
fail_vmcs:
	kfree(vcpu);
fail_vcpu:
	return NULL;
}

void lcd_arch_destroy(struct lcd_arch *vcpu)
{
	/*
	 * Premption Disabled
	 *
	 * The call to vmx_get_cpu is done because if vcpu is
	 * active on a different cpu, it needs to be
	 * vmclear'd there (and vmx_get_cpu will do
	 * that, as a side effect).
	 *
	 * There might be alternative ways, but this works ...
	 */
	vmx_get_cpu(vcpu);
	/*
	 * Invalidate any cached ept and vpid mappings.
	 */
	invept_single_context(vcpu->ept.vmcs_ptr);
	invvpid_single_context(vcpu->vpid);
	/*
	 * VM clear on this cpu
	 */
	vmcs_clear(vcpu->vmcs);
	__get_cpu_var(local_vcpu) = NULL;
	/*
	 * Preemption enabled
	 */
	vmx_put_cpu(vcpu);
	/*
	 * Free remaining junk
	 */
	vmx_free_vpid(vcpu);
	vmx_free_vmcs(vcpu->vmcs);
	vmx_free_ept(vcpu); /* auto frees gdt, tss, utcb */
	kfree(vcpu);
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
	idt = vmx_per_cpu_idt();
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
 * exceptions, etc.
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
		vcpu->run_info.gva = vcpu->exit_qualification;
		return LCD_ARCH_STATUS_PAGE_FAULT;
	default:
		printk(KERN_ERR "lcd vmx: unhandled hw exception:\n");
		printk(KERN_ERR "         vector: %x, info: %x\n",
			vector, vcpu->exit_intr_info);
		return -EIO;
	}
}

/**
 * Processes software / hardware exceptions and nmi's generated
 * while lcd was running.
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
		printk(KERN_ERR "lcd vmx: unhandled exception or nmi:\n");
		printk(KERN_ERR "         interrupt info: %x\n",
			vcpu->exit_intr_info);
		return -EIO;
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
	vcpu->run_info.gva = vmcs_readl(GUEST_LINEAR_ADDRESS);
	vcpu->run_info.gpa = vmcs_readl(GUEST_PHYSICAL_ADDRESS);
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
		"push %%rcx \n\t" 
		/*
		 * Remember the current %rsp
		 * (je 1f jumps forward to numeric label 1)
		 */
		"cmp %%rsp, %c[host_rsp](%0) \n\t"
		"je 1f \n\t"
		"mov %%rsp, %c[host_rsp](%0) \n\t"
		ASM_VMX_VMWRITE_RSP_RDX "\n\t"
		"1: \n\t"
		/*
		 * Re-load %cr2 if changed
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
		"jne .Llaunched \n\t"
		ASM_VMX_VMLAUNCH "\n\t"
		"jmp .Llcd_arch_return \n\t"
		".Llaunched: " ASM_VMX_VMRESUME "\n\t"
		".Llcd_arch_return: "

		/*
		 * Save guest registers, load host registers, keep flags
		 */

		/*
		 * Store guest %rcx at rsp + wordsize (placeholder)
		 */
		"mov %0, %c[wordsize](%%rsp) \n\t"
		/*
		 * Pop %0 (host %rcx is at bottom of stack)
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

		: : "c"(vcpu),
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
		  , "rax", "rbx", "rdi", "rsi"
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

int lcd_arch_set_pc(struct lcd_arch *vcpu, u64 gpa)
{
	vcpu->regs[LCD_ARCH_REGS_RIP] = gpa;
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
EXPORT_SYMBOL(lcd_arch_ept_hpa);
EXPORT_SYMBOL(lcd_arch_ept_map_gpa_to_hpa);
EXPORT_SYMBOL(lcd_arch_ept_gpa_to_hpa);
EXPORT_SYMBOL(lcd_arch_set_pc);

/* DEBUGGING -------------------------------------------------- */

#include "lcd-domains-arch-tests.c"
