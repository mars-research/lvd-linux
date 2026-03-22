/*
 * create.c
 *
 * Code for creating an lcd_arch (the arch-specific part
 * of an LCD)
 *
 * Copyright: University of Utah
 */

#include <asm/vmx.h>
#include <asm/tlbflush.h>

#include <linux/mm.h>
#include <linux/spinlock.h>
#include <asm/desc.h>

#include <asm/lcd_domains/microkernel.h>
#include <lcd_domains/microkernel.h>
#include <asm/liblcd/address_spaces.h>

#if defined(CONFIG_LCD_TRACE_BUFFER) && defined(CONFIG_MAP_TRACE_BUFFER_BFLANK)
#include <linux/lcd_trace.h>
#endif

/* These are initialized by init.c */
struct kmem_cache *lcd_arch_cache;
struct lcd_vmcs_config lcd_global_vmcs_config;
unsigned long *lcd_global_msr_bitmap;
struct lcd_vpids lcd_vpids;


static inline bool cpu_has_vmx_ept_ad_bits(void)
{
	return lcd_vmx_capability.ept & VMX_EPT_AD_BIT;
}

int lcd_arch_create(struct lcd_arch **out)
{
	struct lcd_arch *lcd_arch;
	int ret;
#ifdef CONFIG_LVD
	int cpu;
#endif

	/*
	 * Alloc lcd_arch
	 */
	lcd_arch = kmem_cache_zalloc(lcd_arch_cache, GFP_KERNEL);
	if (!lcd_arch) {
		LCD_ERR("failed to alloc lcd_arch");
		ret = -ENOMEM;
		goto fail_alloc;
	}

#ifdef CONFIG_LVD
	lcd_arch->ept_id = 1; 

	lcd_arch->eptp_lcd = kmalloc(sizeof(u64) * num_online_cpus(), GFP_KERNEL);

	if (!lcd_arch->eptp_lcd) {
		LCD_ERR("Out of memory");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	/* create ept for this LCD on all cpus */
	for_each_online_cpu(cpu) {
		/* Get the EPT VMFUNC switching page for this CPU */
		struct page *eptp_list_page = (struct page *) per_cpu(vmfunc_eptp_list_page, cpu);
#if defined(CONFIG_LCD_TRACE_BUFFER) && defined(CONFIG_MAP_TRACE_BUFFER_BFLANK)
		struct ring_trace_buffer *this_ring_buf = &per_cpu(ring_buffer, cpu);
#endif

		u64 *eptp_list = phys_to_virt(page_to_phys(eptp_list_page)); 
		u64 eptp;
		u64 *root;

		/* Allocate LCDs EPT */
		lcd_arch->eptp_lcd[cpu] = lcd_arch_ept_init_one();
		root = (u64*) lcd_arch->eptp_lcd[cpu];

		eptp = VMX_EPT_DEFAULT_MT |
			(LCD_ARCH_EPT_WALK_LENGTH - 1) << LCD_ARCH_EPTP_WALK_SHIFT;
		if (cpu_has_vmx_ept_ad_bits()) {
			//lcd_arch->ept.access_dirty_enabled = true;
			eptp |= VMX_EPT_AD_ENABLE_BIT;
		}
		eptp |= hpa_val(va2hpa(root)) & PAGE_MASK;

		/* Add EPT to the VMFUNC switching page */
		eptp_list[lcd_arch->ept_id] = eptp;
#if defined(CONFIG_LCD_TRACE_BUFFER) && defined(CONFIG_MAP_TRACE_BUFFER_BFLANK)
		/* pass GVA of this buffer on idx:3 */
		eptp_list[3] = (uint64_t) this_ring_buf;
		/* pass num_pages of ring buffer on idx:4 */
		eptp_list[4] = RING_BUFFER_SIZE >> PAGE_SHIFT;
#endif
	}


#endif
	/* TODO: configure ept switching */
	*out = lcd_arch;

	return 0;

fail_alloc:
	return ret;
}

void lcd_arch_destroy(struct lcd_arch *lcd_arch)
{
	kfree(lcd_arch->eptp_lcd);
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
