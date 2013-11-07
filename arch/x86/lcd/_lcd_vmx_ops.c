
/* The label in the asm code. */
extern unsigned long vmx_return;

static struct vmcs_config {
  int size;
  int order;
  u32 revision_id;
  u32 pin_based_exec_ctrl;
  u32 cpu_based_exec_ctrl;
  u32 cpu_based_2nd_exec_ctrl;
  u32 vmexit_ctrl;
  u32 vmentry_ctrl;
} vmcs_config;


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

static void vmcs_clear(struct vmcs *vmcs)
{
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMCLEAR_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "kvm: vmclear fail: %p/%llx\n",
           vmcs, phys_addr);
}

static void vmcs_load(struct vmcs *vmcs)
{
  u64 phys_addr = __pa(vmcs);
  u8 error;

  asm volatile (ASM_VMX_VMPTRLD_RAX "; setna %0"
                : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
                : "cc", "memory");
  if (error)
    printk(KERN_ERR "vmx: vmptrld %p/%llx failed\n",
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
  printk(KERN_ERR "vmwrite error: reg %lx value %lx (err %d)\n",
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

static struct vmcs *vmx_alloc_vmcs(void)
{
  int cpu = raw_smp_processor_id();
  int node = cpu_to_node(cpu);
  struct page *pages;
  struct vmcs *vmcs;

  pages = alloc_pages_exact_node(node, GFP_KERNEL, vmcs_config.order);
  if (!pages)
    return NULL;
  vmcs = page_address(pages);
  memset(vmcs, 0, vmcs_config.size);
  vmcs->revision_id = vmcs_config.revision_id; /* vmcs revision id */
  return vmcs;
}

static void vmx_free_vmcs(struct vmcs *vmcs)
{
  free_pages((unsigned long)vmcs, vmcs_config.order);
}

static void vmx_setup_constant_host_state(void)
{
  u32 low32, high32;
  unsigned long tmpl;
  struct desc_ptr dt;

  vmcs_writel(HOST_CR0, read_cr0() & ~X86_CR0_TS);  /* 22.2.3 */
  vmcs_writel(HOST_CR4, read_cr4());  /* 22.2.3, 22.2.5 */
  vmcs_writel(HOST_CR3, read_cr3());  /* 22.2.3 */

  vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);  /* 22.2.4 */
  vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
  vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);  /* 22.2.4 */

  native_store_idt(&dt);
  vmcs_writel(HOST_IDTR_BASE, dt.address);   /* 22.2.4 */

  /* TODO: change the label for vmx_return  */
  asm("mov $.Lvmx_return, %0" : "=r"(tmpl));
  vmcs_writel(HOST_RIP, tmpl); /* 22.2.5 */

  rdmsr(MSR_IA32_SYSENTER_CS, low32, high32);
  vmcs_write32(HOST_IA32_SYSENTER_CS, low32);
  rdmsrl(MSR_IA32_SYSENTER_EIP, tmpl);
  vmcs_writel(HOST_IA32_SYSENTER_EIP, tmpl);   /* 22.2.3 */

  rdmsr(MSR_EFER, low32, high32);
  vmcs_write32(HOST_IA32_EFER, low32);

  if (vmcs_config.vmexit_ctrl & VM_EXIT_LOAD_IA32_PAT) {
    rdmsr(MSR_IA32_CR_PAT, low32, high32);
    vmcs_write64(HOST_IA32_PAT, low32 | ((u64) high32 << 32));
  }

  vmcs_write16(HOST_FS_SELECTOR, 0);            /* 22.2.4 */
  vmcs_write16(HOST_GS_SELECTOR, 0);            /* 22.2.4 */

  rdmsrl(MSR_FS_BASE, tmpl);
  vmcs_writel(HOST_FS_BASE, tmpl); /* 22.2.4 */
  rdmsrl(MSR_GS_BASE, tmpl);
  vmcs_writel(HOST_GS_BASE, tmpl); /* 22.2.4 */
}
