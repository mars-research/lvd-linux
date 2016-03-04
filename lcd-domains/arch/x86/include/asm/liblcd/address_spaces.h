/*
 * address_spaces.h
 *
 * The initial virtual and physical address spaces inside
 * an LCD.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LIBLCD_ADDRESS_SPACES_H
#define ASM_X86_LIBLCD_ADDRESS_SPACES_H

#include <lcd_domains/types.h>
#include <liblcd/boot_info.h>

/*
 * Guest Physical Memory Layout
 * ============================
 *
 * This is the guest physical address space layout the LCD boots
 * with. It is of course free to modify it (using the capabilities
 * to the memory).
 *
 * No gdt/tss/idt for now (easier).
 *
 * From bottom to top,
 *
 *   -- The bottom 1 GB is unmapped / reserved. (This could be used
 *      for BIOS memory-mapped information, low DMA, etc. in the
 *      future.) This conveniently causes EPT faults for null physical
 *      addresses.
 *
 *   -- 1 GB miscellaneous (only beginning part is mapped in guest physical):
 *
 *          -- UTCB (microkernel manages this page)
 *          -- Bootstrap info pages
 *          -- Bootstrap guest virtual address space page tables
 *
 *   -- 1 GB HOLE (unmapped)
 *
 *   -- 1 GB for initial stack region (only top part is mapped):
 *
 *          -- Stack starts at the top
 *
 *   -- 1 GB HOLE (unmapped)
 *
 *   -- 1 GB for heap region (unmapped at boot; only bottom part is used)
 *
 *   -- 1 GB HOLE (unmapped)
 *
 *   -- 1 GB for RAM memory mapping region (think: kmap)
 *
 *   -- 1 GB HOLE (unmapped)
 *
 *   -- 1 GB for ioremap region (unmapped at boot)
 *
 *   -- 500 GB HOLE (unmapped)
 *
 *   -- 2 GB for kernel module mapping area. The kernel module itself
 *      is mapped at the correct offset into this area so that
 *      the guest virtual mapping can use huge 1 GB pages.
 *
 *   -- The upper part is unusable (see Intel SDM V3 28.2.2). The last
 *      usable byte is at 0x0000 FFFF FFFF FFFF. This is why we can't do
 *      a direct map of the kernel module.
 *
 *              +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *              |         Unusable          |
 *              +---------------------------+ 0x0000 FFFF FFFF FFFF
 *              |                           |
 *              :           Free            :
 *              |                           |
 *              +---------------------------+ 0x0000 0080 0000 0000 (512 GB)
 *              |       Kernel Module       |
 *              |    (code, bss, rodata)    |
 *              |          (2 GB)           |
 *              +---------------------------+ 0x0000 007f 8000 0000 (510 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (500 GB)         |
 *              +---------------------------+ 0x0000 0002 8000 0000 (10 GB) 
 *              |       ioremap Region      |                    
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0002 4000 0000 (9 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0002 0000 0000 (8 GB) 
 *              |       RAM Map Region      |                    
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0001 c000 0000 (7 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0001 8000 0000 (6 GB)
 *              |        Heap Region        | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0001 4000 0000 (5 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0001 0000 0000 (4 GB)
 *              |     Initial Stack Region  |
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0000 c000 0000 (3 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0000 8000 0000 (2 GB)
 *              |     UTCB and Bootstrap    |
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0000 4000 0000 (1 GB)
 *              |       Free / Unmapped     | 
 *              |          (1 GB)           |
 *              +---------------------------+ 0x0000 0000 0000 0000 (0 GB)
 *
 * Guest Virtual Memory Layout
 * ===========================
 *
 * The layout is simple: the physical address space is mapped once in the
 * high 512 GB range. This puts the kernel module mapping area in the high
 * 2 GBs, the same as on the host (for x86_64 anyway - see
 * Documentation/x86/x86_64/mm.txt, arch/x86/include/asm/page_64_types.h,
 * and arch/x86/include/asm/pgtable_64_types.h).
 *
 *              +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *              |       Kernel Module       |
 *              |    (code, bss, rodata)    |
 *              |          (2 GB)           |        \
 *              +---------------------------+        |
 *              |                           |        |
 *              |      ioremap, heap,       |        |
 *              :       stack, utcb,        :          512 GBs
 *              :           etc.            :        |
 *              |                           |        |
 *              |                           |        |
 *              +---------------------------+        /
 *              |     Unmapped in physical  |
 *              |          (1 GB)           |
 *              +---------------------------+ 0xFFFF FF7F FFFF FFFF 
 *              |                           |           
 *              |                           |
 *              :                           :
 *              :        Unmapped           :
 *              :                           :
 *              |                           |
 *              |                           |         
 *              +---------------------------+ 0x0000 0000 0000 0000
 *
 * Everything else is unmapped. We use 2MB pages. This means there
 * are only 10-20 4 KB pages needed to set up the entire guest virtual
 * address space.
 *
 * (Why don't we map everything in the high physical range so we can
 * identity map? Answer: The high part of the physical address space is
 * unusable.)
 */

