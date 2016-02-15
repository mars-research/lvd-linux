/*
 * address_spaces.h
 *
 * The initial virtual and physical address spaces inside
 * an LCD.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_ADDRESS_SPACES_H
#define LCD_DOMAINS_ADDRESS_SPACES_H

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
 *   -- 2 GB HOLE (unmapped)
 *
 *   -- 8 GB for RAM memory mapping region (think: kmap)
 *
 *   -- 48 GB HOLE (unmapped)
 *
 *   -- 256 GB for ioremap region (unmapped at boot)
 *
 *   -- 190 GB HOLE (unmapped)
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
 *              |          (190 GB)         |
 *              +---------------------------+ 0x0000 0050 0000 0000 (320 GB) 
 *              |       ioremap Region      |                    
 *              |       (64 GB aligned)     |                    
 *              |          (256 GB)         |
 *              +---------------------------+ 0x0000 0010 0000 0000 (64 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (48 GB)          |
 *              +---------------------------+ 0x0000 0004 0000 0000 (16 GB) 
 *              |       RAM Map Region      |                    
 *              |       (8 GB aligned)      |                    
 *              |          (8 GB)           |
 *              +---------------------------+ 0x0000 0002 0000 0000 (8 GB)
 *              |       HOLE / Unmapped     | 
 *              |          (2 GB)           |
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
 * 2 GBs, the same as on the host (for x86_64 anyway).
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
 * Everything else is unmapped. We use huge 1 GB pages. This means there
 * are only two 4 KB pages needed to set up the entire guest virtual
 * address space.
 *
 * (Why don't we map everything in the high physical range so we can
 * identity map? Answer: The high part of the physical address space is
 * unusable.)
 */

/* Region sizes */

#define LCD_MISC_REGION_SIZE (1UL << 30) /* .................... 1 GB    */
#define LCD_STACK_REGION_SIZE (1UL << 30) /* ................... 1 GB    */
#define LCD_HEAP_REGION_SIZE (1UL << 30) /* .................... 1 GB    */
#define LCD_RAM_MAP_REGION_SIZE (1UL << 30) /* ................. 8 GBs   */
#define LCD_IOREMAP_REGION_SIZE (256UL << 30) /* ............... 256 GBs */
#define LCD_KERNEL_MODULE_REGION_SIZE (2UL << 30) /* ........... 2 GBs   */

/* Component Sizes. */

#define LCD_UTCB_SIZE PAGE_SIZE /* ........................... 4  KBs */
#define LCD_BOOTSTRAP_PAGES_SIZE (2 * PAGE_SIZE) /* .......... 8  KBs */
#define LCD_BOOTSTRAP_PAGE_TABLES_SIZE (2 * PAGE_SIZE) /* .... 8  KBs */
#define LCD_STACK_SIZE (2 * PAGE_SIZE) /* .................... 8  KBs */

static inline void
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
	BUILD_BUG_ON(LCD_BOOTSTRAP_PAGE_TABLES_SIZE >> PAGE_SHIFT != 2);
	
	/* Number of LCD stack pages needs to be a power-of-two multiple. */
	BUILD_BUG_ON_NOT_POWER_OF_2(LCD_STACK_SIZE >> PAGE_SHIFT);
}

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
	(LCD_MISC_REGION_OFFSET + LCD_MISC_REGION_SIZE + 1UL << 30)
#define LCD_STACK_OFFSET \
	(LCD_STACK_REGION_OFFSET + LCD_STACK_REGION_SIZE - LCD_STACK_SIZE)

/* HOLE */

#define LCD_HEAP_REGION_OFFSET \
	(LCD_STACK_REGION_OFFSET + LCD_STACK_REGION_SIZE + (1UL << 30))
#define LCD_HEAP_OFFSET LCD_HEAP_REGION_OFFSET

/* HOLE */

#define LCD_RAM_MAP_REGION_OFFSET \
	(LCD_HEAP_REGION_OFFSET + LCD_HEAP_REGION_SIZE + (2UL << 30))
#define LCD_RAM_MAP_OFFSET LCD_RAM_MAP_REGION_OFFSET

/* HOLE */

#define LCD_IOREMAP_REGION_OFFSET \
	(LCD_RAM_MAP_REGION_OFFSET + LCD_RAM_MAP_REGION_SIZE + (48UL << 30))
#define LCD_IOREMAP_OFFSET LCD_IOREMAP_REGION_OFFSET

/* HOLE */

#define LCD_KERNEL_MODULE_REGION_OFFSET \
	(LCD_IOREMAP_REGION_OFFSET + LCD_IOREMAP_REGION_SIZE + (190UL << 30))

/* Addresses */

#define LCD_PHYS_BASE (0UL)
#define LCD_VIRT_BASE (0xFFFFFF7FFFFFFFFFUL)

#define LCD_UTCB_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_UTCB_OFFSET)
#define LCD_UTCB_GV_ADDR __gva(LCD_VIRT_BASE + LCD_UTCB_OFFSET)

#define LCD_BOOTSTRAP_PAGES_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGES_OFFSET)
#define LCD_BOOTSTRAP_PAGES_GV_ADDR \
	__gva(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGES_OFFSET)

#define LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET)
#define LCD_BOOTSTRAP_PAGE_TABLES_GV_ADDR \
	__gva(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET)

#define LCD_STACK_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_STACK_OFFSET)
#define LCD_STACK_GV_ADDR __gva(LCD_VIRT_BASE + LCD_STACK_OFFSET)

#define LCD_HEAP_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_HEAP_OFFSET)
#define LCD_HEAP_GV_ADDR __gva(LCD_VIRT_BASE + LCD_HEAP_OFFSET)

#define LCD_RAM_MAP_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_RAM_MAP_OFFSET)
#define LCD_RAM_MAP_GV_ADDR __gva(LCD_VIRT_BASE + LCD_RAM_MAP_OFFSET)

#define LCD_IOREMAP_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_IOREMAP_OFFSET)
#define LCD_IOREMAP_GV_ADDR __gva(LCD_VIRT_BASE + LCD_IOREMAP_OFFSET)

#define LCD_KERNEL_MODULE_REGION_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_KERNEL_MODULE_REGION_OFFSET)
#define LCD_KERNEL_MODULE_REGION_GV_ADDR \
	__gva(LCD_VIRT_BASE + LCD_KERNEL_MODULE_REGION_OFFSET)

/**
 * lcd_gva2gpa -- Convert a guest virtual to a guest physical address
 * @gva: the guest virtual address to convert
 *
 * For non-isolated code, this is equivalent to __pa.
 *
 * For isolated code, this is LCD_PHYS_BASE + (gva - LCD_VIRT_BASE)
 * Of course, this will give garbage results if gva is garbage.
 */
gpa_t lcd_gva2gpa(gva_t gva);
/**
 * lcd_gpa2gva -- Convert a guest physical to a guest virtual address
 * @gva: the guest physical address to convert
 *
 * For non-isolated code, this is equivalent to __va.
 *
 * For isolated code, this is LCD_VIRT_BASE + (gpa - LCD_PHYS_BASE)
 * Of course, this will give garbage results if gpa is garbage.
 */
gva_t lcd_gpa2gva(gpa_t gpa);


#endif /* LCD_DOMAINS_ADDRESS_SPACES_H */
