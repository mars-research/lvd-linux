/*
 * vmcs.h
 *
 * Functions for accessing an LCD's vmcs.
 *
 * IMPORTANT: Before doing a vmcs_read or vmcs_write, you *must* load
 * the VMCS on the calling CPU. To do this, call vmx_get_cpu/vmx_put_cpu:
 *
 *           vmx_get_cpu(lcd);
 *           vmcs_readl(...some field...);
 *           vmx_put_cpu(lcd);
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_VMCS_H
#define ASM_X86_LCD_DOMAINS_VMCS_H

#include <asm/vmx.h>
#include <linux/kernel.h>

#include <asm/lcd_domains/types.h>
#include <lcd_domains/microkernel.h>

/**
 * Takes vmcs from any state to <inactive, clear, not current>
 */
static inline void vmcs_clear(struct lcd_arch_vmcs *vmcs)
{
	u64 hpa = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&hpa), "m"(hpa)
                : "cc", "memory");
	if (error)
		LCD_ERR("vmclear fail: %p/%llx\n",	vmcs, hpa);
}
/**
 * Takes vmcs to <active, current> on cpu. Any vmcs reads and writes
 * will affect this vmcs.
 */
static inline void vmcs_load(struct lcd_arch_vmcs *vmcs)
{
	u64 hpa = __pa(vmcs);
	u8 error;

	asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&hpa), "m"(hpa)
                : "cc", "memory");
	if (error)
		LCD_ERR("vmptrld %p/%llx failed\n", vmcs, hpa);
}

static __always_inline unsigned long __vmcs_readl(unsigned long field)
{
	unsigned long value;

	asm volatile (ASM_VMX_VMREAD_RDX_RAX
		      : "=a"(value) : "d"(field) : "cc");
	return value;
}

static __always_inline u16 vmcs_read16(unsigned long field)
{
	return __vmcs_readl(field);
}

static __always_inline u32 vmcs_read32(unsigned long field)
{
	return __vmcs_readl(field);
}

static __always_inline u64 vmcs_read64(unsigned long field)
{
#ifdef CONFIG_X86_64
	return __vmcs_readl(field);
#else
	return __vmcs_readl(field) | ((u64)__vmcs_readl(field+1) << 32);
#endif
}

static __always_inline unsigned long vmcs_readl(unsigned long field)
{
	return __vmcs_readl(field);
}

static inline void __vmcs_writel(unsigned long field, unsigned long value)
{
	u8 error;

	asm volatile (ASM_VMX_VMWRITE_RAX_RDX "; setna %0"
                : "=q"(error) : "a"(value), "d"(field) : "cc");
	if (unlikely(error)) {
		LCD_ERR("reg %lx value %lx (err %d)\n",
			field, value, vmcs_read32(VM_INSTRUCTION_ERROR));
		dump_stack();
	}
}

static __always_inline void vmcs_write16(unsigned long field, u16 value)
{
	__vmcs_writel(field, value);
}

static __always_inline void vmcs_write32(unsigned long field, u32 value)
{
	__vmcs_writel(field, value);
}

static __always_inline void vmcs_write64(unsigned long field, u64 value)
{
	__vmcs_writel(field, value);
#ifndef CONFIG_X86_64
	asm volatile ("");
	__vmcs_writel(field+1, value >> 32);
#endif
}

static __always_inline void vmcs_writel(unsigned long field, unsigned long value)
{
	__vmcs_writel(field, value);
}

/**
 * Loads t on the calling cpu.
 *
 * Disables preemption. Call vmx_put_cpu() when finished.
 */
void vmx_get_cpu(struct lcd_arch *lcd_arch);
/**
 * Match with vmx_get_cpu.
 *
 * Enables preemption.
 */
void vmx_put_cpu(struct lcd_arch *lcd_arch);

#endif /* ASM_X86_LCD_DOMAINS_VMCS_H */