/* Region sizes */

#define LCD_MISC_REGION_SIZE (1UL << 30) /* .................... 1 GB  */
#define LCD_STACK_REGION_SIZE (1UL << 30) /* ................... 1 GB  */
#define LCD_HEAP_REGION_SIZE (1UL << 30) /* .................... 1 GB  */
#define LCD_RAM_MAP_REGION_SIZE (1UL << 30) /* ................. 1 GB  */
#define LCD_IOREMAP_REGION_SIZE (1L << 30) /* .................  1 GB  */
#define LCD_KERNEL_MODULE_REGION_SIZE (2UL << 30) /* ........... 2 GBs */

/* Component Sizes. */

#define LCD_UTCB_SIZE PAGE_SIZE /* ........................... 4  KBs */
#define LCD_BOOTSTRAP_PAGES_SIZE (1 * PAGE_SIZE) /* .......... 4  KBs */
#define LCD_BOOTSTRAP_PAGE_TABLES_SIZE (16 * PAGE_SIZE) /* ... 64 KBs */
#define LCD_STACK_SIZE (2 * PAGE_SIZE) /* .................... 8  KBs */

/* Orders (for convenience) */

#define LCD_BOOTSTRAP_PAGES_ORDER \
	(ilog2(LCD_BOOTSTRAP_PAGES_SIZE >> PAGE_SHIFT))
#define LCD_BOOTSTRAP_PAGE_TABLES_ORDER \
	(ilog2(LCD_BOOTSTRAP_PAGE_TABLES_SIZE >> PAGE_SHIFT))
#define LCD_STACK_ORDER \
	(ilog2(LCD_STACK_SIZE >> PAGE_SHIFT))

/* Offsets. */

#define LCD_MISC_REGION_OFFSET (1UL << 30)
#define LCD_UTCB_OFFSET LCD_MISC_REGION_OFFSET
#define LCD_BOOTSTRAP_PAGES_OFFSET (LCD_UTCB_OFFSET + LCD_UTCB_SIZE)
#define LCD_BOOTSTRAP_PAGE_TABLES_OFFSET \
	(LCD_BOOTSTRAP_PAGES_OFFSET + LCD_BOOTSTRAP_PAGES_SIZE)

/* HOLE */

#define LCD_STACK_REGION_OFFSET \
	(LCD_MISC_REGION_OFFSET + LCD_MISC_REGION_SIZE + (1UL << 30))
#define LCD_STACK_OFFSET \
	(LCD_STACK_REGION_OFFSET + LCD_STACK_REGION_SIZE - LCD_STACK_SIZE)

/* HOLE */

#define LCD_HEAP_REGION_OFFSET \
	(LCD_STACK_REGION_OFFSET + LCD_STACK_REGION_SIZE + (1UL << 30))
#define LCD_HEAP_OFFSET LCD_HEAP_REGION_OFFSET

/* HOLE */

#define LCD_RAM_MAP_REGION_OFFSET \
	(LCD_HEAP_REGION_OFFSET + LCD_HEAP_REGION_SIZE + (1UL << 30))
#define LCD_RAM_MAP_OFFSET LCD_RAM_MAP_REGION_OFFSET

/* HOLE */

#define LCD_IOREMAP_REGION_OFFSET \
	(LCD_RAM_MAP_REGION_OFFSET + LCD_RAM_MAP_REGION_SIZE + (1UL << 30))
#define LCD_IOREMAP_OFFSET LCD_IOREMAP_REGION_OFFSET

/* HOLE */

#define LCD_KERNEL_MODULE_REGION_OFFSET \
	(LCD_IOREMAP_REGION_OFFSET + LCD_IOREMAP_REGION_SIZE + (500UL << 30))

/* Addresses */

#define LCD_PHYS_BASE (0UL)
#define LCD_VIRT_BASE (0xFFFFFF8000000000UL)

#define LCD_UTCB_GP_ADDR (__gpa(LCD_PHYS_BASE + LCD_UTCB_OFFSET))
#define LCD_UTCB_GV_ADDR (__gva(LCD_VIRT_BASE + LCD_UTCB_OFFSET))

#define LCD_BOOTSTRAP_PAGES_GP_ADDR \
	(__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGES_OFFSET))
#define LCD_BOOTSTRAP_PAGES_GV_ADDR \
	(__gva(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGES_OFFSET))

