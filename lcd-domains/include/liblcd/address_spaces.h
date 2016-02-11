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
 *   -- 256 GB for ioremap region (unmapped at boot)
 *
 *   -- 2 GB HOLE (unmapped)
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
 *              |         (246 GB)          |
 *              +---------------------------+ 0x0000 0042 0000 0000 (264 GB) 
 *              |       ioremap region      |                    
 *              |         (256 GB)          |
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

/* Sizes. The heap and ioremap are not mapped/backed at boot. */

#define LCD_UTCB_SIZE PAGE_SIZE /* ........................... 4  KBs */
#define LCD_BOOTSTRAP_PAGES_SIZE (2 * PAGE_SIZE) /* .......... 8  KBs */
#define LCD_BOOTSTRAP_PAGE_TABLES_SIZE (2 * PAGE_SIZE) /* .... 8  KBs */
#define LCD_STACK_SIZE (2 * PAGE_SIZE) /* .................... 8  KBs */
#define LCD_HEAP_SIZE (16UL << 20) /* ........................ 16 MBs */
#define LCD_IOREMAP_SIZE (16UL << 20) /* ..................... 16 MBs */

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
	(ilog(LCD_BOOTSTRAP_PAGES_SIZE >> PAGE_SHIFT))
#define LCD_BOOTSTRAP_PAGE_TABLES_ORDER \
	(ilog(LCD_BOOTSTRAP_PAGE_TABLES_SIZE >> PAGE_SHIFT))
#define LCD_STACK_ORDER \
	(ilog(LCD_STACK_SIZE >> PAGE_SHIFT))

/* Offsets. */

#define LCD_MISC_REGION_OFFSET (1UL << 30)
#define LCD_UTCB_OFFSET LCD_MISC_REGION_OFFSET
#define LCD_BOOTSTRAP_PAGES_OFFSET (LCD_UTCB_OFFSET + LCD_UTCB_SIZE)
#define LCD_BOOTSTRAP_PAGE_TABLES_OFFSET \
	(LCD_BOOTSTRAP_PAGES_OFFSET + LCD_BOOTSTRAP_PAGES_SIZE)

/* HOLE */

#define LCD_STACK_REGION_OFFSET (3UL << 30)
#define LCD_STACK_OFFSET \
	(LCD_STACK_REGION_OFFSET + (1UL << 30) - LCD_STACK_SIZE)

/* HOLE */

#define LCD_HEAP_REGION_OFFSET (5UL << 30)
#define LCD_HEAP_OFFSET LCD_HEAP_REGION_OFFSET

/* HOLE */

#define LCD_IOREMAP_REGION_OFFSET (8UL << 30)
#define LCD_IOREMAP_OFFSET LCD_IOREMAP_REGION_OFFSET
#define LCD_IOREMAP_REGION_SIZE (256UL << 30)

/* HOLE */

#define LCD_KERNEL_MODULE_REGION_OFFSET (510UL << 30)

/* Addresses */

#define LCD_PHYS_BASE (0UL)
#define LCD_VIRT_BASE (0xFFFFFF7FFFFFFFFFUL)

#define LCD_UTCB_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_UTCB_OFFSET)
#define LCD_UTCB_GV_ADDR __gva(LCD_VIRT_BASE + LCD_UTCB_OFFSET)

#define LCD_BOOTSTRAP_PAGES_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGES_OFFSET)
#define LCD_BOOTSTRAP_PAGES_GV_ADDR \
	__gpa(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGES_OFFSET)

#define LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET)
#define LCD_BOOTSTRAP_PAGE_TABLES_GV_ADDR \
	__gpa(LCD_VIRT_BASE + LCD_BOOTSTRAP_PAGE_TABLES_OFFSET)

#define LCD_STACK_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_STACK_OFFSET)
#define LCD_STACK_GV_ADDR __gpa(LCD_VIRT_BASE + LCD_STACK_OFFSET)

#define LCD_HEAP_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_HEAP_OFFSET)
#define LCD_HEAP_GV_ADDR __gpa(LCD_VIRT_BASE + LCD_HEAP_OFFSET)

#define LCD_IOREMAP_GP_ADDR __gpa(LCD_PHYS_BASE + LCD_IOREMAP_OFFSET)
#define LCD_IOREMAP_GV_ADDR __gpa(LCD_VIRT_BASE + LCD_IOREMAP_OFFSET)

#define LCD_KERNEL_MODULE_REGION_GP_ADDR \
	__gpa(LCD_PHYS_BASE + LCD_KERNEL_MODULE_REGION_OFFSET)
#define LCD_KERNEL_MODULE_REGION_GV_ADDR \
	__gpa(LCD_VIRT_BASE + LCD_KERNEL_MODULE_REGION_OFFSET)

#endif /* LCD_DOMAINS_ADDRESS_SPACES_H */
