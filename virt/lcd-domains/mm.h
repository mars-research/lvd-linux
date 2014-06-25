/*
 * Simple lcd address space setup.
 *
 * Intra-lcd code is responsible for more
 * sophisticated memory management.
 */

#ifndef VIRT_LCD_DOMAINS_MM_H
#define VIRT_LCD_DOMAINS_MM_H

/*
 * (initial)
 * LCD Memory Layout
 * =================
 *
 * The layout below reflects the guest physical *and* virtual memory
 * layout. Guest physical addresses are mapped one-to-one to the same
 * guest virtual addresses.
 *
 * All allocated guest physical memory--including the arch-dependent region,
 * guest virtual page tables, and the lcd's code--is mapped in the guest
 * virtual address space before the lcd is entered.
 *
 *                         +---------------------------+
 *                         |                           |
 *                         :                           :
 *                         :      Free / Unmapped      :
 *                         +---------------------------+
 *                         |         LCD Code          | (variable, max 16 pgs)
 * LCD_CODE_START--------> +---------------------------+ 
 *                         |       Guest Virtual       | (4 MBs)
 *                         |       Paging Memory       |
 * LCD_ARCH_FREE,--------> +---------------------------+
 * LCD_PAGING_MEM_START    |                           |
 *                         :   Reserved Arch Memory    :
 *                         |                           |
 *                         +---------------------------+ 0x0000 0000 0000 0000
 */

/**
 * Maps 
 *
 *    gpa_start --> gpa_start + npages * PAGE_SIZE
 *
 * to
 *
 *    hpa_start --> hpa_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_mm_gpa_map_range(struct lcd *lcd, u64 gpa_start, u64 hpa_start, 
			u64 npages);

/**
 * Maps 
 *
 *    gva_start --> gva_start + npages * PAGE_SIZE
 *
 * to
 *
 *    gpa_start --> gpa_start + npages * PAGE_SIZE
 *
 * in lcd's guest virtual paging tables.
 *
 * Note! Host physical memory for guest virtual paging should be allocated 
 * and mapped in the lcd's guest physical address space before calling
 * this routine.
 */
int lcd_mm_gva_map_range(struct lcd *lcd, u64 gva_start, u64 gpa_start, 
			u64 npages);

#endif /* VIRT_LCD_DOMAINS_MM_H */