#define LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR \
	(__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET))
#define LCD_BOOTSTRAP_PAGE_TABLES_GV_ADDR \
	(__gva(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET))

#define LCD_STACK_GP_ADDR (__gpa(LCD_PHYS_BASE + LCD_STACK_OFFSET))
#define LCD_STACK_GV_ADDR (__gva(LCD_VIRT_BASE + LCD_STACK_OFFSET))

#define LCD_HEAP_GP_ADDR (__gpa(LCD_PHYS_BASE + LCD_HEAP_OFFSET))
#define LCD_HEAP_GV_ADDR (__gva(LCD_VIRT_BASE + LCD_HEAP_OFFSET))

#define LCD_RAM_MAP_GP_ADDR (__gpa(LCD_PHYS_BASE + LCD_RAM_MAP_OFFSET))
#define LCD_RAM_MAP_GV_ADDR (__gva(LCD_VIRT_BASE + LCD_RAM_MAP_OFFSET))

#define LCD_IOREMAP_GP_ADDR (__gpa(LCD_PHYS_BASE + LCD_IOREMAP_OFFSET))
#define LCD_IOREMAP_GV_ADDR (__gva(LCD_VIRT_BASE + LCD_IOREMAP_OFFSET))

#define LCD_KERNEL_MODULE_REGION_GP_ADDR \
	(__gpa(LCD_PHYS_BASE + LCD_KERNEL_MODULE_REGION_OFFSET))
#define LCD_KERNEL_MODULE_REGION_GV_ADDR \
	(__gva(LCD_VIRT_BASE + LCD_KERNEL_MODULE_REGION_OFFSET))

/* CONTROL REGISTERS -------------------------------------------------- */

#define LCD_ARCH_FS_BASE     __gpa(0UL)
#define LCD_ARCH_FS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GS_BASE     __gpa(0UL)
#define LCD_ARCH_GS_LIMIT    0xFFFFFFFF
#define LCD_ARCH_GDTR_BASE   __gpa(0UL)
#define LCD_ARCH_GDTR_LIMIT  0x0 /* no gdt */
#define LCD_ARCH_TSS_BASE    __gpa(0UL)
#define LCD_ARCH_TSS_LIMIT   0x0 /* no tss */
#define LCD_ARCH_IDTR_BASE   __gpa(0UL)
#define LCD_ARCH_IDTR_LIMIT  0x0 /* no idt right now */

/* CONVERSION MACROS -------------------------------------------------- */

static inline gva_t isolated_lcd_gpa2gva(gpa_t gpa)
{
	return __gva(LCD_VIRT_BASE + (gpa_val(gpa) - LCD_PHYS_BASE));
}

static inline gpa_t isolated_lcd_gva2gpa(gva_t gva)
{
	return __gpa(LCD_PHYS_BASE + (gva_val(gva) - LCD_VIRT_BASE));
}

/* ASSERTIONS -------------------------------------------------- */


static inline void
__attribute__((used)) /* required so that this func is processed */
__lcd_build_checks__(void)
{
	/* 
	 * This function is for build checks only. (We can't put
	 * build bug on's in the top level.)
	 */

	/* LCD Bootstrap pages must be a power-of-two number of pages */
	BUILD_BUG_ON_NOT_POWER_OF_2(LCD_BOOTSTRAP_PAGES_SIZE >> PAGE_SHIFT);

	/* lcd_boot_info won't fit in bootstrap pages. Add more pages. */
	BUILD_BUG_ON(sizeof(struct lcd_boot_info) > LCD_BOOTSTRAP_PAGES_SIZE);

	/* We need exactly two page tables for the gv address space. */
	BUILD_BUG_ON(LCD_BOOTSTRAP_PAGE_TABLES_SIZE >> PAGE_SHIFT != 16);
	
	/* Number of LCD stack pages needs to be a power-of-two multiple. */
	BUILD_BUG_ON_NOT_POWER_OF_2(LCD_STACK_SIZE >> PAGE_SHIFT);

	/* We use the host module loader for loading .ko's and we
	 * don't relink (it's possible but not worth it right now).
	 * So the kernel module region gv address needs to match the
	 * starting gv address for the kernel map in the host. */
	BUILD_BUG_ON(gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR) != 
		__START_KERNEL_map);

	/* All memory should fit into 512 GBs. This is because we only
	 * use one pud (internally in common/module_create.c). */
	BUILD_BUG_ON((LCD_KERNEL_MODULE_REGION_OFFSET + 
			LCD_KERNEL_MODULE_REGION_SIZE) > (512UL << 30));
}

#endif /* ASM_X86_LIBLCD_ADDRESS_SPACES_H */
