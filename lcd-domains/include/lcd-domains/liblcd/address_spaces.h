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

#include <lcd-domains/types.h>

/*
 * Guest Physical Memory Layout
 * ============================
 *
 * No gdt/tss/idt for now (easier). See Documentation/lcd-domains/vmx.txt.
 *
 * From bottom to top,
 *
 *   -- The bottom 1 MB is unmapped / reserved in case the module is expecting 
 *      the standard physical memory layout of a PC. (Of course, it or its 
 *      creator would need to map something there to emulate that memory.) No
 *      memory mapped here for the gcc stack protector, so make sure you have
 *      that turned off when building the code for the lcd.
 *
 *   -- Guest virtual page tables come next, 4 MBs. This puts a (big) upper 
 *      limit on the size of the module that can be mapped. The page tables
 *      in the hierarchy are allocated on demand as the module is mapped.
 *
 *   -- The UTCB used by the initial thread when the lcd boots. (The
 *      microkernel manages this page.)
 *
 *   -- The stack
 *
 *   -- Heap. The module is put at the beginning of the heap.
 *
 *   -- A huge chunk of free/unmapped guest physical memory available to the
 *      module.
 *
 *   -- The upper part is unusable (see Intel SDM V3 28.2.2). The last
 *      usable byte is at 0x0000 FFFF FFFF FFFF.
 *
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                   |         Unusable          |
 *                   +---------------------------+ 0x0000 FFFF FFFF FFFF
 *                   |                           |
 *                   :           Free            :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0150 8000
 *                   |                           |
 *                   |           Heap            |
 *                   |      (module mapped       |
 *                   |       at the bottom)      |
 *                   :         (16 MBs)          :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 8000
 *                   |           Stack           |
 *                   |          (8 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 6000
 *                   |         Guard page        |
 *                   |        (not mapped)       |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 5000
 *                   |           UTCB            |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 4000
 *                   |        Boot info          |
 *                   |         (16 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 *
 * Guest Virtual Memory Layout
 * ===========================
 *
 * The lower part has the same layout as the guest physical.
 *
 * The module is mapped per the guest virtual addresses in the lcd_module_page
 * list returned from the module loader, so that relinking is unnecessary.
 * 
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *  The module       |                           |
 *  gets mapped      |        Upper Part         |
 *  somewhere in     :       (mostly free)       :
 *  here  -------->  |                           |
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0150 8000
 *                   |                           |
 *                   |           Heap            |
 *                   :         (16 MBs)          :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 8000
 *                   |           Stack           |
 *                   |          (8 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 6000
 *                   |         Guard page        |
 *                   |        (not mapped)       |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 5000
 *                   |           UTCB            |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 4000
 *                   |        Boot info          |
 *                   |         (16 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 */

#define LCD_BOOT_PAGES_ORDER 2
#define LCD_STACK_PAGES_ORDER 1

/* guest physical addresses */
#define LCD_GV_PAGING_MEM_GPA __gpa(1 << 20) /* low 1 MB is empty */
#define LCD_GV_PAGING_MEM_SIZE (4 << 20)
#define LCD_BOOT_PAGES_GPA gpa_add(LCD_GV_PAGING_MEM_GPA, \
					LCD_GV_PAGING_MEM_SIZE)
#define LCD_BOOT_PAGES_SIZE ((1 << LCD_BOOT_PAGES_ORDER) * (4 << 10))
#define LCD_UTCB_GPA gpa_add(LCD_BOOT_PAGES_GPA, LCD_BOOT_PAGES_SIZE)
#define LCD_UTCB_SIZE (4 << 10)
#define LCD_UTCB_STACK_GAP (4 << 10) /* guard page, not mapped */
#define LCD_STACK_GPA gpa_add(LCD_UTCB_GPA, LCD_UTCB_SIZE + LCD_UTCB_STACK_GAP)
#define LCD_STACK_SIZE ((1 << LCD_STACK_PAGES_ORDER) * (4 << 10))
#define LCD_MODULE_GPA gpa_add(LCD_STACK_GPA, LCD_STACK_SIZE)

/* guest virtual addresses */
#define LCD_GV_PAGING_MEM_GVA __gva(gpa_val(LCD_GV_PAGING_MEM_GPA))
#define LCD_BOOT_PAGES_GVA __gva(gpa_val(LCD_BOOT_PAGES_GPA))
#define LCD_UTCB_GVA __gva(gpa_val(LCD_UTCB_GPA))
#define LCD_STACK_GVA __gva(gpa_val(LCD_STACK_GPA))

#endif /* LCD_DOMAINS_ADDRESS_SPACES_H */
