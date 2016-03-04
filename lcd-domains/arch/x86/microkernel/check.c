/*
 * check.c
 *
 * This implements the checks stipulated in the Intel manual.
 * 
 * The exit status information is not very informative if
 * you misconfigure the VMCS, so it can be hard to debug. So,
 * this does the checks for you.
 *
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <asm/vmx.h>

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/microkernel.h>


static inline u16 vmx_get16(struct lcd_arch *t, u64 field)
{
	u16 out;
	vmx_get_cpu(t);
	out = vmcs_read16(field);
	vmx_put_cpu(t);
	return out;
}

static inline u32 vmx_get32(struct lcd_arch *t, u64 field)
{
	u32 out;
	vmx_get_cpu(t);
	out = vmcs_read32(field);
	vmx_put_cpu(t);
	return out;
}

static inline u64 vmx_getl(struct lcd_arch *t, u64 field)
{
	u64 out;
	vmx_get_cpu(t);
	out = vmcs_readl(field);
	vmx_put_cpu(t);
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

static inline u32 vmx_get_pin(struct lcd_arch *t)
{
	return vmx_get32(t, PIN_BASED_VM_EXEC_CONTROL);
}

static inline int vmx_pin_has(struct lcd_arch *t, u32 field)
{
	return (vmx_get_pin(t) & field) != 0;
} 

static inline u32 vmx_get_prim_exec(struct lcd_arch *t)
{
	return vmx_get32(t, CPU_BASED_VM_EXEC_CONTROL);
}

static inline int vmx_prim_exec_has(struct lcd_arch *t, u32 field)
{
	return (vmx_get_prim_exec(t) & field) != 0;
} 

static inline int vmx_has_sec_exec(struct lcd_arch *t)
{
	return (vmx_get_prim_exec(t) & 
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) != 0;
}

static inline u32 vmx_get_sec_exec(struct lcd_arch *t)
{
	if (!vmx_has_sec_exec(t)) {
		printk(KERN_ERR "lcd vmx get sec exec: bad check, something wrong; t doesn't have secondary exec ctrls enabled");
	}
	BUG_ON(!vmx_has_sec_exec(t));
	return vmx_get32(t, SECONDARY_VM_EXEC_CONTROL);
}

static inline int vmx_sec_exec_has(struct lcd_arch *t, u32 field)
{
	if (!vmx_has_sec_exec(t)) {
		printk(KERN_ERR "lcd vmx get sec exec: bad check, something wrong; t doesn't have secondary exec ctrls enabled");
	}
	BUG_ON(!vmx_has_sec_exec(t));
	return (vmx_get_sec_exec(t) & field) != 0;
} 

static inline u32 vmx_get_exit(struct lcd_arch *t)
{
	return vmx_get32(t, VM_EXIT_CONTROLS);
}

static inline int vmx_exit_has(struct lcd_arch *t, u32 field)
{
	return (vmx_get_exit(t) & field) != 0;
} 

static inline u32 vmx_get_entry(struct lcd_arch *t)
{
	return vmx_get32(t, VM_ENTRY_CONTROLS);
}

static inline int vmx_entry_has(struct lcd_arch *t, u32 field)
{
	return (vmx_get_entry(t) & field) != 0;
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

static int vmx_check_exec_ctrls(struct lcd_arch *t)
{
	u32 low32;
	u32 high32;
	u32 act32;
	u64 act64;

	/*
	 * Pin based controls
	 */
	rdmsr(MSR_IA32_VMX_PINBASED_CTLS, low32, high32);
	act32 = vmx_get_pin(t);
	if (low32 & ~act32) {
		LCD_ERR("min pin based ctrls not set");
		return -1;
	}
	if (~high32 & act32) {
		LCD_ERR("above max pin based ctrls");
		return -1;
	}

	/*
	 * Primary exec controls
	 */
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS, low32, high32);
	act32 = vmx_get_prim_exec(t);
	if (low32 & ~act32) {
		LCD_ERR("min prim exec ctrls not set");
		return -1;
	}
	if (~high32 & act32) {
		LCD_ERR("above max prim exec ctrls");
		return -1;
	}

	/*
	 * Secondary exec controls
	 */
	if (vmx_has_sec_exec(t)) {
		rdmsr(MSR_IA32_VMX_PROCBASED_CTLS2, low32, high32);
		act32 = vmx_get_sec_exec(t);
		if (low32 & ~act32) {
			LCD_ERR("min second exec ctrls not set");
			return -1;
		}
		if (~high32 & act32) {
			LCD_ERR("above max second exec ctrls");
			return -1;
		}
	}

	/*
	 * cr3 target count
	 */
	act32 = vmx_get32(t, CR3_TARGET_COUNT);
	if (act32 > 4) {
		LCD_ERR("too many cr3 targets (%u > 4)\n",
			act32);
		return -1;
	}

	/*
	 * i/o bitmap addresses
	 */
	if (vmx_prim_exec_has(t, CPU_BASED_USE_IO_BITMAPS)) {
		act64 = vmx_getl(t, IO_BITMAP_A);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			LCD_ERR("bad io bitmap A addr");
			return -1;
		}
		act64 = vmx_getl(t, IO_BITMAP_B);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			LCD_ERR("bad io bitmap B addr");
			return -1;
		}
	}

	/*
	 * msr bitmap address
	 */
	if (vmx_prim_exec_has(t, CPU_BASED_USE_MSR_BITMAPS)) {
		act64 = vmx_getl(t, MSR_BITMAP);
		if ((act64 & ~PAGE_MASK) || vmx_bad_phys_addr(act64)) {
			LCD_ERR("bad msr bitmap addr %llx\n",
				act64);
			return -1;
		}
	}

	/*
	 * tpr shadow, check 1
	 */
	if (vmx_prim_exec_has(t, CPU_BASED_TPR_SHADOW)) {
		LCD_ERR("tpr shadow checks unimplemented");
		return -1;
	}

	/*
	 * tpr shadow, check 2
	 */
	if (!vmx_prim_exec_has(t, CPU_BASED_TPR_SHADOW) &&
		vmx_has_sec_exec(t) &&
		(vmx_sec_exec_has(t, SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE)
			||
			vmx_sec_exec_has(t, SECONDARY_EXEC_APIC_REGISTER_VIRT)
			||
			vmx_sec_exec_has(t, SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY))) {
		LCD_ERR("tpr shadow unset, but one of x2apic, apic reg virt, or virt intr deliv set");
		return -1;
	}

	/*
	 * nmi's
	 */
	if (!vmx_pin_has(t, PIN_BASED_NMI_EXITING) && 
		vmx_pin_has(t, PIN_BASED_VIRTUAL_NMIS)) {
		LCD_ERR("nmi exit unset but virt nmis set");
		return -1;
	}
	if (!vmx_pin_has(t, PIN_BASED_NMI_EXITING) &&
		vmx_prim_exec_has(t, CPU_BASED_VIRTUAL_NMI_PENDING)) {
			LCD_ERR("nmi exit unset but virt nmi pending set");
			return -1;
	}

	/*
	 * virtual apic page address
	 */
	if (vmx_has_sec_exec(t) && vmx_sec_exec_has(t, SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES)) {
		LCD_ERR("virt apic access checks not implemented");
		return -1;
	}

	/*
	 * virt x2apic
	 */
	if (vmx_has_sec_exec(t) && vmx_sec_exec_has(t, SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE)) {
		LCD_ERR("virt x2apic checks not implemented");
		return -1;
	}

	/*
	 * virt interrupt delivery
	 */
	if (vmx_has_sec_exec(t) && vmx_sec_exec_has(t, SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY)) {
		LCD_ERR("virt int deliv checks not implemented");
		return -1;
	}

	/*
	 * process posted interrupts
	 */
	if (vmx_pin_has(t, PIN_BASED_POSTED_INTR)) {
		LCD_ERR("posted intr checks not implemented");
		return -1;
	}

	/*
	 * vpid
	 */
	if (vmx_has_sec_exec(t) && 
		vmx_sec_exec_has(t, SECONDARY_EXEC_ENABLE_VPID)) {
		if (vmx_get16(t, VIRTUAL_PROCESSOR_ID) == 0) {
			LCD_ERR("vpid cannot be 0");
			return -1;
		}
	}

	/*
	 * eptp
	 */
	if (vmx_has_sec_exec(t) && 
		vmx_sec_exec_has(t, SECONDARY_EXEC_ENABLE_EPT)) {
		act64 = vmx_getl(t, EPT_POINTER);
		if (!(lcd_vmx_capability.ept & VMX_EPTP_UC_BIT) &&
			(((act64 & ((1UL << VMX_EPT_MT_EPTE_SHIFT) - 1)) == 0))) {
			LCD_ERR("ept uncacheable not supported");
			return -1;
		}
		if (!(lcd_vmx_capability.ept & VMX_EPTP_WB_BIT) &&
			(((act64 & ((1UL << VMX_EPT_MT_EPTE_SHIFT) - 1)) == 6))) {
			LCD_ERR("ept write-back not supported");
			return -1;
		}
		if (((act64 >> VMX_EPT_MT_EPTE_SHIFT) & 0x7) != 3) {
			LCD_ERR("ept walk length must be 4");
			return -1;
		}
		if ((act64 & VMX_EPT_AD_ENABLE_BIT) &
			!(lcd_vmx_capability.ept & VMX_EPT_AD_BIT)) {
			LCD_ERR("ept access/dirty bit not supported");
			return -1;
		}
		if (act64 & 0xf80UL) {
			LCD_ERR("ept has rsrv bits set");
			return -1;
		}
		if (vmx_bad_phys_addr(act64 & 0xfff)) {
			LCD_ERR("ept bad addr %llx\n",
				act64);
			return -1;
		}
	}

	/*
	 * ept & unrestricted guest
	 */
	if (vmx_has_sec_exec(t) && 
		vmx_sec_exec_has(t, SECONDARY_EXEC_UNRESTRICTED_GUEST)) {
		LCD_ERR("unrestricted guest checks not implemented");
		return -1;
	}

	/*
	 * vm functions
	 */
	if (vmx_has_sec_exec(t) &&
		vmx_sec_exec_has(t, SECONDARY_EXEC_ENABLE_VMFUNCTIONS)) {
		LCD_ERR("vmfunction checking not implemented");
		return -1;
	}

	/*
	 * vmcs shadowing
	 */
	if (vmx_has_sec_exec(t) &&
		vmx_sec_exec_has(t, SECONDARY_EXEC_SHADOW_VMCS)) {
		LCD_ERR("vmcs shadow checking not implemented");
		return -1;
	}

	/*
	 * ept violation
	 */
	if (vmx_has_sec_exec(t) &&
		vmx_sec_exec_has(t, SECONDARY_EXEC_EPT_VIOLATION_VE)) {
		LCD_ERR("ept violation ve checking not implemented");
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

static int vmx_check_exit_ctrls(struct lcd_arch *t)
{
	u32 low32;
	u32 high32;
	u32 act32;

	/*
	 * min / max settings
	 */
	rdmsr(MSR_IA32_VMX_EXIT_CTLS, low32, high32);
	act32 = vmx_get_exit(t);
	if (low32 & ~act32) {
		LCD_ERR("min exit ctrls not set");
		return -1;
	}
	if (~high32 & act32) {
		LCD_ERR("exit ctrls above max");
		return -1;
	}

	/*
	 * timer
	 */
	if (!vmx_pin_has(t, PIN_BASED_VMX_PREEMPTION_TIMER) &&
		vmx_exit_has(t, VM_EXIT_SAVE_VMX_PREEMPTION_TIMER)) {
		LCD_ERR("preempt timer unset, but save on exit set");
		return -1;
	}

	/*
	 * MSR load / store
	 */
	if (vmx_bad_msr_addr(vmx_getl(t, VM_EXIT_MSR_STORE_ADDR),
				vmx_get32(t, VM_EXIT_MSR_STORE_COUNT))) {
		LCD_ERR("bad exit msr store addr");
		return -1;
	}
	if (vmx_bad_msr_addr(vmx_getl(t, VM_EXIT_MSR_LOAD_ADDR),
				vmx_get32(t, VM_EXIT_MSR_LOAD_COUNT))) {
		LCD_ERR("bad exit msr load addr");
		return -1;
	}

	return 0;
}

static int vmx_check_entry_ctrls(struct lcd_arch *t)
{
	u32 low32;
	u32 high32;
	u32 act32;

	/*
	 * min / max settings
	 */
	rdmsr(MSR_IA32_VMX_ENTRY_CTLS, low32, high32);
	act32 = vmx_get_entry(t);
	if (low32 & ~act32) {
		LCD_ERR("min entry ctrls not set");
		return -1;
	}
	if (~high32 & act32) {
		LCD_ERR("entry ctrls above max");
		return -1;
	}

	/*
	 * interrupt info
	 */
	if (vmx_get32(t, VM_ENTRY_INTR_INFO_FIELD) > 0) {
		LCD_ERR("entry interr info check not implemented");
		return -1;
	}

	/*
	 * MSR load
	 */
	if (vmx_bad_msr_addr(vmx_getl(t, VM_ENTRY_MSR_LOAD_ADDR),
				vmx_get32(t, VM_ENTRY_MSR_LOAD_COUNT))) {
		LCD_ERR("bad entry msr load addr");
		return -1;
	}

	/*
	 * smm
	 */
	if (vmx_entry_has(t, VM_ENTRY_SMM) ||
		vmx_entry_has(t, VM_ENTRY_DEACT_DUAL_MONITOR)) {
		LCD_ERR("entry smm / deact dual monitor checks not implemented");
		return -1;
	}
	
	return 0;
}

static inline int vmx_bad_host_sel(u16 sel)
{
	return (sel & SEGMENT_RPL_MASK) != 0 || (sel & SEGMENT_TI_MASK) != 0;
}

static int vmx_check_host_seg(struct lcd_arch *t)
{
	u16 act16;
	u64 act64;

	/*
	 * cs
	 */
	act16 = vmx_get16(t, HOST_CS_SELECTOR);
	if (vmx_bad_host_sel(act16) || act16 == 0) {
		LCD_ERR("bad host cs selector");
		return -1;
	}

	/*
	 * tr
	 */
	act16 = vmx_get16(t, HOST_TR_SELECTOR);
	if (vmx_bad_host_sel(act16) || act16 == 0) {
		LCD_ERR("bad host tr selector");
		return -1;
	}

	/*
	 * ss
	 */
	act16 = vmx_get16(t, HOST_SS_SELECTOR);
	if (vmx_bad_host_sel(act16) ||
		(!vmx_exit_has(t, VM_EXIT_HOST_ADDR_SPACE_SIZE) &&
			act16 == 0)) {
		LCD_ERR("bad host ss selector");
		return -1;
	}

	/*
	 * ds, es, fs, gs
	 */
	if (vmx_bad_host_sel(vmx_get16(t, HOST_DS_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(t, HOST_ES_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(t, HOST_FS_SELECTOR)) ||
		vmx_bad_host_sel(vmx_get16(t, HOST_GS_SELECTOR))) {
		LCD_ERR("bad ds, es, fs, or gs selector");
		return -1;
	}
	
	/*
	 * fs, gs, gdtr, idtr, tr base addresses
	 */
	act64 = vmx_getl(t, HOST_FS_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		LCD_ERR("bad fs base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(t, HOST_GS_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		LCD_ERR("bad gs base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(t, HOST_GDTR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		LCD_ERR("bad gdtr base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(t, HOST_IDTR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		LCD_ERR("bad idtr base addr %llx\n",
			act64);
		return -1;
	}
	act64 = vmx_getl(t, HOST_TR_BASE);
	if (!vmx_addr_is_canonical(act64)) {
		LCD_ERR("bad tr base addr %llx\n",
			act64);
		return -1;
	}

	return 0;
}

static int vmx_check_host_ctrl_regs(struct lcd_arch *t)
{
	u64 act64;
	u64 min;
	u64 max;

	/*
	 * cr0 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR0_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR0_FIXED1, max);
	act64 = vmx_getl(t, HOST_CR0);
	if (min & ~act64) {
		LCD_ERR("host cr0 missing min settings");
		return -1;
	}
	if (~max & act64) {
		LCD_ERR("host cr0 exceeds max settings");
		return -1;
	}
	/*
	 * cr4 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR4_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR4_FIXED1, max);
	act64 = vmx_getl(t, HOST_CR4);
	if (min & ~act64) {
		LCD_ERR("host cr4 missing min settings");
		return -1;
	}
	if (~max & act64) {
		LCD_ERR("host cr4 exceeds max settings");
		return -1;
	}

	/*
	 * cr3
	 */
	if (vmx_bad_phys_addr(vmx_getl(t, HOST_CR3) & PAGE_MASK)) {
		LCD_ERR("bad host cr3 address");
		return -1;
	}

	/*
	 * sysenter_{eip,esp}
	 */
	if (!vmx_addr_is_canonical(vmx_getl(t, HOST_IA32_SYSENTER_EIP)) ||
		!vmx_addr_is_canonical(vmx_getl(t, HOST_IA32_SYSENTER_ESP))) {
		LCD_ERR("bad host sysenter eip or esp");
		return -1;
	}

	/*
	 * perf global msr
	 */
	if (vmx_exit_has(t, VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL)) {
		LCD_ERR("vmexit perf global load check not implemented");
		return -1;
	}

	/*
	 * pat msr
	 */
	if (vmx_exit_has(t, VM_EXIT_LOAD_IA32_PAT)) {
		LCD_ERR("vmexit pat msr load check not implemented");
		return -1;
	}

	/*
	 * efer msr
	 */
	if (vmx_exit_has(t, VM_EXIT_LOAD_IA32_EFER)) {
		act64 = vmx_getl(t, HOST_IA32_EFER);
		if (0x000000feUL & act64 ||
			(1UL << 9) & act64 ||
			act64 & ~((1UL << 12) - 1)) {
			LCD_ERR("host efer has some reserved bits set");
			return -1;
		}
		if (!vmx_bool_eq(vmx_exit_has(t, VM_EXIT_HOST_ADDR_SPACE_SIZE), (act64 & EFER_LME) && (act64 & EFER_LMA))) {
			LCD_ERR("vmexit host addr space size mismatch with lme or lma in efer");
			return -1;
		}
	}

	return 0;
}

static int vmx_check_addr_size(struct lcd_arch *t)
{
	u64 templ;

	rdmsrl(MSR_EFER, templ);
	if (!(templ & EFER_LMA)) {
		LCD_ERR("not in 64 bit mode");
		return -1;
	}

	if (!vmx_exit_has(t, VM_EXIT_HOST_ADDR_SPACE_SIZE)) {
		LCD_ERR("host addr space size unset, needed to run in 64-bit mode");
		return -1;
	}

	if (!(vmx_getl(t, HOST_CR4) & X86_CR4_PAE)) {
		LCD_ERR("pae bit in host cr4 field not set");
		return -1;
	}

	if (!vmx_addr_is_canonical(vmx_getl(t, HOST_RIP))) {
		LCD_ERR("bad host rip");
		return -1;
	}

	return 0;
}

static int vmx_check_pat_msr(struct lcd_arch *t)
{
	u64 pat;
	unsigned char val;
	unsigned int i;

	pat = vmcs_readl(GUEST_IA32_PAT);

	/*
	 * Ensure each entry is 0 (UC), 1 (WC), 4 (WT), 5 (WP),
	 * 6 (WB), or 7 (UC-)
	 */
	for (i = 0; i <= MAX_PAT_ENTRY; i++) {

		val = (pat & (0xFFUL << (i * 8))) >> (i * 8);
		if (val != 0UL && 
			val != 1UL && 
			val != 4UL &&
			val != 5UL &&
			val != 6UL &&
			val != 7UL) {
			LCD_ERR("Invalid pat type %d in PAT idx %d",
				val, i);
			return -1;
		}
	}

	return 0;
}

static int vmx_check_guest_ctrl_regs(struct lcd_arch *t)
{
	u64 act64;
	u64 min;
	u64 max;

	/*
	 * cr0 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR0_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR0_FIXED1, max);
	act64 = vmx_getl(t, GUEST_CR0);
	if (min & ~act64) {
		LCD_ERR("guest cr0 missing min settings");
		return -1;
	}
	if (~max & act64) {
		LCD_ERR("guest cr0 exceeds max settings");
		return -1;
	}
	if ((act64 & X86_CR0_PG) && !(act64 & X86_CR0_PE)) {
		LCD_ERR("guest cr0 PG set but PE unset");
		return -1;
	}

	/*
	 * cr4 -- min / max
	 */
	rdmsrl(MSR_IA32_VMX_CR4_FIXED0, min);
	rdmsrl(MSR_IA32_VMX_CR4_FIXED1, max);
	act64 = vmx_getl(t, GUEST_CR4);
	if (min & ~act64) {
		LCD_ERR("guest cr4 missing min settings");
		return -1;
	}
	if (~max & act64) {
		LCD_ERR("guest cr4 exceeds max settings");
		return -1;
	}

	/*
	 * debug ctrls
	 */
	if (vmx_entry_has(t, VM_ENTRY_LOAD_DEBUG_CONTROLS)) {
		act64 = vmx_getl(t, GUEST_IA32_DEBUGCTL);
		if ((act64 & 0x3c) || (act64 >> 15)) {
			LCD_ERR("bad guest debug ctrl msr");
			return -1;
		}
	}

	/*
	 * ia-32e mode
	 */
	if (vmx_entry_has(t, VM_ENTRY_IA32E_MODE)) {
		act64 = vmx_getl(t, GUEST_CR0);
		if (!(vmx_getl(t, GUEST_CR0) & X86_CR0_PG) || 
			!(vmx_getl(t, GUEST_CR4) & X86_CR4_PAE)) {
			LCD_ERR("guest entry for ia32e mode requries cr0 PG bit and cr4 PAE bit");
			return -1;
		}
	}
	
	/*
	 * no ia-32e mode
	 */
	if (!vmx_entry_has(t, VM_ENTRY_IA32E_MODE) &&
		(vmx_getl(t, GUEST_CR4) & X86_CR4_PCIDE)) {
		LCD_ERR("guest entry not ia32e, but cr4 PCIDE bit is set");
		return -1;
	}
		

	/*
	 * cr3
	 */
	if (vmx_bad_phys_addr(vmx_getl(t, GUEST_CR3) & PAGE_MASK)) {
		LCD_ERR("bad guest cr3 address");
		return -1;
	}

	/*
	 * debug ctrls, check 2
	 */
	if (vmx_entry_has(t, VM_ENTRY_LOAD_DEBUG_CONTROLS) &&
		(vmx_getl(t, GUEST_DR7) >> 32)) {
		LCD_ERR("bad guest dr7 has high bits set");
		return -1;
	}

	/*
	 * sysenter_{eip,esp}
	 */
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_SYSENTER_EIP)) ||
		!vmx_addr_is_canonical(vmx_getl(t, GUEST_SYSENTER_ESP))) {
		LCD_ERR("bad guest sysenter eip or esp");
		return -1;
	}

	/*
	 * perf global msr
	 */
	if (vmx_entry_has(t, VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL)) {
		LCD_ERR("vmentry perf global load check not implemented");
		return -1;
	}

	/*
	 * pat msr
	 */
	if (vmx_entry_has(t, VM_ENTRY_LOAD_IA32_PAT)) {
		if (vmx_check_pat_msr(t))
			return -1;
	}

	/*
	 * efer msr
	 */
	if (vmx_entry_has(t, VM_ENTRY_LOAD_IA32_EFER)) {
		act64 = vmx_getl(t, GUEST_IA32_EFER);
		if (0x000000feUL & act64 ||
			(1UL << 9) & act64 ||
			act64 & ~((1UL << 12) - 1)) {
			LCD_ERR("guest efer has some reserved bits set");
			return -1;
		}
		if (!vmx_bool_eq(act64 & EFER_LME,
					vmx_entry_has(t, 
						VM_ENTRY_IA32E_MODE))) {
			LCD_ERR("guest efer.lme != entry ia32e mode setting");
			return -1;
		}
		if ((vmx_getl(t, GUEST_CR0) & X86_CR0_PG) &&
			!vmx_bool_eq(act64 & EFER_LME, act64 & EFER_LMA)) {
			LCD_ERR("guest cr0.pg is set, but guest efer.lma != efer.lme");
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

static int vmx_check_guest_seg(struct lcd_arch *t)
{
	u64 act64;
	u64 act64b;
	int rslt;

	/*
	 * Disallow virtual 8086 and unrestricted guest for now, to 
	 * minimize number of checks.
	 */
	if ((vmx_getl(t, GUEST_RFLAGS) & X86_EFLAGS_VM) ||
		(vmx_has_sec_exec(t) && 
			vmx_sec_exec_has(t, SECONDARY_EXEC_UNRESTRICTED_GUEST))) {
		LCD_ERR("guest virtual 8086 mode / unrestricted guest segment checks not implemented");
		return -1;
	}

	/*
	 * assume guest is not going to be in virtual 8086 or unrest. guest
	 */

	/*
	 * tr visible
	 */
	if ((vmx_get16(t, GUEST_TR_SELECTOR) & SEGMENT_TI_MASK) != 0) {
		LCD_ERR("bad visible guest tr (wrong TI)");
		return -1;
	}
	
	/*
	 * ldtr visible
	 */
	if (vmx_seg_usable(vmx_getl(t, GUEST_LDTR_AR_BYTES)) &&
		((vmx_get16(t, GUEST_LDTR_SELECTOR) & SEGMENT_TI_MASK) != 0)) {
		LCD_ERR("bad visible guest ldtr (wrong TI)");
		return -1;
	}

	/*
	 * ss rpl == cs rpl
	 *
	 * (Note! Assumes guest will not be in virtual 8086 or unrest guest.)
	 */
	if ((vmx_get16(t, GUEST_SS_SELECTOR) & SEGMENT_RPL_MASK) !=
		(vmx_get16(t, GUEST_CS_SELECTOR) & SEGMENT_RPL_MASK)) {
		LCD_ERR("guest ss rpl != guest cs rpl");
		return -1;
	}

	/*
	 * base addresses
	 *
	 * (Note! Assumes guest will not be in virtual 8086, and host
	 * is in 64-bit mode.)
	 */
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_TR_BASE))) {
		LCD_ERR("bad guest tr base");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_FS_BASE))) {
		LCD_ERR("bad guest fs base");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_GS_BASE))) {
		LCD_ERR("bad guest gs base");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(t, GUEST_LDTR_AR_BYTES)) &&
		!vmx_addr_is_canonical(vmx_getl(t, GUEST_LDTR_BASE))) {
		LCD_ERR("bad guest ldtr base");
		return -1;
	}
	if (vmx_getl(t, GUEST_CS_BASE) >> 32) {
		LCD_ERR("bad guest cs base");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(t, GUEST_SS_AR_BYTES)) &&
		(vmx_getl(t, GUEST_SS_BASE) >> 32)) {
		LCD_ERR("bad guest ss base");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(t, GUEST_DS_AR_BYTES)) &&
		(vmx_getl(t, GUEST_DS_BASE) >> 32)) {
		LCD_ERR("bad guest ds base");
		return -1;
	}
	if (vmx_seg_usable(vmx_getl(t, GUEST_ES_AR_BYTES)) &&
		(vmx_getl(t, GUEST_ES_BASE) >> 32)) {
		LCD_ERR("bad guest es base");
		return -1;
	}

	/*
	 * ar bytes (this is the worst of it ...)
	 *
	 * (Note! Assumes guest not in virtual 8086 / unrest guest.)
	 *
	 * Segment type
	 */
	rslt = vmx_seg_type(vmx_getl(t, GUEST_CS_AR_BYTES));
	if (rslt != 3 && rslt != 9 && rslt != 11 && rslt != 13 && rslt != 15) {
		LCD_ERR("bad guest cs seg type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) != 3 &&
		vmx_seg_type(act64) != 7) {
		LCD_ERR("bad guest ss seg type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		LCD_ERR("bad guest ds seg type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		LCD_ERR("bad guest es seg type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		LCD_ERR("bad guest fs seg type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) &&
		(!(vmx_seg_type(act64) & 0x1) ||
			((vmx_seg_type(act64) & 0x8) && !(vmx_seg_type(act64) & 0x2)))) {
		LCD_ERR("bad guest gs seg type");
		return -1;
	}

	/*
	 * Desc type
	 */
	act64 = vmx_getl(t, GUEST_CS_AR_BYTES);
	if (!vmx_desc_type(act64)) {
		LCD_ERR("wrong guest cs desc type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		LCD_ERR("wrong guest ss desc type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		LCD_ERR("wrong guest fs desc type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		LCD_ERR("wrong guest gs desc type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		LCD_ERR("wrong guest es desc type");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_desc_type(act64)) {
		LCD_ERR("wrong guest ds desc type");
		return -1;
	}

	/*
	 * cs dpl
	 */
	act64 = vmx_getl(t, GUEST_CS_AR_BYTES);
	act64b = vmx_getl(t, GUEST_SS_AR_BYTES);
	if (vmx_seg_type(act64) == 3) {
		LCD_ERR("guest cs seg type of 3 not allowed when not using unrest guest");
		return -1;
	}
	if ((vmx_seg_type(act64) == 9 || vmx_seg_type(act64) == 11) &&
		(vmx_seg_dpl(act64) != vmx_seg_dpl(act64b))) {
		LCD_ERR("guest cs dpl != guest ss dpl");
		return -1;
	}
	if ((vmx_seg_type(act64) == 13 || vmx_seg_type(act64) == 15) &&
		(vmx_seg_dpl(act64) > vmx_seg_dpl(act64b))) {
		LCD_ERR("guest cs dpl > guest ss dpl");
		return -1;
	}

	/*
	 * ss dpl
	 */
	act64 = vmx_getl(t, GUEST_SS_AR_BYTES);
	act64b = vmx_getl(t, GUEST_CS_AR_BYTES);
	if (vmx_seg_dpl(act64) != 
		(vmx_getl(t, GUEST_SS_SELECTOR) & SEGMENT_RPL_MASK)) {
		LCD_ERR("guest ss dpl != ss rpl");
		return -1;
	}
	if ((vmx_seg_type(act64b) == 3 ||
			vmx_getl(t, GUEST_CR0) & X86_CR0_PE) &&
		vmx_seg_dpl(act64) != 0) {
		LCD_ERR("guest ss dpl != 0");
		return -1;
	}

	/*
	 * ds, es, fs, gs dpl
	 */
	act64 = vmx_getl(t, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(t, GUEST_DS_SELECTOR) & SEGMENT_RPL_MASK)) {
			LCD_ERR("guest ds dpl < ds rpl");
			return -1;
		}
	}
	act64 = vmx_getl(t, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(t, GUEST_ES_SELECTOR) & SEGMENT_RPL_MASK)) {
			LCD_ERR("guest es dpl < es rpl");
			return -1;
		}
	}
	act64 = vmx_getl(t, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(t, GUEST_FS_SELECTOR) & SEGMENT_RPL_MASK)) {
			LCD_ERR("guest fs dpl < fs rpl");
			return -1;
		}
	}
	act64 = vmx_getl(t, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && vmx_seg_type(act64) < 12) {
		if (vmx_seg_dpl(act64) < 
			(vmx_getl(t, GUEST_GS_SELECTOR) & SEGMENT_RPL_MASK)) {
			LCD_ERR("guest gs dpl < gs rpl");
			return -1;
		}
	}

	/*
	 * present bit
	 */
	if (!vmx_seg_pres(vmx_getl(t, GUEST_CS_AR_BYTES))) {
		LCD_ERR("guest cs not present");
		return -1;
	}
	act64 = vmx_getl(t, GUEST_SS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		LCD_ERR("guest ss not present");
		return -1;
	}	
	act64 = vmx_getl(t, GUEST_DS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		LCD_ERR("guest ds not present");
		return -1;
	}	
	act64 = vmx_getl(t, GUEST_ES_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		LCD_ERR("guest es not present");
		return -1;
	}	
	act64 = vmx_getl(t, GUEST_FS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		LCD_ERR("guest fs not present");
		return -1;
	}	
	act64 = vmx_getl(t, GUEST_GS_AR_BYTES);
	if (vmx_seg_usable(act64) && !vmx_seg_pres(act64)) {
		LCD_ERR("guest gs not present");
		return -1;
	}

	/*
	 * d/b and ia32e long mode bits
	 */
	act64 = vmx_getl(t, GUEST_CS_AR_BYTES);
	if (vmx_entry_has(t, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_l_mode(act64) && vmx_seg_db(act64)) {
		LCD_ERR("guest cs improper db/l-mode bits");
		return -1;
	}

	/*
	 * granularity
	 */
	if (vmx_seg_bad_limit(vmx_getl(t, GUEST_CS_AR_BYTES),
				vmx_get32(t, GUEST_CS_LIMIT))) {
		LCD_ERR("bad guest cs limit");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(t, GUEST_SS_AR_BYTES),
					vmx_get32(t, GUEST_SS_LIMIT))) {
		LCD_ERR("bad guest ss limit");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(t, GUEST_DS_AR_BYTES),
					vmx_get32(t, GUEST_DS_LIMIT))) {
		LCD_ERR("bad guest ds limit");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(t, GUEST_ES_AR_BYTES),
					vmx_get32(t, GUEST_ES_LIMIT))) {
		LCD_ERR("bad guest es limit");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(t, GUEST_FS_AR_BYTES),
					vmx_get32(t, GUEST_FS_LIMIT))) {
		LCD_ERR("bad guest fs limit");
		return -1;
	}
	if (vmx_seg_usable_bad_lim(vmx_getl(t, GUEST_GS_AR_BYTES),
					vmx_get32(t, GUEST_GS_LIMIT))) {
		LCD_ERR("bad guest gs limit");
		return -1;
	}

	/*
	 * reserved bits
	 */
	if (vmx_seg_resrv(vmx_getl(t, GUEST_CS_AR_BYTES))) {
		LCD_ERR("guest cs resrv bits set");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(t, GUEST_SS_AR_BYTES))) {
		LCD_ERR("guest ss resrv bits set");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(t, GUEST_DS_AR_BYTES))) {
		LCD_ERR("guest ds resrv bits set");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(t, GUEST_ES_AR_BYTES))) {
		LCD_ERR("guest es resrv bits set");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(t, GUEST_FS_AR_BYTES))) {
		LCD_ERR("guest fs resrv bits set");
		return -1;
	}
	if (vmx_seg_resrv(vmx_getl(t, GUEST_GS_AR_BYTES))) {
		LCD_ERR("guest gs resrv bits set");
		return -1;
	}
	

	/*
	 * tss descriptor
	 */
	act64 = vmx_getl(t, GUEST_TR_AR_BYTES);
	if (!vmx_entry_has(t, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_type(act64) != 3 && vmx_seg_type(act64) != 11) {
		LCD_ERR("wrong guest tr type (outside ia32e)");
		return -1;
	}
	if (vmx_entry_has(t, VM_ENTRY_IA32E_MODE) &&
		vmx_seg_type(act64) != 11) {
		LCD_ERR("wrong guest tr type (inside ia32e)");
		return -1;
	}
	if (vmx_desc_type(act64)) {
		LCD_ERR("wrong guest tr desc type");
		return -1;
	}
	if (!vmx_seg_pres(act64)) {
		LCD_ERR("guest tr not present");
		return -1;
	}
	if (vmx_seg_bad_limit(act64, vmx_get32(t, GUEST_TR_LIMIT))) {
		LCD_ERR("bad guest tr limit");
		return -1;
	}
	if (!vmx_seg_usable(act64)) {
		LCD_ERR("guest tr marked as unusable");
		return -1;
	}
	if (vmx_seg_resrv(act64)) {
		LCD_ERR("guest tr resrv bits set");
		return -1;
	}

	/*
	 * ldtr
	 */
	act64 = vmx_getl(t, GUEST_LDTR_AR_BYTES);
	if (vmx_seg_usable(act64)) {
		if (vmx_seg_type(act64) != 2) {
			LCD_ERR("bad guest ldtr type");
			return -1;
		}
		if (vmx_desc_type(act64)) {
			LCD_ERR("wrong guest ldtr desc type");
			return -1;
		}
		if (!vmx_seg_pres(act64)) {
			LCD_ERR("guest ldtr not present");
			return -1;
		}
		if (vmx_seg_bad_limit(act64, vmx_get32(t, GUEST_LDTR_LIMIT))) {
			LCD_ERR("bad guest ldtr limit");
			return -1;
		}
		if (vmx_seg_resrv(act64)) {
			LCD_ERR("guest ldtr resrv bits set");
			return -1;
		}
	}

	/*
	 * gdtr and idtr
	 */
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_GDTR_BASE))) {
		LCD_ERR("bad guest gdtr base");
		return -1;
	}
	if (!vmx_addr_is_canonical(vmx_getl(t, GUEST_IDTR_BASE))) {
		LCD_ERR("bad guest idtr base");
		return -1;
	}
	if (vmx_getl(t, GUEST_GDTR_LIMIT) >> 15) {
		LCD_ERR("bad guest gdtr limit");
		return -1;
	}
	if (vmx_getl(t, GUEST_IDTR_LIMIT) >> 15) {
		LCD_ERR("bad guest idtr limit");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_rip_rflags(struct lcd_arch *t)
{
	u64 act64;
	u32 act32;
	signed long sact64;
	u32 lin_addr_width;

	/*
	 * rip
	 */
	act64 = vmx_getl(t, GUEST_RIP);
	if (!vmx_entry_has(t, VM_ENTRY_IA32E_MODE) ||
		!vmx_seg_l_mode(vmx_getl(t, GUEST_CS_AR_BYTES))) {
		if (act64 >> 31) {
			LCD_ERR("guest rip has bits in 63:32 set");
			return -1;
		}
	} else {
		lin_addr_width = (cpuid_eax(0x80000008) >> 8) & 0xff;
		sact64 = (signed long)act64;
		if ((sact64 >> lin_addr_width) != 0 &&
			(sact64 >> lin_addr_width) != -1) {
			LCD_ERR("guest rip 0x%llx exceeds max linear addr\n", act64);
			return -1;
		}
	}

	/*
	 * rflags
	 */
	act64 = vmx_getl(t, GUEST_RFLAGS);
	if (act64 >> 21 ||
		(act64 & (1 << 15)) ||
		(act64 & (1 << 5)) ||
		(act64 & (1 << 3))) {
		LCD_ERR("guest rflags has resrv bits set");
		return -1;
	}
	if (!(act64 & (1 << 1))) {
		LCD_ERR("guest rflags bit 1 not set");
		return -1;
	}
	if ((vmx_entry_has(t, VM_ENTRY_IA32E_MODE) ||
		!(vmx_getl(t, GUEST_CR0) & X86_CR0_PE)) &&
		(act64 & X86_EFLAGS_VM)) {
		LCD_ERR("guest rflags vm bit set mistakenly");
		return -1;
	}
	act32 = vmx_get32(t, VM_ENTRY_INTR_INFO_FIELD);
	if ((act32 & INTR_INFO_VALID_MASK) &&
		(act32 & INTR_INFO_INTR_TYPE_MASK) &&
		!(act64 & X86_EFLAGS_IF)) {
		LCD_ERR("valid ext intr info, but rflags.if not set");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_non_reg(struct lcd_arch *t)
{
	if (vmx_get32(t, GUEST_ACTIVITY_STATE) !=
		GUEST_ACTIVITY_ACTIVE) {
		LCD_ERR("checks for non-active guest activity states not implemented");
		return -1;
	}

	if (vmx_get32(t, GUEST_INTERRUPTIBILITY_INFO) != 0) {
		LCD_ERR("checks for non-zero guest intr info not implemented");
		return -1;
	}

	if (vmx_entry_has(t, VM_ENTRY_SMM)) {
		LCD_ERR("guest intr info check for entry to smm not implemented");
		return -1;
	}

	if (vmx_get32(t, GUEST_PENDING_DBG_EXCEPTIONS) != 0) {
		LCD_ERR("nonzero guest pending dbg exc checks not implemented");
		return -1;
	}

	if (vmx_getl(t, VMCS_LINK_POINTER) != -1ull) {
		LCD_ERR("checks when vmcs link pointer != -1 not implemented");
		return -1;
	}

	return 0;
}

static int vmx_check_guest_pdpte(struct lcd_arch *t)
{
	
	if (vmx_getl(t, GUEST_CR0) & X86_CR0_PG &&
		vmx_getl(t, GUEST_CR4) & X86_CR4_PAE &&
		!(vmx_getl(t, GUEST_IA32_EFER) & EFER_LMA)) {
		LCD_ERR("guest pdpte checks for PAE paging not implemented");
		return -1;
	}

	return 0;
}

static int vmx_check_guest(struct lcd_arch *t)
{
	if (vmx_check_guest_ctrl_regs(t))
		return -1;
	if (vmx_check_guest_seg(t))
		return -1;
	if (vmx_check_guest_rip_rflags(t))
		return -1;
	if (vmx_check_guest_non_reg(t))
		return -1;
	if (vmx_check_guest_pdpte(t))
		return -1;
	return 0;
}

static int vmx_check_host(struct lcd_arch *t)
{
	if (vmx_check_host_ctrl_regs(t))
		return -1;
	if (vmx_check_host_seg(t))
		return -1;
	return 0;
}

static int vmx_check_ctrls(struct lcd_arch *t)
{
	if (vmx_check_exec_ctrls(t))
		return -1;
	if (vmx_check_exit_ctrls(t))
		return -1;
	if (vmx_check_entry_ctrls(t))
		return -1;
	return 0;
}

static int vmx_check_ctrls_and_host(struct lcd_arch *t)
{
	if (vmx_check_ctrls(t))
		return -1;
	if (vmx_check_host(t))
		return -1;
	if (vmx_check_addr_size(t))
		return -1;
	return 0;
}

int lcd_arch_check(struct lcd_arch *t)
{
	if (vmx_check_ctrls_and_host(t))
		return -1;
	if (vmx_check_guest(t))
		return -1;

	return 0;
}
